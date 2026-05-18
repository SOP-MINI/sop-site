---
title: "Serwery"
weight: 70
bookCollapseSection: true
---

# Serwery

## Zakres wykładu

- Serwery iteracyjne
- De-multipleksery zdarzeń: `select()`/`poll()`/`epoll()`
- Architektura "wątek na połączenie"
- Problem C10k
- Metryki jakości: QPS, EPS, latencja
- Serwery współbieżne oparte o pętlę zdarzeń
- Skalowanie pętli zdarzeń
  - wpółdzielone demultipleksery z użyciem `EPOLLONESHOT`
  - prywatne demultipleksery
  - `SO_REUSEPORT` dla skalowania `accept()`
- Wzorzec _Reactor_
- Wzorzec _Maszyna Stanów_
- Korutyny
- Rozwiązania _Zero-Copy_

## Materiały

1. [Slajdy wykładowe](slides)
2. [Programy przykładowe](code)
3. Stare slajdy: [Inet\_3.pdf]({{< resource "Inet_3_1.pdf" >}})
4. Dokumentacja *The GNU C library*:
   13.8: [Waiting for Input or Output](http://www.gnu.org/software/libc/manual/html_node/Waiting-for-I_002fO.html#Waiting-for-I_002fO),
   16.11: [The inet daemon](http://www.gnu.org/software/libc/manual/html_node/Inetd.html#Inetd),
   18.1 [Overview of Syslog](http://www.gnu.org/software/libc/manual/html_node/Overview-of-Syslog.html#Overview-of-Syslog),
   24.2.4: [Asynchronous I/O Signals](http://www.gnu.org/software/libc/manual/html_node/Asynchronous-I_002fO-Signals.html#Asynchronous-I_002fO-Signals)
