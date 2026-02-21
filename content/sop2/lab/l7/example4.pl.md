---
title: "Zadanie testowe z sieci"
bookHidden: true
---

# Οἱ ἄγγελοι (The messengers)

Jest rok 480 p.n.e.  
Perski król Kserkses (staroperski: 𐎧𐏁𐎹𐎠𐎾𐏁𐎠, Khshayārsha) właśnie najechał greckie państwa-miasta.  
To niefortunne wydarzenie spowodowało, że wielu posłańców podróżujących po Grecji, zaczyna działać w charakterze zwiadowców.  
Odwiedzają oni różnorakie miasta. Niektóre osady kontrolowane są przez Greków, inne zostały przechwycone przez Persów.  

Aby uzyskać przewagę informacyjną, obywatele Sparty (grecki dorycki: Σπάρτα) decydują się na otwarcie biblioteki połączonej z najnowszą siecią telekomunikacyjną posłańców.

Każdy z posłańców otrzymuje aplikację klienta TCP, która może łączyć się z serwerem biblioteki.  
Biblioteka zawiera listę państw-miast oraz informacje, czy są one kontrolowane przez Greków, czy przez Persów.  
Podobną listę utrzymuje też każdy z posłańców, aktualizując ją, gdy otrzyma nowe informacje z centralnej biblioteki.  
Jest 20 państw-miast. Każde z nich jest oznaczone liczbą ze zbioru [1, 20].  
Twoim zadaniem jest **napisanie programu klienta** kompatybilnego z serwerem biblioteki.  

Serwer dostarczony jest w postaci pliku wykonywalnego. Można go uruchomić przez:

```shell
./server <PORT>
```

Dostarczony jest też plik wykonywalny przykładowej implementacji programu klienta.

## Stages:

1. Zaimplementuj łączenie się z serwerem TCP.  
   - Klient przyjmuje dwa argumenty: adres serwera i numer portu.  
   - Przykładowe uruchomienie klienta:
     ```shell
     ./client 127.0.0.1 8888
     ```
   - Po uruchomieniu:
     - Połącz się z podanym serwerem za pomocą protokołu TCP.
     - Po nawiązaniu połączenia:
       - Wyślij 4-znakową wiadomość (wczytaną ze standardowego wejścia) do serwera.
       - Zamknij połączenie i zakończ działanie programu.

2. Zaimplementuj obsługę komend wprowadzanych przez `stdin` oraz wysyłanie wiadomości.
   - Po połączeniu z serwerem, oczekuj na komendy wprowadzone z klawiatury (`stdin`).
   - Obsłuż następujące komendy:
     - `e` (exit) – zamknij połączenie z serwerem i zakończ działanie programu.
     - `m XXX` (message) – wyślij do serwera 4-znakową wiadomość (3 znaki + znak nowej linii), gdzie `XXX` to dowolne 3 znaki.
     - `t XX` (travel) – losowo wybierz jedną z liter {g, p} i wyślij:
       ```
       YXX\n
       ```
       do serwera, gdzie `Y` to wylosowana litera, a `XX` to dwucyfrowy numer miasta z zakresu [1, 20].  
       Dla liczb jednocyfrowych (1–9) dodaj zero z przodu.  
       Jeśli numer miasta nie mieści się w tym zakresie, wypisz błąd i oczekuj kolejnej komendy.
     - `o` (owners) – wyświetl informacje o właścicielach wszystkich miast (opisane poniżej).
   - Przechowuj tablicę określającą, do której ze stron konfliktu (Grecy lub Persowie) należy każde z miast.
   - Początkowo wszystkie miasta mają właściciela nieznanego.
   - Gdy zostanie wysłana wiadomość `gXX\n` przez komendę `t`, przypisz miastu `XX` właściciela: Greków.
   - Analogicznie, po `pXX\n`, przypisz właściciela: Persów.

3. Oprócz oczekiwania na dane z `stdin`, nasłuchuj również danych przychodzących z serwera.
   - Użyj mechanizmu `epoll` (lub alternatywnie `ppoll` albo `pselect`), aby monitorować jednocześnie deskryptory `STDIN_FILENO` i gniazda serwera.
   - Serwer może wysyłać wiadomości w takim samym formacie, jak komenda `t XX` wysyłana przez klienta.
   - Odbierając taką wiadomość z serwera, potraktuj ją dokładnie tak samo, jakby została wygenerowana lokalnie przez komendę `t` (czyli zaktualizuj własność miasta).

4. Po otrzymaniu sygnału `SIGINT`:
   - Wypisz właściciela każdego z miast.
   - Zamknij aktywne połączenia.
   - Zwolnij wszystkie zasoby.
   - Zakończ działanie programu.

## Client and server executables:

- [sop2l7e3-4.zip](/files/sop2l8e3-4.zip)
