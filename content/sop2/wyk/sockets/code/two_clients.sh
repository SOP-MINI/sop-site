#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

ACTION="${1:-up}"

setup() {
    echo "[setup] Starting environment configuration (two_clients)..."

    echo "[setup] Creating namespaces (ns_server, ns_client1, ns_client2)..."
    ip netns add ns_server
    ip netns add ns_client1
    ip netns add ns_client2

    echo "[setup] Creating virtual links (veth_srv_c1 <-> veth_c1, veth_srv_c2 <-> veth_c2)..."
    ip link add veth_srv_c1 type veth peer name veth_c1
    ip link add veth_srv_c2 type veth peer name veth_c2

    echo "[setup] Assigning interfaces to respective namespaces..."
    ip link set veth_srv_c1 netns ns_server
    ip link set veth_srv_c2 netns ns_server
    ip link set veth_c1 netns ns_client1
    ip link set veth_c2 netns ns_client2

    echo "[setup] Configuring interfaces in ns_server..."
    ip netns exec ns_server ip addr add 10.0.1.1/24 dev veth_srv_c1
    ip netns exec ns_server ip addr add 10.0.2.1/24 dev veth_srv_c2
    ip netns exec ns_server ip link set veth_srv_c1 up
    ip netns exec ns_server ip link set veth_srv_c2 up
    ip netns exec ns_server ip link set lo up

    # Enable IPv4 forwarding on the server (so packets can route between c1 and c2 if needed)
    echo "[setup] Enabling IPv4 forwarding on ns_server..."
    ip netns exec ns_server sysctl -q -w net.ipv4.ip_forward=1

    echo "[setup] Configuring interfaces in ns_client1 (LAN simulation)..."
    ip netns exec ns_client1 ip addr add 10.0.1.2/24 dev veth_c1
    ip netns exec ns_client1 ip link set veth_c1 up
    ip netns exec ns_client1 ip link set lo up
    # Set default gateway to ns_server
    ip netns exec ns_client1 ip route add default via 10.0.1.1

    echo "[setup] Configuring interfaces in ns_client2 (WAN simulation)..."
    ip netns exec ns_client2 ip addr add 10.0.2.2/24 dev veth_c2
    ip netns exec ns_client2 ip link set veth_c2 up
    ip netns exec ns_client2 ip link set lo up
    # Set default gateway to ns_server
    ip netns exec ns_client2 ip route add default via 10.0.2.1

    echo "[setup] Setup successful. Environment is ready."
}

cleanup() {
    echo "[cleanup] Starting environment cleanup..."

    # Remove namespaces (this automatically deletes the associated veth interfaces)
    echo "[cleanup] Removing namespaces..."
    ip netns del ns_server || true
    ip netns del ns_client1 || true
    ip netns del ns_client2 || true

    echo "[cleanup] Environment cleaned up successfully."
}

case "$ACTION" in
    up)
        setup
        ;;
    down)
        cleanup
        ;;
    *)
        echo "Invalid argument."
        echo "Usage: $0 {up|down}"
        exit 1
        ;;
esac