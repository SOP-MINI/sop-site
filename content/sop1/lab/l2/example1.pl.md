---
title: "Zadanie testowe nr 1 z tematu Procesy, sygnały i deskryptory"
date: 2022-02-05T19:34:07+01:00
bookHidden: true
---

## Treść

Napisz program, który tworzy po jednym procesie potomnym dla każdego podanego parametru. Parametry mają być liczbami całkowitymi z przedziału \[0-9\]. Zatem proces potomny otrzymuje cyfrę "n" z parametrów oraz dodatkowo losuje sobie liczbę "s" z przedziału \[10-100\]KB. Proces potomny ma (niskopoziomowo) utworzyć plik o nazwie PID.TXT (PID zamieniamy na identyfikator procesu) zawierający tyle bloków o rozmiarze "s" złożonych tylko z cyfr "n" ile razy otrzyma SIGUSR1. Proces potomny kończy działanie poprawnie zamykając swój plik po upływie około 1 sekundy.

Proces rodzic przez 1 sekundę co 10 ms wysyła wszystkim swoim procesom potomnym SIGUSR1 po czym czeka na zakończenie procesów potomnych i sam się kończy.

## Oceniane etapy

1.  Tworzenie procesów potomnych z przekazaniem im stosownej cyfry, losowanie rozmiarów s oraz wypisanie n i s w procesach potomnych. Rodzic czeka na procesy potomne. *Jak pokazać*: uruchomić z parametrami 2 4 8 **(3p)**
2.  Procesy potomne tworzą pliki z jednym tylko blokiem danych. *Jak pokazać*: ls -ltr ; uruchomić z parametrami 2 4 8; ls -ltr **(3p)**
3.  Proces rodzic ma wysyłać sygnały tak jak w zadaniu, procesy potomne mają je zliczać i przy każdym odebraniu wypisywać na ekran aktualny stan licznika wraz ze swoim PIDem. Nadal zapisują tylko 1 blok i kończą działanie. *Jak pokazać:* uruchomić z parametrami 2 4 8 **(3p)**
4.  Usuwamy wszystkie niepotrzebne komunikaty ekranowe. Procesy potomne czekają na sygnał SIGUSR1 i za każdym razem gdy go dostaną dopisywać jeden blok do swojego pliku. Na tym etapie procesy potomne się nie kończą, program kończymy C-c. *Jak pokazać*: ls -ltr ; uruchomić z parametrami 2 4 8; C-c; ls -ltr **(3p)**
5.  Dodajemy czas życia potomka (1s), po odczekaniu na sygnał dopisujemy nie jeden blok a tyle aby ich ilość zgadzała się z licznikiem. *Jak pokazać*: ls -ltr ; uruchomić z parametrami 2 4 8; ls -ltr **(3p)**
6.  Dodajemy kod niezbędny do ochrony zapisu do pliku tak aby zapisywane rozmiary były zgodne z liczbą "s". *Jak pokazać:* pokazać kod odpowiedzialny za zapis do pliku. **(2p)**
