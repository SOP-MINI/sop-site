---
title: "Zadanie testowe z tematu Synchronizacja"
date: 2022-02-05T19:36:50+01:00
bookHidden: true
---

## Treść

Napisz program, który przyjmuje 3 parametry wejściowe: n, m, path. Gdzie n to liczba wątków posix, a path to ścieżka do pliku csv. Wątek główny czyta nagłówek pliku i sprawdza jego rozmiar. Następnie dzieli resztę pliku równo na m fragmentów. Wątek główny tworzy pulę n wątków. Każdy wątek czyta następny dostępny fragment i zapisuje wczytane linie do listy dwukierunkowej. Jeśli wątek napotka błąd podczas czytania pliku (brak przecinka lub zbyt wiele przecinków w lini), czeka, aż wszystkie aktualnie działające wątki kończą czytanie, raportuje błąd do stderr, w tym numer linii, na której został napotkany, zatrzymuje dalsze przetwarzanie, a następnie program kończy działanie. Po wczytaniu całej zawartości pliku wynikowe listy są łączone w jedną, której zawartość jest wypisywana przez jeden wątek na stdout.

## Oceniane etapy (w ramach pokazania jak działa etap proszę uruchomić z parametrem 10)

1.  Wątek główny czyta nagłówek csv i rozmiar pliku, a następnie dzieli go na m fragmentów. Tworzy n wątków. Każdy wątek drukuje "*" i kończy działanie. (3p.)
2.  Zaimplementowana została pula wątków. Wątki biorą jedno dostępne zadanie z fragmentem i drukują przypisany fragment (start i rozmiar w bajtach), dopóki nie pozostanie żadnych zadań z fragmentami. Wątek główny kończy działanie, gdy nie ma już zadań. (4p)
3.  Wszystkie wątki przetwarzające czytają przypisane fragmenty i zapisują wczytane linie do list. (3p)
4.  Gdy wątek napotyka błąd, czeka na wszystkie aktualnie działające wątki, sprawdza numer linii błędu i drukuje go do stderr. Przetwarzanie zostaje zatrzymane, a program kończy działanie. (4p)
5.  Jeden wątek łączy wynikowe listy połączone i drukuje je linijka po linijce do stdout. (2p)
