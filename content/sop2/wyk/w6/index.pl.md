---
title: "Wykład 6. Interfejs gniazd"
date: 2022-02-03T19:18:40+01:00
---

## Zakres wykładu

  - Gniazda a system plików UNIXa: deskryptory gniazd/plików, read/write/close, obsługa błędów, API gniazd częscią interfejsu jądra.
  - Modele prostej komunikacji klient-serwer w realizacji połączeniowej i datagramowej
  - Tworzenie gniazd: `socket()`, rodziny: protokołów i adresów, opcje gniazd i
  - Struktury adresowe gniazd: ogólna `sockaddr`, domeny inet `sockaddr_in`, domeny Unix `sockaddr_un`.
  - Porządek sieciowy bajtów i funkcje do zmiany porządku bajtów: `htons()`, `htonl()`, `ntohs()`, `ntohl()`
  - Funkcje do konwersji adresów: `inet_addr()`, `inet_ntoa()`, `inet_aton()`, `inet_ntop()`, `inet_pton()`
  - Korzystanie z nazw stacji: struktura `hostent` i funkcje `gethostbyname()`, `gethostbyaddr()`; struktura `addrinfo` i funkcje: `getaddrinfo()`, `freeaddrinfo()`, `gai_strerror()`, `getnameinfo()`
  - Adresowanie gniazda TCP: jawne (`bind()`), niejawne (`connect()`)
  - Otwieranie połączenia: strona bierna (`listen()`, `accept`), strona aktywna (`connect`)
  - Przesyłanie danych przez połączenie TCP: `read()`, `recv()`, `readv()`,`recvmsg`, `write()`, `send()`, `writev()`, `sendmsg`, sygnał `SIGPIPE`.
  - Zamykanie połączenia: uporządkowane (`shutdown()`) i nieuporządkowane (`close()`). Zwlekanie (*linger*).
  - Komunikacja UDP: adresowanie (nadawca, odbiorca), podstawowe funkcje komunikacji: `recvfrom()`, `sendto()`
  - Połączone gniazda UDP: jak i dlaczego. Asynchroniczny błąd komunikacji UDP.

### Uwaga

Kolokwia wykładowe oczekują od studenta znajomości ogólnych koncepcji budowy i podstawowych własności interfejsu gniazd, a także zasad wykorzystania podstawowych funkcji interfejsu gniazd do realizacji komunikacji połączeniowej i bezpołączeniowej. Nie jest wymagane pamiętanie takich szczegółów interfejsu, jak np. argumentów funkcji.
  
## Materiały

1.  Slajdy: [Inet\_2.pdf]({{< resource "Inet_2_7.pdf" >}})
2.  Dokumentacja *The GNU C library*: rozdz. 16: [Sockets](http://www.gnu.org/software/libc/manual/html_node/#toc-Sockets-1), rozdz. 13.6 [Fast Scatter-Gather I/O](http://www.gnu.org/software/libc/manual/html_node/Scatter_002dGather.html#Scatter_002dGather)
3.  Literatura uzupełniająca: W.R. Stevens, Unix programowanie usług sieciowych, t. 1, wyd. 2: API: gniazda i XTI
