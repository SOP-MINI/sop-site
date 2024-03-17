---
title: "L3 - Pamięć dzielona i mmap"
weight: 30
---


## Zadanie mmap

Napisz program obliczający liczbę PI metodą Monte Carlo. Program ma jeden argument - `0 < N < 30` - liczbę procesów obliczeniowych. Każdy proces obliczeniowy wykonuje `100000` iteracji Monte Carlo.

Proces główny mapuje dwa obszary pamięci. Pierwszy obszar służy do współdzielenia wyników obliczeń procesów potomnych. Ma rozmiar N*4 bajtów. Każdy proces potomny zapisuje wynik swoich obliczeń do jednej 4-bajtowej komórki pamięci jako float. Drugi obszar pamięci jest mapowanie pliku `log.txt`. Proces główny ustawia rozmiar tego pliku na `N*8`. Następnie procesy dzieci zapisują tam swoje wyniki końcowe w postaci tekstowej - każdy w jendej linii o szerokości 7 (+ósmy znak to `\n`).

### Rozwiązanie

rozwiązanie `l3-1.c`:
{{< includecode "l3-1.c" >}}


## Zadanie pamięć dzielona i robust mutex

Napisz dwa programy - klienta i serwera. Proces serwera ma jeden parametr - `3 < N <= 20`. Po uruchomieniu najpierw wypisuje komunikat `My PID is: <pid>`. Następnie tworzy segment pamięci dzielonej o nazwie `<pid>-board` i rozmiarze 1024 bajtów. W pamięci dzielonej umieszcza mutex, `N` oraz planszę - tablicę bajtów rozmiaru NxN wypełnioną losowymi liczbami z zakresu `[1,9]`. Następnie co trzy sekundy wypisuje stan planszy. Po otrzymaniu `SIGINT` wypisuje stan planszy po raz ostatni i się kończy.

Program klienta przyjmuje jeden parametr - `PID` serwera. Otwiera obszar pamięci stworzone przez serwer. Następnie wykonuje następującą procedurę:

1. Blokuje mutex
2. Losuje liczbę z zakresu `[1,10]`. W przypadku wylosowani `1` wypisuje `Ops...` i się kończy.
3. Losuje dwie liczby `x` i `y` z zakresu od `0` do `N-1` i wypisuje `trying to search field (x,y)`
4. Sprawdza jaka liczba znajduje się na planszy na polu o współrzędnych `(x,y)`
5. Jeśli nie jest to zero program dodaje tę liczbę do sumy swoich punktów, wypisuje `found <P> points`, odblokowywuje mutex, czeka sekundę i wraca do kroku 1.
6. Jeśli jest to zero program odblokowywuje mutex, wypisuje `GAME OVER: score <X>` (gdzie `X` to zdobyta suma punktów) i się kończy.

### Rozwiązanie

Nowe strony z manuala:
```
man 3p shm_open
man 3p ftruncate
man 3p shm_unlink
man 3p pthread_mutexattr_destroy
man 3p pthread_mutexattr_setpshared
man 3p pthread_mutexattr_setrobust
man 3p pthread_mutex_consistent
```

rozwiązanie `l3-1_server.c`:
{{< includecode "l3-2_server.c" >}}

rozwiązanie `l3-1_client.c`:
{{< includecode "l3-2_client.c" >}}


- Zwróć uwagę w jaki sposób w programie obsługiwane są sygnały. Dzięki użyciu dedykowanego wątku problematyczny kod do obsługi sygnałów jest ograniczony do jednej funkcji i nie ma zmiennych globalnych. Ma to znaczenie zwłaszcza w bardziej złożonych projektach.

## Kody źródłowe z treści tutoriala
{{% codeattachments %}}

## Materiały dodatkowe

- <http://cs341.cs.illinois.edu/coursebook/Ipc#mmap>
