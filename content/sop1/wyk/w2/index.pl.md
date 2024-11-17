---
title: "System plików"
date: 2022-02-05T18:02:26+01:00
weight: 20
---

# Interfejs systemu plików i strumieniowe wejście/wyjście

## Zakres wykładu

- Koncepcja pliku. Struktury, atrybuty i podstawowe operacje na pliku.
- Otwieranie pliku. Sesja pliku. Blokada pliku.
- Tryby dostępu do pliku: sekwencyjny, bezpośredni (random access), indeksowy.
- Organizacja systemu plików: partycje, katalagi, formatowanie.
- Organizacja katalogów: cele i implementacje: jedno- i dwupoziomowa, drzewiasta, struktura grafu acyklicznego bądź
  dowolnego grafu.
- Montaż systemu plików.
- Współdzielenie dostępu do pliku: koncepcja, semantyka, ochrona.

Niskopoziomowe API POSIX:

- Otwieranie i zamykanie sesji plikowej: `open()`, `close()`
- Atrybuty plików w systemie UNIX i funkcje odpytywania: `stat(), lstat(),fstat()`
- Funkcje odczytu/zapisu: `read(), write(), pread(), pwrite(), readv(), writev()`
- Swobodny dostęp: `lseek()`
- Duplikowanie deskryptorów: `dup(), dup2()`
- Zmiany w tablicach deskryptorów, otwartych plików i i-węzłów związane z wywołaniem `open(), dup()/dup2(), fork()`
- Związki pomiędzy strumieniami, a deskryptorami: `fdopen(), fileno()`
- Synchronizacja deskryptorów: `fsync(), sync()`

Standardowe API języka C:

- Strumienie a deskryptory dostępu do pliku.
- Buforowanie strumieni.
- Otwieranie/zamykanie strumieni. Wykrywanie/kasowanie znaczników końca danych (EOF) i błędów.
- Pozycjonowanie w strumieniu i operacje odczytu/zapisu.
- Katalogi POSIX: koncepcja, katalog bieżący i główny procesu.
- Strumień katalogu i operacje na nim.

## Materiały

1. Podręcznik: rozdział 11 (Interfejs systemu plików).
2. [Prezentacja wykładowa](https://link.excalidraw.com/p/readonly/g9ICNTqqCX154vDBmccC) ([PDF]({{< resource OPS1_Filesystem_API.pdf >}}))
3. [Programy wykładowe](https://gitlab.com/SaQQ/sop1/-/tree/master/02_filesystem2?ref_type=heads)
4. The GNU C library documentation: [Input/output concepts](https://www.gnu.org/software/libc/manual/html_node/I_002fO-Concepts.html#I_002fO-Concepts)
5. The GNU C library documentation: [Low-Level Input/Output (13.1-13.5,13.8)](https://www.gnu.org/software/libc/manual/html_node/#toc-Low_002dLevel-Input_002fOutput)
6. Fragmenty standardu "POSIX IEEE Std 1003.1 2017" - [definicje]({{< resource "POSIX_excerpts.pdf" >}}) pojęć
   związanych z procesami 
7. Stare slajdy: [FS\_interfejs.pdf]({{< resource "FS_interfejs.pdf" >}}), [IO\_1.pdf]({{< resource "IO_1_14.pdf" >}}) [IO\_2.pdf]({{< resource "IO_2_3.pdf" >}})