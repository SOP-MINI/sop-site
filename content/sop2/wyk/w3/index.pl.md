---
title: "Wykład 3. Synchronizacja. Zakleszczenia"
date: 2022-01-28T17:23:23+01:00
---

## Zakres wykładu
  
  - Synchronizacja.
      - Wprowadzenie - naiwne rozwiązanie problemu producent-konsument; analiza niepowodzenia, wyścigi.
      - Problem sekcji krytycznej i 3 warunki poprawnego rozwiązania.
      - Sprzętowe wspomaganie synchronizacji. Wykorzystanie funkcjonalnoci: `zamień()` i `Testuj_i_Ustal()`. Realizacja ograniczonego oczekiwania.
      - Semafory: binarne i zliczające. Implementacja. Wykorzystanie. Ograniczenia: blokada, głodzenie, inwersja priorytetów. Podstawowe błędy użycia.
      - Klasyczne problemy synchronizacji: ograniczonego buforowania, czytelników i pisarzy, obiadujących filozofów. Rozwiązania.
      - Koncepcja monitora. Zmienne warunkowe.
      - Dostępność mechanizmów synchronizacji: MSWin, Linux. POSIX.
  - Zakleszczenia
      - Problem zakleszczenia. 4 warunki konieczne zakleszczenia.
      - Graf alokacji zasobów, a zakleszczenie.
      - 3 metody postępowania z zakleszczeniami: zapobieganie lub unikanie, obserwacja i likwidacja, ignorowanie problemu.
      - Stan bezpieczny, ciąg bezpieczny, algorytm bankiera.
      - Wykrywanie zakleszczeń

## Materiały

1.  Podręcznik: rozdz. 7 (Synchronizacja procesów), 8 (Zakleszczenia).
2.  Slajdy: [Synchro.pdf]({{< resource "Synchro_3.pdf" >}}), [Zakleszczenia.pdf]({{< resource "Zakleszczenia_1.pdf" >}})

