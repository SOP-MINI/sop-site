---
title: "Sygnały"
date: 2022-02-05T18:02:32+01:00
weight: 50
---

# Sygnały POSIX

## Zakres wykładu

  - Koncepcja sygnału POSIX.
  - Przyczyny sygnałów: wyjątki sprzętowe, czynności wykonywane przez procesy
  - Cele sygnałów: określony wątek, proces lub grupa procesów.
  - Najczęściej wykorzystywane sygnały UNIX.
  - Reakcje na sygnały:
      - doręczenie (*signal delivery*):
          - ignorowanie sygnału
          - wykonanie funkcji obsługi zdefiniowanej przez użytkownika.
          - wykonanie czynności domyślnej (exit, core dump+exit, continue, stop)
      - przyjęcie (*signal acceptance*: `sigwait()`
  - Programowa generacja sygnału: `kill()`, `alarm()`,...
  - Ustanawianie funkcji obsługi zdefiniowanej przez użytkownika: `sigaction()`
  - Koncepcja maski sygnałów i blokowanie sygnałów: `sigprocmask()`, `sigpending()`
  - Reakcja na wielokrotne wystąpienie sygnałów.
  - Atrybuty `volatile sig_atomic_t` a obsługa sygnałów.
  - Efekty uboczne doręczania sygnałów:
      - przedwczesne kończenie "długich" funkcji systemowych (z `errno==EINTR`) ; makro `TEMP_FAILURE_RETRY` .
      - nieprzewidywalne zachowanie funkcji ze stanem (*non-reentrant*); funkcje odporne na asynchroniczną obsługę sygnałów (`async-safe functions`).
  - Oczekiwanie na sygnały: `pause()`, `sigsuspend()`, `sigwait()`.
  - Sygnały generowane przez terminal. `stty`.

## Materiały

1. [Prezentacja wykładowa](https://link.excalidraw.com/p/readonly/awkHYH5frIwYCA09WJLg)
2. [Programy wykładowe](https://gitlab.com/SaQQ/sop1/-/tree/master/04_signals?ref_type=heads)
3. Dodatkowa lektura: The GNU C library documentation: [Signal Handling (24.1-24.8)](https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html#Signal-Handling)
4. Stare Slajdy: [POSIX\_signals.pdf]({{< resource "POSIX_signals_7.pdf" >}})
