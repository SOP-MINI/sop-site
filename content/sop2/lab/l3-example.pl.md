---
title: "Zadanie testowe z tematu gniazd sieciowych i funkcji select"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Treść

Serwer TCP przyjmuje połączenia od klientów, każdy z klientów przesyła do serwera 3 losowe liczby z przedziału [1-1000] co 0.75 sekundy. W odpowiedzi na każdą z liczb serwer przesyła maksymalną liczbę jaką dostał do tej pory. Jeśli klient w odpowiedzi otrzyma tą samą liczbę co wysłał to ma wypisać słowo „HIT” na stdout. Klient kończy się po 3 próbach, serwer działa aż do otrzymania SIGINT, w reakcji na który ma wypisać ile w sumie liczb dostał. Dane mają być przesyłane jako liczby (binarnie) a nie jako tekst. Serwer ma być programem jedno procesowym i jedno wątkowym.

## Etapy

1. (4p) Pojedynczy klient łączy się z serwerem i przesyła mu losową liczbę, serwer wypisuje co dostał i się kończy. Serwer kończy się po jednym połączeniu z klientem
2.(3p) Serwer akceptuje wiele połączeń jedno po drugim (nie w tym samym czasie), odsyła klientom to co wysłali zwiększone o 1, klient wypisuje co dostał i po 3 próbach się kończy. Serwer nie ma poprawnego zakończenia, zamykamy C-c.
3. (4p) Serwer nadal pracuje szeregowo, wyznacza aktualną maksymalną wartość otrzymaną i ją odsyła klientom. Klient rozpoznaje kiedy dostaje tą samą liczbę jak wysłał i wypisuje „HIT”.
4. (6p) Serwer przyjmuje klientów równolegle, oblicza ile było liczb przesłanych do niego i w reakcji na SIGINT kończy się wypisując tę liczbę 
