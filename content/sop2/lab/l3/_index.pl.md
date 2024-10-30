---
title: "L3 - Pamięć dzielona i mmap"
weight: 30
---

# Pamięc dzielona i mmap

{{< hint info >}}

Jest to pierwszy z serii tutoriali w "nowej formule". Zawiera więcej wyjaśnień działania użytych funkcji oraz ich parametrów. Mamy nadzieję, że w ten sposób będzie wam łatwiej "wejść w temat". Jest to jednak wciąż zbiór jedynie poglądowy najważniejszych informacji - należy **koniecznie przeczytać wskazane strony manuala** aby dobrze poznać i zrozumieć wszystkie szczegóły.

{{< /hint >}}


## mmap

Na poprzednich zajęciach omówiliśmy już kilka sposobów synchronizacji oraz współdzielenia danych między procesami. Wszystkie jednak wydają się być zawsze bardziej skomplikowane, niż jest to w przypadku wątków - gdzie możemy po prostu mieć współdzielone zmienne. Gdy jednak używamy funkcji `fork()` procesy potomne otrzymują własną kopią wszystkich danych procesu rodzica. Tak więc, jeśli np. w procesie dziecka zmodyfikujemy tablicę stworzoną przed wywołaniem `fork()`, zmiany te zajdą jedynie w obrębie procesu dziecka.

Możliwe jest jednak zadeklarowanie tak zwanego "mapowania" między pamięcią programu a tak zwanym "obiektem pamięci" (ang. "memory object"). Wszelkie zmiany zrobione w tak zmapowanym obszarze zostaną przeniesione na wspomniany "obiekt pamięci", który jest zarządzany przez system operacyjny. Do stworzenia takiego mapowaniu służy polecenie `mmap` (`man 3p mmap`, `man 2 mmap`).

Funkcja ta ma z naszego punktu widzenia kilka głównych zastosowań:

- Pozwala stworzyć mapowanie między fragmentem pamięci a plikiem - dzięki temu możemy w wygodny sposób modyfikować zawartość pliku, tak jak zwykłą tablicę.
- Stworzyć obszar pamięci współdzielony z procesami potomnymi.
- Zmapować obszar pamięci dzielonej (o tym w drugiej części tego tutoriala).

Przyjrzyjmy się sygnaturze i parametrom tej funkcji:

```
void *mmap(void *addr, size_t len, int prot, int flags,
           int fildes, off_t off);
```

- `addr` - ten parametr pozwala poinformować system operacyjny, jaki obszar pamięci chcemy zmapować (po stronie systemu). W naszych zastosowaniach zwykle przekazujemy tutaj po prostu `NULL` żeby system sam o tym zdecydował.
- `len` - oznacza oczywiście wielkość pamięci
- `prot` - oznacza operacje (odczyt/zapis/wykonanie - patrz manual) jakie będziemy wykonywać na pamięci - trochę jak w przypadku plików.
- `flags` - ten parametr determinuje rodzaj mapowania stworzonego przez `mmap`. Poniżej opisane zostały najważniejsze flagi, koniecznie przeczytaj `man 2 mmap` żeby zapoznać się z pełną listą oraz szczegółowymi informacjami.
  - `MAP_ANONYMOUS` - oznacza, że nie mapujemy pliku a tworzymy nowy, anonimowy obszar pamięci. W takiej sytuacji parametr `filedes` powinien być ustawiony na `-1`. W przypadku, gdy nie użyjemy tej flagi funkcja działa domyślnie w trybie mapowania pliku.
  - `MAP_SHARED` - oznacza, że mapowanie jest współdzielone z innymi procesami. W naszym przypadku prawie zawsze chcemy użyć tej flagi.
  - `MAP_PRIVATE` - przeciwieństwo powyższej flagi - mapowanie nie jest współdzielone, pamięć zostanie skopiowana w procesach potomnych. Mało przydatne do naszych zastosowań, ale również ważne. Czy nie zastanawiało cię kiedyś jak działa `malloc`?
- `filedes` - deskryptor pliku, który chcemy zmapować.
- `off` - offset w pliku, który chcemy zmapować. Przy `MAP_ANONYMOUS` powinien zawsze być ustawiony na 0.

To tak naprawdę tyle - wskaźnik na pamięć otrzymany z wywołania `mmap` z flagą `MAP_SHARED` będzie mógł być użyty w dowolnym procesie potomnym a zmiany będą widoczne we wszystkich pozostałych procesach. Ważnym zagadnieniem jest jednak zadbanie o synchronizację albo o to, żeby każdy proces działał tylko w ściśle wyznaczonym dla siebie obszarze wspólnej pamięci.

Z `mmap` wiążą się jeszcze dwie funkcje:

- `msync` - przydatna przy mapowaniu plików, pozwala wymusić na systemie operacyjnym wykonanie modyfikacji mapowanego pliku. Ze względu na wydajność (modyfikacje plików na dysku są wolne) w momencie gdy zmodyfikujemy fragment pamięci zmapowanej do pliku, system od razu nie odzwierciedli tych zmian. Inaczej tworzyłoby to bardzo wiele bardzo małych modyfikacji, co bardzo spowolniłoby działanie systemu. Jeśli chcemy mieć pewność, że zmiany zostaną odwzorowane należy użyć funkcji `msync`. Można o tym myśleć, jako o funkcji analogicznej do `fflush` dla strumieni. Patrz `man 3p msync`.
- `munmap` - zmapowaną pamięć trzeba kiedyś "odmapować" - służy do tego to polecenie - patrz `man 3p munmap`. Warto wspomnieć, że przy zmapowanych plikach `munmap` nie gwarantuje synchronizacji - konieczne jest wywołanie wcześniej `msync`.


### Zadanie

Napisz program obliczający liczbę PI metodą Monte Carlo. Program ma jeden argument - `0 < N < 30` - liczbę procesów obliczeniowych. Każdy proces obliczeniowy wykonuje `100 000` iteracji Monte Carlo.

Proces główny mapuje dwa obszary pamięci. Pierwszy obszar służy do współdzielenia wyników obliczeń procesów potomnych. Ma rozmiar N*4 bajtów. Każdy proces potomny zapisuje wynik swoich obliczeń do jednej 4-bajtowej komórki pamięci jako float. Drugi obszar pamięci jest mapowanie pliku `log.txt`. Proces główny ustawia rozmiar tego pliku na `N*8`. Następnie procesy dzieci zapisują tam swoje wyniki końcowe w postaci tekstowej - każdy w jendej linii o szerokości 7 (+ósmy znak to `\n`).

### Rozwiązanie zadania

Nowe strony z manuala:
```
man 3p mmap
man 2 mmap
man 3p munmap
man 3p msync
man 0p sys_mman.h
man 3p ftruncate
```

rozwiązanie `l3-1.c`:
{{< includecode "l3-1.c" >}}

### Uwagi i pytania

- Czemu w zadaniu nie występuje żadna synchronizacja?
{{< answer >}} Każdy z procesów potomnych piszę do własnego wydzielonego fragmentu pamięci dzielonej a dopiero rodzic na końcu łączy wyniki. Z tego powodu nie występuje  żaden konflikt wymagający synchronizacji. {{< /answer >}}

- Do czego konieczne jest wywołanie `ftruncate`?
{{< answer >}} Bez tego wywołania otwarty plik `log.txt` miałby rozmiar 0, ponieważ jest otwierany z flagą `O_TRUNC`, która powoduje skasowanie jego zawartości. Z kolei bez tej flagi rozmiar zależałby od poprzedniej zawartości pliku, byłoby więc to nieprzewidywalne. {{< /answer >}}

## Pamięć dzielona i robust mutex

### shm_*

W przykładzie wyżej dzięki `mmap` tworzyliśmy obszar pamięci, który współdzieliła ze sobą grupa procesów stworzona przez `fork()`. Co jednak, gdy procesy są od siebie niezależne? W takiej sytuacji możemy stworzyć nazwany obiekt pamięci dzielonej, która każdy proces może zmapować po unikalnej nazwie. Jest więc to rzecz bardzo podobna i analogiczna w różnicach co pipe i FIFO omawiane na L1.

Do tworzenia nazwanego obiektu pamięci dzielonej służy funkcja `shm_open` (patrz `man 3p shm_open`). Jej użycie jest bardzo proste, a parametry analogiczne do zwykłego `open` albo `mq_open`, tak więc po prostu przeczytaj stronę manuala.

`shm_open` zwróci deskryptor, zachowujący się bardzo podobnie co deskryptor pliku. Na początku stworzona pamięć ma zerowy rozmiar, dlatego należy ją powiększyć używająć np. `ftruncate`. Następnie wystarczy użyć funkcji `mmap`. 

Gdy wszystkie procesy skończą działać z pamięcią dzieloną należy ją usunąć funkcją `shm_unlink`, co również jest bardzo proste, więc po prostu sprawdź `man 3p shm_unlink`.

### Współdzielenie obiektów synchronizacji i robust mutex

Mając współdzieloną pamięć możemy synchronizować procesom dostęp do niej (albo wykonanie innych operacji) przy pomocy poznanych już pipe-ów, fifo i kolejek komunikatów. Możliwe jest jednak również umieszczenie semaforów oraz mutexów w pamięci dzielonej tak, aby wszystkie procesy miały do nich dostęp.

W przypadku semaforów sprawa jest prosta. Zobacz `man 3p sem_init` - jeśli drugi parametr, `pshared`, zostanie ustawiony na wartość inną niż zero, semafor może być dzielony między procesami. Tak więc, wystarczy wybrać pewien obszar pamięci dzielonej o rozmiarze `sizeof(sem_t)` i zainicjalizować tam semafor w ten sposób a wszystkie procesy będą mogły z niego korzystać.

Nieco bardziej skomplikowane jest współdzielenie mutexów. Ustawianie specjalnych właściwości mutexa również odbywa się przy jego inicjalizacji (`pthread_mutex_init`, ale patrz `man 3p pthread_mutex_destroy`) - jako drugi parametr możemy podać specjalne atrybuty - `pthread_mutexattr_t`. Tak więc, żeby mutex mógł być współdzielony między procesami należy stworzyć obiekt atrybutów mutexa (`pthread_mutexattr_init`) a następnie wywołać na tak zainicjalizowanych atrybutach `pthread_mutexattr_setpshared` (patrz `man 3p pthread_mutexattr_getpshared`). Następnie wystarczy przekazać te atrybuty do `pthread_mutex_init`.

Stworzony w ten sposób mutex będzie mógł być poprawnie współdzielony między procesami, to jednak nie koniec problemów. Wciąż bowiem obowiązuje nas podstawowe ograniczenie - mutex musi zostać odblokowany w tym samym wątku, który go zablokował. Stwarza to problem, gdy w wyniku błędu albo awarii proces zablokuje mutex i zakończy się przed jego odblokowaniem. Mutex znajdzie się wtedy w niepoprawnym stanie. Aby zaradzić takim sytuacjom należy stworzyć tzw. "niezawodny mutex" (ang. "robust mutex").

Tworzenie "robust mutexa" polega na dodaniu mu odpowiedniego atrybytu przy użyciu `pthread_mutexattr_setrobust` (patrz `man 3p pthread_mutexattr_getrobust`) - podobnie jak to było ze współdzieleniem. Gdy spróbujemy zablokować mutex (`pthread_mutex_lock`), który jest w niepoprawnym stanie (został zablokowany przez inny proces, który zakończył się i go nie odblokował) otrzymamy błąd `EOWNERDEAD` (patrz `man 3p pthread_mutex_lock`) - ale mutex zostanie mimo wszystko **zablokowany**. Żeby naprawić stan mutexu należy dodatkowo wywołać funkcję `pthread_mutex_consistent` (`man 3p pthread_mutex_consistent`).

Podobnie jak mutex inicjalizowane są równie zmienne warunkowe i bariery. One również mogą być współdzielone między procesami, dzięki ustawieniu odpowiednich atrybutów. Zobacz `man 3p pthread_condattr_getpshared` i `man 3p pthread_barrierattr_getpshared`. W przypadku tych obiektów na szczęście nie występują dodatkowe problemy, które trzeba rozwiązać, tak jak przy mutexie.

Zobacz, jak współdzielona pamięć oraz robust mutex zostały wykorzystane w poniższym zadaniu.

### Zadanie

Napisz dwa programy - klienta i serwera. Proces serwera ma jeden parametr - `3 < N <= 20`. Po uruchomieniu najpierw wypisuje komunikat `My PID is: <pid>`. Następnie tworzy segment pamięci dzielonej o nazwie `<pid>-board` i rozmiarze 1024 bajtów. W pamięci dzielonej umieszcza mutex, `N` oraz planszę - tablicę bajtów rozmiaru NxN wypełnioną losowymi liczbami z zakresu `[1,9]`. Następnie co trzy sekundy wypisuje stan planszy. Po otrzymaniu `SIGINT` wypisuje stan planszy po raz ostatni i się kończy.

Program klienta przyjmuje jeden parametr - `PID` serwera. Otwiera obszar pamięci stworzone przez serwer. Następnie wykonuje następującą procedurę:

1. Blokuje mutex
2. Losuje liczbę z zakresu `[1,10]`. W przypadku wylosowani `1` wypisuje `Ops...` i się kończy.
3. Losuje dwie liczby `x` i `y` z zakresu od `0` do `N-1` i wypisuje `trying to search field (x,y)`
4. Sprawdza jaka liczba znajduje się na planszy na polu o współrzędnych `(x,y)`
5. Jeśli nie jest to zero program dodaje tę liczbę do sumy swoich punktów, wypisuje `found <P> points`, zeruje to pole, odblokowywuje mutex, czeka sekundę i wraca do kroku 1.
6. Jeśli jest to zero program odblokowywuje mutex, wypisuje `GAME OVER: score <X>` (gdzie `X` to zdobyta suma punktów) i się kończy.

### Rozwiązanie zadania

Nowe strony z manuala:
```
man 3p shm_open
man 3p shm_unlink
man 3p pthread_mutexattr_destroy
man 3p pthread_mutexattr_setpshared
man 3p pthread_mutexattr_setrobust
man 3p pthread_mutex_consistent
```

rozwiązanie `l3-2_server.c`:
{{< includecode "l3-2_server.c" >}}

rozwiązanie `l3-2_client.c`:
{{< includecode "l3-2_client.c" >}}

### Uwagi i pytania

- Zwróć uwagę w jaki sposób w programie obsługiwane są sygnały. Dzięki użyciu dedykowanego wątku problematyczny kod do obsługi sygnałów jest ograniczony do jednej funkcji, nie ma zmiennych globalnych i nie musimy używać wszędzie `TEMP_FAILURE_RETRY`. Ma to znaczenie zwłaszcza w bardziej złożonych projektach.

- Czy gdyby nie nasze losowe zamykanie procesu potomnego po wylosowaniu 1 robust mutex byłby potrzebny?
{{< answer >}} Tak, proces i tak mógłby się zakończyć w tym miejscu, np. z powodu otrzymania sygnału `SIGKILL`. Generalnie przy współdzieleniu mutexu z innymi procesami, najlepiej żeby zawsze był to robust mutex.  {{< /answer >}}

- Czy robust mutex sprawia, że nie może dojśc do deadlocka z powodu błędu w jednym z procesów?
{{< answer >}} Niestety nie. Proces wciąż może np. nie zwalniać mutexa albo mieć inny błąd. Należy zwrócić szczególną uwagę na sytuację, w której proces robiłby `unmap` przed zwolnieniem mutexu. W takiej sytuacji mechanizm "robustness" nie zadziała! {{< /answer >}}

- Do czego konieczne jest wywołanie `ftruncate`?
{{< answer >}} Stworzony obiekt pamięci dzielonej ma na początku rozmiar 0 i musi być powiększony. {{< /answer >}}



## Nazwany semafor

Nazwane semafory były już wspomniane na SOP1, jednak warto je powtórzyć. Nazwany semafor ma się do zwykłego semafora podobnie jak fifo do pipe. Tworzymy go przy pomocy funkcji `sem_open` (patrz `man 3p sem_open`) przekazując flagę `O_CREAT`, uprawnienia oraz początkową wartość, np:

```
sem_t *semaphore = sem_open("SOP-semaphore", O_CREAT, 0666, 5);
```

utworzy semafor o nazwie `SOP-semaphore` i zainicjalizuje go wartością 5. Wykonanie potem tego samego wywołania (albo prostszego, bez `O_CREATE`: `sem_open("SOP-semaphore", 0);`) w innym procesie zwróci ten sam, już istniejący semafor. Jak widać jest to zupełnie analogiczne do fifo albo kolejek komunikatów.

Nazwane semafory dzięki swojej prostocie są wygodne w użyciu do zapewniania synchronizacji dla procesów korzystających z pamięci dzielonej. Gdy kilka niezależnych procesów zamierza korzystać z jednego obiektu pamięci dzielonej tylko jeden będzie ją inicjalizował. Istotne jest więc, żeby pozostałe procesy wtedy czekały, aż inicjalizacja zostanie zakończona - co można właśnie prosto zrealizować przy użyciu semafora.


## Kody źródłowe z treści tutoriala
{{% codeattachments %}}

## Materiały dodatkowe

- Spróbuj rozwiązać przykładowe [zadanie z L1]({{% ref "/sop2/lab/l1/example2" %}}) używając pamięci dzielonej. Zamiast wysyłania wiadomości o nowej rundzie synchronizacja może odbywać się przy pomocy semafora (rodzic wywołuje `sem_post` odpowiednią ilość razy na początku rundy). Przetestuj semafor współdzielony w pamięci dzielonej oraz nazwany semafor otwierany niezależnie przez procesy potomne. Zamiast wysyłania kart przez pipy gracze mogą umieszczać je w odpowiednim obszarze pamięci dzielonej. Do zasygnalizowania serwerowi, że gracze wystawili już swoje kart możesz użyć współdzielonej bariery albo ponownie semafora.
- <http://cs341.cs.illinois.edu/coursebook/Ipc#mmap>
