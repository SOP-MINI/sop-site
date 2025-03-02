---
title: "Zadanie testowe z tematu pipe/FIFO"
date: 2022-02-01T19:32:59+01:00
bookHidden: true
---

## Zadanie
Napisz program symulujący grę "My Ship Sails" przy użyciu procesów komunikujących się przez łącza nienazwane.  
Program przyjmuje dwa argumenty:  
`N` - liczba graczy (`4 <= N <= 7`)  
`M` - liczba kart na ręce (`M >= 4` i `M ⋅ N <= 52`)  

## Zasady
Każdy gracz otrzymuje `M` kart z talii 52 kart (karty są reprezentowane jako liczby całkowite `0-51`).  
Co turę gracze jednocześnie przekazują jedną kartę sąsiadowi po prawej stronie.  
Gra trwa do momentu, gdy któryś z graczy zbierze `M` kart w tym samym kolorze i ogłosi: `My ship sails!`.  
Wygrywa pierwszy, kto ogłosi `My ship sails!`.  
Kolor karty można określić operacją `% 4`.  

## Etapy

1. **Inicjalizacja:**
   - Proces serwera tworzy `N` procesów graczy.
   - Tasuje talię i rozdaje `M` kart każdemu graczowi przez łącza nienazwane.
   - Każdy gracz wypisuje swoją rękę wraz z identyfikatorem procesu i kończy działanie.

2. **Rozgrywka:**
   - Gracze tworzą pierścień i przekazują karty przez łącza nienazwane (`n`-ty gracz → `(n+1 % N)`-ty gracz).
   - Gracz, który zbierze `M` kart w jednym kolorze, wypisuje `[PID]: My ship sails!` (gra toczy się dalej).

3. **Warunek zwycięstwa:**
   - Serwer tworzy współdzielone łącze do ogłaszania zwycięzcy.
   - Zwycięzca zapisuje swój PID do łącza, wypisuje `[PID]: My ship sails!`, a następnie kończy działanie.
   - Serwer odczytuje PID, wypisuje `Server: [PID] won!` i kończy działanie.

4. **Zakończenie:**
   - `Ctrl-C` natychmiast zatrzymuje wszystkie procesy i czyści zasoby.
