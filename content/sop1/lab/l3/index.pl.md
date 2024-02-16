---
title: "L3 - Wątki, mutexy i sygnały"
date: 2022-02-05T19:20:38+01:00
weight: 40
---

# Tutorial 3 - Wątki, mutexy i sygnały

{{< hint info >}}
Uwagi wstępne:

- Szybkie przejrzenie tutoriala prawdopodobnie nic nie pomoże, należy samodzielnie uruchomić programy, sprawdzić jak
  działają, poczytać materiały dodatkowe takie jak strony man. W trakcie czytania sugeruję wykonywać ćwiczenia a na
  koniec przykładowe zadanie.
- Na żółtych polach podaję dodatkowe informacje, niebieskie zawierają pytania i ćwiczenia. Pod pytaniami znajdują się
  odpowiedzi, które staną się widoczne dopiero po kliknięciu. Proszę najpierw spróbować sobie odpowiedzieć na pytanie
  samemu a dopiero potem sprawdzać odpowiedź.
- Pełne kody do zajęć znajdują się w załącznikach na dole strony. W tekście są tylko te linie kodu, które są konieczne
  do zrozumienia problemu.
- Materiały i ćwiczenia są ułożone w pewną logiczną całość, czasem do wykonania ćwiczenia konieczny jest stan osiągnięty
  poprzednim ćwiczeniem dlatego zalecam wykonywanie ćwiczeń w miarę przyswajania materiału.
- Większość ćwiczeń wymaga użycia konsoli poleceń, zazwyczaj zakładam, ze pracujemy w jednym i tym samym katalogu
  roboczym więc wszystkie potrzebne pliki są "pod ręką" tzn. nie ma potrzeby podawania ścieżek dostępu.
- To co ćwiczymy wróci podczas kolejnych zajęć. Jeśli po zajęciach i teście coś nadal pozostaje niejasne proszę to
  poćwiczyć a jeśli trzeba dopytać się u prowadzących.
- Ten tutorial opiera się na przykładach od naszego studenta, drobne ich słabości występują jedynie w częściach nie
  związanych z wątkami i zostawiłem je celowo. Są one omawiane w komentarzach i zadaniach. To co się powtarza to:
- Funkcje main są za długie, można je łatwo podzielić
- Nadużywany jest typ bool, wymaga to nawet specjalnego pliku nagłówkowego, prościej i bardziej klasycznie można użyć 0
  i 1
- Pojawiają się "magic numbers" - stałe liczbowe zamiast zdefiniowanych makr
{{< /hint >}}

_Ten tutorial został przygotowany w oparciu o pomysły na zadania i ich rozwiązania przygotowane przez studenta Mariusza Kowalskiego._

## Zadanie 1 - proste wątki typu joinable, synchronizacja, zwracanie wyników przez wątek

Cel:
Napisać program przybliżający wartość PI metodą Monte Carlo. Program przyjmuje dwa parametry:
- k ... liczbę wątków użytych do estymacji,
- n ... liczbę losowanych wartości przez każdy wątek.

Każdy wątek (za wyjątkiem głównego) ma przeprowadzać własną estymację. 
W momencie kiedy wszystkie wątki zakończą obliczenia, wątek główny wypisuje obliczoną wartość jako średnią z wszystkich symulacji.

Co student musi wiedzieć:
- man 7 pthreads
- man 3p pthread_create
- man 3p pthread_join
- man 3p rand (informacja o rand_r)
- metoda Monte-Carlo, w paragrafie "Monte Carlo methods" na <a href="https://en.wikipedia.org/wiki/Pi#Use"> stronie.</a>

<em>rozwiązanie <b>Makefile</b>:</em>

```makefile
CC=gcc
CFLAGS=-std=gnu99 -Wall -fsanitize=address,undefined
LDFLAGS=-fsanitize=address,undefined
LDLIBS=-lpthread -lm
```

Flaga `-lpthread` będzie nam potrzebna w całym tutorialu, biblioteka nazywa się libpthread.so (po -l podajemy nazwę z
odciętym początkowym lib)

Flaga `-lm` włącza bibliotekę math plik biblioteki to lm.so a nie libmath.so jak moglibyśmy zgadywać

<em>rozwiązanie, cały <b>prog17.c</b>:</em>
{{< includecode "prog17.c" >}}

Program ten (i następne) nie pokazuje usage, zamiast tego ma zdefiniowane wartości domyślne dla parametrów pracy,
wywołaj go bez parametrów w celu sprawdzenia.

Deklaracje zapowiadające (nie definicje) funkcji mogą być bardzo użyteczne, czasem są wręcz niezbędne, jeśli nie wiesz o
czym mowa poczytaj <a href="http://en.cppreference.com/w/c/language/function_declaration"> tutaj</a>.

W programach wielowątkowych nie można poprawnie używać funkcji rand(), zamiast tego używamy rand_r, jednak ta funkcja
wymaga indywidualnego ziarna dla każdego z wątków.

W tym programie wykorzystanie wątków jest bardzo proste, program główny czeka zaraz po utworzeniu na ich zakończenie,
możliwe są dużo bardziej skomplikowane scenariusze.

Pamiętaj, że niemal każde wywołanie funkcji systemowej (i wielu funkcji bibliotecznych) wymaga sprawdzenia czy nie
wystąpił błąd i odpowiedniej reakcji jeśli wystąpił.

Makro ERR nie wysyła sygnału jak w programie wieloprocesowym, czemu?
{{< expand "Odpowiedź" >}} Funkcja exit zamyka cały proces tzn. wszystkie wątki w jego obrębie. {{< /expand >}}

Jak dane są przekazywane do nowo tworzonych wątków?
{{< expand "Odpowiedź" >}} Wyłącznie przez strukturę argsEstimation_t do której wskaźnik jest argumentem funkcji wątku, nie ma żadnego powodu aby użyć zmiennych globalnych. {{< /expand >}}

Czy dane na których pracują wątki są współdzielone pomiędzy nimi?
{{< expand "Odpowiedź" >}} W tym programie nie, zatem nie ma też konieczności niczego synchronizować, każdy wątek dostaje tylko swoje dane. {{< /expand >}}

Skąd wątki biorą "ziarno" do swoich wywołań rand_r?
{{< expand "Odpowiedź" >}} Dostają to ziarno jako jedno z pól struktury z danymi wątku. {{< /expand >}}

Czemu w kodzie używamy srand/rand czy to nie przeczy uwadze podanej kilka punktów powyżej? 
{{< expand "Odpowiedź" >}} Tylko jeden wątek używa srand/rand do tego czyni to zanim wystartują inne wątki zatem na etapie gdy program jest jeszcze jednowątkowy. Problem z srand/rand polega na pojedynczej zmiennej globalnej przechowującej aktualne ziarno (zmienna ta jest ukryta w bibliotece).  {{< /expand >}}

Czy moglibyśmy mieć jedną strukturę z parametrami startowymi programu? Czemu?
{{< expand "Odpowiedź" >}} Nie, ze względu na pole z ziarnem losowości, to musi być inne dla wszystkich wątków {{< /expand >}}

Czy tablica z strukturami startowymi mogłaby być zmienna automatyczną a nie dynamiczną ? 
{{< expand "Odpowiedź" >}} Tyko jeśli dodalibyśmy istotne ograniczenie na ilość wątków (np.: do tysiąca), w przeciwnym wypadku tablica ta może zająć cały stos.  {{< /expand >}}

Czemu służy zwalnianie pamięci danych zwróconych przez wątek?
{{< expand "Odpowiedź" >}} Pamięć na te dane te została przydzielona dynamicznie, sterta jest wspólna dla wątków więc musimy zwolnić tą pamięć, zakończenie się wątku nie zrobi tego za nas. {{< /expand >}}

Czy można zwrócić z wątku adres lokalnej (dla wątku) zmiennej automatycznej z wynikiem? 
{{< expand "Odpowiedź" >}} Nie, w momencie końca wątku jego stos jest zwalniany, więc jakikolwiek wskaźnik na tą pamięć przestaje być wiarygodny. Co gorsza, w wielu przypadkach to będzie wyglądać jakby działało, bo do "zepsucia" potrzeba aby program napisał sobie tą pamięć. Należy być wyjątkowo wyczulonym na tego rodzaju błędy bo są one potem bardzo trudne do wyszukania! {{< /expand >}}

Czy można jakoś uniknąć dodatkowej alokacji w funkcji wątku?

## Zadanie 2 - proste wątki typu detachable z wspólnymi zmiennymi i mutexem do synchronizacji

Cel:
Napisać program symulujący wizualizację rozkładu dwumianowego za pomocą deski Galtona (Galton Board) z 11 pojemnikami na kulki.
Program ma przyjmować dwa parametry:
- k ... liczbę wątków zrzucających kulki,
- n ... całkowitą liczbę kulek do zrzucenia (w sumie).

Każdy wątek ma zrzucać kulki pojedynczo i po każdym rzucie aktualizuje licznik kulek dla odpowiedniego pojemnika.
Główny wątek co sekundę sprawdza czy symulacja została zakończona (nie korzystamy z funkcji pthread_join).
Po zrzuceniu wszystkich kulek główny wątek ma wypisać ilości kulek w pojemnikach, całkowitą liczbę kulek i otrzymaną "wartość oczekiwaną" przy dowolnym numerowaniu pojemników.

Co student musi wiedzieć:

- man 3p pthread_mutex_destroy (cały opis)
- man 3p pthread_mutex_lock
- man 3p pthread_mutex_unlock
- man 3p pthread_detach
- man 3p pthread_attr_init
- man 3p pthread_attr_destroy
- man 3p pthread_attr_setdetachstate
- man 3p pthread_attr_getdetachstate
- deska Galtona na <a href="https://pl.wikipedia.org/wiki/Deska_Galtona"> stronie.</a>

<em>rozwiązanie, cały <b>prog18.c</b>:</em>
{{< includecode "prog18.c" >}}

Wszystkie dane konieczne do pracy wątku są przekazywane przez strukturę argsThrower_t, wynik jego pracy to modyfikacja w
tablicy bins, znowu nic nie jest przekazywane przez zmienne globalne.

W programie mamy dwa muteksy chroniące dostęp do liczników oraz całą tablice muteksów chroniących dostęp do komórek
tablicy bins (po jednym na komórkę). Zatem muteksów jest BIN_COUNT+2.

W programie tworzymy wątki typu "detachable" czyli nie ma potrzeby (ani możliwości) czekać na ich zakończenie, stąd brak
pthread_join, za to nie wiemy kiedy wątki robocze skończą działanie, musimy do tego stworzyć własny test.

W programie muteksy są inicjowane zarówno automatycznie jak i dynamicznie, jako zmienne automatyczne muteksy są prostsze
w tworzeniu ale trzeba z góry znać ilość muteksów. Muteksy jako zmienne dynamicznie nie mają tego ograniczenia ale
trzeba inicjować i usuwać mutex w kodzie.

Czy dane przekazane przez argsThrower_t są współdzielone pomiędzy wątki?
{{< expand "Odpowiedź" >}} Częściowo tak, wspólne dane muszą być chronione przy wielodostępie, stąd muteksy dla liczników i koszy. {{< /expand >}}

Czy struktura argsThrower_t  jest optymalna?
{{< expand "Odpowiedź" >}} Nie 
wiele pól kopiujemy dla każdego wątku, chociaż te wskaźniki są takie same. Wspólne elementy można przenieść do drugiej struktury i tylko podawać jeden wskaźnik każdemu wątkowi zamiast 6 jak w kodzie. Dodatkowo przechowujmy w tej strukturze tid'y wątków, ale z tego nigdzie nie korzystamy.   {{< /expand >}}

Czemu używamy głównie wskaźników do przekazania danych do wątków?
{{< expand "Odpowiedź" >}}  Współdzielimy te dane, nie chcemy mieć lokalnych kopi zupełnie niezależnych od innych wątków, chcemy mieć jedną kopię tych danych dla wszystkich wątków. {{< /expand >}}

Czy mutex'y można przekazać nie przez wskaźnik?
{{< expand "Odpowiedź" >}}  NIE, TO ZABRONIONE PRZEZ POSIX, kopia muteksu nie musi być muteksem, a już zupełnie oczywiste, że nie byłaby tym samym muteksem {{< /expand >}}

Ten program używa  dużo muteksów, czy może być ich mniej?
{{< expand "Odpowiedź" >}}  Tak, w skrajnym wypadku wystarczyłby 1 ale kosztem równoległości, możliwe są też rozwiązania pośrednie np. po jednym na liczniki i jeden wspólny na kosze. To ostatnie rozwiązanie, pomimo zmniejszenia równoległości, może okazać się szybsze bo liczne operacje na muteksach są dodatkowych obciążeniem dla systemu. {{< /expand >}}

Metoda czekania na zakończenie się  wątków liczących polega na okresowym sprawdzaniu czy licznik kul zrzuconych jest równy sumie kul w programie, czy jest to optymalne rozwiązanie?
{{< expand "Odpowiedź" >}}  Nie jest. To jest tzw. soft busy waiting, ale bez użycia zmiennych warunkowych nie możemy tego zrobić lepiej. {{< /expand >}}

Czy wszystkie wątki w tym programie na prawdę pracują?
{{< expand "Odpowiedź" >}}   Nie, zwłaszcza gdy będzie ich dużo. Możliwe jest tzw. zagłodzenie wątku, czyli nie danie mu szansy na pracę. W tym programie działania wątku są na prawdę szybkie a tworzenie nowych wątków dość czasochłonne, może się tak stać, że wątki utworzone jako ostatnie nie będą miały co robić bo te wcześniejsze przerzucą wszystkie kulki wcześniej. Jako sprawdzenie dodaj do wątków liczniki ile kulek przerzucił dany wątek i wypisz je na koniec na ekran. Można zapobiec temu zjawisku synchronizując moment rozpoczęcia pracy przez wątki, ale tu znowu nie wystarczy muteks, najlepiej taką synchronizację zrobić na barierze lub zmiennej warunkowej, a te mechanizmy poznacie w przyszłym semestrze. {{< /expand >}}

## Zadanie 3 - wątki i sygnały z czekaniem na sygnał za pomocą funkcji sigwait

Cel:
Napisać program, który przyjmuje jeden parametr 'k' i co sekundę wyświetla listę liczb, początkowo od 1 do k. 
Program ma obsługiwać dwa sygnały za pomocą oddzielnego wątku. Wątek ten po otrzymaniu sygnału podejmuje następujące akcje:
- SIGINT  (C-c) ... usuwa losową liczbę z listy (jeżeli lista jest pusta to nic nie robi),
- SIGQUIT  (C-\) ... ustawia flagę 'STOP' i kończy swoje działanie.

Wątek główny ma za zadanie co sekundę wyświetlić listę lub, jeżeli ustawiona jest flaga 'STOP', poprawnie zakończyć swoje działanie.

Co student musi wiedzieć:

- man 3p pthread_sigmask
- man 3p sigprocmask
- man 3p sigwait

<em>rozwiązanie, cały <b>prog19.c</b>:</em>
{{< includecode "prog19.c" >}}

W strukturze argumentów argsSignalHandler_t przekazujemy wskazania na dane współdzielone przez oba wątki czyli tablicę i
flagę STOP oraz muteksy je chroniące. Dodatkowo maskę sygnałów i tid wątku obsługi sygnałów, które to dane nie są
współdzielone.

W wątkach do ustawiania maski sygnałów (per wątek) używamy pthread_sigmask, nie powinno się, gdy istnieje więcej wątków
niż jeden w programie używać f. sigprocmask.

Delegowanie oddzielnego wątku do obsługi sygnałów jest typowym i wygodnym sposobem radzenia sobie w programach
wielowątkowych.

Ile wątków jest w tym programie?
{{< expand "Odpowiedź" >}} Dwa, wątek główny utworzony przez system (każdy proces ma ten jeden wątek) oraz watek utworzony przez nas w kodzie.  {{< /expand >}}

Wypunktuj różnicę i podobieństwa pomiędzy sigwait i sigsuspend: 
{{< expand "Odpowiedź" >}}
- sigwait nie wymaga funkcji obsługi sygnału tak jak sigsuspend
- obie metody wymagają blokowania sygnałów oczekiwanych
- sigwait nie jest przerywany przez funkcję obsługi sygnałów -  nie może to wynika z POSIX
- sigwait nie zmienia maski blokowanych sygnałów więc nie ma mowy o uruchomieniu obsługi sygnału blokowanego (jeśli by takowa była, w przykładzie nie ma) jak przy sigsuspend
{{< /expand >}}

Po wywołaniu sigwait tylko jeden typ sygnału jest zdejmowany z wektora sygnałów oczekujących wiec problem jaki mieliśmy z obsługa wielu sygnałów w trakcie pojedynczego sigsuspend w przykładzie z L2  nie wystąpiłby jeśli zamienić użycie sigsuspend na sigwait (ćwiczenie do zrobienia)

Czy metoda czekania na potomka jest w tym programie równie słaba jak w poprzednim?
{{< expand "Odpowiedź" >}}   Nie,  tutaj sprawdzanie co sekundę jest częścią zadania, gdyby jednak nie było to i tak unikniemy busy waitingu bo możemy się synchronizować na pthread_join. {{< /expand >}}

Czy w tym programie można użyć sigprocmask zamiast pthread_sigmask?
{{< expand "Odpowiedź" >}} Tak, wywołanie blokujące sygnały pojawia się zanim powstaną dodatkowe wątki. {{< /expand >}}

Czemu nie sprawdzamy błędów wywołań funkcji systemowych związanych pozyskaniem i zwolnieniem muteksów
{{< expand "Odpowiedź" >}} Podstawowy typ muteksu (a taki używamy w programie) nie sprawdza i nie raportuje błędów. Dodanie sprawdzania nie byłoby wcale złe, łatwiej można potem zmienić muteks na typ raportujący błędy.  {{< /expand >}}


## Zadanie 4 - anulowanie wątków, cleanup handlers

Cel:
Napisać program symulujący losy studentów wydziału MiNI. Program ma następujące argumenty:
- n <= 100 ... liczba nowych studentów,

Program przechowuje liczniki studentów na 1, 2, 3 roku oraz inżynierów.

Wątek główny: Inicjuje studentów, a przez następne 4 sekundy, w losowych odstępach czasu (100-300ms) wyrzuca jednego studenta (anuluje wątek). 
Po czterech sekundach, czeka na zakończenie działania studentów i wypisuje liczbę inżynierów oraz wyrzuconych osób. 

Wątek studenta: Każdy student jest nowo-utworzonym wątkiem. Student dodaje się do licznika 1. roku, po sekundzie odejmuje się z tego licznika
i dodaję do 2. roku. Analogicznie po sekundzie przechodzi do 3. roku i po jeszcze jednej sekundzie do inżynierów. Student jest zawsze przygotowany na opuszczenie uczelni.

Co student musi wiedzieć:

- man 3p pthread_cancel
- man 3 pthread_cleanup_push
- man 3 pthread_cleanup_pop
- man 7 time
- man 3p clock_getres

<em>rozwiązanie, cały <b>prog20.c</b>:</em>
{{< includecode "prog20.c" >}}

Wątkom przekazuje się strukturę z aktualnym rokiem oraz z wskazaniem na 4 liczniki z muteksami, struktura argsModify_t
nie powiela błędu z zadania 2 czyli nie przekazuje indywidualnych kopii zbyt wielu wskaźników.

Struktura `studentsList_t` jest używana jedynie przez wątek główny, nie jest "widziana" przez wątki studentów.

"Sprytna" inicjalizacja struktury yearCounters_t nie zadziała z archaicznymi standardami języka C (c89/c90). Warto
wiedzieć, oczywiście używamy wszelkich dobrodziejstw nowszych standardów.

Cleanup handlery w funkcji wątku służą do bezpiecznego usunięcia studenta z licznika roku w razie anulowania w czasie
spania, bez nich takie anulowanie pozostawiłoby studenta na roku do końca działania programu!

Pamiętaj, że pthread_cleanup_push musi być sparowany z pthread_cleanup_pop w obrębie tego samego bloku {}

Ile w programie występuje mutexów i co chronią?
{{< expand "Odpowiedź" >}} Dokładnie 4, każdy chroni inny współdzielony licznik. {{< /expand >}}

Czy aktualny rok studenta musi być częścią struktury argsModify_t?
{{< expand "Odpowiedź" >}} Nie, równie dobrze mógłby być automatyczną zmienną utworzoną w wątku, nie byłaby wtedy potrzebna struktura argsModify_t , wątkom przekazywalibyśmy wskaźnik na strukturę yearCounters.  {{< /expand >}}

Co to znaczy, że wątek ma stan anulowania PTHREAD_CANCEL_DEFERRED ?
{{< expand "Odpowiedź" >}} Anulowanie nastąpi dopiero podczas wywołania funkcji z listy funkcji "cancelation points" {{< /expand >}}

Które z funkcji użytych w funkcji wątku są punktami anulowania wątku?
{{< expand "Odpowiedź" >}} W tym kodzie  tylko nanosleep (wywołany z msleep). {{< /expand >}}

Skąd wiemy, które funkcje są takimi punktami?
{{< expand "Odpowiedź" >}} $man 7 pthreads {{< /expand >}}

Co oznacza jedynka w wywołaniu: pthread_cleanup_pop(1); ?
{{< expand "Odpowiedź" >}} Oznacza, że poza zdjęciem handlera ze stosu jest on też wykonywany. {{< /expand >}}

Kiedy jest zmniejszany liczniku roku?
{{< expand "Odpowiedź" >}} W dwu przypadkach, podczas anulowania (rzadki przypadek) oraz  podczas zdjęcia handlera decrement_counter ze stosu cleanup handlerów. {{< /expand >}}

W algorytmie losowania wątku do anulowania jest spory błąd logiczny, gdzie i jakie niesie zagrożenie?
{{< expand "Odpowiedź" >}} Losowy  wybór wątku do anulowania  może trwać bardzo długo gdy na dużej liście wątków zostanie np. tylko 1 nie anulowany.  Można to zaobserwować, jeśli wywołać program z parametrem 10. {{< /expand >}}

Jako ćwiczenie popraw sposób losowania "żyjącego" wątku do anulowania.

Zwróć uwagę na sposób odliczenia 4 sekund w losowych interwałach za pomocą clock_gettime i nanosleep,  to można zmienić, dodając wywołanie f.alarm i oddając obsługę sygnału SIGALRM. Rozwiązanie takie można wykonać jako ćwiczenie.

Wykonaj przykładowe <a href="{{< ref "../l3-example" >}}">ćwiczenie</a> z poprzednich lat. To zadanie szacuję na 60 minut, jeśli wyrobisz się w tym czasie to znaczy, że jesteś dobrze przygotowany/a  do zajęć. Pamiętaj, że w aktualnym układzie zajęć będzie dane zadnie na około 1,5 godziny, więc trochę bardziej pracochłonne.

## Kody źródłowe z treści tutoriala

{{% codeattachments %}}
