---
title: "L2 - Procesy, sygnały i deskryptory"
date: 2022-02-05T19:10:36+01:00
weight: 30
---

# Tutorial 2 - Procesy, sygnały i deskryptory

{{< hint info >}}
Uwagi wstępne:

- Szybkie przejrzenie tutoriala prawdopodobnie nic nie pomoże, należy samodzielnie uruchomić programy, sprawdzić jak działają, poczytać materiały dodatkowe takie jak strony man. W trakcie czytania sugeruję wykonywać ćwiczenia a na koniec przykładowe zadanie.
- Na żółtych polach podaję dodatkowe informacje, niebieskie zawierają pytania i ćwiczenia. Pod pytaniami znajdują się odpowiedzi, które staną się widoczne dopiero po kliknięciu. Proszę najpierw spróbować sobie odpowiedzieć na pytanie samemu a dopiero potem sprawdzać odpowiedź.
- Pełne kody do zajęć znajdują się w załącznikach na dole strony. W tekście są tylko te linie kodu, które są konieczne do zrozumienia problemu.
- Materiały i ćwiczenia są ułożone w pewną logiczną całość, czasem do wykonania ćwiczenia konieczny jest stan osiągnięty poprzednim ćwiczeniem dlatego zalecam wykonywanie ćwiczeń w miarę przyswajania materiału.
- Większość ćwiczeń wymaga użycia konsoli poleceń, zazwyczaj zakładam, ze pracujemy w jednym i tym samym katalogu roboczym więc wszystkie potrzebne pliki są "pod ręką" tzn. nie ma potrzeby podawania ścieżek dostępu.
- Czasem podaję znak $ aby podkreślić, że chodzi o polecenie konsolowe, nie piszemy go jednak w konsoli np.: piszę "$make" w konsoli wpisujemy samo "make".
- To co ćwiczymy wróci podczas kolejnych zajęć. Jeśli po zajęciach i teście coś nadal pozostaje niejasne proszę to poćwiczyć a jeśli trzeba dopytać się u <a href="6">prowadzących</a>.
- Tym razem część rozwiązań jest podzielone na 2 możliwe do uruchomienia etapy
{{< /hint >}}

## Zadanie 1 - procesy potomne

Cel: Program tworzy n procesów potomnych (n jest parametrem na pozycji 1), każdy z tych procesów czeka przez losowy czas [5-10] sekund po czym wypisuje na ekran  swój PID i się kończy. Proces rodzica co 3s ma wyświetlać na stdout ile jeszcze posiada pod-procesów.
Co student musi wiedzieć: 
- man 3p fork
- man 3p getpid
- man 3p wait
- man 3p waitpid
- man 3p sleep
- <a href="https://www.gnu.org/software/libc/manual/html_node/Job-Control.html">Job Control</a>

<em>rozwiązanie 1 etap <b>prog13a.c</b>:</em>
{{< includecode "prog13a.c" >}}

Do kompilacji używamy "make prog13a" ostatni plik Makefile z tutoriala nr 1

Upewnij się, że wiesz jak powstaje grupa procesów tzn. kiedy powłoka tworzy nową grupę i jakie procesy do niej należą

Zwróć uwagę, że w makrze ERR dodano kill(0, SIGKILL). Chodzi o to aby w razie błędu zatrzymać cały program (wszystkie
procesy danej grupy).

Zero jako argument kill jest bardzo użyteczne, odnosi się do wszystkich procesów danej grupy, nie musimy przechowywać
listy PID'ów.

Zwróć uwagę, że nie analizujemy błędów funkcji kill w makrze ERR, to dla tego, że w przypadku sytuacji krytycznej
podejmujemy jak najmniej działań, zresztą co mielibyśmy zrobić wywołać rekurencyjnie ERR?

Pytanie czemu po uruchomieniu wraca linia poleceń?
{{< expand "Odpowiedź" >}} Proces rodzic nie czeka na zakończenie procesów potomnych, brak wywołania wait lub waitpid, naprawimy to w 2 etapie {{< /expand >}}

Jak sprawdzić kto jest rodzicem procesów prog13a? Dlaczego właśnie ten proces?
{{< expand "Odpowiedź" >}} Zaraz po uruchomieniu wykonać np.: $ps -f, rodzicem procesów będzie proces o PID 1 czyli init/systemd. Dzieje się tak dlatego, że proces rodzic kończy się bez czekania na procesy potomne a te nie mogą "wisieć" poza drzewem procesów. Nie przechodzą pod powłokę ale właśnie pod proces po PID=1 {{< /expand >}}

Zwróć uwagę na miejsce i sposób ustalenia ziarna liczb losowych. Czy jeśli przesuniemy srand z procesu dziecka do rodzica to zmieni to działanie programu? Jak?
{{< expand "Odpowiedź" >}}  Procesy potomne będą losowały te sam liczby ponieważ zaczynają z tym samym ziarnem, tak być nie może. {{< /expand >}}

Czy można jako argument srand  podać time() zamiast getpid()?
{{< expand "Odpowiedź" >}}  Nie można, wywołania time() odbędą się w tej samej sekundzie wiec ziarna będą takie same, efekt taki sam jak poprzednio. {{< /expand >}}

Zwróć uwagę na losowanie liczby z przedziału [A,B]. Niby oczywiste, ale warto opracować sobie prosty wzór.

Jak zachowa się program bez exit  wywołanym zaraz po child_work w procesie potomnym?
{{< expand "Odpowiedź" >}} Procesy potomne po zakończeniu wykonania kodu funkcji child_work przechodzą dalej w kodzie czyli wracają do pętli forkującej i tworzą własne procesy potomne, które to też mogą utworzyć swoje potomne itd. Niezły bałagan, do tego procesy dzieci nie czekają na swoje dzieci.  {{< /expand >}}

Ile procesów (w sumie ) zostanie utworzonych jeśli jako parametr uruchomienia podamy 3 przy założeniu, że nie ma exit wspomnianego w poprzenim pytaniu?
{{< expand "Odpowiedź" >}}  1 rodzic 3 potomne 1 poziomu,  3 drugiego i  1 trzeciego poziomu, w sumie 8 procesów, narysuj sobie drzewko, opisz gałęzie wartością n w momencie wykonania forka. {{< /expand >}}

Co zwraca sleep?  Czy powinniśmy jakoś na to reagować? 
{{< expand "Odpowiedź" >}} zwraca czas "niedospany" w przypadku przerwania funkcją obsługi sygnału w tym programie proces dziecka nie dostaje sygnałów i ich nie obsługuje więc nie musimy się tym martwić. W innych programach często będziemy restartować sleep z nowym czasem spania tak aby w sumie przespać tyle ile było założone. {{< /expand >}}

W kolejnym etapie dodamy czekanie i zliczanie procesów potomnych. Pytanie skąd mamy wiedzieć ile procesów potomnych w danej chwili istnieje?
{{< expand "Odpowiedź" >}} Można próbować zliczać sygnały SIGCHLD ale to zawodny sposób bo mogą się "sklejać" czyli mniej ich dostaniemy niż potomków się na prawdę zakończyło. Jedyną pewną metodą jest zliczanie udanych wywołań wait i waitpid. {{< /expand >}}

<em>rozwiązanie 2 etap <b>prog13b.c</b>:</em>
{{< includecode "prog13b.c" >}}

Koniecznie trzeba rozróżnić kiedy waitpid informuje nas o chwilowym braku zakończonych potomków (wartość zwracana zero) od permanentnego braku potomków (błąd ECHILD). Ten ostatni przypadek nie jest w zasadzie błędem, tę sytuację trzeba normalnie obsłużyć. 

Czemu wołamy waitpid w pętli?
{{< expand "Odpowiedź" >}} Nie wiemy z góry ile procesów zombie czeka, może być 0 , 1 , 2 aż do n) {{< /expand >}}

Czemu wołamy waitpid z flagą WNOHANG?
{{< expand "Odpowiedź" >}} Nie chcemy czekać nie wiadomo ile na jeszcze żyjące procesy, działamy w pętli, która co 3 sekundy ma wypisać dane {{< /expand >}}

Czemu wołamy waitpid z pidem zero?
{{< expand "Odpowiedź" >}} Czekamy na dowolny proces potomny, nie musimy znać jego PID, zero oznacza dowolny z potomków. {{< /expand >}}

Czy w tym programie występują sygnały? 
{{< expand "Odpowiedź" >}} SIGCHILD, nie ma f. obsługi ale to nie szkodzi, w pewnym sensie jest obsługiwany przez waitpid {{< /expand >}}

Czy tym razem nie powinniśmy sprawdzać co zwraca sleep skoro są sygnały?
{{< expand "Odpowiedź" >}} Nie bo nie ma funkcji obsługi sygnału. {{< /expand >}}

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
się okazać, że czytelna dla nas pęta while(work) gdzie work jest zmienną globalną zmienia się na while(1) po
optymizacji. Secundo sig_atomic_t oznacza największy typ numeryczny, który jest przetwarzany w pojedynczej instrukcji
CPU. Jeśli weźmiemy większy typ numeryczny przerwanie obsługą sygnału może zakłócić wartość wynikową nawet prostego
porównania a==0 o ile przerwanie wypadnie w trakcie porównania i zmieni już porównane bajty.

Z powyższego wynika, że nie przekazujemy pomiędzy funkcją obsługi a głównym kodem nic poza prostymi liczbami, stanami.
Do tego dochodzi dobra praktyka nie przerywania programu na zbyt długo co pozostawia nam bardzo mało poprawnych,
przenośnych i bezpiecznych rozwiązań w kwestii jak dzielić logikę programu pomiędzy główny kod a funkcję obsługi
sygnału. Najprostsza zasada aby funkcję obsługi były ekstremalnie krótkie (przypisanie, inkrementacja zmiennej itp) a
cała logika pozostała w głównym kodzie jest najlepsza.

Funkcja memset bywa konieczna a zazwyczaj jest użyteczna przy inicjowaniu nie w pełni znanych nam struktur (nie wiemy
jakie tam są jeszcze pola w danej implementacji).

Zwróć uwagę, że obsługa sigchild w pętli jest prawie identyczna jak poprzednio w pętli.

Czy podczas obsługi sygnału SIGCHLD można się spodziewać więcej niż jednego zakończonego procesu dziecka?
{{< expand "Odpowiedź" >}}  Tak, sygnały mogą się skleić, dziecko może się zakończyć akurat w trakcie obsługi  {{< /expand >}}

Czy podczas obsługi sygnału SIGCHLD można się spodziewać braku zakończonego procesu dziecka? Zerknij na zakończenie main
{{< expand "Odpowiedź" >}}  Tak, wait na końcu main może "podkradać" te czekające zombie tj. wywoła się poprawnie wait zanim wykona się funkcja obsługi.  {{< /expand >}}

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
{{< expand "Odpowiedź" >}}  sleep jest przerywane przez obsługę sygnału, zatem restart jest konieczny. Ponieważ sleep zwraca pozostały czas spania w sekundach to z uwagi na zaokrąglenia nie można po restarcie uzyskać dokładnego czasu spania.   {{< /expand >}}

Jaka jest domyślna dyspozycja znakomitej większości sygnałów (w tym SIGUSR1 i 2)?
{{< expand "Odpowiedź" >}} Zabicie procesu do którego wysyłamy, w tym programie brak reakcji (funkcji obsługi), blokowania lub ignorowania sygnałów SIGUSR1 i 2 skutkowałoby przedwczesnym zabiciem procesów. {{< /expand >}}

Jaka jest konsekwencja wysyłana przez proces rodzic sygnałów SIGUSR1/2 do całej grupy procesów?
{{< expand "Odpowiedź" >}} Proces rodzic musi jakoś reagować na te sygnały mimo, że nie są one mu potrzebne do pracy, zatem je ignoruje. {{< /expand >}}

Co by się stało, gdyby nie było włączone ignorowanie SIGUSR1i2 w procesie rodzicu?
{{< expand "Odpowiedź" >}} Proces rodzic zabiłby się pierwszym wysyłanym sygnałem. {{< /expand >}} 

Czy można przesunąć ustawienie ignorowania sygnałów za funkcję create_children? Procesy potomne przecież nie potrzebują tego ignorowania, mają od razu po starcie ustawianą funkcję obsługi? 
{{< expand "Odpowiedź" >}} Nie można, mogłoby się zdarzyć (choć rzadko), że procesy potomne zostałyby tylko utworzone (a nie zaczęte) a przydział CPU po tym utworzeniu przeszedłby do procesu rodzica. który zdążyłby wysłać sygnał SIGUSR1 do dzieci. Kolejny przydział CPU do procesu dziecka/dzieci spowodowałby najpierw obsługę sygnału a to oznaczałoby zabicie! {{< /expand >}}

Czy można jakoś zmienić ten program tak aby wykluczyć ignorowanie sygnałów SIGUSR1i2?
{{< expand "Odpowiedź" >}} Ten akurat program może mieć identyczną reakcję na te sygnały w rodzicu i potomkach, można zatem ustawić obsługę od razu w procesie rodzicielskim przed fork. {{< /expand >}}

A co się stanie jeśli za fork  przeniesiemy obsługę SIGCHLD? 
{{< expand "Odpowiedź" >}} Jeśli jeden z procesów potomnych "umrze" zanim włączymy tą obsługę to będzie on "zombie" aż do momentu gdy kolejny w pod-procesów  się zakończy. Nie jest to bardzo duży błąd ale warto i na takie zwracać uwagę. {{< /expand >}}

Pytanie, czy wait na końcu main jest potrzebny? Przecież i tak funkcja parent_work() powinna działać co najmniej tyle czasu co najdłuższy z podprocesów?
{{< expand "Odpowiedź" >}} Wyliczenie czasu w pętli rodzica nie wystarczy, w obciążonym systemie możliwe są dowolnie długie opóźnienia, bez wait powstaje zatem tzw. race condition - kto się pierwszy zakończy rodzic czy potomne procesy. {{< /expand >}}

## Zadanie 3 - czekanie na sygnał

Cel: Program tworzy jeden pod-proces, który co określony parametrami czas (w mikrosekundach) wysyła do procesu rodzica
SIGUSR1, co n-ty raz (określony parametrami) wysyła SIGUSR2. Proces rodzic czeka na otrzymanie sygnału SIGUSR2. Oba
procesy zliczają i wypisują na bieżąco ilości wysłanych/odebranych SIGUSR2. Używamy część funkcji z poprzedniego
zadania.

Co student musi wiedzieć: 
- man 3p sigsuspend
- Opis metod czekania na sygnał w glibc <a href="http://www.gnu.org/software/libc/manual/html_node/Waiting-for-a-Signal.html#Waiting-for-a-Signal"> tutaj</a>
- man 3p getppid 
- man 3p pthread_sigmask (tylko opis sigprocmask)
- man 3p sigaddset
- man 3p sigemptyset

<em>rozwiązanie, plik <b>prog15.c</b>:</em>
{{< includecode "prog15.c" >}}

Program działa aż do SIGINT (C-c)

Uruchom z różnymi parametrami, z małą przerwą w mikrosekundach i częstymi SIGUSR2 różnica w licznikach powinna wzrastać
szybciej niż gdy te parametry są większe. Za chwilę wyjaśni się skąd te błędy. Jeśli nie obserwujesz błędów spróbuj dać
programowi trochę więcej czasu na działanie, około minuty powinno dać już różne wyniki.

Program jest celowo napisany tak, aby występowały pewne problemy, można go lepiej napisać i je wyeliminować, proszę miej
to na względzie kopiując rozwiązania z tego kodu!

Zwróć uwagę na getppid, niby oczywiste ale widziałem prace gdzie studenci przekazywali pid rodzica do procesu potomnego.

Oczekiwanie na sygnał poprzez sigsuspend jest bardzo ważną technika, którą musisz rozumieć, dobrze omawia to
dokumentacja GNU podlinkowana powyżej. Pamiętaj, że zasada jest taka, blokujemy oczekiwane sygnały w całej pętli (
większej części programu) i przywracamy ich dochodzenie tylko wtedy gdy możemy czekać czyli właśnie w wywołaniu
sigsuspend. Mamy zatem w programie dobrze określone momenty w których obsługujemy sygnały, to spore ułatwienie.

Jeśli funkcje obsługujące sygnały mogą być wykonywane tylko w czasie nieaktywności programu głównego (czyli jak w tym
przykładzie) można nieco rozluźnić zasady tego co wolno przekazywać przez zmienne globalne, oraz te dotyczące długości i
czasu ich działania.

Który licznik jest niepoprawny, ten w procesie dziecka czy rodzica?
{{< expand "Odpowiedź" >}} To musi być ten który jest mniejszy, nie mamy w tym kodzie możliwości zliczania nieistniejących sygnałów, możemy tylko gubić te wysłane. Gubić może oczywiście tylko odbiorca. Problem jest w procesie rodzica. {{< /expand >}}

Pytanie czemu liczniki się różnią ? 
{{< expand "Odpowiedź" >}} 1. sklejanie sygnałów ale to ma mały wpływ, 2.  fakt, że sigsuspend NIE GWARANTUJE WYKONANIA MAKSYMALNIE JEDNEJ OBSŁUGI SYGNAŁU! To częsty błąd w rozumowaniu! Zaraz po wywołaniu obsługi SIGUSR2 jeszcze w obrębie jednego wykonania sigsuspend następuje obsługa SIGUSR1, zmienna globalna jest nadpisywana i proces rodzic nie ma szansy zliczyć SIGUSR2!!! {{< /expand >}}

Jak uruchomić ten program aby zminimalizować szansę na sklejanie się SIGUSER2 i jednocześnie obserwować  różnice w licznikach?
{{< expand "Odpowiedź" >}} Uruchomić dla małych czasów i dużych ilości SIGUSR1 między SIGUSR2, teraz jest prawie zerowa szansa na sklejenie SIGUSR2, za to duża na wykonanie wiele razy funkcji obsługi sygnału w obrębie jednego sigsuspend {{< /expand >}}

Popraw powyższy program tak aby wyeliminować problem wielu wywołań obsługi sygnału w obrębie jednego sigsuspend 
{{< expand "Odpowiedź" >}} Można to zrobić np. dodając drugą zmienną globalną tylko do obsługi SIGUSR2, zwiększanie zmiennej count też można przenieść do funkcji obsługi sygnału w ten sposób uniknie się potencjalnego problemu z obsługą dwóch SIGUSR2  w obrębie jednego sigsuspend. Trzeba jeszcze przebudować kod związany z wypisywaniem zmienionego licznika count w rodzicu i gotowe. {{< /expand >}}

## Zadanie 4 - operacje niskopoziomowe na plikach

Cel: Zmodyfikować program z zadania 3 tak aby proces rodzic odbierał sygnały SIGUSR1 wysyłane co zadany czas (parametr 1) i zliczał je.  Dodatkowo proces główny tworzył plik o nazwie podanej jako parametr 4 o zadanej ilości bloków o zadanym rozmiarze (parametry 2 i 3). Zawartość pliku ma pochodzić z /dev/urandom. Każdy blok kopiujemy osobno, kontrolując rozmiary. Po skopiowaniu bloku należy podać na stderr realną ilość przepisanych bloków oraz stan liczników sygnałów.
Co student musi wiedzieć: 
- man 3p open
- man 3p close
- man 3p read
- man 3p write
- man 4 urandom
- man 3p mknod (tylko stałe opisujące uprawnienia do open)
- opis makra TEMP_FAILURE_RETRY <a href="http://www.gnu.org/software/libc/manual/html_node/Interrupted-Primitives.html">tutaj</a>

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
{{< expand "Odpowiedź" >}}  w tej architekturze (GNU/Linux) planista procesora blokuje uruchomienie obsługi sygnału podczas większych operacji IO, w tym czasie sygnały się sklejają. {{< /expand >}}

W jakim celu proces rodzic na zakończenie wysyła do całej grupy SIGUSR1?
{{< expand "Odpowiedź" >}} Aby zakończyć proces potomny. {{< /expand >}}

Jak proces potomny może się zakończyć po nadejściu SIGUSR1 skoro dziedziczy obsługę tego sygnału?
{{< expand "Odpowiedź" >}} Zaraz po starcie potomka przywracana jest domyślna reakcja na ten sygnał, która właśnie zapewnia zabicie procesu. {{< /expand >}}

Czemu proces rodzic nie zabija się sam tym sygnałem?
{{< expand "Odpowiedź" >}} Ma włączoną obsługę tego sygnału zanim wyśle sygnał do grupy. {{< /expand >}}

Czy taka strategia może się  nie powieść?
{{< expand "Odpowiedź" >}} Tak, jeśli proces rodzic upora się ze swoim zadaniem zanim proces potomny zmieni dyspozycję odnośnie SIGUSR1 na domyślną. {{< /expand >}}

Czy można to jakoś poprawić? Tzn. proces rodzic zawsze zabije potomka ale jednocześnie sam nie narazi się na przedwczesną śmierć?
{{< expand "Odpowiedź" >}} Wyślij do potomka SIGUSR2. {{< /expand >}} 

Czy taka strategia zakończenia potomka zawsze jest poprawna i  łatwa do przeprowadzenia?
{{< expand "Odpowiedź" >}} Tylko jeśli proces zabijany nie posiada zasobów, jeśli by takowe posiadał to musisz dodać obsługę sygnału kończącego co nie musi być  łatwe. {{< /expand >}}

Czemu po wywołaniu alokacji pamięci sprawdzamy przydzielony wskaźnik czy aby nie jest NULL? 
{{< expand "Odpowiedź" >}} System może nie móc nam przydzielić więcej pamięci, musimy być na to przygotowani. Brak tego sprawdzania jest bardzo częstym błędem w pracach studentów. {{< /expand >}}

Czy nie dałoby się tego bufora uczynić zmienną automatyczną i uniknąć kodu związanego z alokacją i zwalnianiem?
{{< expand "Odpowiedź" >}} Nie ma w znanych mi architekturach tak dużych stosów aby można było  na nich umieszczać tak duże zmienne (40MB dla przykładowych parametrów), zazwyczaj stos ma wymiar do kilku megabajtów, gdyby  nasz bufor był mały (kilka KB) to moglibyśmy tak zrobić. {{< /expand >}}  

Czemu uprawnienia do nowego pliku są  pełne (0777)? 
{{< expand "Odpowiedź" >}} umask zredukuje uprawnienia, jeśli nie chcemy mieć konkretnych ustawień to jest to dobra strategia {{< /expand >}}

<em>rozwiązanie drugi etap, plik <b>prog16b.c</b>:</em>
{{< includecode "prog16b.c" >}}

Uruchamiamy jak poprzednio - błędy znikają.

Aby dostępne było makro `TEMP_FAILURE_RETRY` trzeba najpierw zdefiniować `GNU_SOURCE` a następnie dołączyć plik
nagłówkowy `unistd.h`.

Co to jest błąd EINTR?
{{< expand "Odpowiedź" >}} To nie jest błąd, to tylko informacja o przerwaniu danej funkcji poprzez funkcję obsługi sygnału {{< /expand >}}

Jaka jest poprawna reakcja na ten błąd?
{{< expand "Odpowiedź" >}} Nie jest to wyjście z programu, prawie zawsze jest to restart funkcji z tymi samymi parametrami jakie były podane przy pierwszym wywołaniu. {{< /expand >}}

Kiedy dokładnie te funkcje są przerywane?
{{< expand "Odpowiedź" >}} EINTR oznacza przerwanie zanim funkcja coś zrobi! Można zatem bez obaw restartować, za jedynym używanym tu wyjątkiem funkcji przyłączania gniazda "connect" (SOP2) {{< /expand >}} 

Jakie inne przerwania w programie może spowodować funkcja obsługi sygnału?
{{< expand "Odpowiedź" >}} Może przerwać operacje IO lub spanie, nie jest to raportowane przez EINTR, w obu przypadkach reakcja na takie zdarzenie nie jest prosta.  {{< /expand >}}

Skąd wiemy, które funkcje mogą być przerwane zanim coś osiągną (EINTR)?
{{< expand "Odpowiedź" >}} Strony man pages, dział o zwracanych błędach. Łatwo zgadnąć, że to te funkcje, które mogą/muszą czekać zanim coś zrobią. {{< /expand >}}

Jako ważne ćwiczenie przeanalizuj jak działa bulk_read i bulk_write. Musisz rozumieć czemu uwzględniają tak dużo przypadków, jakie to przypadki, kiedy operacja IO może być przerwana, jak rozpoznać EOF. To na pewno będzie omawiane na zajęciach przed testem, ale najpierw sam/sama spróbuj to "rozgryźć".

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

Wykonaj przykładowe <a href="{{< ref "../l2-example">}}">ćwiczenie</a> z poprzednich lat. To zadanie szacuję na 60 minut, jeśli wyrobisz się w tym czasie to znaczy, że jesteś dobrze przygotowany/a  do zajęć. Pamiętaj, że w aktualnym układzie zajęć będzie dane zadnie na około 1,5 godziny, więc trochę bardziej pracochłonne.

## Kody źródłowe z treści tutoriala

{{% codeattachments %}}

