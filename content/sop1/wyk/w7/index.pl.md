---
title: "Synchronizacja"
date: 2022-02-03T19:09:45+01:00
weight: 70
---

# Synchronizacja POSIX

## Zakres wykładu

- Semafory POSIX
    - Trwałość, nazywanie, lokalizacja w pamięci semaforów nazwanych oraz nienazwanych.
    - Tworzenie i inicjacja, otwieranie/zamykanie dostępu i usuwanie nazwanych semaforów. `sem_open()`, `sem_close()`, `sem_unlink()`.
    - Tworzenie i inicjacja oraz usuwanie nienazwanych semaforów. Uwaga: przy synchronizacji wątków różnych procesów konieczne jest użycie *współdzielonej* struktury semaforowej.
    - Blokujące i nieblokujące operacje **wait**: `sem_wait()`, `sem_trywait()`.
    - Operacja **post**: `sem_post()`. Pobieranie wartości semafora: `sem_ getvalue()`
- Inne obiekty synchronizacji POSIX
    - Zmienna warunku. Wykorzystanie pary: zmienna warunku - muteks do synchronizacji.
    - Bariera. Zasada działania.
    - Zamek czytelników-pisarzy (rygle odczytu-zapisu).

## Materiały

1. [Slajdy wykładowe](https://link.excalidraw.com/p/readonly/75Wgi5dzsPNMh1fi5wYy)
2. [Programy wykładowe](https://gitlab.com/SaQQ/sop1/-/tree/master/07_synchro?ref_type=heads)
3. Rozdz. 27.1 w dokumentacji *the GNU C library*: [Semaphores](https://www.gnu.org/software/libc/manual/html_node/Semaphores.html#Semaphores)
4. Literatura uzupełniająca: W.R. Stevens, Unix programowanie usług sieciowych, t. 2, wyd. 2: Komunikacja międzyprocesowa
   Rozdziały: 7 (Muteksy i zmienne warunku), 8 (Rygle odczytu zapisu), 10 (Semafory posiksowe), 11 (Semafory Systemu V).
5. Stare Slajdy: [IPC\_2.pdf]({{< resource "IPC_2_7.pdf" >}})

