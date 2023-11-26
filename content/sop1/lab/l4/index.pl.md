---
title: "L4 - Synchronizacja"
date: 2022-02-05T19:27:11+01:00
weight: 50
---

# L4 - Synchronizacja

{{< hint info >}}
Uwagi wstępne:

- Szybkie przejrzenie tutoriala prawdopodobnie nic nie pomoże, należy samodzielnie uruchomić programy, sprawdzić jak działają, poczytać materiały dodatkowe takie jak strony man. W trakcie czytania sugeruję wykonywać ćwiczenia a na koniec przykładowe zadanie.
- Na żółtych polach podaję dodatkowe informacje, niebieskie zawierają pytania i ćwiczenia. Pod pytaniami znajdują się odpowiedzi, które staną się widoczne dopiero po kliknięciu. Proszę najpierw spróbować sobie odpowiedzieć na pytanie samemu a dopiero potem sprawdzać odpowiedź.
- Pełne kody do zajęć znajdują się w załącznikach na dole strony. W tekście są tylko te linie kodu, które są konieczne do zrozumienia problemu.
- Materiały i ćwiczenia są ułożone w pewną logiczną całość, czasem do wykonania ćwiczenia konieczny jest stan osiągnięty poprzednim ćwiczeniem dlatego zalecam wykonywanie ćwiczeń w miarę przyswajania materiału.
- Większość ćwiczeń wymaga użycia konsoli poleceń, zazwyczaj zakładam, ze pracujemy w jednym i tym samym katalogu roboczym więc wszystkie potrzebne pliki są "pod ręką" tzn. nie ma potrzeby podawania ścieżek dostępu.
- Czasem podaję znak $ aby podkreślić, że chodzi o polecenie konsolowe, nie piszemy go jednak w konsoli np.: piszę "$make" w konsoli wpisujemy samo "make".
- To co ćwiczymy wróci podczas kolejnych zajęć. Jeśli po zajęciach i teście coś nadal pozostaje niejasne proszę to poćwiczyć a jeśli trzeba dopytać się u prowadzących.
{{< /hint >}}

## The alarm - Semaphores

Napisz wielowątkowy program zegara. Użytkownik wprowadza liczbę sekund, które program ma odmierzyć, a następnie oczekuje na odpowiedź. Program uruchamia osobny wątek dla każdego nowego żądania. Wątek usypia się na określony czas i wypisuje komunikat "Wake up". Następnie wątek kończy działanie.

Program ma ograniczenie 5 równoczesnych wątków. Jeśli program otrzyma więcej żądań, natychmiastowo wyświetli komunikat "Only 5 alarms can be set at the time".

Ograniczenie na współbieżne wątki może być narzucone za pomocą semafora POSIX.

<em>solution <b>prog21.c</b>:</em>
{{< includecode "prog21.c" >}}

Warto samemu dokonać dokładnej analizy powyższego kodu pod kątem elementów nie omawianych tutaj, proszę to zrobić jako ćwiczenie.

Czemu przed uruchomieniem wątku w serwerze alokujemy pamięć? Czy można się tej alokacji pozbyć z kodu?
{{< expand "Answer" >}} Nie możemy użyć jednej wspólnej struktury bo na raz może być więcej niż jeden wątek roboczy. Można zrobić tablice FS_NUM struktur ale  trzeba by wtedy zarządzać wolnymi polami w tej tablicy a to oznacza trochę więcej kodu. Najłatwiej jest zaalokować pamięć dla tej struktury przed uruchomieniem wątku. Pamiętamy o zwolnieniu tej pamięci w wątku zanim się skończy. {{< /expand >}}

Gdzie jest odliczany czas?
{{< expand "Answer" >}} W wątku roboczym, w tym celu go tworzymy. {{< /expand >}}

Czemu służy semafor?
{{< expand "Answer" >}} Semafor odlicza ile jeszcze wątków można utworzyć aby na raz nie działało ich więcej niż FS_NUM (10). Na początek jego wartość to 10, przed utworzeniem kolejnego wątku zmniejszamy wartość semafora. Zanim wątek skończy zwiększa tą wartość. Jeśli wartość semafora jest zerowa to nie da się już utworzyć kolejnego wątku. {{< /expand >}}

Czemu przed utworzeniem kolejnego wątku roboczego w programie używamy sem_trywait? Co się stanie gdy zamienimy to na sem_wait?
{{< expand "Answer" >}} Robimy tak, aby od razu zorientować się, że wyczerpaliśmy limit wątków. Gdyby zastąpić trywait wait'em to program poczekałby aż się zwolni jakiś wątek i dopiero wtedy obsłużyłby zadanie, ale to oznacza, że czas odliczony dla tego zadania byłby dłuższy niż użytkownik żądał! {{< /expand >}}

Co powoduje limitowanie ustawionych alarmów do 5 naraz?
{{< expand "Answer" >}} Semafor, zerknij na 2 powyższe pytania i odpowiedzi. {{< /expand >}}

## Threads pool - conditional variables
Original author: **Jerzy Bartuszek**

Napisz prosty program, który czyta z "/dev/urandom" i zapisuje jego zawartość do plików. Za każdym razem, gdy użytkownik naciśnie Enter, program odczytuje losowe bajty i zapisuje je do pliku. Program jest wielowątkową aplikacją - każde żądanie obsługiwane jest w osobnym wątku. Każdy wątek zapisuje losowe bajty do swojego własnego pliku. Program tworzy wątki z góry określoną liczbę THREADS_NUM i przechowuje nieużywane wątki w puli wątków. Po otrzymaniu sygnału SIGINT serwer przestaje akceptować wejście od użytkownika i kończy swoje wykonanie po obsłużeniu wszystkich bieżących żądań.

<em>solution <b>prog22.c</b>:</em>
{{< includecode "prog22.c" >}}

Warto samemu dokonać dokładnej analizy powyższego kodu pod kątem elementów nie omawianych tutaj, proszę to zrobić jako ćwiczenie.

Czy warunek dla zmiennej warunkowej może być oparty o jedną zwykłą zmienna?
{{< expand "Answer" >}} Tak. {{< /expand >}}

Czy warunek dla zmiennej warunkowej może być oparty na wartościach kilku zmiennych w programie?
{{< expand "Answer" >}} Tak. {{< /expand >}}

Czy warunek dla zmiennej warunkowej może być oparty na zawartości pliku?
{{< expand "Answer" >}} Tak. {{< /expand >}}

Czy warunek dla zmiennej warunkowej może być oparty na istnieniu pliku?
{{{< expand "Answer" >}} Tak. {{< /expand >}}

Jak można ogólnie określić na czym może być oparty warunek zmiennej warunkowej
{{< expand "Answer" >}} Warunek może być absolutnie dowolny, to kod napisany przez programistę o tym decyduję więc granicą tego czym może być warunek jest tylko wyobraźnia kodującego. {{< /expand >}}

Czy zmienna warunkowa może być użyta bez żadnego warunku?
{{< expand "Answer" >}} Tak, jest wtedy zwykłą pulą czekających wątków które możemy wybudzać pojedynczo lub wszystkie naraz. {{< /expand >}}

Co chroni muteks związany z zmienną warunkowa?
{{< expand "Answer" >}} Muteks ma chronić dostęp do tych elementów programu (zmiennych, plików itd.) które są testowane w warunku zmiennej warunkowej. Chodzi o to, aby w czasie sprawdzania warunku zmiennej (zablokowania muteksu) mieć pewność, że warunek ten nie zostanie zmodyfikowany. {{< /expand >}}

Czy jeden muteks może chronić więcej niż jedną zmienna warunkową?
{{< expand "Answer" >}} Może, ale proszę pamiętaj o wydajności programu, takie rozwiązanie ograniczy też równoległość. {{< /expand >}}

Co wchodzi w skład zmiennej warunkowej w powyższym programie?
{{< expand "Answer" >}} Warunek jest oparty wyłącznie o zmienną "condition", do której wątki mają dostęp poprzez wskaźniki. {{< /expand >}}

Jak działa warunek zmiennej w tym programie?
{{< expand "Answer" >}} Gdy główny przyjmie nowe żądanie ustawia zamienną "condition" na 1 i budzi jeden z wątków czekających. Obudzony wątek jeśli widzi, że "condition==1" i obsługuje żądanie. {{< /expand >}}

Który wątek powinien sprawdzić, że warunek zmiennej jest spełniony? Budzący czy obudzony?
{{< expand "Answer" >}} Warunek musi zawsze być sprawdzony przez wątek budzony, nie ma gwarancji że będzie on spełniony nawet jeśli wątek budzący go sprawdzał ponieważ muteks który blokuje zmianę warunku musi być na chwilę zwolniony pomiędzy wysłaniem prośby o obudzenie a samym obudzeniem. Jeśli w tym czasie muteks zostałby przejęty przez inny wątek to warunek może ulec zmianie! Dobrze jeśli wątek budzący sprawdzi warunek ale jeśli nie może tego zrobić bo nie ma dostępu do wszystkich składowych warunku to można ten krok pominąć. {{< /expand >}}

Czemu służy cleanup handler w wątku roboczym?
{{< expand "Answer" >}} Bardzo ważne jest, aby wątek nie zakończył się nie zwolniwszy muteksu chroniącego zmienna warunkową, to zablokowałoby całą aplikację. Ten handler w razie awaryjnego wyjścia zwolni muteks. {{< /expand >}}

## Dice game - barrier

Zasymuluj następującą grę w kości:
Każdy uczestnik jednocześnie rzuca kością sześcienną przez 10 rund. Po każdym rzucie gracza, jeden z graczy posumowuje rundę i przypisuje punkty. Gracz z najwyższym wynikiem w danej rundzie otrzymuje jeden punkt. W przypadku remisu, wszyscy zremisowani gracze otrzymują punkt. Gra kończy się po 10 rundach, a zwycięzcą zostaje gracz z najwyższą sumą punktów.
Reprezentuj każdego gracza za pomocą wątku i użyj bariery do synchronizacji gry.

<em>solution <b>prog23.c</b>:</em>
{{< includecode "prog23.c" >}}

Jak działa bariera w tym programie?
{{< expand "Answer" >}} Jest używana do synchronizacji wątków w dwóch kluczowych punktach w funkcji wątku. Bariera zapewnia, że wszystkie uczestniczące wątki osiągną określony punkt swojego wykonania, zanim którykolwiek z nich wznowi działanie. Bariera jest inicjowana z liczbą PLAYER_COUNT, co oznacza, że zablokuje się, dopóki PLAYER_COUNT wątków nie wywoła pthread_barrier_wait. {{< /expand >}}

Które części funkcji wątku są wywoływane współbieżnie?
{{< expand "Answer" >}} Każdy wątek niezależnie rzuca sześcienną kością, a wyniki są przechowywane w tablicy args->rolls. Ten fragment kodu jest wykonywany równocześnie przez wszystkie wątki. {{< /expand >}}

Jak jest wybierany wątek jednego gracza do posumowania rundy?
{{< expand "Answer" >}} pthread_barrier_wait zwraca PTHREAD_BARRIER_SERIAL_THREAD tylko dla jednego wątku (standard nie określa, który to wątek), a dla innych wątków zwraca 0. Ten mechanizm zapewnia, że akcja jest wykonywana tylko przez jeden wątek w każdej rundzie, uniemożliwiając wielokrotne równoczesne wykonanie tego samego kodu, który powinien być wykonany tylko raz. {{< /expand >}}

## Kody źródłowe z treści tutoriala

{{% codeattachments %}}