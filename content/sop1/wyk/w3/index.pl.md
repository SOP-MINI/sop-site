---
title: "W3: Interfejs systemu plików. Strumieniowe wejście/wyjście."
date: 2022-02-05T18:02:26+01:00
draft: false
---

## Zakres wykładu

  - Koncepcja pliku. Struktury, atrybuty i podstawowe operacje na pliku.
  - Otwieranie pliku. Sesja pliku. Blokada pliku.
  - Tryby dostępu do pliku: sekwencyjny, bezpośredni (random access), indeksowy.
  - Organizacja systemu plików: partycje, katalagi, formatowanie.
  - Organizacja katalogów: cele i implementacje: jedno- i dwupoziomowa, drzewiasta, struktura grafu acyklicznego bądź dowolnego grafu.
  - Montaż systemu plików.
  - Współdzielenie dostępu do pliku: koncepcja, semantyka, ochrona.
  - Strumienie a deskryptory dostępu do pliku.
  - Buforowanie strumieni.
  - Otwieranie/zamykanie strumieni. Wykrywanie/kasowanie znaczników końca danych (EOF) i błędów.
  - Pozycjonowanie w strumieniu i operacje odczytu/zapisu.
  - Katalogi POSIX: koncepcja, katalog bieżący i główny procesu.
  - Strumień katalogu i operacje na nim.

## Materiały

1.  Podręcznik: rozdział 11 (Interfejs systemu plików).
2.  Slajdy: [FS\_interfejs.pdf]({{< resource "FS_interfejs.pdf" >}}), [IO\_1.pdf]({{< resource "IO_1_14.pdf" >}})
3.  Dodatkowa lektura: The GNU C library documentation: [Input/output concepts](https://www.gnu.org/software/libc/manual/html_node/I_002fO-Concepts.html#I_002fO-Concepts)
4.  Fragmenty standardu "POSIX IEEE Std 1003.1 2017" - [definicje]({{< resource "POSIX_excerpts.pdf" >}}) pojęć związanych z procesami
