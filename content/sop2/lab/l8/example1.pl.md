---
title: "Zadanie testowe UDP"
bookHidden: true
---

## 100 dni Napoleona

1 marca 1815, ku zaskoczeniu świata, Napoleon Bonaparte wylądował z niewielkim oddziałem w Juan we Francji.
Witany entuzjastycznie przez francuzów szybko z powrotem przejął władzę.
Rozpoczął się okres znany dziś jako ,,100 dni Napoleona'', zakończony bitwą pod Waterloo.

W tym zadaniu będziemy symulować dostarczanie meldunków do sztabu Napoleona w czasie bitwy.
Meldunki są przekazywane przez posłańców.
W chaosie bitwy często zdarza się, że posłańcowi nie uda się dotrzeć.
Dlatego dostarczanie meldunków będziemy symulować przez protokół UDP.

## Etapy

1. Program serwera przyjmuje jeden parametr - numer portu. 
Program oczekuje na datagramy na podanym porcie.
Wiadomości mają postać `<X> <Y> <P> <nazwa oddziału>`.
`X` i `Y` to współrzędne na mapie (liczby naturalnie z zakresu od 0 do 99) natomiast `<nazwa oddziału>` to tekst nie dłuższy niż 128 znaków.
`P` to oznacza przynależność oddziału i może wynosić `0` (wrogi) lub `1` sojuszniczy.
Wiadomość oznacza, że podany oddział (sojuszniczy lub wrogi) przesunął się na daną pozycję.
Po otrzymaniu datagramu sparsuj wiadomość i wypisz na terminal komunikat postaci `<Nasz/wrogi> oddział <nazwa oddziału> był widziany na pozycji <X>:<Y>`.
Jeśli wiadomość jest źle sformatowana wypisz komunikat o błędzie, ale nie kończ programu!

2. Przy tak zaciętej bitwie nawet w samym sztabie panuje spore zamieszanie.
Przychodzący posłańcy wrzucają raporty na stos przy wejściu, skąd są one zabierane przez czterech adiutantów, którzy na tej podstawie aktualizują mapy sztabowe.
Zaimplementuj pulę wątków adiutantów.
Po otrzymaniu wiadomości serwer dodaje ją na stos (o rozmiarze `STACK_SIZE` równym 16).
Wątki adiutantów czekają na nową wiadomość - ten który ją odbierze przeprowadza parsowanie i wypisuje wiadomość jak w etapie pierwszym.
Do synchronizacji użyj muteksu (do ochrony stosu) oraz semafora lub zmiennej warunkowej (wskaźnik na wierzch stosu).

3. Dodaj aktualizację map sztabowych.
Stwórz współdzieloną między wątkami tablicę oddziałów - o rozmiarze `DIVISION_NAMES_SIZE` równym 128.
Po otrzymaniu nowego meldunku adiutant pracuje (czyli śpi przez 10ms).
Następnie sprawdza, czy nazwa oddziału znajduje się już w tablicy, jeśli nie dopisuję ją na koniec (pamiętaj o synchronizacji!).
Dodaj współdzieloną mapę sztabową - tablicę dwuwymiarową o rozmiarze `100`x`100` wypełnioną na początku zerami.
Adiutant aktualizuje pozycję oddziału na mapie - tj. szuka jego numeru, zeruje dane pole, po czym wpisuje numer oddziału na współrzędne z wiadomości.
Żeby zapewnić synchronizację dodaj muteks na każdy wiersz mapy.

4. Dodaj wątek Napoleona.
Zapamiętuj adresy z których przyszedł ostatni meldunek dla danego oddziału.
Cesarz Francuzów co 30ms wypisuje stan mapy.
Następnie wybiera losowy sojuszniczy oddział i wysyła do niego rozkaz postaci `<X> <Y> <P> <nazwa oddziału>`.

