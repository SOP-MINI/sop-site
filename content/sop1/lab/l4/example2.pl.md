---
title: "Laboratory task 4: synchronization"
bookHidden: true
---

# Description

Napisz program symulujący przebieg gry karcianej "My Ship Sails".
Jako parametr program przyjmuje `n` - liczbę graczy (`4 ≤ n ≤ 7`).

## Zasady gry

Każdy z graczy otrzymuje 7 kart, ze standardowej puli 52 kart. 
Gra odbywa się w turach. 
W czasie jednej z nich wszyscy gracze równocześnie przekazują jedną kartę do gracza siedzącego po jego prawej stronie.
Trwa to dopóki któryś z graczy nie będzie miał w ręku wszystkich kart w tym samym kolorze. 
Mówi wtedy "My ship sails!" i gra się kończy.
Jeżeli kilku graczy spełni warunek równocześnie wszyscy wygrywają.

## Opis zadania

Talia kart jest reprezentowania poprzez tablicę liczb od 0 do 51. 
Wartość karty jest determinowana przez wynik z dzielenia jej numery przez 4 a jej kolor przez resztę z tego dzielenia.

W symulacji używany jest krupier (główny wątek) oraz wątki graczy. 
Główny wątek koordynuje grę, rozdaje karty i usadza graczy przy stole.

Główny wątek czeka na sygnał `SIGUSR1` oznaczający przybycie nowego gracza. 
Jeśli po jego otrzymaniu są wciąż wolne miejsca przy stole (liczba graczy jest mniejsza niż `n`) 
główny wątek dodaje gracza do gry (tworzy nowy wątek) i daje mu karty.
W przypadku gdy przy stole nie ma już miejsca główny wątek wypisuje informację na ten temat na standardowe wyjście.

Po otrzymaniu kart wątek gracza wypisuje swoje karty na standardowe wyjście i czeka na dołączenie pozostałych graczy.

Gra rozpoczyna się po dołączeniu n-tego gracza.
Gracze równolegle sprawdzają warunek zwycięstwa a następnie wybierają losową kartę ze swojej talii i przekazują graczowi po prawej.
Gdy któryś z graczy spełnia warunki zwycięstwa wypisuje na standardowe wyjście "My ships sails!" i swoją aktualną talię. 
Następnie informuje pozostałych graczy, że gra się zakończyła - wszystkie wątki graczy się kończą. 

Główny wątek czeka na zakończenie wątków graczy. 
Po tym tasuje talię i znowu przechodzi do czekania na graczy i nową grę.

Po otrzymaniu sygnału `SIGINT` wątek główny sygnalizuje graczom, że powinni natychmiast się zakończyć, czeka na ich zakończenie, zwalnia wszystkie zasoby i się kończy.

# Etapy

- Wątek główny tworzy `n` wątków graczy. Każdemu daje 7 kart. Wątek gracza wypisuje swoją talię i się kończy. 
Główny watek czeka na zakończenie wszystkich wątków graczy i sam się kończy

- Główny wątek czeka na sygnał `SIGUSR1` i poprawnie dodaje nowego gracza do gry. 
Gracz wypisuje swoją talię i czeka na dołączenie pozostałych graczy. 
Gdy zbierze się dostateczna ilość graczy, ich wątki się kończą a główny wątek jest gotowy powtórzyć cały proces.

- Gra odbywa się zgodnie z zasadami. **Podpowiedź:** Aby zsynchronizować etapy gry można użyć bariery.

- Działa kończenie programu przez wysłanie `SIGINT`. **Podpowiedź:** Aby zasygnalizować wątkom graczy zakończenie programu można użyć zmiennej warunkowej.

# Kod początkowy

- [sop1l4e2.zip](/files/sop1l4e2.zip)
