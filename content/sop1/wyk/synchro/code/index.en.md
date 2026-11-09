---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Synchronization

```shell
# Fixes tsan unexpected memory mapping issues
sudo sysctl vm.mmap_rnd_bits=28
```

```shell
make 01_semaphore_base
./01_semaphore_base
```
Source: [01_semaphore_base.c]({{< github_url "01_semaphore_base.c" >}})

```shell
make 07_semaphore
./07_semaphore
```
Source: [07_semaphore.c]({{< github_url "07_semaphore.c" >}})

```shell
make 11_sem_open
./11_sem_open
```
Source: [11_sem_open.c]({{< github_url "11_sem_open.c" >}})

```shell
make 14_cv_base
./14_cv_base
```
Source: [14_cv_base.c]({{< github_url "14_cv_base.c" >}})

```shell
#gcc -o 15_cv -g 15_cv.c -pthread -fsanitize=thread
make 15_cv
./15_cv
```
Source: [15_cv.c]({{< github_url "15_cv.c" >}})

```shell
#gcc -o 20_barrier -g 20_barrier.c -pthread -fsanitize=thread
make 20_barrier
./20_barrier
```
Source: [20_barrier.c]({{< github_url "20_barrier.c" >}})
