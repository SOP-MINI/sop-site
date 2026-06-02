---
title: "Zadanie testowe UDP"
bookHidden: true
---

## L8: Cluster Computing

Dziekan wydziału MiNI ogłosił radosną nowinę -- dzięki wysokiemu
wskaźnikowi ankietyzacji zajęć, udało się dostać grant i zakupić nowe
komputery do wydziałowego klastra EDEN. Pracownicy i doktoranci nie mogą
doczekać się, aby użyć nowych maszyn do przeprowadzania zaawansowanych
obliczeń, jak np. obliczanie liczby π. Jednak nowe komputery
wymagają systemu do zarządzania dostępem i kolejkowania zadań. Wszystkie
pieniądze z grantu zostały już wydane na nowe maszyny, dlatego dziekan
postanowił, że system zostanie napisany przez studentów w ramach
przedmiotu Systemy Operacyjne.

startowe definicje `sop-eden-init.c`:
{{< includecode "sop-eden-init.c" >}}

[bashowy skrypt do testowania rozwiązania]({{< resource sop-eden-test.sh >}})

## Etapy:

1. Stwórz serwer UDP przyjmujący wiadomości w następującym formacie:
   -   16 bajtów loginu (w razie potrzeby uzupełnione zerami) -- jeden z
       loginów w tablicy `LOGINS`
   -   8 bajtów polecenia (w razie potrzeby uzupełnione zerami) -- `RUN`,
       `EXIT`, `PAUSE`, `COMPUTE`, `LIST`, albo `GATHER`
   -   (opcjonalne) parametry polecenia
   
   W przypadku polecenia `COMPUTE` parametry to niepusty ciąg par
   4-bajtowych nieujemnych liczb całkowitych (`uint32_t`). Pozostałe
   komendy nie mają dodatkowych parametrów. Liczby są zapisane w network
   byte order.
   
   Wiadomość może mieć maksymalnie długość `MSG_MAX`. Program nasłuchuje na
   porcie podanym jako jego pierwszy parametr:
   
           ./sop-eden <port>
   
   W przypadku wiadomości o złym formacie, wypisuj na terminal deskryptywne
   informacje o błędzie (np. `error: unknown user <login>`,
   `error: wrong message length <length>`,
   `error: unknown command <command>`).
   
   W przypadku otrzymania poprawnej wiadomości, wypisz ją i jej parametry
   na terminal w formacie `<user>: <command> <parameters>`.
   
   Po otrzymaniu poprawnej komendy `EXIT`, zakończ czysto program.

2. Komenda `COMPUTE` oznacza zlecenie rozpoczęcia nowych zadań obliczania
   liczby π, używając dostarczonej w kodzie startowym funkcji
   `double compute_pi(const int count, const int* seed)`. Dla każdej pary
   liczb z wiadomości utwórz nowe zadanie. Pierwszy parametr oznacza liczbę
   próbek i nie może być większy niż 10 milionów (sprawdź to, w przypadku
   przekroczenia zakresu zignoruj to zadanie i wypisz komunikat o błędzie).
   Drugi parametr oznacza ziarno do generatora liczb losowych. Dodaj
   zadanie do kolejki zadań. Kolejka zadań powinna umożliwiać dodawanie w
   sposób nieblokujący dowolnej liczby zadań w czasie stałym oraz
   pobieranie ich w czasie stałym (*podpowiedź:* lista spełnia te
   wymagania).
   
   Po otrzymaniu polecenia `LIST`, wyślij w odpowiedzi wszystkie znajdujące
   się aktualnie w kolejce zadania należące do danego użytkownika. Jeśli
   nie zmieszczą się w jednej wiadomości (obowiązuje limit `MSG_MAX`)
   wyślij je w kilku kolejnych. Odpowiedzi wysyłaj na port o 1 wyższy niż
   polecenie -- np. jeśli polecenie przyszło na porcie '8000' wyślij
   odpowiedź na porcie '8001'.
   
   Dla każdego użytkownika trzymaj jego aktualne przybliżenie liczby π.

3. Zaimplementuj pulę wątków (o `THREADS` wątkach) obliczającą liczbę
   π. Każdy wątek z puli pobiera wiadomość z początku kolejki (zadbaj o
   odpowiednią synchronizację) i oblicza maksymalnie 1000 próbek. Jeżeli
   próbek jest więcej po zakończeniu obliczeń zwróć zadanie (pomniejszone o
   przeliczoną liczbę próbek) na koniec kolejki.
   
   Uzyskany wynik uśrednij (pamiętaj o odpowiedniej wadze!) z przybliżeniem
   danego użytkownika.
   
   Po otrzymaniu polecenia `GATHER` wyślij w odpowiedzi aktualne
   przybliżenie π danego użytkownika jako wiadomość tekstową -- 12
   miejsc po przecinku.
   
4. Po otrzymaniu polecenia `PAUSE` pula wątków powinna nie obliczać zadań
   danego użytkownika aż do otrzymania polecenia `RUN`. W przypadku
   otrzymania `PAUSE` gdy aktualny użytkownik już jest zapauzowany, albo
   `RUN` gdy nie jest, wypisz na terminal, oraz w wiadomości zwrotnej
   `<username>: already paused/running`.
   
   Uwaga: zatrzymanie zadań wszystkich użytkowników nie może prowadzić do
   busy pollingu!

[Przykładowe rozwiązanie pierwszego etapu]({{< resource sop-eden.c >}})
