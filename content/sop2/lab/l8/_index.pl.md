---
title: "L8 - UDP i ?"
weight: 50
---

## WIP

		
## UDP

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

rozwiązanie `l8-1_server.c`:
{{< includecode "l8-1_server.c" >}}

rozwiązanie `l8-1_client.c`:
{{< includecode "l8-1_client.c" >}}


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

Wykonaj przykładowe zadania. Podczas laboratorium będziesz miał więcej czasu oraz dostępny startowy kod, jeśli jednak wykonasz poniższe zadania w przewidzianym czasie, to znaczy że jesteś dobrze przygotowany do zajęć.
## Kody źródłowe z treści tutoriala

{{% codeattachments %}}

## Materiały dodatkowe

 - <https://cs341.cs.illinois.edu/coursebook/Networking#layer-4-udp>
