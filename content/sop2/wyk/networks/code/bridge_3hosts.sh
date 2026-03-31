#!/bin/bash

set -e

ACTION="${1:-up}"

setup() {
    echo "[setup] Checking for existing namespaces..."
    # Fail if any namespace already exists
    for n in 1 2 3; do
        if ip netns list | grep -q "^ns_br3_$n\b"; then
            echo "[setup] Namespace ns_br3_$n already exists; refusing to set up."
            exit 1
        fi
    done

    echo "[setup] Creating bridge br0..."
    ip link add br0 type bridge
    ip link set br0 up
    echo "[setup] Bridge br0 is up."

    # Create 3 namespaces connected via bridge br0
    for n in 1 2 3; do
        echo "[setup] Creating namespace ns_br3_$n..."
        ip netns add ns_br3_$n

        echo "[setup] Creating veth pair veth$n <-> veth$n-br..."
        ip link add veth$n type veth peer name veth$n-br

        echo "[setup] Moving veth$n into ns_br3_$n..."
        ip link set veth$n netns ns_br3_$n

        echo "[setup] Attaching veth$n-br to br0..."
        ip link set veth$n-br master br0
        ip link set veth$n-br up
        echo "[setup] veth$n-br is up."

        echo "[setup] Configuring ns_br3_$n network interface..."
        ip netns exec ns_br3_$n ip link set dev veth$n address 02:00:00:00:00:0$n
        ip netns exec ns_br3_$n ip addr add 10.0.0.$n/24 dev veth$n
        ip netns exec ns_br3_$n ip link set veth$n up
        ip netns exec ns_br3_$n ip link set lo up
        echo "[setup] Namespace ns_br3_$n is ready."
    done

    echo "[setup] Completed successfully."
}

cleanup() {
    echo "[cleanup] Removing bridge br0..."
    # Remove bridge if it exists
    ip link set br0 down || true
    ip link del br0 || true

    echo "[cleanup] Removing namespaces..."
    # Remove namespaces first
    for n in 1 2 3; do
        echo "[cleanup] Deleting namespace ns_br3_$n..."
        ip netns del ns_br3_$n || true
        echo "[cleanup] Deleting interface veth$n..."
        ip link del veth$n || true
        ip link del veth$n-br || true
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
