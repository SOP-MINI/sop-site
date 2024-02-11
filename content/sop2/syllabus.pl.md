---
title: "Program przedmiotu"
date: 2022-02-10T10:00:00+01:00
---

 - Wykład: 30h
 - Laboratorium: 15h
 - Ćwiczenia: none
 - Projekt: none
 - ECTS: 3

## Cele przedmiotu

Celem drugiej części przedmiotu "Systemy Operacyjne" jest zapoznanie studentów z zaawansowanymi problemami implementacji współczesnych systemów operacyjnych, a także kształtowanie umiejętności wykorzystania zaawansowanych mechanizmów systemowych do poprawnej realizacji (w środowisku POSIX/UNIX/Linux) aplikacji wieloprocesowych/wielowątkowych, wykorzystujących różne środki komunikacji międzyprocesowej i synchronizacji oraz komunikację sieciową.

## Planowane efekty kształcenia

Po ukończeniu kursu studenci powinni:

-   posiadać wiedzę na temat:
    -   algorytmów planowania przydziału czasu procesora/procesorów i metod ich oceny
    -   organizacji i efektywnego wykorzystania pamięci systemu komputerowego
    -   zasady działania i efektywnego wykorzystania pamięci wirtualnej
    -   środków komunikacji pomiędzy procesami (pamięci współdzielonej, kolejek komunikatów, łączy)
    -   podstawowych środków i schematów synchronizacji oraz problemu zakleszczeń
    -   komunikacji za pomocą gniazd sieciowych
    -   problemów bezpieczeństwa i ochrony zasobów systemów komputerowych
-   umieć:
    -   wyznaczać wskaźniki jakości algorytmów przydziału czasu procesora, czy średni czas dostępu do pamięci
    -   wykrywać zakleszczenia algorytmów przydziału zasobów za pomocą algorytmu bankiera
    -   zaprojektować, napisać w języku C i przetestować stworzoną przez siebie prostą aplikację sieciową w układzie klient-serwer, wykorzystując interfejs gniazd sieciowych, kolejki komunikatów lub łącza.

### Treść wykładu

 - **Komunikacja międzyprocesowa i synchronizacja.** Komunikacja między procesami jednego systemu komputerowego: łącza nazwane (FIFO) i nienazwane (pipe), pamięć dzielona, kolejki komunikatów; synchronizacja współpracujących procesów i wątków: sposoby realizacji i interfejs programisty. Problem zakleszczania: model i metody postępowania.
 - **Realizacja systemów plików.** Organizacja wybranych systemów plików. Dostęp i ochrona plików, tryby dostępu do plików.
 - **Podystem wejścia/wyjścia.** Urządzenia znakowe i blokowe; tryby dostępu. Moduły obsługi urządzeń wejścia/wyjścia
 - **Zarządzanie pamięcią.** Organizacja pamięci operacyjnej i dostępu do pamięci przez procesor. Algorytmy przydziału pamięci. Modele wykorzystania pamięci przez proces. Pamięć wirtualna: budowa, własności, interfejs programisty.
 - **Komunikacja sieciowa.** Model komunikacji OSI. Interfejs gniazd w programowaniu połączeniowych i bezpołączeniowych aplikacji sieciowych (w układzie klient-serwer). Problemy komunikacji i synchronizacji aplikacji sieciowych. Przykłady usług sieciowych.
 - **Szeregowanie zadań.** Realizacja współbieżności procesów i wątków. Algorytmy szeregowania zadań (systemów jedno- i wieloprocesorowych). Specyfika szeregowania zadań w systemach czasu rzeczywistego. Ocena jakości algorytmów szeregowania.
 - **Ochrona i bezpieczeństwo systemów komputerowych.** Cele ochrony. Uwierzytelnianie, kontrola dostępu do zasobów, integralność, niezaprzeczalność, poufność - koncepcje i realizacje. Standardy oceny bezpieczeństwa.
 - **Przegląd współczesnych systemów operacyjnych.** Linux, MS Windows, QNX/Neutrino, freeRTOS. Wirtualizacja systemów komputerowych.

### Zakres zajęć laboratoryjnych

-   L1: Łącza nazwane/nienazwane (pipe/FIFO) (3g)
-   L2: Kolejki POSIX (3g)
-   L3: Gniazda, pselect (3g)
-   L4: Gniazda, wątki, pamięć wspólna, synchronizacja (3g)
-   L5: Indywidualna poprawa jednego ćwiczenia (3g)

### Literatura

-   podstawowa:
    1.  A. Silberschatz,  P.B. Galvin, G. Gagne, *Podstawy systemów operacyjnych*, wyd. 6 zm. i rozsz., WNT 2005.
    2.  The GNU C Library Manual, <http://www.gnu.org/software/libc/manual/>
-   uzupełniająca:
    1.  W. Stallings, *Systemy operacyjne - architektura, funkcjonowanie i projektowanie*, Wyd. 9, Helion, 2018.
    2.  A. S. Tanenbaum, H. Bos, *Systemy operacyjne.*, Wyd. IV, Helion, 2015
    3.  K. Stencel, *Systemy operacyjne*, Wyd. PJWSTK, 2004
    4.  W.R. Stevens, *Unix programowanie usług sieciowych*, [t. 1](http://www.kohala.com/start/unpv12e.html), wyd. 2: API: gniazda i XTI, WNT, 2000
    5.  W.R. Stevens, *Unix Network Programming*, [t. 2](http://www.kohala.com/start/unpv22e/unpv22e.html), wyd. 2: Komunikacja międzyprocesowa, WNT, 2001
    6.  M.J. Rochkind, *Programowanie w systemie Unix dla zaawansowanych* (wyd. 2), WNT, 2007
