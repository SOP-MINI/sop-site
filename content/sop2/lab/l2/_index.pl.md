---
title: "L2 - Kolejki POSIX"
date: 2022-02-10T10:00:00+01:00
weight: 20
---

# Tutorial 6 - kolejki POSIX


Temat kolejek jest wyraźnie mniej obszerny niż temat łączy typu pipe/fifo, a ten tutorial jest również krótszy. Kolejki POSIX nie mają problemów z równoczesnym zapisem i odczytem, gwarantują atomowość zapisu nawet dla dużych wiadomości, posiadają wyznaczone granice rekordów oraz obsługują priorytety. Wszystkie te cechy sprawiają, że temat ten jest stosunkowo łatwy, a ewentualne trudności ograniczają się głównie do obsługi notyfikacji o zdarzeniach w kolejce.

Głównym tematem tych zajęć są kolejki komunikatów POSIX, ale zadania będą również opierać się na elementach już wcześniej ćwiczonych, takich jak wątki, procesy i sygnały.

Przed przystąpieniem do wejściówki i zadania prosimy o zapoznanie się z poniższymi materiałami. Nie ma potrzeby czytania całych stron man; wystarczy zrozumieć, co dana funkcja robi, jakie przyjmuje parametry i jak działają jej flagi. **Materiały przygotowawcze z poprzednich zajęć nadal obowiązują!**
Poniżej podane są komendy do wywołania w konsoli:

- `man 7 mq_overview`
- `man 3p mq_open`
- `man 3p mq_close`
- `man 3p mq_unlink`
- `man 3p mq_getattr`
- `man 3p mq_setattr`
- `man 3p mq_send`
- `man 3p mq_receive`
- `man 3p mq_notify`
- `man 7 sigevent`

Dołączenie biblioteki `librt` jest wymagane podczas linkowania programu używającego kolejek POSIX.
Jeśli otwieramy istniejącą kolejkę, może zawierać ona jakieś dane. Nie należy zakładać, że jest pusta. Aby zapewnić, że kolejka jest pusta, można ją usunąć przed ponownym utworzeniem.

## Przykład: Bingo!
Napisz program, który symuluje prostą wersję gry w BINGO. Losującym liczby jest proces rodzic, a graczami - jego procesy potomne. Komunikacja między nimi odbywa się za pomocą kolejek komunikatów POSIX. Proces rodzic tworzy `n` procesów potomnych (`0 < n < 100`, gdzie `n` to parametr programu) oraz dwie kolejki komunikatów. Pierwsza kolejka `pout` służy do przekazywania co sekundę losowanych liczb z przedziału `[0-9]` do procesów potomnych, druga `pin` do odbierania od procesów potomnych informacji o wygranej lub zakończeniu gry.

Procesy potomne na początku losują swoją liczbę oczekiwaną (wygrywającą) `E` (`[0-9]`) oraz ilość liczb, jakie odczytają `N` (`[0-9]`) z kolejki. Następnie cyklicznie konkurują o dostęp do danych w kolejce `pout` - jedna wysłana liczba może być odebrana tylko przez jeden proces, a nie przez wszystkie naraz. Procesy potomne porównują odczytaną z `pout` liczbę do swojej liczby `E` i, jeśli jest to ta sama liczba, to poprzez drugą kolejkę `pin` przekazują informację o jej wartości, a następnie kończą działanie. Po wykonaniu `N` sprawdzeń proces potomny przed zakończeniem wysyła przez kolejkę `pin` swój numer porządkowy (z przedziału `1..n`).

Proces rodzica cały czas, asynchronicznie względem wysyłania liczb, ma odbierać komunikaty z `pin` i wyświetlać odpowiednie treści na ekranie. Gdy wszystkie procesy potomne zakończą działanie, proces rodzic również kończy działanie i usuwa kolejki.

**Rozmiar komunikatów w kolejce jest ograniczony do 1 bajta!**

### Rozwiązanie
{{< includecode "mq_example.c" >}}

Zwróć uwagę na użycie wskaźnika przesyłanego z sygnałem. Prototyp funkcji obsługi sygnału zawiera dodatkowy parametr `siginfo_t*`, a podczas instalowania obsługi użyto flagi SA_SIGINFO, aby przesłanie wskaźnika było możliwe. Warto zauważyć, że nie wysyłamy sami takiego sygnału. Dostajemy go jako notyfikację o zdarzeniu w kolejce. Funkcją `kill` nie można wysłać wskaźnika. Można to zrobić jedynie za pomocą funkcji `sigqueue`.
Do przesyłania liczb wykorzystano typ uint8_t (`stdint.h`), czyli jednobajtową liczbę bez znaku o zakresie od 0 do 255. Typy całkowite o precyzyjnie określonym rozmiarze (np. `int32_t`) są bardziej przenośne niż typy bazowe, takie jak np. int, który może mieć różne zakresy poprawnych wartości na różnych platformach.
W programie występuje obsługa sygnałów, więc konieczna jest ochrona przed przerwaniami sygnałem, np. za pomocą `TEMP_FAILURE_RETRY`. Makra te zostały dodane w całym kodzie, chociaż zagrożony przerwaniem jest tylko kod procesu rodzica, bo tylko on otrzymuje notyfikacje o stanie kolejki. Dodawanie zabezpieczeń przed przerwaniem funkcji przez obsługę sygnału nie spowalnia kodu, a czyni go bardziej przenośnym.
Ustawienia notyfikacji na kolejce są jednokrotne, czyli po odebraniu notyfikacji nie spodziewamy się kolejnych, o ile nie wywołamy `mq_notify` ponownie. Notyfikacja zadziała tylko wtedy, gdy kolejka przejdzie ze stanu pustego w niepusty. Tylko jeden proces może być zarejestrowany do otrzymywania notyfikacji.

W kodzie procesu rodzica brak jest ochrony przed przerwaniem w jednym z wywołań, w którym? Dlaczego tam ochrona taka nie jest konieczna? 
{{< answer >}} Chodzi o wywołanie `mq_receive` w funkcji obsługi sygnału. Nie spodziewamy się przerwania funkcji obsługi sygnału obsługiwanym sygnałem, gdyż domyślnie na czas tej obsługi sygnał ten jest blokowany. {{< /answer >}}

Jak zrealizowane jest zliczanie procesów potomnych?
{{< answer >}} Zliczane są skuteczne wywołania `waitpid` w funkcji obsługi `SIGCHLD`, nie sygnały `SIGCHLD`, ponieważ te mogą się sklejać. Licznik aktywnych procesów potomnych jest zmienną globalną. {{< /answer >}}

Czemu podczas odbierania sygnału najpierw instalujemy/restartujemy notyfikację, a dopiero później czytamy z kolejki?
{{< answer >}} Gdyby było odwrotnie, to po przeczytaniu wiadomości z kolejki (możliwe, że kilku) a tuż przed założeniem notyfikacji mogłoby się coś w kolejce pojawić. Ponieważ powiadomienia są wysyłane tylko, gdy w pustej kolejce pojawi się coś nowego, nie mielibyśmy szansy na odczyt, bo nasza kolejka w momencie instalowania notyfikacji już coś zawierałaby. Jeśli jednak zainstalujemy notyfikację najpierw, a potem przeczytamy zawartość kolejki, mamy pewność, że kolejka jest pusta z włączoną notyfikacją. Jeśli teraz coś się w niej znajdzie, to na pewno dostaniemy notyfikację. {{< /answer >}}

Czemu jedna z kolejek jest w trybie nieblokującym?
{{< answer >}} Kolejka `pin` jest w stanie nieblokującym ze względu na powyższy punkt. Gdyby mogła się blokować, to po odczytaniu ostatniej wiadomości program zablokowałby się wewnątrz asynchronicznej obsługi sygnału, co byłoby poważnym błędem. {{< /answer >}}

Jak są wykorzystane priorytety wiadomości i jak to się ma do limitu 1 bajta dla długości wiadomości?
{{< answer >}} W tym programie priorytety nie służą do ustalania kolejności, ale jako wyznaczniki typu informacji. Zmusza nas do tego krótki, jednobajtowy rozmiar wiadomości. Trudno (choć się da) zakodować w nim więcej niż samą liczbę. Informacja, czy chodzi o wygraną czy zakończenie gry, jest zawarta właśnie w priorytecie. {{< /answer >}}

W ramach dalszych ćwiczeń można przerobić ten program, aby notyfikacje odbywały się nie przez sygnał, a przez wątek.
Można również uprościć program, zastępując notyfikacje wątkiem dedykowanym tylko do odczytu z kolejki. Wykonaj taką modyfikację jako ćwiczenie.
Spora część logiki programu "wylądowała" w funkcji obsługi sygnału, co było możliwe, ponieważ nie ma zależności między kodem wysyłającym liczby a tym odbierającym notyfikacje. Jednak zadanie łatwo można skomplikować, aby taka zależność istniała. Jako ćwiczenie przenieś całą logikę związaną z odbiorem wiadomości do kodu właściwego rodzica (czyli poza asynchroniczne wywołanie funkcji obsługi sygnału).

## Przykładowe zadania

Wykonaj przykładowe zadania. Podczas laboratorium będziesz miał więcej czasu oraz dostępny startowy kod, jeśli jednak wykonasz poniższe zadania w przewidzianym czasie, to znaczy że jesteś dobrze przygotowany do zajęć.

- [Zadanie 1]({{< ref "/sop2/lab/l2/example1" >}}) ~90 minut
- [Zadanie 2]({{% ref "/sop2/lab/l2/example2" %}}) ~120 minut
- [Zadanie 3]({{% ref "/sop2/lab/l2/example3" %}}) ~120 minut
- [Zadanie 4]({{< ref "/sop2/lab/l2/example4" >}}) ~150 minut

## Kody źródłowe z treści tutoriala
{{% codeattachments %}}
