---
title: "Projekt"
---

Projekt trwa cztery tygodnie od 18.11 do 16.12. W czasie W3 (16.12) ma miejsce sprawdzanie i ocenianie zgodnie ze szczegółami opisanymi w treści.

Kod źródłowy do oceny powinien zostać umieszczony na serwerze podobnie jak na laboratoriach - ścieżka do projektu to `p1pl`. Należy mieć na uwadze pewne ograniczenia techniczne:
- Niestety serwer jest włączany tylko dwa razy w tygodniu - w poniedziałki w godzinach 20:15-23:00 oraz we wtorki 14:15-17:00. Tylko wtedy jest możliwe spushowanie swojego rozwiązania
- Serwer jest dostępny tylko z sieci wydziałowej. Żeby wypchnąć swoje rozwiązanie spoza niej można skorzystać z dwóch możliwości:
  - przekopiować je na serwer ssh (scp na ssh.mini.pw.edu.pl) i stamtąd wykonać push jak na laboratorium
  - użyć tunelowania ssh (uwaga - trzeba wtedy też zmienić/dodać `remote` w repozytorium)

Serwer wykonuje wstępne sprawdzenie, podobnie jak na laboratorium, a w przypadku błędu może odrzucić rozwiązanie. Jednak, ponieważ projekt wymaga napisania większej ilości kodu niż na zwyczajnym laboratorium, zasady są nieco luźniejsze. Dopuszczalne (i rekomendowane) jest tworzenie i modyfikacja nowych plików .c i .h. Nie można jedynie modyfikować startowego `Makefile` oraz `.clang-format`. Jak zawsze pliki źródłowe powinny być sformatowane a program kompilować się bez błędów i ostrzeżeń.

[Treść zadania znajduje się tutaj](/files/p1pl.pdf)

W razie wątpliwości zapraszamy do zadawania pytań po zajęciach/drogą mailową.
