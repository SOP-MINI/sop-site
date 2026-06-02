---
title: "L8 - Protokoły datagramowe i wielowątkowe serwery"
weight: 50
---

## Protokoły datagramowe

W przeciwieństwie do protokołów strumieniowych jak TCP, w protokołach datagramowych przesyłamy paczki danych, czyli _datagramy_. 
Tak jak w przypadku protokołów strumieniowych będziemy zajmować się nimi w kontekście gniazd sieciowych (UDP) oraz lokalnych (UNIX).

### UNIX

Lokalne gniazdo datagramowe tworzymy podobnie jak strumieniowe, po prostu specyfikując protokół na `SOCK_DGRAM`:

```C
socket(PF_UNIX, SOCK_DGRAM, 0)
```

Obowiązują wszystkie zasady związane z gniazdami lokalnymi (`man 7 unix`).
Warto zauważyć że, w przeciwieństwie do internetowych gniazd datagramowych (UDP), lokalne są niezawodne - `as on most UNIX implementations, UNIX domain datagram sockets are always reliable and don't reorder datagrams`.
Jest to więc dość wygodny sposób na przesyłanie wiadomości między procesami.
Nie mamy np. znanego z protokołów strumieniowych problemu sklejania się wiadomości.

### UDP

Sieciowe gniazdo datagramowe, czyli gniazdo UDP tworzymy analogicznie do TCP, wystarczy zmienić protokół na `SOCK_DGRAM`, np:

```C
socket(AF_INET, SOCK_DGRAM, 0)
```

dla gniazda IPv4 (`AF_INET6` dla IPv6).

Aby dowiedzieć się więcej o protokole UDP przeczytaj koniecznie `man 7 udp` oraz przejrzyj jeszcze raz [wykład o gniazdach sieciowych](../../wyk/sockets/).
W szczególności jest ważne, żeby rozumieć, że protokół UDP jest zawodny - wiadomości mogą zaginąć, przyjść w złej kolejności, przyjść zduplikowane etc.
Ponieważ jednak protokół UDP nie ma narzutu związanego z niezawodnością jak TCP, pozwala przesyłać wiadomości z mniejszym opóźnieniem.
Jest więc wykorzystywany w zastosowaniach, gdzie szybkie przesłanie danych jest ważniejsze niż niezawodność, jak np. sieciowe gry komputerowe.
Ponieważ protokół UDP jest bezpołączeniowy wspiera tzw. _broadcast_ czyli wysyłanie jednej wiadomości na wiele adresów.

### Komunikacja

Protokoły datagramowe są bezpołączeniowe - nie wywołujemy `connect` w kliencie ani `listen` oraz `accept` po stronie serwera.
Zamiast tego mamy po prostu dwie funkcje do wysyłania i odbierania danych:

```C
ssize_t recvfrom(int socket, void *restrict buffer, size_t length,
    int flags, struct sockaddr *restrict address, socklen_t *restrict address_len);

ssize_t sendto(int socket, const void *message, size_t length,
    int flags, const struct sockaddr *dest_addr, socklen_t dest_len);
``` 

Przeczytaj ich strony w manualu - `man 3p recvfrom` oraz `man 3p sendto`.
Jak widać funkcje te są analogiczne do `recv` oraz `send`, po prostu biorą po dwa dodatkowe parametry - adres oraz jego rozmiar.

Przy odbieraniu wiadomości używając `recvfrom` warto zwrócić uwagę, na specyficzny sposób jego działania.
Funkcja ta zwraca ilość przeczytanych bajtów, jednak zawsze odczytuje tylko jedną wiadomość (datagram) na raz.
Parametr `length` wskazuje na wielkość dostarczanego bufora `buffer` i oznacza _maksymalny_ rozmiar wiadomości jakiej się spodziewamy.

Jeśli wiadomość jest krótsza, po prostu zostanie wczytana w całości, natomiast jeśli jest dłuższa - nadmiarowe bajty zostaną **zignorowane i porzucone**.
Dlatego ważne jest, żeby `buffer` miał odpowiedni rozmiar, a parametr `length` był ustawiony na rozmiar największej wiadomości jaką obsługujemy w programie, a nie takiej, jakiej akurat się spodziewamy.
W protokole UDP nie mamy gwarancji, że wiadomości przyjdą w dobrej kolejności.

W manualu `man 2 recvfrom` znajdziesz nieco dodatkowych informacji specyficznych dla systemu Linux. Istnieje np. funkcja `recvmsg`, która w niektórych przypadkach pozwala uzyskać lepszą wydajność, oraz np. zwrócić flagę sygnalizującą, że wiadomość została obcięta. Pamiętaj jednak, że zawartość tej strony manuala nie należy do standardu POSIX i tym samym jest nieprzenośna na inne systemy unixowe (np. MacOS, BSD). Na laboratoriach generalnie używamy funkcji ze standardu.

Funkcja `sendto` jest jeszcze prostsza w użyciu - albo wyśle cały datagram, albo zakończy się błędem i nie wyśle nic.

## Zadanie

Cel:

Napisz dwa programy pracujące w architekturze klient-serwer poprzez
połączenie UDP. Zadanie programu klienta polega na wysłaniu pliku
tekstowego podzielonego na datagramy. Zadanie programu serwera polega
na odbieraniu plików przesyłanych przez socket i wypisywaniu ich
na ekran (bez informacji o pliku z którego dane pochodzą).

Każdy wysłany do serwera pakiet musi być potwierdzony odpowiednim
komunikatem zwrotnym, w razie braku takiego zwrotnego komunikatu
(czekamy 0,5s) należy ponawiać wysłanie pakietu. W razie 5 kolejnych
niepowodzeń program klienta powinien zakończyć działanie. Potwierdzenia
też mogą zaginąć w sieci, ale program powinien sobie i z tym radzić -
serwer nie może dwa razy wypisać tego samego fragmentu tekstu.

Wszystkie dodatkowe dane (wszystko poza tekstem z pliku) przesyłane
między serwerem i klientem mają mieć postać liczb typu int32_t.  Należy
przyjąć, że rozmiar przesyłanych jednorazowo danych (tekst z pliku i
dane sterujące)  nie może przekroczyć 576B. Naraz serwer może odbierać
maksymalnie 5 plików, szósta jednoczesna transmisja ma być zignorowana.

Program serwer jako parametr przyjmuje numer portu na którym będzie
pracował, program klient przyjmuje jako parametry adres i port serwera
oraz nazwę pliku.

Co student musi wiedzieć:
- man 7 udp
- man 3p sendto
- man 3p recvfrom
- man 3p recv
- man 3p send

rozwiązanie `l8-1_server.c`:
{{< includecode "l8-1_server.c" >}}

rozwiązanie `l8-1_client.c`:
{{< includecode "l8-1_client.c" >}}


Zwróć uwagę, że w protokole UDP nie nawiązujemy połączenia, gniazda komunikują się ze sobą "ad hoc". Nie ma gniazda nasłuchującego. Możliwe są straty, duplikaty i zmiany kolejności datagramów.
W przykładzie występują kolejne przydatne do biblioteki wariacje funkcji: make_socket, bind_inet_socket, ponieważ mają te same nazwy co funkcje użyte w poprzednim zadaniu trzeba je inaczej ponazywać.

W tym zadaniu  kontekst połączenia jest ważny i wymaga wysiłku aby go utrzymać. Co jest kontekstem połączenia?
{{< answer >}} Kontekstem jest liczba poprawnie przesłanych pakietów do danej chwili. {{< /answer >}}

Jakie dane są przesyłane w pojedynczym datagramie? Czemu służą przesyłane metadane?</br>
{{< answer >}} Pakiet składa się z (1) 32 bitowego numeru fragmentu, (2) 32 bitowej informacji czy to ostatni fragment oraz (3) z fragmentu pliku. Metadane służą do kontroli kontekstu (1) oraz do zakończenia transmisji (2).   {{< /answer >}}

Czemu i na jakich deskryptorach są używane funkcje bulk_read i bulk_write, czy nie powinno się rozszerzyć tego użycia na wszystkie deskryptory?</br>
{{< answer >}}  Funkcje są potrzebne do restartowania read i write  w sytuacji przerwania w trakcie operacji IO ( w odróżnieniu od `EINTR` czyli przerwania przed operacją). Funkcje te są używane tylko do działań na plikach ponieważ przesyłanie datagramów jest atomowe i nie może być przerwane w trakcie. W tym programie występuje obsługa sygnałów ale tam gdzie się ich spodziewamy nie dokonujemy operacji na plikach. To zabezpieczenie jest nadmiarowe, dodane z myślą o przenoszeniu tego kodu do innych programów.  {{< /answer >}}

Czy może wystąpić sytuacja zerwania połączenia? Czy nie powinniśmy tego rozpoznawać?
{{< answer >}} Nie może, udp nie wytwarza połączenia, które mogłoby być zerwane. {{< /answer >}}

Przeanalizuj jak działa findIndex w serwerze, zwłaszcza jak są porównywane adresy. W jakim byte orderze są? Jak zachowa się ta funkcja jeśli adres jest nowy? 
{{< answer >}} Porównywane adresy są w byte order sieci, nie mamy potrzeby ich konwertować skoro jedynie je porównujemy a nie np. wyświetlamy. Funkcja dla nowego adresu zakłada nowy rekord (o ile ma jeszcze wolne miejsce w tablicy). {{< /answer >}}

Jak sobie poradzimy z duplikatami datagramów?
{{< answer >}} Trzymamy tablicę stanu połączeń "struct connections", wiemy, który fragment już wypisaliśmy i nie powtarzamy go. {{< /answer >}}

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


## Wielowątkowe serwery

Na poprzednim laboratorium ćwiczyliśmy pisanie serwerów działających w oparciu o jeden wątek.
Taka architektura ma wiele sensu gdy musimy oszczędzać zasoby a spodziewamy się niezbyt dużego obciążenia.

Często jednak jest tak, że nasz serwer musi obsłużyć bardzo dużą ilość zapytań.
W takiej sytuacji, żeby uzyskać odpowiednią wydajność na nowoczesnym sprzęcie, konieczne jest wykorzystanie wielu wątków.
Typową i naturalną architekturą jest jeden wątek odbierający wiadomości oraz przekazujący zadania do wątków roboczych, które je wykonują i wysyłają rezultaty do klientów.
Z drugiej strony atomowość operacji na datagramach pozwala też na wiele wątków jednocześnie czekać na wiadomość na jednym gnieździe. Oczywiście często w takiej sytuacji jest z nim związany jakiś dodatkowy stan, dlatego i tak może być konieczna synchronizacja np. za pomocą mutexu.

W celu napisania wydajnych wielowątkowych programów na laboratorium warto powtórzyć sobie L4 (synchronizacja), w szczególności mutexy, semafory i zmienne warunkowe. Ponadto typowe struktury danych używane do tego typu zadań jak pula wątków czy bufor cykliczny. Jeśli słabo pamiętasz te zagadnienia przejrzyj tutoriale do [L3](../../../sop1/lab/l3/) i [L4](../../../sop1/lab/l4) oraz [slajdy i programy wykładowe z synchronizacji](../../../sop1/wyk/w7).


## Przykładowe zadania

Wykonaj przykładowe zadania. Podczas laboratorium będziesz miał więcej czasu oraz dostępny startowy kod, jeśli jednak wykonasz poniższe zadania w przewidzianym czasie, to znaczy, że jesteś dobrze przygotowany do zajęć.

- [Zadanie 1]({{< ref "/sop2/lab/l8/example1" >}}) ~100 ~~dni~~ minut
- [Zadanie 2]({{< ref "/sop2/lab/l8/example2" >}}) ~130 minut
- [Zadanie 2 z L7]({{< ref "../l7/example2" >}}) ~120 minut na całość, etapy 4-5 dotyczą L8


## Kody źródłowe z treści tutoriala

{{% codeattachments %}}

## Materiały dodatkowe

 - <https://cs341.cs.illinois.edu/coursebook/Networking#layer-4-udp>
