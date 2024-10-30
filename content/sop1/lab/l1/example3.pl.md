---
title: "Example task 3 on POSIX program environment"
bookHidden: true
---

# Treść

Napisz program symulujący tworzenie menadżer pakietów w wirtualnym
środowisku.

Tworzenie środowiska wirtualnego polega na:

- Tworzeniu katalogu z nazwą nowego środowiska
- Dodaniu do niego pliku `requirements`

Do tworzenia wirtualnego środowiska służy opcja `-c`. Poprzez opcję
`-v` użytkownik może podać środowisko, w którym chce wykonać
daną operację (w tym wypadku tworzenie), np:

`./sop-venv -c -v <ENVIRONMENT_NAME>`.

Zarządzanie środowiskiem polega na dodawaniu i usuwaniu
pakietów. Instalacja polega na:

- Dodaniu do pliku `requirements` nazwy pakietu i jego wersji rozdzielonych spacją. The package can have any name
    and must have any version at this point.
- Dodaniu pliku nazwanego jak pakiet wypełnionego losowymi znakami
z uprawnieniami tylko do odczytu dla wszystkich.

Odinstalowanie pakietu polega na usunięciu odpowiedniej linii z pliku `requirements`

Składnia:
- Instalacja:
    `./sop-venv -v <ENVIRONMENT_NAME> -i <PACKAGE_NAME>==<VERSION>`

- Deinstalacja: `./sop-venv -v <ENVIRONMENT_NAME> -r <PACKAGE_NAME>`

Program powinien szukać środowiska w aktualnym katalogu. Opcja
`-v` może być podana kilka razy (poza przypadkiem tworzenia nowego
środowiska) i oznacza wykonanie podanych operacji na wszystkich
podanych środowiskach. Błąd podczas operacji na którymś środowisku
powinien wstrzymać wykonywanie programy. Opcje mogą być podawane
w dowolnej kolejności.

Program powinien poprawnie obsługiwać błędy (wyświetlać
wiadomość w terminalu i zwracać odpowiedni kod) zwłaszcza
w przypadku błędu w składni komendy, operacji na pliku,
dodawania/usuwania pakietów czy operowania na nieistniejącym
środowisku. Program nie powinien dodawać dwóch pakietów o tej samej
nazwie (nawet o innej wersji) czy usuwać niezainstalowanych pakietów.


## Przykładowe wywołanie:

```
    $ ../sop-venv -c new_environment
    $ ls ./
    new_environment
    $ ls new_environment/
    requirements
```
Powyższa komenda stworzy katalog `new_environment` z plikiem
`requirements` w środku.

```
    $ ../sop-venv -v new_environment -i numpy==1.0.0
    $ ../sop-venv -v new_environment -i pandas==1.2.0
    $ cat new_environment/requirements
    numpy 1.0.0
    pandas 1.2.0
    $ ls -l new_environment/
    -r--r--r-- 1 user user 20 Oct 13 19:51 numpy
    -r--r--r-- 1 user user 30 Oct 13 19:51 pandas
    -rw-r--r-- 1 user user 17 Oct 13 19:50 requirements
    $ ../sop-venv -v new_environment -r numpy
    $ cat new_environment/requirements
    pandas 1.2.0
    $ ls -l new_environment/
    -r--r--r-- 1 user user 30 Oct 13 19:51 pandas
    -rw-r--r-- 1 user user 17 Oct 13 19:50 requirements
```
Powyższe komendy zainstalują dwa pakiety w środowisku
`new_environment` a następnie usuną jeden z nich.

```
    $ ../sop-venv -v non_existing_environment -i pandas
    sop-venv: the environment does not exist
    usage: ...
```
W powyższym wywołaniu program operuje na nieistniejącym środowisku
dlatego wypisuje błąd i tzw. `usage`.

## Etapy

- Implementacja tworzenia środowiska
- Dodanie możliwości instalacji pakietów (i obsługi błędów z tym związanych)
- Dodanie obsługi flagi `-v` kilka razy
- Dodanie możliwości usuwania pakietów
