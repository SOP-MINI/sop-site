---
title: "Użycie GITa w czasie laboratorium"
weight: 30
---


## Schemat użycia GITa na laboratorium

{{< hint info >}}

Tą sekcję należy wykonać w sali na laboratorium L0.
Przed przejściem przez tą sekcję trzeba zapoznać się z opisem użycia [GITa]({{< ref "/info/git" >}})
Wymaga ona dostępu do serwera działającego tylko w trakcie trwania laboratorium.
Zachęcam jednak do przeczytania jej i przygotowania się do wykonania poniższych kroków.

{{< /hint >}}

Na laboratorium każde zadanie będzie rozwiązywane w takim repozytorium.
Twoim celem jest śledzenie zmian w repozytorium w trakcie trwania laboratorium i ich synchronizacja z serwerem.
**Jeżeli jakiś kod nie znajdzie się na serwerze, nie będzie oceniany.**

Zanim przejdziemy do pobierania kodu, trzeba uzyskać swój osobisty klucz SSH z platformy [LeON](https://leon.pw.edu.pl).
W dziale *klucze ssh* należy pobrać dwa pliki: `id_ed25519` oraz `id_ed25519.pub` oraz przekopiować je do folderu `~/.ssh`.
Plik `id_ed25519` jest *kluczem prywatnym* - to znaczy że może być użyty do uwierzytelnienia wobec serwera znającego klucz publiczny (oraz do wielu innych rzeczy, jak szyfrowanie, podpisy cyfrowe i inne).
Każdy klucz jest unikatowy i - zgodnie z nazwą - prywatny, tzn. działa jak hasło. Nie powinno się go udostępniać, ponieważ, podobnie jak z hasłem - jeżeli inna osoba wejdzie w jego posiadanie jest w stanie się pod nas podszyć i np. skasować nasze rozwiązanie z serwera. Zmień uprawnienia poleceniem:
```shell
chmod 600 ~/.ssh/id_ed25519
```

Pierwszym krokiem będzie wykonanie kopii zdalnego repozytorium na swoją stację roboczą poleceniem

```shell
$ git clone ssh://gitolite@vl01/l0pl/imię_nazwisko l0
```

Polecenie stworzy folder o nazwie `l0` i wykona do niego kopie plików.
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
