---
title: "Zadanie testowe z tematu: gniazda, wątki, pamięć wspólna, synchronizacja"
date: 2022-02-01T19:36:27+01:00
---

## Treść

Serwer przyjmuje połączenia TCP od klientów (telnet) i tworzy (lub przydziela z puli) dla każdego połączenia oddzielny wątek. Wątek ten od razu po utworzeniu rozpoczyna wysyłanie do swojego klienta komunikatów kontrolnych co 1 sek. W momencie gdy do serwera będzie podłączonych dokładnie 3 klientów, serwer ma wstrzymać przyjmowanie kolejnych połączeń a wątki robocze przerywają wysyłanie komunikatów kontrolnych, program przechodzi w tryb „literowania”. Pierwszy z klientów (można ich dowolnie uszeregować) ma dostać literę A, na którą ma odpisać B, drugi gdy pierwszy już odpisze dostanie B, na która powinien odpowiedzieć C itd. aż do ostatniego klienta. Naraz tylko jeden klient działa, reszta czeka. Jeśli klient zwróci niepoprawną literę to należy ponowić pytanie (i tak aż do skutku). Gdy ostatni z klientów odpowie poprawnie wszystkie wątki mają zerwać połączenie z klientem (zakończyć się lub wrócić do puli) a serwer powrócić do przyjmowania kolejnych połączeń. Serwer działa aż do SIGINT, który to kończy cały serwer natychmiast. Jeśli klient rozłączy się podczas fazy oczekiwania na 3 to nie powinno to zaburzyć działania programu, jeśli rozłączy się podczas fazy przepisywania liter to serwer ma go pominąć podczas literowania. 

## Etapy

1. (5p) Serwer przyjmuje połączenia od klienta (telnetu), tworzy lub przydziela wątek i wysyła jeden komunikat kontrolny po czym rozłącza klienta a wątek się kończy lub wraca do puli. Serwer działa aż do SIGINT.
2. (3p) Połączone wątki wysyłają komunikaty kontrolne co wyznaczony czas , gdy będzie ich 3 rozłączają klientów
3. (6p) Serwer przechodzi w tryb literowania gdy zbierze się 3 klientów
4. (3p) Program prawidłowo reaguje na zerwanie połączenia z klientem
