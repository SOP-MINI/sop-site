#!/usr/bin/env python3
import socket
import argparse
import random
import string

def send_udp(ip, port, size):
    chars = string.ascii_letters + string.digits
    payload = ''.join(random.choices(chars, k=size))

    print(f"[*] Sending {size} bytes to {ip}:{port}...")

    try:
        # Create UDP socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(payload.encode('utf-8'), (ip, port))
        print("[+] UDP packet sent successfully.")
    except Exception as e:
        print(f"[-] Error: {e}")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="UDP payload generator")
    parser.add_argument('-d', '--dst', required=True, help="Destination IP")
    parser.add_argument('-p', '--port', type=int, default=9999, help="Destination UDP port")
    parser.add_argument('-s', '--size', type=int, default=64, help="Payload size in bytes")

    args = parser.parse_args()
    send_udp(args.dst, args.port, args.size)
