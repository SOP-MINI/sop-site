---
title: "L0 - Środowisko wykonania programu POSIX"
date: 2022-02-05T17:26:02+01:00
weight: 10
---

# Laboratorium wstępne do zajęć z SOP1. Środowisko wykonania programu POSIX.

{{< hint info >}}

Pierwsze zajęcia wymagają od ciebie pewnego przygotowania, jednak głównie 
mają służyć wyjaśnieniu wszelkich wątpliwości co do zasad prowadzenia zajęć i konfiguracji środowiska na dalsze laboratoria.
Przeczytaj proszę informacje podane w działach [GIT]({{< ref "../../git">}}), [program]({{< ref "../../syllabus" >}}),
[regulamin]({{< ref "../../zasady" >}}) i [harmonogram]({{< ref "../../harmonogram" >}}).
Zerknij też na materiały [pomocnicze]({{< ref "../../materialy" >}}).

W dziale z [materiałami]({{< ref "../../materialy" >}}) znajdziesz tutoriale do samodzielnego wykonania
w ramach przygotowania do kolejnych zajęć. Podczas punktowanych zajęć zawsze poświęcamy
od 15 do 30 minut na odpowiadanie na pytania od studentów dotyczące danego działu.
Można też się skonsultować droga mailową lub osobiście w czasie dyżurów prowadzących laboratoria.

W ramach zerowych laboratoriów zrobimy proste ćwiczenie symulujące pracę na punktowanych laboratoriach.
Zadanie będzie wymagać od ciebie znalezienia kilku błędów i rozszerzenia programu o prostą funkcjonalność.
Dzięki temu będzie można sprawdzić, jak twoje środowisko sprawuje się w akcji.
W przypadku znalezienia problemów będzie można je wyeliminować przed pierwszymi punktowanymi laboratoriami.

{{< /hint >}}


## Przygotowanie środowiska

W przeciwieństwie do poprzednich przedmiotów nie wymagamy żadnego konkretnego środowiska.
Niemniej jednak odpowiedni edytor powinien potrafić:
- wyświetlać błędy kompilacji na żywo wraz z kodem (co powoduje, że dowiadujemy się o nich znacznie szybciej,
jak również pozwala nam znacznie łatwiej znaleźć odpowiedni fragment kodu),
- podpowiadać nazwy funkcji (przydatne przy wpisywaniu dłuższych nazw),
- dawać możliwość uruchomienia pisanego programu z debuggerem.

Przykładowym edytorem, który dobrze się sprawdzi, jest Visual Studio Code (nie mylić z Visual Studio) oraz QT Creator, które są
częstym wyborem ze względu na niski próg wejścia. Innymi częstymi wyborami są Emacs oraz Vim (oraz niedostępny domyślnie
w laboratorium Neovim), które jednak cechują się wyższym progiem wejścia. Wszystkie wymienione edytory wymagają
dodatkowej konfiguracji, którą warto przeprowadzić przed pierwszym punktowanym laboratorium. Oczywiście warto, żeby
konfiguracja edytora w domu była podobna do tej laboratorium.

Nie zalecamy korzystania z większych środowisk, takich jak na przykład ~~CLion~~, osobom które dobrze ich nie znają. 
W przeciwnym wypadku ich dodatkowe funkcje mogą bardziej przeszkadzać niż pomagać.
Ponadto nie wolno używać środowisk opartych na przeglądarce,
takich jak na przykład ~~<https://www.onlinegdb.com>~~.
Wynika to z tego, że przechowują one kod wyłącznie w przeglądarce,
co w przypadku problemów ze stabilnością komputera może skutkować utratą kodu i
koniecznością zaczęcia od nowa.

{{< hint info >}}
Jeżeli Państwo będą korzystać z Visual Studio Code, to może pojawić się problem, w którym edytor podświetla część nazw
jako nieznanych mimo tego, że program się kompiluje (zwykle są to nazwy związane z sygnałami, na przykład `sigset_t`
oraz `SIG_BLOCK`). Rozwiązaniem tego jest znalezienie w `C/C++: Edit Configurations (UI)` ustawienia `C standard` i zmiana go
z opcji zaczynającej się od `c` na odpowiadającą opcję zaczynającą się od `gnu`, na przykład `gnu17` zamiast `c17`.
{{< /hint >}}

## Zadania przygotowujące do laboratorium

{{< hint info >}}
Uwagi wstępne:
- To jest bardzo łatwy tutorial, ale za to długi, kolejne będą coraz trudniejsze i krótsze
- Szybkie przejrzenie tutoriala prawdopodobnie nic nie pomoże, należy samodzielnie uruchomić programy, sprawdzić jak
  działają, poczytać materiały dodatkowe takie jak strony man. W trakcie czytania sugeruję wykonywać ćwiczenia a na
  koniec przykładowe zadanie.
- Na żółtych polach podaję dodatkowe informacje, niebieskie zawierają pytania i ćwiczenia. Pod pytaniami znajdują się
  odpowiedzi, które staną się widoczne dopiero po kliknięciu. Proszę najpierw spróbować sobie odpowiedzieć na pytanie
  samemu a dopiero potem sprawdzać odpowiedź.
- Pełne kody do zajęć znajdują się w załącznikach na dole strony.
- Materiały i ćwiczenia są ułożone w pewną logiczną całość, czasem do wykonania ćwiczenia konieczny jest stan osiągnięty
  poprzednim ćwiczeniem, dlatego zalecam wykonywanie ćwiczeń w miarę przyswajania materiału.
- Materiały zakładają, że umiesz używać języka C w stopniu podstawowym (tzn. umiesz operować na pamięci, przetwarzać stringi itp.). Jeżeli nie czujesz się pewnie z tymi operacjami, polecam przypomnieć sobie materiał z przedmiotu Programowanie 1.
- Większość ćwiczeń wymaga użycia konsoli poleceń, zazwyczaj zakładam, że pracujemy w jednym i tym samym katalogu
  roboczym więc wszystkie potrzebne pliki są "pod ręką" tzn. nie ma potrzeby podawania ścieżek dostępu.
- Czasem podaję znak $ aby podkreślić, że chodzi o polecenie konsolowe, nie piszemy go jednak w konsoli np.: piszę "$make" w konsoli wpisujemy samo "make".
- To, co ćwiczymy, wróci podczas kolejnych zajęć. 
Jeśli po zajęciach i teście coś nadal pozostaje niejasne, proszę to poćwiczyć.
Prowadzący także mogą pomóc rozwiać wątpliwości.
{{< /hint >}}

## Zadanie 1 - stdout

Cel: Napisać i skompilować za pomocą programu make najprostszy program używający standardowego strumienia wyjścia

Co student musi wiedzieć:
- man 3p stdin
- man 3p printf
- man stdlib.h
- man make
- [Tutorial na temat gcc i make]({{< resource "tutorial.gcc_make.txt" >}})

Absolutnie nie doradzam szukania stron man przez googla, bo często znajdują się stare dokumenty a czasem dokumentacje
innych systemów (HP-UX,solaris), które mogą się różnić od standardu i zawierać błędy.
Najlepiej używać lokalnego systemu man na stacjach w laboratoriach.

Dokumentacja systemowa man podzielona jest na sekcje, opis tych sekcji jest podany na stronie opisu komendy man.

Uwaga! Proszę szukać informacji głównie na stronach posix (3p), a nie na stronach dokumentacji implementacji Linuksowej 
(2 i 3), różnice mogą być bardzo duże, obowiązuje znajomość standardu przed znajomością implementacji

Gdzie znaleźć dokumentację samego polecenia man?
{{< expand "Odpowiedź" >}}
```shell
man man
``` 
{{< /expand >}}

Jeśli ćwiczysz na własnym systemie Linux, upewnij się, że masz zainstalowane strony z dokumentacją posix, jeśli nie to z
pomocą "Wujka Google" sprawdź jak doinstalować te strony i zrób to.

Czemu wpisanie man printf nie pomoże nam w zrozumieniu funkcji printf?
{{< expand "Odpowiedź" >}}
Ponieważ otwiera się pierwsza sekcja pomocy, a ta dotyczy poleceń powłoki, ponieważ istnieje polecenie printf to
dostajemy informacje na jego temat.
Należy podać numer sekcji: `man 3 printf` aby obejrzeć pomoc Linuksową lub lepiej `man 3p printf` aby obejrzeć opis
standardu.
{{< /expand >}}

<em>rozwiązanie <b>prog1.c</b>:</em>
{{< includecode "prog1.c">}}

Zwróćcie uwagę na konieczność zwrócenia kodu wyjścia w funkcji main, chociaż można zmienić int na void to kłóci się to z
wymogami co do wartości, jakie mają zwracać wszystkie procesy wg POSIX.

`\n` na końcu tekstu nie jest przypadkowe, bez tego po wykonaniu programu w konsoli kolejny tzw. znak zachęty (zazwyczaj
$) nie pojawi się w nowej linii. Dobrą praktyką w systemach UNIX jest kończenie strumieni tekstowych oraz plików tekstu
znakiem nowej linii. Drugi aspekt to chęć opróżnienia bufora standardowego strumienia - stdout i stderr są buforowane
liniami, będzie jeszcze o tym mowa.

Zamiast podawać zero jako wartość zwracaną w razie pełnego powodzenia programu lepiej korzystać ze zdefiniowanych
stałych: `EXIT_SUCCESS` i `EXIT_FAILURE`, znajdziecie je w man `stdlib.h`

Skąd wiadomo jakie pliki nagłówkowe trzeba włączyć?
{{< expand "Odpowiedź" >}} stdio.h z man 3 printf, stdlib.h dla stałych {{< /expand >}}

Skompiluj program poleceniem "make prog1", używasz w ten sposób domyślnego szablonu kompilacji programu GNU make. 
Uruchom program wynikowy. Czemu taki sposób kompilacji będzie dla nas nieprzydatny?
{{< expand "Odpowiedź" >}} Brak flagi -Wall, nie wiemy jakie warningi "czają" się w naszym kodzie.  {{< /expand >}}

<em>rozwiązanie <b>Makefile</b></em>
```makefile
all: prog1
prog1: prog1.c	
	gcc -Wall -fsanitize=address,undefined -o prog1 prog1.c
.PHONY: clean all
clean:
	rm prog1
```

Struktura pliku Makefile jest opisana w tutorialu podanym na początku ćwiczenia, najważniejsze dla nas to pamiętać o obowiązkowym tabulatorze przed instrukcją kompilacji, pierwszy cel w pliku jest jednocześnie domyślnym celem (make == make all w powyższym przykładzie), cele opisane jako PHONY nie są powiązane z fizycznie istniejącymi plikami.

Nazwa każdego celu w makefile (poza .PHONY) musi być powiązana z plikiem wynikowym kompilacji (lub linkowania), częstym błędem studentów jest zapominanie o konsekwentnej zmianie wszystkich nazw w pliku Makefile podczas kopiowania powyższego pliku do innych przykładów!

Jak za pomocą programu make i podanego Makefile usunąć stary plik wykonywalny?
{{< expand "Odpowiedź" >}}
```shell
make clean
```
{{< /expand >}}

Jak za pomocą programu make i podanego Makefile przeprowadzić kompilacje?
{{< expand "Odpowiedź" >}}
`make` lub `make prog1`
{{< /expand >}}

Jak przekierować wyjcie tego programu do pliku?
{{< expand "Odpowiedź" >}} 
```shell
./prog1 > plik.txt
```
{{< /expand >}}

Jak teraz wyświetlić ten plik?
{{< expand "Odpowiedź" >}} 
```shell
cat plik.txt
```
{{< /expand >}}

Zrób kopię programu prog1.c, niech się nazywa prog1b.c. Przerób plik Makefile tak, aby kompilował ten nowy plik do binarki o nazwie prog1b, zwróć uwagę, aby przy faktycznej kompilacji (a nie tylko w Makefile) była obecna flaga -Wall. Jeśli uda Ci się to za pierwszym razem to spróbuj sprowokować błąd celowo zmieniając niektóre prog1b z powrotem na prog1 w różnych konfiguracjach.

## Zadanie 2 - stdin, stderr

Cel: Rozwinąć prog1.c tak, aby wypisywał na ekran powitanie dla imienia podanego z linii poleceń, imiona powyżej 20
znaków powinny generować błąd programu (natychmiastowe wyjście i komunikat)

Co student musi wiedzieć: 
- man 3p fscanf
- man 3p perror
- man 3p fprintf

Dla ułatwienia pisania dodajmy makro:
```c
#define ERR(source) (perror(source),\
fprintf(stderr, "%s:%d\n", __FILE__, __LINE__),\
exit(EXIT_FAILURE))
```

-Zwróć uwagę na makra `__FILE__` i `__LINE__`, pokazują lokalizację wywołania w pliku źródłowy, to cecha preprocesora C.
- Brak średnika po makro jest celowy.
- Komunikaty o błędach zawsze wypisujemy na stderr.
- W reakcji na większość błędów kończymy program, stąd exit.

<em>dodatkowy kod do <b>prog2.c</b></em>

```c
char name[22];
scanf("%21s", name);
if (strlen(name) > 20) ERR("Name too long");
printf("Hello %s\n", name);
```

Przerób Makefile z poprzedniego przykłady tak, aby można było skompilować ten program.

Uruchom i przetestuj program.

Czemu w kodzie pojawia się 21 jako rozmiar maksymalny w formatowaniu scanf (%21s)?
{{< expand "Odpowiedź" >}}
Czytając maksymalnie 20 znaków, nie wiemy, czy użytkownik podał dokładnie 20, czy może więcej.
Czytając maksymalnie 21, wiemy o przekroczeniu limitu. 
{{< /expand >}}

Czemu deklarujemy 22 jako rozmiar tablicy na ciąg znaków, skoro czytamy najwyżej 21 znaków?
{{< expand "Odpowiedź" >}}
W C ciągi znaków kończą się znakiem zera, scanf dopisze to zero za ostatnim znakiem, 
zatem każdy ciąg zawsze musi mieć ten dodatkowy znak doliczony do rozmiaru.
{{< /expand >}}

Jak można zmienić sposób wywołania tego programu tak, aby komunikat o ewentualnym błędzie wykonania nie pojawił się na
ekranie?
{{< expand "Odpowiedź" >}}
Można przekierować stderr do /dev/null, np. tak: `./prog2 2>/dev/null`
{{< /expand >}}

Gdy podajemy zbyt długie imię, pojawia się komunikat "Name too long: Success", czemu success? Zmienna errno nie jest
ustawiana, bo to błąd w naszym kodzie, a nie w wywołaniu funkcji bibliotecznej - makro jest napisane przyszłościowo pod
kątem błędów w funkcjach.

Gdy podamy podwójne imię np.: "Anna Maria" program wypisze tylko pierwsze, w scanf prosimy o jeden wyraz, a nie całą
linię. Całe linie będziemy czytać za chwilę.

Program nie zaczyna się od zapytania użytkownika o imię, tylko od razu oczekuje danych. Takie podejście jest bardzo
UNIXowe, takie programy lepiej nadają się do użycia w skryptach lub w przetwarzaniu danych z plików (następne zadanie).
Na drugim biegunie jest podejście interaktywne (bardziej w stylu Windows), z grzeczną prośbą o podanie imienia na
początku. Wybór stylu interakcji zależy od zastosowania, można połączyć oba, jeśli rozpoznamy (f. isatty) czy program
działa interaktywnie na konsoli, czy może jest to tzw. przetwarzanie wsadowe.

W prog2.c nie wykorzystano wartości zwracanej przez scanf. Niestety zbyt długi wyraz na stdin
(dla specyfikacji %s) nie jest traktowany jako błąd i scanf() zwraca 1 (tak jak i dla wyrazu krótszego od 21 znaków)

W programie występują tzw. "magic numbers" 20,21,22. Nie powinno się ich tworzyć to zły styl programowania, trudno potem
zmienić limit z 20 linii na 30 bez analizy kodu. Powinno się dodać stałą (#define MAXL 20) i wszystko od niej
wyprowadzić (MAXL+1 , MAXL+2)

Musimy wcisnąć enter, aby program mógł wykonać scanf, wynika to z liniowego buforowania terminala, natychmiastowe
czytanie znaków z klawiatury wymaga sporo pracy w tym usunięcie buforowania terminala (man 3p tcsetattr), dodania flagi
O_NONBLOCK i czytania bezpośrednio z deskryptora, a nie strumienia. Zazwyczaj łatwiej do tego użyć biblioteki (np.
ncurses). Nie będziemy się tym zajmować na SOP.

## Zadanie 3 - stdin cd..

Cel: Rozwinąć prog2.c tak, aby wypisywał na ekran powitanie dla każdego ciągu imion (słów) podanego ze standardowego
wejścia. Program ma pobierać całe linie tekstu (do 20 znaków) i wypisywać na stdout. Operacja jest powtarzana aż do
napotkania końca strumienia (C-d). Linie dłuższe niż 20 znaków mają być skracane, ale błąd nie ma być zgłaszany

Co student musi wiedzieć: 
- man 3p fgets

`C-d` (czyli w notacji Windowsowej Ctrl d) powoduje zamknięcie strumienia, zupełnie jakbyśmy wykonali na nim close.
Gdy strumień napotyka permanentny koniec danych to taki stan nazywamy EOF (end of file).
Zwróć uwagę, że to nie to samo co chwilowy brak danych, 
gdy program czeka na "powolnego" człowieka, aby ten coś wpisał.
Są dwie różne sytuacje które musisz rozróżniać!

`C-d` działa tylko po znaku nowej linii! Strumień tekstowy powinien kończyć się znakiem nowej linii.

`C-c` wysyła SIGINT do aktywnej grupy procesów - zazwyczaj pozwala zgrabnie zakończyć cały program

`C-z` zwiesza program ( wysyłając SIGSTOP), można potem poleceniem jobs obejrzeć listę takich wiszących programów i
przywrócić wybrany do życia piszac %N gdzie N to numer wiszącego procesu

`C-\` wysyła SIGQUIT, kończy program i generuje zrzut pamięci

`C-s` zamraża terminal, nie ma wpływu na proces, o ile ten nie przepełni bufora, wtedy musi czekać. Mając nawyk
zapisywania pracy Ctrl S z Windows, łatwo można sobie niechcący zawiesić terminal. Aby odwiesić terminal, naciskamy C-q.

Powyższe skróty działają w powłoce bash, której używamy w labach (też w kilku innych powłokach).

<em>kod do <b>prog3.c</b></em>
{{< includecode "prog3.c" >}}
<em>Nowy <b>Makefile</b> do kompilacji wielu programów (celów):</em>
```makefile
all: prog1 prog2 prog3
prog1: prog1.c
	gcc -Wall -fsanitize=address,undefined -o prog1 prog1.c
prog2: prog2.c
	gcc -Wall -fsanitize=address,undefined -o prog2 prog2.c
prog3: prog3.c
	gcc -Wall -fsanitize=address,undefined -o prog3 prog3.c
.PHONY: clean all
clean:
	rm prog1 prog2 prog3
```

Skompiluj za pomocą polecenia make i uruchom program, jak dla powyższego Makefile skompilować tylko jeden cel?
{{< expand "Odpowiedź" >}}
```shell
make prog3
```
{{< /expand >}}
 
Sprawdź, jak się zachowa dla ciągów 20 i 21 znakowych. Czemu akurat tak?
{{< expand "Odpowiedź" >}}
Dla ciągu 21 znakowego w tablicy nie zmieści się znak nowej linii. 
fgets nie przepełnia buforu, zatem obcina wszystko, co przekracza podany limit - w tym przypadku znak nowej linii. 
{{< /expand >}}
 
Czemu w wywołaniu printf nie dodaliśmy znaku nowej linii na końcu, a mimo to powitania wyświetlają się w oddzielnych liniach?
{{< expand "Odpowiedź" >}} 
fgets pobiera też \n o ile zmieści się w buforze.
Wypisując taki ciąg nie musimy już dodawać swojego `\n`.
{{< /expand >}}

Czemu rozmiar bufora jest MAX_LINE+2?
{{< expand "Odpowiedź" >}}
Musi się zmieścić znak nowej linii i kończące łańcuch znakowy zero
{{< /expand >}}

Zwróć uwagę, że `fgets` pracuje na dowolnym strumieniu, nie tylko na `stdin`.

Ten program jest wolny od "magic numbers" i tak powinno być zawsze.

<em>Plik testowy <b>dane.txt</b></em>
```
Alice 
Marry Ann
Juan Luis
Tom
```

Utwórz plik tekstowy z powyższą zawartością.

Jak skłonić nasz program, aby pobrał dane z pliku, a nie z klawiatury (na dwa sposoby)?
{{< expand "Odpowiedź 1" >}} 
przekierować plik na stdin: `./prog3 < dane.txt`
{{< /expand >}}
{{< expand "Odpowiedź 2" >}}
wykorzystać potok : `cat dane.txt | ./prog3`
{{< /expand >}}

## Zadanie 4 - parametry wywołania programu 1

Cel: Napisać program wyświetlający wszystkie parametry wywołania programu

Co student musi wiedzieć: 
- man 1 xargs

<em>kod do <b>prog4.c</b></em>
{{< includecode "prog4.c" >}}

<em>Pełny kod <b>Makefile</b></em>

```makefile
CC=gcc
CFLAGS=-Wall -fsanitize=address,undefined
LDFLAGS=-fsanitize=address,undefined
```

Aby używać szablonów kompilacji, trzeba podać nazwę pliku wykonywalnego jako parametr make.
W tym przykładzie będzie to "$ make prog4".
Tym razem kompilacja jest z pożądaną flagą -Wall, gdyż podany Makefile modyfikuje standardowy szablon.

Aby parametrem uczynić dwa lub więcej słów trzeba ująć je w apostrofy albo wpisać znak \ przed każdą spacją (lub innym
tzw. białym znakiem).

Argument o numerze zero to zawsze nazwa programu!

Skompiluj i uruchom program, przetestuj dla różnych parametrów.

Jak za pomocą programu xargs przekształcić zawartość pliku dane.txt na argumenty wywołania naszego programu?
{{< expand "Odpowiedź 1" >}} 
każdy wyraz to oddzielny argument: `cat dane.txt | xargs ./prog4`
{{< /expand >}}
{{< expand "Odpowiedź 2" >}} 
każda linia to argument: `cat dane.txt |tr "\n" "\0"| xargs -0 ./prog4`
{{< /expand >}}

Używając powyższego dla większych plików, trzeba pamiętać, że długość linii poleceń jest limitowana (różnie w różnych
systemach). `xargs` może podzielić dane na więcej wywołań, gdy sobie tego zażyczymy.

## Zadanie 5 - parametry wywołania programu 2

Cel: Napisać program przyjmujący 2 argumenty: imię i licznik n>0, podanie złej ilości parametrów lub niepoprawnego
licznika ma zatrzymać program. Dla poprawnych argumentów program wypisuje "Hello IMIĘ"   n - raz

Co student musi wiedzieć: 
- man 3p exit
- man 3p atoi
- man 3p strtol

<em>Funkcja pomocnicza w pliku <b>prog5.c</b></em>
```c
void usage(char *pname)
{
  fprintf(stderr, "USAGE:%s name times>0\n", pname);
  exit(EXIT_FAILURE);
}
```

Zwróć uwagę, że funkcji exit możesz podawać takie same statusu jak zwracasz w main.

Dobrą praktyką jest wypisywanie tzw. usage przy braku wymaganych argumentów

<em>Kod do pliku <b>prog5.c</b>:</em>
{{< includecode "prog5.c" >}}

Skompiluj ten program, używając uniwersalnego Makefile z poprzedniego zadania.

Jak działa program dla wartości powtórzeń niepoprawnie podanych, czemu tak?
{{< expand "Odpowiedź" >}}
Nic nie wypisze, ponieważ atoi pod Linuksem zwraca zero, jeśli nie można zamienić tekstu na liczbę.
{{< /expand >}}

Czemu argc ma być 3, mamy przecież 2 argumenty? 
{{< expand "Odpowiedź" >}}
`argc` to jest licznik elementów w tablicy argv, mieszczą się tam dwa argumenty i nazwa programu, w sumie 3 elementy
{{< /expand >}}

Zwróć uwagę na obrócony zapis porównania (0==j), jaki z takiego pisania może być zysk? Jeśli pomyłkowo zapiszę (0=j) to
się nie skompiluje i szybko dowiem się o błędzie. Gdybym napisał (j=0) to się skompiluje, a o miejscu wystąpienia błędu
dowiem się dopiero po czasochłonnej analizie kodu. To jest dodatkowe zabezpieczenie.

Zwróć uwagę, że starsze standardy C nie dopuszczały deklarowania zmiennych w środku kodu, ale tak jest czytelniej, bo
zmienne deklarujemy, gdy stają się potrzebne.

Co zwraca atoi dla nie liczb? W praktyce zero wg. POSIX - undefined. Czasem chcemy mieć nad tym kontrolę, umieć
rozróżnić zero od błędu, wtedy używamy funkcji strtol.

Argumenty programu można nadpisać w trakcie jego działania, w tym i nazwę! Może to służyć próbie ukrycia procesu i/lub
jego argumentów (np. hasła).

## Zadanie 6 - parametry wywołania programu 3

Cel: Napisać program przyjmujący dowolną liczbę parametrów typu -t x i dowolną liczbę parametrów -n NAME. Parametry mogą
wystąpić w dowolnej kolejności. Dla każdego wystąpienia parametru -n wypisujemy dokładnie x razy powitanie "Hello NAME".
Początkowo x=1, ale kolejne wystąpienia -t mogą to zmienia

Np.: `./prog6 -n Anna -t 2 -n John -n Matt -t 1 -n Danny` wypisze
```
Hello Anna
Hello Jonh
Hello Jonh
Hello Matt
Hello Matt
Hello Danny
```

Co student musi wiedzieć: 
- man 3p getopt 
- [dokumentacja GNU](http://www.gnu.org/software/libc/manual/html_node/Using-Getopt.html)

<em>poprawmy funkcję usage w <b>prog6.c</b></em>
```c
fprintf(stderr, "USAGE:%s ([-t x] -n Name) ... \n", pname);
```
<em>kod do pliku <b>prog6.c</b></em>
{{< includecode "prog6.c" >}}

Sprawdź, jak działa program dla niepoprawnych argumentów lub jeśli podamy argumenty pozycyjne (niepoprzedzone oznaczeniem
opcji) oraz gdy nie podamy argumentów lub parametrów.

W programie zmienną c (int) porównujemy ze znakami 't' i 'n' (char), to poprawne podejście w C char jest po prostu
jedno-bajtowym typem numerycznym.

W parametrach getopt opisujemy pożądaną składnię - zazwyczaj literki opcji z opcjonalnymi przyrostkami - `:` znaczy ma
argument, natomiast `::` znaczy, że ma opcjonalny argument.

Komunikaty dla błędnych opcji "invalid option -- '?'" są generowane w samej funkcji getopt, jeśli chcemy je wyłączyć
ustawiamy zmienną globalną opterr na zero.

Zmienne optarg i optind to kolejne zmienne globalne typu extern z biblioteki powiązane z getopt.
Co zawiera pierwsza, widać w kodzie.
Druga mówi, ile opcji z linii wywołania do danego momentu przetworzono.
Trzeba przy tym wiedzieć, że argumenty niebędące opcjami lub ich argumentami są w trakcie przetwarzania przesuwane na koniec linii poleceń, teraz
już wiesz co robi ostatni warunek w kodzie powyżej - sprawdza czy nie było argumentów pozycyjnych spoza wymaganej
składni.

Zwróć uwagę, że lepiej byłoby najpierw przeprowadzić parsowanie opcji kontrolnie a dopiero potem wykonać program, ale
jako przykład tyle wystarczy.

`optarg` to statyczny bufor nadpisywany kolejnymi wywołaniami, czasem gdy musimy przechować te wartości trzeba je
kopiować (strcpy, alokacja pamięci).

Długie opcje np.: `--count 10` są obsługiwane przez `getopt_long` ale nie są one standaryzowane przez POSIX to
rozszerzenie GNU i nie będziemy ich używać.

## Zadanie 7 - zmienne środowiskowe 1

Cel: Napisać program wypisujący listę wszystkich zmiennych środowiskowych

Co student musi wiedzieć: 
- man 3p environ
- man 7 environ

<em>zawartość do pliku <b>prog7.c</b></em>
{{< includecode "prog7.c" >}}

Własną zmienna mogę dodać np. tak: `TVAR2="122345" ./prog7` , pojawi się na wypisie, ale nie zostanie zapamiętana w
powłoce, tzn. kolejne wywołania programu  `./prog7` już jej nie pokażą.

Mogę też dodać zmienną trwale do środowiska powłoki `export TVAR1='qwert'` i teraz, ilekroć wywołam program `./prog7`
ta zmienna wciąż tam będzie.

Czy jeśli uruchomię drugą powłokę z menu środowiska i w niej uruchomię program to zmienna TVAR1 nadal będzie widoczna?
{{< expand "Odpowiedź" >}}
Nie, te dwie powłoki dziedziczą zmienne od procesu uruchamiającego programy w środowisku,
zatem druga powłoka nie może dziedziczyć nic "w bok" od pierwszej.
{{< /expand >}}

Czy jeśli uruchomię drugą powłokę z pierwszej i w niej uruchomię program to zmienna TVAR1 nadal będzie widoczna?
{{< expand "Odpowiedź" >}}
Tak, druga powłoka dziedziczy zmienne od swojego rodzica, czyli od pierwszej powłoki. 
{{< /expand >}}

## Zadanie 8 - zmienne środowiskowe 2

Cel: Zmodyfikować prog3.c tak, aby każda linia tekstu "Hello NAME" była powielona tyle razy, ile nakazuje mnożnik
przekazany przez zmienną środowiskową TIMES. Na zakończenie programu ustawić zmienną środowiskową RESULT na "Done".

Co student musi wiedzieć: 
- man 3p getenv
- man 3p putenv
- man 3p setenv
- man 3 system (3p) jest trochę mniej czytelny

<em>kod do pliku <b>prog8.c</b></em>
{{< includecode "prog8.c" >}}

Zwróć uwagę na możliwy błąd braku zmiennej środowiska i jego obsługę na początku kodu. Dobry programista nigdy nie
pomija sprawdzania błędów, jeśli się śpieszysz i pomijasz to Twój kod jest gorszy - musisz być tego świadomy/a.
Najgorzej, gdy programista pomija sprawdzanie błędów z niewiedzy lub ignorancji.

Drugi przykład sprawdzenia błędu jest po wywołaniu putenv.
Skoro tak często mamy to robić to przydałoby się makro.
Coś na wzór ERR, którego już raz użyliśmy.

Innym sposobem na manipulowanie środowiskiem jest funkcja setenv.

Funkcja system służy nam do sprawdzenia tego, co i tak wiemy.
Skoro nie było błędu putenv to nie spodziewamy się, aby tutaj był.
Jednak na potrzeby przykładu robimy takie sprawdzenie dwa razy, raz getenv("RESULT"), a potem jeszcze system.

Użycie funkcji system jest równoważne z uruchomieniem powłoki jako procesu dziecka, komenda jest podana. Funkcja system
zwróci to, co normalnie zwróciłby program, to co można w bash'u sprawdzić wywołaniem "$ $?" (pierwszy dolar nie jest
częścią polecenia), czyli status zakończenia ostatniego polecenia/programu.

Co ciekawe niektóre systemy nie mają powłoki!
Gdy brak shella funkcja system nie działa. Dostajemy błąd 127.

Wykonaj program z różnymi ustawieniami TIMES.

Jak po wykonaniu programu sprawdzić, czy zmienna RESULT jest ustawiona?
{{< expand "Odpowiedź" >}}  
`env|grep RESULT` nie będzie ustawiona, bo zmienne nie są propagowane wzwyż drzewa procesów. 
Zmienna była ustawiona tylko w programie i w powłoce w nim na chwilę wywołanej (f. system),
ale po zakończeniu tych procesów się nie zachowała. 
{{< /expand >}}

### Zadanie do sprawdzenia środowiska. 

Cel: Napisać trywialny program "hello world", skompilować go, uruchomić, spakować i przesłać na wskazane konto. 
*Co student musi wiedzieć:*
- znać dowolne środowisko programisty w systemie Linux
- znać podstawowe flagi polecenia `gcc`
- umieć posługiwać się konsolą
  
*Co mile widziane gdy student wie:*
- podstawowa wiedza o programie `git`

*rozwiązanie **prog1.c**:*

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    printf("Hello world\n");
    return EXIT_SUCCESS;
}
```

*kompilacja:*

```shell
gcc -Wall -fsanitize=address,undefined -o prog1 prog1.c
```

Użycie flagi `-Wall` jest obowiązkowe w trakcie laboratorium, 
program nie powinien generować istotnych ostrzeżeń. 
Każde zadanie wymaga również użycia odpowiedniego zestawu opcji `-fsanitize`. 
[Więcej informacji o sanitizerach.]({{< ref "../sanitizers" >}})
{{< hint danger >}}
**UWAGA** nie wolno dowolnie mieszać parametrów polecenia `gcc`, 
parametr `-o` ma argument — ścieżkę do wynikowego programu.
{{< /hint >}}

*uruchomienie:*

```shell
./prog1
```

## Schemat użycia GITa na laboratorium

{{< hint info >}}

Tą sekcję należy wykonać w sali na laboratorium L0.
Przed przejściem przez tą sekcję trzeba zapoznać się z opisem użycia [GITa]({{< ref "../../git" >}})
Wymaga ona dostępu do serwera działającego tylko w trakcie trwania laboratorium.
Zachęcam jednak do przeczytania jej i przygotowania się do wykonania poniższych kroków.

{{< /hint >}}

Na laboratorium każde zadanie będzie rozwiązywane w takim repozytorium.
Twoim celem jest śledzenie zmian w repozytorium w trakcie trwania laboratorium i ich synchronizacja z serwerem.
**Jeżeli jakiś kod nie znajdzie się na serwerze, nie będzie oceniany.**

Zanim przejdziemy do pobierania kodu, trzeba uzyskać swój osobisty klucz SSH z platformy [LeON](https://leon.pw.edu.pl).
W dziale *klucze ssh* należy pobrać dwa pliki: `id_ed25519` oraz `id_ed25519.pub` oraz przekopiować je do folderu `~/.ssh`.
Plik `id_ed25519` jest *kluczem prywatnym* - to znaczy że może być użyty do uwierzytelnienia wobec serwera znającego klucz publiczny (oraz do wielu innych rzeczy, jak szyfrowanie, podpisy cyfrowe i inne).
Każdy klucz jest unikatowy i - zgodnie z nazwą - prywatny, tzn. działa jak hasło. Nie powinno się go udostępniać, ponieważ, podobnie jak z hasłem - jeżeli inna osoba wejdzie w jego posiadanie jest w stanie się pod nas podszyć i np. skasować nasze rozwiązanie z serwera.

W plikach na LeONie znajduje się również pusty plik postaci `imię_nazwisko.txt` - `imię_nazwisko` jest loginem na serwerze gita.

Pierwszym krokiem będzie wykonanie kopii zdalnego repozytorium na swoją stację roboczą poleceniem

```shell
$ git clone ssh://gitolite@vl01/imię_nazwisko/l0pl
```

Polecenie stworzy folder o nazwie `l0pl` i wykona do niego kopie plików.
W tym folderze należy wykonać laboratorium.
W przypadku gdyby polecenie zakończyło się niepowodzeniem należy sprawdzić, czy dobrze wpisaliśmy w adresie repozytorium nasz login postaci `imię_nazwisko` - powinien być bez polskich znaków, taki sam jak we wspomnianych plikach na LeONie - najlepiej przekopiować stamtąd. W przypadku gdy mimo to pobieranie repozytorium nie działa należy zgłosić to prowadzącemu.

Zadanie składa się z etapów.
Po zakończeniu etapu należy wykonać commita do repozytorium.
Commit powinien mieć nazwę mówiącą, którego etapu dotyczy oraz co dodaje/naprawia, w rodzaju "Etap 2 - poprawka zwalniania pamięci"
Aby zsynchronizować lokalne zmiany do serwera, należy wykonać polecenie

```shell
$ git push
```

Proszę pamiętać, że za etap można uzyskać punkty dopiero, gdy jego kod znajdzie się na zdalnym repozytorium.
Możliwość synchronizacji z serwerem zostaje utracona punktualnie z końcem czasu przeznaczonego na zadanie.

Aby rozwiązanie zostało przyjęte przez serwer musi spełniać następujące warunki:
- Jedynie pliki rozwiązania (`.c`) zostały zmodyfikowane - modyfikacja jakichkolwiek innych plików, np. makefile spowoduje odrzucenie rozwiązania
- Pliki rozwiązania są poprawne sformatowane. W folderze repozytorium znajduje się plik `.clang-format` będący konfiguracją dla programu `clang-format` zainstalowanego na komputerach laboratorium. Umożliwia on ładne sformatowanie kodu poprzez polecenie `clang-format -i <nazwapliku>.c` . Wiele edytorów pozwala na integrację z `clang-format` i automatyczne formatowanie pliku podczas pisania albo przy zapisie.
- Rozwiązanie nie jest zbyt długie - domyślnie 600 linii (zadania laboratoryjne powinny być zwykle możliwe do rozwiązania w mniej niż 300 liniach)
- Rozwiązanie powinno się kompilować bez błędów przy użyciu makefile zawartego w repozytorium

W przypadku niespełnienia którego z warunków serwer odrzuci rozwiązanie z odpowiednim komunikatem. Należy poprawić swój kod, zrobić commit i ponownie wykonać push. Serwer pozwala wykonać jeden push na minutę.
