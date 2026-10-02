---
title: "Użycie GITa w czasie laboratorium"
weight: 30
---


## Schemat użycia GITa na laboratorium

Na laboratorium każde zadanie będzie rozwiązywane w repozytorium.
Twoim celem jest śledzenie zmian w repozytorium w trakcie trwania laboratorium i ich synchronizacja z serwerem.
**Jeżeli jakiś kod nie znajdzie się na serwerze, nie będzie oceniany.**

### Wydziałowy serwer git

Wydziałowy serwer GIT, na którym będą znajdować się repozytoria w czasie laboratorium dostepny jest pod adressem <https://sgit.mini.pw.edu.pl>.
Pod adresem <https://sgit.mini.pw.edu.pl/git-tutorial> znajdują się informacje o dostępie oraz konfiguracji.
W szczególności warto skonfigurować sobie klucze SSH, jeśli jeszcze się tego nie zrobiło na innych zajęciach.
Pozwalają one wykonywać operacje na zdalnym repozytorium bez ciągłego wpisywania swojego hasła, co w efekcie bardzo przyspiesza pracę.

### Praca z repozytorium podczas laboratorium

Na czas laboratorium zostanie przygotowane repozytorium z plikami startowymi.
Oznacza to, że pierwszym krokiem będzie wykonanie kopii zdalnego repozytorium na swoją stację roboczą poleceniem:

```shell
$ git clone ssh://git@192.168.137.60/OPS2_26L/w1_<nazwisko_imię> w1
```

Polecenie stworzy folder o nazwie `w1` i wykona do niego kopie plików.
Ostatni parametr polecenia określa nazwę folderu, który zostanie stworzony dla repozytorium - tak więc na kolejnych laboratoriach może to być `l1`, `l2` etc.
Jeżeli nie podamy żadnej nazwy git domyślnie utworzy folder o takiej samej nazwie jak nazwa repozytorium - w tym wypadku `w1_imię_nazwisko`.
Nie musimy próbować wpisywać powyższego adresu ręcznie.
Po zalogowaniu na serwer repozytorium laboratoryjne będzie widoczne na górze listy dostępnych repozytoriów.
Można w nie wejść a następnie skopiować ścieżkę SSH.

Zadanie składa się z etapów.
Po zakończeniu etapu należy wykonać commita do repozytorium (polecenia `git add` i `git commit`).
Commit powinien mieć nazwę mówiącą, którego etapu dotyczy oraz co dodaje/naprawia, w rodzaju "Etap 2 - poprawka zwalniania pamięci" - ułatwia (a tym samym przyspiesza) to sprawdzanie.
Aby zsynchronizować lokalne zmiany do serwera, należy wykonać polecenie

```shell
$ git push
```

Proszę pamiętać, że za etap można uzyskać punkty dopiero, gdy jego kod znajdzie się na zdalnym repozytorium.
Możliwość synchronizacji z serwerem zostaje utracona punktualnie z końcem czasu przeznaczonego na zadanie.

Aby rozwiązanie zostało przyjęte przez serwer musi spełniać następujące warunki:
- Jedynie wyznaczone pliki rozwiązania (`.c`) zostały zmodyfikowane - modyfikacja jakichkolwiek innych plików, np. makefile spowoduje odrzucenie rozwiązania, chyba, że w poleceniu jest napisane inaczej.
- Pliki rozwiązania są poprawne sformatowane. W folderze repozytorium znajduje się plik `.clang-format` będący konfiguracją dla programu `clang-format` zainstalowanego na komputerach laboratorium. Umożliwia on ładne sformatowanie kodu poprzez polecenie `clang-format -i <nazwapliku>.c` . Wiele edytorów pozwala na integrację z `clang-format` i automatyczne formatowanie pliku podczas pisania albo przy zapisie (patrz [konfiguracja IDE]({{< ref "info/IDE-configuration" >}}).).
- Rozwiązanie nie jest zbyt długie - domyślnie 600 linii (zadania laboratoryjne powinny być zwykle możliwe do rozwiązania w mniej niż 300 liniach)
- Rozwiązanie powinno się kompilować bez błędów przy użyciu makefile zawartego w repozytorium

W przypadku niespełnienia którego z warunków serwer odrzuci rozwiązanie z odpowiednim komunikatem.
Należy poprawić swój kod, zrobić commit i ponownie wykonać push.
Serwer pozwala wykonać jeden push na minutę.
