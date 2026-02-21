---
title: "Zadanie testowe z sieci"
bookHidden: true
---

## Treść

Serwer TCP przyjmuje połączenia od klientów, każdy z klientów wypisuje swój pid, podłącza się do serwera i wysyła swój PID jako tekst. Serwer odsyła klientowi sumę cyfr jego PIDu jako int16_t. Klient wypisuje otrzymany wynik i się kończy. Serwer akceptuje połączenie do czasu otrzymania sygnału SIGINT. Po otrzymaniu tego wypisuje najwyższy otrzymany wynik i się kończy.

## Przykład

```
./server 2000&
./client localhost 2000
PID=1244
SUM=11
./client localhost 2000
PID=1245
SUM=12
killall -s SIGINT server
HIGH SUM=12
```

## Etapy

1. Serwer akceptuje tylko jedno połączenie, odczytuje dane od klienta i wypisuje. Klient po wysłaniu danych się kończy.
2. Serwer akceptuje wiele połączeń, liczy i odsyła wyniki do klienta, klient wypisuje wynik.
3. Serwer reaguje na SIGINT
4. Wszystkie możliwości zerwania połączenie są odpowiednio sprawdzane i osługiwane.
