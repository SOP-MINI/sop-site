---
title: "Zadanie testowe z tematu kolejek POSIX"
date: 2022-02-01T19:36:27+01:00
---

## Treść

Program tworzy tyle kolejek (q1,q2,..qn) ile wynosi wartość parametru startowego (1<=n<=100), ze stdin czytane są liczby całkowite (inne wpisy odrzucamy) aż do EOF. Liczby te są bez zwłoki przesyłane do kolejki q1. Każda kolejka jest obsługiwana przez wątki, tzn jeśli w kolejce pojawią się komunikaty ma być uruchamiana notyfikacja poprzez utworzenie wątku. Wątek taki czyta z kolejki i liczby, jeśli liczby nie są podzielne przez i+1 to przesyła je do kolejki i+1, te podzielne ignoruje. Jeśli i=n to zamiast przesyłać wypisuje liczbę na ekran. Notyfikacje są przywracane po przeczytaniu aktualnej zawartości kolejki. Po wczytaniu ostatniej liczby wątek główny ma spać 2 sekundy a następnie usunąć wszystkie kolejki.

## Etapy

1. (3p) Program tworzy swoje kolejki, wypisuje ich nazwy wraz z dzielnikami, śpi 2 sekundy po czym je kasuje i się kończy.
2. (3p) Dodatkowo po utworzeniu kolejek serwer czyta dane z stdin, wysyła je do losowej kolejki, dane pozostają w kolejkach.
3. (5p) Dodajemy obsługę notyfikacji wątkiem, wątek czyta liczby z kolejki (tyle ile ich jest) i wypisuje je na ekran wraz z numerem kolejki
4. (6p) Pełne zadanie jak w opisie. 
