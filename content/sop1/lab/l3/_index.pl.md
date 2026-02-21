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

## Tworzenie wątków
Wątek tworzymy używając funkcji `pthread_create`:

```
int pthread_create(pthread_t *restrict thread,
                  const pthread_attr_t *restrict attr,
                  void *(*start_routine)(void*),
                  void *restrict arg);
```

W przeciwieństwie do funkcji `fork`, wartość zwracana przez `pthread_create` niesie informacje jedynie o poprawnym wykonaniu funkcji lub błędzie. Identyfikator nowego wątku zostaje zapisany do pierwszego argumentu.

Drugi argument pozwala wyspecyfikować atrybuty wątku - zobacz  `man 3p pthread_attr_destroy` it `man -k pthread_attr_set`. Przekazując `NULL` ustawimy domyślne wartości artybutów.

Trzecim argumentem `pthread_create` jest wskaźnik na funkcję o sygnaturze:

```
void *function(void *args);
```

która będzie wykonywana przez nowoutworzony wątek.

Ostatni argument służy do przekazania argumentów do funkcji. Jest typu `void*`, co w C oznacza wskaźnik o nieustalonym typie - na cokolwiek. Żeby go użyć musimy zrzutować go właściwy typ.


```
man 3p pthread_create
```

### Czekanie na zakończenie wątków

Podobnie jak przy procesach, najczęściej chcemy, żeby główny wątek poczekał na zakończeniu nowoutworzonych wątków. Służy do tego funkcja `pthread_join`:

```
int pthread_join(pthread_t thread, void **value_ptr);
```

działająca podobnie do `wait` przy procesach. Jej wywołanie blokuje wykonanie programu do czasu aż dany wątek się zakończy.

Podobnie jak przy `pthread_create` wartość zwracania służy tu tylko da sygnalizowania błędów. Wartość zwracana przez wątek (czyli przez funkcję, którą wykonuje) zostanie zapisana do drugie argumentu `pthread_join`.

W przeciwieństwie do funkcji `wait` nie mamy tu możliwości czekania na dowolny wątek - zawsze musimy podać konkretny identyfikator wątku, na który czekamy.

```
man 3p pthread_join
```

### Zadanie

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
- metoda Monte-Carlo, w paragrafie "Monte Carlo methods" na <a href="https://en.wikipedia.org/wiki/Pi#Monte_Carlo_methods"> stronie.</a>

### Rozwiązanie

**Makefile**

```makefile
CC=gcc
CFLAGS=-std=gnu99 -Wall -fsanitize=address,undefined
LDFLAGS=-fsanitize=address,undefined
LDLIBS=-lpthread
```

Flaga `-lpthread` będzie nam potrzebna w całym tutorialu, biblioteka nazywa się libpthread.so (po -l podajemy nazwę z
odciętym początkowym lib)

**prog17.c**:
{{< includecode "prog17.c" >}}


### Uwagi i pytania

Deklaracje zapowiadające (nie definicje) funkcji mogą być bardzo użyteczne, czasem są wręcz niezbędne, jeśli nie wiesz o
czym mowa poczytaj <a href="http://en.cppreference.com/w/c/language/function_declaration"> tutaj</a>.

W programach wielowątkowych nie można poprawnie używać funkcji `rand`, zamiast tego używamy `rand_r`, jednak ta funkcja
wymaga indywidualnego ziarna dla każdego z wątków.

W tym programie wykorzystanie wątków jest bardzo proste, program główny czeka zaraz po utworzeniu na ich zakończenie,
możliwe są dużo bardziej skomplikowane scenariusze.

Pamiętaj, że niemal każde wywołanie funkcji systemowej (i wielu funkcji bibliotecznych) wymaga sprawdzenia czy nie
wystąpił błąd i odpowiedniej reakcji jeśli wystąpił.

Makro ERR nie wysyła sygnału jak w programie wieloprocesowym, czemu?
{{< details "Odpowiedź" >}} Funkcja exit zamyka cały proces tzn. wszystkie wątki w jego obrębie. {{< /details >}}

Jak dane są przekazywane do nowo tworzonych wątków?
{{< details "Odpowiedź" >}} Wyłącznie przez strukturę `targs_t` do której wskaźnik jest argumentem funkcji wątku, nie ma żadnego powodu aby użyć zmiennych globalnych. {{< /details >}}

Czy dane na których pracują wątki są współdzielone pomiędzy nimi?
{{< details "Odpowiedź" >}} W tym programie nie, zatem nie ma też konieczności niczego synchronizować, każdy wątek dostaje tylko swoje dane. {{< /details >}}

Skąd wątki biorą "ziarno" do swoich wywołań `rand_r`?
{{< details "Odpowiedź" >}} Dostają to ziarno jako jedno z pól struktury z danymi wątku. {{< /details >}}

Czemu w kodzie używamy srand/rand czy to nie przeczy uwadze podanej kilka punktów powyżej? 
{{< details "Odpowiedź" >}} Tylko jeden wątek używa srand/rand do tego czyni to zanim wystartują inne wątki zatem na etapie gdy program jest jeszcze jednowątkowy. Problem z srand/rand polega na pojedynczej zmiennej globalnej przechowującej aktualne ziarno (zmienna ta jest ukryta w bibliotece).  {{< /details >}}

Czy moglibyśmy mieć jedną strukturę z parametrami startowymi programu? Czemu?
{{< details "Odpowiedź" >}} Nie, ze względu na pole z ziarnem losowości, to musi być inne dla wszystkich wątków {{< /details >}}

Czy tablica z strukturami startowymi mogłaby być zmienna automatyczną a nie dynamiczną ? 
{{< details "Odpowiedź" >}} Tylko jeśli dodalibyśmy istotne ograniczenie na ilość wątków (np.: do tysiąca), w przeciwnym wypadku tablica ta może zająć cały stos.  {{< /details >}}

Czemu służy zwalnianie pamięci danych zwróconych przez wątek?
{{< details "Odpowiedź" >}} Pamięć na te dane te została przydzielona dynamicznie, sterta jest wspólna dla wątków więc musimy zwolnić tą pamięć, zakończenie się wątku nie zrobi tego za nas. {{< /details >}}

Czy można zwrócić z wątku adres lokalnej (dla wątku) zmiennej automatycznej z wynikiem? 
{{< details "Odpowiedź" >}} Nie, w momencie końca wątku jego stos jest zwalniany, więc jakikolwiek wskaźnik na tą pamięć przestaje być wiarygodny. Co gorsza, w wielu przypadkach to będzie wyglądać jakby działało, bo do "zepsucia" potrzeba aby program napisał sobie tą pamięć. Należy być wyjątkowo wyczulonym na tego rodzaju błędy bo są one potem bardzo trudne do wyszukania! {{< /details >}}

Czy można jakoś uniknąć dodatkowej alokacji w funkcji wątku?
{{< details "Odpowiedź"  >}} Tak, możemy dodać pole na wynik w strukturze `targs_t`. {{< /details  >}}

## Wątki typu detached i synchronizacja mutexem

Co do zasady przy zwykłych wątkach musimy wywołać `pthread_join`. Wyjątek stanowią wątki typu "detached" (ang. _detached thread_) - czyli wątki "odłączone" od głownego. Taki typ wątków może być użyteczny, ma jednak dwie wady - skoro nie możemy na nim użyć `pthread_join` nie mamy jak poczekać na jego zakończenie oraz jak przekazać z niego dane do wątku głównego.

Wątek "detached" możemy uzyskać przez podanie odpowiednich atrybutów przy `pthread_create` albo poprzez "odłączenie" już uruchomionego wątku.

Pierwsza metoda polega na inicjalizacji obiektu typu `pthread_attr_t` używając `pthread_attr_init`. Następnie używając funkcji `pthread_attr_setdetachstate` ustawiamy `PTHREAD_CREATE_DETACHED`. Potem tworzymy wątek funkcją `pthread_create` przekazując utworzone atrybuty jak parametr. Należe pamiętać, żeby po użyciu usunąć stworzone atrybuty funkcją `pthread_attr_destroy`.

```
man 3p pthread_attr_destroy
man 3p pthread_attr_getdetachstate
man 3p pthread_create
```

Druga metoda polega na wywołaniu funkcji `pthread_detach` podając identyfikator wątku. Tym sposobe wątek może też sam się "odłączyć" - uzyskując swój identyfikator dzięki `pthread_self`. Próba "odłączenia" już "odłączonego" wątku to UB.

```
man 3p pthread_detach
man 3p pthread_self
```

### Mutex

Żeby z wątków tego typu dało się zwrócić dane potrzebujemy zmiennych współdzielonych między wątkami. Jednak sytuacja, w której kilka wątków na raz zmienia współdzieloną zmienną prowadzi do błędów, jeśli nie zapewnimy odpowiedniej synchronizacji.

Podstawowym sposobem na zapewninie tejże jest użycie mutexu, czyli obiektu pozwalającemu tylko jednemu wątkowi na raz wejść do danej sekcji krytycznej (więcej o tym w wykładzie). 


Mutex inicjalizujemy funkcją `pthread_mutex_init`, a gdy skończymy go używać niszczymy `pthread_mutex_destroy`.

```
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
   const pthread_mutexattr_t *restrict attr);
```

Podobnie jak przy `pthread_create` możemy tu podać dodatkowe atrybuty - dla uzyskania domyślnego zachowaniu po prostu przekazujemy `NULL`

Mając utworzony mutex wątek może go zająć używając funkcji `pthread_mutex_lock`. Ta funkcja blokuje wykonanie programu do czasu, aż mutex nie będzie wolny - po czym zajmuje go dla jednego z czekających wątków. Funkcja ta może zakończyć się błędem tylko w bardzo specyficznych sytuacjach (więcej o nich na SOP2), dlatego w programach niżej nie będziemy tego sprawdzać i nie jest to wymagane na laboratorium.

Po zajęciu mutexu i wykonaniu niezbędnych operacji w sekcji krytycznej wątek powinien zwolnić mutex funkcją `pthread_mutex_unlock`. Warto pamiętać o tym, żeby w programie sekcja krytyczna była możliwie krótka - jej wydłużenie sprawie, że pozostałe wątki muszą dłużej czekać.

Z mutexami trzeba bardzo uważać. Łatwo o zakleszczenie, np. gdy dany wątek spróbuje zająć dany mutex dwa razy pod rząd (bez zwolnienia go pomiędzy). Dodatkowo mutex może być odblokowany tylko przez ten sam wątek, który go zablokował - próba odblokowania z innego wątku to UB.

{{< details "Uwaga"  >}}
Nigdy nie należy kopiować mutexów (np. `pthread_mutex_t mtx1_copy = mtx1`)

POSIX tego zabrania, taki skopiowany mutex może nie działać poprawnie, jego zachowanie może być bardzo nieprzewidywalne.
{{< /details  >}}


```
man 3p pthread_mutex_destroy
man 3p pthread_mutexattr_destroy
man -k pthread_mutexattr_set
man 3p pthread_mutex_lock
```

### Zadanie

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

**prog18.c**:
{{< includecode "prog18.c" >}}

### Uwagi i pytania

Wszystkie dane konieczne do pracy wątku są przekazywane przez strukturę `thrower_args_t`, wynik jego pracy to modyfikacja w
tablicy bins, znowu nic nie jest przekazywane przez zmienne globalne.

W programie mamy dwa muteksy chroniące dostęp do liczników oraz całą tablice muteksów chroniących dostęp do komórek
tablicy bins (po jednym na komórkę). Zatem muteksów jest BIN_COUNT+2.

W programie tworzymy wątki typu "detachable" czyli nie ma potrzeby (ani możliwości) czekać na ich zakończenie, stąd brak
pthread_join, za to nie wiemy kiedy wątki robocze skończą działanie, musimy do tego stworzyć własny test.

W programie muteksy są inicjowane zarówno automatycznie jak i dynamicznie, jako zmienne automatyczne muteksy są prostsze
w tworzeniu ale trzeba z góry znać ilość muteksów. Muteksy jako zmienne dynamicznie nie mają tego ograniczenia ale
trzeba inicjować i usuwać mutex w kodzie.

Czy dane przekazane przez `thrower_args_t` są współdzielone pomiędzy wątki?
{{< details "Odpowiedź" >}} Częściowo tak, wspólne dane muszą być chronione przy wielodostępie, stąd muteksy dla liczników i koszy. {{< /details >}}

Czemu używamy wskaźnika  do przekazania `shared_state_t`?
{{< details "Odpowiedź" >}}  Współdzielimy te dane, nie chcemy mieć lokalnych kopi zupełnie niezależnych od innych wątków, chcemy mieć jedną kopię tych danych dla wszystkich wątków. {{< /details >}}

Ten program używa  dużo muteksów, czy może być ich mniej?
{{< details "Odpowiedź" >}}  Tak, w skrajnym wypadku wystarczyłby 1 ale kosztem równoległości, możliwe są też rozwiązania pośrednie np. po jednym na liczniki i jeden wspólny na kosze. To ostatnie rozwiązanie, pomimo zmniejszenia równoległości, może okazać się szybsze bo liczne operacje na muteksach są dodatkowych obciążeniem dla systemu. {{< /details >}}

Metoda czekania na zakończenie się  wątków liczących polega na okresowym sprawdzaniu czy licznik kul zrzuconych jest równy sumie kul w programie, czy jest to optymalne rozwiązanie?
{{< details "Odpowiedź" >}}  Nie jest. To jest tzw. soft busy waiting, ale bez użycia zmiennych warunkowych nie możemy tego zrobić lepiej. {{< /details >}}

Czy wszystkie wątki w tym programie na prawdę pracują?
{{< details "Odpowiedź" >}}   Nie, zwłaszcza gdy będzie ich dużo. Możliwe jest tzw. zagłodzenie wątku, czyli nie danie mu szansy na pracę. W tym programie działania wątku są na prawdę szybkie a tworzenie nowych wątków dość czasochłonne, może się tak stać, że wątki utworzone jako ostatnie nie będą miały co robić bo te wcześniejsze przerzucą wszystkie kulki wcześniej. Jako sprawdzenie dodaj do wątków liczniki ile kulek przerzucił dany wątek i wypisz je na koniec na ekran. Można zapobiec temu zjawisku synchronizując moment rozpoczęcia pracy przez wątki, ale tu znowu nie wystarczy muteks, najlepiej taką synchronizację zrobić na barierze lub zmiennej warunkowej, a te mechanizmy poznacie w przyszłym semestrze. {{< /details >}}


## Wątki i sygnały
## Zadanie 3 - wątki i sygnały z czekaniem na sygnał za pomocą funkcji sigwait

Cel:
Napisać program, który przyjmuje jeden parametr 'k' i co sekundę wyświetla listę liczb, początkowo od 1 do k. 
Program ma obsługiwać dwa sygnały za pomocą oddzielnego wątku. Wątek ten po otrzymaniu sygnału podejmuje następujące akcje:
- SIGINT  (C-c) ... usuwa losową liczbę z listy (jeżeli lista jest pusta to nic nie robi),
- SIGQUIT  (C-\\) ... ustawia flagę 'STOP' i kończy swoje działanie.

Wątek główny ma za zadanie co sekundę wyświetlić listę lub, jeżeli ustawiona jest flaga 'STOP', poprawnie zakończyć swoje działanie.

Co student musi wiedzieć:

- man 3p pthread_sigmask
- man 3p sigwait

**prog19.c**:
{{< includecode "prog19.c" >}}

### Uwagi i pytania

W strukturze argumentów argsSignalHandler_t przekazujemy wskazania na dane współdzielone przez oba wątki czyli tablicę i
flagę STOP oraz muteksy je chroniące. Dodatkowo maskę sygnałów i tid wątku obsługi sygnałów, które to dane nie są
współdzielone.

W wątkach do ustawiania maski sygnałów (per wątek) używamy pthread_sigmask, nie powinno się, gdy istnieje więcej wątków
niż jeden w programie używać f. sigprocmask.

Delegowanie oddzielnego wątku do obsługi sygnałów jest typowym i wygodnym sposobem radzenia sobie w programach
wielowątkowych.

Ile wątków jest w tym programie?
{{< details "Odpowiedź" >}} Dwa, wątek główny utworzony przez system (każdy proces ma ten jeden wątek) oraz watek utworzony przez nas w kodzie.  {{< /details >}}

Wypunktuj różnicę i podobieństwa pomiędzy sigwait i sigsuspend: 
{{< details "Odpowiedź" >}}
- sigwait nie wymaga funkcji obsługi sygnału tak jak sigsuspend
- obie metody wymagają blokowania sygnałów oczekiwanych
- sigwait nie jest przerywany przez funkcję obsługi sygnałów -  nie może to wynika z POSIX
- sigwait nie zmienia maski blokowanych sygnałów więc nie ma mowy o uruchomieniu obsługi sygnału blokowanego (jeśli by takowa była, w przykładzie nie ma) jak przy sigsuspend
{{< /details >}}

Po wywołaniu sigwait tylko jeden typ sygnału jest zdejmowany z wektora sygnałów oczekujących wiec problem jaki mieliśmy z obsługa wielu sygnałów w trakcie pojedynczego sigsuspend w przykładzie z L2  nie wystąpiłby jeśli zamienić użycie sigsuspend na sigwait (ćwiczenie do zrobienia)

Czy metoda czekania na potomka jest w tym programie równie słaba jak w poprzednim?
{{< details "Odpowiedź" >}}   Nie,  tutaj sprawdzanie co sekundę jest częścią zadania, gdyby jednak nie było to i tak unikniemy busy waitingu bo możemy się synchronizować na pthread_join. {{< /details >}}

Czy w tym programie można użyć sigprocmask zamiast pthread_sigmask?
{{< details "Odpowiedź" >}} Tak, wywołanie blokujące sygnały pojawia się zanim powstaną dodatkowe wątki. {{< /details >}}

Czemu nie sprawdzamy błędów wywołań funkcji systemowych związanych pozyskaniem i zwolnieniem muteksów
{{< details "Odpowiedź" >}} Podstawowy typ muteksu (a taki używamy w programie) nie sprawdza i nie raportuje błędów. Dodanie sprawdzania nie byłoby wcale złe, łatwiej można potem zmienić muteks na typ raportujący błędy.  {{< /details >}}

## Anulowanie wątków

Program może anulować wątek poprzez użycie funkcji `pthread_cancel`.

```
man 3p pthread_cancel
```

To jak wątek zareaguje na `pthread_cancel` zależy od jego ustawień, ustawianych funkcjami:

```
int pthread_setcancelstate(int state, int *oldstate);
int pthread_setcanceltype(int type, int *oldtype);
man 7 pthreads (specifically the "Cancelation Points" section)
```

W szczególności wątek może ignorować anulowanie poprzez ustawienie `PTHREAD_CANCEL_DISABLE` jak również z powrotem na nie reagować poprzez `PTHREAD_CANCEL_ENABLE`. Domyślnie anulowanie jest włączone.

Typ anulacji (__cancelacion type__) definiuje w jaki sposób wątek się zakończy po anulowaniu.
W przypadku `PTHREAD_CANCEL_DEFERRED` (domyślna wartość) wątek zakończy się na najbliższym "cancelation point".
W przypadku `PTHREAD_CANCEL_ASYNCHRONOUS` wątek zakończy się tak szybko, jak to możliwe.

Ogólnie rzecz biorąc bezpieczniej jest używać `PTHREAD_CANCEL_DEFERRED` jako że anulowanie asynchroniczne może prowadzić do nieoczywistych błędów, wątek nie ma jak po sobie "posprzątać" (o czym niżej).

### Cleanup handlers

Zwykle zakończenie wątku w trakcie może prowadzić do problemów, takich jak zakleszczenia (wątek zablokował mutex i już go nie odblokuje), resource leak (nie zwolnił zasobów), itd.

Aby temu zapobiec możliwe jest zdefiniowane tzw. "funkcji czyszczących" (ang. __cleanup handlers__), które zostaną wywołane, gdy wątek zakończy się w niestandardowy sposób (przez anulowanie lub `pthread_exit`). Taka funkcja ma syngaturę:

```
void function(void* arg);
```

i może zostać przypisana do wątku poprzez wywołanie `pthread_cleanup_push`.

```
void pthread_cleanup_push(void (*routine)(void*), void *arg);
```

Jak widać, jest nieco podobna do `pthread_create` - poza samą funkcją przekazujemy też jej parametry jako `void*`.

Przy zakończeniu wątku wszystkie funkcje czyszczące zostaną wywołane w odwrotnej kolejności niż były dodawane.

Funkcje czyszczące mogą być też usunięte przez przez wywołanie:

```
void pthread_cleanup_pop(int execute);
```

Ciekawostką jest tutaj argument `execute`, który pozwala wywołać usuwaną funkcję przed usunięciem.

Na każde wywołanie `pthread_cleanup_push` musi przypadać odpowiadające mu `pthread_cleanup_pop`.

```
man 3p pthread_cleanup_pop
```

### Czekanie na anulowane wątki

Na anulowane wątki możemy czekać używając `pthread_join`. Jeśli wątek został anulowany wskaźnik przekazany jako `value_ptr` zostanie ustawiony na `PTHREAD_CANCELED`.

```
man 3p pthread_join
man 3p pthread_exit (sekcja RATIONALE)
```


### Zadanie
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

**prog20.c**
{{< includecode "prog20.c" >}}

### Uwagi i pytania

Wątkom przekazuje się strukturę z aktualnym rokiem oraz z wskazaniem na 4 liczniki z muteksami, struktura argsModify_t
nie powiela błędu z zadania 2 czyli nie przekazuje indywidualnych kopii zbyt wielu wskaźników.

Struktura `studentsList_t` jest używana jedynie przez wątek główny, nie jest "widziana" przez wątki studentów.

"Sprytna" inicjalizacja struktury yearCounters_t nie zadziała z archaicznymi standardami języka C (c89/c90). Warto
wiedzieć, oczywiście używamy wszelkich dobrodziejstw nowszych standardów.

Cleanup handlery w funkcji wątku służą do bezpiecznego usunięcia studenta z licznika roku w razie anulowania w czasie
spania, bez nich takie anulowanie pozostawiłoby studenta na roku do końca działania programu!

Pamiętaj, że pthread_cleanup_push musi być sparowany z pthread_cleanup_pop w obrębie tego samego bloku {}

Ile w programie występuje mutexów i co chronią?
{{< details "Odpowiedź" >}} Dokładnie 4, każdy chroni inny współdzielony licznik. {{< /details >}}

Czy aktualny rok studenta musi być częścią struktury argsModify_t?
{{< details "Odpowiedź" >}} Nie, równie dobrze mógłby być automatyczną zmienną utworzoną w wątku, nie byłaby wtedy potrzebna struktura argsModify_t , wątkom przekazywalibyśmy wskaźnik na strukturę yearCounters.  {{< /details >}}

Co to znaczy, że wątek ma stan anulowania PTHREAD_CANCEL_DEFERRED ?
{{< details "Odpowiedź" >}} Anulowanie nastąpi dopiero podczas wywołania funkcji z listy funkcji "cancelation points" {{< /details >}}

Które z funkcji użytych w funkcji wątku są punktami anulowania wątku?
{{< details "Odpowiedź" >}} W tym kodzie  tylko nanosleep (wywołany z msleep). {{< /details >}}

Skąd wiemy, które funkcje są takimi punktami?
{{< details "Odpowiedź" >}} $man 7 pthreads {{< /details >}}

Co oznacza jedynka w wywołaniu: pthread_cleanup_pop(1); ?
{{< details "Odpowiedź" >}} Oznacza, że poza zdjęciem handlera ze stosu jest on też wykonywany. {{< /details >}}

Kiedy jest zmniejszany liczniku roku?
{{< details "Odpowiedź" >}} W dwóch przypadkach, podczas anulowania (rzadki przypadek) oraz  podczas zdjęcia handlera decrement_counter ze stosu cleanup handlerów. {{< /details >}}

W algorytmie losowania wątku do anulowania jest spory błąd logiczny, gdzie i jakie niesie zagrożenie?
{{< details "Odpowiedź" >}} Losowy  wybór wątku do anulowania  może trwać bardzo długo gdy na dużej liście wątków zostanie np. tylko 1 nie anulowany.  Można to zaobserwować, jeśli wywołać program z parametrem 10. {{< /details >}}

Jako ćwiczenie popraw sposób losowania "żyjącego" wątku do anulowania.

Zwróć uwagę na sposób odliczenia 4 sekund w losowych interwałach za pomocą clock_gettime i nanosleep,  to można zmienić, dodając wywołanie f.alarm i oddając obsługę sygnału SIGALRM. Rozwiązanie takie można wykonać jako ćwiczenie.

Wykonaj przykładowe zadania. Podczas laboratorium będziesz miał więcej czasu oraz dostępny startowy kod, jeśli jednak wykonasz poniższe zadania w przewidzianym czasie, to znaczy, że jesteś dobrze przygotowany do zajęć.

- [Zadanie 1]({{< ref "/sop1/lab/l3/example1" >}}) ~60 minut
- [Zadanie 2]({{< ref "/sop1/lab/l3/example2" >}}) ~120 minut
- [Zadanie 3]({{< ref "/sop1/lab/l3/example3" >}}) ~100 minut

## Kody źródłowe z treści tutoriala

{{% codeattachments %}}
