---
title: "L4 - gniazda sieciowe i epoll"
date: 2022-02-01T19:51:45+01:00
weight: 30
---

Uwagi wstępne:

1. W trakcie tych zajęć przydatny jest [program netcat]({{< ref "../netcat" >}})
1. Obowiązują wszystkie materiały z SOP1 i SOP2 jakie były do tej pory, szczególnie ważne są te dotyczące wątków i procesów!
1. Szybkie przejrzenie tutoriala prawdopodobnie nic nie pomoże, należy samodzielnie uruchomić programy, sprawdzić jak działają, poczytać materiały dodatkowe takie jak strony man. W trakcie czytania sugeruję wykonywać ćwiczenia a na koniec przykładowe zadanie.
1. Na żółtych polach podaję dodatkowe informacje, niebieskie zawierają pytania i ćwiczenia. Pod pytaniami znajdują się odpowiedzi, które staną się widoczne dopiero po kliknięciu. Proszę najpierw spróbować sobie odpowiedzieć na pytanie samemu a dopiero potem sprawdzać odpowiedź.
1. Pełne kody do zajęć znajdują się w załącznikach na dole strony. W tekście są tylko te linie kodu, które są konieczne do zrozumienia problemu.
1. Materiały i ćwiczenia są ułożone w pewną logiczną całość, czasem do wykonania ćwiczenia konieczny jest stan osiągnięty poprzednim ćwiczeniem dlatego zalecam wykonywanie ćwiczeń w miarę przyswajania materiału.
1. Większość ćwiczeń wymaga użycia konsoli poleceń, zazwyczaj zakładam, ze pracujemy w jednym i tym samym katalogu roboczym więc wszystkie potrzebne pliki są "pod ręką" tzn. nie ma potrzeby podawania ścieżek dostępu.
1. Czasem podaję znak $ aby podkreślić, że chodzi o polecenie konsolowe, nie piszemy go jednak w konsoli np.: piszę "$make" w konsoli wpisujemy samo "make".
1. To co ćwiczymy wróci podczas kolejnych zajęć. Jeśli po zajęciach i teście coś nadal pozostaje niejasne proszę to poćwiczyć a jeśli trzeba dopytać się u prowadzących.
1. W wielu miejscach w kodzie dodano obsługę przerwania sygnałem (makro TEMP_FAILURE_RETRY), nawet jeśli w danym programie to nie ma uzasadnienia bo brak w nim obsługi sygnałów. Duża część prezentowanego kodu będzie przez studentów przenoszona do ich rozwiązań, zwłaszcza gotowe funkcję biblioteczne,  w których to rozwiązaniach mogą się pojawiać sygnały. Jest to zabieg zwiększający przenośność kodu.
1. Podczas planowania w jakiej postaci dane będą przesyłane przez sieć bardzo ważne jest aby zawsze pamiętać o tym, że komunikują się programy działające (być może) na różnych architekturach sprzętowych, trzeba mieć wzgląd na następujące kwestie:
1. byte order - czyli w jakiej kolejności w pamięci są przechowywane liczby całkowite, jeśli przesyłamy liczby binarnie pomiędzy programami działającymi na różnych architekturach to zmiana tej kolejności będzie katastrofalna, równoznaczna ze zmianą liczby, np: 0x00FF zmienia się nam na 0xFF00! Nie musimy znać byte orderu architektury na której działamy, wystarczy pamiętać o zasadzie, że do sieci wysyłamy wszystkie liczby całkowite zakodowane w tzw. network byte order. Do konwersji do tej postaci mamy makra htons i htonl (dla 16 bitowego shorta i 32 bitowego integera). Po odebraniu danych z sieci następuje odwrotna konwersja z byte orderu sieciowego na lokalny, makra ntohs i ntohl.
1. Nie ma uniwersalnego kodowania dla liczb zmiennopozycyjnych, różne mogą być ich implementacje na różnych platformach więc albo musimy przesłać je jako tekst lub jako liczby o ustalonej precyzji (czyli jako liczby całkowite tak naprawdę).
1. Pamiętamy oczywiście, że problem byte order nie dotyczy danych jedno-bajtowych w tym przede wszystkim tekstu. Przesłanie danych w formie tekstowej jest prawie zawsze poprawnym rozwiązaniem.
1. Różne mogą być rozmiary typów int,short czy long na różnych architekturach. Aby uniknąć problemów najlepiej jest używać typów o ustalonym wymiarze takich jak np.: int32_t czy uint16_t
1. Niezbyt wygodne będzie też przesyłanie struktur ze względu na różne ułożenie ich pól w pamięci na różnych platformach . Występują różne przerwy pomiędzy polami wynikające z optymizacji adresowania  - adresy podzielne przez różne zależne od architektury potęgi 2 są szybciej pobierane z pamięci.  Aby to obejść, należy zadbać aby kompilator nie optymizował tych struktur tylko "pakował" je bez żadnych przerw. Nie zawsze da się to osiągnąć z poziomu kodu, zazwyczaj konieczne są dyrektywy kompilatora co czyni to rozwiązanie mniej przenośnym. Jak się da to unikamy przesyłania całych struktur binarnie, możemy przesyłać strukturę pole po polu. 

 
## Zadanie local + TCP
Napisz prosty sieciowy kalkulator liczb całkowitych. Dane przesyłane pomiędzy klientami a serwerem mają postać:
- operand 1
- operand 2
- wynik
- operator (+,-,*,/)
- status
Wszystko przekonwertowane do postaci 32 bitowych liczb w tablicy.

Czemu nie struktura? 
{{< answer >}} Struktury bez pakowania są mniej przenośne ze względu na optymizację adresacji pamięci - różne przerwy pomiędzy polami, w tym przypadku łatwiej wyślemy tablice, która nie ma tej przypadłości co struktura. {{< /answer >}} 

Serwer wylicza wynik i odsyła go do klienta. Jeśli wyliczenie przebiegło pomyślnie pole status przyjmuje wartość 1, jeśli nie (np. dzielenie przez zero) wartość 0. Komunikacja z serwerem jest możliwa na 2 sposoby:
- gniazda lokalne
- gniazda sieciowe tcp

Serwer ma być jednoprocesowy i jednowątkowy, przyjmuje następujące argumenty:
- nazwa lokalnego gniazda
- port
	
Należy napisać 2 programy klientów, po jednym dla każdego typu połączenia, programy te pobierają następujące parametry:
- adres gniazda serwera (nazwa domenowa dla tcp, nazwa pliku specjalnego dla połączenia lokalnego)
- numer portu
- operand 1
- operand 2
- operator (+,-,*,/)
Jeśli wyliczenie się uda należy wyświetlić wynik. 
Wszystkie 3 programy można przerwać C-c, nie wolno po sobie zostawiać pliku połączenia lokalnego.
		
### Rozwiązanie

Cel:

Serwer akceptuje połączenia lokalne
Klient do połączeń lokalnych

Co student musi wiedzieć:
```
man 7 socket
man 7 unix
man 7 epoll
man 3p socket
man 3p bind
man 3p listen
man 3p connect
man 3p accept
man 2 epoll_create
man 2 epoll_ctl
man 2 epoll_wait
```

Zwróć uwagę zwłaszcza na sekcję Q&A w `man 7 epoll`. Ponieważ jest bardzo dobrze przygotowana, nie będziemy jej tutaj powtarzać.

rozwiązanie `prog23a_s.c`:
{{< includecode "prog23a_s.c" >}}
rozwiązanie `prog23_local.c`:
{{< includecode "prog23_local.c" >}}
Uruchomienie:
```
$ ./prog23a_s a 2000&
$ ./prog23_local a 2 1 +
$ ./prog23_local a 2 1 '*'
$ ./prog23_local a 2 0 /
$ killall -s `SIGINT` prog23a_s
```

Zwróć uwagę, że funkcje make_socket i bind_socket, connect_socket i add_new_cleint  nadają się do biblioteki. Ich działanie powinno być oczywiste. Schemat nawiązania połączenia sieciowego był omówiony na wykładzie.

Może zastanawiać czemu stała `BACKLOG` jest ustalona na 4 a nie 5, 7 czy 9? To może być dowolna mała liczna, to tylko wskazówka dla systemu, ten program nie będzie obsługiwał dużego ruchu i kolejka czekających połączeń nie będzie nigdy duża, w praktyce połączenia są tu od razu realizowane. Przy większym ruchu trzeba empirycznie sprawdzać jaka wartość tego parametru dobrze się spisze i niestety będzie ona inna na różnych systemach. 

W programie używamy makra `SUN_LEN`, czemu nie sizeof? Oba rozwiązania działają poprawnie. Warto wiedzieć, że użycie zwykłego sizeof zwróci większy rozmiar niż makro a to dlatego, że rozmiar liczony przez makro to suma pól struktury (czyli typ i string) a rozmiar podany przez sizeof dodaje jeszcze kilka bajtów przerwy pomiędzy tymi polami. Implementacja oczekuje mniejszej z tych dwóch wartości ale podanie większej nic nie zepsuje ponieważ sam adres jest w postaci ciągu znaków zakończonego zerem. Co zatem wybrać? W materiałach wybieramy zgodność ze standardem czyli makro zatem oszczędzamy te kilka bajtów kosztem nieco dłuższego wyliczania rozmiaru. Jeśli użyjesz sizeof to nie będzie to traktowane jako błąd.

Standard POSIX mówi, że nawiązanie połączenia sieciowego nie może być przerwane (co jest dość logiczne ponieważ biorą w nim udział dwie strony). Zatem jeśli funkcja obsług sygnału przerwie `connect` to tak naprawdę jego nawiązywanie nadal trwa asynchronicznie względem głównego kodu. Próba restartu funkcji np. makrem `TEMP_FAILURE_RETRY` nie jest już możliwa (spowoduje błąd `EALREADY`). W kodzie naszego klienta nie obsługujemy sygnałów, co jednak, gdybyśmy to robili? Należy sprawdzić, czy `errno` jest równe `EINTR` i w takim wypadku użyć funkcji `select`, `poll` albo `epoll*` żeby poczekać aż będzie możliwy zapis.

Można od razy zażyczyć sobie asynchronicznego połączenia, wystarczy przed wywołaniem connect ustawić na deskryptorze gniazda flagę `O_NONBLOC`. Nawiązywanie połączenia jest dość czasochłonne i jest to sposób aby w czasie oczekiwania program mógł coś wykonać.

W programie przepływ danych jest trywialny, klient łączy się, wysyła zapytanie i od razy dostaje odpowiedź po czym się rozłącza. Możliwe są oczywiście znacznie bardziej złożone scenariusze wymiany komunikatów (protokoły).
W danym momencie komunikacji pomiędzy programami to co zostało przesłane wcześniej tworzy "kontekst" dla tego co wysyłamy. W zależności od protokołu, kontekst może być bardziej lub mniej złożony. W tym programie zapytanie jest kontekstem dla odpowiedzi, czyli odpowiedź wynika z kontekstu komunikacji (pytania).

Warto zapytać czemu służą makra ntohl i htonl użyte do konwersji byte orderu liczb skoro połączenie jest lokalne? W tym programie sporo kodu w drugim etapie będzie współdzielone z połączeniem sieciowym tcp, które takiej konwersji wymaga. Połączeniu lokalnemu taka konwersja nie przeszkadza. Warto też dodać, że nic w standardzie POSIX nie zabrania aby powstały w przyszłości gniazda lokalne działające na sieciowym systemie plików, czyli takie które będą tej konwersji jednak wymagać, nasz program jest na to gotowy.

W kodzie używana jest funkcja `bulk_read`, trzeba wiedzieć, że w takiej postaci jak powyżej ta funkcja nie potrafi sobie poradzić z deskryptorem w trybie nieblokującym - zwróci błąd `EAGAIN`. Czy w tym przypadku mamy taki deskryptor? Nowo otwarte (przez f. accept) połączenie dostaje  nowy deskryptor, jego flagi nie muszą być dziedziczone (na Linuksie nie są) z deskryptora gniazda nasłuchującego. W tym programie nie będzie problemu bo wiemy, że dane już czekają na odbiór i pod Linuksem mamy tryb blokujący ale warto może przerobić funkcję bulk_read tak aby czekała na dane gdy gniazdo jest w trybie nieblokującym.


Jak po uruchomieniu serwera można podejrzeć plik gniazda? 
{{% answer %}} `$ls -l a AAAAA`  {{% /answer %}}

Czym jest w programie wywołanie pselect?
{{< answer >}} Jest punktem w którym program czeka na dostępność danych na wielu deskryptorach oraz na nadejście sygnału `SIGINT` {{< /answer >}}

Czy można użyć tu select a zamiast pselect?
{{< answer >}} Można ale nie warto bo dodanie poprawnej obsługi `SIGINT` będzie wtedy bardziej pracochłonne {{< /answer >}}

Skoro czekamy tylko na połączenie na jednym deskryptorze to może warto pozbyć się pselect zupełnie?</br>
{{< answer >}} Nie warto, bo za chwilę w drugim etapie dodamy czekanie na drugim deskryptorze, dodatkowo implementacja prawidłowej obsługi sygnału `SIGINT` będzie pracochłonna. {{< /answer >}}

Czemu gniazdo sieciowe jest w trybie nieblokującym? 
{{< answer >}} Bez tego trybu mogłoby się zdarzyć, że klient który chce się połączyć "zginie" pomiędzy select, które potwierdzi gotowość do połączenia a accept, które faktycznie przyjmie to połączenie. Wtedy na nieblokującym gnieździe program zatrzymałby się aż do nadejścia kolejnego połączenia nie reagując na sygnał `SIGINT`. {{< /answer >}}

Czemu użyto `int32_t` (stdint.h) a nie zwykły int? 
{{< answer >}} Ze względu na różne rozmiary int'a  na różnych architekturach co stanie się ważne w 2 etapie. {{< /answer >}}

Czemu ignorujemy `SIGPIPE` w serwerze?
{{< answer >}} Łatwiej obsłużyć błąd `EPIPE` niż sygnał, zwłaszcza, że informacja o przedwczesnym zakończeniu się klienta nie może prowadzić do zamknięcia serwera. {{< /answer >}}

Czemu w programie użyto bulk_read i bulk_write? Nie ma obsługi sygnałów poza `SIGINT`, który i tak kończy działanie programu. 
{{< answer >}} Z tego samego powodu dla którego tak często `EINTR` jest obsłużony - przenośność kodu do innych rozwiązań. {{< /answer >}}

Czemu służy unlink w kodzie serwera? 
{{< answer >}} Usuwamy gniazdo lokalne tak samo jak usuwamy plik -  funkcja porządkowa. {{< /answer >}}

		
### Etap 2

Cel:

Serwer akceptuje połączenia sieciowe TCP
Klient sieciowy TCP

Co student musi wiedzieć:

- man 7 tcp
- man 3p freeaddrinfo (obie funkcje, getaddrinfo też)
- man 3p gai_strerror

rozwiązanie `prog23b_s.c`:
{{< includecode "prog23b_s.c" >}}

rozwiązanie `prog23_tcp.c`:
{{< includecode "prog23_tcp.c" >}}

Uruchomienie: 
```
$ ./prog23b_s a 2000&
$ ./prog23_tcp localhost 2000 234 17  /
$ ./prog23_local a 2 1 '*'
$ killall -s SIGINT prog23b_s
Dobierzcie się w pary, podajcie sobie swoje nazwy hosta (są w linii komend, np: p21804), niech każdy uruchomi serwer a potem wyśle zapytanie do sąsiada:
$ ./prog23b_s a 2000 &
$ ./prog23_tcp p21804 2000  2 2  +
...
$ killall -s SIGINT prog23b_s
```


Zwróć uwagę na rozwinięcie funkcji obsługi połączenia make_socket, bind_local_socket, bind_tcp_socket tak aby objąć też TCP, klient TCP ma także przydatne funkcje make_socket i make_address, inne niż w kliencie lokalnym które warto włączyć do biblioteki własnych funkcji.
Obowiązuje Państwa użycie funkcji getaddrinfo, starsza funkcja gethostbyname  jest w dokumentacji oznaczona jako przestarzała i ma nie być używana w pracach studenckich.

Czemu służy opcja gniazda SO_REUSEADDR? 
{{< answer >}} Dodanie tej opcji pozwala na szybkie ponowne wykorzystanie tego samego portu serwera, jest to ważne gdy chcemy szybko poprawić jakiś błąd w kodzie i ponownie uruchomić program. Bez tej opcji system blokuje dostęp do portu na kilka minut. {{< /answer >}}

Czy powyższa opcja nie naraża nas na błędy komunikacji wynikające z możliwości otrzymania jakichś pozostałości po poprzednim połączeniu?
{{< answer >}} Nie, protokół tcp jest na takie "śmieci" uodporniony. Gdyby połączenie było typu udp wtedy odpowiedź byłaby twierdząca. {{< /answer >}}

Co to za adres INADDR_ANY i czemu jest często używany jako adres lokalny?
{{< answer >}} To adres specjalny postaci 0.0.0.0, który ma szczególne znaczenie w adresacji. Oznacza dowolny adres. Jeśli gniazdu serwera (nasłuchującemu) damy taki adres to oznacza, że jakiekolwiek połączenie nadejdzie do serwera, niezależnie jak zaadresowane to będzie przyjęte. To oczywiście nie oznacza, że przechwycimy wszystkie połączenia  w sieci, to oznacza, że nie jest ważne jaki adres w sieci ma serwer (może mieć nawet kilka) jeśli tylko ustawienia sieci spowodują, że połączenie będzie kierowane na dany serwer to nasz program na nim będzie mógł je przyjąć nie znając swojego faktycznego lokalnego adresu!  {{< /answer >}}

Przeanalizuj jak został w serwerze rozwinięty pselect aby obsłużyć drugi typ połączenia. Zwrócić uwagę, że dodanie TCP w serwerze wymagało tylko dodania kodu nawiązania połączenia, reszta jest ta sama.

Kod klienta jest bardzo podobny do klienta lokalnego, jako ćwiczenie proponuję zintegrowanie tych kodów do jednego programu z przełącznikiem -p local|tcp



		
## Zadanie 2 - UDP

Cel:

Napisz dwa programy pracujące w architekturze klient-serwer poprzez
połączenie UDP. Zadanie programu klienta polega na wysłaniu pliku
podzielonego na datagramy. Zadanie programu serwera polega na
odbieraniu plików przesyłanych przez socket i wypisywaniu ich na
ekran (bez informacji o pliku z którego dane pochodzą).

Każdy wysłany do serwera pakiet musi być potwierdzony odpowiednim
komunikatem zwrotnym, w razie braku takiego zwrotnego komunikatu
(czekamy 0,5s) należy ponawiać wysłanie pakietu. W razie 5 kolejnych
niepowodzeń program klienta powinien zakończyć działanie. Potwierdzenia
też mogą zaginąć w sieci, ale program powinien sobie i z tym radzić -
serwer nie może dwa razy wypisać tego samego fragmentu tekstu.

Wszystkie dodatkowe dane (wszystko poza tekstem z pliku) przesyłane
między serwerem i klientem mają mieć postać liczb typu int32_t.  Należ
przyjąć, że rozmiar przesyłanych jednorazowo danych (tekst z pliku i
dane sterujące)  nie może przekroczyć 576B. Naraz serwer może odbierać
maksymalnie 5 plików, 6 jednoczesna transmisja ma być zignorowana.

Program serwer jako parametr przyjmuje numer portu na którym będzie
pracował, program klient przyjmuje jako parametry adres i port serwera
oraz nazwę pliku.

Co student musi wiedzieć:
- man 7 udp
- man 3p sendto
- man 3p recvfrom
- man 3p recv
- man 3p send

rozwiązanie `prog24s.c`:
{{< includecode "prog24s.c" >}}

rozwiązanie `prog24c.c`:
{{< includecode "prog24c.c" >}}


Zwróć uwagę, że w protokole UDP nie nawiązujemy połączenia, gniazda komunikują się ze sobą "ad hoc". Nie ma gniazda nasłuchującego. Możliwe są straty, duplikaty i zmiany kolejności datagramów.
W przykładzie występują kolejne przydatne do biblioteki wariacje funkcji: make_socket, bind_inet_socket, ponieważ mają te same nazwy co funkcje użyte w poprzednim zadaniu trzeba je inaczej ponazywać.

W tym zadaniu  kontekst połączenia jest ważny i wymaga wysiłku aby go utrzymać. Co jest kontekstem połączenia?
{{< answer >}} Kontekstem jest ilość poprawnie przesłanych pakietów do danej chwili. {{< /answer >}}

Jakie dane są przesyłane w pojedynczym datagramie? Czemu służą przesyłane metadane?</br>
{{< answer >}} Pakiet składa się z (1) 32 bitowego numeru fragmentu, (2) 32 bitowej informacji czy to ostatni fragment oraz (3) z fragmentu pliku. Metadane służą do kontroli kontekstu (1) oraz do zakończenia transmisji (2).   {{< /answer >}}

Czemu i na jakich deskryptorach są używane funkcje bulk_read i bulk_write, czy nie powinno się rozszerzyć tego użycia na wszystkie deskryptory?/br>
{{< answer >}}  Funkcje są potrzebne do restartowania read i write  w sytuacji przerwania w trakcie operacji IO ( w odróżnieniu od `EINTR` czyli przerwania przed operacją). Funkcje te są używane tylko do działań na plikach ponieważ przesyłanie datagramów jest ATOMOWE i nie może być przerwane w trakcie. W tym programie występuje obsługa sygnałów ale tam gdzie się ich spodziewamy nie dokonujemy operacji na plikach. To zabezpieczenie jest nadmiarowe, dodane z myślą o przenoszeniu tego kodu do innych programów.  {{< /answer >}}

Czy może wystąpić sytuacja zerwania połączenia? Czy nie powinniśmy tego rozpoznawać?
{{< answer >}} Nie może, udp nie wytwarza połączenia, które mogłoby być zerwane. {{< /answer >}}

Przeanalizuj jak działa findIndex w serwerze, zwłaszcza jak są porównywane adresy. W jakim byte orderze są? Jak zachowa się ta funkcja jeśli adres jest nowy? 
{{< answer >}} Porównywane adresy są w byte order sieci, nie mamy potrzeby ich konwertować skoro jedynie je porównujemy a nie np. wyświetlamy. Funkcja dla nowego adresu zakłada nowy rekord (o ile ma jeszcze wolne miejsce w tablicy). {{< /answer >}}

Jak sobie poradzimy z duplikatami datagramów?
{{< answer >}} Trzymamy tablice stanu połączeń "struct connections", wiemy, który fragment już wypisaliśmy i nie powtarzamy go. {{< /answer >}}

Jak sobie poradzimy, z odwrotną kolejnością datagramów, czyli gdy otrzymamy fragment dalszy niż aktualnie oczekiwany?
{{< answer >}} Odwrócenie nie  może się zdarzyć, bo klient nie prześle dalszych części dopóki nie potwierdzimy wcześniejszych. {{< /answer >}}

Jak sobie poradzimy z ginącymi pakietami ?
{{< answer >}} Obsługuje to retransmisja po stronie klienta. {{< /answer >}}

Co się stanie jeśli zaginie potwierdzenie pakietu a nie sam pakiet?
{{< answer >}} Klient uzna, że pakiet nie dotarł i prześle go ponownie. Serwer nie wyświetli pakietu drugi raz ale odeśle potwierdzenie po raz kolejny.  {{< /answer >}}

Co zawierają potwierdzenia?
{{< answer >}} Odsyłamy to co dostaliśmy, cały pakiet bez zmiany. {{< /answer >}}

Jak jest zaimplementowany timeout na odpowiedź od serwera? 
{{< answer >}} W funkcji sendAndConfirm najpierw ustawiamy alarm na 0.5 sekundy (setitimer) następnie program stara się odebrać potwierdzenie. Brak restartu funkcji recv makrem nie  jest przypadkowy, po ew. przerwaniu musimy móc sprawdzić czy to nie był oczekiwany timeout.  {{< /answer >}}

Czemu konwertujemy tylko byte order numeru fragmentu i znacznika ostatniego elementu a reszta danych nie jest odwracana?
{{< answer >}} Tylko te dwie dane są przesyłane jako liczby binarne,  reszta to tekst, który nie wymaga tego zabiegu.  {{< /answer >}}

Przeanalizuj jak działa limitowanie do 5 połączeń, zwróć uwagę na pole free w strukturze i znaczenie znacznika ostatniego fragmentu przesyłanego przez klienta.


## Przykładowe zadanie
Wykonaj przykładowe [ćwiczenie]({{< ref "../l4-example">}}") z poprzednich lat. To zadanie szacuję na 60 minut, jeśli wyrobisz się w tym czasie to znaczy, że jesteś dobrze przygotowany/a do zajęć.


## Kody źródłowe z treści tutoriala
{{% codeattachments %}}
