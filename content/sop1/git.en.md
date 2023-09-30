---
title: "GIT version control system"
date: 2022-02-10T10:00:00+01:00
draft: false
layout: single
menu: sop1
---

## GIT - system kontroli wersji rodem z GNU Linux

Taki system ma za zadanie ułatwić programistom (ale nie tylko) śledzić zmiany w plikach oraz ułatwiać przez to współpracę.
W przypadku naszych laboratoriów ma on za zadanie uporządkować oddawane przez ciebie kody z laboratorium.

Na laboratoriach zapoznamy się z podstawami systemu kontroli wersji GIT.
Powstał on przy rozwijaniu kodu jądra Linuxa i od tamtego czasu rozpowszechnił się jako jeden z najbardziej popularnych systemów tego typu.
Mimo powierzchownego skomplikowania posiada on wiele przydatnych cech.

## Podstawowe działanie GITa - co znaczy śledzić plik?

Wyobraźmy sobie, że dostaliśmy jakiś kod wykonany przez innego programistę.
Naszym zadaniem jest poprawić jakiś problem w tym kodzie.
Bez systemu kontroli wersji pewnie wykonalibyśmy kopię tego pliku i rozpoczęli edycję właśnie tej kopii.
Jak już zmiana byłaby gotowa i przetestowana naturalnym krokiem będzie podmiana oryginalnego pliku tym nowym.
W taki sposób, gdy coś popsujemy, nadal będzie można wrócić do oryginalnego kodu i zacząć od zmiany od nowa.

Natomiast w przypadku systemu kontroli wersji dostajemy takie zabezpieczenie i więcej.
Gdy jakiś plik jest śledzony przez GITa, to w każdej chwili można takie przywrócenie wykonać.
Co lepsze, można zobaczyć dokładnie wykonane zmiany w postaci pliku różnicowego, potocznie zwanego *diffem*.
Gdy zdecydujemy, że kod jest gotowy, to zatwierdzamy zmiany wykonując *commit*.
Na tym bonusy się nie kończą.
Przez to, jak działa wyznaczanie różnic między plikami, możemy w każdej chwili cofnąć się do poprzedniej **wersji** jednym poleceniem.
Wszystkie nasze zmiany zostaną w historii.

## Podstawowe działanie GITa - co znaczy śledzić zmiany?

Teraz gdy wiemy, że GIT może śledzić zmiany na poziomie pliku przez pliki różnicowe, to zastanówmy się, co można z tym zrobić.
Spróbujmy teraz wykonać kolejną zmianę na pliku.
Znowu wykonamy *commit*, gdy wszystko będzie gotowe i dostajemy kolejny krok.
Możemy tak zrobić jeszcze raz i jeszcze raz.
Gdy mamy wiele zmian i z każdej można przejść do wcześniejszej, to można z naszego mocno zmodyfikowanego pliku wrócić do oryginalnego.
Wtedy uzyskujemy całą historię zmian z jednoczesną możliwością wybrania dowolnej wersji, jeśli przyjdzie taka potrzeba.

## Rozproszony system kontroli wersji - co to znaczy?

W przypadku pracy z wieloma programistami chcielibyśmy współdzielić kod, nad którym pracujemy.
Zapewne do tego celu będzie istnieć jakiś serwer przechowujący aktualny kod projektu.
W przypadku GITa każdy programista będzie na swoim komputerze posiadać lokalną kopię **wszystkich** tam składowanych plików i ich historii zmian.
Taki zestaw plików nazywamy *repozytorium*, a serwer *remotem*.

Intuicyjnie wydaje się, że marnujemy przestrzeń dyskową i czas wykonując takie kopie.
Jednak to było bardzo rozsądne rozwiązanie w połowie lat 90, jak i teraz.
Jako programista mamy możliwość pracy bez aktywnego łącza internetowego na naszej lokalnej kopii.
Dopiero gdy będziemy chcieli się podzielić naszym kodem, zsynchronizujemy się ze zdalnym repozytorium.
Taką lokalną kopię nazywamy potocznie *localem*

## Jak używać GITa?

Po wstępie teoretycznym wykonajmy proste ćwiczenie.
Stworzymy lokalne repozytorium z prostym kodem i dokonamy w nim zmian.
Na początku trzeba sprawdzić, czy GIT jest zainstalowany na twoim komputerze.

```shell
$ git
``` 

Stwórzmy folder, w którym umieścimy lokalne repozytorium GITa.
Będąc w środku folderu, wykonaj następujące polecenie.

```shell
$ git init
``` 

To polecenie powinno utworzyć w folderze o nazwie `.git`.
Tam znajdują się wszystkie dane wykorzystywane przez GITa do śledzenia zmian.
Stwórzmy teraz plik `hello.c`, który będzie zawierać minimalny przykład z laboratorium zerowego. 

**hello.c**:
```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    printf("Hello world\n");
    return EXIT_SUCCESS;
}
```

To będzie nasz plik, który chcemy śledzić.
GIT nie śledzi każdego pliku znalezionego w folderze z repozytorium.
Skompilujmy teraz nasz minimalny przykład przy pomocy `gcc`.

```shell
$ gcc -o hello hello.c
``` 

W tym momencie mamy teraz dwa pliki w folderze.
Przez to, że plik `hello` można w każdej chwili wygenerować z pliku `hello.c`, nie chcemy śledzić jego zmian.
Jest ku temu kilka dobrych powodów:
- Pamiętajmy, że każdy programista musi wykonać kopię całego repozytorium przed rozpoczęciem pracy. Z tego powodu dobrą praktyką jest minimalizacja śledzonych plików w repozytorium.
- GIT wykorzystuje funkcjonalności programów `diff` i `patch`, które działają dobrze tylko z plikami tekstowymi. Śledzenie plików binarnych oznacza wykonywanie kopii i podmienianie jej z wersji na wersje.
Sprawdźmy, jak wygląda nasze repozytorium.
Do tego celu służy polecenie
```shell
$ git status
On branch master

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
	hello
	hello.c

nothing added to commit but untracked files present (use "git add" to track)
``` 
Możemy zobaczyć, że nasze dwa pliki nie są śledzone.
Dodajmy teraz nasz plik `hello.c` do śledzonych poleceniem

```shell
$ git add hello.c
```

To polecenie zrobiło dwie rzeczy: dodało plik `hello.c` do śledzonych plików oraz dodało ten sam plik do poczekalni (ang. *stage*).
Po sprawdzeniu statusu zobaczymy, że w kategorii śledzonych zmian pojawił się dodany plik.
Zapiszmy teraz stan tego pliku wykonując

```shell
$ git commit -m "Add hello.c to repository"
```

Powyższe polecenie zapisało stan plików z poczekalni w repozytorium.
Teraz GIT zapamiętał plik w tym stanie w swojej historii.
Parametr `-m` służy do przekazania wiadomości do zmiany.
Aby zobaczyć listę wszystkich zmian w repozytorium, należy wykonać polecenie

```shell
$ git log
```


## Dodatkowe materiały

Powyższy opis pokrywa tylko podstawowe funkcje GITa.
Nie został tutaj poruszony tutaj temat gałęzi, łączenia zmian czy rozwiązywania konfliktów.
W praktyce są to bardzo popularne problemy, jednak zbyt złożone do opisania tutaj.
Ta [książka](https://git-scm.com/book/en/v2) jest bardzo dobrym źródłem do zapoznania się z wieloma funkcjami GITa.
Polecam szczególnie rozdział drugi jako uzupełnienie tego opisu.
