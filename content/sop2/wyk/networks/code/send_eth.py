#!/usr/bin/env python3
import socket
import binascii
import argparse

def send_frame(interface, src_mac, dst_mac, payload_text):
    clean_src = src_mac.replace(':', '').replace('-', '')
    clean_dst = dst_mac.replace(':', '').replace('-', '')

    try:
        src = binascii.unhexlify(clean_src)
        dst = binascii.unhexlify(clean_dst)
    except binascii.Error:
        print("Error: Invalid MAC address format. Use XX:XX:XX:XX:XX:XX format.")
        return

    try:
        s = socket.socket(socket.AF_PACKET, socket.SOCK_RAW)
        s.bind((interface, 0))
    except PermissionError:
        print("Error: Permission denied. Run as root or use sudo.")
        return

    ethertype = b'\x08\x00' # 0x0800 = IPv4
    payload = payload_text.encode('utf-8')
    frame = dst + src + ethertype + payload

    s.send(frame)
    print(f"[+] Sent frame ({len(frame)} bytes) through {interface}")
    print(f"    src: {src_mac} -> dst: {dst_mac}")
    print(f"    payload: '{payload_text}'")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Tool sending raw ethernet frames (L2)")
    parser.add_argument('-i', '--interface', required=True, help="Outbound interface (i.e. veth1)")
    parser.add_argument('-s', '--src', required=True, help="Source MAC")
    parser.add_argument('-d', '--dst', required=True, help="Destination MAC")
    parser.add_argument('-p', '--payload', required=True, help="Frame contents (payload)")

    args = parser.parse_args()
    send_frame(args.interface, args.src, args.dst, args.payload)