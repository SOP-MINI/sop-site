---
title: "Zadanie testowe nr 2 z tematu Procesy, sygnały i deskryptory"
date: 2022-02-05T19:34:07+01:00
bookHidden: true
---

## Treść
Napisz program symulujący klaster obliczeniowy.

Proces-rodzic tworzy N dzieci, gdzie N jest jedynym argumentem przyjmowanym przez program.
Każde dziecko przyjmuje numer od 0 do N-1.

Rodzic zleca pierwszemu dziecku pracę, wysyłając sygnał SIGUSR1.
Dziecko po otrzymaniu sygnału rozpoczyna pracę w pętli, śpi losową ilość czasu z przedziału 100-200ms, po czym dodaje jeden do lokalnego licznika i wypisuje {PID}: {licznik}\n na stdout.

Jeśli rodzic otrzyma sygnał SIGUSR1 to wysyła sygnał SIGUSR2 do obecnie pracującego dziecka i SIGUSR1 do następnego dziecka o indeksie większym o jeden (dla ostatniego dziecka wyślij do dziecka o indeksie 0).
Dziecko, które otrzyma sygnał SIGUSR2 zatrzymuje swoją pętlę i oczekuje na sygnał SIGUSR1 aby wznowić działanie.
W skrócie, SIGUSR1 zleca dziecku pracę, a SIGUSR2 zleca zatrzymanie pracy.

Po otrzymaniu sygnału SIGINT rodzic przekazuje ten sam sygnał wszystkim dzieciom.
Po otrzymaniu sygnału SIGINT dzieci zapisują (wykorzystując wywołanie systemowe) swój licznik do pliku {PID}.txt i kończą działanie.
Rodzic czeka na zakończenie działania wszystkich dzieci i kończy pracę.

## Oceniane etapy

1. Proces-rodzic tworzy dzieci. Proces-dziecko wypisuje swój PID oraz swój indeks i kończy działanie. Rodzic czeka na zakończenie działania wszystkich dzieci i kończy pracę. **(3p)**
2. Rodzic zleca pierwszemu dziecku pracę, wysyłając sygnał SIGUSR1. Dziecko po otrzymaniu sygnału wykonuje pracę w pętli. **(5p)**
3. Obsługa sygnału SIGUSR1 przez rodzica, obsługa sygnału SIGUSR2 przez dzieci. **(4p)**
4. Obsługa zakończenia działania programu za pomocą sygnału SIGINT. Zapis liczników do pliku przez dzieci. **(4p)**
