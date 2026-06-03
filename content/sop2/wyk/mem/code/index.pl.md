---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Physical Address Space

```bash
sudo cat /proc/iomem
```

### Page Tables

```shell
make stackprint
./stackprint
```

```shell
cat /proc/$(cat stackprint.pid)/maps
```

```shell
VADDR="$(cat stackprint.addr)"
VADDR_HEX=${VADDR#0x}
VADDR=$((16#$VADDR_HEX))
PAGE_SIZE=4096

VPN=$(( VADDR / PAGE_SIZE ))
echo "VPN=${VPN}"

ENTRY=$(sudo dd if=/proc/$(pidof stackprint)/pagemap bs=8 count=1 skip=$VPN 2>/dev/null | od -An -t u8 | awk '{print $1}')
echo "ENTRY=${ENTRY}"
printf "ENTRY=%x\n" "$ENTRY"

PRESENT=$(( (ENTRY >> 63) & 1 ))
SWAPPED=$(( (ENTRY >> 62) & 1 ))
PFN_MASK=$(( (1 << 55) - 1 ))
PFN=$(( ENTRY & PFN_MASK ))

echo "PRESENT=$PRESENT"
echo "SWAPPED=$SWAPPED"

if [ "$PRESENT" -eq 1 ]; then
    echo "PFN=$PFN"
    printf "PFN=0x%x\n" "$PFN"
fi
```

