---
title: "Wykład 13. Systemy czasu rzeczywistego"
date: 2022-02-05T15:59:28+01:00
---

## Zakres wykładu

Systemy czasu rzeczywistego (RT)

  - Systemy rygorystyczne, a systemy łagodne - podstawowe własności, zastosowania.
  - Systemy RT, a systemy biurkowe: terminal/interakcja, zarządzanie pamięci± dynamiczną, pamięć wirtualna, specjalizacja/uniwersalność
  - "Niezbędniki" systemu RT:
      - wywłaszczalne, oparte na priorytetach, planowanie przydziału procesora
      - wywłaszczalne jądro
      - opóźnienie reakcji musi być minimalizowane
  - Planowanie przydziału procesora:
      - Planowanie częstotliwościowo-monotoniczne (RM, Rate-Monotonic Scheduling) - optymalny przydział statycznych priorytetów zadań okresowych, daje dość małe oszacowanie najgorszego przypadku wykorzystania czasu procesora (N(2^1/N-1))
      - Planowanie wg najwcześniejszego terminu (EDF, Earliest Deadline First) - nie wymaga okresowości zadań, konieczne jest jedynie powiadomienie planisty o ograniczeniu czasowym procesu (deadline), kiedy proces stanie się gotowy do wykonania.
  - POSIX: planowanie przydziału procesora
      - Real-time (RT): SCHED\_FIFO, SCHED\_RR, SCHED\_SPORADIC
      - Nie RT: SCHED\_OTHER
      - Przykłady systemów RT - VxWorks, RTLinux

## Materiały

1.  Slajdy: [RT.pdf]({{< resource "RT_4.pdf" >}})
2.  Podręcznik: rozdz. 6.5 - "Planowanie w czasie rzeczywistym".
3.  `man: sched(7)`
