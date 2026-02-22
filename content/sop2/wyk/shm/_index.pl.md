---
title: "Pamięć wspólna"
date: 2022-02-10T10:00:00+01:00
weight: 10
---

# Pamięć wspólna

## Zakres wykładu

- Mapowanie plików do pamięci procesu (`mmap()`, `munmap()`).
- Widoczność mapowania (`MAP_SHARED`, `MAP_PRIVATE`, `MAP_ANONYMOUS`).
- Ochrona zmapowanej pamięci (`PROT_READ`, `PROT_WRITE`, `PROT_EXEC`, `PROT_NONE`).
- Komunikacja międzyprocesowa (IPC) przez mapowanie plików.
- Synchronizacja pamięci z nośnikiem (`msync()`, `MS_SYNC`, `MS_ASYNC`).
- Struktura przestrzeni adresowej procesu.
- Debugowanie mapowań w Linuksie (`/proc/<pid>/maps`).
- Zachowanie pamięci zmapowanej po wywołaniu `fork()`.
- Cykl życia obiektów pamięci współdzielonej POSIX (`shm_open()`, `ftruncate()`, `shm_unlink()`).
- Przestrzeń nazw SHM (`/dev/shm/`).
- Mapowania anonimowe do szybkiego IPC między procesami pokrewnymi.

## Materiały

1. [Slajdy wykładowe]({{< ref path="slides" lang="en" >}})
2. Litera uzupełniająca: W.R. Stevens, Unix programowanie usług sieciowych, t. 2, wyd. 2: Komunikacja międzyprocesowa
   Rozdziały:
    - 12 (Podstawy na temat pamięci wspólnej)
    - 13 (POSIX'owa pamięć wspólna)
    - 14 (Pamięć wspólna Systemu V)
