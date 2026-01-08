---
title: "Zdarzenia"
date: 2025-11-18T22:35:27+01:00
 weight: 55
---

# Programowanie zdarzeniowe

## Zakres materiału

- Aplikacje ograniczone przez wejście/wyjście
- Aplikacje ograniczone przez CPU
- Problem blokujących wywołań systemowych
- Busy waiting
- Programowanie oparte o zdarzenia
  - Odbieranie zdarzeń sygnałami
  - Odbieranie zdarzeń wywołaniami systemowymi
- Interfesy generujące zdarzenia:
  - `timer_create`
  - `timerfd_create`
  - `signalfd`
- Detekcja zmian
  - Problem ABA
- Zdarzenia systemu plików
  - `inotify` API
- Asynchroniczne I/O
  - `io_uring` API (uwaga: zaawansowany temat dodatkowy)
- Multipleksery zdarzeń
  - `select()` API
  - pętle zdarzeń

## Materiały

1. [Prezentacja wykładowa](https://link.excalidraw.com/p/readonly/iDk9bAu6v23YGNE9iXHs) ([PDF]({{< resource OPS1_EventIO.pdf >}}))
2. [Programy wykładowe](https://gitlab.com/SaQQ/sop1/-/tree/master/05_events?ref_type=heads)
