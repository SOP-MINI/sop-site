---
title: "Synchronizacja POSIX"
date: 2022-02-03T19:09:45+01:00
weight: 40
---

# Wykład 4 - Synchronizacja POSIX

## Zakres wykładu

  - Interfejs IPC Unix System V:
      - Trwałość, przestrzeń nazw.
      - Oryginalne cechy: zestaw semaforów, rozszerzona lista operacji (zwiększanie/zmniejszanie o wartość całkowitą, czekanie na wartość zero, atomowa realizacja zestawu operacji, cofanie efektów operacji po zakończeniu procesu)
      - Podstawowe wykorzystanie rozszerzeń
      - (\*) Budowa i wykorzystanie interfejsu zestawu semaforów.
          - Dane związane z każdym semaforem: `semval, sempid,            semncnt, semzcnt, semadj`. Ważne struktury danych: `struct sembuf`, `union semun`.
          - Tworzenie i inicjacja, otwarcie dostępu, usuwanie zestawów semaforów: `semget()`, `semctl()`.
          - Podstawowe i rozszerzone operacje semaforowe: funkcja `semop()`.
  - (\*\*) Semafory POSIX
      - Trwałość, nazywanie, lokalizacja w pamięci semaforów nazwanych oraz nienazwanych.
      - Tworzenie i inicjacja, otwieranie/zamykanie dostępu i usuwanie nazwanych semaforów. `sem_open()`, `sem_close()`, `sem_unlink()`.
      - Tworzenie i inicjacja oraz usuwanie nienazwanych semaforów. Uwaga: przy synchronizacji wątków różnych procesów konieczne jest użycie *współdzielonej* struktury semaforowej.
      - Blokujące i nieblokujące operacje **wait**: `sem_wait()`, `sem_trywait()`.
      - Operacja **post**: `sem_post()`. Pobieranie wartości semafora: `sem_ getvalue()`
  - Inne obiekty synchronizacji POSIX
      - (\*\*) Muteks (zamek). Własności. Podstawowy schematy wykorzystania.
      - (\*\*) Zmienna warunku. Wykorzystanie pary: zmienna warunku - muteks do synchronizacji.
      - Bariera. Zasada działania.
      - Zamek czytelników-pisarzy (rygle odczytu-zapisu).

### Uwagi

  - Treści oznaczone (\*) należy traktować jako pomocnicze, ułatwiające zrozumienie pozostałych treści. Znajomość tego materiału (w szczególności wywołań funkcji interfejsu `UNIX System V IPC` oraz systemowych struktur danych) nie jest wymagana w czasie kolokwiów wykładowych.
  - Treści oznaczone (\*\*) są bezpośrednio związane z zajęciami laboratoryjnymi. Znajomość szczegółow interfejsu `POSIX IPC` jest przydatna w czasie laboratoriów, może być przydatna dla zrozumienia ogólnych koncepcji tego wykładu, ale nie jest wymagana w czasie kolokwiów wykładowych.
  - Kolokwia wykładowe oczekują od studenta znajomości ogólnych koncepcji oraz podstawowych własności omawianych interfejsów IPC.

## Materiały

1.  Slajdy: [IPC\_2.pdf]({{< resource "IPC_2_7.pdf" >}})
2.  Rozdz. 27.1 w dokumentacji *the GNU C library*: [Semaphores](https://www.gnu.org/software/libc/manual/html_node/Semaphores.html#Semaphores)
3.  Literatura uzupełniająca: W.R. Stevens, Unix programowanie usług sieciowych, t. 2, wyd. 2: Komunikacja międzyprocesowa
    Rozdziały: 7 (Muteksy i zmienne warunku), 8 (Rygle odczytu zapisu), 10 (Semafory posiksowe), 11 (Semafory Systemu V).

