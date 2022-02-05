---
title: "Wykład2: Procesy"
date: 2022-02-05T18:02:21+01:00
draft: false
---

## Zakres wykładu
  
  - Koncepcja procesu. Modele procesu: UNIX (POSIX), MSWin (Win32).
  - Graf stanów procesu
  - Blok Kontrolny Procesu. Kontekst procesu. Przełączanie CPU pomiędzy procesami.
  - Kolejki planowania procesów. Planiści: krótkoterminowy, długoterminowy, średnioterminowy.
  - Tworzenie procesów. Modele: POSIX, Win32.
  - Kończenie procesów. POSIX: rola wait(), sierota, zombie.
  - Procesy współpracujące. Modele komunikacji międzyprocesowej. Rola synchronizacji procesów.
  - Środowisko wykonania procesu POSIX: zmienne środowiskowe, 3 początkowe strumienie (stdin, stdout, stderr). Moduł startowy (crt0)
  - Cykl życia procesu POSIX
  - Dziedziczenie przy wywołaniu funkcji systemowych `fork` i `exec`.
  - Funkcje standardowej biblioteki języka C opakowujące wywołania funkcji systemowej `exec`.
  - Identyfikatory związane z procesem: PID, GID, real/effective ID. `setuid/setgid`.
  - Grupy procesów.
  - Tworzenie procesu powłoki zgłoszeniowej (*login shell*).

## Materiały

1.  Podręcznik: rozdz. 4 (Procesy).
2.  Slajdy: [Procesy.pdf]({{< resource "Procesy_0.pdf" >}}), [Procesy\_POSIX.pdf]({{< resource "Procesy_POSIX_5.pdf" >}})
3.  Rozdział 26 dokumentacji "GNU C library": [Processes](http://www.gnu.org/software/libc/manual/html_node/Processes.html#Processes)
4.  [Archiwum](http://alpha.mini.pw.edu.pl/~opalskil/OS1src/processes/S2.zip) kodu wykorzystywanego na wykładzie do eksperymentów, które przede wszystkim mają pokazać interesujące własności funkcji systemowych (POSIX) i biblioteki C. Zazwyczaj przykłady nie promują jednak dobrych praktyki programowania. Jeśli kod skłoni studentów do dalszych interesujących eksperymentów - proszę o podzielenie się z wykładowcą obserwacjami.
