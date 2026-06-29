#!/bin/bash
# Demonstrates running the thrashing program within a constrained cgroup
# using systemd-run (doesn't require root if user delegation is enabled).

echo "Starting thrashing program (100MB allocation) within a 50MB cgroup limit..."
echo "This will cause local thrashing, but the rest of the OS will remain responsive."
echo ""
echo "Press Ctrl+C to stop."
echo ""

# We limit memory to 50M. The thrashing binary allocates 100M.
systemd-run --user --scope -p MemoryMax=50M ./thrashing 100
