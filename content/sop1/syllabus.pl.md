---
title: "Program przedmiotu"
date: 2022-02-10T10:00:00+01:00
---

 - Wykład: 15h
 - Laboratorium: 15h
 - Ćwiczenia: brak
 - Projekt: brak
 - ECTS: 2

## Cele przedmiotu

Celem pierwszej częsci przedmiotu *Systemy Operacyjne* jest zapoznanie studentów z podstawami działania i konstrukcji współczesnych systemów operacyjnych, a także kształtowanie umiejętności wykorzystania funkcji systemowych do poprawnej realizacji prostych aplikacji wieloprocesowych/wielowątkowych, wykorzystujących standardardowe kanały wejścia/wyjścia i dostęp do plików.

## Planowane efekty kształcenia

Po ukończeniu kursu studenci powinni:

  - posiadać wiedzę na temat:
      - zadań, właściwości i budowy systemów operacyjnych
      - podstawowych modeli i sposobów realizacji współbieżności przez system komputerowy
      - planowania przydziału czasu procesora/procesorów
      - systemów plików i efektywnego wykorzystania interfejsu systemu plików (operacje synchroniczne i asynchroniczne)
  - umieć:
      - narysować diagram Gantta i wyznaczyć wskaźniki jakości algorytmów przydziału czasu procesora narysować diagram Gantta i obliczyć wartości wskaźników jakości algorytmów przydziału czasu procesora dla zadanego stanu kolejki procesów oczekujących na wykonanie
      - zaprojektować, napisać w języku C i przetestować stworzone przez siebie proste aplikacje wieloprocesowe/wielowątkowe (realizujące komunikację przez standardardowe kanały wejścia/wyjścia i system plików) korzystając umiejętnie z własności i interfejsu jądra systemu operacyjnego (API POSIX)

## Powiązanie z innymi przedmiotami

Wymagana jest praktyczna umiejętność programowania w języku C oraz wiedza z przedmiotu "Podstawy systemu UNIX"  
Następnikiem tego przedmiotu jest "Systemy Operacyjne 2".

## Treść wykładu

**Wprowadzenie.** System operacyjny, a system komputerowy. Zadania i interfejs systemów operacyjnych. Struktury systemów operacyjnych. Działanie systemu komputerowego.

**Procesy i wątki.** Koncepcje procesu, procesu lekkiego, włókna i wątku; cykl życia i interfejs programisty. Realizacja współbieżności procesów i wątków. Współprogramy (*coroutines*) i kontynuacje. Sygnały i wyjątki oraz ich obsługa. Funkcje systemowe POSIX związane z obsługą procesów, sygnałów i wątków.

**Modele i interfejsy systemów plików.** Plik, organizacja systemu plików. Operacje na systemie plików i interfejs programisty. Dostęp i ochrona plików, tryby dostępu do plików. Funkcje systemowe (POSIX) związane z obsługą synchroniczną i asynchroniczną plików i katalogów. Funkcje obsługi strumieni wejścia/wyjścia standardowej biblioteki języka C.

## Zakres zajęć laboratoryjnych:

  - Środowisko wykonania programu POSIX
  - Procesy i sygnały
  - Wątki i muteksy

## Literatura

  - podstawowa:
    1.  A. Silberschatz,  P.B. Galvin, G. Gagne, *Podstawy systemów operacyjnych*, wyd. 6 zm. i rozsz., WNT 2005.
    2.  The GNU C Library Manual, <http://www.gnu.org/software/libc/manual/>
  - uzupełniająca:
    1.  A.S. Tanenbaum, H. Bos, *Systemy operacyjne*, wyd. IV, Wyd. Helion, 2015
    2.  W. Stallings, *Systemy operacyjne. Struktura i zasady budowy*, Wyd. Naukowe PWN SA, 2006.
    3.  K. Stencel, *Systemy operacyjne*, Wyd. PJWSTK, 2004
    4.  U. Vahalia, *Jądro systemu UNIX. Nowe horyzonty*, WNT 2001
    5.  W.R. Stevens, *Programowanie w środowisku systemu UNIX*, WNT 2002.
    6.  M.K. Johnson, E.W. Troan, *Oprogramowanie użytkowe w systemie Linux*, WNT 2000.
    7.  K. Haviland, D. Gray, B. Salama, *Unix. Programowanie systemowe*, Wyd. RM, Warszawa 1999.
    8.  K. Wall, *Linux, programowanie w przykładach*, MIKOM 2000.
    9.  POSIX.1-2017 - dokumentacja ([The Open Group Base Specifications Issue 7, 2018 edition IEEE Std 1003.1-2017](http://pubs.opengroup.org/onlinepubs/9699919799/))

