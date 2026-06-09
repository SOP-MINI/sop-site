---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Lazy allocation

```shell
make large_mmap
./large_mmap
```

Monitor memory usage:

```shell
watch -n 0.2 'ps -o pid,vsz,rss,comm -p $(pidof large_mmap)'
```

Monitor page fault events:

```bash
sudo perf stat -p $(pidof large_mmap) -e page-faults -I 1000
```

Observe perf counter going up with each page fault and RSS growing.

Follow-up: allow huge page mapping.

### PF cost

```shell
make pf_cost
```

Run few times:
```shell
./pf_cost
```

Observe large timing discrepancy between 1st and 2nd passes.
Follow-up: allow huge page mapping.

### Memory overcommit

Inspect current memory state:

```shell
LANG=en watch -n0.2 free -h
```

Disable swap if you have one:

```shell
sudo swapoff -a
```

Then try to allocate something:

```shell
make alloc
```

```shell
./alloc 100
```

Obviously large overallocations are disallowed.

Try however to allocate slightly more than available:

```shell
./alloc 30
```

Inspect OOM killer events:

```shell
journalctl -k | grep -i "Out of memory"
```

