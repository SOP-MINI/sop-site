---
title: "Konfiguracja IDE"
weight: 40
---

# Konfiguracja IDE

Na laboratoriach nie jest wymagane korzystanie z konkretnego środowiska. Można wybrać dowolny program z zainstalowanych na wydziałowych komputerach a nawet (dla zaawansowanych) jeśli bardzo brakuje nam ulubionego programu, spróbować lokalnej instalacji. Jednak dobry edytor kodu czy IDE (ang. "Integrated development environment" - "Zintegrowane środowisko programistyczne") powinno posiadać kilka bardzo przydatnych w czasie rozwiązywania zadań laboratoryjnych funkcjonalności. Należą do nich:

- podświetlanie składni programu
- raportowanie i podświetlanie błędów
- podpowiedzi i autouzupełnianie (za wyjątkiem narzędzi wykorzystujących AI jak np. Copilot - te są zakazane)
- formatowanie kodu przy zapisie

Na komputerach laboratoryjnych jest zainstalowane kilka programów, które (po odpowiedniej konfiguracji) spełniają wszystkie te wymagania. Należą do nich (w kolejności alfabetycznej):

- CLion
- Emacs
- Geany
- QtCreator
- Vim
- Visual Studio Code

Poniżej przedstawiamy konfigurację przykładowego IDE, jednak warto podkreślić, że jest to tylko przykład podyktowany prywatnymi preferencjami autora. Wszystkie wymienione programy (po odpowiedniej konfiguracji) powinny sprawdzić się równie dobrze, więc jeżeli już ma się ulubione narzędzie można przy nim zostać (chociaż warto dać szansę QtCreatorowi, bardzo przyjemne IDE ~ _osobista wstawka autora_).

# Konfiguracja IDE QtCreator

W pierwszej kolejności w celu przetestowania programu stworzymy prosty projekt. Stwórz nowy folder o wybranej nazwie (u nas będzie to `test`) a w nim dwa pliki: `main.c` oraz `Makefile`:

**main.c**:
```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Hello world\n");
    return EXIT_SUCCESS;
}
```

**Makefile**:
```
all: main
main: main.c	
	gcc -fsanitize=address,undefined -Wall -Wextra -Wshadow -Werror -o main main.c
.PHONY: clean all
clean:
	rm main
```

W tym momencie wywołanie polecenie `make` powinno poprawnie skompilować nasz program, który po uruchomieniu powinien wypisywać klasyczne `Hello world`.

Możemy teraz uruchomić program QtCreator. Po uruchomieniu wybieramy `File->New Project`. To ważne, żeby wybrać `New Project` a nie `Open File or Project` - jeżeli po prostu otworzymy nasz plik `main.c` QtCreator nie będzie wiedział, z jakimi flagami będziemy chcieli kompilować program a tym samym, jego działanie może być niestabilne (np. raportowanie błędów, których tak naprawdę nie ma). W oknie, które się pojawi wybieramy `Import project` oraz `Import Existing Project`.

![](/img/qtcreator1.png) 

Następnie wybieramy nazwę dla naszego projektu (w przykładzie jest to po prostu `test`) oraz folder projektu, z którego potem dodajemy pliki źródłowe - w naszym przypadku `main.c`. Po wykonaniu tego QtCreator stworzy szereg plików postaci `test.<rozszerzenie>` w folderze projektu oraz otworzy nasz nowy projekt. Powinno to wyglądać mniej-więcej tak:

![](/img/qtcreator2.png) 

W czasie laboratorium nowo utworzonych plików nie należy dodawać do gita (serwer je odrzuci) - jest to tylko nasz lokalna konfiguracja, w praktyce gdy kilku programistów współpracuje ze sobą każdy może używać innych narzędzi. Z tych plików interesuje nas tylko `test.cflags`. W tym pliku podajemy flagi kompilacji, których QtCreator użyje do swoich "inteligentnych" funkcjonalności, jak np. raportowanie błędów. Jest ważne, żeby flagi były takie same jak w pliku `Makefile`, tak więc teraz należy wpisać tam `-fsanitize=address,undefined -Wall -Wextra -Wshadow -Werror`.

W tym momencie wszystkie podstawowe funkcjonalności powinny działać. W miarę jak będziesz pisał kod powinieneś widzieć okienko z podpowiedziami. Gdy popełnisz błąd (spróbuj np. nie dodać średnika na końcu linii) program podświetli błędną linię na czerwono.

Ostatnim elementem jest włączenie formatowania przy zapisie - oszczędzi to nam uciążliwego wywoływania `clang-format` przed każdym commitem. W tym celu musimy włączyć plugin `beautifier`. Z menu `Help` kliknij `About Plugins` a następnie znajdź na liście `Beautifier` i aktywuj po kliknięciu `Ok` konieczny będzie restart programu. W folderze projektu stwórz plik konfiguracyjny dla `clang-format` - na laboratorium będzie zawarty w startowym repozytorium.

**.clang-format**:
```
BasedOnStyle: Google
IndentWidth: 4
ColumnLimit: 120
BreakBeforeBraces: Allman
BreakConstructorInitializersBeforeComma: false
AllowShortIfStatementsOnASingleLine: false
AllowShortBlocksOnASingleLine: false
AllowShortLoopsOnASingleLine: false
IncludeBlocks: Preserve
PointerAlignment: Left
InsertNewlineAtEOF: true
```

Zauważ, że nazwa tego pliku zaczyna się od kropki - jest on domyślnie niewidoczny. Teraz wejdź w menu `Edit->Preferences` i wybierz sekcje `Beautifier`. W zakładce `General` zaznacz `Automatic formatting on file save` a z menu `Tool` wybierz `ClangFormat`. W zakładce `ClangFormat` wybierz `Use predefined style` i ustaw jako `File`. Zaakceptuj ustawienia klikając `Ok`.

![](/img/qtcreator3.png) 

Teraz za każdym razem gdy zapiszesz plik (Ctrl+S) będzie on automatycznie formatowany. Aby sprawdzić, czy funkcjonalność działa spróbuj dodać gdzieś w programie kilka zbędnych spacji i zapisz plik.

Program jest w tym momencie gotowy na laboratoria. Wciąż warto spędzić kilka minut przeglądając i dostosowując różne opcje w ustawieniach (`Edit->Preferences`), zwłaszcza w sekcji `Environment` gdzie można skonfigurować wygląd oraz skróty klawiszowe. Domyślnie pod skrótem `ctrl+k` QtCreator ma bardzo wygodną wyszukiwarkę pozwalającą znajdować różne klasy obiektów (pliki, funkcje, zmienne globalne itp.) w projekcie. Inne opcje oraz instrukcje można znaleźć w pomocy oraz dokumentacji.

