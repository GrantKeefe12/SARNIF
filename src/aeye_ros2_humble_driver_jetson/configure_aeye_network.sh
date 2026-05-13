#!/bin/bash

set -euo pipefail

NIC="${1:-enP8p1s0}"
MTU="${2:-9000}"
RBUF="${3:-52428800}"

if [[ $EUID -ne 0 ]]; then
  echo "Run this script with sudo: sudo $0 [nic] [mtu] [rmem]" >&2
  exit 1
fi

echo "Configuring AEye network on ${NIC}"
sysctl -w net.core.rmem_max="${RBUF}"
sysctl -w net.core.rmem_default="${RBUF}"
ip link set dev "${NIC}" mtu "${MTU}"

echo
echo "Applied settings:"
sysctl net.core.rmem_max net.core.rmem_default
ip link show "${NIC}" | sed -n '1p'