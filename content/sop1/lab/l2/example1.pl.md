---
title: "Zadanie testowe 1 z tematu niskopoziomowe operacje na plikach i poll"
bookHidden: true
---

## Treść

W zamku **Lśniącego Diamentu** zbliżają się urodziny księżniczki Joanny. Dworskie kucharki chcą upiec legendarny **Tort**, ale przepis znają tylko stare babcie, które plotkują w wieżach przy herbatce i dyktują kroki do kuchni przez **łącza nazwane**. Mówią z pełnymi ustami, więc kroki docierają w kawałkach, a zagadane mylą numery kroków. Do tego Joanna, odwiedzając każdą z babć, rozlewa herbatę i kruszy ciastka na ich notatki, przez co słowo `raspberries` bywa zapisane jako `raw~berries`.

Napisz program `sop-cake recipe tower1 [tower2 ... tower5]`, gdzie `recipe` to ścieżka do pliku przepisu, a `tower1...` to ścieżki do łączy nazwanych, przez które dyktują babcie.

Przepis to plik o rozmiarze 2048 bajtów: 32 linie po 64 bajty (63 znaki dopełnione spacjami + `\n`), linia `NN` to krok numer `NN`. Babcia wysyła wiadomości `NN:treść\n` (ASCII, maks. 127 bajtów bez `\n`). Wiadomość jest poprawna, gdy przed dwukropkiem jest tylko liczba z zakresu 0–31.

Babcie są tworzone za pomocą skryptu `granny.sh`. Polecenie `./granny.sh <tower> <number_of_steps>` tworzy łącze nazwane `<tower>`, czeka, aż ktoś otworzy je do odczytu, a potem dyktuje kolejne kroki, wypisuje je w swoim terminalu i na koniec usuwa łącze. Każdą babcię uruchom w osobnym terminalu, zanim uruchomisz program:

```
    terminal 1:  $ ./granny.sh tower1 8
    terminal 2:  $ ./granny.sh tower2 8
    terminal 3:  $ ./sop-cake recipe.txt tower1 tower2
```

Pliki i deskryptory należy obsługiwać wyłącznie funkcjami niskopoziomowymi. *Busy waiting* jest zabroniony.

## Etapy

1.  Jeśli plik przepisu nie istnieje, program tworzy go funkcją `write` jako 32 puste linie (każda linia to 63 spacje + `\n`). Jeśli istnieje, otwiera go i sprawdza przez `lseek`, czy ma dokładnie 2048 bajtów – jeśli rozmiar jest inny, wypisuje na `stderr` "Ta ksiega to nie przepis!" i kończy działanie.

    Następnie program otwiera do odczytu łącze `tower1`. Jeśli taki plik nie istnieje albo nie jest łączem nazwanym, program wypisuje na `stderr` "Nie ma babci na wieży X", gdzie `X` to podana ścieżka, i kończy działanie. Na koniec czyta dane z łącza aż do `EOF` i wypisuje je bez zmian na `stdout`.

    *Wskazówka:* łącze nazwane otwiera się funkcją `open` tak samo jak zwykły plik – patrz [tutorial]({{< ref "/sop1/lab/l2" >}}#łącza-nazwane-w-skrócie). Typ pliku sprawdzisz funkcją `stat` lub `fstat` i makrem `S_ISFIFO` (`man 7 inode`). Plik przepisu od razu otwórz z flagą `O_RDWR` – będzie potrzebna do `mmap` w etapie 4.

    *Jak pokazać:*
    ```
    terminal 1:  $ ./granny.sh tower1 8
    terminal 2:  $ ./sop-cake recipe.txt tower1
    terminal 2:  $ cat recipe.txt                     # utworzony plik
    terminal 2:  $ ./sop-cake recipe.txt recipe.txt   # brak babci
    terminal 2:  $ ./sop-cake granny.sh tower1        # zły przepis
    ```
2.  Zamiast wypisywać dane z łącza, program dzieli je na wiadomości zakończone `\n` – jedna wiadomość może przyjść w kilku wywołaniach `read`. Poprawny krok w formacie `NN:treść` program zapisuje w linii `NN` pliku przepisu. Do pliku trafia sama `treść`, bez prefiksu `NN:`, obcięta do 63 znaków i dopełniona spacjami. Potem program wypisuje na `stdout` "Krok NN zapisany", gdzie `NN` ma zawsze dwie cyfry (np. `07`). Błędne wiadomości pomija i wypisuje na `stderr` "Babcia się zagadała: <message>".

    *Jak pokazać:*
    ```
    terminal 1:  $ ./granny.sh tower1 8
    terminal 2:  $ ./sop-cake recipe.txt tower1
    terminal 2:  $ cat recipe.txt                     # zapisane kroki
    ```
3.  Program otwiera wszystkie podane łącza (tower1 ... tower5) i oczekuje na dane z nich wszystkich naraz, używając funkcji `poll`. Po wykryciu końca pliku na łączu program zamyka jego deskryptor, wypisuje na `stdout` "Babcia z wieży X skończyła dyktować" (X to ścieżka łącza) i usuwa go ze zbioru monitorowanego przez `poll`. Jeśli przez 3 sekundy na żadnym łączu nie pojawią się dane, program wypisuje na `stdout` "Babcie dolewają herbatki..." i dalej czeka.

    *Wskazówka:* kawałki wiadomości z różnych łączy mogą przychodzić naprzemiennie, więc każde łącze potrzebuje własnego bufora na niedokończoną wiadomość.

    *Jak pokazać:*
    ```
    terminal 1:  $ ./granny.sh tower1 8
    terminal 2:  $ ./granny.sh tower2 8
    terminal 3:  $ ./granny.sh tower3 8
    terminal 4:  $ ./sop-cake recipe.txt tower1 tower2 tower3
    ```
    Komunikat o dolewaniu herbatki najłatwiej zobaczyć, uruchamiając tylko jedną babcię.
4.  Gdy wszystkie łącza zostaną zamknięte, program mapuje plik przepisu do pamięci funkcją `mmap`. W pamięci zamienia każde wystąpienie `raw~berries` na `raspberries`. Następnie wypisuje na `stdout`:
    - "Brudy księżniczki Joanny wyczyszczone: N", gdzie N to liczba zamienionych wystąpień,
    - każdy niepusty krok (linię, która nie składa się z samych spacji) w formacie `NN: treść`, w kolejności numerów kroków, gdzie `NN` ma zawsze dwie cyfry, a `treść` jest wypisana bez spacji dopełniających z końca linii,
    - "Brakuje K kroków, tort sie zapadnie!", gdzie K to liczba pustych linii, albo "Tort gotowy!", jeśli żadna linia nie jest pusta.

    *Jak pokazać:*
    ```
    terminal 1:  $ ./granny.sh tower1 8
    terminal 2:  $ ./granny.sh tower2 8
    terminal 3:  $ ./granny.sh tower3 8
    terminal 4:  $ ./sop-cake recipe.txt tower1 tower2 tower3
    terminal 4:  $ grep -c '~' recipe.txt             # powinno wypisać 0
    ```

Pamiętaj, aby na końcu zwolnić wszystkie zasoby wywołując `munmap` i zamykając wszystkie otwarte deskryptory!


<em>kod do pliku <b>granny.sh</b></em>
{{< includecode "granny.sh" >}}
