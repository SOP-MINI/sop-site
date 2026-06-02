#!/bin/sh
set -eu

PORT="${1:-9000}"
HOST="${2:-127.0.0.1}"
NC_BIN="${NC_BIN:-nc}"

send_hex() {
    label="$1"
    hex="$2"
    printf '== %s ==\n' "$label"
    printf '%s' "$hex" | xxd -r -p | "$NC_BIN" -u -w1 "$HOST" "$PORT"
}

pad_hex() {
    python3 - "$1" "$2" <<'PY'
import sys
text = sys.argv[1].encode('ascii')
size = int(sys.argv[2])
if len(text) > size:
    raise SystemExit(f"field too long: {text!r}")
print((text + b'\0' * (size - len(text))).hex())
PY
}

u32be_hex() {
    python3 - "$1" <<'PY'
import struct
import sys
value = int(sys.argv[1], 0)
if not 0 <= value <= 0xffffffff:
    raise SystemExit(f"value out of range: {value}")
print(struct.pack('!I', value).hex())
PY
}

build_message_hex() {
    login="$1"
    cmd="$2"
    shift 2
    hex="$(pad_hex "$login" 16)$(pad_hex "$cmd" 8)"
    while [ "$#" -gt 0 ]; do
        hex="${hex}$(u32be_hex "$1")"
        shift
    done
    printf '%s' "$hex"
}

VALID_LOGIN="krasowskip"
INVALID_LOGIN="not-in-logins"

send_hex "valid RUN" "$(build_message_hex "$VALID_LOGIN" RUN)"
sleep 0.2
send_hex "valid COMPUTE with two jobs" "$(build_message_hex "$VALID_LOGIN" COMPUTE 5000 123 1200 456)"
sleep 0.2
send_hex "valid LIST" "$(build_message_hex "$VALID_LOGIN" LIST)"
sleep 0.2
send_hex "valid GATHER" "$(build_message_hex "$VALID_LOGIN" GATHER)"
sleep 0.2
send_hex "valid PAUSE" "$(build_message_hex "$VALID_LOGIN" PAUSE)"
sleep 0.2
send_hex "valid RUN after PAUSE" "$(build_message_hex "$VALID_LOGIN" RUN)"
sleep 0.2
send_hex "invalid unknown user" "$(build_message_hex "$INVALID_LOGIN" LIST)"
sleep 0.2
send_hex "invalid unknown command" "$(build_message_hex "$VALID_LOGIN" STATUS)"
sleep 0.2
send_hex "invalid wrong length for COMPUTE" "$(build_message_hex "$VALID_LOGIN" COMPUTE 10)"
sleep 0.2
send_hex "invalid too many samples" "$(build_message_hex "$VALID_LOGIN" COMPUTE 10000001 999)"
sleep 0.2
send_hex "invalid too short datagram" "0011223344556677"
sleep 0.2
send_hex "valid EXIT" "$(build_message_hex "$VALID_LOGIN" EXIT)"
