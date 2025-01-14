# Treść

Twoim zadaniem jest przygotowanie zestawu wątków oczekujących na funkcje
do wykonania. W przeciwieństwie do tworzenia wątków na żądanie pula
wątków alokuje wątki robocze (tzw. *worker threads*) na początku
uruchomienia programu i czekają one na sygnał wykonania pracy. Następnie
z innych wątków można przekazać funkcję do wykonania przez jeden wątek z
puli.

Na tym laboratorium pula wątków powinna implementować 3 funkcje:

1.  `initialize` -- w ramach puli wątków tworzone jest `N` wątków.

2.  `dispatch` -- do wątku oczekującego na prace należy przekazać
    wskaźnik na funkcję o sygnaturze `void (*work_function)(void*)` oraz
    argumenty wywołania. Jeżeli nie ma wolnego wątku do wykonania pracy
    funkcja `dispatch` powinna blokować do czasu znalezienia wolnego
    wątku.

3.  `cleanup` -- funkcja powinna czekać na skończenie wszystkich *worker
    threads* i wykonać funkcje `join`. Niedopuszczalne jest przerwanie
    pracy wątku, który przyjął dyspozycję do wykonania funkcji.

Pracę wątku roboczego można opisać w następujących krokach:

1.  Czekaj na zmiennej warunkowej na pracę zleconą z innego wątku (jedna
    zmienna warunkowa dzielona między wszystkie wątki robocze).

2.  Po przejęciu funkcji roboczej i argumentów wykonania poinformuj, że
    to się stało. W przeciwnym wypadku uzyskamy wyścig, który spowoduje
    niestabilną pracę puli wątków.

3.  Wywołaj funkcję z otrzymanymi argumentami.

4.  Powtarzaj do czasu jak nie została wywołana funkcja `cleanup` na
    puli wątków.

W celu użycia puli wątków została zaimplementowana funkcja obliczania
pola koła metodą monte carlo. Procedura składa się z dwóch dwóch części:
losowania punktów oraz uśredniania uzyskanych wyników. Implementacja
przy użyciu naszej puli wątków realizuje powyższe części jako dwie różne
funkcje robocze: `circle_monte_carlo` oraz `accumulate_monte_carlo`.
Funkcja `accumulate_monte_carlo` musi rozpocząć uśrednianie wyników
dopiero po zakończeniu wszystkich `circle_monte_carlo`. Twoim zadaniem w
ramach tego laboratorium będzie zsynchronizowanie tych dwóch funkcji
(bariera nadaje się idealnie do tego celu) oraz przekazanie do nich
prawidłowych danych.

Dla ułatwienia testowania programu w ramach kodu startowego masz do
dyspozycji interfejs CLI: 
```
enter command
1. circle <n> <r> <s>
2. hello <n>
3. exit
```
Umożliwia on wybranie polecenia odpowiednim numerem i przekazanie
argumentów po spacji. Pierwszym poleceniem jest uruchomienie procedury
obliczającej pole koła o promieniu `r` wykorzystując `n` wątków
losujących w sumie `s` próbek. Drugie polecenie uruchamia `n` funkcji
testowych wypisujących `Hello world from worker n!`. Przy wybraniu
trzeciej opcji program się zamyka czekając na zakończenie wszystkich
zleconych poleceń.

**Podpowiedź**: Dla ułatwienia zadania ważne miejsca w kodzie, gdzie w
danym etapie należy coś dopisać zostały oznaczone komentarzem
`TODO STAGE-X`.

Etapy:

- Zaimplementuj funkcję `initialize`. Utworzonę watki wypisują swój `TID`
i czekają na zmiennej warunkowej. 
- Zaimplementuj funkcje `dispatch`. Opcja `hello` powinna działać na tym etapie.
- Zaimplementuj przekazanie
danych do obliczenia pola koła oraz synchronizację z wątkami losującymi.
Opcja `circle` powinna działać na tym etapie.
- Zaimplementuj funkcje
`cleanup`. 
- Program powinien czekać na zakończenie obliczeń i prawidłowo
kończyć wszystkie wątki przy wybraniu opcji `exit`.

# Kod początkowy

- [sop1l4e3.zip](/files/sop1l4e3.zip)
