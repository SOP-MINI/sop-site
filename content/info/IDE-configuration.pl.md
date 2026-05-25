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

Poniżej przedstawiamy konfigurację dla dwóch przykładowych IDE, jednak warto podkreślić, że są to tylko przykłady podyktowane prywatnymi preferencjami autora. Wszystkie wymienione programy (po odpowiedniej konfiguracji) powinny sprawdzić się równie dobrze, więc jeżeli już ma się ulubione narzędzie można przy nim zostać (chociaż warto dać szansę QtCreatorowi, bardzo przyjemne IDE ~ _osobista wstawka autora_).

## Pierwszy krok - tworzenie przykładowego projektu

Jako pierwszy krok w konfiguracji do dowolnego z poniższych poradników, powinniśmy stworzyć przykładowy projekt, który to później będziemy ładować/otwierać za pomocą wybranego IDE.  

Tworzymy prosty projekt. Stwórz nowy folder o wybranej nazwie (u nas będzie to `test`) a w nim dwa pliki: `main.c` oraz `Makefile`:

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

W folderze projektu tworzymy jeszcze plik konfiguracyjny dla `clang-format` - na laboratorium będzie zawarty w startowym repozytorium. **Ważne!** Plik ten zaczyna się od kropki, więc jest domyślnie niewidoczny.

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

Teraz możemy przejść do konfiguracji wybranego IDE.

## Konfiguracja IDE QtCreator

Zakładamy, iż stworzyliśmy już przykładowy projekt, jak zostało to opisane [tutaj](#pierwszy-krok---tworzenie-przykładowego-projektu). Następnie przechodzimy do utworzonego katalogu.  

W tym momencie wywołanie polecenia `make` powinno poprawnie skompilować nasz program, który po uruchomieniu powinien wypisywać klasyczne `Hello world`.

Możemy teraz uruchomić program QtCreator. Po uruchomieniu wybieramy `File->New Project`. To ważne, żeby wybrać `New Project` a nie `Open File or Project` - jeżeli po prostu otworzymy nasz plik `main.c` QtCreator nie będzie wiedział, z jakimi flagami będziemy chcieli kompilować program a tym samym, jego działanie może być niestabilne (np. raportowanie błędów, których tak naprawdę nie ma). W oknie, które się pojawi wybieramy `Import project` oraz `Import Existing Project`.

![](/img/qtcreator1.png) 

Następnie wybieramy nazwę dla naszego projektu (w przykładzie jest to po prostu `test`) oraz folder projektu, z którego potem dodajemy pliki źródłowe - w naszym przypadku `main.c`. Po wykonaniu tego QtCreator stworzy szereg plików postaci `test.<rozszerzenie>` w folderze projektu oraz otworzy nasz nowy projekt. Powinno to wyglądać mniej-więcej tak:

![](/img/qtcreator2.png) 

W czasie laboratorium nowo utworzonych plików nie należy dodawać do gita (serwer je odrzuci) - jest to tylko nasz lokalna konfiguracja, w praktyce gdy kilku programistów współpracuje ze sobą każdy może używać innych narzędzi. Z tych plików interesuje nas tylko `test.cflags`. W tym pliku podajemy flagi kompilacji, których QtCreator użyje do swoich "inteligentnych" funkcjonalności, jak np. raportowanie błędów. Jest ważne, żeby flagi były takie same jak w pliku `Makefile`, tak więc teraz należy wpisać tam `-fsanitize=address,undefined -Wall -Wextra -Wshadow -Werror`.

W tym momencie wszystkie podstawowe funkcjonalności powinny działać. W miarę jak będziesz pisał kod powinieneś widzieć okienko z podpowiedziami. Gdy popełnisz błąd (spróbuj np. nie dodać średnika na końcu linii) program podświetli błędną linię na czerwono.

Ostatnim elementem jest włączenie formatowania przy zapisie - oszczędzi to nam uciążliwego wywoływania `clang-format` przed każdym commitem. W tym celu musimy włączyć plugin `beautifier`. Z menu `Help` kliknij `About Plugins` a następnie znajdź na liście `Beautifier` i aktywuj przez kliknięcie `Ok`. Konieczny będzie restart programu. W folderze naszego projektu znajduje się plik konfiguracyjny `.clang-format`, który uprzednio utworzyliśmy.  

Zauważ, że nazwa tego pliku zaczyna się od kropki - jest on domyślnie niewidoczny. Teraz wejdź w menu `Edit->Preferences` i wybierz sekcje `Beautifier`. W zakładce `General` zaznacz `Automatic formatting on file save` a z menu `Tool` wybierz `ClangFormat`. W zakładce `ClangFormat` wybierz `Use predefined style` i ustaw jako `File`. Zaakceptuj ustawienia klikając `Ok`.

![](/img/qtcreator3.png) 

Teraz za każdym razem gdy zapiszesz plik (Ctrl+S) będzie on automatycznie formatowany. Aby sprawdzić, czy funkcjonalność działa spróbuj dodać gdzieś w programie kilka zbędnych spacji i zapisz plik.

Program jest w tym momencie gotowy na laboratoria. Wciąż warto spędzić kilka minut przeglądając i dostosowując różne opcje w ustawieniach (`Edit->Preferences`), zwłaszcza w sekcji `Environment` gdzie można skonfigurować wygląd oraz skróty klawiszowe. Domyślnie pod skrótem `ctrl+k` QtCreator ma bardzo wygodną wyszukiwarkę pozwalającą znajdować różne klasy obiektów (pliki, funkcje, zmienne globalne itp.) w projekcie. Inne opcje oraz instrukcje można znaleźć w pomocy oraz dokumentacji.

## Konfiguracja Visual Studio Code

Przed utworzeniem projektu należy najpierw zainstalować rozszerzenia potrzebne do pracy z C/C++. W tym celu uruchamiamy Visual Studio Code, a następnie przechodzimy w zakładkę rozszerzenia (skrót klawiszowy: `Ctrl + Shift + X`).

![](/img/vsc1.png) 

Wyszukujemy rozszerzenia **C/C++** (wydane przez Microsoft, powinno być to pierwsze rozszerzenie po wyszukiwaniu) po czym klikamy *install*. Instalacja jest automatyczna i nie jest wymagany restart edytora.

![](/img/vsc2.png) 

Podobnie postępujemy ze wtyczką **Makefile tools** (również dystrybuowane przez Microsoft).

![](/img/vsc3.png) 

Są to wszystkie rozszerzenia potrzebne do wygodnej pracy z kodem w języku C.   

W Visual Studio Code klikamy w lewym górnym rogu `File -> Open Folder` i wybieramy nasz [uprzednio utworzony](#pierwszy-krok---tworzenie-przykładowego-projektu) folder `test`.

W takiej konfiguracji wywołanie polecenia `make` kompiluje program wypisujący `Hello world` na standardowe wyjście.  

**Uwaga!** Bardzo ważnym krokiem jest zmiana domyślnego standardu C, którego używa edytor! W przeciwnym razie edytor podświetla część nazw jako nieznane, mimo że program poprawnie się kompiluje! By to rozwiązać należy zmienić domyślny standard `c17` na `gnu17`. By to zrobić klikamy skrót klawiszowy `Ctrl + Shift + P`, wpisujemy `C/C++: Edit Configurations (UI)` oraz naciśnijmy enter. Powinna pojawić się nam strona z ustawieniami kompilatora. Przewijamy stronę w dół, gdzie w zakładce `C standard` zmieniamy z `c17` na `gnu17`. Powinno wyglądać to mniej więcej tak:

![](/img/vsc5.png) 

Od tego momentu edytor powinien poprawnie analizować i podświetlać kod.  

**Uwaga!** Koniecznym jest by w środowisku nie było żadnych pomocniczych narzędzi AI (w VS Code domyślnie jest zainstalowany GitHub Copilot, niemniej jednak trzeba się zalogować + narzędzie to domyślnie nie podpowiada w trakcie manualnego pisania kodu). Oczywiście instalacja dodatkowych rozszerzeń z agentami AI jest dość odradzana.  
Łatwym sposobem na wyłączenie jakichkolwiek pomocy związanych z AI jest: wciśnięcie `Ctrl +` , oraz wpisanie `disable ai features` oraz zaznaczenie `chat.disableAIFeatures`.

![](/img/vsc14.png) 

Kolejnym krokiem będzie ustawienie opcji do podkreślania błędów w kodzie (teoretycznie opcja ta dla języka C jest włączona domyślnie, lecz często zdarza się, iż z jakiegoś powodu jest wyłączona bez dokonania żadnej interwencji).  
Wciskamy skrót klawiszowy `Ctrl + ,` (Lewy przycisk Ctrl oraz przecinek), po czym wpisujemy `error squiggles` w wyszukiwarkę w opcjach, po czym zmieniamy opcję na `enabled`.

![](/img/vsc6.png) 

Podobnie postępujemy z silnikiem edycji IntelliSense (którego wartość domyślna powinna również być ustawiona na wartość `default`, choć nie zawsze tak jest). Wciskamy `Ctrl + ,` oraz wpisujemy `IntelliSense engine`. Ustawiamy wartość `C_Cpp.intelliSenseEngine` na `default`.

![](/img/vsc7.png)

Kolejnym krokiem będzie skonfigurowanie obsługi pliku `Makefile`, tak aby kompilacja korzystała ze zdefiniowanych flag.

Wciskamy `Ctrl + ,` po czym wpisujemy w wyszukiwarkę `configuration provider`. W polu `C_Cpp.default.configurationProvider` wpisujemy daną wartość: `ms-vscode.makefile-tools`. Dzięki temu VS Code będzie automatycznie korzystał z ustawień podanych w pliku Makefile.  

![](/img/vsc8.png)

Następnie (w otworzonym projekcie) wciskamy skrót `Ctrl + Shift + P` oraz uruchamiamy polecenie `Makefile: Configure`. Dokona to automatycznej konfiguracji opcji Makefile do VS Code. Następnie ponownie wciskamy skrót `Ctrl + Shift + P` oraz uruchamiamy komendę `Makefile: Set the target to be built by make` oraz wybieramy `all`.

Następnie skonfigurujemy opcjonalną, ale bardzo przydatną funkcję automatycznego formatowania kodu przy zapisie plików C. W trakcie laboratoriów, w naszym projekcie będziemy mieli dostęp do pliku konfiguracyjnego `.clang-format`.

Będziemy chcieli wskazać powyższy plik jako opcje formatowania dla VS Code. Najpierw włączamy opcje formatowania kodu przy zapisie: `Ctrl + ,` wpisujemy w wyszukiwarkę `format on save` oraz włączamy opcję `Format on File save`.  

![](/img/vsc9.png)

Następnie wyszukujemy `default formatter` oraz w ustawieniu `Default Formatter` wybieramy `C/C++ ms-vscode.cpptools`.

![](/img/vsc10.png)

Potem wyszukujemy `C_Cpp formatting` gdzie zmieniamy ustawienie `C_Cpp: Formatting` na wartość `clangformat`.

![](/img/vsc11.png)

Na koniec upewnijmy się, iż opcja `C_Cpp: Clang_format_style` (dostępna po wyszukaniu) jest ustawiona na wartość `file` (powinna być tak ustawiona domyślnie) by VS Code używał opcji formatowania zdefiniowanych w pliku `.clang-format`.

![](/img/vsc12.png)

Są to wszystkie kroki potrzebne by formatowanie wykonywało się automatycznie przy zapisie pliku, co przyspieszy nam pracę i znacznie ułatwi wykonywanie zadań laboratoryjnych.  

Dość problematyczną funkcją edytora może być automatyczne dopisywanie dyrektyw `#include` do plików `.c` (automatycznie dodaje dyrektywy `include` co sprawia duże problemy na laboratoriach). By ją wyłączyć klikamy `Ctrl + ,`, wpisujemy `Refactoring Include Header` oraz ustawiamy opcję `Include Header` na wartość `never`. Teraz VS Code nie powinien samoczynnie dodawać niepotrzebnych `#include` do plików źródłowych `.c`.

![](/img/vsc15.png)

Jako ostatnią kwestię zajmijmy się podpowiedziami i autouzupełnianiem. Większość z opcji podpowiedzi oraz autouzupełniania powinno być już aktywnych, niemniej jednak możemy upewnić się wciskając `Ctrl + ,` oraz wyszukaniu `parameter hints`. Zaleca się zastosowanie ustawień pokazanych na poniższym zrzucie ekranu:

![](/img/vsc13.png)

Po wykonaniu opisanych kroków Visual Studio Code powinien być gotowy do użycia podczas laboratoriów. Warto jednak poświęcić chwilę na poznanie kilku dodatkowych funkcji edytora, ponieważ VS Code jest środowiskiem bardzo elastycznym i w dużym stopniu konfigurowalnym. Szczególnie przydatna może być paleta poleceń dostępna pod skrótem `Ctrl + Shift + P`, z której można uruchamiać większość funkcji programu bez szukania ich w menu. Dobrze jest także zapoznać się z panelem rozszerzeń, ustawieniami formatowania kodu oraz możliwością otwierania zintegrowanego terminala skrótem `Ctrl + ,`. Więcej opcji i przykładów konfiguracji można znaleźć w [dokumentacji Visual Studio Code](https://code.visualstudio.com/docs).