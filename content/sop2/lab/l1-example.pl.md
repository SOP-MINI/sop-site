---
title: "Zadanie testowe z tematu pipe/FIFO"
date: 2022-02-01T19:32:59+01:00
---
## Treść

Napisz program używający łączy pipe do jednostronnej komunikacji pomiędzy trzema procesami. Każdy proces jest połączony z każdym innym jednym łączem pipe. Procesy tworzą coś w rodzaju trójkąta z jednym wyróżnionym rogiem (proces rodzic), kierunek łącza ma być tak dobrany aby możliwe było przesłanie komunikatów „w koło” pomiędzy procesami.

Początkowo proces rodzic ma wysłać liczbę 1 (jako tekst o zmiennej długości) w obieg, potem procesy pracują już identycznie tzn. odbierają liczbę, wypisują ją na stdout wraz ze swoim PID, zmieniają ją o losowy czynnik [-10,10] i przesyłają dalej. Jeśli któryś z procesów odbierze liczbę 0 to ma się zakończyć. Inne procesy poprzez detekcję zerwanego łącza także mają się zakończyć.

## Etapy

1. Tworzenie procesów i łączy, właściwe zamykanie nieużywanych deskryptorów. Każdy proces (łącznie z rodzicem) ma wypisać jakie deskryptory (numery) tworzy i jakie zamyka. Proces rodzic czeka na procesy potomne po czym się kończy. (6p)
2. Każdy z procesów wysyła przez swoje łącze losowa cyfrę [0-99] po czym odbiera to co dostał od innego procesu, wypisuje wraz ze swoim PID i kończy się. Na tym etapie trzeba rozwiązać kwestie różnych rozmiarów liczb przesyłanych. (4p)
3. Proces rodzic wysyła początkową liczbę 0, która jest przesyłana wkoło (z wypisaniem) bez zmian aż do C-c (2p)
4. Dodajemy zamiany liczby, warunek STOP i sprawdzanie zerwania łącza. (5p) 
