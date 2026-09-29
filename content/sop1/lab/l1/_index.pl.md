---
title: "L1 - System plików 1"
date: 2022-02-05T18:39:22+01:00
weight: 20
---

# Tutorial 1 - System plików

{{< hint info >}}
Ten tutorial zawiera wyjaśnienia działania funkcji wymaganych na laboratoriach oraz ich parametrów.
Jest to jednak wciąż jedynie poglądowy zbiór najważniejszych informacji -- 
należy **koniecznie przeczytać wskazane strony manuala**, aby dobrze poznać i zrozumieć wszystkie szczegóły.

{{< /hint >}}

W tym tutorialu zajmiemy się obsługą systemu plików poprzez wysokopoziomowe API, będące częścią jezyka C, oraz jego rozszerzeniami POSIX.
Charakterystyczną cechą tego API jest operowanie na ścieżkach do pliku oraz strukturach `DIR` i `FILE`.
Istnieje również niskopoziomowe API POSIX operujące na deskryptorach.
Zajmiemy się nim na następnym laboratorium.
Zasadniczo powinniście być zaznajomieni ze standardowymi funkcjami języka C po przedmiocie "Programowanie 1" - tutaj powtórzymy je tylko pokrótce.
Warto powtórzyć sobie informacje z tego przedmiotu, np. poprzez [cppreference](https://en.cppreference.com/c/io).


## Przeglądanie katalogu

Przejrzenie katalogu umożliwia nam poznanie nazw oraz atrybutów zawartych w nim plików. 
Realizuje to zadanie np. terminalowe polecenie `ls -l`. Aby natomiast uzyskać dostęp do
tych informacji z poziomu języka C, należy "otworzyć" przeglądany katalog funkcją
`opendir`, a następnie kolejne rekordy odczytać funkcją `readdir`. Wspomniane funkcje
obecne są w pliku nagłówkowym `<dirent.h>` (`man 3p fdopendir`). Spójrzmy na definicje obu funkcji:

```
DIR *opendir(const char *dirname);
struct dirent *readdir(DIR *dirp);
```

Jak widzimy, `opendir` zwraca nam wskaźnik na obiekt typu `DIR`, którym będziemy się posługiwać przy odczytywaniu
danych o zawartości katalogu. Funkcja `readdir` zwraca natomiast wskaźnik na strukturę typu `dirent`, która posiada 
(wg POSIX) następujące pola (`man 0p dirent.h`):

```
ino_t  d_ino       -> identyfikator pliku (numer inode, więcej w man 7 inode)
char   d_name[]    -> nazwa pliku
```

Pozostałe dane o pliku można odczytać używając funkcji `stat` lub `lstat` z pliku nagłówkowego `<sys/stat.h>` (`man 3p fstatat`).
Ich definicje są następujące:

```
int  stat(const char *restrict path, struct stat *restrict buf);
int lstat(const char *restrict path, struct stat *restrict buf);
```
- `path` jest tutaj ścieżką do pliku,
- `buf` jest wskaźnikiem do (wcześniej zaalokowanej) struktury typu `stat` (nie mylić z nazwą funkcji!) 
przechowującej informacje o pliku.

*Manualowe* definicje argumentów funkcji często zawierają słowo kluczowe `restrict`. Jest to deklaracja
mówiąca, że dany argument musi być blokiem pamięci rozłącznym z innymi argumentami. W takim przypadku, 
podanie tego samego bloku pamięci (np. tego samego wskaźnika) jest poważnym błędem i może spowodować 
SEGFAULT lub nieprawidłowe działanie programu. 

Jedyną różnicą w działaniu funkcji `stat` i `lstat` jest obsługa linków. `stat` zwróci informacje o pliku, do 
którego dany link prowadzi, natomiast `lstat` zwróci informacje o samym linku. 

Struktura `stat` zawiera m.in. informacje o rozmiarze pliku, właścicielu, czy dacie ostatniej modyfikacji. Dostępne
są też makra sprawdzające typ pliku. Poniżej znajdują się ważniejsze przykłady takich makr:
- Makra przyjmujące `buf->st_mode` (pole typu `mode_t`):
   - `S_ISREG(m)` -- czy mamy do czynienia ze zwykłym plikiem,
   - `S_ISDIR(m)` -- czy mamy do czynienia z katalogiem,
   - `S_ISLNK(m)` -- czy mamy do czynienia z linkiem.
- Istnieją też makra `S_TYPE*(buf)` przyjmujące sam wskaźnik `buf`, służące identyfikacji typów plików takich, jak
semafory czy pamięć dzielona (więcej o tym będzie w przyszłym semestrze).
Szczegóły znajdują się w manualu `man sys_stat.h`. Warto się zapoznać ze wszystkimi atrybutami struktury `stat` 
i makrami, jest tego dość dużo.

Po przejrzeniu katalogu należy (będąc dobrym programistą i chcąc zdać przedmiot) pamiętać o zwolnieniu zasobów 
za pomocą funkcji `closedir`.

### Informacje techniczne

W celu przejrzenia całego katalogu, funkcję `readdir` należy wywoływać tyle razy, aż nie zwróci `NULL`.
W przypadku wystąpienia błędu, zarówno `opendir`, jak i `readdir` zwracają `NULL`. Wynika z tego ważny wniosek
w przypadku funkcji `readdir`: przed jej wywołaniem należy wyzerować zmienną `errno`, a w razie zwrócenia `NULL`
sprawdzić, czy ta zmienna nie została ustawiona na niezerową wartość (oznaczającą błąd).
`errno` jest zmienną globalną używaną przez funkcje systemowe do wskazania kodu napotkanego błędu.

Funkcje `stat`, `lstat` i `closedir` zwracają `0` w razie sukcesu, inna wartość oznacza błąd.

### Zadanie

Napisz program zliczający: pliki, linki, katalogi i inne obiekty w katalogu roboczym (bez podkatalogów).

### Rozwiązanie zadania

Nowe strony z manuala:
```
man 3p fdopendir (tylko opis opendir)
man 3p closedir
man 3p readdir
man 0p dirent.h
man 3p fstatat (tylko opis stat i lstat)
man sys_stat.h
man 7 inode (pierwsza połowa sekcji "The file type and mode")
```

rozwiązanie `l1-1.c`:
{{< includecode "l1-1.c" >}}

### Uwagi i pytania

- Uruchom ten program w katalogu, w którym nie ma żadnych podkatalogów, czy wyniki zgadzają się z tym czego oczekujemy tj. zero katalogów i, ewentualnie, pliki?
{{< answer >}} 
Nie, są dwa katalogi, program policzył katalogi `.` i `..`. Każdy katalog ma *hardlinka* na samego siebie (`.`) i katalog nadrzędny (`..`). 
{{< /answer >}}

- Jak utworzyć link symboliczny do testów? 
{{< answer >}} 
```shell
ln -s prog9.c prog_link.c
```
{{< /answer >}}

- Przeczytaj `man readdir`. Jakie pola zawiera struktura opisująca obiekt w systemie plików (`dirent`) w Linuksie? 
{{< answer >}} 
Identyfikator, nazwę i 3 inne pola nie objęte standardem.
{{< /answer >}}

- Tam, gdzie implementacja Linuksa odbiega od standardu, trzymamy się zawsze standardów, to powoduje większą przenośność
naszego kodu pomiędzy różnymi Unixami.

- Zwróć uwagę na sposób obsługi błędów funkcji systemowych, zazwyczaj robimy to tak: `if(fun()) ERR()` (makro `ERR` było już
omawiane wcześniej). Wszystkie funkcje mogące sprawiać kłopoty (w szczególności, prawie wszystkie funkcje systemowe) 
należy sprawdzać. Większość błędów, jakie napotkamy, będzie wymagać zakończenia programu. Wyjątki omówimy w kolejnych tutorialach.

- Zwróć uwagę na użycie katalogu `.` w kodzie, nie musimy znać aktualnego katalogu roboczego, tak jest prościej.

- Zwróć uwagę, że `errno` jest zerowane w pętli bezpośrednio przed wywołaniem `readdir`, a nie np. raz przed pętlą oraz na to, że w
razie zwrócenia `NULL` przez `readdir`, sterowanie przechodzi jeszcze przez dwa proste warunki, zanim dojdzie do sprawdzenia
`errno` i rozpoznania błędu.

- Dlaczego w ogóle zerujemy `errno`? Czy funkcja `readdir` nie mogłaby robić tego za nas? No właśnie *mogłaby* (dokładnie tak
definiuje to standard), funkcje systemowe mogą zerować `errno` w razie poprawnego wykonania, ale nie muszą.

- Jeśli chcemy w warunkach logicznych w C dokonywać przypisań, to powinniśmy ująć całe przypisanie w nawiasy. Wartość
przypisywana będzie wtedy uznana za wartość wyrażenia w nawiasie. Robimy tak w przypadku wywołania `opendir` oraz `readdir`.

## Katalog roboczy

Program z poprzedniego zadania umożliwiał skanowanie zawartości tylko katalogu, w którym został uruchomiony. 
Dużo lepsza byłaby możliwość wyboru, jaki katalog należałoby zeskanować. Widzimy, że wystarczyłoby w tym celu podmienić
argument funkcji `opendir` na ścieżkę podaną np. w parametrze pozycyjnym. Nie będziemy jednak chcieli modyfikować funkcji `scan_dir`, aby przedstawić sposób na wczytanie i zmianę katalogu roboczego z poziomu kodu programu.

Operacje na katalogu roboczym umożliwiają funkcje `getcwd` i `chdir`, dostępne po dołączeniu pliku nagłówkowego `<unistd.h>` (`man 3p getcwd`, `man 3p chdir`). 
Ich deklaracje, według standardu, są następujące:

```
char *getcwd(char *buf, size_t size);
```
- `buf` jest wcześniej zaalokowaną tablicą znaków, do której zostanie zapisana **bezwzględna** ścieżka do katalogu roboczego.
Tablica ta powinna mieć długość co najmniej `size`,
- funkcja zwraca `buf` w przypadku sukcesu. W razie niepowodzenia, zwracany jest `NULL`, a `errno` ustawiane jest na odpowiednią
wartość.

```
int chdir(const char *path);
```
- `path` jest ścieżką do nowego katalogu roboczego (może być względna lub bezwzględna),
- tak jak wiele funkcji systemowych zwracających `int`, funkcja `chdir` zwraca `0` w przypadku sukcesu i inną wartość w razie niepowodzenia.

### Zadanie

Bazując na funkcji z poprzedniego zadania, napisz program, który będzie zliczał obiekty we wszystkich folderach
podanych jako parametry pozycyjne programu.

### Rozwiązanie zadania

Nowe strony z manuala:
```
man 3p getcwd
man 3p chdir
```

rozwiązanie `l1-2.c`:
{{< includecode "l1-2.c" >}}

### Uwagi i pytania

- Sprawdź, jak program zachowa się w przypadku: 
   - nieistniejących katalogów, 
   - katalogów, co do których nie masz prawa dostępu, 
   - czy poprawnie poradzi sobie ze ścieżkami zarówno względnymi, jak i bezwzględnymi, podawanymi jako parametr.

- Dlaczego program pobiera i zapamiętuje aktualny katalog roboczy?
{{< answer >}}
Jest to rozwiązanie przypadku, w którym użytkownik poda kilka ścieżek względnych jako parametry, np. 
`l1-2 dir1 dir2/dir3`. Program z rozwiązania zmienia katalog roboczy na docelowy przed wywołaniem skanowania. 
Gdybyśmy zatem po sprawdzeniu katalogu nie wracali każdorazowo do katalogu początkowego,
próbowalibyśmy odwiedzić najpierw folder `./dir1/` (to jeszcze poprawne), a następnie `./dir1/dir2/dir3/` zamiast 
przewidywanego `./dir2/dir3/`.
{{< /answer >}}

- Czy prawdziwe jest stwierdzenie, że program powinien "wrócić" do tego katalogu w którym był uruchomiony?
{{< answer >}}
Nie, katalog roboczy to właściwość procesu. Jeśli proces-dziecko zmienia swój CWD, to nie ma to wpływu na proces-rodzic,
zatem nie ma obowiązku ani potrzeby wracać.
{{< /answer >}}

- W tym programie nie wszystkie błędy muszą zakończyć się wyjściem: który można inaczej obsłużyć i jak?
{{< answer >}}
Chodzi o błędy funkcji `chdir`: może się np. zdarzyć sytuacja, w której użytkownik poda nieistniejący katalog.
Najprostsze rozwiązanie to `if(chdir(argv[i])) continue;`, można by jednak dodać jakiś komunikat.
{{< /answer >}}

- Nigdy i pod żadnym pozorem nie pisz `printf(argv[i])`! Jeśli ktoś poda jako katalog `%d` to jak to wyświetli `printf`?
To dotyczy nie tylko argumentów programu, ale dowolnych ciągów znaków.

## Operacje na plikach

Duża część programów wchodzi w interakcję z plikami na dysku. Najprostszym sposobem którym można to zrealizować jest:
1. Otwarcie (stworzenie) za pomocą `fopen` (`man 3p fopen`),
2. Ustawienie kursora pliku z `fseek` (`man 3p fseek`),
3. Wpisanie danych `fprintf`, `fputc`, `fputs`, `fwrite` lub wczytanie ich `fscanf`, `fgetc`, `fgets`, `fread`
4. Powtórzenie kroków 2.-3. w miarę potrzeby,
5. Zamknięcie pliku `fclose` (`man 3p fclose`).

Potrzebne funkcje znajdziemy w nagłówku `<stdio.h>`.
```
FILE *fopen(const char *restrict pathname, const char *restrict mode);
```
- `pathname` oznacza ścieżkę otwieranego pliku,
- `mode` to tryb w którym chcemy go otworzyć. String trybu może wyglądać w następujący sposób, co może dawać różne możliwości manipulacji plikiem:
   - `r` - plik udostępnia czytanie danych,
   - `w` lub `w+` - plik zostaje skrócony do zera (lub stworzony) i udostępnia pisanie danych,
   - `a` lub `a+` - plik udostępnia dopisywanie danych do końca jego istniejącej treści.
   - `r+` - plik udostępnia czytanie oraz pisanie danych.

Do każdego z trybów możemy dodać na koniec `b`, co w standardzie UNIX nic nie zmieni w deskryptorze który otrzymamy. Jest to opcja utrzymywana dla kompatybilności ze standardem C.

Funkcja ta zwraca wskaźnik do wewnętrznej struktury `FILE`, która pozwala na kontrolowanie strumienia powiązanego z otwartym przez nią plikiem. Zgodnie z mądrością komentarza umieszczonego w jednej z implementacji `FILE` `<stdio.h>` przez Pedro A. Aranda Gutiérreza:

>\* Some believe that nobody in their right mind should make use of the\
>\* internals of this structure.

nie będziemy się przyglądać temu co jest wewnątrz. Budowa tej struktury zależy od konkretnej implementacji systemu, więc zwykle traktujemy ją jako typ nieprzejrzysty, nie ustawiamy ani nie odczytujemy z niej bezpośrednio jej pól. Przechowujemy wyłącznie wskaźnik i używamy go poprzez wywoływanie na nim różnych funkcji.

Funkcja `fseek` przyjmuje wskaźnik `FILE` i pozwala nam przesunąć się na odpowiednie miejsce w pliku. Wyjątkiem jest plik otwarty w trybie "a" - append, który niezmiennie wskazuje na koniec treści niezależnie od wywołań `fseek`. Poza tym przypadkiem, tuż po otwarciu, kursor pliku wskazuje na pierwszy bajt.

```
int fseek(FILE *stream, long offset, int whence);
```
- `stream` jest wyżej wymienionym identyfikatorem strumienia pliku,
- `offset` określa liczbę bajtów o którą chcemy się przesunąć,
- `whence` mówi o tym jaki punkt odniesienia powinniśmy przyjąć w momencie przesunięcia. Może przyjmować następujące wartości:
   - SEEK_SET - punktem odniesienia jest początek pliku, funkcja ustawia kursor pliku na `offset`-ym bajcie pliku.
   - SEEK_CUR - przesunięcie relatywne do obecnego kursora pliku, funkcja ustawia kursor o `offset` bajtów do przodu (do tyłu w przypadku wartości ujemnej).
   - SEEK_END - punktem odniesienia jest koniec pliku. Kursor pliku będzie wskazywał na dane tylko jeśli wartość `offset` jest ujemna.
      - Dla wartości `offset` równej `0` kursor pliku ustawiony jest na bajt po ostatnim bajcie pliku. Odczytanie pozycji kursora funkcją `ftell` podaje wtedy dokładny rozmiar pliku w bajtach. Operacja ta pozwala programiście zaalokować dokładną ilość bajtów która będzie potrzebna na wczytanie całego pliku.

Kiedy ustawimy kursor pliku na pożądaną pozycję, możemy zacząć wczytywać dane z pliku lub je do niego zapisywać. Funkcje `fprintf` oraz `fscanf` działają analogicznie do dobrze znanych wszystkim funkcji działających na standardowym wejściu - `printf` oraz `scanf`. Pozostałe funkcje na początku mogą wydawać się mniej użyteczne, chociaż w szczególności `fread` (`man 3p fread`) w praktyce stanowczo przewyższa częstością użycia swojego kuzyna `fscanf`. Własnoręczna implementacja konwersji danych z pliku na wartości o docelowych typach danych daje znacznie większą kontrolę niż implementacje biblioteczne.
```
size_t fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
```
- `ptr` - bufor w który będą zapisywane dane,
- `size` - rozmiar nieprzerwanych elementów do wczytania,
- `nitems` - liczba elementów do wczytania,
- `stream` - wskaźnik pozyskany z `fopen`.

Zwrócona wartość oznacza liczbę elementów wczytanych z sukcesem. Będzie ona mniejsza niż `nitems` w przypadku błędu lub zakończenia pliku. Można pomyśleć że podział wczytanych danych na elementy jest niepotrzebną komplikacją, ale daje to pewną korzyść w przypadku gdy wczytywany plik składa się z pewnych rekordów których nie chcemy przerywać w połowie (np. 4-bajtowe zmienne całkowitoliczbowe `int`). W przypadku niepełnego odczytu nie musimy obliczać ile obiektów wczytaliśmy poprawnie ani cofać kursora pliku aby wczytać niepełny rekord jeszcze raz. W momencie gdy pierwsze wywołanie zwróciło `n` wystarczy wywołać funkcję jeszcze raz z przesuniętym buforem `ptr+size*n` oraz liczbą elementów `nitems-n`.

Należy pamiętać, żeby po zakończeniu działaniu na danym pliku zwolnić używane zasoby przy użyciu funkcji `fclose`.

W przypadku gdy potrzebujemy usunąć plik należy wywołać `unlink` (`man 3p unlink`). Jeżeli jakiś proces (w tym nasz) wciąż otwiera `unlink`-owany przez nas plik, jest on usunięty z systemu plików, lecz istnieje wciąż w pamięci. Jest on ostatecznie usunięty w momencie gdy ostatni używający proces go zamknie.

### Zadanie

Napisać program tworzący nowy plik o podanej parametrami nazwie (-n NAME), uprawnieniach (-p OCTAL ) i rozmiarze (
-s SIZE). Zawartość pliku ma się składać w około 10% z losowych znaków [A-Z], resztę pliku wypełniają zera (znaki o
kodzie zero, nie '0'). Jeśli podany plik już istnieje, należy go skasować.

### Rozwiązanie zadania

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

### Uwagi i pytania

- Jaką maskę bitową tworzy wyrażenie `~perms&0777` ? 
{{< answer >}}
odwrotność wymaganych parametrem -p uprawnień przycięta do 9 bitów, 
jeśli nie rozumiesz jak to działa koniecznie powtórz sobie operacje bitowe w C.
{{< /answer >}}

- Jak działa losowanie znaków ? 
{{< answer >}}
W losowych miejscach wstawia kolejne znaki alfabetu od A do Z potem znowu A itd.
Wyrażenie 'A'+(i%('Z'-'A'+1)) powinno być zrozumiałe, jeśli nie poświęć mu więcej czasu takie losowania będą się jeszcze pojawiać.
{{< /answer >}}

- Uruchom program kilka razy, pliki wynikowe wyświetl poleceniem cat i less sprawdź jakie mają rozmiary (ls -l), czy zawsze równe podanej w parametrach wartości? Z czego wynikają różnice dla małych rozmiarów -s a z czego dla dużych (> 64K) rozmiarów?
{{< answer >}}
Prawie zawsze rozmiary są różne w obu przypadkach wynika to ze sposobu tworzenia pliku,
który jest na początku pusty a potem w losowych lokalizacjach wstawiane są znaki, 
nie zawsze będzie wylosowany znak na ostatniej pozycji. Losowanie podlega limitowi 2-bajtowego RAND_MAX, 
więc w dużych plikach losowane są znaki na pozycjach do granicy RAND_MAX.
{{< /answer >}}

- Przerób program tak, aby rozmiar zawsze był zgodny z założonym.

- Czemu podczas sprawdzania błędu unlink jeden przypadek ignorujemy?
{{< answer >}}
ENOENT oznacza brak pliku, jeśli plik o podanej nazwie nie istniał to nie możemy go skasować,
ale to nie przeszkadza programowi, w tym kontekście to nie jest błąd. 
Bez tego wyjątku moglibyśmy tylko nadpisywać istniejące pliki a nie tworzyć nowe.
{{< /answer >}}

- Zwrócić uwagę na wyłączenie z `main` funkcji do tworzenia pliku, im więcej kodu tym ważniejszy jest jego podział na funkcje. Przy okazji krótko omówmy cechy dobrej funkcji:
   - robi jedną rzecz na raz (krótki kod)
   - możliwie duży stopień generalizacji problemu (dodano procent jako parametr)
   - wszystkie dane wejściowe dostaje przez parametry (nie używamy zmiennych globalnych)
   - wyniki przekazuje przez parametry wskaźnikowe lub wartość zwracaną (w tym przypadku wynikiem jest plik) a nie przez zmienne globalne

- W kodzie używamy specjalnych typów numerycznych `ssize_t`, `mode_t` zamiast int robimy to ze względu na zgodność typów z
prototypami funkcji systemowych.

- Czemu w tym programie używamy umask? Otóż funkcja fopen nie pozwala ustawić uprawnień, a przez umask możemy okroić
uprawnienia jakie są nadawane domyślnie przez fopen, niskopoziomowe open daje nam nad uprawnieniami większą kontrolę.

- Czemu zatem nie możemy dodać uprawnień `x`? Funkcja fopen domyślnie nadaje tylko prawa 0666 a nie pełne 0777, przez
bitowe odejmowanie nijak nam nie może wyjść ta brakująca część 0111.

- Jak zwykle sprawdzamy wszystkie błędy, ale nie sprawdzamy statusu umask, czemu? Otóż umask nie zwraca błędów tylko starą
maskę.

- Zmiana `umask` jest lokalna dla naszego procesu i nie ma wpływu na proces rodzicielski zatem nie musimy jej przywracać.

- Parametr tekstowy -p został zmieniony na oktalne uprawnienia dzięki funkcji strtol, warto znać takie przydatne funkcje
aby potem nie wyważać otwartych drzwi i nie próbować pisać samemu oczywistych konwersji.

- Pytanie czemu kasujemy plik skoro tryb otwarcia `w+` nadpisuje plik? Jeśli plik o danej nazwie istniał to jego uprawnienia
są zachowywane a my przecież musimy nadać nasze, przy okazji jest to pretekst do ćwiczenia kasowania.

- Tryb otwarcia pliku `b` nie ma w systemach POSIX-owych żadnego znaczenia, nie rozróżniamy dostępu na tekstowy i binarny,
jest tylko binarny.

- W programie nie wypełniamy pliku zerami, dzieje się to automatycznie ponieważ gdy zapisujemy coś poza aktualnym końcem
pliku system automatycznie dopełnia lukę zerami. Co więcej, jeśli tych zer ciągiem jest sporo to nie zajmują one
sektorów dysku!

- Jeśli wykonamy unlink na pliku już otwartym i używanym w innym programie to plik zniknie z filesystemu ale nadal
zainteresowane procesy będą mogły z niego korzystać. Gdy skończą plik zniknie na dobre.

- Najlepiej w procesie wywołać srand dokładnie jeden raz z unikalnym ziarnem,w tym programie wystarczy czas podany w
sekundach.

## Inne operacje na systemie plików

### Tworzenie katalogu

Do tworzenia nowych katalogów służy funkcja `mkdir` (`man 3p mkdir`):

```
int mkdir(const char *path, mode_t mode);
```

Jak widać jest dość prosta w użyciu - podajemy ścieżkę do katalogu oraz uprawnienia - jeśli będą poprawne funkcja stworzy pusty katalog.
Należy pamiętać, że w przypadku katalogów niezbędne są uprawnienia do wykonania (`x`), które w tym wypadku oznaczają możliwość przejście przez katalog i dostęp do plików i katalogów wewnątrz niego.

### Uprawnienia

Przy użyciu wspomnianych wcześniej funkcji `stat` i `lstat` możemy sprawdzać uprawnienia pliku.
Funkcja `chmod` (`man 3p chmod`) pozwala nam je ustawiać. Sygnatura jest taka sama jak `mkdir` wyżej:

```
int mkdir(const char *path, mode_t mode);
```

Należy pamiętać, że żeby zmienić uprawnienia nasz proces musi mieć uprawnienia do zapisu danego pliku.
Wygodnym sposobem, żeby sprawdzić, czy plik istnieje i mamy odpowiednie uprawnienia jest funkcja `access` (`man 3p access`).

```
int access(const char *path, int amode);
```
Należy uważać, bo parametr `amode` nie jest tym samym co `mode` w `chmod` i `mkdir`.
Przyjmuje jedną z czterech flag lub ich kombinację (przez `|`) w zależności od tego, co chcemy sprawdzić: `F_OK` (istnienie), `R_OK` (odczyt), `W_OK` (zapis), `X_OK` (wykonanie).

Istnieje również funkcja `chown` pozwalająca zmieniać właściciela pliku, nie będziemy się jednak nią zajmować, gdyż jej użycie wymaga uprawnień superusera.

### Dowiązania

W systemach POSIX istnieją dwa rodzaje dowiązań - twarde (ang. _hardlink_) oraz symboliczne (ang. _symlink_).

Dowiązanie twarde jest referencją do danego inoda - tym samym z punktu widzenia API jest po prostu inną nazwą tego samego pliku.
Każde jest równoważne i nie ma między nimi różnicy - po prostu niektóre pliki mają tylko jedną nazwę w systemie plików a inne kilka
Do tworzenie dowiązań twardych służy funkcja `link` (`man 3p link`).

Dowiązania twarde są bardzo użyteczne ale równocześnie niebezpieczne.
Takie dowiązanie może np. mieć inne uprawnienia albo znajdować się w publicznym katalogu dając niepowołanym osobom dostęp do pliku.
Ogólnie rzecz biorąc na laboratorium nie będziemy ich używać.

Dowiązania symboliczne są tak naprawdę specjalnym rodzajem pliku, zawierającym ścieżkę do innego pliku.
Działają więc w innej warstwie niż dowiązania twarde - są od nich bezpieczniejsze ale jednocześnie mają ograniczenia.
Np. jeżeli plik na który wskazuje dowiązanie symboliczne zmieni nazwę takie dowiązanie przestanie być poprawne - plik na który wskazuje przestaje istnieć.
W przypadku dowiązań twardych wszystkie one są równorzędne. W przypadku dowiązań symbolicznych są one tylko wskaźnikami odrębnymi od właściwego pliku.

Do tworzenia dowiązań twardych służy funkcja `symlink` (`man 3p symlink`).

```
int symlink(const char *path1, const char *path2);
```

Jej wywołanie tworzy dowiązanie symboliczne o ścieżce `path2` do pliku o nazwie `path1` (czyli `path2->path1`, `->` oznacz ,,wskazuje na'').
W terminalu mamy bezpośredni odpowiednik tej funkcji `ln -s <TARGET> <PATH>` (`man 1 ln`).
Możemy użyć `ln` żeby w prosty sposób poeksperymentować z symlinkami.
Warto, ponieważ potrafią one być mylące.

Rozważ następujący przypadek:

w naszym katalogu roboczym mamy dwa podkatalogi `a` oraz `b`.
W katalogu `a` tworzymy plik `file.txt`.
Chcemy teraz stworzyć dowiązanie symboliczne do tego pliku w katalogu `b` o nazwie np. `file.txt_bak`
Wywołujemy więc `ln -s a/file.txt b/file.txt_bak`.
Czy to zadziała poprawnie?

{{< answer >}} 
Nie, nie zadziała!
Zostanie stworzone dowiązanie `b/file.txt_bak` jednak będzie ono wskazywać na względną ścieżkę `a/file.txt`, która z punktu widzenia pliku `b/file.txt_bak` jest niepoprawna, bo dla niego ścieżka do tego pliku to `../a/file.txt`.

Żeby poprawnie stworzyć dowiązanie z naszego katalogu roboczego musimy wywołać `ln -s ../a/file.txt b/file.txt_bak` - wygląda nieintuicyjnie na pierwszy rzut oka, ale po prostu trzeba pamiętać, że ścieżka docelowa jest zawsze z punktu widzenia dowiązania. Alternatywnie możemy wejść do katalogu `b` i stamtąd wywołać `ln -s ../a/file.txt file.txt_bak`. Aby wygodnie podejrzeć na jaki plik wskazuje dowiązania w danym katalogu najlepiej użyć `ls -l`.
{{</ answer >}}

Ponieważ dowiązania symboliczne to jedynie ścieżki wskazujące na inne pliki istotne jest, czy będzie ona bezwzględna czy względna.
W zależności od tego dowiązanie może być poprawnie lub nie po zmianie nazwy lub położenia pliku lub dowiązania (ćwiczenie: przeanalizuj jak zmiana położenia pliku albo symlinku wpływa na jego poprawność gdy ścieżki są względne albo bezwględne. Rozważ przypadek, gdy plik i jego symlink poruszają się razem, bo cały katalog w którym się znajdują jest przenoszony)

Jeśli nadal nie czujesz się pewnie z dowiązaniami symbolicznymi koniecznie je przećwicz w terminalu używając takich komend jak `ls -l`, `ln -s`, `cd`, `touch`, `echo`, `cat`.
Mają one swoje bezpośrednie odpowiedniki w C, wspomniane wyżej.

## Buforowanie standardowego wyjścia

### Eksperyment

<em>kod do pliku <b>prog13.c</b></em>
{{< includecode "prog13.c" >}}

- Spróbuj uruchomić ten (bardzo prosty!) kod z terminala. Co widać na terminalu? 
{{< answer >}} 
To czego się spodziewaliśmy: co sekundę pokazuje się liczba.
{{</ answer >}}

- Spróbuj uruchomić kod ponownie, tym razem jednak przekierowując wyjście do pliku `./plik_wykonwyalny >
plik_z_wyjściem`. Następnie spróbuj otworzyć plik z wyjściem w trakcie działania programu, a potem zakończyć
działanie programu przez Ctrl+C i otworzyć plik jeszcze raz. Co widać tym razem? 
{{< answer >}} Jeśli zrobimy te kroki wystarczająco szybko, plik okazuje się być pusty! To zjawisko wynika z
tego, że biblioteka standardowa wykrywa, że dane nie trafiają bezpośrednio do terminala, i dla wydajności buforuje je, 
zapisując je do pliku dopiero gdy zbierze się ich wystarczająco dużo. To oznacza, że dane nie są dostępne od razu,
a w razie nietypowego zakończenia programu (tak jak kiedy użyliśmy Ctrl+C) mogą wręcz zostać stracone. 
Oczywiście, jeśli damy programowi dojść do końca działania, to wszystkie dane zostaną zapisane do pliku (proszę spróbować!). Mechanizm buforowania można skonfigurować,
ale nie musimy tego robić, jak za chwilę zobaczymy. 
{{</ answer >}}

- Spróbuj uruchomić kod podobnie, ponownie pozwalając wyjściu trafić do terminala (jak za pierwszym razem), ale spróbuj usunąć nową linię z argumentu `printf`: `printf("%d", i);`. Co widzimy tym razem?
{{< answer >}}
Wbrew temu co powiedzieliśmy wcześniej, nie widać wyjścia mimo to, że tym razem dane trafiają bezpośrednio do terminala;
dzieje się natomiast to samo co w poprzednim kroku. Otóż biblioteka buforuje standardowe wyjście nawet jeśli dane
trafiają do terminala; jedyną różnicą jest to, że reaguje na znak nowej linii, wypisując wszystkie dane zebrane w
buforze. To ten mechanizm sprawił, że w pierwszym kroku nie wydarzyło się nic dziwnego. Właśnie dlatego czasami zdarza
się Państwu, że `printf` nie wypisuje nic na ekran; jeśli zapomnimy o znaku nowej linii, standardowa
biblioteka nic nie wypisze na ekran dopóki w innym wypisywanym stringu nie pojawi się taki znak, lub program się nie
zakończy poprawnie.
{{</ answer >}}

- Spróbuj ponownie zrobić poprzednie trzy kroki, tym razem jednak wypisując dane do strumienia standardowego błędu: `fprintf(stderr, /* parametry wcześniej przekazywane do printf */);`. Co dzieje się tym razem? Żeby przekierować standardowy błąd do pliku, należy użyć `>2` zamiast `>`. 
{{< answer >}}
Tym razem nic się nie buforuje i zgodnie z oczekiwaniami widzimy jedną cyfrę co sekundę. Standardowa biblioteka nie
buforuje standardowego błędu, bowiem często wykorzystuje się go do debugowania. 
{{</ answer >}}

- Często możemy chcieć użyć `printf(...)` do debugowania, dodając wywołania tej funkcji w celu sprawdzenia
wartości zmiennych bądź czy wywołanie dochodzi do jakiegoś miejsca w naszym kodzie. W takich przypadkach należy zamiast
tej funkcji użyć `fprintf(stderr, ...)` i wypisywać do standardowego błędu. W przeciwnym przypadku może się
okazać, że nasze dane zostaną zbuforowane i zostaną wypisane później niż się spodziewamy, a w skrajnych przypadkach
wcale. Jeśli nie wiemy, do którego strumienia wypisywać, należy preferować standardowy błąd.
Przy pisaniu prawdziwych aplikacji konsolowych strumienia standardowego wyjścia używa się wyłącznie do wypisywania
rezultatów, a do czegokolwiek innego używa się standardowego błędu. Na przykład `grep` wypisze znalezione
wystąpienia na standardowe wyjście, ale ewentualne błędy przy otwarciu pliku trafią na standardowy błąd. Nawet nasze
makro `ERR` wypisuje błąd do strumienia standardowego błędu.

## Przykładowe zadania

Wykonaj przykładowe zadania. Podczas laboratorium będziesz miał więcej czasu oraz dostępny startowy kod, jeśli jednak wykonasz poniższe zadania w przewidzianym czasie, to znaczy że jesteś dobrze przygotowany do zajęć.

- [Zadanie 1]({{< ref "/sop1/lab/l1/example1" >}}) ~75 minut
- [Zadanie 2]({{< ref "/sop1/lab/l1/example2" >}}) ~75 minut
- [Zadanie 3]({{< ref "/sop1/lab/l1/example3" >}}) ~120 minut
- [Zadanie 4]({{< ref "/sop1/lab/l1/example4" >}}) ~130 minut


## Kody źródłowe z treści tutoriala
{{% codeattachments %}}
