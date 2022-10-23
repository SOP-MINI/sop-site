---
title: "L0 - Wstęp"
date: 2022-02-05T17:26:02+01:00
weight: 10
---

# Laboratorium wstępne do zajęć z SOP1

{{< hint info >}}

Te pierwsze zajęcia nie wymagają od Ciebie dużego przygotowania,
mają służyć wyjaśnieniu wszelkich wątpliwości co do zasad prowadzenia zajęć i harmonogramu.
Przeczytaj proszę informacje podane w działach [program]({{< ref "../../syllabus" >}}),
[regulamin]({{< ref "../../zasady" >}}) i [harmonogram]({{< ref "../../harmonogram" >}}).
Zerknij też na materiały [pomocnicze]({{< ref "../../materialy" >}}).

W dziale z [materiałami]({{< ref "../../materialy" >}}) znajdziesz tutoriale do samodzielnego wykonania
w ramach przygotowania do kolejnych zajęć. Podczas punktowanych zajęć zawsze poświęcamy
od 15 do 30 minut na odpowiadanie na pytania od studentów dotyczące danego działu.
Można też się skonsultować droga mailową lub osobiście w czasie dyżurów prowadzących laboratoria.

W ramach przygotowania do kolejnych zajęć zrobimy proste ćwiczenie symulujące pracę na punktowanych laboratoriach.

{{< /hint >}}

### Zadanie

Cel: Napisać trywialny program "hello world", skompilować go, uruchomić, spakować i przesłać na wskazane konto. 
*Co student musi wiedzieć:*
- znać dowolne środowisko programisty w systemie Linux
- znać podstawowe flagi polecenia `gcc`
- umieć posługiwać się konsolą
- umieć pakować pliki poleceniem `tar`
- umieć kopiować pliki poleceniem `cp`

*rozwiązanie **prog1.c**:*

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    printf("Hello world\n");
    return EXIT_SUCCESS;
}
```

*kompilacja:*

```shell
gcc -Wall -fsanitize=address,undefined -o prog1 prog1.c
```

Użycie flagi `-Wall` jest obowiązkowe w trakcie laboratorium, 
program nie powinien generować istotnych ostrzeżeń. 
Każde zadanie wymaga również użycia odpowiedniego zestawu opcji `-fsanitize`. 
[Więcej informacji o sanitizerach.]({{< ref "../sanitizers" >}})
{{< hint danger >}}
**UWAGA** nie wolno dowolnie mieszać parametrów polecenia `gcc`, 
parametr `-o` ma argument -- ścieżkę do wynikowego programu.
{{< /hint >}}

*uruchomienie:*

```shell
./prog1
```

*spakowanie i sprawdzenie:*

```shell
tar -cjf $USER.tar.bz2 prog1.c
tar -tjf $USER.tar.bz2
```

{{< hint info >}}
Można podać wiele plików lub katalogów do spakowania. Na przykład:
```shell
tar -cjf $USER.tar.bz2 file1.c file2.c
```
utworzy archiwum składające się z dwóch plików: `file1.c` oraz `file2.c`. Oczywiście mogą Państwo również spakować cały katalog.
{{< /hint >}}

{{< hint danger >}}
**UWAGA** nie wolno dowolnie mieszać parametrów polecenia `tar`, 
parametr `-f` ma argument -- ścieżkę do archiwum.
{{< /hint >}}

{{< hint danger >}}
**UWAGA** nie wolno dowolnie mieszać argumentów polecenia `tar`, np. podanie:

~~`tar -cjf prog1.c $USER.tar.bz2`~~

spowoduje skasowanie pliku z kodem źródłowym! Nazwa archiwum zawsze powinna być bezpośrednio za parametrem `-f`.
{{< /hint >}}

*wgranie pracy ze sprawdzeniem:*
```shell
cp $USER.tar.bz2 {{< teacherdir >}}
ls -l {{< teacherdir >}}$USER.tar.bz2
```

{{< hint info >}}
Katalog do którego wgrywacie prace może być inny niż ten przykładowy, 
będzie to podane w zadaniu.
{{< /hint >}}

### Przygotowanie środowiska

W przeciwieństwie do poprzednich przedmiotów nie wymagamy żadnego konkretnego środowiska.
Niemniej jednak odpowiedni edytor powinien potrafić:

- wyświetlać błędy kompilacji na żywo wraz z kodem (co powoduje, że dowiadujemy się o nich znacznie szybciej, jak
  również pozwala nam znacznie łatwiej znaleźć odpowiedni fragment kodu),
- podpowiadać nazwy funkcji (przydatne przy wpisywaniu dłuższych nazw).

Przykładowym edytorem, który dobrze się sprawdzi, jest Visual Studio Code (nie mylić z Visual Studio), który jest
częstym wyborem ze względu na niski próg wejścia. Innymi częstymi wyborami są Emacs oraz Vim (oraz niedostępny domyślnie
w laboratorium Neovim), które jednak cechują się wyższym progiem wejścia. Wszystkie wymienione edytory wymagają
dodatkowej konfiguracji, którą warto przeprowadzić przed pierwszym punktowanym laboratorium. Oczywiście warto, żeby
konfiguracja edytora w domu była podobna do tej laboratorium.

Nie zalecamy korzystania z większych środowisk, takich jak na przykład ~~CLion~~, osobom które dobrze ich nie znają; w
przeciwnym wypadku dodatkowe ich funkcje mogą bardziej przeszkadzać niż pomagać. Ponadto nie wolno używać środowisk
opartych na przeglądarce, takich jak na przykład ~~<https://www.onlinegdb.com>~~; wynika to z tego, że przechowują one
kod wyłącznie w przeglądrce, co w przypadku problemów ze stabilnością komputera może skutkować utratą kodu i
koniecznością zaczęcia od nowa.

### Przed pierwszym punktowanym laboratorium

Należy:

- zapoznać się z [programem]({{< ref "../../syllabus" >}}), [regulaminem]({{< ref "../../zasady" >}}) i [harmonogramem]({{< ref "../../harmonogram" >}}),
- przygotować edytor wedle własnych upodobań (lub być w stanie to szybko zrobić na początku laboratorium),
- zapoznać się z [odpowiednim tutorialem]({{< ref "../l1/index.pl.html" >}}),
- wykonać [przykładowe zadanie]({{< ref "../l1-example.pl.md" >}}),
- przećwiczyć operacje wykonywane na laboratorium zerowym,
- zapoznać się ze stronami `man` i materiałem na wejściówkę.
