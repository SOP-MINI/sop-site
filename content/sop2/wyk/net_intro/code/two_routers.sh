#!/bin/bash

set -e

ACTION="${1:-up}"

NAMESPACES=("ns_h1" "ns_r1" "ns_sw" "ns_h2" "ns_r2" "ns_h3")
BRIDGE="br_mid"

setup() {
    echo "[setup] Checking for existing namespaces..."
    for ns in "${NAMESPACES[@]}"; do
        if ip netns list | grep -q "^${ns}\b"; then
            echo "[setup] Namespace ${ns} already exists; refusing to set up."
            exit 1
        fi
    done

    for ns in "${NAMESPACES[@]}"; do
        echo "[setup] Creating namespace ${ns}..."
        ip netns add ${ns}
        ip netns exec ${ns} ip link set lo up
    done

    echo "[setup] Creating bridge ${BRIDGE} inside ns_sw..."
    ip netns exec ns_sw ip link add ${BRIDGE} type bridge
    ip netns exec ns_sw ip link set ${BRIDGE} up
    echo "[setup] Bridge ${BRIDGE} is up and isolated."

    # ==========================================
    # Subnet 1: ns_h1 <---> ns_r1 (10.0.1.0/24)
    # ==========================================
    echo "[setup] Wiring Subnet 1 (Host 1 <-> Router 1)..."
    ip link add veth_h1 type veth peer name veth_r1_l
    ip link set veth_h1 netns ns_h1
    ip link set veth_r1_l netns ns_r1

    ip netns exec ns_h1 ip link set dev veth_h1 address 02:00:01:00:00:01
    ip netns exec ns_h1 ip addr add 10.0.1.1/24 dev veth_h1
    ip netns exec ns_h1 ip link set veth_h1 up
    ip netns exec ns_h1 ip route add default via 10.0.1.254

    ip netns exec ns_r1 ip link set dev veth_r1_l address 02:00:01:00:00:fe
    ip netns exec ns_r1 ip addr add 10.0.1.254/24 dev veth_r1_l
    ip netns exec ns_r1 ip link set veth_r1_l up

    # ==========================================
    # Subnet 2: ns_r1 <-> ns_sw <-> ns_h2 <-> ns_sw <-> ns_r2 (10.0.2.0/24)
    # ==========================================
    echo "[setup] Wiring Subnet 2 (Router 1 <-> Switch <-> Host 2 <-> Switch <-> Router 2)..."

    # R1 to ns_sw
    ip link add veth_r1_r type veth peer name veth_r1_br
    ip link set veth_r1_r netns ns_r1
    ip link set veth_r1_br netns ns_sw  # Wrzucamy końcówkę do switcha!

    ip netns exec ns_sw ip link set veth_r1_br master ${BRIDGE}
    ip netns exec ns_sw ip link set veth_r1_br up

    ip netns exec ns_r1 ip link set dev veth_r1_r address 02:00:02:00:00:fd
    ip netns exec ns_r1 ip addr add 10.0.2.253/24 dev veth_r1_r
    ip netns exec ns_r1 ip link set veth_r1_r up
    ip netns exec ns_r1 sysctl -w net.ipv4.ip_forward=1
    ip netns exec ns_r1 ip route add 10.0.3.0/24 via 10.0.2.254

    # H2 to ns_sw
    ip link add veth_h2 type veth peer name veth_h2_br
    ip link set veth_h2 netns ns_h2
    ip link set veth_h2_br netns ns_sw  # Wrzucamy końcówkę do switcha!

    ip netns exec ns_sw ip link set veth_h2_br master ${BRIDGE}
    ip netns exec ns_sw ip link set veth_h2_br up

    ip netns exec ns_h2 ip link set dev veth_h2 address 02:00:02:00:00:01
    ip netns exec ns_h2 ip addr add 10.0.2.1/24 dev veth_h2
    ip netns exec ns_h2 ip link set veth_h2 up
    ip netns exec ns_h2 ip route add 10.0.1.0/24 via 10.0.2.253
    ip netns exec ns_h2 ip route add 10.0.3.0/24 via 10.0.2.254

    # R2 to ns_sw
    ip link add veth_r2_l type veth peer name veth_r2_br
    ip link set veth_r2_l netns ns_r2
    ip link set veth_r2_br netns ns_sw  # Wrzucamy końcówkę do switcha!

    ip netns exec ns_sw ip link set veth_r2_br master ${BRIDGE}
    ip netns exec ns_sw ip link set veth_r2_br up

    ip netns exec ns_r2 ip link set dev veth_r2_l address 02:00:02:00:00:fe
    ip netns exec ns_r2 ip addr add 10.0.2.254/24 dev veth_r2_l
    ip netns exec ns_r2 ip link set veth_r2_l up
    ip netns exec ns_r2 sysctl -w net.ipv4.ip_forward=1
    ip netns exec ns_r2 ip route add 10.0.1.0/24 via 10.0.2.253

    # ==========================================
    # Subnet 3: ns_r2 <---> ns_h3 (10.0.3.0/24)
    # ==========================================
    echo "[setup] Wiring Subnet 3 (Router 2 <-> Host 3)..."
    ip link add veth_r2_r type veth peer name veth_h3
    ip link set veth_r2_r netns ns_r2
    ip link set veth_h3 netns ns_h3

    ip netns exec ns_r2 ip link set dev veth_r2_r address 02:00:03:00:00:fe
    ip netns exec ns_r2 ip addr add 10.0.3.254/24 dev veth_r2_r
    ip netns exec ns_r2 ip link set veth_r2_r up

    ip netns exec ns_h3 ip link set dev veth_h3 address 02:00:03:00:00:01
    ip netns exec ns_h3 ip addr add 10.0.3.1/24 dev veth_h3
    ip netns exec ns_h3 ip link set veth_h3 up
    ip netns exec ns_h3 ip route add default via 10.0.3.254

    echo "[setup] Completed successfully."
}

cleanup() {
    echo "[cleanup] Removing namespaces (this automatically destroys the bridge and all veth pairs)..."
    for ns in "${NAMESPACES[@]}"; do
        echo "[cleanup] Deleting namespace ${ns}..."
        ip netns del ${ns} || true
    done

    echo "[cleanup] Completed successfully."
}

case "$ACTION" in
    up)
        setup
        ;;
    down)
        cleanup
        ;;
    *)
        echo "Usage: $0 {up|down}"
        exit 1
        ;;
esac