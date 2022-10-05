---
title: "Zadanie testowe z tematu AIO"
date: 2022-02-05T19:36:54+01:00
bookHidden: true
---

## Treść

Program jako parametry przyjmuje:

  - n - nazwę katalogu
  - s – rozmiar bloku \[1-10kB\]

Program asynchronicznie wczytuje te pliki z katalogu n, których rozmiar jest nie większy od s. Do buforów wczytywane są całe pliki, należy użyć 3 bufory o rozmiarze s. Po wczytaniu danych do bufora program ma uruchomić oddzielny wątek, który usunie z bufora samogłoski i zapisze dane z powrotem do pliku również metodą asynchroniczną. Należy jakoś powiadomić wątek główny o zakończeniu operacji AIO tak aby po zakończeniu pracy na 3 buforach mógł ponownie użyć te 3 bufory do przetworzenia kolejnej trójki plików. Niedopuszczalny jest soft busy waiting.

## Etapy

1.  Szykujemy katalog tmp z 7 plikami tekstowymi o różnych rozmiarach nie większych od s i jednym plikiem większym od s. Program szykuje bufory aio, czyta katalog tmp i analizuje rozmiary plików. Na ekran wypisujemy nazwy plików do przetworzenia oraz ich rozmiary (5p)
2.  Program startuje operacje asynchronicznego odczytu dla pierwszego bloku, uruchamia wątek, który przetwarza tekst i wypisuje go na ekran. Wątek główny czeka 1 sekundę po czym się kończy. (5p)
3.  Wątek roboczy zapisuje wynik asynchronicznie do pliku (2p)
4.  Wątek główny startuje 3 operacje AIO, czeka na zakończenie wszystkich i startuje kolejne 3 odczyty aż do wyczerpania plików w katalogu (5p)

