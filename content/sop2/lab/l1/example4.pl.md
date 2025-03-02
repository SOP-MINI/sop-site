---
title: "Zadanie testowe z tematu pipe/FIFO"
date: 2022-02-01T19:32:59+01:00
bookHidden: true
---

## Zadanie

Napisz symulator ruletki, używając procesów komunikujących się przez nienazwane łącza.
Program przyjmuje dwa parametry: liczbę graczy `N` (`N ≥ 1`) oraz początkową kwotę `M` (`M ≥ 100`).

Gracze obstawiają pojedynczy numer `[0, 36]` z wypłatą 35:1. Zwycięski numer jest losowany z zakresu `[0, 36]`.

## Etapy

1. Krupier (główny proces) tworzy `N` procesów graczy. Każdy gracz wypisuje:
   `[process id]: I have [amount] and I'm going to play roulette`.

2. Gracze komunikują się z krupierem przez nienazwane łącza. Każdy gracz wysyła losową stawkę (nieprzekraczającą jego budżetu) oraz wybrany numer.
   - Krupier wypisuje: `Dealer: [process id] placed [amount] on [number]` po otrzymaniu zakładu.
   - Krupier losuje numer i ogłasza: `Dealer: [number] is the lucky number`.
   - Po jednej rundzie wszyscy gracze i krupier kończą grę.

3. Gra trwa, dopóki co najmniej jeden gracz ma pieniądze.
   - Jeśli gracz straci wszystkie pieniądze, wypisuje: `[process id]: I'm broke` i wychodzi.
   - Jeśli gracz wygra, wypisuje: `[process id]: I won [amount]`.
   - Gdy wszyscy gracze opuszczą grę, krupier wypisuje: `Dealer: Casino always wins` i kończy pracę.

4. W każdej rundzie gracz ma 10% szans na opuszczenie gry z pozostałymi pieniędzmi. Wypisuje wtedy: `[process id]: I saved [amount left]` i opuszcza grę.
