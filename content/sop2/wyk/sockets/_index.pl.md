---
title: "Gniazda"
weight: 60
bookCollapseSection: true
---

# Gniazda

## Zakres wykładu

* Rola warstwy transportowej
* Pojęcie gniazda (socket) i portu
* Porównanie protokołów transportowych: UDP vs TCP
* Struktura nagłówka datagramu UDP
* Cechy UDP
* Tworzenie gniazd – funkcja `socket()`
* Wiązanie gniazda z adresem – funkcja `bind()` i porty efemeryczne
* Rodziny struktur adresowych (`sockaddr`, `sockaddr_in`, `sockaddr_in6`)
* Network Byte Order vs Host Byte Order
* Funkcje konwersji i pomocnicze (`inet_pton`, `inet_ntop`, `getsockname`, `getpeername`)
* Komunikacja bezpołączeniowa – funkcje `sendto()` oraz `recvfrom()`
* Model strumienia bajtów w protokole TCP
* Identyfikacja połączenia TCP (krotka 4-elementowa)
* Struktura segmentu TCP i zastosowanie flag (`SYN`, `FIN`, `RST`, `PSH`, `ACK`)
* Numery sekwencyjne (SEQ) i mechanizm potwierdzeń (ACK)
* Zarządzanie danymi w jądrze systemu – bufory nadawcze (Tx) i odbiorcze (Rx)
* Nawiązywanie połączenia TCP (3-way handshake, Initial Sequence Number)
* Gniazda nasłuchujące i kolejka połączeń (backlog, funkcja `listen()`)
* Akceptowanie połączeń na serwerze – funkcja `accept()`
* Inicjowanie połączeń przez klienta – funkcja `connect()`
* Diagram stanów połączenia TCP
* Odczyt i zapis ze strumienia TCP – funkcje send() oraz recv()
* Zamykanie: `close()` vs `shutdown()`
* Stan `TIME_WAIT`

## Materiały

1. [Slajdy wykładowe](slides)
2. [Programy przykładowe](code)
3. Stare Slajdy: [Inet\_2.pdf]({{< resource "Inet_2_7.pdf" >}})
4. Dokumentacja *The GNU C library*: rozdz.
   16: [Sockets](http://www.gnu.org/software/libc/manual/html_node/#toc-Sockets-1), rozdz.
   13.6 [Fast Scatter-Gather I/O](http://www.gnu.org/software/libc/manual/html_node/Scatter_002dGather.html#Scatter_002dGather)
5. Literatura uzupełniająca: W.R. Stevens, Unix programowanie usług sieciowych, t. 1, wyd. 2: API: gniazda i XTI
