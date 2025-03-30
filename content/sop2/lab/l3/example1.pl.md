---
title: "Zadanie testowe z tematu pamięć dzielona i mmap"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Treść

Celem jest napisanie programu do zliczania wystąpień pojedynczych znaków w zadanym pliku.
Przy pisaniu programu możesz założyć, że każdy znak jest reprezentowany przez jeden bajt.

W celu przyśpieszenia obliczeń program powinien wykorzystywać pamięć dzieloną do synchronizacji miedzy wieloma procesami.
Dla ułatwienia przetwarzania zawartości plik powinien być zmapowany do pamięci procesu przy pomocy funkcji `mmap`.

## Etapy

1. Otwórz plik przy pomocy funkcji `mmap` w procesie rodzica.  
   Wypisz jego zawartość na standardowe wyjście.  
   Zakazane jest użycie strumieni oraz funkcji `read`.  
2. Zaimplementuj logikę zliczania znaków występujących w pliku.  
   Na końcu działania programu wypisz podsumowanie ile znalazło się poszczególnych znaków w pliku.  
3. Podziel pracę na pliku pomiędzy `N` procesów potomnych.  
   Przenieś otwieranie pliku do procesu potomnego.  
   Każdy z procesów powinien zliczyć znaki niezależnie od pozostałych.  
   Użyj pamięci dzielonej do przekazania wyników obliczeń do procesu rodzica.  
   Proces rodzica powinien wypisać podsumowanie po zakończeniu działania wszystkich procesów potomnych.  
4. Dodaj obsługę przypadku śmierci procesu potomnego.  
   W takim wypadku proces rodzica powinien zaniechać wypisania podsumowania.  
   Zamiast tego powinien wypisać informację, że obliczenia się nie powiodły.  
   Każdy proces potomny powinien mieć `3\%` szans na nagłą śmierć w momencie zgłaszania wyników do procesu rodzica (implementowane jako `abort`).  
