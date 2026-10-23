---
title: "L2 - Niskopoziomowy operacje na plikach i poll"
date: 2022-02-05T18:39:22+01:00
weight: 30
---

# Tutorial 1 - System plików


## Operacje niskopoziomowe na plikach

Do realizacji odczytu i zapisu plików można użyć też funkcji niskopoziomowych, t.j. takich, których nie definiuje biblioteka standardowa C, a które udostępnia sam system operacyjny. Można przy ich pomocy np. wysyłać pakiety przez sieć, czym zajmiemy się w przyszłym semestrze.

Zamiast wskaźników na struktury `FILE` funkcje niskopoziomowe pracują na deskryptorach (ang. *file descriptor* - fd - wartościach całkowitoliczbowych identyfikujących zasoby w obrębie procesu. W tym przypadku będą to pliki, ale w ogólności mogą odnosić się do różnego typu zasobów systemowych. Aby użyć poniższych funkcji konieczne jest dołączenie nagłówków `<fcntl.h>` i `<unistd.h>`.

```
int open(const char *path, int oflag, ...);
```
Zobacz `man 3p open`.
- `path` - ścieżka otwieranego pliku,
- `oflag` - flagi otwarcia pliku połączone operacją bitowego OR `|`, analogiczne do trybu otwarcia funkcji `fopen`, ale też precyzujące zachowanie w różnych warunkach brzegowych. Odnieś się do `man 3p open` dla listy flag. Ważne flagi na laboratorium to:
    - tryb, np. `O_RDONLY`, `O_WRONLY`, `O_RDWR`
    - `O_CREAT` - tworzy plik, jeżeli nie istnieje
    - `O_EXCL` - używane zawsze z `O_CREAT` - tworzy plik jeśli nie istnieje, zwraca błąd gdy plik już istnieje
    - `O_APPEND` - dopisywanie danych na koniec pliku
    - `O_TRUNC` - przycięcie pliku do 0 przy otwarciu
    - `O_NONBLOCK` - otwarcie w trybie nieblokującym - `read` i `write` nie będą czekać, tylko od razu zwrócą tyle danych, ile akurat jest dostępne 
- `...` - trzeci argument może być potrzebny gdy tworzymy plik - podajemy tam wtedy uprawnienia.


Funkcja zwraca deskryptor pliku `fd`, którego używać będziemy w kolejnych funkcjach.
Deskryptor jest zasobem procesu, dlatego po użyciu należy go zwolnić funkcją `close` (`man 3p close`).

Funkcje niskopoziomowe nie korzystają z buforowania. `read` (`man 3p read`) dostaje znaki natychmiastowo kiedy są dostępne.
Oznacza to, że nie zawsze wczyta tyle znaków ile oczekujemy.
Z jednej strony dostajemy dane najszybciej jak się da nie musząc czekać aż system załaduje resztę z dysku.
Z drugiej jednak musimy uważać żeby faktycznie wczytać całość danych które chcemy.

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

Powyższe funkcje odczytują (albo zapisują) dane po kolei.
Czasami jednak interesuje potrzebujemy dostępu do danych np. gdzieś w środku pliku.
Moglibyśmy oczywiście przeczytać dostatecznie dużo bajtów, byłoby to jednak bardzo nieoptymalne.
Funkcja `lseek` (`man 3p lseek`) pozwala na bezpośrednie poruszanie kursorem

```
off_t lseek(int fildes, off_t offset, int whence);
```

Jako parametr `whence` możemy podać `SEEK_SET` (ustawienie kursora na daną pozycję), `SEEK_CUR` (przesunięcie kursora względem aktualnej pozycji) albo `SEEK_END` (przesunięcie kursora za koniec pliku).
Jako, że `lseek` zwraca pozycję kursora po wykonaniu operacji, wywołanie go z parametrami `0` i `SEEK_CUR` pozwala uzyskać aktualną pozycję w pliku.

Przeglądając manuale mogłeś zauważyć istnienie wielu odpowiedników już poznanych funkcji ale z końcówką `-at` -np. `fstatat`, `openat`, `symlink`.
Zwykle te funkcją biorą dodatkowy parametr - deskryptor pliku, względem którego będą wyznaczane ścieżki względne.
Są więc często miłym ułatwieniem dla programisty, pozwalając pominąć konstruowanie długich ścieżek względnych albo zmienianie katalogu roboczego.


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

<em>kod do pliku <b>l2-1.c</b></em>
{{< includecode "l2-1.c" >}}

### Uwagi i pytania

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


## Operacje blokujące, nieblokujące i fcntl

Przy opisie funkcji `open` wspomniana była flaga `O_NONBLOCK` pozwalająca otworzyć plik w trybie *nieblokującym*.
Domyślnie pliki (warto pamiętać, że pracujemy w systemach POSIX, gdzie ,,wszystko jest plikiem'') otwierane są w trybie blokującym.
Oznacza to, że wywołanie `read` albo `write` na takim deskryptorze będzie czekać tak długo, aż dane będą dostępne do odczytu/zapisu.
Przykładem takiego zachowania jest terminal i funkcje takiej jak `scanf` czy `getchar`.
Gdy je wywołamy wykonanie programu jest zatrzymywane tak długo, aż nie podamy jakiś danych na terminal.
Gdybyśmy jednak przełączyli `stdin` w tryb nieblokujący takie funkcje kończyłyby się od razu - rozważ poniższy przykład:

{{< includecode "l2-2.c" >}}

Skompiluj i przetestuj powyższy program.
W pierwszej, blokującej części, zarówno `scanf` jak i `read` w nieskończoność czeka aż wpiszemy dane (i enter - domyślnie terminal nie wysyła danych dopóki nie zatwierdzi się ich enterem).
Po zmianie deskryptora na nieblokujące `read` zaczyna zachowywać się inaczej - po prostu zwraca aktualnie dostępne dane, nawet jeżeli nic nie ma.
Widać też tutaj dobrze, że nie zawsze zwrócenie przez funkcję błędu (-1 w tym wypadku) wymaga od razu zakończenia programu.
W przypadku nieblokującego `read` jest to wręcz oczekiwane - sprawdzamy errno i dowiadujemy się, że przyczyną jest po prostu brak danych (`EAGAIN`, `EWOULDBLOCK`).
Dopiero gdy błąd jest inny wiemy, że coś poszło nie tak.

Zwróć również uwagę na funkcję `fcntl` (`man 3p fcntl`).
Pozwala ona sprawdzać oraz zmieniać właściwości deskryptora (flagi) po jego otwarciu.

```
int fcntl(int fildes, int cmd, ...);
```

Zarówno `cmd` jak i opcjonalny trzeci argument może przyjmować bardzo dużo parametrów.
Dla na istotne są teraz komendy `F_GETFD` and `F_SETFD`.
Podanie `F_GETFD` sprawia, że funkcja zwraca aktualnie ustawione flagi jako maskę bitową zapisaną w zmiennej typu int.
Z kolei użycie `F_SETFD` wymaga podania maski definiującej flagi, jakie chcemy ustawić.
Dlatego zwykle proces ustawiania flag deskryptora wygląda jak w funkcji `set_nonblocking()` w powyższym kodzie.
Najpierw pobieramy aktualne flagi, ustawiamy te pola, które nas interesują, a następnie zapisujemy.
Próba ustawienia flag poprzez wywołanie np. `fcntl(fd, F_SETFL, O_NONBLOCK)` jest błędem, ponieważ w ten sposób przy okazji resetujemy wszystko poza flagą `O_NONBLOCK`.

Czasami potrzebujemy żeby nasz plik, zachowywał się w różny sposób zależnie od kontekstu - np. chcemy raz czytać blokująca a raz nieblokująco.
W takiej sytuacji zmiana flag co chwilę byłaby nieoptymalna.
Zamiast tego możemy użyć funkcji `dup()` żeby zduplikować deskryptor.
Oba deskryptory wskazują wtedy na ten sam plik, ale mogą mieć ustawione inne flagi.


## Poll

### Problem multipleksowania IO

Jak można zauważyć w przykładzie wyżej nieblokujące IO jest niewygodne w użyciu, jeżeli czekamy na pojawienie się danych.
Musimy wtedy periodycznie sprawdzać czy dane są już dostępne - jest to tak zwany *busy waiting* albo *busy polling*.
Jeżeli, czekając na dane, napiszemy pętle, która w kółko będzie sprawdzać czy dane są potrzebne zajmiemy w pełni cały rdzeń procesora - proszę sobie wyobrazić, że każdy otwarty terminal działałby w taki sposób - komputer zupełnie nie nadawałby się do użytku.
Z drugiej strony możemy wprowadzić przerwy w sprawdzaniu - tak jak w przykładzie powyżej funkcją `sleep`.
Obciążenie systemu spada, ale w zamian za to rośnie opóźnienie w działaniu programu.
Warto dodać, że na laboratorium używanie busy waitingu jest niedopuszczalne, chyba że zadanie wprost na to pozwala.

Jak widać blokujące IO wydaje się być idealnym rozwiązaniem powyższego problemu.
Nasz program zostaje wstrzymany i nie zużywa zasobów, a gdy dane są dostępne i może działać dalej system sam go wybudza.
Zauważmy jednak podstawowy problem - możemy w ten sposób obsługiwać tylko jeden deskryptor na raz.
Dotąd nie stanowiło to problemu, bo znaliśmy tylko `stdin` (zaraz poznamy kilka innych przykładów), jednak możemy sobie wyobrazić, że mamy teraz kilka deskryptorów, które zachowują się jak `stdin` i na których czekamy na dane.
Wydaje się, że jedynym rozwiązaniem jest używanie nieblokującego IO i robienie busy waitingu - ma to jednak duże wady, jak to zostało opisane wyżej.

Aby rozwiązać powyższe problemy i umożliwić responsywną obsługę wielu deskryptorów systemy operacyjne wprowadzają tzw. multipleksowanie IO (ang. _IO multiplexing_).
Mamy kilka funkcji zapewniające ten mechanizm - `select`, `poll` (POSIX), `epoll` (linux).
Poniżej opiszemy działanie funkcji `poll`.
W materiałach dodatkowych możesz znaleźć opis działania funkcji `epoll`.
Należy ona do rozszerzeń wprowadzonych przez linuxa (nie jest częścią standardu POSIX) i jest bardzo zoptymalizowana pod wydajność na tym systemie - nie będziemy jej jednak wymagać na laboratorium.

### Poll


TODO: opisać pokrótce jak działa `poll`, dodać info o tym jak w linuxie działa input (<https://www.kernel.org/doc/html/latest/input/input.html> dla ciekawskich można napisać coś więcej), może dodać opis `timerfd` żeby było coś jeszcze prostego na czym można działać z tym pollem.

{{< includecode "l2-3.c" >}}


## Mmap

Jeszcze jeden, bardzo wygodny sposób na dostęp do pliku to ,,zmapowanie'' go do pamięci.
Służy do tego funkcja `mmap`.
Jest ona szeroko używana również do wielu innych rzeczy, które omówimy w przyszłości.
Na chwilę obecną skupimy się jedynie na mapowaniu plików.
Wygląda to zwykle w następujący sposób:

{{< includecode "l2-4.c" >}}

Najważniejszą funkcją jest tutaj oczywiście sam `mmap`:

```
char *buff = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
```

W ten sposób prosimy system operacyjny o ,,zmapowanie'' pliku o deskryptorze `fd` do pamięci o rozmiarze `length` tak, żebyśmy mogli go zarówno czytać jak i modyfikować (`PROT_READ | PROT_WRITE`).
Wynikiem jest wskaźnik na zmapowaną pamięć lub wartość `MAP_FAILED` gdy wystąpi błąd.
O pozostałych parametrach i zastosowaniach tej funkcji możesz poczytać w manualu (`man 3p mmap`, `man 2 mmap` dla linuxowych rozszerzeń), jednak nie będziemy ich wymagać.
Na razie interesuje nas tylko, że, jak widać w powyższym kodzie, `mmap` pozwala nam bardzo wygodnie modyfikować pliki, tak samo jak zwykłe tablice.
Dwie funkcje pomocnicze to `msync` i `munmap`.
`msync` wymusza synchronizację zmian na dysk - system operacyjny nie robi tego zawsze od razu, ponieważ zapisy na dysk są wolne.
`munmap` to likwidacja mapowania, czyli sprzątanie.

Ważną uwagą jest, że deskryptor musi mieć odpowiednie uprawnienia.
Gdybyśmy do `mmap` podali `PROT_READ | PROT_WRITE` oraz deskryptor który otworzyliśmy np. z flagą `O_RDONLY` funkcja zwróciłaby błąd.

## Kody źródłowe z treści tutoriala
{{% codeattachments %}}
