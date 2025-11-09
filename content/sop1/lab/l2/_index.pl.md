---
title: "L2 - Procesy, sygnały i deskryptory"
date: 2022-02-05T19:10:36+01:00
weight: 30
---

# Tutorial 2 - Procesy, sygnały i deskryptory

{{< hint info >}}
Ten tutorial zawiera wyjaśnienia działania funkcji wymaganych na laboratoriach oraz ich parametrów.
Jest to jednak wciąż zbiór jedynie poglądowy najważniejszych informacji -- 
należy **koniecznie przeczytać wskazane strony manuala**, aby dobrze poznać i zrozumieć wszystkie szczegóły.
{{< /hint >}}

## Zarządzanie procesami

### Tworzenie procesów
Stworzenie procesu potomnego wykonawane jest za pomocą polecenia `fork`. Przyjrzymy się definicji tej funkcji: 

```
pid_t fork(void)
```

Jak widać, zwraca ona obiekt typu `pid_t`, jest to typ całkowitoliczbowy ze znakiem. W procesie rodzica funkcja zwraca identyfikator nowo utworzonego procesu, natomiast w utworzonym procesie zwracane jest `0`, pozwala to łatwo rozdzielić logikę na tę wykonywaną przez dzieci oraz na tę wykonywaną prez rodzica.


Oczywiście stworzenie nowego procesu może się nie powieść (np. gdy systemowi zabraknie potrzebnych zasobów). W takim wypadku funkcja `fork` zwraca wartość `-1` i ustawia odpowiednią wartość zmiennej `errno`.

Procesy utworzone przez dany proces nazywamy jego __dziećmi__, natomiast z perspektywy procesu potomnego, proces który go stworzył nazywamy __rodzicem__.

### Identyfikacja procesów
Każdy proces posiada unikatowy identyfikator typu `pid_t`. 
Aby uzyskać informację o identyfikatorze procesu używamy funkcji `getpid()`, natomiast aby dowiedzieć się jaki jest idenyfikator procesu rodzica funkcji  `getppid()`. 
Ich definicje są następujące: 

```
pid_t getpid(void)
pid_t getppid(void)
```

Jak widać nie przyjmują one żadnych argumentów i zwracają obiekt typu `pid_t`.:

Zgodnie ze standardem POSIX obie funkcję __zawszę kończą się sukcesem__.


### Zadanie 

Napisz program tworzący n procesów potomnych (n jest parametrem na pozycji 1), każdy z tych procesów czeka przez losowy czas [5-10] sekund po czym wypisuje na ekran swój PID i się kończy. Proces rodzica co 3s ma wyświetlać na stdout ile jeszcze posiada pod-procesów. Narazie nie przejmuj się czekaniem na zakończenie procesów potomnych.

### Rozwiązanie zadania

Nowe strony z manuala: 
- man 3p fork
- man 3p getpid
- man 3p wait
- man 3p waitpid
- man 3p sleep
- <a href="https://www.gnu.org/software/libc/manual/html_node/Job-Control.html">Job Control</a>

<em>rozwiązanie 1 etap <b>prog13a.c</b>:</em>
{{< includecode "prog13a.c" >}}

### Uwagi i pytania

- Upewnij się, że wiesz jak powstaje grupa procesów tzn. kiedy powłoka tworzy nową grupę i jakie procesy do niej należą.

- Zwróć uwagę, że w makrze ERR dodano kill(0, SIGKILL). Chodzi o to aby w razie błędu zatrzymać cały program (wszystkie
procesy danej grupy).

- Zero jako argument kill jest bardzo użyteczne, odnosi się do wszystkich procesów danej grupy, nie musimy przechowywać
listy PID'ów.

- Zwróć uwagę, że nie analizujemy błędów funkcji kill w makrze ERR, to dla tego, że w przypadku sytuacji krytycznej
podejmujemy jak najmniej działań, zresztą co mielibyśmy zrobić wywołać rekurencyjnie ERR?

- Pytanie czemu po uruchomieniu wraca linia poleceń?
{{< details "Odpowiedź" >}} Proces rodzic nie czeka na zakończenie procesów potomnych, brak wywołania wait lub waitpid, naprawimy to w 2 etapie {{< /details >}}

- Jak sprawdzić kto jest rodzicem procesów prog13a? Dlaczego właśnie ten proces?
{{< details "Odpowiedź" >}} Zaraz po uruchomieniu wykonać np.: $ps -f, rodzicem procesów będzie proces o PID 1 czyli init/systemd. Dzieje się tak dlatego, że proces rodzic kończy się bez czekania na procesy potomne a te nie mogą "wisieć" poza drzewem procesów. Nie przechodzą pod powłokę ale właśnie pod proces po PID=1 {{< /details >}}

- Zwróć uwagę na miejsce i sposób ustalenia ziarna liczb losowych. Czy jeśli przesuniemy srand z procesu dziecka do rodzica to zmieni to działanie programu? Jak?
{{< details "Odpowiedź" >}}  Procesy potomne będą losowały te sam liczby ponieważ zaczynają z tym samym ziarnem, tak być nie może. {{< /details >}}

- Czy można jako argument srand  podać time() zamiast getpid()?
{{< details "Odpowiedź" >}}  Nie można, wywołania time() odbędą się w tej samej sekundzie wiec ziarna będą takie same, efekt taki sam jak poprzednio. {{< /details >}}

- Zwróć uwagę na losowanie liczby z przedziału [A,B]. Niby oczywiste, ale warto opracować sobie prosty wzór.

- Jak zachowa się program bez exit  wywołanym zaraz po child_work w procesie potomnym?
{{< details "Odpowiedź" >}} Procesy potomne po zakończeniu wykonania kodu funkcji child_work przechodzą dalej w kodzie czyli wracają do pętli forkującej i tworzą własne procesy potomne, które to też mogą utworzyć swoje potomne itd. Niezły bałagan, do tego procesy dzieci nie czekają na swoje dzieci.  {{< /details >}}

- Ile procesów (w sumie) zostanie utworzonych jeśli jako parametr uruchomienia podamy 3 przy założeniu, że nie ma exit wspomnianego w poprzenim pytaniu?
{{< details "Odpowiedź" >}}  1 rodzic 3 potomne 1 poziomu,  3 drugiego i  1 trzeciego poziomu, w sumie 8 procesów, narysuj sobie drzewko, opisz gałęzie wartością n w momencie wykonania forka. {{< /details >}}

- Co zwraca sleep?  Czy powinniśmy jakoś na to reagować? 
{{< details "Odpowiedź" >}} zwraca czas "niedospany" w przypadku przerwania funkcją obsługi sygnału w tym programie proces dziecka nie dostaje sygnałów i ich nie obsługuje więc nie musimy się tym martwić. W innych programach często będziemy restartować sleep z nowym czasem spania tak aby w sumie przespać tyle ile było założone. {{< /details >}}

- W kolejnym etapie dodamy czekanie i zliczanie procesów potomnych. Pytanie skąd mamy wiedzieć ile procesów potomnych w danej chwili istnieje?
{{< details "Odpowiedź" >}} Można próbować zliczać sygnały SIGCHLD ale to zawodny sposób bo mogą się "sklejać" czyli mniej ich dostaniemy niż potomków się na prawdę zakończyło. Jedyną pewną metodą jest zliczanie udanych wywołań wait i waitpid. {{< /details >}}

### Czekanie na procesy potomne
Aby nie dopuścić do żadnego wycieku, przed zakończeniem procesu rodzica należy zaczekać na zakończenie wszystkich procesów potomnych, możemy to zrobić korzystając z funkcji `wait`, która czeka na dowolony proces potomny, lub funkcji `waitpid`, która umożliwia określenie na które procesy czekamy. 
Przyjrzyjmy się ich definicjom:

```
pid_t wait(int *stat_loc);
pid_t waitpid(pid_t pid, int *stat_loc, int options);
```

Jak można zauważyć, obie funkcje zwracają obiekt typu `pid_t` będący identyfikatorem procesu, o którego stanie dostaniemy informację. 

Obie funkcje przyjmują argument `stat_loc` typu `int*`, który wskazuje miejsce w pamięci, do którego ma zostać zapisana informacja o odczytanym stanie procesu (jeżeli informacja ta nie będzie nam potrzebna możemy wstawić w to miejsce `NULL`).

Funkcja `waitpid` posiada dwa dodatkowe argumenty, kolejno `pid` typu `pid_t` oraz `options` typu `int`.

 Argument `pid` określa, na które procesy chcemy czekać. Dla różnych wartości funkcja zachowuje się następująco:
- `pid == -1` - czekamy na dowolny proces potomny.
- `pid > 0` - czekamy na proces o identyfikatorze równym `pid`.
- `pid == 0` - czekamy na dowolny proces którego identyfikator grupy jest równy identyfikatorowi grupy procesu wywołującego.
- `pid < -1` czekamy na dowolny proces którego identyfikator grupy jest równy wartości bezwzględnej `pid`.

argument `options` określa modyfikacje sposobu działania funkcji, i jest kombinacją następujących opcji:
- `WCONTINUED` - funkcja powinna zwrócić również informacje o procesach, które zostały wznowione po zatrzymaniu.
- `WNOHANG` - funkcja `waitpid` nie powinna zatrzymywać wywołania biężącego procesu jeżeli żaden z procesów na które czekamy nie może natychmiast powiadomić o swoim statusie.W takiej sytuacji funkcja zwaraca wartość `0`.
- `WUNTRACED` - funkcja powinna zwrócić również informacje o procesach, które zostały zatrzymane.
  
W ramach laboratorium wystarczy znajomość opcji `WNOHANG`.

Podsumowując możemy patrzeć na funkcję `waitpid` jako na bardziej rozbudowaną wersję funkcji `wait` - wywołanie funkcji `wait(stat_loc)` jest równażne wywołaniu `waitpid(-1, stat_loc, 0)`.

Oczywiście obie te funkcję mogą się nie powieść, zwracają one wtedy `-1` i ustawiają odpowiednią zmienną wartość errno. 

__Uwaga:__ Jeżeli wywołamy funkcję `wait` lub `waitpid` i pula procesów potomnych na,  które możemy czekać będzie pusta funkcja zwróci `-1` i ustawi wartość zmiennej `errno` na `ECHILD`. Warto z tego skorzystać by mieć pewność że nie osierocimy żadnych procesów przed zakończeniem procesu.

### Zadanie

Rozbuduj program z poprzedniego zadania o poprawne czekanie na procesy potomne.

### Rozwiązanie zadania

<em> rozwiązanie <b>prog13b.c</b>:</em>
{{< includecode "prog13b.c" >}}

### Uwagi i pytania
- Koniecznie trzeba rozróżnić kiedy waitpid informuje nas o chwilowym braku zakończonych potomków (wartość zwracana zero) od permanentnego braku potomków (błąd ECHILD). Ten ostatni przypadek nie jest w zasadzie błędem, tę sytuację trzeba normalnie obsłużyć. 

- Czemu wołamy waitpid w pętli?
{{< details "Odpowiedź" >}} Nie wiemy z góry ile procesów zombie czeka, może być 0 , 1 , 2 aż do n {{< /details >}}

- Czemu wołamy waitpid z flagą WNOHANG?
{{< details "Odpowiedź" >}} Nie chcemy czekać nie wiadomo ile na jeszcze żyjące procesy, działamy w pętli, która co 3 sekundy ma wypisać dane {{< /details >}}

- Czemu wołamy waitpid z pidem zero?
{{< details "Odpowiedź" >}} Czekamy na dowolny proces potomny, nie musimy znać jego PID, zero oznacza dowolny z potomków. {{< /details >}}

- Czy w tym programie występują sygnały? 
{{< details "Odpowiedź" >}} SIGCHILD, nie ma f. obsługi ale to nie szkodzi, w pewnym sensie jest obsługiwany przez waitpid {{< /details >}}

- Czy tym razem nie powinniśmy sprawdzać co zwraca sleep skoro są sygnały?
{{< details "Odpowiedź" >}} Nie bo nie ma funkcji obsługi sygnału. {{< /details >}}

## Zadanie 2 - sygnały

Cel: Program przyjmuje 4 parametry pozycyjne (n,k,p i r). Tworzy n procesów potomnych. Proces rodzic wysyła
naprzemiennie sygnały SIGUSR1 i SIGUSR2 do wszystkich procesów potomnych w pętli po odpowiednio k i p sekundach. Kończy
się gdy kończą się wszystkie procesy potomne. Każdy proces potomny losuje czas swojego spania z przedziału 5-10 sekund a
następnie w pętli śpi i wypisuje na ekran SUKCES jeśli ostatnim otrzymanym przez niego sygnałem był SIGUSR1 lub FAILURE
jeśli SIGUSER2. Taka pętla powtarza się r razy.

Co student musi wiedzieć:
- man 7 signal
- man 3p sigaction
- man 3p nanosleep
- man 3p alarm
- man 3p memset
- man 3p kill

<em>plik <b>prog14.c</b>:</em>
{{< includecode "prog14.c" >}}

Do komunikacji pomiędzy funkcją obsługi sygnału a resztą programu musimy użyć zmiennych globalnych, proszę pamiętać, że
jest to sytuacja wyjątkowa, zmienne globalne są jako takie niepożądane oraz, co powinno być oczywiste, ale czasem się
studentom myli - nie są współdzielone pomiędzy procesami pokrewnymi.

Typ zmiennej globalnej nie jest przypadkowy, co więcej jest to jedyny BEZPIECZNY i POPRAWNY typ. Wynika to z
asynchronicznej natury wywołania f. obsługi sygnału a dokładniej: Primo "volatile" oznacza wyłączenie optymizacji
kompilatora, ważne żeby kompilator nie uznał wartości zmiennej za stałą bo jej zmiany nie wynikają z kodu i tak mogłoby
się okazać, że czytelna dla nas pętla while(work) gdzie work jest zmienną globalną zmienia się na while(1) po
optymizacji. Secundo sig_atomic_t oznacza największy typ numeryczny, który jest przetwarzany w pojedynczej instrukcji
CPU. Jeśli weźmiemy większy typ numeryczny przerwanie obsługą sygnału może zakłócić wartość wynikową nawet prostego
porównania a==0 o ile przerwanie wypadnie w trakcie porównania i zmieni już porównane bajty.

Z powyższego wynika, że nie przekazujemy pomiędzy funkcją obsługi a głównym kodem nic poza prostymi liczbami, stanami.
Do tego dochodzi dobra praktyka nie przerywania programu na zbyt długo co pozostawia nam bardzo mało poprawnych,
przenośnych i bezpiecznych rozwiązań w kwestii jak dzielić logikę programu pomiędzy główny kod a funkcję obsługi
sygnału. Najprostsza zasada aby funkcje obsługi były ekstremalnie krótkie (przypisanie, inkrementacja zmiennej itp) a
cała logika pozostała w głównym kodzie jest najlepsza.

Funkcja memset bywa konieczna a zazwyczaj jest użyteczna przy inicjowaniu nie w pełni znanych nam struktur (nie wiemy
jakie tam są jeszcze pola w danej implementacji).

Zwróć uwagę, że obsługa sigchild w pętli jest prawie identyczna jak poprzednio w pętli.

Czy podczas obsługi sygnału SIGCHLD można się spodziewać więcej niż jednego zakończonego procesu dziecka?
{{< details "Odpowiedź" >}}  Tak, sygnały mogą się skleić, dziecko może się zakończyć akurat w trakcie obsługi  {{< /details >}}

Czy podczas obsługi sygnału SIGCHLD można się spodziewać braku zakończonego procesu dziecka? Zerknij na zakończenie main
{{< details "Odpowiedź" >}}  Tak, wait na końcu main może "podkradać" te czekające zombie tj. wywoła się poprawnie wait zanim wykona się funkcja obsługi.  {{< /details >}}

<em>plik <b>prog14.c</b>:</em>
{{< includecode "prog14.c" >}}

Pamiętaj o możliwym KONFLIKCIE sleep i alarm - wg. POSIX sleep może używać w implementacji SIGALRM a nie ma jak
zagnieżdżać sygnałów, nigdy zatem w kodzie oczekującym na alarm nie używamy sleep, można za to użyć nanosleep tak jak w
kodzie powyżej.

W wysyłaniu sygnałów (kill) pojawia się jako PID zero, dzięki temu nie musimy znać pidów procesów potomnych ale też
wysyłamy sygnał sami do siebie!

Miejsca ustawienia obsługi sygnałów i ich blokowania w tym programie są bardzo ważne, zwróć uwagę jak to działa i
odpowiedz na pytania poniżej. Pamiętaj aby zawsze dobrze przemyśleć kolejność tych ustawień w swoim programie, z tym
jest związanych sporo błędów w pracach studentów!

Zwróć uwagę na sleep, czemu jest w pętli? Czy czas spania jest/mógłby być dokładny?
{{< details "Odpowiedź" >}}  sleep jest przerywane przez obsługę sygnału, zatem restart jest konieczny. Ponieważ sleep zwraca pozostały czas spania w sekundach to z uwagi na zaokrąglenia nie można po restarcie uzyskać dokładnego czasu spania.   {{< /details >}}

Jaka jest domyślna dyspozycja znakomitej większości sygnałów (w tym SIGUSR1 i 2)?
{{< details "Odpowiedź" >}} Zabicie procesu do którego wysyłamy, w tym programie brak reakcji (funkcji obsługi), blokowania lub ignorowania sygnałów SIGUSR1 i 2 skutkowałoby przedwczesnym zabiciem procesów. {{< /details >}}

Jaka jest konsekwencja wysyłana przez proces rodzic sygnałów SIGUSR1/2 do całej grupy procesów?
{{< details "Odpowiedź" >}} Proces rodzic musi jakoś reagować na te sygnały mimo, że nie są one mu potrzebne do pracy, zatem je ignoruje. {{< /details >}}

Co by się stało, gdyby nie było włączone ignorowanie SIGUSR1i2 w procesie rodzicu?
{{< details "Odpowiedź" >}} Proces rodzic zabiłby się pierwszym wysyłanym sygnałem. {{< /details >}} 

Czy można przesunąć ustawienie ignorowania sygnałów za funkcję create_children? Procesy potomne przecież nie potrzebują tego ignorowania, mają od razu po starcie ustawianą funkcję obsługi? 
{{< details "Odpowiedź" >}} Nie można, mogłoby się zdarzyć (choć rzadko), że procesy potomne zostałyby tylko utworzone (a nie zaczęte) a przydział CPU po tym utworzeniu przeszedłby do procesu rodzica. który zdążyłby wysłać sygnał SIGUSR1 do dzieci. Kolejny przydział CPU do procesu dziecka/dzieci spowodowałby najpierw obsługę sygnału a to oznaczałoby zabicie! {{< /details >}}

Czy można jakoś zmienić ten program tak aby wykluczyć ignorowanie sygnałów SIGUSR1i2?
{{< details "Odpowiedź" >}} Ten akurat program może mieć identyczną reakcję na te sygnały w rodzicu i potomkach, można zatem ustawić obsługę od razu w procesie rodzicielskim przed fork. {{< /details >}}

A co się stanie jeśli za fork  przeniesiemy obsługę SIGCHLD? 
{{< details "Odpowiedź" >}} Jeśli jeden z procesów potomnych "umrze" zanim włączymy tą obsługę to będzie on "zombie" aż do momentu gdy kolejny w pod-procesów  się zakończy. Nie jest to bardzo duży błąd ale warto i na takie zwracać uwagę. {{< /details >}}

Pytanie, czy wait na końcu main jest potrzebny? Przecież i tak funkcja parent_work() powinna działać co najmniej tyle czasu co najdłuższy z podprocesów?
{{< details "Odpowiedź" >}} Wyliczenie czasu w pętli rodzica nie wystarczy, w obciążonym systemie możliwe są dowolnie długie opóźnienia, bez wait powstaje zatem tzw. race condition - kto się pierwszy zakończy rodzic czy potomne procesy. {{< /details >}}

## Czekanie na sygnał

Często podczas pisania programów natkniemy sie na sytuację, w której proces, zanim wykona prace musi zostać poinformowany o tym, że inny proces zakończył swoje zadanie.
Jak się pewnie domyślasz, ten problem można łatwo rozwiązać z wykorzystaniem __sygnałów__. Inspirując się poprzednim zadaniem, moglibyśmy napisać logikę, która opiera się na tym, że nasz proces
śpi w pętli i sprawdza, czy ostatni otrzymany przez niego sygnał to ten, na który czeka. Niestety nie dość, że to rozwiązanie jest nieeleganckie, to jeszcze jest __niepoprawne__ - mogło by dojść do sytuacji że sygnał na który czekamy zostanie "sklejony" z innym sygnałem i tak naprawdę nigdy nie dowiedzielibyśmy się, że nasz proces może rozpocząć pracę.
Na szczęscie system operacyjny dostarcza nam narzędzia, które pozwalają rozwiązać ten problem.

Aby zablokować program do momentu, gdy otrzyma on sygnał będziemy używać funkcji `sigsuspend`. Przyjrzyjmy się jej definicji:

```
int sigsuspend(const sigset_t *sigmask);
```

Jak możemy zauważyć funkcja ta zwaraca wartość typu `int` służącą do powiadomienia o potencjalnym błędzie, zwracane wtedy jest `-1`. Przyjmuje argument `sigmask` typu `const sigset_t *` jest to wskaźnik na zbiór sygnałów, na które funkcja będzie czekać. 

Działanie tej funkcji jest następujące: ustawia ona maskę sygnałów na tę podaną w argumencie, czeka na przechwycenie jednego z tych sygnałów, po czym przywraca poprzednią maskę sygnałów i wznawia wykonywanie procesu.

### Zarządzanie maską sygnałów

Zbiór sygnałów będzie nazywać maską sygnałów. Maskę sygnałów będziemy przechowywać jako obiekt o typie `sigset_t`. Standard nie określa w jaki sposób ma być zaimplementowany ten typ, może być to zarówno `int`, jak i struktura. 
W celu modyfikacji maski sygnałów będziemy używac funcji `sigsemptyset`, inicjalizującej maskę jako zbiór pusty, oraz `sigaddset` ,dodającej sygnał do maski. 
Przyjrzyjmy się ich definicją:

```
int sigemptyset(sigset_t *set);
int sigaddset(sigset_t *set, int signo);
```

Jak możemy zauważyć, obie funkcję przyjmują jako pierwszy argument `set` typu `sigset_t *`, jest to wskaźnik na maskę, którą chcemy edytować. 
funkcja `sigaddset` dodatkowo przyjmuje argument `signo` będący kodem sygnału, który chcemy dodać do maski.

Obie funkcje zwracają wartość typu int, służącą do sygnalizacji przebiegu operacji: w razie sukcesu zwracają `0`, a w razie błędu `-1`, ustawiając odpowiednią wartość zmiennej `errno`.

### Zmiana maski sygnałów

Skoro już zdefiniowaliśmy nową maskę sygnałów, chcielibśmy sprawić, by wpłynęła ona na działanie naszego procesu. 
W tym celu będziemy używać funkcji `sigprocmask`, która określa, w jaki sposób zdefiniowana przez nas maska sygnałów ma wpłynąć na aktulaną maskę sygnałów procesu.
Przyjrzyjmy się jej definicji:
```
int sigprocmask(int how, const sigset_t *restrict set,
           sigset_t *restrict oset);
```

Jak możemy zauważyć funkcja ta przyjmuje kolejno argumenty:
- `how` typu `int` określa w jaki sposób nowa maska ma wpłynąć na aktualną maskę. Dostępne opcje:
  - `SIG_BLOCK` - wynikowa maska sygnałów jest sumą zbiorów maski wskazanej przez `set` i aktualnej maski sygnałów (określamy jakie sygnały checmy __dodać__ do maski).
  - `SIG_SETMASK` - wynikowa maska sygnałów jest maską sygnałów wskazywaną przez `set`.
  - `SIG_UNBLOCK` - wynikowa maska sygnałów jest przecięciem aktualnej maski i dopełnieiem zbioru maski wskazanej przez `set` (Określamy jakie sygnały chcemy __usunąć__ z maski).

- `set` typu `const sigset_t` to wskaźnik na maskę, na podstawie której chcemy modyfikować poprzednią maskę.
- `oset` typu `sigset_t *` jest wskaźnikiem na obiekt do którego chcemy zapisać maskę sygnałów sprzed edycji.


### Zadanie
Napisz program, który tworzy jeden pod-proces, który co określony parametrami czas (w mikrosekundach) wysyła do procesu rodzica
SIGUSR1, co n-ty raz (określony parametrami) wysyła SIGUSR2. Proces rodzic czeka na otrzymanie sygnału SIGUSR2. Oba
procesy zliczają i wypisują na bieżąco ilości wysłanych/odebranych SIGUSR2. Używamy część funkcji z poprzedniego
zadania.

### Rozwiązanie zadania

Nowe strony z manuala
- man 3p sigsuspend
- Opis metod czekania na sygnał w glibc <a href="http://www.gnu.org/software/libc/manual/html_node/Waiting-for-a-Signal.html#Waiting-for-a-Signal"> tutaj</a>
- man 3p pthread_sigmask (tylko opis sigprocmask)
- man 3p sigaddset
- man 3p sigemptyset

<em>rozwiązanie, plik <b>prog15.c</b>:</em>
{{< includecode "prog15.c" >}}

### Uwagi i pytania

- Program działa aż do SIGINT (C-c)

- Uruchom z różnymi parametrami, z małą przerwą w mikrosekundach i częstymi SIGUSR2 różnica w licznikach powinna wzrastać
szybciej niż gdy te parametry są większe. Za chwilę wyjaśni się skąd te błędy. Jeśli nie obserwujesz błędów spróbuj dać
programowi trochę więcej czasu na działanie, około minuty powinno dać już różne wyniki.

- Program jest celowo napisany tak, aby występowały pewne problemy, można go lepiej napisać i je wyeliminować, proszę miej
to na względzie kopiując rozwiązania z tego kodu!

- Zwróć uwagę na getppid, niby oczywiste ale widziałem prace gdzie studenci przekazywali pid rodzica do procesu potomnego.

- Oczekiwanie na sygnał poprzez sigsuspend jest bardzo ważną technika, którą musisz rozumieć, dobrze omawia to
dokumentacja GNU podlinkowana powyżej. Pamiętaj, że zasada jest taka, blokujemy oczekiwane sygnały w całej pętli (
większej części programu) i przywracamy ich dochodzenie tylko wtedy gdy możemy czekać czyli właśnie w wywołaniu
sigsuspend. Mamy zatem w programie dobrze określone momenty w których obsługujemy sygnały, to spore ułatwienie.

- Jeśli funkcje obsługujące sygnały mogą być wykonywane tylko w czasie nieaktywności programu głównego (czyli jak w tym
przykładzie) można nieco rozluźnić zasady tego co wolno przekazywać przez zmienne globalne, oraz te dotyczące długości i
czasu ich działania.

- Który licznik jest niepoprawny, ten w procesie dziecka czy rodzica?
{{< details "Odpowiedź" >}} To musi być ten który jest mniejszy, nie mamy w tym kodzie możliwości zliczania nieistniejących sygnałów, możemy tylko gubić te wysłane. Gubić może oczywiście tylko odbiorca. Problem jest w procesie rodzica. {{< /details >}}

- Pytanie czemu liczniki się różnią ? 
{{< details "Odpowiedź" >}} 1. sklejanie sygnałów ale to ma mały wpływ, 2.  fakt, że sigsuspend NIE GWARANTUJE WYKONANIA MAKSYMALNIE JEDNEJ OBSŁUGI SYGNAŁU! To częsty błąd w rozumowaniu! Zaraz po wywołaniu obsługi SIGUSR2 jeszcze w obrębie jednego wykonania sigsuspend następuje obsługa SIGUSR1, zmienna globalna jest nadpisywana i proces rodzic nie ma szansy zliczyć SIGUSR2!!! {{< /details >}}

- Jak uruchomić ten program aby zminimalizować szansę na sklejanie się SIGUSER2 i jednocześnie obserwować  różnice w licznikach?
{{< details "Odpowiedź" >}} Uruchomić dla małych czasów i dużych ilości SIGUSR1 między SIGUSR2, teraz jest prawie zerowa szansa na sklejenie SIGUSR2, za to duża na wykonanie wiele razy funkcji obsługi sygnału w obrębie jednego sigsuspend {{< /details >}}

- Popraw powyższy program tak aby wyeliminować problem wielu wywołań obsługi sygnału w obrębie jednego sigsuspend 
{{< details "Odpowiedź" >}} Można to zrobić np. dodając drugą zmienną globalną tylko do obsługi SIGUSR2, zwiększanie zmiennej count też można przenieść do funkcji obsługi sygnału w ten sposób uniknie się potencjalnego problemu z obsługą dwóch SIGUSR2  w obrębie jednego sigsuspend. Trzeba jeszcze przebudować kod związany z wypisywaniem zmienionego licznika count w rodzicu i gotowe. {{< /details >}}

## Zadanie 4 - operacje niskopoziomowe na plikach a sygnały

Cel: Zmodyfikować program z zadania 3 tak aby proces rodzic odbierał sygnały SIGUSR1 wysyłane co zadany czas (parametr 1) i zliczał je.  Dodatkowo proces główny tworzył plik o nazwie podanej jako parametr 4 o zadanej ilości bloków o zadanym rozmiarze (parametry 2 i 3). Zawartość pliku ma pochodzić z /dev/urandom. Każdy blok kopiujemy osobno, kontrolując rozmiary. Po skopiowaniu bloku należy podać na stderr realną ilość przepisanych bloków oraz stan liczników sygnałów.
Co student musi wiedzieć: 
- man 4 urandom

{{< hint info >}}
Tym razem rozwiązanie jest podzielone na 2 możliwe do uruchomienia etapy.
{{< /hint >}}

<em>rozwiązanie 1 etap, plik <b>prog16a.c</b>:</em>
{{< includecode "prog16a.c" >}}

Pamiętaj, z pliku /dev/random możesz pobrać na prawdę losowe bajty ale w małych ilościach, z /dev/urandom odwrotnie,
pseudo losowe liczby za to w dowolnych ilościach.

Powinieneś obserwować następujące problemy podczas uruchamiania z parametrami 1 20 40 out.txt :

Kopiowanie krótszych bloków niż zadano, na moim laptopie nigdy nie przekraczam 33554431 a powinno być 40MB, ale
pojawiają się też i krótsze, powód to przerwanie odczytu (W TRAKCIE) obsługą sygnału

fprintf: Interrupted system call - przerwanie funkcją obsługi sygnału funkcji fprintf ZANIM ta coś wyświetli

Analogiczne komunikaty dla open i close - może to być trudno zaobserwować w tym programie ale jest to możliwe wg. POSIX

Jak sobie z tymi efektami radzić pokazujemy w następnym etapie.

Zawsze gdy w poprawnym programie pojawia się alokacja pamięci musi być też jej zwalnianie!

Uprawnienia podawane w funkcji open mogą być także podane przy użyciu stałych (man 3p mknod), wyjątkowo ze względu na
bardzo silne zakorzenienie notacji oktalnej u programistów i administratorów oraz na fakt, że łatwo takie liczby w
kodzie wyszukać nie uznajemy tego za błąd stylu tzw. "magic numbers".

Widać, że zliczamy w rodzicu mniej sygnałów niż wysyła potomek, ponieważ sumowanie odbywa się bezpośrednio w nieblokowanej obsłudze sygnału to łatwo się domyślić, że w grę wchodzi sklejanie się sygnałów, pytanie czemu w tym programie to sklejanie jest aż tak silne?
{{< details "Odpowiedź" >}}  w tej architekturze (GNU/Linux) planista procesora blokuje uruchomienie obsługi sygnału podczas większych operacji IO, w tym czasie sygnały się sklejają. {{< /details >}}

W jakim celu proces rodzic na zakończenie wysyła do całej grupy SIGUSR1?
{{< details "Odpowiedź" >}} Aby zakończyć proces potomny. {{< /details >}}

Jak proces potomny może się zakończyć po nadejściu SIGUSR1 skoro dziedziczy obsługę tego sygnału?
{{< details "Odpowiedź" >}} Zaraz po starcie potomka przywracana jest domyślna reakcja na ten sygnał, która właśnie zapewnia zabicie procesu. {{< /details >}}

Czemu proces rodzic nie zabija się sam tym sygnałem?
{{< details "Odpowiedź" >}} Ma włączoną obsługę tego sygnału zanim wyśle sygnał do grupy. {{< /details >}}

Czy taka strategia może się  nie powieść?
{{< details "Odpowiedź" >}} Tak, jeśli proces rodzic upora się ze swoim zadaniem zanim proces potomny zmieni dyspozycję odnośnie SIGUSR1 na domyślną. {{< /details >}}

Czy można to jakoś poprawić? Tzn. proces rodzic zawsze zabije potomka ale jednocześnie sam nie narazi się na przedwczesną śmierć?
{{< details "Odpowiedź" >}} Wyślij do potomka SIGUSR2. {{< /details >}} 

Czy taka strategia zakończenia potomka zawsze jest poprawna i  łatwa do przeprowadzenia?
{{< details "Odpowiedź" >}} Tylko jeśli proces zabijany nie posiada zasobów, jeśli by takowe posiadał to musisz dodać obsługę sygnału kończącego co nie musi być  łatwe. {{< /details >}}

Czemu po wywołaniu alokacji pamięci sprawdzamy przydzielony wskaźnik czy aby nie jest NULL? 
{{< details "Odpowiedź" >}} System może nie móc nam przydzielić więcej pamięci, musimy być na to przygotowani. Brak tego sprawdzania jest bardzo częstym błędem w pracach studentów. {{< /details >}}

Czy nie dałoby się tego bufora uczynić zmienną automatyczną i uniknąć kodu związanego z alokacją i zwalnianiem?
{{< details "Odpowiedź" >}} Nie ma w znanych mi architekturach tak dużych stosów aby można było  na nich umieszczać tak duże zmienne (40MB dla przykładowych parametrów), zazwyczaj stos ma wymiar do kilku megabajtów, gdyby  nasz bufor był mały (kilka KB) to moglibyśmy tak zrobić. {{< /details >}}  

Czemu uprawnienia do nowego pliku są  pełne (0777)? 
{{< details "Odpowiedź" >}} umask zredukuje uprawnienia, jeśli nie chcemy mieć konkretnych ustawień to jest to dobra strategia {{< /details >}}

<em>rozwiązanie drugi etap, plik <b>prog16b.c</b>:</em>
{{< includecode "prog16b.c" >}}

Uruchamiamy jak poprzednio - błędy znikają.

Co to jest błąd EINTR?
{{< details "Odpowiedź" >}} To nie jest błąd, to tylko informacja o przerwaniu danej funkcji poprzez funkcję obsługi sygnału {{< /details >}}

Jaka jest poprawna reakcja na ten błąd?
{{< details "Odpowiedź" >}} Nie jest to wyjście z programu, prawie zawsze jest to restart funkcji z tymi samymi parametrami jakie były podane przy pierwszym wywołaniu. {{< /details >}}

Kiedy dokładnie te funkcje są przerywane?
{{< details "Odpowiedź" >}} EINTR oznacza przerwanie zanim funkcja coś zrobi! Można zatem bez obaw restartować, za jedynym używanym tu wyjątkiem funkcji przyłączania gniazda "connect" (SOP2) {{< /details >}} 

Jakie inne przerwania w programie może spowodować funkcja obsługi sygnału?
{{< details "Odpowiedź" >}} Może przerwać operacje IO lub spanie, nie jest to raportowane przez EINTR, w obu przypadkach reakcja na takie zdarzenie nie jest prosta.  {{< /details >}}

Skąd wiemy, które funkcje mogą być przerwane zanim coś osiągną (EINTR)?
{{< details "Odpowiedź" >}} Strony man pages, dział o zwracanych błędach. Łatwo zgadnąć, że to te funkcje, które mogą/muszą czekać zanim coś zrobią. {{< /details >}}

Jako ważne ćwiczenie przeanalizuj jak działa bulk_read i bulk_write. Musisz rozumieć czemu uwzględniają tak dużo przypadków, jakie to przypadki, kiedy operacja IO może być przerwana, jak rozpoznać EOF.
W przeciwieństwie do laboratorium L1, na L2 i kolejnych trzeba używać tych funkcji (lub analogicznych) gdy używasz `read` lub `write` (ponieważ w programie mamy już sygnały).
I brak będzie powodował odejmowanie punktów.

Obie funkcje bulk_ mogą być pomocne nie tylko gdy chodzi o ochronę przed sygnałami lub sklejanie dużych transferów I/O,
ale także tam gdzie dane nie są dostępne w sposób ciągły - pipe/fifo/gniazda które poznamy nieco później.

Podobnie jak read/write zachowują się wszystkie funkcje pokrewne takie jak fread/fwrite czy send/recv

Warto sobie uświadomić czemu użycie flagi SA_RESTART podczas instalowania funkcji obsługi sygnału nie rozwiązuje nam
problemu z EINTR:

Z góry musimy wiedzieć jakie sygnały będą obsługiwane w naszym programie i wszystkie one muszą być włączone z tą flagą,
wystarczy jeden bez tej niej i problem EINTR powraca. Łatwo o taki błąd jeśli powrócimy do starszego kodu, łatwo
zapomnieć o tym wymogu.

Jeśli chcemy napisać sobie funkcję biblioteczną (np. bulk_read) to nie możemy nic zakładać o obsłudze sygnałów w
programie używającym naszej biblioteki.

Nie możemy łatwo przenieść takiego kodu, w programie docelowym musiałaby być dokładnie taka sama obsługa sygnałów.

Czasem zależy nam na tym, aby właśnie być informowanym o przerwaniu, jaskrawym przykładem jest funkcja sigsuspend, która
z tą flagą traci sens!

Po wywołaniu fprintf nie sprawdzamy błędów innych niż EINTR, czemu? Jeśli nie możemy pisać na stderr (zapewne ekran) to
i tak nie zaraportujemy błędu.

Zwróć uwagę, że naprawdę duże (f)printf'y mogą być przerwane także w trakcie wypisywania! Trudno będzie coś mądrego z
tym zrobić, zwłaszcza jeśli do tego wypisywania używamy skomplikowanych formatów. Co prawda funkcja zwróci ile znaków
wypisała ale jak to potem zrestartować? Zazwyczaj nie łatwo jest nam policzyć ile znaków powinno być wypisane, tym
bardziej nie mamy jak wyznaczyć od którego momentu trzeba wznowić wypisywanie. Dlatego funkcji tej rodziny unikamy tam
gdzie restartowanie będzie krytyczne i musimy wypisać dużo danych. Tam gdzie błąd braku restartu nie jest krytyczny (
wypisywanie na ekran) lub gdy ilość danych jest mała tam nie ma tego problemu.

## Przykładowe zadania

Wykonaj przykładowe zadania. Podczas laboratorium będziesz miał więcej czasu oraz dostępny startowy kod, jeśli jednak wykonasz poniższe zadania w przewidzianym czasie, to znaczy, że jesteś dobrze przygotowany do zajęć.

- [Zadanie 1]({{< ref "/sop1/lab/l2/example1" >}}) ~60 minut
- [Zadanie 2]({{< ref "/sop1/lab/l2/example2" >}}) ~120 minut
- [Zadanie 3]({{< ref "/sop1/lab/l2/example3" >}}) ~120 minut

## Kody źródłowe z treści tutoriala

{{% codeattachments %}}

