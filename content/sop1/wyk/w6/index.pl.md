---
title: "Wątki"
date: 2022-02-05T18:02:35+01:00
weight: 60
---

# Wykład 6 - Wątki, P-wątki i muteksy

## Zakres wykładu

  - Koncepcja wątków. Procesy jedno- i wielowątkowe.
  - Zalety wielowątkowości. Skalowanie. Prawo Amdahla.
  - Problemy programowania dla procesorów wielordzeniowych.
  - Wątki poziomu jądra i użytkownika.
  - Modele wielowątkowości
  - Biblioteki wątków: POSIX threads, Windows threads. Co-routines, fibers
  - Problemy wielowątkowości: fork/exec, obsługa sygnałów, anulowanie (cancellation), lokalne zmienne wątków (thread-local storage), interakcja z planistą (scheduler activations)
  - Wątki POSIX (P-wątki):
      - tworzenie wątków (`pthread_create()`) i ich kończenie (`pthread_exit(), exit(), pthread_cancel()`, powrót z funkcji `main()`)
      - atrybuty wątków
      - przekazywanie parametrów do i z funkcji roboczej wątku.
      - oczekiwanie na dołączalny wątek (`pthread_join()`), odłączanie wątku (`pthread_detach`)
      - synchronizacja: motywacja, użycie `mutex` do ochrony (w sekcji krytycznej) współdzielonej danej przed współbieżnym dostępem.
      - Funkcje wielowejściowe (re-entrant) i odporne na wątki (thread safe)
      - Sygnały i wątki; wątkowe maski blokowania sygnałów i ich wykorzystanie.

## Materiały

1.  Slajdy: [Threads.pdf]({{< resource "Threads_4.pdf" >}}), [POSIX\_threads.pdf]({{< resource "POSIX_threads_10.pdf" >}})
2.  Dodatkowa lektura: The GNU C library documentation: [1.2.2.1 POSIX Safety Concepts](https://www.gnu.org/software/libc/manual/html_node/POSIX-Safety-Concepts.html#POSIX-Safety-Concepts), [12.5 Streams and Threads](https://www.gnu.org/software/libc/manual/html_node/Streams-and-Threads.html#Streams-and-Threads)
3.  Przykładowy kod omawiany na wykładzie: [pth1.c]({{< resource "pth1.c" >}}), [pth2.c]({{< resource "pth2.c" >}}), [pth3.c]({{< resource "pth3.c" >}}), [sighand.c]({{< resource "sighand.c" >}})
4.  [Fragment]({{< resource "IEEE_Std_1003.1_2008_Thread_cancellation.pdf" >}}) Standardu Posix dotyczący kasowania (anulowania) wątków (thread cancellation)
