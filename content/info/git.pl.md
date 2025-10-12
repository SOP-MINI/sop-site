---
title: "System kontroli wersji GIT"
weight: 20
---

{{< hint info >}}

Ta strona zawiera ogólne informacje o systemie GIT.
Po jej przeczytaniu przejżyj też stronę [Użycie GITa w czasie laboratorium]({{< ref "git-on-lab" >}}).

{{< /hint >}}


## GIT - przykład rozproszonego systemu kontroli wersji

System kontroli wersji ma za zadanie ułatwić programistom (ale nie tylko) śledzić zmiany w plikach oraz ułatwiać przez to współpracę.
W przypadku naszych laboratoriów ma on za zadanie uporządkować oddawane kody zadań przez studentów.
Na laboratoriach zapoznamy się z podstawami systemu kontroli wersji GIT.

## Podstawowe działanie GITa - śledzenie pliku

Wyobraźmy sobie, że dostaliśmy jakiś kod wykonany przez innego programistę.
Naszym zadaniem jest poprawić jakiś problem w tym kodzie.
Bez systemu kontroli wersji wykonalibyśmy kopię tego pliku i rozpoczęli edycję tej kopii.
Kiedy zmiana byłaby gotowa i przetestowana naturalnym krokiem będzie podmiana oryginalnego pliku zmienionym plikiem.
W taki sposób, gdy zmiana będzie błędna, nadal można wrócić do oryginalnego kodu.

Natomiast w przypadku użycia systemu kontroli wersji dostajemy identyczne zabezpieczenie i więcej.
Gdy jakiś plik jest śledzony przez GITa, w każdej chwili można takie przywrócenie wykonać.
Dodatkowo można zobaczyć dokładnie wykonane zmiany w postaci pliku różnicowego, potocznie zwanego *diffem*.
Gdy zdecydujemy, że kod jest gotowy, to zatwierdzamy zmiany wykonując *commit*.
Przez to, jak działa wyznaczanie różnic między plikami, możemy w każdej chwili cofnąć się do poprzedniej **wersji** jednym poleceniem.
Wszystkie nasze zmiany zostaną w historii.

## Podstawowe działanie GITa - śledzenie zmian

GIT fundamentalnie przechowuje zmiany plików w postaci plików różnicowych.
Każda zmiana na śledzonym pliku w repozytorium jest reprezentowana przez jeden taki plik.
Pamiętając kolejność stosowania plików różnicowy, możemy uzyskać dowolny stan takiego pliku.
Wtedy uzyskujemy całą historię zmian z jednoczesną możliwością wybrania dowolnej wersji, jeśli przyjdzie taka potrzeba.

## Rozproszony system kontroli wersji

W przypadku pracy z wieloma programistami chcielibyśmy współdzielić kod, nad którym pracujemy.
Do tego celu najczęściej istnieje serwer przechowujący aktualny kod projektu.
W przypadku GITa każdy programista będzie na swoim komputerze posiadać lokalną kopię **wszystkich** tam składowanych plików i ich historii zmian.
Taki zestaw plików nazywamy *repozytorium*, a serwer *remotem*.

Jako programista mamy możliwość pracy na lokalnej kopii bez aktywnego łącza internetowego.
Dopiero gdy będziemy chcieli się podzielić naszym kodem, zsynchronizujemy go ze zdalnym repozytorium.
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

## Wypychanie zmian na zdalny serwer

Jak było wspomniane na początku GIT jest używany do ułatwienia współpracy między programistami. W związku z tym repozytorium rzadko jest tylko lokalne - ważną funkcjonalnością jest możliwość "wypychania" (ang. "push") swoich zmian tak, żeby inni mogli je zobaczyć. Jest to możliwe za pośrednictwem różnych metod i protokołów, jednak w ostatnim czasie największą popularność zdobyły tak zwane "kuźnie oprogramowania" (ang. "source forge") - serwerów trzymających repozytoria GITa oraz umożliwiających zarządzanie nimi przy użyciu wygodnego interfejsu webowego. Popularne strony to np. [GitHub](https://github.com/), [GitLab](https://gitlab.com/) czy [CodeBerg](https://codeberg.org/), ale jest ich [o wiele więcej](https://en.wikipedia.org/wiki/Comparison_of_source-code-hosting_facilities).

Na laboratoriach z SOPów używamy własnego systemu - jednak jest on włączany tylko w trakcie zajęć. Wciąż warto przećwiczyć wypychanie zmian używając jednej z ogólnodostępnych i darmowych stron. W tym celu najpierw załóż konto (lub zaloguj się, jeżeli już masz) na wybranej przez siebie stronie oraz stwórz nowe repozytorium na swoim koncie. Szczegóły różnią się między różnymi oprogramowaniami, jednak najczęściej tworzone jest puste repozytorium a strona wyświetla instrukcję jak podpiąć nasze lokalne repozytorium na komputerze do zdalnego (ang. "remote"). Przykładowy wygląd strony pustego zdalnego repozytorium:

![](/img/empty-repo.png)

Jak widać strona udostępnia dwie instrukcje. Pierwsza dotyczy nowego repozytorium i pokrywa się w dużej części tym, co dopiero przećwiczyliśmy. Na taką okazję jest właśnie przygotowana druga instrukcja. Nowym elementem w obu jest ustawianie adresu zdalnego repozytorium komendą `git remote` oraz wypychanie swoich zmian (commitów) komendą `git push`. To, co powinno wystarczyć to użycie:

```shell
$ git remote add origin <adres zdalnego repozytorium>
```
ta komenda dodaje do repozytorium nowy "remote" o nazwie `origin` (jest to konwencjonalnie nazwa "głównego" remote-a z którym pracujemy) i podanym adresie. Następnie wystarczy już napisać tylko:

```shell
$ git push
```
aby wypchnąć commity z aktualnej gałązi na domyślne zdalne repozytorium.

## Dodatkowe materiały

Powyższy opis pokrywa tylko podstawowe funkcje GITa.
Nie został tutaj poruszony temat gałęzi, łączenia zmian czy rozwiązywania konfliktów.
W praktyce są to bardzo popularne problemy, jednak zbyt złożone do opisania tutaj.
Ta [książka](https://git-scm.com/book/en/v2) jest bardzo dobrym źródłem do zapoznania się z wieloma funkcjami GITa.
Polecamy szczególnie rozdział drugi jako uzupełnienie tego opisu.


{{< hint info >}}

Ta strona zawiera ogólne informacje o systemie GIT.
Po jej przeczytaniu zobacz też [Użycie GITa w czasie laboratorium]({{< ref "sop1/lab/l0#schemat-użycia-gita-na-laboratorium" >}}).

{{< /hint >}}
