---
title: "Zadanie testowe 4 z tematu system plików"
bookHidden: true
---

# L1: Ἡ Βιβλιοθήκη τῆς Ἀλεξάνδρειας

Jest trzeci wiek przed Chrystusem. Kallimach z Cyreny (Καλλίμαχος ὁ
Κυρηναῖος) właśnie ukończył swój słynny Pinakes (Πίνακες). Jest to
wielki katalog setek tysięcy zwojów Bilbioteki Aleksandryjskiej - w 120
tomach. Wymagało to ogromnego nakładu pracy, jednak jak można się
domyślić, tak wielki spis jest i tak niewygodny w użyciu. Kallimach
jednak nie traci zapału i już myśli o kolejnym usprawnieniu w
indeksowaniu zasobów Biblioteki. W tym celu zamierza wykorzystać
najnowsze osiągnięcie helleńskiej technologii - komputer z systemem
Λίνουξ.

Kallimach zajmie się wprowadzeniem informacji na temat zwojów do
komputera. Twoim zadaniem jest napisanie programu działającego w
systemie Λίνουξ i wykorzystującego API ΠΟΣΙΞ aby stworzyć odpowiednie
indeksy.

Pamiętaj o zwalnianiu wszystkich nieużywanych zasobów (pamięci,
deskryptorów), sprawdzaniu błędów funkcji systemowych i dobrych
praktykach programistycznych.

Etapy:

1. Na tym etapie program przyjmuje jeden argument -- ścieżkę do pliku
    będącego metadanymi książki z biblioteki. Są one zapisane przy użyciu
    prostego formatu -- w każdej linii występuje para `klucz:wartość`.
    Możesz założyć, że w linii nie występują nadmiarowe znaki `:` (jednak
    inne niepoprawne sytuacje należy obsłużyć). Odczytaj zawartość pliku, a
    następnie wypisz zawartość pól `author`, `title` oraz `genre` (w tej
    kolejności). W przypadku braku któregoś z pól wypisz dla niego
    `missing!`. Dla przykładowego pliku o zawartości:
    
        uthor:Plutarch
        latin_title:De fluviorum et montium nominibus et de iis, quae in illis inveniuntur
        title:Περὶ ποταμῶν καὶ ὀρῶν ἐπωνυμίας
        genre:geography
        incipit:When Chrysippe, through the anger of Aphrodite
        had fallen into a yearning 
        for Hydaspes
    
    należy wypisać:
    
        author: missing!
        title: Περὶ ποταμῶν καὶ ὀρῶν ἐπωνυμίας
        genre: geography
    
    (w pierwsze linii mamy literówkę, więc program nie znajduje pola
    `author`).
    
    *Podpowiedź*: Biblioteka standardowa zawiera sporo funkcji przydatnych
    do takiego parsowania jak `getline`, `strchr`, `strcmp` czy też
    `strdup`.
    
    *Podpowiedź 2*: Ta funkcjonalność jeszcze nam się przyda, chociaż nie od
    razu -- najlepiej umieść ją w osobnej funkcji.
    
2. Baza danych stworzona przez Kallimacha odzwierciedla w strukturze
    katalogów fizyczną strukturę Wielkiej Biblioteki. Poszczególne
    zagnieżdżone foldery symbolizują odpowiednie skrzydła budynku, pokoje,
    regały, półki, skrzynie\... Na końcu w katalogach znajdują się zwykłe
    pliki, których nazwa odzwierciedla tytuł zapisany na zewnętrznej części
    zwoju (nie musi być tożsama z prawdziwym tytułem książki zapisanym w
    metadanych).
    
    W tym etapie zmodyfikuj działanie programu -- program nie pobiera dłużej
    argumentów. Po uruchomieniu program rekurencyjnie przeszukuje katalog
    `library`. Wszystkie zwykłe pliki symbolizują książki. W katalogu
    programu stwórz katalog `index` (jeśli już istnieje zwróć błąd).
    Wewnątrz tego katalogu stwórz kolejny o nazwie `by-visible-title`.
    Umieść tam dowiązania symboliczne (względne) do wszystkich książek z
    biblioteki (plików) o takich samych nazwach jednak bez zagnieżdżeń.
    Jeżeli jakaś nazwa się powtarza należy zwrócić błąd.
    
    
3. Utwórz dwa nowe indeksy obok `by-visible-title`.
    
    -   Indeks `by-title` jest podobny do tego z poprzedniego etapu, jednak
        jako nazwy pliku używa pola `title` z metadanych. Jeżeli pole nie
        istnieje plik należy pominąć. Jeżeli tytuł jest dłuższy niż 64 bajty
        należy przyciąć go do tej długości.
    
    -   Indeks `by-genre` umieszcza książki w podfolderach postaci
        `index/by-genre/<gatunek>`, gdzie `<gatunek>` to wartość pola
        `genre` w metadanych. Nazwą pliku jest tytuł książki z metadanych
        jak wyżej. Wartość `genre` należy przyciąć w razie potrzeby do 64
        bajtów.
    
4. Ostatnią funkcjonalnością indeksu będzie sprawdzanie, czy żadnej książki
    nie brakuje. W tym celu inny programista już stworzył plik zawierający
    spis wszystkich książek w formacie binarnym. Na tym etapie pierwszy
    argument programu, gdy jest obecny, oznacza ścieżkę do pliku bazy.
    Należy wczytać jej zawartość i sprawdzić, czy żadnej książki nie
    brakuje. Format bazy jest następujący: występuje pewna ilość wpisów, z
    których każdy składa się 4 bajtów oznaczających rozmiar pliku (unsigned
    int) oraz z 64 pierwszych bajtów tytułu (z metadanych). W przypadku gdy
    tytuł jest krótszy niż 64 bajty pole jest uzupełniane zerami, tak więc
    każdy wpis ma zawsze 68 bajtów -- ich liczbę można wyznaczyć na
    podstawie rozmiaru pliku. Należy przejrzeć zawartość biblioteki. Za
    każdym razem gdy brakuje danej książki wypisać komunikat
    `Book "<tytuł>" is missing`, a gdy rozmiar się nie zgadza,
    `Book "<tytuł>" size mismatch (<oczekiwany rozmiar> vs <rozmiar pliku>)`.
    Do wczytania indeksu użyj funkcji `readv`. Do sprawdzenia, czy książek
    nie brakuje, użyj indeksu `by-title`.
    
    W plikach startowych są zawarte dwie przykładowe bazy danych. Pierwsza o
    nazwie `database_correct` zawiera wszystkie książki w katalogu
    `library`. Druga o nazwie `database_missing` zawiera dodatkowe książki,
    tak więc przy sprawdzaniu powinien być błąd.
    
    *Podpowiedź*: Aby nasz string był zawsze poprawnie zakończony zerem, w
    strukturze pojedynczej książki możemy zrobić tablicę 65 char-ów i
    ostatni wyzerować. W ten sposób nawet jeśli tytuł został przycięty i ma
    pełne 64 bajty, po wczytaniu string będzie poprawnie zakończony.


## Kod początkowy i pliki

- [sop1l1e4.zip](/files/sop1l1e4.zip)

