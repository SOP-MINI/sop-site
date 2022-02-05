---
title: "W4. Niskopoziomowe operacje wejścia/wyjścia."
date: 2022-02-05T18:02:29+01:00
draft: false
---

## Zakres wykładu

Niskopoziomowe (oparte o deskryptory) wejście-wyjście w systemach POSIX

  - Otwieranie i zamykanie sesji plikowej: `open()`, `close()`
  - Atrybuty plików w systemie UNIX i funkcje odpytywania: `stat(), lstat(),fstat()`
  - Funkcje odczytu/zapisu: `read(), write()`
  - Swobodny dostęp: `lseek()`
  - Duplikowanie deskryptorów: `dup(), dup2()`
  - Zmiany w tablicach deskryptorów, otwartych plików i i-węzłów związane z wywołaniem `open(), dup()/dup2(), fork()`
  - Badanie aktywności deskryptorów: `select()`
  - Związki pomiędzy strumieniami, a deskryptorami: `fdopen(), fileno()`
  - Synchronizacja deskryptorów: `fsync(), sync()`

## Materiały

1.  Slajdy: [IO\_2.pdf]({{< resource "IO_2_3.pdf" >}})
2.  Dodatkowa lektura: The GNU C library documentation: [Low-Level Input/Output (13.1-13.5,13.8)](https://www.gnu.org/software/libc/manual/html_node/#toc-Low_002dLevel-Input_002fOutput)
3.  Przykładowy kod omawiany na wykładzie: [list.c]({{< resource "list.c" >}}), [read\_dup.c]({{< resource "read_dup.c" >}}), [read\_fork.c]({{< resource "read_fork.c" >}}), [read2.c]({{< resource "read2.c" >}}), [unlnk.c]({{< resource "unlnk.c" >}})
