---
title: "L1 - System plików"
date: 2022-02-05T18:39:22+01:00
weight: 20
---

# Tutorial 1 - System plików

{{< hint info >}}
Ten tutorial zawiera wyjaśnienia działania funkcji wymaganych na laboratoriach oraz ich parametrów.
Jest to jednak wciąż zbiór jedynie poglądowy najważniejszych informacji -- 
należy **koniecznie przeczytać wskazane strony manuala**, aby dobrze poznać i zrozumieć wszystkie szczegóły.

{{< /hint >}}


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
Ich definicje są nastepujące:

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

Jedyną różnicą w działaniu funkcji `stat` i `lstat` jest obługa linków. `stat` zwróci informacje o pliku, do 
którego dany link prowadzi, natomiast `lstat` zwróci informacje o samym linku. 

Struktura `stat` zawiera m.in. informacje o rozmiarze pliku, właścielu, czy dacie ostatniej modyfikacji. Dostępne
są też makra sprawdzające typ pliku. Poniżej znajdują się ważniejsze przykłady takich makr:
- Makra przyjmujące `buf->st_mode` (pole typu `mode_t`):
   - `S_ISREG(m)` -- czy mamy do czynienia ze zwykłym plikiem,
   - `S_ISDIR(m)` -- czy mamy do czynienia z katalogiem,
   - `S_ISLNK(m)` -- czy mamy do czynienia z linkiem.
- Istnieją też makra `S_TYPE*(buf)` przyjmujące sam wskaźnik `buf`, służące identyfikacji typów plików takich, jak
semafory czy pamięć dzielona (więcej o tym będzie w przyszłym semestrze).
Szczegóły znajdują się w manualu `man sys_stat.h`. Warto się zapoznać ze wszystkimi atrybutami struktury `stat` 
i makrami, jest tego dość dużo.

Po przejrzeniu katalogu, należy (będąc dobrym programistą i chcąc zdać przedmiot) pamiętać o zwolnieniu zasobów 
za pomocą funkcji `closedir`.

### Informacje techniczne

W celu przejrzenia całego katalogu, funkcję `readdir` należy wywoływać tyle razy, aż nie zwróci `NULL`.
W przypadku wystąpienia błędu, zarówno `opendir`, jak i `readdir` zwracają `NULL`. Wynika z tego ważny wniosek
w przypadku funkcji `readdir`: przed jej wywołaniem należy wyzerować zmienną `errno`, a w razie zwrócenia `NULL`
sprawdzić, czy ta zmienna nie została ustawiona na niezerową wartość (oznaczającą błąd).
`errno` jest zmienną globalną używana przez funkcje systemowe do wskazania kodu napotkanego błędu.

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
Nie, są dwa katalogi, program policzył katalogi `.` i  `..`. Każdy katalog ma *hardlinka* na samego siebie (`.`) i katalog nadrzędny (`..`). 
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

## Przeglądanie katalogów i podkatalogów (rekursywne)

Gdyby zaszła potrzeba odwiedzenia nie tylko danego katalogu, ale całego poddrzewa katalogów, rozwiązanie bazujące na funkcji
`opendir` byłoby dość kłopotliwe. Są za to dostępne funkcje `ftw` i `nftw` obecne w pliku nagłówkowym `<ftw.h>`, które przechodzą
całe drzewo katalogów, startując z podanego katalogu, i wywołują na każdym z odwiedzonych katalogów i plików pewną funkcję.
Opisana zostanie tutaj tylko funkcja `nftw`, ponieważ `ftw` jest oznaczona jako przestarzała i nie powinna być używana. Deklaracja
funkcji `nftw` jest następująca:

```
int nftw(const char *path, int (*fn)(const char *, const struct stat *, int, struct FTW *), int fd_limit, int flags);
```
- `path` oznacza ścieżkę do katalogu, od którego zacznie się przeszukanie,
- `fn` to **wskaźnik na funkcję** przyjmującą cztery argumenty:
   - pierwszy: typu `const char*`, w którym znajdzie się ścieżka do rozważanego pliku/katalogu,
   - drugi: typu `const struct stat*`, zawierający wskaźnik na strukturę `stat`, która została omówiona we wcześniejszej części tutoriala,
   - trzeci: typu `int`, zawierający dodatkową informację. Może ona przyjąć jedną z ustalonych wartości (patrz `man 3p nftw`), z czego ważniejsze
   to: 
      - `FTW_D`: odwiedzono katalog,
      - `FTW_F`: odwiedzono plik,
      - `FTW_SL`: odwiedzono link,
      - `FTW_DNR`: odwiedzono katalog, którego nie można było otworzyć.
   - czwarty: typu `struct FTW *`, zawierający wskaźnik na strukturę, której pole `level` informuje, jak głęboko aktualnie jesteśmy 
   w drzewie przeszukania, a pole `base` zawiera indeks znaku w ścieżce (obecnej w pierwszym argumencie), 
   który rozpoczyna właściwą nazwę pliku, np. dla ścieżki `/usr/bin/cat` tą wartością byłoby `9`. 
Funkcja ta jest wywoływana dla każdego odwiedzonego pliku i katalogu, można ją traktować jako pewnego rodzaju callback.
W funkcji `fn` powinniśmy zwykle zwrócić `0`, jeśli zwrócimy coś innego, `nftw` natychmiast zakończy działanie i zwróci też tę wartość
(to można także wykorzystać jako sygnalizację błędu).
- `fd_limit` oznacza maksymalną głębokość przeszukania drzewa,
- `flags` oznacza flagi modyfikujące działanie funkcji, z czego ciekawsze to:
   - `FTW_CHDIR`: zmienia katalog roboczy na aktualnie przeglądany katalog w trakcie wykonywania funkcji,
   - `FTW_DEPTH`: przeszukanie wgłąb (domyślnie `nftw` przeszukuje wszerz),
   - `FTW_PHYS`: jeśli obecna, odwiedzane będą linki same w sobie, domyślnie odwiedzane są pliki, do których link prowadzi.
Flagi te można łączyć ze sobą operatorem logicznym `|`.

Manual (`man 3p nftw`) zawiera bardziej szczegółowe informacje i wszystkie możliwe wartości, jakie mogą być przekazane
lub napotkane w trakcie wykonywania `nftw`.

### Zadanie

Napisz program zliczający wystąpienia plików, katalogów, linków i innych typów dla całych poddrzew zaczynających
się w podanych jako parametry folderach.

### Rozwiązanie zadania

Nowe strony z manuala:
```
man 3p ftw
man 3p nftw
```

rozwiązanie `l1-3.c`:
{{< includecode "l1-3.c" >}}

### Uwagi i pytania

- Jeśli definicja funkcji `nftw` lub użycie `walk` w rozwiązaniu są dla Ciebie niezrozumiałe, 
powtórz koniecznie, co to są i jak działają wskaźniki na funkcje w C.

- Sprawdź, jak program sobie radzi z niedostępnymi i nieistniejącymi katalogami.

- W jakim celu użyta jest flaga `FTW_PHYS`?
{{< answer >}} 
Bez tej flagi, `nftw` przechodzi przez linki symboliczne do wskazywanych obiektów, czyli nie może ich zliczać, 
analogicznie jak `stat`. 
{{< /answer >}}

- Sprawdź, jak inne flagi modyfikują zachowanie `nftw`.

- Deklaracja `_XOPEN_SOURCE` jest na Linuksie niezbędna, inaczej nie widzi deklaracji funkcji `nftw` (ważna jest kolejność,
deklaracja przed `include`). Funkcję `ftw` oznaczono już jako przestarzałą i nie powinno się jej używać.

- Zmienne globalne to "zło wcielone", zbyt łatwo ich użyć, a przychodzi za to zapłacić przy analizowaniu cudzego kodu lub
podczas przenoszenia funkcji z jednego projektu do drugiego. Tworzą one niejawne zależności w kodzie. Tutaj (niestety) musimy
ich użyć, ponieważ funkcja callback `nftw` nie pozwala nic przekazać na zewnątrz inaczej, niż przez zmienną globalną. To jest
wyjątkowa sytuacja, używanie zmiennych globalnych, poza wskazanymi koniecznymi przypadkami, jest na laboratoriach zabronione!

- Bardzo przydatna jest możliwość nałożenia limitu otwieranych przez `nftw` deskryptorów, co prawda może to uniemożliwić
przeskanowanie bardzo głębokiego drzewa katalogów (głębszego niż limit), ale pozwala to nam zarządzać zasobami, które
mamy. W zakresie deskryptorów, maksima systemowe pod Linuksem są nieokreślone, ale można oddzielnie limitować procesy na
poziomie administracji systemem.

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
nie zawsze będzie wylosowany znak na ostatniej pozycji. Losowanie podlega limitowi 2 bajtowego RAND_MAX, 
wiec w dużych plikach losowane są znaki na pozycjach do granicy RAND_MAX.
{{< /answer >}}

- Przerób program tak, aby rozmiar zawsze był zgodny z założonym.

- Czemu podczas sprawdzania błędu unlink jeden przypadek ignorujemy?
{{< answer >}}
ENOENT oznacza brak pliku, jeśli plik o podanej nazwie nie istniał to nie możemy go skasować,
ale to nie przeszkadza programowi, w tym kontekście to nie jest błąd. 
Bez tego wyjątku moglibyśmy tylko nadpisywać istniejące pliki a nie tworzyć nowe.
{{< /answer >}}

- Zwrócić uwagę na wyłączenie z `main` funkcji do tworzenia pliku, im więcej kodu tym ważniejszy podział na użyteczne funkcję. Przy okazji krótko omówmy cechy dobrej funkcji:
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

## Operacje niskopoziomowe na plikach

Do realizacji odczytu i zapisu plików można użyć też funkcji niskopoziomowych, t.j. takich, których nie definiuje biblioteka standardowa C, a które udostępnia sam system operacyjny. Można przy ich pomocy np. wysyłać pakiety przez sieć, czym zajmiemy się w przyszłym semestrze.

Zamiast wskaźników na struktury `FILE` funkcje niskopoziomowe pracują na *file descriptors* (fd) - wartościach całkowitoliczbowych identyfikujących zasoby w obrębie procesu. W tym przypadku będą to pliki, ale w ogólności mogą odnosić się do różnego typu zasobów systemowych. Aby użyć poniższych funkcji konieczne jest dołączenie nagłówków `<fcntl.h>` i `<unistd.h>`.

```
int open(const char *path, int oflag, ...);
```
Zobacz `man 3p open`.
- `path` - ścieżka otwieranego pliku,
- `oflag` - flagi otwarcia pliku połączone operacją bitowego OR `|`, analogiczne do trybu otwarcia funkcji `fopen`, ale też precyzujące zachowanie w różnych warunkach brzegowych. Odnieś się do `man 3p open` dla listy flag.

Funkcja zwraca deskryptor pliku `fd`, którego używać będziemy w kolejnych funkcjach.

Funkcje niskopoziomowe nie korzystają z buforowania. `read` (`man 3p read`) dostaje znaki natychmiastowo kiedy są dostępne. Oznacza to, że nie zawsze wczyta tyle znaków ile oczekujemy. Z jednej strony dostajemy dane najszybciej jak się da nie musząc czekać aż system załaduje resztę z dysku. Z drugiej jednak musimy uważać żeby faktycznie wczytać całość danych które chcemy.

```
ssize_t read(int fildes, void *buf, size_t nbyte);
```
- `filedes` - deskryptor pliku, pozyskany z `open`,
- `buf` - wskaźnik na bufor w którym zapisane będą dane,
- `nbyte` - rozmiar danych których ma oczekiwać funkcja.

Funkcja zwraca ilość bajtów które udało się wczytać.

W analogiczny sposób działa funkcja `write` (`man 3p write`):

```
ssize_t write(int fildes, const void *buf, size_t nbyte);
```
- `filedes` - deskryptor pliku, pozyskany z `open`,
- `buf` - wskaźnik na bufor z którego pobierane będą dane,
- `nbyte` - rozmiar danych przeznaczonych do wysłania.

Zwrócona wartość oznacza ilość zapisanych bajtów.


### Zadanie

Napisz prosty program kopiujący pliki.
Powinien akceptować jako swoje argumenty dwie ścieżki i skopiować plik z pierwszej na drugą. 

Tym razem użyj funkcji niskopoziomowych.

### Rozwiązanie zadania

Co student musi wiedzieć: 
- man 3p open
- man 3p close
- man 3p read
- man 3p write
- man 3p mknod (tylko stałe opisujące uprawnienia do open)
- opis makra TEMP_FAILURE_RETRY <a href="http://www.gnu.org/software/libc/manual/html_node/Interrupted-Primitives.html">tutaj</a>

<em>kod do pliku <b>prog14.c</b></em>
{{< includecode "prog14.c" >}}

### Uwagi i pytania

Aby dostępne było makro `TEMP_FAILURE_RETRY` trzeba najpierw zdefiniować `GNU_SOURCE` a następnie dołączyć plik
nagłówkowy `unistd.h`. Nie musisz jeszcze w pełni rozumieć działania tego makra, będzie on ważniejsze w trakcie kolejnego laboratorium gdy zajmiemy się sygnałami.

- Dlaczego w powyższym programie używane są funkcje `bulk_read` i `bulk_write`?
Czy nie wystarczyłoby po prostu użyć `read` i `write`
{{< answer >}}
Zgodnie ze specyfikacją funkcje `read` i `write` mogą zwrócić zanim ilość danych której zażądał użytkownik zostanie odczytana/zapisana.
Więcej o tym zachowaniu dowiesz się w tutorialu do kolejnego laboratorium.
Teoretycznie w tym zadaniu nie ma to znaczenia (ponieważ nie używamy sygnałów), ale dobrze się do tego przyzwyczaić już teraz.
{{< /answer >}}

- Czy powyższy program mógłby być zaimplementowany funkcjami bibliotecznymi z C zamiast niskopoziomowym IO? (`fopen`, `fprintf`, ...)
{{< answer >}}
Tak, w tym programie nie ma niczego co nie pozwala użyć wcześniej pokazanych funkcji.
{{< /answer >}}

- Czy do deskryptora zwróconego z `open` można zapisać dane przez `fprintf`?
{{< answer >}}
Nie! Funkcje `fprintf`, `fgets`, `fscanf` itd. przyjmują jako argument zmienną typu `FILE*`, deskryptor jest natomiast pojedynczą liczbą `int` używaną przez system operacyjny do identyfikacji otwartego pliku.
{{< /answer >}}

## Operacje wektorowe na plikach

Funkcja `writev` (`man 3p writev`) oferuje wygodne rozwiązanie w przypadku gdy dane które chcemy zapisać nie znajdują się w jednym ciągłym fragmencie pamięci. Pozwala ona zebrać dane z wielu miejsc i zapisać je w pliku za pomocą jednego wywołania funkcji. Znajduje się w nagłówku `<sys/uio.h>`.
```
ssize_t writev(int fildes, const struct iovec *iov, int iovcnt);
```
- `filedes` - deskryptor do którego zapisywane są dane,
- `iov` - tablica struktur opisujących bufory z których funkcja zbiera dane - `struct iovec` (`man 0p sys_uio.h`), które mają następujące pola: 
```
void   *iov_base -> wskaźnik na obszar pamięci
size_t  iov_len  -> długość obszaru pamięci
```
- `iovcnt` - rozmiar tablicy `iov`.

Funkcja ta zapisuje do `filedes`:\
`iov[0].iov_len` bajtów zaczynając od `iov[0].iov_base`, następnie\
`iov[1].iov_len` bajtów zaczynając od `iov[1].iov_base`, ... aż do\
`iov[iovcnt-1].iov_len` bajtów zaczynając od `iov[iovcnt-1].iov_base`.

W analogiczny sposób działa funkcja `readv` (`man 3p readv`):

```
ssize_t readv(int fildes, const struct iovec *iov, int iovcnt);
```

- `filedes` - deskryptor z którego czytane są dane,
- `iov` - tablica struktur `struct iovec` opisujących bufory do których funkcja rozprasza dane.
- `iovcnt` - rozmiar tablicy `iov`.

W pozostałych aspektach funkcje te zachowują się jak ich niewektorowe odpowiedniki `write` i `read`.

### Przydatne strony

- man 3p writev
- man 3p readv
- man 0p sys_uio.h

## Przykładowe zadania

Wykonaj przykładowe zadania. Podczas laboratorium będziesz miał więcej czasu oraz dostępny startowy kod, jeśli jednak wykonasz poniższe zadania w przewidzianym czasie, to znaczy że jesteś dobrze przygotowany do zajęć.

- [Zadanie 1]({{< ref "/sop1/lab/l1/example1" >}}) ~75 minut
- [Zadanie 2]({{< ref "/sop1/lab/l1/example2" >}}) ~75 minut
- [Zadanie 3]({{< ref "/sop1/lab/l1/example3" >}}) ~120 minut

## Kody źródłowe z treści tutoriala
{{% codeattachments %}}
