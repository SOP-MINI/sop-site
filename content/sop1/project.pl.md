---
title: "Projekt"
---

Projekt trwa cztery tygodnie od 18.11 do 16.12. W czasie W3 (16.12) ma miejsce sprawdzanie i ocenianie zgodnie ze szczegółami opisanymi w treści.

Kod źródłowy do oceny powinien zostać umieszczony na serwerze podobnie jak na laboratoriach - ścieżka do projektu to `p1pl`. Należy mieć na uwadze pewne ograniczenia techniczne:
- Niestety serwer jest włączany tylko dwa razy w tygodniu - w poniedziałki w godzinach 20:15-23:00 oraz we wtorki 14:15-17:00. Tylko wtedy jest możliwe spushowanie swojego rozwiązania
- Serwer jest dostępny tylko z sieci wydziałowej. Żeby wypchnąć swoje rozwiązanie spoza niej można skorzystać z dwóch możliwości:
  - przekopiować je na serwer ssh (scp na ssh.mini.pw.edu.pl) i stamtąd wykonać push jak na laboratorium
  - użyć tunelowania ssh (uwaga - trzeba wtedy też zmienić/dodać `remote` w repozytorium)

Serwer wykonuje wstępne sprawdzenie, podobnie jak na laboratorium, a w przypadku błędu może odrzucić rozwiązanie. Jednak, ponieważ projekt wymaga napisania większej ilości kodu niż na zwyczajnym laboratorium, zasady są nieco luźniejsze. Dopuszczalne (i rekomendowane) jest tworzenie i modyfikacja nowych plików .c i .h. Nie można jedynie modyfikować startowego `Makefile` oraz `.clang-format`. Jak zawsze pliki źródłowe powinny być sformatowane a program kompilować się bez błędów i ostrzeżeń.

[Treść zadania znajduje się tutaj](/files/p1pl.pdf)

W razie wątpliwości zapraszamy do zadawania pytań po zajęciach/drogą mailową.

## Użycie tunelu ssh

Tunel ssh pozwala połączyć się z serwerem, do którego nie mamy bezpośredniego dostępu, za pośrednictwem serwera dostępowego. W naszym przypadku serwer przedmiotu (`vl01`) jest w sieci lokalnej wydziału - nie ma do niego dostępu z zewnątrz. Jednak możemy z zewnątrz zalogować się na wydziałowy serwer ssh (`ssh.mini.pw.edu.pl`), który z kolei ma dostęp do sieci lokalnej.

Aby móc wypchnąć zmiany na serwer spoza wydziału należy wykonać następujące kroki:

1. Upewnić się, że mamy poprawnie skonfigurowane klucze ssh - tak jak na laboratoriach, trzeba je ściągnąć z LeONa

2. Potwierdzić, że możemy się zalogować na serwer ssh - `ssh <login>@ssh.mini.pw.edu.pl`

3. Oczywiście połączenie z serwerem `vl01` jest możliwe tylko, gdy jest on włączony - kroki opisane niżej należy więc wykonywać w określonych porach opisanych wyżej.

4. Tworzymy tunel: 
```
ssh -L 22222:vl01:22 <login>@ssh.mini.pw.edu.pl
```
To polecenie sprawi, że ssh będzie kierować cały ruch sieciowy z portu 22222 na naszym komputerze przez wydziałowy serwer ssh na serwer `vl01` port 22 - czyli port ssh. Nie zamykaj tego terminala ani programu ssh - to on realizuje przekierowanie. Kolejne kroki wykonaj w nowym oknie terminala.

5. Żeby zweryfikować, że mamy połączenie i dobrze skonfigurowane klucz możemy spróbować zalogować się na `vl01` poprzez polecenie `ssh -p 22222 gitolite@localhost`. Jeżeli wszystko jest poprawne powinniśmy zobaczyć krótki komunikat o poprawnym logowaniu. Powinno być też możliwe pobranie repozytorium używając nowego adresu `git clone ssh://gitolite@localhost:22222/p1pl/<imię_nazwisko>`

6. Teraz musimy skonfigurować gita w naszym repozytorium, żeby łączył się przez nowy tunel. Ten krok nie jest niezbędny jeżeli właśnie pobrałeś repo z nowego adresu (przez `git clone` j.w. i chcesz go używać). Będąc w repozytorium użyj polecenia: 
```
git config url."ssh://gitolite@localhost:22222".insteadOf "ssh://gitolite@vl01"
```
W tym momencie polecenie
```
git remote -v
```
powinno zwrócić adres zaczynający się od `ssh://gitolite@localhost:22222` a wszystkie polecenia gita w rodzaju `fetch`, `push` czy `pull` powinny działać normalnie.


## Kopiowanie na serwer ssh

Ta metoda nie jest tak elegancka jak tunel, ale jest bardzo prosta:

1. Skopiuj swoje rozwiązanie na serwer ssh:
```
scp -r <katalog repozytorium> <login>@ssh.mini.pw.edu.pl:
```

2. Zaloguj się na serwer ssh:
```
ssh <login>@ssh.mini.pw.edu.pl
```

3. Na serwerze ssh wejdź do repozytorium i wykonaj `push` jak zwykle.

