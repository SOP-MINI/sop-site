---
title: "Zadanie testowe z tematu Wątki muteksy i sygnały"
date: 2022-02-05T19:36:50+01:00
bookHidden: true
---

## Treść

Należy napisać program symulujący wyścig psów. Program przyjmuje dwa
argumenty, liczby naturalne: `n>20` — długość toru oraz `m>2` — liczbę
psów. Główny wątek programu tworzy tor wyścigowy dla psów — tablicę
intów o długości `n`. Tablica początkowo jest wypełniona zerami. Tablica
będzie przechowywać liczbę psów znajdujących się w danym miejscu na
torze. Po zainicjowaniu tablicy program tworzy `m` wątków, każdy wątek
reprezentuje jednego psa.

Wątek psa działa w następujący sposób: początkowo pies znajduje się poza
torem, na początku wątek umieszcza psa na pozycji 0 i zwiększa wartość
tablicy o 1. Kierunek przemieszczania się psa to rosnące indeksy
tablicy. Potem, w pętli wykonuje następujące akcje:

1.  Czeka losowy czas z przedziału `[200, 1520]`ms
2.  Losuje liczbę całkowitą z przedziału `[1,5]` – odległość, o którą
    się przemieści.
3.  Sprawdza, czy wylosowana pozycja jest w zakresie tablicy. Jeśli nie,
    to przemieszcza się tak daleko, jak się da, po czym zmienia kierunek
    przemieszczania.
4.  Sprawdza, czy na pozycji, na którą się przeniósł, znajduje się inny
    pies. W takiej sytuacji wypisuje komunikat `waf waf waf` i pozostaje
    na swoim miejscu.
5.  Wypisuje na stdout swój numer i nową pozycję i ew. informacje o
    dotarciu do końca toru
6.  Aktualizuje odpowiednio wartości w tablicy (zmniejsza wartość na
    pozycji, którą opuszcza i zwiększa na pozycji, do której przybywa).

Po dobiegnięciu do końca tablicy pies wypisuje, który w kolejności
przybył do mety i kończy swoją pracę.

Główny wątek co 1000 ms wypisuje stan tablicy. Gdy każdy pies skończy
wyścig, program powinien wypisać trzy najszybsze psy i się zakończyć. W
reakcji na sygnał `SIGINT` główny wątek anuluje wszystkie wątki psów,
czeka na ich zakończenie, po czym program się kończy wypisując
informacje o przerwanych zawodach.

Dostęp do tablicy należy blokować odrębnym mutexem na każdy element.
Licznik psów, które dotarły do mety także powinien być blokowany
mutexem.

W programie nie można używać żadnych zmiennych globalnych.

## Oceniane etapy

1. Odczytać argumenty, zainicjować tablicę. Utworzyć wątki psów. Każdy
   wątek zwiększa o 1 losową komórkę tablicy, wypisuje jej numer i się
   kończy. Po zakończeniu wszystkich wątków psów wątek główny wypisuje
   końcowy stan toru.
2. Działa logika wątku psa, na razie bez blokowania.
3. Działa blokowanie z użyciem mutexów.
4. Działa kończenie programu z użyciem `SIGINT`.
