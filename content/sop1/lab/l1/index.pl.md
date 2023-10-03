---
title: "L1 - System plików"
date: 2022-02-05T18:39:22+01:00
weight: 20
---

# Tutorial 1 - System plików

{{< hint info >}}
Uwagi wstępne:
- To jest mega łatwy tutorial, ale za to długi, kolejne będą coraz trudniejsze i krótsze
- Szybkie przejrzenie tutoriala prawdopodobnie nic nie pomoże, należy samodzielnie uruchomić programy, sprawdzić jak
  działają, poczytać materiały dodatkowe takie jak strony man. W trakcie czytania sugeruję wykonywać ćwiczenia a na
  koniec przykładowe zadanie.
- Na żółtych polach podaję dodatkowe informacje, niebieskie zawierają pytania i ćwiczenia. Pod pytaniami znajdują się
  odpowiedzi, które staną się widoczne dopiero po kliknięciu. Proszę najpierw spróbować sobie odpowiedzieć na pytanie
  samemu a dopiero potem sprawdzać odpowiedź.
- Pełne kody do zajęć znajdują się w załącznikach na dole strony.
- Materiały i ćwiczenia są ułożone w pewną logiczną całość, czasem do wykonania ćwiczenia konieczny jest stan osiągnięty
  poprzednim ćwiczeniem dlatego zalecam wykonywanie ćwiczeń w miarę przyswajania materiału.
- Większość ćwiczeń wymaga użycia konsoli poleceń, zazwyczaj zakładam, że pracujemy w jednym i tym samym katalogu
  roboczym więc wszystkie potrzebne pliki są "pod ręką" tzn. nie ma potrzeby podawania ścieżek dostępu.
- Czasem podaję znak $ aby podkreślić, że chodzi o polecenie konsolowe, nie piszemy go jednak w konsoli np.: piszę "
  $make" w konsoli wpisujemy samo "make".
- To, co ćwiczymy wróci podczas kolejnych zajęć. Jeśli po zajęciach i teście coś nadal pozostaje niejasne proszę to
  poćwiczyć a jeśli trzeba dopytać się u prowadzących.
{{< /hint >}}


## Zadanie 1 - katalogi 1

Cel: Napisać program zliczający (pliki, linki, katalogi i inne obiekty) w katalogu roboczym (bez podkatalogów)

Co student musi wiedzieć: 
- man 3p fdopendir (tylko opis opendir)
- man 3p closedir
- man 3p readdir
- man 0p dirent.h
- man 3p fstatat (tylko opis stat i lstat)
- man 3p errno
- man 7 inode (pierwsza połowa sekcji "The file type and mode")

<em>kod do pliku <b>prog9.c</b></em>
{{< includecode "prog10.c" >}}

Uruchom ten program w katalogu w którym masz jakieś pliki, może być ten w którym wykonujesz ten tutorial, ważne aby nie było w nim katalogów, czy wyniki zgadzają się z tym czego oczekujemy tj. zero katalogów, trochę plików?
{{< expand "Odpowiedź" >}} 
Nie, są dwa katalogi, program policzył katalog "." i  "..", każdy katalog ma hard link na samego siebie "." i katalog rodzic ".." 
{{< /expand >}}

Jak utworzyć link symboliczny do testów ? 
{{< expand "Odpowiedź" >}} 
```shell
ln -s prog9.c prog_link.c
```
{{< /expand >}}

Jak różnią się stat i lstat? Czy jeśli w kodzie zmienić lstat na stat to zliczymy linki poprawnie ? 
{{< expand "Odpowiedź" >}} 
Nie, link zostanie potraktowany tak jak obiekt który wskazuje, to jest właśnie różnica pomiędzy tymi dwoma funkcjami.
{{< /expand >}}

Jakie pola zawiera struktura opisująca obiekt w systemie plików (dirent) wg. POSIX ? 
{{< expand "Odpowiedź" >}} 
Tylko numer inode i nazwę, resztę danych o pliku odczytujemy funkcjami lstat/stat
{{< /expand >}}

Jakie pola zawiera struktura opisująca obiekt w systemie plików (dirent) w Linuksie (man readdir) ? 
{{< expand "Odpowiedź" >}} 
Numer inode, nazwę  i 3 inne nie objęte standardem
{{< /expand >}}

Tam gdzie implementacja Linuksa odbiega od standardu trzymamy się zawsze standardów, to powoduje większą przenośność
naszego kodu pomiędzy różnymi Unix'ami.

Zwróć uwagę na sposób obsługi błędów funkcji systemowych, zazwyczaj robimy to tak:  if(fun()) ERR(), makro ERR było już
omawiane wcześniej. Wszystkie funkcje mogące sprawiać kłopoty należy sprawdzać. W praktyce prawie wszystkie funkcje
systemowe wymagają sprawdzania. Będę to powtarzał do znudzenia, niesprawdzanie błędów jest grzechem głównym
programistów.

Większość błędów jakie napotkamy będzie wymagać zakończenia programu, wyjątki omówimy w kolejnych tutorialach.

Zwróć uwagę na użycie katalogu "." w kodzie, nie musimy znać aktualnego katalogu roboczego, tak jest prościej.

Funkcja readdir jest dość specyficzna gdyż zwraca NULL zarówno jako oznaczenie końca katalogu jak i jako oznaczenie
błędu! Jak więc sobie z tym poradzić?

Używamy errno do rozpoznania błędu readdir, wartość errno musi być zerowana przed KAŻDYM wywołaniem readdir a test jego
wartości musi być wykonany zanim nastąpią wywołania funkcji systemowych i bibliotecznych, które mogłyby to errno
wyzerować. Zwróć uwagę, że errno = 0 jest ustawiane w pętli przed readdir a nie np. raz przed pętlą oraz na to, że w
razie zwrócenia NULL przez readdir sterowanie przechodzi jedynie przez dwa proste warunki zanim dojdzie do sprawdzenia
errno i rozpoznania błędu.

Czemu w ogóle zerujemy errno? Przecież funkcja readdir mogłaby robić to za nas? No właśnie "mogłaby", dokładnie tak
definiuje to standard, funkcje systemowe mogą zerować errno w razie poprawnego wykonania ale nie muszą.

Jeśli chcemy w warunkach logicznych w C dokonywać przypisań to powinniśmy ująć całe przypisanie w nawiasy, wartość
przypisywana będzie wtedy uznana za wartość wyrażenia w nawiasie. Robimy tak w przypadku wywołania readdir.

Dobrzy programiści zawsze zwalniają zasoby, w tym programie zasobem jest otwarty katalog. W Linuksie otwarty katalog
liczy się jak otwarty plik, proces może mieć limit otwartych deskryptorów co daje nam już dwa bardzo ważne argumenty aby
pamiętać o closedir. Trzecim powodem będzie sprawdzający kod nauczyciel :-).

## Zadanie 2 - katalogi 2

Cel: Bazując na funkcji z poprzedniego zadania napisać program, który będzie zliczał obiekty we wszystkich folderach
podanych jako parametry pozycyjne programu

Co student musi wiedzieć: 
- man 3p getcwd
- man 3p chdir

<em>kod do pliku <b>prog10.c</b></em>
{{< includecode "prog10.c" >}}

Sprawdź jak program zachowa się w przypadku nieistniejących katalogów, katalogów co do których nie masz prawa dostępu , czy poprawnie poradzi sobie ze ścieżkami względnymi podawanymi jako parametr a jak z bezwzględnymi.

Czemu program pobiera i zapamiętuje aktualny katalog roboczy?
{{< expand "Odpowiedź" >}}
Podawane programowi jako parametry ścieżki mogą być względne czyli zapisane względem początkowego położenia w drzewie
katalogów, program przed wywołaniem funkcji skanującej z poprzedniego zadania zmienia katalog roboczy tak aby "być" w
folderze skanowanym. Ta zmiana powoduje, że wszystkie inne ścieżki względne stają się niepoprawne (są prowadzone z
innego punktu drzewa niż program działa) dlatego po sprawdzeniu katalogu program musi wrócić do katalogu wyjściowego
skąd kolejna ścieżka względna ma sens.
{{< /expand >}}

Czy prawdziwe jest stwierdzenie, że program powinien "wrócić" do tego katalogu w którym był uruchomiony?
{{< expand "Odpowiedź" >}}
Nie, katalog roboczy to właściwość procesu, jeśli proces dziecko zmienia swój CWD to nie ma to wpływu na proces rodzic,
zatem nie ma obowiązku ani potrzeby wracać.
{{< /expand >}}

W tym programie nie wszystkie błędy muszą zakończyć się wyjściem, który można inaczej obsłużyć i jak?
{{< expand "Odpowiedź" >}}
`if(chdir(argv[i])) continue;` To oczywiście nieco uproszczone rozwiązanie, można by dodać jakiś komunikat.
{{< /expand >}}

Nigdy i pod żadnym pozorem nie pisz `printf(argv[i])`, jeśli ktoś poda jako katalog %d to jak to wyświetli `printf`?
To dotyczy nie tylko argumentów programu ale dowolnych ciągów znaków.

## Zadanie 3 - katalogi 3

Cel: Napisać program zliczający wystąpienia plików, katalogów, linków i innych typów dla całych poddrzew zaczynających
się w podanych jako parametry folderach.

Co student musi wiedzieć: 
- man 3p ftw
- man 3p nftw

<em>kod w pliku <b>prog11.c</b></em>
{{< includecode "prog11.c" >}}

Powtórz sobie jak działają wskazania na funkcje w C.

Sprawdź jak program sobie radzi z niedostępnymi i nieistniejącymi katalogami.

W jakim celu użyta jest flaga `FTW_PHYS`?
{{< expand "Odpowiedź" >}} 
Bez tej flagi nftw przechodzi przez linki symboliczne do wskazywanych obiektów, czyli nie może ich zliczać, 
analogicznie jak fstat 
{{< /expand >}}

Sprawdź jak inne flagi modyfikują zachowanie nftw.

Deklaracja `_XOPEN_SOURCE` jest na Linuksie niezbędna, inaczej nie widzi deklaracji funkcji nftw (ważna jest kolejność,
deklaracja przed include), funkcję ftw oznaczono już jako przestarzałą.

Zmienne globalne to "zło wcielone", zbyt łatwo ich użyć a przychodzi za to zapłacić przy analizowaniu cudzego kodu lub
podczas przenoszenia funkcji z jednego projektu do drugiego. Tworzą one niejawne zależności w kodzie. Tu niestety musimy
ich użyć, funkcja callback nftw nie pozwala nic przekazać na zewnątrz inaczej jak przez zmienną globalną. To jest
wyjątkowa sytuacja, używanie zmiennych globalnych poza wskazanymi koniecznymi przypadkami jest na labach zabronione!

Bardzo przydatna jest możliwość nałożenia limitu otwieranych przez nftw deskryptorów, co prawda może to uniemożliwić
przeskanowanie bardzo głębokiego drzewa katalogów (głębszego niż limit) ale pozwala to nam zarządzać zasobami które
mamy. W zakresie deskryptorów maksima systemowe pod Linuksem są nieokreślone, ale można procesy oddzielnie limitować na
poziomie administracji systemem.

## Zadanie 4 - operacje na plikach

Cel: Napisać program tworzący nowy plik o podanej parametrami nazwie (-n NAME), uprawnieniach (-p OCTAL ) i rozmiarze (
-s SIZE). Zawartość pliku ma się składać w około 10% z losowych znaków [A-Z], resztę pliku wypełniają zera (znaki o
kodzie zero, nie '0'). Jeśli podany plik już istnieje, należy go skasować.

Co student musi wiedzieć: 
- man 3p fopen
- man 3p fclose
- man 3p fseek
- man 3p rand
- man 3p unlink
- man 3p umask

Dokumentacja glibc dotycząca umask <a href="http://www.gnu.org/software/libc/manual/html_node/Setting-Permissions.html">link</a>

<em>kod do pliku <b>prog12.c</b></em>
{{< includecode "prog12.c" >}}

Jaką maskę bitową tworzy wyrażenie `~perms&0777` ? 
{{< expand "Odpowiedź" >}}
odwrotność wymaganych parametrem -p uprawnień przycięta do 9 bitów, 
jeśli nie rozumiesz jak to działa koniecznie powtórz sobie operacje bitowe w C.
{{< /expand >}}

Jak działa losowanie znaków ? 
{{< expand "Odpowiedź" >}}
W losowych miejscach wstawia kolejne znaki alfabetu od A do Z potem znowu A itd.
Wyrażenie 'A'+(i%('Z'-'A'+1)) powinno być zrozumiałe, jeśli nie poświęć mu więcej czasu takie losowania będą się jeszcze pojawiać.
{{< /expand >}}

Uruchom program kilka razy, pliki wynikowe wyświetl poleceniem cat i less sprawdź jakie mają rozmiary (ls -l), czy zawsze równe podanej w parametrach wartości? Z czego wynikają różnice dla małych rozmiarów -s a z czego dla dużych (> 64K) rozmiarów?
{{< expand "Odpowiedź" >}}
Prawie zawsze rozmiary są różne w obu przypadkach wynika to ze sposobu tworzenia pliku,
który jest na początku pusty a potem w losowych lokalizacjach wstawiane są znaki, 
nie zawsze będzie wylosowany znak na ostatniej pozycji. Losowanie podlega limitowi 2 bajtowego RAND_MAX, 
wiec w dużych plikach losowane są znaki na pozycjach do granicy RAND_MAX.
{{< /expand >}}

Przerób program tak, aby rozmiar zawsze był zgodny z założonym.

Czemu podczas sprawdzania błędu unlink jeden przypadek ignorujemy?
{{< expand "Odpowiedź" >}}
ENOENT oznacza brak pliku, jeśli plik o podanej nazwie nie istniał to nie możemy go skasować,
ale to nie przeszkadza programowi, w tym kontekście to nie jest błąd. 
Bez tego wyjątku moglibyśmy tylko nadpisywać istniejące pliki a nie tworzyć nowe.
{{< /expand >}}

Zwrócić uwagę na wyłączenie z main funkcji do tworzenia pliku, im więcej kodu tym ważniejszy podział na użyteczne funkcję. Przy okazji krótko omówmy cechy dobrej funkcji:
- robi jedną rzecz na raz (krótki kod)
- możliwie duży stopień generalizacji problemu (dodano procent jako parametr)
- wszystkie dane wejściowe dostaje przez parametry (nie używamy zmiennych globalnych)
- wyniki przekazuje przez parametry wskaźnikowe lub wartość zwracaną (w tym przypadku wynikiem jest plik) a nie przez zmienne globalne

W kodzie używamy specjalnych typów numerycznych `ssize_t`, `mode_t` zamiast int robimy to ze względu na zgodność typów z
prototypami funkcji systemowych.

Czemu w tym programie używamy umask? Otóż funkcja fopen nie pozwala ustawić uprawnień, a przez umask możemy okroić
uprawnienia jakie są nadawane domyślnie przez fopen, niskopoziomowe open daje nam nad uprawnieniami większą kontrolę.

Czemu zatem nie możemy dodać uprawnień "x"? Funkcja fopen domyślnie nadaje tylko prawa 0666 a nie pełne 0777, przez
bitowe odejmowanie nijak nam nie może wyjść ta brakująca część 0111.

Jak zwykle sprawdzamy wszystkie błędy, ale nie sprawdzamy statusu umask, czemu? Otóż umask nie zwraca błędów tylko starą
maskę.

Zmiana umask jest lokalna dla naszego procesu i nie ma wpływu na proces rodzicielski zatem nie musimy jej przywracać.

Parametr tekstowy -p został zmieniony na oktalne uprawnienia dzięki funkcji strtol, warto znać takie przydatne funkcje
aby potem nie wyważać otwartych drzwi i nie próbować pisać samemu oczywistych konwersji.

Pytanie czemu kasujemy plik skoro tryb otwarcia w+ nadpisuje plik? Jeśli plik o danej nazwie istniał to jego uprawnienia
są zachowywane a my przecież musimy nadać nasze, przy okazji jest to pretekst do ćwiczenia kasowania.

Tryb otwarcia pliku "b" nie ma w systemach POSIX-owych żadnego znaczenia, nie rozróżniamy dostępu na tekstowy i binarny,
jest tylko binarny.

W programie nie wypełniamy pliku zerami, dzieje się to automatycznie ponieważ gdy zapisujemy coś poza aktualnym końcem
pliku system automatycznie dopełnia lukę zerami. Co więcej, jeśli tych zer ciągiem jest sporo to nie zajmują one
sektorów dysku!

Jeśli wykonamy unlink na pliku już otwartym i używanym w innym programie to plik zniknie z filesystemu ale nadal
zainteresowane procesy będą mogły z niego korzystać. Gdy skończą plik zniknie na dobre.

Najlepiej w procesie wywołać srand dokładnie jeden raz z unikalnym ziarnem,w tym programie wystarczy czas podany w
sekundach.

## Zadanie 5 – buforowanie standardowego wyjścia

Ten temat ma więcej wspólnego z ogólnym programowaniem w C niż z systemami operacyjnymi, niemniej jednak wspominamy o nim, bowiem w poprzednich latach był częstym źródłem problemów.

<em>kod do pliku <b>prog13.c</b></em>
{{< includecode "prog13.c" >}}

Spróbuj uruchomić ten (bardzo prosty!) kod z terminala. Co widać na terminalu? 
{{< expand "Odpowiedź" >}} 
To czego się spodziewaliśmy: co sekundę pokazuje się liczba.
{{</ expand >}}

Spróbuj uruchomić kod ponownie, tym razem jednak przekierowując wyjście do pliku `./plik_wykonwyalny >
plik_z_wyjściem`. Następnie spróbuj otworzyć plik z wyjściem w trakcie działania programu, a potem zakończyć
działanie programu przez Ctrl+C i otworzyć plik jeszcze raz. Co widać tym razem? 
{{< expand "Odpowiedź" >}} Jeśli zrobimy te kroki wystarczająco szybko, plik okazuje się być pusty! To zjawisko wynika z
tego, że biblioteka standardowa wykrywa, że dane nie trafiają bezpośrednio do terminala, i dla wydajności buforuje je, 
zapisując je do pliku dopiero gdy zbierze się ich wystarczająco dużo. To oznacza, że dane nie są dostępne od razu,
a w razie nietypowego zakończenia programu (tak jak kiedy użyliśmy Ctrl+C) mogą wręcz zostać stracone. 
Oczywiście, jeśli damy programowi dojść do końca działania, to wszystkie dane zostaną zapisane do pliku (proszę spróbować!). Mechanizm buforowania można skonfigurować,
ale nie musimy tego robić, jak za chwilę zobaczymy. 
{{</ expand >}}

Spróbuj uruchomić kod podobnie, ponownie pozwalając wyjściu trafić do terminala (jak za pierwszym razem), ale spróbuj usunąć nową linię z argumentu `printf`: `printf("%d", i);`. Co widzimy tym razem?
{{< expand "Odpowiedź" >}}
Wbrew temu co powiedzieliśmy wcześniej, nie widać wyjścia mimo to, że tym razem dane trafiają bezpośrednio do terminala;
dzieje się natomiast to samo co w poprzednim kroku. Otóż biblioteka buforuje standardowe wyjście nawet jeśli dane
trafiają do terminala; jedyną różnicą jest to, że reaguje na znak nowej linii, wypisując wszystkie dane zebrane w
buforze. To ten mechanizm sprawił, że w pierwszym kroku nie wydarzyło się nic dziwnego. Właśnie dlatego czasami zdarza
się Państwu, że `printf` nie wypisuje nic na ekran; jeśli zapomnimy o znaku nowej linii, standardowa
biblioteka nic nie wypisze na ekran dopóki w innym wypisywanym stringu nie pojawi się taki znak, lub program się nie
zakończy poprawnie.
{{</ expand >}}

Spróbuj ponownie zrobić poprzednie trzy kroki, tym razem jednak wypisując dane do strumienia standardowego błędu: `fprintf(stderr, /* parametry wcześniej przekazywane do printf */);`. Co dzieje się tym razem? Żeby przekierować standardowy błąd do pliku, należy użyć `>2` zamiast `>`. 
{{< expand "Odpowiedź" >}}
Tym razem nic się nie buforuje i zgodnie z oczekiwaniami widzimy jedną cyfrę co sekundę. Standardowa biblioteka nie
buforuje standardowego błędu, bowiem często wykorzystuje się go do debugowania. 
{{</ expand >}}

Często możemy chcieć użyć `printf(...)` do debugowania, dodając wywołania tej funkcji w celu sprawdzenia
wartości zmiennych bądź czy wywołanie dochodzi do jakiegoś miejsca w naszym kodzie. W takich przypadkach należy zamiast
tej funkcji użyć `fprintf(stderr, ...)` i wypisywać do standardowego błędu. W przeciwnym przypadku może się
okazać, że nasze dane zostaną zbuforowane i zostaną wypisane później niż się spodziewamy, a w skrajnych przypadkach
wcale. Jeśli nie wiemy, do którego strumienia wypisywać, należy preferować standardowy błąd.
Przy pisaniu prawdziwych aplikacji konsolowych strumienia standardowego wyjścia używa się wyłącznie do wypisywania
rezultatów, a do czegokolwiek innego używa się standardowego błędu. Na przykład `grep` wypisze znalezione
wystąpienia na standardowe wyjście, ale ewentualne błędy przy otwarciu pliku trafią na standardowy błąd. Nawet nasze
makro `ERR` wypisuje błąd do strumienia standardowego błędu.

## Zadanie 6 - operacje niskopoziomowe na plikach

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

Wykonaj przykładowe <a href="{{< ref "../l1-example" >}}">ćwiczenie</a> z poprzednich lat. To zadanie szacuję na 60
minut, jeśli wyrobisz się w tym czasie to znaczy, że jesteś dobrze przygotowany/a do zajęć. Pamiętaj, że w aktualnym
układzie zajęć będzie dane zadnie na nieco ponad godzinę, więc trochę bardziej pracochłonne.

## Kody źródłowe z treści tutoriala
{{% codeattachments %}}
