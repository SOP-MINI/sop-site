---
title: "Użycie GITa w czasie laboratorium"
weight: 30
---


## Schemat użycia GITa na laboratorium

Na laboratorium każde zadanie będzie rozwiązywane w repozytorium.
Twoim celem jest śledzenie zmian w repozytorium w trakcie trwania laboratorium i ich synchronizacja z serwerem.
**Jeżeli jakiś kod nie znajdzie się na serwerze, nie będzie oceniany.**

### Konfiguracja ssh

W celu efektywniejsze pracy warto skonfigurować sobie autoryzację przy użyciu kluczy SSH.
Pozwalają one wykonywać operacje na zdalnym repozytorium bez ciągłego wpisywania swojego hasła, co w efekcie oszczędza dużo czasu.
Aby skonfigurować klucze SSH należy <tu link do tutorialu na sgicie>

#### Wiele różnych kluczy

Może zdarzyć się tak, że w folderze `~/.ssh` znajdują się już inne klucze (np. dla innego serwera). W takiej sytuacji najlepszy rozwiązaniem, jest powiedzenie programowi `ssh`, jakich kluczy ma używać przy połączeniu z danym serwerem. Zmień nazwę pobranych kluczy na inną, np. `id_ed25519_sop` oraz `id_ed25519_sop.pub` i umieść w katalogu `~/.ssh/` (pamiętaj o uprawnieniach, `chmod 600`). Następnie utwórz plik `~/.ssh/config` z zawartością postaci:

```
IdentityFile ~/.ssh/id_ed25519
Host vl01
  IdentityFile ~/.ssh/id_ed25519_sop
```

Tak skonstruowany plik konfiguracyjny mówi programowi `ssh`, żeby zawsze patrzył na domyślny klucz `id_ed25519` (jak wcześniej), a jeżeli łączymy się do servera `vl01` (serwer używany na SOPach) żeby użył klucza `id_ed25519_sop`. W ten sposób możesz dodawać kolejne klucze do różnych serwerów. 

### Praca z repozytorium

Pierwszym krokiem będzie wykonanie kopii zdalnego repozytorium na swoją stację roboczą poleceniem

```shell
$ git clone ssh://gitolite@vl01/w1pl/imię_nazwisko w1
```

Polecenie stworzy folder o nazwie `w1` i wykona do niego kopie plików.
Ostatni parametr polecenia określa nazwę folderu, który zostanie stworzony dla repozytorium - tak więc na kolejnych laboratoriach może to być `l1`, `l2` etc.
Jeżeli nie podamy żadnej nazwy git domyślnie utworzy folder o takiej samej nazwie jak nazwa repozytorium - w tym wypadku `imię_nazwisko`.

W przypadku gdyby polecenie zakończyło się niepowodzeniem należy sprawdzić, czy dobrze wpisaliśmy w adresie repozytorium nasz login postaci `imię_nazwisko` - powinien być bez polskich znaków, taki sam jak we wspomnianych plikach na LeONie - najlepiej przekopiować stamtąd. W przypadku gdy mimo to pobieranie repozytorium nie działa należy zgłosić to prowadzącemu.

Zadanie składa się z etapów.
Po zakończeniu etapu należy wykonać commita do repozytorium (polecenia `git add` i `git commit`).
Commit powinien mieć nazwę mówiącą, którego etapu dotyczy oraz co dodaje/naprawia, w rodzaju "Etap 2 - poprawka zwalniania pamięci"
Aby zsynchronizować lokalne zmiany do serwera, należy wykonać polecenie

```shell
$ git push
```

Proszę pamiętać, że za etap można uzyskać punkty dopiero, gdy jego kod znajdzie się na zdalnym repozytorium.
Możliwość synchronizacji z serwerem zostaje utracona punktualnie z końcem czasu przeznaczonego na zadanie.

Aby rozwiązanie zostało przyjęte przez serwer musi spełniać następujące warunki:
- Jedynie pliki rozwiązania (`.c`) zostały zmodyfikowane - modyfikacja jakichkolwiek innych plików, np. makefile spowoduje odrzucenie rozwiązania
- Pliki rozwiązania są poprawne sformatowane. W folderze repozytorium znajduje się plik `.clang-format` będący konfiguracją dla programu `clang-format` zainstalowanego na komputerach laboratorium. Umożliwia on ładne sformatowanie kodu poprzez polecenie `clang-format -i <nazwapliku>.c` . Wiele edytorów pozwala na integrację z `clang-format` i automatyczne formatowanie pliku podczas pisania albo przy zapisie.
- Rozwiązanie nie jest zbyt długie - domyślnie 600 linii (zadania laboratoryjne powinny być zwykle możliwe do rozwiązania w mniej niż 300 liniach)
- Rozwiązanie powinno się kompilować bez błędów przy użyciu makefile zawartego w repozytorium

W przypadku niespełnienia którego z warunków serwer odrzuci rozwiązanie z odpowiednim komunikatem. Należy poprawić swój kod, zrobić commit i ponownie wykonać push. Serwer pozwala wykonać jeden push na minutę.
