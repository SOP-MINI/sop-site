---
title: "Zadanie testowe z tematu Wątki muteksy i sygnały"
date: 2022-02-05T19:36:50+01:00
bookHidden: true
---

## Treść

W tym zadaniu będziemy symulować pisanie odtwarzacza filmów. Aby
osiągnąć dobrą wydajność oraz stabilność odtwarzania, praca w tego typu
programach jest zwykle rozłożona na kilka wątków.

Program powinien składać się z czterech wątków:

1.  Wątek główny - tworzy pozostałe wątki i jest odpowiedzialny za
    obsługę sygnałów.
2.  Wątek dekodujący - zajmuje się dekodowaniem klatek filmu
    (`decode_frame()`) i przesyłaniem ich dalej.
3.  Wątek transformujący - odbiera klatki od wątku dekodującego,
    transformuje je (`transform_frame()`) i przesyła dalej.
4.  Wątek wyświetlający - odbiera klatki od wątku transformującego i
    wysyła je na ekran (`display_frame()`) pilnując stałego klatkażu 30
    FPS (30 klatek na sekundę).

Nagłówek `video-player.h` zawiera definicje makr, funkcji i struktur,
których powinieneś/powinnaś użyć. Modyfikacja tego pliku jest
niedozwolona, cała funkcjonalność powinna być zawarta w pliku
`sop-vp.c`.

Aby przekazywać klatki pomiędzy wątkami wykorzystaj *bufor cykliczny*
(ang. *circular buffer*). Jest to bardzo użyteczna struktura, mająca
wiele zastosowań w aplikacjach wymagających przetwarzania danych w
czasie rzeczywistym. Składa się z bufora (tablicy, w naszym przypadku
tablicy wskaźników na strukturę `Frame` rozmiaru `BUFFER_SIZE`),
wskaźników (indeksów) na początek (*głowę, head*) oraz koniec (*ogon,
tail*) bufora, licznika ilości elementów oraz obiektów niezbędnych do
zapewnienia synchronizacji. Założeniem struktury jest, że dane są
,,zawinięte” - po dojściu do ostatniego elementu w buforze (tablicy)
zaczynamy wstawianie od początku.

W tym zadaniu zaimplementuj bufor cykliczny w następujący sposób:

- Podczas tworzenia struktury ustaw *head* oraz *tail* na zero. *Head*
  oznacza miejsce, na które wstawimy następny element, natomiast
  *tail* - miejsce na którym znajduje się ostatni element.

- Do synchronizacji użyjemy tylko jednego mutexu - dla bufora (tablicy)
  oraz, na drodze wyjątku, techniki ,,*busy waiting*”. Nie jest to
  optymalny sposób na implementację tej struktury danych, jako ćwiczenie
  przed następnym laboratorium, możesz spróbować poprawić ten program
  przy wykorzystaniu innych technik synchronizacji.

- Aby dodać nowy element (*push*) do bufora, poczekaj aż nie będzie
  pełny (*busy waiting* z 5ms usypianiem wątku). Dodaj nowy element na
  miejsce wskazywane przez *head*, przesuń *head* na nową pozycję i
  zwiększ licznik elementów.

- Podobnie, aby pobrać element (*pop*) z bufora, poczekaj aż nie będzie
  pusty (*busy waiting* z 5ms usypianiem wątku). Pobierz element
  wskazywany przez *tail*, przesuń *tail* na następną pozycję i zmniejsz
  licznik elementów.

- Przemyśl, jak powyższe funkcje powinny być synchronizowane, aby
  zapewnić bezpieczeństwo i uniknąć deadlocka, przy jednocześnie
  możliwie krótkiej sekcji krytycznej. Możesz założyć, że bufor jest
  używany jedynie przez jeden wątek producenta (wywołujący *push*) oraz
  jeden konsumenta (wywołujacy *pop*) na raz.

Wskazówka: w przypadku tego zadania implementujemy bardzo szczególny
przypadek bufora cyklicznego. W internecie możesz znaleźć wiele innych
rodzajów tej struktury. Pamiętaj jednak, że na tym laboratorium
powinieneś ją zaimplementować dokładnie tak, jak to zostało wyżej
opisane.

Synchronizacja wyświetlania (w etapie 2) oznacza, że klatki filmu
powinny być wyświetlane tak szybko jak to możliwe, ale nie częściej niż
co 33.33ms (czyli 30 na sekundę).

W rozwiązaniu nie powinno być innych zmiennych globalnych niż ewentualna
jedna flaga w etapie 4 (wskazówka: ona też nie jest potrzebna).

## Etapy

1. Stwórz 3 wątki jak w opisie zadania. Na tym etapie dane mogą być
   przesyłane między wątkami przez współdzieloną zmienną chronioną mutexem.
   W przypadku braku dostępnych danych wątek czeka 5ms i próbuje ponownie.
   Mutexy nie powinny być zablokowane dłużej, niż to konieczne.

2. Dodaj synchronizację wyświetlania w wątku wyświetlającym (podpowiedź:
   `clock_gettime`).

3. Zaimplementuj bufor cykliczny tak, jak to opisano w
   treści zadania.

4. Główny wątek obsługuje sygnały - po otrzymaniu SIGINT
   (ctrl+c) główny wątek sygnalizuje pozostałym wątkom koniec pracy, czeka
   na ich zakończenie, zwalnia wszystkie zasoby i się kończy.

## Kod początkowy

- [sop1l3e2pl.zip](/files/sop1l3e2pl.zip)
