---
title: "L2 - Kolejki POSIX"
date: 2022-02-10T10:00:00+01:00
weight: 20
---

# Tutorial 6 - kolejki POSIX


Temat kolejek jest wyraźnie mniej obszerny od tematu łączy pipe/fifo i ten tutorial jest też krótszy. Kolejki posix nie mają problemu z współdzieleniem zapisu i odczytu, z atomowością zapisu nawet dużych wiadomości, mają wyznaczone granice rekordów oraz priorytety. Wszystko to czyni ten temat względnie łatwym, trudności jakie  mogą powstać ograniczają się do notyfikacji o zdarzeniach w kolejce.

Tematem przewodnim zajęć są kolejki komunikatów POSIX ale samo zadanie będzie oparte także o elementy już wcześniej przez Państwa ćwiczone, na pewno pojawią się wątki, procesy i sygnały.

Do wejściówki i zadania proszę zapoznać się z materiałami podanymi poniżej, nie czytamy całości stron man ale tylko tyle aby wiedzieć co dana funkcja robi, jakie z grubsza przyjmuję parametry i jeśli ma flagi to jak one zmieniają jej działanie. **Materiały przygotowawcze z poprzednich zajęć nadal obowiązują!**
Podane zostały całe polecenia man do wydania z konsoli:

- `man 7 mq_overwiev`
- `man 3p mq_open`
- `man 3p mq_close`
- `man 3p mq_unlink`
- `man 3p mq_getattr`
- `man 3p mq_setattr`
- `man 3p mq_send`
- `man 3p mq_receive`
- `man 3p mq_notify`
- `man 7 sigevent`

Kolejki POSIX wymagają librt podczas linkowania.
Jeśli otwieramy istniejącą kolejkę to mogą się w niej znajdować jakieś dane. Nie wolno zakładać, że taka kolejka jest pusta. Jeśli chcemy aby była pusta to można ją usunąć przed ponownym utworzeniem.

## Przykład: Bingo!
Napisz program, który symuluje prosty wariant gry w BINGO. Losującym liczby jest proces rodzic, graczami jego procesy potomne. Cała komunikacja pomiędzy nimi odbywa się przez kolejki komunikatów POSIX. Proces rodzic tworzy n procesów potomnych (`0 < n < 100`, gdzie `n` to parametr programu) oraz dwie kolejki komunikatów. Pierwsza kolejka `pout` służy do przekazywania co 1sek.  liczb losowanych z przedziału `[0,9]` do procesów potomnych, druga `pin` do odbierania  od procesów potomnych informacji o wygranej lub zakończeniu gry.

Procesy potomne na początek losują swoją liczbę oczekiwaną (wygrywającą) `E` (`[0,9]`) oraz ilość liczb jakie odczytują `N` (`[0,9]`) z kolejki. Następnie cyklicznie  konkurują o dostęp do danych w kolejce `pout` -  jedna wysłana liczba może być odebrana tylko przez jeden proces a nie przez wszystkie naraz. Procesy potomne odczytaną z pout liczbę  porównują do swojej liczby `E` i jeśli jest to ta sama liczba to poprzez drugą kolejkę `pin` przekazują informację o jej wartości poczym się kończą. Po wykonaniu  `N`  sprawdzeń proces potomny przed zakończeniem wysyła przez kolejkę `pin` swój numer  porządkowy (z przedziału `1..n`).

Proces rodzica cały czas, asynchronicznie względem wysyłania liczb, ma odbierać  komunikaty z `pin` i wyświetlać odpowiednie treści na ekran. Gdy wszystkie procesy potomne się skończą proces rodzic też się kończy i usuwa kolejki. 

**Rozmiar komunikatów w kolejce jest ograniczony do 1 bajtu!**

### Rozwiązanie
{{< includecode "mq_example.c" >}}

Zwróć uwagę na użycie wskaźnika przesyłanego z sygnałem, nieco inny jest prototyp funkcji obsługi sygnału (ma dodatkowy parametr `siginfo_t*`) , podczas instalowania obsługi użyto flagi SA_SIGINFO  aby przesłanie wskaźnika było możliwe. Warto zauważyć, że nie wysyłamy sami takiego sygnału, dostajemy go jako notyfikację zdarzenia w kolejce. Funkcją `kill` nie możemy wysłać  wskaźnika można to zrobić jedynie funkcją `sigqueue`.
Do przesyłania liczb wykorzystano typ uint8_t (`stdint.h`), czyli jednobajtowa liczba bez znaku o zakresie dopuszczalnych wartości 0-255. Typy całkowite o precyzyjnie podanym rozmiarze (np. `int32_t`) są bardziej przenośne niż typy bazowe takie jak np. int który może mieć różne zakresy poprawnych wartości na różnych platformach.
W programie występuje obsługa sygnałów więc konieczna jest ochrona przed przerwaniami sygnałem np. za pomocą `TEMP_FAILURE_RETRY`. Makra te zostały dodane w całym kodzie chociaż zagrożony przerwaniem jest tylko kod procesu rodzica bo tylko on dostaje notyfikacje o stanie kolejki. Dodawanie zabezpieczeń przed przerwaniem funkcji obsługą sygnałem nie spowalnia istotnie kodu za to czyni go dużo lepiej przenośnym. 
Ustawienia notyfikacji na kolejce są jednokrotne tzn. po odebraniu notyfikacji nie spodziewamy się kolejnych o ile nie wywołamy po raz kolejny `mq_notify`. Notyfikacja zadziała tylko wtedy gdy kolejka przejdzie ze stanu pustego w niepusty. Tylko jeden proces może być  zarejestrowany do notyfikacji. 

W kodzie procesu rodzica brak ochrony przed przerwaniem w jednym wywołaniu, którym?  Dlaczego tam ochrona taka nie jest konieczna? 
{{< answer >}} Chodzi o wywołanie mq_receive w funkcji obsługi sygnału, nie spodziewamy się przerwania funkcji obsługi sygnału obsługiwanym sygnałem gdyż domyślnie na czas tej obsługi sygnał ten jest blokowany.  {{< /answer >}}

Jak zrealizowane jest zliczanie procesów potomnych?
{{< answer >}} Zliczane są skuteczne wywołania waitpid w funkcji obsługi SIGCHLD, nie sygnały SIGCHLD bo te się mogą sklejać. Licznik aktywnych procesów potomnych jest zmienną globalną.  {{< /answer >}}

Czemu podczas  odebraniu sygnału najpierw instalujemy/restartujemy notyfikację, a dopiero później czytamy z kolejki?
{{< answer >}} Gdyby było odwrotnie to po przeczytaniu wiadomości z kolejki (możliwe że kilku) a tuż przed założeniem notyfikacji mogłoby się coś w kolejce pojawić a ponieważ powiadomienia są wysyłane tylko gdy w pustej kolejce się coś pojawi to nie mielibyśmy szans już nic odczytać bo nasza kolejka w momencie instalowania notyfikacji coś już zawierała. Jeśli natomiast zrobimy tak jak w przykładzie to po zainstalowaniu notyfikacji i przeczytaniu tego co się w kolejce znajduje na pewno mamy kolejkę pustą z włączoną notyfikacją, jeśli teraz coś się w kolejce znajdzie dostaniemy notyfikację. {{< /answer >}}

Czemu jedna z kolejek jest w trybie nie blokującym?
{{< answer >}} Kolejka pin jest w stanie nieblokującym ze względu na powyższy punkt. Gdyby mogła się blokować to po odczytaniu ostatniej wiadomości program zablokował by się wewnątrz asynchronicznej obsługi sygnału co byłoby dużym błędem.  {{< /answer >}}

Jak są wykorzystane priorytety wiadomości i jak to się ma do limitu 1 bajtu długości wiadomości?
{{< answer >}} W tym programie priorytety nie służą do uszeregowania kolejności ale jako wyznaczniki typu inforamcji. Zmusza nas do tego krótki jednobajtowy rozmiar wiadomości, trudno (choć się da) poza liczbą zakodować tam jeszcze informację czy chodzi o wygraną czy zakończenie gry, ta informacja jest zawarta właśnie w priorytecie. {{< /answer >}}

W ramach dalszych ćwiczeń można przerobić ten program tak aby  notyfikacją odbywała się nie sygnałem a wątkiem.
Można też uprościć program, zamiast notyfikacji stworzyć wątek dedykowany tylko do odczytu z kolejki, wykonaj taką modyfikację jako ćwiczenie.
Spora część logiki programu "wylądowała" w funkcji obsługi sygnału, było to możliwe bo nie ma zależności pomiędzy kodem wysyłającym liczby oraz tym odbierającym notyfikacje, ale zadanie łatwo można skomplikować w taki sposób aby ta zależność była. Jako ćwiczenie przenieś całą logikę związaną z odbiorem wiadomości do kodu właściwego rodzica (czyli poza asynchroniczne wywołanie funkcji obsługi sygnału).

## Ćwiczenie
Wykonaj przykładowe [ćwiczenie]({{< ref "../l2-example" >}}) z poprzednich lat. To zadanie szacuję na 90 minut, jeśli wyrobisz się w tym czasie to znaczy, że jesteś dobrze przygotowany/a do zajęć.


## Kody źródłowe z treści tutoriala
{{% codeattachments %}}
