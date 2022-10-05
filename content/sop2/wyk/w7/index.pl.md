---
title: "Wykład 7. Programowanie sieciowe"
date: 2022-02-03T19:24:27+01:00
---

## Zakres wykładu
  
### Treści podstawowe

  - Model klient-serwer.
      - Serwer współbieżny, a iteracyjny. Wpływ typu komunikacji na wybór typu serwera.
      - Usługa sieciowa ze stanem. Przechowywanie stanu sesji
  - Modele partnerskie (P2P): czysty i hybrydowy. Zalety i wady P2P w porównaniu z modelem klient-serwer.
  - Kryteria oceny jakości usług sieciowych
  - Realizacja współbieżnego dostępu do wielu kanałów komunikacji sieciowej
      - blokujące wej/wyj + wątki lub procesy robocze.
      - wejście/wyjście zwielokrotnione - funkcje `select()`, `pselect()`

### Treści dodatkowe

  - Realizacja współbieżnego dostępu do wielu kanałów komunikacji sieciowej
      - wejście/wyjście nieblokujące
      - wejście/wyjście sterowane sygnałami
  - Uruchamianie serwerów

## Materiały

1.  Slajdy: [Inet\_3.pdf]({{< resource "Inet_3_1.pdf" >}})
2.  Dokumentacja *The GNU C library*: 13.8: [Waiting for Input or Output](http://www.gnu.org/software/libc/manual/html_node/Waiting-for-I_002fO.html#Waiting-for-I_002fO), 16.11: [The inet daemon](http://www.gnu.org/software/libc/manual/html_node/Inetd.html#Inetd), 18.1 [Overview of Syslog](http://www.gnu.org/software/libc/manual/html_node/Overview-of-Syslog.html#Overview-of-Syslog), 24.2.4: [Asynchronous I/O Signals](http://www.gnu.org/software/libc/manual/html_node/Asynchronous-I_002fO-Signals.html#Asynchronous-I_002fO-Signals)
