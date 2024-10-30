---
title: "Zadanie testowe nr 2 z tematu Środowisko wykonania programu POSIX"
bookHidden: true
---

## Treść

W tym zadaniu należy napisać program służący do skanowania
katalogu. Argumentami programu jest lista katalogów do
przeskanowanie. Wyjściem programu jest lista wszystkich plików z
ich rozmiarami (w bajtach) znajdująca się w podanych katalogach.

## Przykładowe wywołanie:

```
$ ./prog -p ./dir1 -p ./dir2 -d 2 -e txt -o
path: ./dir1
file1.txt 15
file2.txt 3315
path: ./dir2
file3.txt 23
path: ./dir2/dir3
file4.txt 44
file5.txt 12342
```

## Etapy

- Program wspiera flagę `-p <path>` służącą do podawania ścieżek
katalogów. Może ona wystąpić kilka razy.

- Program wspiera dodatkowo flagę `-e <extension>` służącą do
filtrowania wyjścia - tylko pliki z rozszerzeniem podanym po tej
fladze powinny być wypisywane.

- Program wspiera dodatkowo flagę `-d <depth>` oznaczającą
głębokość przeszukiwania - głębokość równa 1 oznacza
skanowanie tylko samego katalogu, bez wchodzenia do podkatalogów.

- Program wspiera dodatkowo flagę `-o`. Gdy zostanie podana wyjście
programu zostaje zapisane do pliku zapisanego w zmiennej środowiskowej
`L1_OUTPUTFILE` (jeśli taka zmienna istnieje). Jeśli plik o tej
nazwie już istnieje powinien zostać nadpisany.
