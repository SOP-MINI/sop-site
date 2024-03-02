---
title: "L1 - FIFO/pipe"
date: 2022-02-10T10:00:00+01:00
weight: 10
---

# Tutorial 1 - FIFO/pipe

{{< hint info >}}
Uwagi wstępne:
- Obowiązują wszystkie materiały z SOP1!
- Szybkie przejrzenie tutoriala prawdopodobnie nic nie pomoże, należy samodzielnie uruchomić programy, sprawdzić jak działają, poczytać materiały dodatkowe takie jak strony man. W trakcie czytania sugeruję wykonywać ćwiczenia a na koniec przykładowe zadanie.
- Na żółtych polach podaję dodatkowe informacje, niebieskie zawierają pytania i ćwiczenia. Pod pytaniami znajdują się odpowiedzi, które staną się widoczne dopiero po kliknięciu. Proszę najpierw spróbować sobie odpowiedzieć na pytanie samemu a dopiero potem sprawdzać odpowiedź.
- Pełne kody do zajęć znajdują się w załącznikach na dole strony. W tekście są tylko te linie kodu, które są konieczne do zrozumienia problemu.
- Materiały i ćwiczenia są ułożone w pewną logiczną całość, czasem do wykonania ćwiczenia konieczny jest stan osiągnięty poprzednim ćwiczeniem dlatego zalecam wykonywanie ćwiczeń w miarę przyswajania materiału.
- Większość ćwiczeń wymaga użycia konsoli poleceń, zazwyczaj zakładam, ze pracujemy w jednym i tym samym katalogu roboczym więc wszystkie potrzebne pliki są \"pod ręką\" tzn. nie ma potrzeby podawania ścieżek dostępu.
- Czasem podaję znak `$` aby podkreślić, że chodzi o polecenie konsolowe, nie piszemy go jednak w konsoli np.: piszę `$make` w konsoli wpisujemy samo `make`.
- To co ćwiczymy wróci podczas kolejnych zajęć. Jeśli po zajęciach i teście coś nadal pozostaje niejasne proszę to poćwiczyć a jeśli trzeba dopytać się u prowadzących.
- W tym materiale do synchronizacji procesów używamy tylko FIFO/pipe, jeśli moglibyśmy dodać semafor (lub inne metody IPC) to niektóre ograniczenia można łatwo obejść (np. wiele procesów czytających z wspólnego FIFO/pipe), w ten sposób jednak łatwiej w pełni zrozumieć opisywane mechanizmy. Często też dodawanie zewnętrznej synchronizacji jest zbędne i wynika tylko z niewiedzy.
- Przypominam, że FIFO i pipe są jednokierunkowymi kanałami komunikacji. W przypadku FIFO jest to dość łatwe do zapamiętania bo o kierunku decydujemy podczas jego otwierania. W przypadku pipe mamy parę deskryptorów i tu już trzeba widzieć że indeks\[0\] to wyłącznie odczyt, \[1\] wyłącznie zapis. Zapamiętanie może ułatwić analogia z stdin/out ( 0 stdin, 1 stdout).
- W przypadku gdy z jednego łącza korzysta wiele procesów piszących i jeden czytający podczas *jednego zapisu* danych do pipe/FIFO mamy gwarancję ciągłości danych wpisanych przez jeden proces jeśli ilość tych danych nie przekracza stałej PIPE\_BUF. Planując komunikację od wielu procesów do jednego nie można przekraczać rozmiaru PIPE\_BUF dla pojedynczego zapisu.
- Analogiczna sytuacja z odczytem (wiele procesów czytających) nie jest już w żaden sposób atomowa, nie ma gwarancji, ile danych można odczytać w sposób ciągły zanim inny proces przejmie kontrolę nad odczytem.
- Zerwanie łącza (czy to na skutek jego zamknięcia przez wszystkie procesy na jednym końcu komunikacji czy też na skutek \"zabicia\" tych procesów) objawi się powstaniem sytuacji EOF (End od File) podczas odczytu lub otrzymaniem sygnału SIGPIPE podczas zapisu. Domyślną reakcją na SIGPIPE jest zabicie procesu. Jeśli sygnał ten będzie blokowany/ignorowany lub obsługiwany przez własną funkcję to zapis do łącza zwróci błąd EPIPE.
- Warto zapamiętać, że identyczne zasady zerwania połączenia obowiązują dla gniazd sieciowych.
- Pipe jest od razu połączony, do FIFO trzeba się podłączyć. Jeśli nie użyjemy flagi O\_NONBLOCK to podłączanie będzie blokować wykonanie programu/wątku aż do momentu gdy druga strona połączenia będzie dostępna. Użycie flagi O\_NONBLOCK wyłączy blokowanie ale do momentu pełnego nawiązania połączenia nie mamy użytecznego łącza i musimy się liczyć z błędami komunikacji przy zapisie ENXIO oraz z EOF podczas odczytu.
- `PIPE_BUF` na Linuxie to 4kb
- W POSIXie `PIPE_BUF` to co najmniej 512 bajtów.
{{< /hint >}}

## Pytania kontrolne

1. Czy w sytuacji połączenia jednego procesu piszącego z jednym czytającym też trzeba przesyłać dane w paczkach nie przekraczających `PIPE_BUF`?  
{{< answer >}} Nie ma takiej konieczności, nie występuje w tym, wariancie problem konkurowania o możliwość zapisu. {{< /answer >}}
2. Czy można otworzyć łącze pipe lub FIFO w obrębie jednego procesu/wątku?  
{{< answer >}}Można, choć to bardzo rzadki przypadek. {{< /answer >}}
3. O jakim zagrożeniu związanym z powyższym typem połączenia trzeba szczególnie pamiętać?  
{{< answer >}} O zakleszczeniu, łącze ma wewnętrzny bufor o wymiarze `PIPE_BUF`, próba wpisania tam większej ilości danych skończy się zablokowaniem procesu/wątku, który nie może w tym samym czasie czytać tych danych i zwalniać bufora. Można sobie znacząco ułatwić zadanie używając deskryptorów w trybie `O_NONBLOCK`. {{< /answer >}}
4. Czy wysyłając kilka porcji danych bezpośrednio po sobie (kilka wywołań write) o sumarycznej wielkości poniżej PIPE\_BUF nadal mamy gwarancję ciągłości zapisu?  
{{< answer >}} NIE! ciągłość jest gwarantowana tylko dla POJEDYNCZEGO zapisu. {{< /answer >}}
5. Jeśli procesy piszące (wiele) wysyłają dane w porcjach mniejszych od `PIPE_BUF` ale o różnych rozmiarach to czy atomowość zapisu wystarczy aby te dane prawidłowo odebrać?  
{{< answer >}} NIE, proces czytający musi wiedzieć ile danych ma czytać. Jeśli dane będą wysyłane w tym samym czasie to nie ma możliwości aby proces czytający wiedział gdzie kończy się jedna porcja a gdzie zaczyna druga. Można na początku danych podać ilość aktualnie przesyłaną aby proces czytający wiedział ile jeszcze doczytać z łącza lub można zawsze przesyłać pakiety o wymiarze `PIPE_BUF` a brakujące dane uzupełniać np. zerami. {{< /answer >}}
6. Kiedy może zostać przerwany zapis `2*PIPE_BUF` danych do łącza? Skąd wiadomo o przerwaniu? Chodzi o dowolną metodę przerwania ciągłości zapisu, zarówno przez obsługę sygnału jak i przerwanie procesu zapisu przez scheduler CPU oraz przez potencjalne zerwanie łącza.  
{{< answer >}} Przed wysłaniem jakichkolwiek danych, wtedy wywołanie write zwraca błąd EINTR lub po przesłaniu PIPE\_BUF, wtedy write zwraca ilość przesłanych danych \< `2*PIPE_BUF`. {{< /answer >}}
7. Zakładając intensywną obsługę sygnałów w programie, ile razy może zostać przerwane powyżej omawiane wywołanie?  
{{< answer >}} Dowolnie dużo razy zanim zacznie się wykonywać i dowolnie dużo razy po zapisaniu `PIPE_BUF` bajtów zanim zacznie się zapis kolejnego bufora `PIPE_BUF`. {{< /answer >}}
8. W jakich okolicznościach może zostać przerwane wysyłanie `PIPE_BUF` danych?  
{{< answer >}} W przypadku występowania obsługi sygnałów w programie, tylko zanim rozpocznie się transfer (błąd EINTR) {{< /answer >}}
9. Co oznacza odczyt zera bajtów z łącza?  
{{< answer >}} EOF czyli zerwanie połączenia. {{< /answer >}}
10. Jeśli program bazujący na fifo/pipe kończy się nagle komunikatem o przerwaniu działania sygnałem to o czym zapomniałeś?  
{{< answer >}} O prawidłowej obsłudze zerwania łącza w przypadku zapisu, trzeba dodać ignorowanie sygnału `SIGPIPE` i sprawdzać write pod kątem błędu `EPIPE`. {{< /answer >}}
11. Czy EPIPE jest możliwy podczas odczytu z łącza?  
{{< answer >}} Nie. SIGPIPE/EPIPE dotyczy wyłącznie zapisu! {{< /answer >}}
12. Czy zapis do łącza (write) może zwrócić zero?  
{{< answer >}}Zakładając, że nie próbujesz zapisać zera bajtów to nie. EOF dotyczy wyłącznie odczytu!{{< /answer >}}


## Zadanie 1: FIFO

Cel:

Napisz aplikację typu klient-serwer, komunikacja pomiędzy klientami a serwerem odbywa się za pomocą pojedynczego,
współdzielonego łącza FIFO. Serwer odbiera dane w porcjach, usuwa z nich wszystkie nie alfanumeryczne znaki. Dane
wynikowe są wypisywane na ekranie wraz z informacją o PID procesu od jakiego pochodzą. Dane klienta do przesłania
serwerowi pochodzą z pliku wskazanego jako parametr programu klienta.

Aplikacja klienta kończy się jak tylko prześle cały plik, aplikacja serwera kończy się gdy nie będzie żadnego klienta
połączonego z FIFO serwera. Oba programy mają poprawnie reagować na zerwanie łącza!

Zadanie podzielimy na etapy.

Co student musi wiedzieć:

- `man 7 fifo`
- `man 7 pipe`
- `man 3p mkfifo`
- `man 3 isalpha`
- `man 0p limits.h`

Makefile wspólny dla wszystkich zadań w tym tutorialu:

```makefile
CC=gcc
CFLAGS= -std=gnu99 -Wall
```

### Etap 1

1. Przygotować uproszczony serwer, który utworzy FIFO i wszystko, co z niego przeczyta pozbawi znaków nie
   alfanumerycznych, a następnie wypisze na ekran
2. Użyć polecenia `cat` jako klienta

Rozwiązanie **prog21a\_s.c**:

{{< includecode "prog21a_s.c"  >}}

Uruchamianie: `./prog21a_s a & sleep 1; cat prog21a_s.c > a`

{{< hint warning >}}
Proszę pamiętać o obowiązku sprawdzania błędów funkcji systemowych (open,close,read itd.) to m.in. różni dobry kod od
złego.
{{< /hint >}}

1. Czemu służy sekundowa przerwa pomiędzy uruchomieniem serwera a polecenie cat w wywołaniu programu?  
{{< answer >}} Daje czas na utworzenie fifo, inaczej może się zdarzyć, że szybciej uruchomi się polecenie cat i utworzy plik \"a\", wtedy nasz program nie będzie mógł utworzyć fifo o tej nazwie i zwróci błąd. Problemu nie zaobserwujemy jeśli w katalogu roboczym jest już fifo o nazwie \"a\", więc jeśli chcesz wymusić takie zachowanie programu upewnij się, że nie ma \"a\" w katalogu. {{< /answer >}}
2. Jaki jest typ pliku \"a\" jak to sprawdzić?  
{{< answer >}} \$ls -l - typ ten to fifo \"p\" {{< /answer >}}
3. Czemu `EEXIST` nie jest traktowane jako błąd krytyczny po wykonaniu `mkfifo`?  
{{< answer >}}Wcześniej utworzone fifo też się nadaje na potrzeby programu zwłaszcza, że nie kasujemy fifo w kodzie.{{< /answer >}}
4. Czy czytanie z fifo po jednym znaku nie ogranicza wydajności?  
{{< answer >}} Nieznacznie spowalnia dodając wywołanie funkcji read do odczytu z bufora jądra. Nie ma absolutnej konieczności kopiowania tego bufora jeśli przetwarzanie ma odbywać się znak po znaku. {{< /answer >}}
5. Czy pisanie znak po znaku nie ogranicza wydajności?  
{{< answer >}} W tym wypadku piszemy do buforowanego strumienia więc nie ma tu znacznego opóźnienia, ale już użycie write (czyli bez bufora) do zapisu pojedynczego znaku byłoby bardzo dużym ograniczeniem wydajności. {{< /answer >}}
6. Skąd wiadomo, że nie ma i nie będzie już więcej danych w łączu?  
{{< answer >}} EOF - zerwanie łącza wykryte podczas odczytu, gdy program/y piszący zakończą działanie i opróżniony będzie bufor łącza. {{< /answer >}}

### Etap 2

1.  Przygotować kompletny program klienta, który czyta plik i wysyła go w porcjach `PIPE_BUF` do fifo.
2.  Wszystkie wysyłane bufory muszą mieć rozmiar `PIPE_BUF`, także ten ostatni.
3.  Każdy bufor musi być oznaczony numerem PID.

Rozwiązanie **prog21\_c.c**:

{{< includecode "prog21_c.c"  >}}

Uruchamianie: `./prog21a_s a & ./prog21_c a prog21a_s.c`

Proszę zwrócić uwagę jak PID jest przekazany binarnie w buforze. Dzięki takiemu kodowaniu oszczędzamy czas na konwersji z formy binarnej na tekstową i odwrotnie a dodatkowo wiemy dokładnie ile bajtów zajmie PID (`sizeof(pid_t)`), jak widać technicznie wymaga to tylko rzutowania wskaźników i sprytnego przesunięcia początku treści w buforze. Co więcej możemy w ten sposób zakodować większą ilość danych binarnych używając struktury zamiast typu prostego. Jeśli odbiorca skompilował program tak samo jak nadawca (chodzi o ew. pakowanie struktury) to nie trzeba robić konwersji, ani myśleć o długościach składników struktury.

Tym razem plik łącza otwieramy do zapisu a nie do odczytu jak to miało miejsce na serwerze, proszę pamiętać o jednym kierunku przepływu danych w pipe/fifo.

1. Czemu tym razem nie ma wywołania sleep podczas uruchamiania?  
{{< answer >}} Nieważne kto utworzy łącze, oraz w jakiej kolejności będą się programy do niego podłączać, druga strona zawsze poczeka na nawiązanie łączności. Zwróć uwagę, że program klient też może utworzyć łącze (mkfifo) oraz na brak flagi `O_NONBLOCK` która zmieniłaby sposób nawiązania połączenia. {{< /answer >}}
2. Czemu stały rozmiar w tej komunikacji jest ważny?  
{{< answer >}} Bo tak najłatwiej serwer będzie wiedział ile bajtów czytać aby nie pomieszać danych od różnych klientów. {{< /answer >}}
3. Czy można wysyłać bufor partiami, np oddzielnie wysłać zera uzupełniające ostatni bufor?  
{{< answer >}} Nie bo się pomiesza z danymi z innych procesów. {{< /answer >}}
4. Czemu służy memset?  
{{< answer >}} Uzupełnienie ostatniego bufora do pełnego rozmiaru `PIPE_BUF` aby zachować stałe długości komunikatów, zera są ignorowane przez program serwera w sposób naturalny (zero jest terminatorem stringu). {{< /answer >}}
5. Jak ten program zareaguje na zerwanie łącza?  
{{< answer >}} Zabije go sygnał `SIGPIPE`. {{< /answer >}}

### Etap 3

1. Dodać podział na bloki po stronie serwera
2. Dodać kasowanie FIFO

Rozwiązanie **prog21b\_s.c**:

{{< includecode "prog21b_s.c" >}}

Uruchamianie: `./prog21_c a Makefile & ./prog21_c a prog21b_s.c & ./prog21_c a prog21_c.c & sleep 1; ./prog21b_s a`

1.  Zwróć uwagę na unlink na końcu programu - usuwa FIFO z systemu plików tak samo jak usuwa zwykły plik.
2.  Zwróć uwagę jak rozkodowano PID - analogicznie do jego kodowania.

1.  Czemu ponownie pojawia się sleep w wywołaniu?  
{{< answer >}} Bez tego \"sleep\" może się zdarzyć, że jeden klient oraz serwer szybciej się uruchomią od reszty klientów, klient dokona transferu a serwer go przetworzy po czym oba programy się skończą. Pozostali klienci połączą się z fifo ale serwera który mógłby ich obsłużyć już nie będzie. Ci klienci będą czekać na połączenie aż ponownie uruchomimy serwer. {{< /answer >}}
2.  Skąd wiadomo ile danych pochodzi od jednego klienta?  
{{< answer >}} Ustalono stały rozmiar wiadomości równy `PIPE_BUF`. {{< /answer >}}
3.  Czy można przesyłać bloki większe niż `PIPE_BUF` bajtów?  
{{< answer >}} Nie ze względu brak gwarancji ciągłości zapisu w fifo. {{< /answer >}}

## Zadanie 2: pipe

Cel:

Napisać wieloprocesowy program w którym n procesów potomnych komunikuje się z procesem rodzica poprzez współdzielony
pipe R, a rodzic komunikuje się z procesami potomnymi poprzez indywidualne łącza pipe P1,P2,\...,Pn

W reakcji na C-c proces rodzic losuje pipe do którego wysyła losowy znak z przedziału \[a-z\], w reakcji na ten sam
sygnał procesy potomne kończą się z 20% prawdopodobieństwem. Proces potomny który dostanie na swoim pipe znak wysyła na
pipe R bufor tych samych znaków o wymiarze losowych \[1,200\] bajtów.

Proces rodzic wypisuje znaki otrzymane z pipe R na bieżąco. Gdy skończą się wszystkie procesy potomne proces rodzić
również ma się skończyć

Rozwiązanie dzielimy na 2 etapy

1.  Proszę zwrócić uwagę na zerwanie łącza i prawidłową reakcję na nie - zamykamy jeden pipe a nie cały program.
2.  200 bajtów to maksymalny rozmiar przesyłu, czy będzie atomowy? Na Linuksie wiemy że tak, gdy nie ma pewności (program przenośny między platformami) można rozmiar uzależnić od wymiaru PIPE\_BUF lub dodać warunek sprawdzający w programie w stylu \"jeśli rozmiar \> PIPE\_BUF to rozmiar=PIPE\_BUF\".

Co student musi wiedzieć:

- `man 3p pipe`

### Etap 1

1. Tworzymy tyle procesów potomnych ile trzeba (parametr programu)
2. Tworzymy łącza do komunikacji
3. Zamykamy nieużywane deskryptory
4. Inicjujemy generatory liczb losowych
5. Proces rodzic czeka na dane na pipe R, wypisuje je na ekran, jak się skończą kończy działanie
6. Procesy potomne wypisują losową literę do pipe R i się kończą

Rozwiązanie **prog22a.c**:

{{< includecode "prog22a.c" >}}

1.  Ważne jest, aby w programie zamykać nieużywane deskryptory, w tym programie jest ich naprawdę sporo, upewnij się, że rozumiesz które deskryptory są niepotrzebne.
2.  Podobnie jak deskryptory, nieużywana pamięć na stercie powinna być zwolniona, upewnij się że rozumiesz które bloki i kiedy należy zwolnić w procesie potomnym.
3.  Metoda losowania znaków z przedziału \[a,z\] powinna być absolutnie oczywista, jeśli nie jest to rozpisz to sobie na kartce i poćwicz dla różnych przedziałów liter i liczb
4.  Czasem podczas działania programu (najlepiej ustawić n=10) pojawia się błąd: \"Interrupted system call\" , czemu?  
{{< answer >}}  Wywołanie funkcji obsługi SIGCHLD przerywa read zanim coś uda się przeczytać. {{< /answer >}}
5.  Jak się przed tym błędem bronić?  
{{< answer >}}  Dodać restart nałatwiej w postaci makra `TEMP_FAILURE_RETRY(read(...))`. {{< /answer >}}
6.  Jak program reaguje na zerwanie łącza R ?  
{{< answer >}}  Jest to naturalny koniec głównej pętli, kończy się proces rodzica gdyż zerwanie następuje dopiero po odłączeniu się ostatniego potomka. {{< /answer >}}
7.  Czemu nie ma w tym programie wywołania wait/waitpid na końcu procesu rodzica?  
{{< answer >}}  Wszystkie procesy potomne muszą się zakończyć zanim osiągnięty będzie koniec procesu rodzica, bez tego nie byłoby zerwania łącza. Wszystkie zombi są \"łapane\" przez obsługę SIGCHLD.  {{< /answer >}}

#### Etap 2

1.  Dodajemy obsługę sygnału `SIGINT`
2.  Dodajemy zabezpieczenia przed przerwaniem funkcji systemowych przez sygnał (cały czas działania programu)
3.  Dodajemy brakującą część zadania

Rozwiązanie **prog22b.c**:

{{< includecode "prog22b.c"  >}}

1.  Zwróć uwagę, że komunikaty przesyłane przez pipe R mają różne długości - pierwszy bajt opisuje długość.
2.  Ponieważ długość bloku wysyłanego jako wynik jest zakodowana na jednym bajcie na początku bloku nie możemy dowolnie (ponad 255) zwiększać stałej `MAX_BUFF` , ponieważ jest to zupełnie nieoczywiste i ważne dodano stosowny komentarz przy definicji stałej.
3.  Z logiki zadania wynika, że z czasem coraz więcej deskryptorów pipe od procesów potomnych będzie nieczynna (bo procesy dzieci giną z 20% prawdopodobieństwem) aby niepotrzebnie do takich \"umarłych\" procesów nie wysyłać liter, ich deskryptory w rodzicu musimy jakoś oznaczyć jako zamknięte. Używamy do tego wartości zero, która co prawda jest poprawną wartością deskryptora ale zazwyczaj jest używana jako stdin więc nie pojawi się naturalnie jako deskryptor pipe. Gdy program zorientuje się, że nie może wysłać do procesu potomnego, zamyka odpowiedni deskryptor pipe funkcją close, ale to nie wystarcza do oznaczenia deskryptora jako nieczynnego, musimy go jeszcze wyzerować bo funkcja close nie robi (i nie może) tego dla nas.
4.  Losowanie deskryptora do wysyłki musi się liczyć z \"trafieniem\" zera w tablicy, aby nie powtarzać losowania zastosowano prosty trik, szukamy w sąsiednich polach tablicy niezerowego deskryptora, do tego używając operacji modulo \"zawijamy tablice w koło. Aby nie kręcić się bez końca gdy tablica jest już cała wyzerowana dodano warunek sprawdzający czy aby nie szukamy za długo.

---

1. Czy w tej postaci program poprawnie zamknie wszystkie swoje łącza? 
{{< answer >}} Nie, ponieważ w signal handlerze używamy `exit()` nigdy do tego nie dojdzie. Program należy poprawić. Można np. ustawiać w handlerze flagę sygnalizującą koniec, a w `child_work` zamiast `TEMP_FAILURE_RETRY` ręcznie sprawdzać nie tylko przerwanie sygnałem ale również jej stan. {{< /answer >}}
2.  Jak jest zorganizowane czekanie na sygnał w procesie rodzicu? Nie ma blokowania, nie używamy sigsuspend, sigwait ani pselect?  
{{< answer >}} Program zorientuje się na pierwszym read w głównej pętli, dostaje informacje o \"błędzie\" EINTR. {{< /answer >}}
3.  Zwrócić uwagę na wszechobecne `TEMP_FAILURE_RETRY`. Czemu nie ma go przy tym omawianym powyżej read?  
{{< answer >}} Bo chcemy móc zareagować na ewentualna zmianę zmiennej globalnej, makro by nam to uniemożliwiło restartując read bez względu na tą zmienną - rodzic nie rozsyłałby liter do potomków. {{< /answer >}}
4.  Czy zamiast tak często wołać `TEMP_FAILURE_RETRY` można by użyć flagi `SA_RESTART`?  
{{< answer >}} Nie, bo powyżej omawiany read nie byłby w ogóle przerywany - program nie działałby, dodatkowo kod stałby się mniej przenośny o czym wspominałem w materiałach do L2 na SOP1. {{< /answer >}}
5.  Z jakich powodów nie każdy C-c powoduje wypisanie?  
{{< answer >}}  Może akurat wylosowany adresat postanawia się zakończyć, zdarza się też że rodzic może zgubić sygnały przez ich sklejanie gdy jest zajęty obsługą poprzedniego a na kolejne sygnały czeka tylko podczas wywołania funkcji read. {{< /answer >}}
6.  Drugi z powyższych powodów mógłby być ograniczony przez globalny licznik zamiast binarnej flagi `last_signal`. Zrób taką modyfikację jako ćwiczenie.
7.  Czy proces potomny może zgubić SIGINT?  
{{< answer >}}  Teoretycznie mogą się \"skleić\" ale praktycznie jest na to bardzo mała szansa bo są natychmiast obsługiwane. {{< /answer >}}
8.  Czemu odczyt z R w rodzicu jest dwuetapowy a zapis w potomku MUSI być jednorazowy?  
{{< answer >}} Inaczej mogły się przemieszać odczyty, patrz uwagi na początku tego tutoriala. {{< /answer >}}
9.  Czemu ignorujemy SIGPIPE i czy to niezbędne?  
{{< answer >}}  To jest niezbędne, inaczej pisanie do \"martwego\" dziecka zamknęłoby cały programu. Prawidłową reakcją na zerwanie łącza od potomka NIE JEST WYJŚCIE Z PROGRAMU. {{< /answer >}}
10.  Kiedy normalnie kończy się proces rodzic?  
{{< answer >}} Gdy odczyta z R zero bajtów czyli gdy łacze R zostanie zerwane, czyli gdy skończą się procesy potomne. {{< /answer >}}
11. Prawidłowa reakcja na zerwanie łącza jest zawsze ważna, sprawdź czy umiesz w kodzie wskazać wszystkie takie przypadki zarówno podczas odczytu jak i zapisu danych. Ile miejsc w kodzie tego dotyczy?  
{{< answer >}}  4  {{< /answer >}}
12. Czemu używamy unsigned char, co jeśli będzie sam char?  
{{< answer >}} Dla buforów o rozmiarze powyżej 126 c przedstawiałoby wartości ujemne! {{< /answer >}}
13. Czemu najpierw ustawiamy ignorowanie SIGINT a dopiero po forku zmieniamy to na funkcję obsługi?  
{{< answer >}}  Aby szybki C-c na początku programu go nie zabił. {{< /answer >}}
14. Czy obsługa SIGCHLD w tym programie jest niezbędna  
{{< answer >}} Jej brak nie zepsuje działania co zadowoli słabszego programistę ale powstaną zombi czego dobry programista wolałby uniknąć. {{< /answer >}}

## Kody źródłowe z treści tutoriala

{{< codeattachments >}}


## Dodatkowe materiały

- Proste przykładowe [zadanie]({{% ref "/sop2/lab/l1-task-easy" %}}). Szacunkowy czas na rozwiązanie ~60 minut.
- Nieco trudniejsze [zadanie]({{% ref "/sop2/lab/l1-task-normal" %}}). Szacunkowy czas na rozwiązanie ~90 minut.
- <http://cs341.cs.illinois.edu/coursebook/Ipc#pipes>
- <http://cs341.cs.illinois.edu/coursebook/Ipc#named-pipes>
