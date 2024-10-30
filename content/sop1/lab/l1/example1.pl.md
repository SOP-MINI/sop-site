---
title: "Zadanie testowe nr 1 z tematu Środowisko wykonania programu POSIX"
bookHidden: true
---

## Treść

Napisz program, który przyjmuje następujące parametry wykonania:

  - `-p` ścieżka do katalogu (dowolna ilość wystąpień)  
    wskazujemy katalogi które będą przetwarzane przez program
  - `-o` nazwa pliku (maksymalnie jedno wystąpienie)  
    nazwa pliku do którego będą wpisywane wyniki, w razie braku tego parametru wyniki są wypisywane na stdout

Dla każdej podanej ścieżki program ma utworzyć listing katalogu w pliku (lub na stdout), przykładowo:

```
    SCIEZKA:
    ./katalogA
    LISTA PLIKÓW:
    test.txt 12345
    dane.dat 15
```

Liczby przy plikach oznaczają rozmiary plików. Listujemy wszystkie obiekty w katalogu, nie tylko pliki.

## Etapy

1.  Program robi listing tylko dla katalogu roboczego, bez rozmiarów. Wyniki są wypisywane na stdout. *Jak pokazać:* uruchomić program bez parametrów
2.  Dodajemy rozmiary obiektów. *Jak pokazać:* uruchomić program bez parametrów
3.  Program rozpoznaje parametr `-p` (inne ignoruje) i dla każdej ścieżki wypisuje ma stdout listing. *Jak pokazać:* uruchomić program z parametrami `-p /run -p /var`
4.  Program rozpoznaje parametr `-o` i jeśli on występuje wyniki są zapisywane do wskazanego pliku. *Jak pokazać:* uruchomić program z parametrami `-p /run -p /var -o \~/out.txt` następnie wykonać polecenie `cat \~/out.txt`
5.  Zaimplementuj alternatywną wersję etapu 4., tym razem używając niskopoziomowych funkcji `open`, `close` i `read`.
6.  W przypadku podania nieobsługiwanych parametrów lub dodatkowych pozycyjnych parametrów program ma wypisać usage, w przypadku podania nieistniejącego katalogu lub katalogu do którego nie mamy dostępu program ma wypisać błąd na stderr ale kontynuować wypisywanie dla pozostałych parametrów. Podanie nadmiarowych -o ma prowadzić do błędu i wypisania usage. *Jak pokazać:* po jednym błędnym wywołaniu dla każdego typu błędu
