---
title: "Laboratorium wstępne do zajęć z SOP1"
date: 2022-02-05T17:26:02+01:00
draft: false
---

<div class="dlaN">

Te pierwsze zajęcia nie wymagają od Ciebie dużego przygotowania, mają służyć wyjaśnieniu wszelkich wątpliwości co do zasad prowadzenia zajęć i harmonogramu. Przeczytaj proszę informacje podane w działach [program]({{< ref "../../syllabus" >}}), [regulamin]({{< ref "../../zasady" >}}) i [harmonogram]({{< ref "../../harmonogram" >}}). Zerknij też na materiały [pomocnicze]({{< ref "../../materialy" >}}).

W dziale z [materiałami]({{< ref "../../materialy" >}}) znajdziesz tutoriale do samodzielnego wykonania w ramach przygotowania do kolejnych zajęć. Podczas punktowanych zajęć zawsze poświęcamy od 15 do 30 minut na odpowiadanie na pytania od studentów dotyczące danego działu. Można też się skonsultować droga mailową lub osobiście w czasie dyżurów prowadzących laboratoria.

W ramach przygotowania do kolejnych zajęć zrobimy proste ćwiczenie symulujące pracę na punktowanych laboratoriach.

</div>

-----

### Zadanie

*Cel:*Napisać trywialny program "hello world", skompilować go, uruchomić , spakować i przesłać na wskazane konto. *Co student musi wiedzieć:*

  - znać dowolne środowisko programisty w systemie Linux
  - znać podstawowe flagi polecenia gcc
  - umieć posługiwać się konsolą
  - umieć pakować pliki poleceniem tar
  - umieć kopiować pliki poleceniem cp

*rozwiązanie **prog1.c**:*

    #include <stdio.h>
    #include <stdlib.h>
    
    int main(int argc, char** argv) {
    
        printf("Hello world\n");
        return EXIT_SUCCESS;
    }

*kompilacja:*

    $ gcc -Wall -fsanitize=address,undefined -o prog1 prog1.c

Uwaga: użycie flagi -Wall jest obowiązkowe w trakcie laboratorium, program nie powinien generować istotnych ostrzeżeń. Każde zadanie wymaga również użycia odpowiedniego zestawu opcji -fsanitize. [Więcej informacji o sanitizerach.]({{< ref "../sanitizers" >}})

*uruchomienie:*

    $ ./prog1

*spakowanie i sprawdzenie:*

    $ tar -cjf nowakj.tar.bz2 prog1.c
    $ tar -tjf nowakj.tar.bz2

Zamiast nowakj należy wstawić swój login wydziałowy.

<div class="dlaN">

  - Można podać wiele plików lub katalogów do spakowania.

  - **UWAGA** nie wolno dowolnie mieszać parametrów polecenia tar, parametr -f ma argument - archiwum.

  - **UWAGA** nie wolno dowolnie mieszać argumentów polecenia tar, np podanie:
    
    ~~`$ tar -cjf prog1.c nowakj.tar.bz2`~~
    
    spowoduje skasowanie pliku z kodem źródłowym \!\!\!

</div>

*wgranie pracy ze sprawdzeniem:*

    $ cp nowakj.tar.bz2 {{< teacherdir >}}
    $ ls -l {{< teacherdir >}}nowakj.tar.bz2

Zamiast nowakj należy wstawić swój login wydziałowy.

<div class="dlaN">

Katalog do którego wgrywacie prace może być inny niż ten przykładowy, będzie to podane w zadaniu.

</div>

