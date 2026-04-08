#!/bin/bash

set -e

ACTION="${1:-up}"

setup() {
    echo "[setup] Checking for existing namespaces..."
    for ns in ns_client ns_server; do
        if ip netns list | grep -q "^${ns}\b"; then
            echo "[setup] Namespace ${ns} already exists; refusing to set up."
            exit 1
        fi
    done

    echo "[setup] Creating namespace ns_client..."
    ip netns add ns_client

    echo "[setup] Creating namespace ns_server..."
    ip netns add ns_server

    echo "[setup] Creating veth pair veth_c <-> veth_s..."
    ip link add veth_c type veth peer name veth_s

    echo "[setup] Moving veth_c into ns_client..."
    ip link set veth_c netns ns_client

    echo "[setup] Moving veth_s into ns_server..."
    ip link set veth_s netns ns_server

    echo "[setup] Configuring ns_client interface..."
    ip netns exec ns_client ip addr add 10.0.0.1/24 dev veth_c
    ip netns exec ns_client ip link set veth_c up
    ip netns exec ns_client ip link set lo up

    echo "[setup] Configuring ns_server interface..."
    ip netns exec ns_server ip addr add 10.0.0.2/24 dev veth_s
    ip netns exec ns_server ip link set veth_s up
    ip netns exec ns_server ip link set lo up

    echo "[setup] Completed successfully."
}

cleanup() {
    echo "[cleanup] Removing namespaces..."
    echo "[cleanup] Deleting namespace ns_client..."
    ip netns del ns_client || true
    echo "[cleanup] Deleting namespace ns_server..."
    ip netns del ns_server || true
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
