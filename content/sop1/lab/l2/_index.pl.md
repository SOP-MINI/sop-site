---
title: "L2 - Niskopoziomowy operacje na plikach i poll"
date: 2022-02-05T18:39:22+01:00
weight: 30
---

# Tutorial 1 - System plików


## Operacje niskopoziomowe na plikach

Do realizacji odczytu i zapisu plików można użyć też funkcji niskopoziomowych, t.j. takich, których nie definiuje biblioteka standardowa C, a które udostępnia sam system operacyjny. Można przy ich pomocy np. wysyłać pakiety przez sieć, czym zajmiemy się w przyszłym semestrze.

Zamiast wskaźników na struktury `FILE` funkcje niskopoziomowe pracują na deskryptorach (ang. *file descriptor* - fd) - wartościach całkowitoliczbowych identyfikujących zasoby w obrębie procesu. W tym przypadku będą to pliki, ale w ogólności mogą odnosić się do różnego typu zasobów systemowych. Aby użyć poniższych funkcji konieczne jest dołączenie nagłówków `<fcntl.h>` i `<unistd.h>`.

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

Przeglądając manuale mogłeś zauważyć istnienie wielu odpowiedników już poznanych funkcji ale z końcówką `-at` -np. `fstatat`, `openat`, `symlinkat`.
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
Dla nas istotne są teraz komendy `F_GETFD` and `F_SETFD`.
Podanie `F_GETFD` sprawia, że funkcja zwraca aktualnie ustawione flagi jako maskę bitową zapisaną w zmiennej typu int.
Z kolei użycie `F_SETFD` wymaga podania maski definiującej flagi, jakie chcemy ustawić.
Dlatego zwykle proces ustawiania flag deskryptora wygląda jak w funkcji `set_nonblocking()` w powyższym kodzie.
Najpierw pobieramy aktualne flagi, ustawiamy te pola, które nas interesują, a następnie zapisujemy.
Próba ustawienia flag poprzez wywołanie np. `fcntl(fd, F_SETFL, O_NONBLOCK)` jest błędem, ponieważ w ten sposób przy okazji resetujemy wszystko poza flagą `O_NONBLOCK`.

Czasami potrzebujemy żeby nasz plik, zachowywał się w różny sposób zależnie od kontekstu - np. chcemy raz czytać blokująco a raz nieblokująco.
W takiej sytuacji zmiana flag co chwilę byłaby nieoptymalna.
Zamiast tego możemy użyć funkcji `dup()` żeby zduplikować deskryptor.
Oba deskryptory wskazują wtedy na ten sam plik, ale mogą mieć ustawione inne flagi.


## Multipleksowanie IO

Jak można zauważyć w przykładzie wyżej nieblokujące IO jest niewygodne w użyciu, jeżeli czekamy na pojawienie się danych.
Musimy wtedy periodycznie sprawdzać czy dane są już dostępne - jest to tak zwany *busy waiting* albo *busy polling*.
Jeżeli, czekając na dane, napiszemy pętle, która w kółko będzie sprawdzać czy dane są dostępne zajmiemy w pełni cały rdzeń procesora - proszę sobie wyobrazić, że każdy otwarty terminal działałby w taki sposób - komputer zupełnie nie nadawałby się do użytku.
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
Należy ona do rozszerzeń wprowadzonych przez linuxa (nie jest częścią standardu POSIX) i jest bardzo zoptymalizowana na tym systemie - nie będziemy jej jednak wymagać na laboratorium.

### Poll

Zacznijmy od przyjrzenia się sygnaturze funkcji `poll` (`man 3p poll`):

```
int poll(struct pollfd fds[], nfds_t nfds, int timeout);
```

`fds` to tablica struktur z desktryptorami, na które będziemy czekać - zaraz przyjrzymy się jej dokładniej.
`nfds` to oczywiście rozmiar tej tablicy.
`poll` zakończy się gdy któryś z deskryptorów będzie gotowy do wykonania operacji albo osiągnięty zostanie `timeout` w milisekundach - możemy też podać tam wartość `-1` żeby czekać w nieskończoność.

Struktura `pollfd` jest opisana w `man 0p poll.h`.
Musi definiować przynajmniej następujące pola:

- `int fd` - deskryptor na który będziemy czekać
- `short events` - bitmaska w której zapisujemy wydarzenia (_events_) na które chcemy czekać. Dla na istotne jest przede wszystkim `POLLIN` czyli oczekiwanie aż deskryptor będzie miał gotowe do odczytu dane, oraz `POLLOUT` - gdy deskryptor będzie gotowy do zapisu.
- `short revents` - od _return events_ - w tej bitmasce `poll` ustawi flagi wskazujące, co było przyczyną jego zakończenia - może to być przede wszystkim to, co ustawiliśmy w `events` (`POLLIN` lub `POLLOUT`), ale także wartości takie jak `POLLERR`, `POLLHUP` lub `POLLNVAL` oznaczające, że wystąpił błąd lub problem z deskryptorem - szczegóły sprawdź w manualu.

Użycie funkcji `poll` polega więc na stworzeniu tablicy `fds` i wpisanie tam deskryptorów oraz wydarzeń na które będziemy czekać.
Na przykład chcąc czekać aż jakieś użytkownik wpisze jakieś dane na standardowe wejście ustawimy `.fd` na `1` lub `STDIN_FILENO` a `.events` na `POLLIN`.
`revents` będzie ustawiany przez samego `poll`-a, dla bezpieczeństwa najlepiej go wyzerować.


W poniższym przykładzie próbujemy odczytać informacje z plików `/dev/input/event*` - w systemie linux odpowiadają one za przekazywanie danych z klawiatur, myszy i podobnych urządzeń wejściowych.
Aktywacja urządzenia wejściowego, np. naciśnięcie klawisza na klawiaturze, powoduje ,,dopisanie'' do tych plików nowej struktury `input_event` zdefiniowanej w nagłówku `linux/input.h`.
Więcej informacji nt. działania podsystemu urządzeń wejściowych w linuxie można znaleźć pod adresem <https://www.kernel.org/doc/html/latest/input/input.html>.
Nagłówek `linux/input-event-codes.h` definiuje wartości ze struktury `input_event` - np. klawisz F odpowiada wartości 33.
Nie musisz się uczyć tych rzeczy na pamięć, nie będą one wymagane na laboratorium - podajemy ja jako ciekawostkę i interesujący przykład.

{{< includecode "l2-3.c" >}}

Po uruchomieniu za każdym razem jak naciśniesz klawisz na klawiaturze, poruszysz myszką etc. powyższy program będzie wypisywał przychodzące wydarzenia wejściowe.
W przypadku gdy program nie zadziała z powodu niewystarczających uprawnień sprawdź, czy możesz czytać pliki w `/dev/input` - może być konieczne przynależenie do grupy `input`.
W ostateczności można odpalić program z `sudo`.

Kiedy program się kończy?
{{< details "Answer" >}}
Program kończy się po tym, jak `poll` zwróci 30 razy.
Nie jest to równoważne z otrzymaniem 30 eventów.
Za jednym razem możemy odczytać zdarzenie z kilku urządzeń, a każde urządzenie może mieć zakolejkowane kilka zdarzeń.
{{< /details >}}


Czemu otworzyliśmy pliki w trybie nieblokującym?
Czy program działałby z deskryptorami w trybie blokującym?
{{< details "Answer" >}}
Należy uważać z używaniem blokującego IO i multipleksingu.
Zauważmy, że `poll` informuje nas tylko, że jakieś dane są dostępne, jednak nie wiemy jak dużo.
Przy nieblokującym deskryptorze możemy po prostu czytać do skutku - co w istocie robimy.
Gdyby deskryptory były w trybie blokującym program nigdy nie wyszedłby z pętli w linii 98.
Moglibyśmy skasować tę pętlę i czytać tylko jedną wiadomość na raz - gdyby było ich kilka, to po prostu `poll` nam od razu to zwróci i odczytamy kolejne.
Należy jednak z tym uważać.
W tym wypadku mamy gwarancję, że dane mają rozmiar struktury `input_event`.
Gdybyśmy jednak tego nie wiedzieli musielibyśmy czytać po jednym bajcie.
Jak widać dużo lepiej jest tutaj używać nieblokującego IO, blokujące nie ma żadnych zalet a stwarza ryzyko zablokowania programu.
{{< /details >}}

Czy możemy użyć `poll`-a do czytania ze zwykłych plików?
{{< details "Answer" >}}
Możemy ale nie ma to za dużo sensu.
Zwykłe pliki są zawsze gotowe do odczytu, `poll` zawsze od razu to zwróci.
{{< /details >}}

Powstaje pytanie, do jakich (sensownych) rodzajów plików możemy jeszcze użyć `poll`-a.
Z poznanych dotąd (na SOPach oraz ,,Podstawach systemu UNIX'') są to przede wszystkim urządzenia znakowe - należą do nich terminale (`/dev/tty`, ale również po prostu standardowe wejście i wyjście naszych programów), pokazane wyżej urządzenia `/dev/input/*` oraz wiele innych plików w `/dev`.
Ponadto łącza (pipe/FIFO), gniazda sieciowe i wiele innych deskryptorów zwracanych przez różne funkcje systemowe (`timerfd`, `signalfd`, `inotify`...).

### Timerfd (dodatkowy materiał)

Prostym i bardzo przydatnym system na którym możemy testować funkcję `poll` jest `timerfd`.
Należy on do linuxowych rozszerzeń (nie jest częścią standardu POSIX) dlatego nie będziemy go wymagać na laboratorium.
Jest to jednak przydatny i ciekawy mechanizm, dlatego warto przeczytać resztę tego podrozdziału.

`timerfd` pozwala nam tworzyć zegary które następnie odczytujemy przez deskryptor.
Pełen opis znajduje się w `man 2 timerfd`.

Aby stworzyć nowy timer używamy funkcji 

```
int timerfd_create(int clockid, int flags);
```

`clockid` to rodzaj zegara systemowego, którego użyjemy - najprościej wybrać `CLOCK_REALTIME`, ale są też inne opcje, których opis znajdziesz w manualu.
Z flag jakie można ustawić interesujące jest dla nas przede wszystkim `TFD_NONBLOCK`.

Gdy już nasz timer jest stworzony należy go ustawić poleceniem:

```
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value,
                         struct itimerspec *_Nullable old_value);
```

Podajemy tu oczywiście nasz deskryptor uzyskany z `timerfd_create`.
Przez `new_value` przekazujemy na jaki czas chcemy ustawić zegar.
Struktura ma ta dwa pola (`man 3type itimerspec`).
Ustawiając `it_value` wybieramy czas do pierwszego powiadomienia zegara.
Ustawiając `it_interval` możemy dodatkowo ustawić powiadomienia okresowe.
Oba pola są typu `timespec` (`man 3type timespec`) to znaczy że specyfikują sekundy i nanosekundy.
Wiele przydatnych funkcji używa struktury `timespec`, dlatego warto się z nią zapoznać.
Parametr `old_value` jest opcjonalny, możemy ustawic go na `NULL` - więcej informacji przeczytaj o nim w manualu.


{{< includecode "l2-4.c" >}}

Działanie powyższego programu jest chyba jasne.
Żeby potwierdzić, że timer działa dobrze możesz uruchomić program przez polecenie `time`, np. `time ./l2-4`.

Zauważ, że w tym wypadku nie ustawiliśmy naszego timera w trybie nieblokującym - celem tutaj był jak najprostszy przykład demonstrujący działanie tej funkcjonalności, więc używamy pojedynczego `read`.
Jako ćwiczenie spróbuj zmodyfikować poprzedni program z `poll`-em w taki sposób, żeby program kończył się po określonym czasie (albo może po określonym czasie nieaktywności? spróbuj paru rzeczy!).

### Epoll (dodatkowy materiał)

## Mmap

Jeszcze jeden, bardzo wygodny sposób na dostęp do pliku to ,,zmapowanie'' go do pamięci.
Służy do tego funkcja `mmap`.
Jest ona szeroko używana również do wielu innych rzeczy, które omówimy w przyszłości.
Na chwilę obecną skupimy się jedynie na mapowaniu plików.
Wygląda to zwykle w następujący sposób:

{{< includecode "l2-5.c" >}}

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
