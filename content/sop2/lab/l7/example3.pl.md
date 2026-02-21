---
title: "Zadanie testowe z sieci"
bookHidden: true
---

# Οἱ ἄγγελοι (Posłańcy)

Jest rok 480 p.n.e.  
Perski król Kserkses (staroperski: 𐎧𐏁𐎹𐎠𐎾𐏁𐎠, Khshayārsha) właśnie najechał greckie państwa-miasta.  
To niefortunne wydarzenie spowodowało, że wielu posłańców podróżujących po Grecji, zaczyna działać w charakterze zwiadowców.  
Odwiedzają oni różnorakie miasta. Niektóre osady kontrolowane są przez Greków, inne zostały przechwycone przez Persów.  

Aby uzyskać przewagę informacyjną, obywatele Sparty (grecki dorycki: Σπάρτα) decydują się na otwarcie biblioteki połączonej z najnowszą siecią telekomunikacyjną posłańców.

Każdy z posłańców otrzymuje aplikację klienta TCP, która może łączyć się z serwerem biblioteki.  
Biblioteka zawiera listę państw-miast oraz informacje, czy są one kontrolowane przez Greków, czy przez Persów.  
Podobną listę utrzymuje też każdy z posłańców, aktualizując ją, gdy otrzyma nowe informacje z centralnej biblioteki.  
Jest 20 państw-miast. Każde z nich jest oznaczone liczbą ze zbioru [1, 20].  
Twoim zadaniem jest **napisanie serwera** biblioteki kompatybilnego z klientem.  

Program klienta dostarczony jest w postaci pliku wykonywalnego. Można go uruchomić przez:

```shell
./client <ADRES SERWERA> <NUMER PORTU>
```

Dostarczony jest też plik wykonywalny przykładowej implementacji serwera.

## Etapy:

1. Zaimplementuj przyłączanie pojedynczego klienta do serwera TCP.
   
   Serwer akceptuje jako argument wejściowy numer portu.
   
   Przykładowe wykonanie serwera:
   
   ```shell
   ./server 8888
   ```
   
   Po uruchomieniu serwer nasłuchuje na przyłączających klientów.
   
   Gdy ustanowione zostanie połączenie z pierwszym klientem, serwer czeka na odbiór 4 bajtów, wypisuje je na stdout, zamyka połączenie, a następnie się kończy.
   
2. Zaimplementuj obsługę wielu klientów i odbieranie danych.
   
   - Po przyłączeniu się nowego klienta, zostaje on dodany do listy aktualnych klientów.
   - Maksymalna liczba klientów to 4.
   - Jeśli przyłączy się maksymalna liczba klientów, należy odrzucać nowe połączenia.
   - Kiedy serwer otrzyma dane o długości 4 bajtów od któregokolwiek klienta, wyświetla je w terminalu.
   
   Do implementacji tego etapu użyj funkcji epoll  
   (alternatywnie można użyć pselect lub ppoll).
   
   > ⚠️ **Uwaga: w tym etapie nie trzeba poprawnie obsługiwać rozłączania klientów.**

3. Dodaj do serwera tablicę zawierającą informacje, które miasto należy do której ze stron konfliktu.
   
   - Na samym początku wszystkie miasta należą do Greków.
   - Za każdym razem, gdy serwer otrzyma wiadomość postaci:
   
   ```
   pXX\n
   ```
   
   lub
   
   ```
   gXX\n
   ```
   
   gdzie `XX` jest dwucyfrową liczbą oznaczającą numer miasta, oznacza to, że miasto `XX` należy do odpowiednio Persów lub Greków.
   
   - Jeśli właściciel miasta się nie zmienił, nie należy podejmować żadnych akcji.
   - Jeśli właściciel miasta się zmienił, należy:
     - zaktualizować tablicę miast,
     - a wiadomość otrzymaną od klienta rozesłać do wszystkich innych połączonych klientów.
   
   Założenia:
   - W tym etapie nadal nie trzeba poprawnie obsługiwać rozłączania klientów.
   - Można też założyć, że przychodzące wiadomości są w poprawnym formacie.
   
4. Po otrzymaniu sygnału `SIGINT`, serwer:
   
   - zamyka wszystkie połączenia,
   - wypisuje na ekran do kogo należy każde z miast,
   - zwalnia zasoby i kończy pracę.
   
   Dodaj poprawną obsługę rozłączania klientów w następujących przypadkach:
   
   - Czytanie z deskryptora danego klienta zwraca wiadomość długości 0.
   - Pisanie do deskryptora danego klienta zgłasza błąd `EPIPE`.
   - Wiadomość otrzymana od klienta nie jest w odpowiednim formacie lub numer miasta nie jest w zakresie [1, 20].
   - Serwer otrzymał C-c.
   
   > ⚠️ **Uwaga: Klienci rozłączający się zwalniają miejsce dla kolejnych, nowych klientów.**

## Client and server executables:

- [sop2l7e3-4.zip](/files/sop2l8e3-4.zip)
