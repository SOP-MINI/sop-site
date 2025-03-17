---
title: "L2 - Kolejki POSIX"
date: 2022-02-10T10:00:00+01:00
weight: 20
---

# Tutorial 6 - kolejki POSIX

{{< hint info >}}
Ten tutorial zawiera wyjaśnienia działania funkcji wymaganych na laboratoriach oraz ich parametrów.
Jest to jednak wciąż zbiór jedynie poglądowy najważniejszych informacji -- 
należy **koniecznie przeczytać wskazane strony manuala**, aby dobrze poznać i zrozumieć wszystkie szczegóły.

{{< /hint >}}

## Operacje na kolejkach POSIX

Temat kolejek jest wyraźnie mniej obszerny niż temat łączy typu pipe/FIFO, a ten tutorial jest również krótszy. Kolejki POSIX nie mają problemów z równoczesnym zapisem i odczytem, gwarantują atomowość zapisu nawet dla dużych wiadomości, posiadają wyznaczone granice rekordów oraz obsługują priorytety. Wszystkie te cechy sprawiają, że temat ten jest stosunkowo łatwy, a ewentualne trudności ograniczają się głównie do obsługi notyfikacji o zdarzeniach w kolejce.

Głównym tematem tych zajęć są kolejki komunikatów POSIX, ale zadania będą również opierać się na elementach już wcześniej ćwiczonych, takich jak wątki, procesy i sygnały.

Manual `man 7 mq_overview` zawiera opis działania kolejki. Omawiane funkcje są zdefiniowane w pliku nagłówkowym `<mqueue.h>`, a interfejs został zaprojektowany tak, aby z kolejek można było korzystać na zasadzie podobnej do korzystania z plików, warto zauważać analogie w definicjach i użyciu kolejno omawianych funkcji.

Kolejki POSIX są kolejkami **priorytetowymi**, zatem kolejność komunikatów zależy najpierw od priorytetu wiadomości, a następnie od kolejności wysłania.

### Tworzenie, otwieranie i usuwanie kolejek

Kolejki w systemie są identyfikowane po nazwie (definiowanej przez programistę) `/<name>`, np. `/queue`. Do utworzenia i otwarcia kolejki służy funkcja `mq_open` (`man 3p mq_open`):

```
mqd_t mq_open(const char *name, int oflag, ...);
```
- `name` jest tutaj nazwą kolejki, 
- `oflag` jest maską bitową definiującą sposób dostępu do kolejki. Mamy do dyspozycji następujące opcje:
  - `O_RDONLY`, `O_WRONLY`, `O_RDWR` -- otwarcie kolejki w trybie odczytu/zapisu/oba,
  - `O_CREAT` -- tworzenie kolejki; ta opcja wymaga kolejnych dwóch argumentów: `mode_t mode`, określający uprawnienia dostępu, oraz `mq_attr* attr`, strukturę zawierającą 4 pola:
    - `mq_flags`, będąca maską bitową (POSIX definiuje tylko obecność (lub nie) flagi `O_NONBLOCK`, inne zależne od implementacji),
    - `mq_maxmsg` -- maksymalna liczba wiadomości w kolejce,
    - `mq_msgsize` -- maksymalny rozmiar kolejki,
    - `mq_curmsgs` -- aktualna liczba wiadomości w kolejce (ignorowana przy tworzeniu kolejki).

    Jako `attr` można także podać `NULL`, wtedy kolejka zostanie utworzona z domyślnymi ustawieniami.
  - `O_EXCL` -- może być obecna tylko z `O_CREAT`, powoduje błąd, gdy kolejka o podanej nazwie już istnieje,
  - `O_NONBLOCK` -- ustawia odczyt/zapis nieblokujący.

Funkcja zwraca deskryptor kolejki (typu `mqd_t`) lub `(mqd_t)-1` w przypadku błędu.
Nieużywany deskryptor zamykamy funkcją `mq_close` (`man 3p mq_close`). 

Kolejkę o podanej nazwie można natomiast usunąć funkcją `mq_unlink`. Należy jednak uważać na sytuację, w której chcemy usunąć kolejkę niezamkniętą przez pewien inny proces. Jak można przeczytać w manualu (`man 3p mq_unlink`), wywołanie usunięcia kolejki powoduje tylko zwolnienie jej nazwy (np. do ponownego utworzenia). Sama kolejka jest rzeczywiście usuwana dopiero, gdy wszystkie związane z nią deskryptory zostaną zamknięte. W zależności od implementacji, `mq_unlink` może wstrzymać dalsze działanie programu, aż inne procesy zamkną odpowiednie deskryptory. W Linuksie jednak (`man 3 mq_unlink`), funkcja ta zwraca natychmiastowo. Nawet w takiej sytuacji, stworzona kolejka o tej samej nazwie będzie inną kolejką od tej starszej. 
### Atrybuty kolejki

Kolejka przechowuje swoje atrybuty w strukturze `mq_attr`, omówionej w poprzedniej sekcji (patrz wyjaśnienie funkcji `mq_open`, flaga `O_CREAT`). Pobrać atrybuty kolejki możemy, używając funkcji `mq_getattr` (`man 3p mq_getattr`), której definicja wygląda następująco:

```
int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat);
```
- `mqdes` to deskryptor otwartej kolejki,
- `mqstat` to wskaźnik do już zaalokowanej zmiennej typu `struct mq_attr`, której pola wypełni omawiana funkcja.

Atrybuty kolejki ustawiamy za pomocą funkcji `mq_setattr` (`man 3p mq_setattr`):

```
int mq_setattr(mqd_t mqdes, const struct mq_attr *restrict mqstat, struct mq_attr *restrict omqstat);
```
- `mqdes` to deskryptor otwartej kolejki,
- `mqstat` to wskaźnik do zmiennej typu `struct mq_attr` z atrybutami, które chcemy ustawić,
- `omqstat` to wskaźnik do już zaalokowanej zmiennej typu `struct mq_attr`. Pola tej struktury zostaną wypełnione wartościami sprzed wywołania `mq_setattr`. Można też jako ten argument podać `NULL`.

**WAŻNE**: Funkcja `mq_setattr` ustawia wartość tylko pola `mq_flags`. Wartości pozostałych pól (`mq_maxmsg`, `mq_msgsize` i `mq_curmsgs`) są niezmienne (można je ustawić tylko w momencie tworzenia kolejki) i są ignorowane przez `mq_setattr`. W praktyce więc, zgodnie z POSIX-em, mamy możliwość zmiany tylko trybu zapisu/odczytu na blokujący lub nie.

Obie funkcje, standardowo, zwracają `0` w razie sukcesu i `-1` w razie niepowodzenia.

### Wysyłanie i odbieranie sygnałów

Do tej pory, za czytanie i pisanie do różnych obiektów (pliki, łącza) odpowiadały funkcje `read` i `write`. Kolejki używają zaś własnych funkcji: `mq_send` i `mq_receive` (`man 3p mq_send`, `man 3p mq_receive`). Przyjrzyjmy się definicjom obu funkcji:

```
int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio);
```
- `mqdes` to deskryptor otwartej kolejki,
- `msg_ptr` to wskaźnik do bufora z wiadomością do wysłania (dla `mq_send`) lub do wypełnienia odbieraną wiadomością (dla `mq_receive`),
- `msg_len` to rozmiar bufora `msg_ptr` w bajtach (nie może być większy niż wartość atrybutu `mq_msgsize` w przypadku `mq_send`, a dla `mq_receive` musi być większy lub równy `mq_msgsize`),
- `msg_prio`:
  - dla `mq_send` jest to priorytet wiadomości (musi być mniejszy niż wartość stałej `MQ_PRIO_MAX` -- POSIX zapewnia, że `MQ_PRIO_MAX >= 32`),
  - dla `mq_receive`, jest to wskażnik na zmienną, do której zostanie zapisany priorytet odbieranej wiadomości (można też podać `NULL`, jeśli priorytet nie jest nam potrzebny).

Jeśli próbujemy odczytać dane z pustej kolejki lub zapisać dane do pełnej kolejki i kolejka jest w trybie nieblokującym, funkcje zwrócą `-1`, a `errno` zostanie ustawione na `EAGAIN`.
W trybie blokującym, funkcję czekają, aż możliwe będzie odebranie lub wysłanie komunikatu.

Jeśli wysłanie komunikatu funkcją `mq_send` się powiedzie, funkcja zwróci `0`. Funkcja `mq_receive`, w razie odebrania komunikatu, zwraca długość tego komunikatu w bajtach.

W trybie blokującym, możemy także ustawić maksymalny czas oczekiwania na wiadomość lub na możliwość jej wysłania. Służą do tego funkcje `mq_timedsend` i `mq_timedreceive`:

```
int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio, const struct timespec *abstime);
ssize_t mq_timedreceive(mqd_t mqdes, char *restrict msg_ptr, size_t msg_len, unsigned *restrict msg_prio, const struct timespec *restrict abstime);
```
które zawierają dodatkowy argument `abstime`, będący wskaźnikiem na strukturę typu `timespec` (z pliku nagłówkowego `<time.h>`), wyznaczającą maksymalny czas oczekiwania na wiadomość (`mq_timedreceive`) lub na miejsce w kolejce (`mq_timedsend`). Obie funkcje zachowują się tak jak ich odpowiedniki bez podawanego czasu, z wyjątkiem sytuacji, w której przekroczymy podany czas oczekiwania. Wtedy, obie funkcje zwrócą `-1`, a `errno` zostanie ustawione na `ETIMEDOUT`.

Gdy kolejka jest w trybie nieblokującym, funkcje `mq_timed(send|receive)` zachowują się dokładnie tak samo jak `mq_(send|receive)`.

### Powiadamianie

POSIX przewiduje możliwość asynchronicznego powiadamiania procesów o nadchodzących wiadomościach do pustej kolejki. Zarejestrować proces w celu powiadomienia możemy używając funkcji `mq_notify` (`man 3p mq_notify`):

```
int mq_notify(mqd_t mqdes, const struct sigevent *notification);
```
- `mqdes` to desktyptor otwartej kolejki,
- `notification` to wskaźnik na strukturę `sigevent` (plik nagłówkowy `<signal.h>`), mówiącą, w jaki sposób powiadomienie powinno zostać zrealizowane. 

Strukturę `sigevent` powinniśmy wypełnić w następujący sposób, w zależności od typu powiadomienia (`man 7 sigevent`):
- Dla powiadomienia sygnałem (tylko sygnały *realtime*):
  - ustawiamy `sigev_notify` na `SIGEV_SIGNAL`,
  - ustawiamy `sigev_signo` na sygnał, który chcemy otrzymać (np. `SIGRTMIN`),
  - ustawiamy `sigev_value.sival_int` lub `sigev_value.sival_ptr` na wartość, którą chcemy otrzymać w argumencie handlera sygnału (odpowiednio: liczba całkowita lub adres w pamięci).

  Ponadto:
  - przy ustawianiu handlera sygnałów funkcją `sigaction`, w strukturze `sigaction` podanej jako argument funkcji, należy ustawić pole `sa_flags` na `SA_SIGINFO` (patrz funkcja `sethandler` z rozwiązania poniższego zadania),
  - przekazane wartości (w `sigev_value`) są przekazywane do handlera w polu `si_value` struktury `siginfo_t` (patrz funkcja `mq_handler` z rozwiązania zadania).
- Dla powiadomienia wątkiem:
  - ustawiamy `sigev_notify` na `SIGEV_THREAD`,
  - ustawiamy `sigev_notify_function` na wskaźnik do funkcji przyjmującej `union sigval` (typ pola `sigev_value`) i zwracającej `void`,
  - ustawiamy `sigev_value.sival_int` lub `sigev_value.sival_ptr` na wartość, którą chcemy otrzymać w argumencie podanej wyżej funkcji (odpowiednio: liczba całkowita lub adres w pamięci).
  - opcjonalnie ustawiamy parametry wątku w `sigev_notify_attributes`.

Jeśli chcemy wyrejestrować proces z powiadomienia, należy wywołać `mq_notify` i jako `notification` przekazać `NULL`. Przekazanie `NULL`, gdy proces nie jest zarejestrowany, spowoduje zwrócenie wartości `-1` i ustawienie `errno` na `EINVAL` (czyli błąd).

**WAŻNE**: 
- Ustawienia powiadomień na kolejce są jednokrotne, czyli po odebraniu notyfikacji nie spodziewamy się kolejnych. Jeśli chcemy otrzymać kolejne powiadomienie, musimy wywołać `mq_notify` ponownie. 
- Notyfikacja zadziała tylko wtedy, gdy kolejka przejdzie ze stanu pustego w niepusty. 
- Tylko jeden proces może być zarejestrowany do otrzymywania notyfikacji (inaczej `mq_notify` zwróci `-1` i ustawi `errno` na `EBUSY`).

Zwróć uwagę na realizację powiadamiania sygnałem w Zadaniu 1. Powiadamianie wątkiem jest natomiast zaimplementowane w rozwiązaniu Zadania 2.

### Uwagi
1. Dołączenie biblioteki `librt` jest wymagane podczas linkowania programu używającego kolejek POSIX.
2. Jeśli otwieramy istniejącą kolejkę, może zawierać ona jakieś dane. Nie należy zakładać, że jest pusta. Aby zapewnić, że kolejka jest pusta, można ją usunąć przed ponownym utworzeniem.

### Zadanie 1

Napisz program, który symuluje prostą wersję gry w *bingo*. Losującym liczby jest proces rodzic, a graczami -- jego procesy potomne. Komunikacja między nimi odbywa się za pomocą kolejek komunikatów POSIX. Proces rodzic tworzy `n` procesów potomnych (`0 < n < 100`, gdzie `n` to parametr programu) oraz dwie kolejki komunikatów. Pierwsza kolejka `pout` służy do przekazywania co sekundę losowanych liczb z przedziału `[0,9]` do procesów potomnych, druga `pin` do odbierania od procesów potomnych informacji o wygranej lub zakończeniu gry.

Procesy potomne na początku losują swoją liczbę oczekiwaną (wygrywającą) `E` (z przedziału `[0,9]`) oraz liczbę `N` liczb, jakie odczytają z kolejki (także z przedziału `[0,9]`). Następnie cyklicznie konkurują o dostęp do danych w kolejce `pout` -- jedna wysłana liczba może być odebrana tylko przez jeden proces, a nie przez wszystkie naraz. Procesy potomne porównują odczytaną z `pout` liczbę do swojej liczby `E` i, jeśli jest to ta sama liczba, to poprzez drugą kolejkę `pin` przekazują informację o jej wartości, a następnie kończą działanie. Po wykonaniu `N` sprawdzeń proces potomny przed zakończeniem wysyła przez kolejkę `pin` swój numer porządkowy (z przedziału `[1,n]`).

Proces rodzica cały czas, asynchronicznie względem wysyłania liczb, ma odbierać komunikaty z `pin` i wyświetlać odpowiednie treści na ekranie. Gdy wszystkie procesy potomne zakończą działanie, proces rodzic również kończy działanie i usuwa kolejki.

**UWAGA**: W tym zadaniu, rozmiar komunikatów w kolejce powinien być ograniczony do 1 bajta!

#### Rozwiązanie zadania

Nowe strony z manuala:

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

{{< includecode "l6-1.c" >}}

#### Uwagi i pytania

- Zwróć uwagę na użycie wskaźnika przesyłanego z sygnałem. Prototyp funkcji obsługi sygnału zawiera dodatkowy parametr `siginfo_t*`, a podczas instalowania obsługi użyto flagi SA_SIGINFO, aby przesłanie wskaźnika było możliwe. Warto zauważyć, że nie wysyłamy sami takiego sygnału. Dostajemy go jako notyfikację o zdarzeniu w kolejce. Funkcją `kill` nie można wysłać wskaźnika. Można to zrobić jedynie za pomocą funkcji `sigqueue`.

- Do przesyłania liczb wykorzystano typ uint8_t (`stdint.h`), czyli jednobajtową liczbę bez znaku o zakresie od 0 do 255. Typy całkowite o precyzyjnie określonym rozmiarze (np. `int32_t`) są bardziej przenośne niż typy bazowe, takie jak np. int, który może mieć różne zakresy poprawnych wartości na różnych platformach.

- W programie występuje obsługa sygnałów, więc konieczna jest ochrona przed przerwaniami sygnałem, np. za pomocą `TEMP_FAILURE_RETRY`. Makra te zostały dodane w całym kodzie, chociaż zagrożony przerwaniem jest tylko kod procesu rodzica, bo tylko on otrzymuje notyfikacje o stanie kolejki. Dodawanie zabezpieczeń przed przerwaniem funkcji przez obsługę sygnału nie spowalnia kodu, a czyni go bardziej przenośnym.

- W kodzie procesu rodzica brak jest ochrony przed przerwaniem w jednym z wywołań, w którym? Dlaczego tam ochrona taka nie jest konieczna? 
{{< answer >}} Chodzi o wywołanie `mq_receive` w funkcji obsługi sygnału. Nie spodziewamy się przerwania funkcji obsługi sygnału obsługiwanym sygnałem, gdyż domyślnie na czas tej obsługi sygnał ten jest blokowany. {{< /answer >}}

- Jak zrealizowane jest zliczanie procesów potomnych?
{{< answer >}} Zliczane są skuteczne wywołania `waitpid` w funkcji obsługi `SIGCHLD`, nie sygnały `SIGCHLD`, ponieważ te mogą się sklejać. Licznik aktywnych procesów potomnych jest zmienną globalną. {{< /answer >}}

- Czemu podczas odbierania sygnału najpierw instalujemy/restartujemy notyfikację, a dopiero później czytamy z kolejki?
{{< answer >}} Gdyby było odwrotnie, to po przeczytaniu wiadomości z kolejki (możliwe, że kilku) a tuż przed założeniem notyfikacji mogłoby się coś w kolejce pojawić. Ponieważ powiadomienia są wysyłane tylko, gdy w pustej kolejce pojawi się coś nowego, nie mielibyśmy szansy na odczyt, bo nasza kolejka w momencie instalowania notyfikacji już coś zawierałaby. Jeśli jednak zainstalujemy notyfikację najpierw, a potem przeczytamy zawartość kolejki, mamy pewność, że kolejka jest pusta z włączoną notyfikacją. Jeśli teraz coś się w niej znajdzie, to na pewno dostaniemy notyfikację. {{< /answer >}}

- Czemu jedna z kolejek jest w trybie nieblokującym?
{{< answer >}} Kolejka `pin` jest w stanie nieblokującym ze względu na powyższy punkt. Gdyby mogła się blokować, to po odczytaniu ostatniej wiadomości program zablokowałby się wewnątrz asynchronicznej obsługi sygnału, co byłoby poważnym błędem. {{< /answer >}}

- Jak są wykorzystane priorytety wiadomości i jak to się ma do limitu 1 bajta dla długości wiadomości?
{{< answer >}} W tym programie priorytety nie służą do ustalania kolejności, ale jako wyznaczniki typu informacji. Zmusza nas do tego krótki, jednobajtowy rozmiar wiadomości. Trudno (choć się da) zakodować w nim więcej niż samą liczbę. Informacja, czy chodzi o wygraną czy zakończenie gry, jest zawarta właśnie w priorytecie. {{< /answer >}}

- Spora część logiki programu "wylądowała" w funkcji obsługi sygnału, co było możliwe, ponieważ nie ma zależności między kodem wysyłającym liczby a tym odbierającym notyfikacje. Jednak zadanie łatwo można skomplikować, aby taka zależność istniała. Jako ćwiczenie przenieś całą logikę związaną z odbiorem wiadomości do kodu właściwego rodzica (czyli poza asynchroniczne wywołanie funkcji obsługi sygnału).

### Zadanie 2: powiadamianie wątkiem

Napisz program, który symuluje rozmowy na rzymskim forum.
Zdefiniuj stałą `CHILD_COUNT`.
Proces rodzic tworzy `CHILD_COUNT` kolejek, otwiera je w trybie nieblokującym i uruchamia `CHILD_COUNT` dzieci.
Każdemu dziecku przekazuje jego wygenerowane w dowolny sposób imię.
Każde dziecko to inny obywatel na forum.
Obywatele wysyłają sobie losowo wiadomości z ich imionami, a otrzymując je wypisują treść poprzedzoną swoim imieniem.
Po każdym wysłaniu śpią losową ilość czasu, a po wysłaniu `ROUNDS` (stała) wiadomości kończą pracę.
Każdy obywatel czeka na wiadomości poprzez powiadamianie wątkiem.

#### Rozwiązanie zadania

{{< includecode "l6-2.c" >}}

#### Uwagi i pytania

- Dlaczego wywołując `mq_receive` przerywamy pętlę przy błędzie `EAGAIN`?
{{< answer >}} Błąd ten oznacza, że w kolejce nie ma już niczego więcej do przeczytania. {{< /answer >}}

- Dlaczego w `child_function` wywołujemy funkcję `handle_messages`, a nie tylko `register_notification`?
{{< answer >}} Bo jeśli w kolejce już są jakieś wiadomości od innych dzieci, to powiadomienie nigdy by nie zostało wysłane. Musimy więc przeczytać wiadomości w głównym wątku. {{< /answer >}}

- Proces rodzica zamyka wszystkie deskryptory kolejek zaraz po utworzeniu dzieci. Dlaczego nie stanowi to dla nich problemu?
{{< answer >}} Ponieważ dziecko dziedziczy kopię danych w rodzicu, w tym kopię deskryptorów. Oznacza to, że jeśli rodzic zamknie swoje deskryptory, to deskryptory dzieci nadal pozostają otwarte. {{< /answer >}}

- Dlaczego procesy dzieci nie zamykają kolejek na sam koniec?
{{< answer >}} Jest to jeden z bardzo rzadkich przypadków, gdzie sprzątanie na koniec działania programu byłoby błędem! Kiedy zamykamy kolejki, to usuwane są powiadomienia, więc nie wystartują nowe wątki obsługi powiadomień. Co jednak, gdybyśmy zamknęli deskryptory kolejek w trakcie działania wątku powiadamiającego? Próbowałby on wtedy wywołać `mq_notify` lub `mq_receive` na niepoprawnych deskryptorach. Kolejki muszą więc być otwarte do samego końca działania programu. Deskryptory zostaną zamknięte przez jądro systemu przy wyjściu z programu, więc nie jest to stricte błąd. {{< /answer >}}

- Czy w powyższej sytuacji nie możemy użyć jakiejś struktury synchronizacyjnej, aby uniknąć takiego impasu?
{{< answer >}} Niestety nie możemy. Gdybyśmy tak zrobili, to problem byłby z samą strukturą. Należałoby ją na koniec działania programu zniszczyć, ale co jeśli w tym czasie działa jeszcze wątek obsługi powiadomień? Będzie wtedy próbował użyć zniszczonej struktury. {{< /answer >}}

- Czy nie wystarczyłoby zaczekać, aż wątek obsługi sygnału zakończy pracę, zanim zamknęlibyśmy kolejki?
{{< answer >}} Nie możemy czekać na ten wątek. Nie mamy jego TID. Nawet gdyby jakimś kanałem komunikacji wątek ten przekazywałby nam TID, to przy zamykaniu kolejek nie wiemy, czy nadal on istnieje. Poza tym, implementacja może ten wątek tworzyć w stanie `detached`. Na taki wątek nie możemy już nigdy czekać. {{< /answer >}}

- Czy nie wystarczyłoby zaczekać, aż wątek obsługi sygnału zakończy pracę, zanim zamknęlibyśmy kolejki?
{{< answer >}} Nie możemy czekać na ten wątek. Nie mamy jego TID. Nawet gdyby jakimś kanałem komunikacji wątek ten przekazywałby nam TID, to przy zamykaniu kolejek nie wiemy, czy nadal on istnieje. Poza tym, implementacja może ten wątek tworzyć w stanie `detached`. Na taki wątek nie możemy już nigdy czekać. {{< /answer >}}

- Czy wywołanie `exit(EXIT_SUCCESS)` w procesie dziecka możemy przenieść z funkcji `child_function` do `spawn_child` zaraz za wywołanie `child_function`?
{{< answer >}} Nie, z tego samego powodu, dlaczego nie zamykamy kolejek. Gdybyśmy tak zrobili, to zmienna `child_data` straciłaby ważność po wyjściu z funkcji `child_function`. Ale używa jej wątek, który być może jeszcze działa. Jeśli `exit(EXIT_SUCCESS)` pozostaje w `child_function`, to nie ma tego problemu. {{< /answer >}}

## Przykładowe zadania

Wykonaj przykładowe zadania. Podczas laboratorium będziesz miał więcej czasu oraz dostępny startowy kod, jeśli jednak wykonasz poniższe zadania w przewidzianym czasie, to znaczy że jesteś dobrze przygotowany do zajęć.

- [Zadanie 1]({{< ref "/sop2/lab/l2/example1" >}}) ~90 minut
- [Zadanie 2]({{% ref "/sop2/lab/l2/example2" %}}) ~120 minut
- [Zadanie 3]({{% ref "/sop2/lab/l2/example3" %}}) ~120 minut
- [Zadanie 4]({{< ref "/sop2/lab/l2/example4" >}}) ~150 minut

## Kody źródłowe z treści tutoriala
{{% codeattachments %}}
