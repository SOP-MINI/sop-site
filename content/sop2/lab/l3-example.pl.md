---
title: "Zadanie testowe z tematu pamięć dzielona i mmap"
bookHidden: true
---

## Treść

Napisz dwa programy: klienta i serwera, komunikujące się przy użyciu pamięci współdzielonej POSIX.

Serwer przyjmuje argumenty `N` oraz `M` i tworzy obiekt pamięci współdzielonej o nazwie `PID`, gdzie `PID` to identyfikator procesu. Ustawia on rozmiar tej pamięci na `N`\*12 bajtów. Następnie wypisuje on nazwę obiektu pamięci współdzielonej.

Serwer wypełnia `N` 12-bajtowych fragmentów pamięci w następujący sposób:

| Numer bajtu | 0-4 | 4-8 | 8-12 |
| - | - | - | - |
| Zawartosć | Losowa liczba całkowita z zakresu od 0 do `M` | Losowa liczba całkowita z zakresu od 0 do `M` | Zera (miejsce na liczbę wynikową) |

Serwer czeka na otrzymanie sygnału `SIGINT`. Następnie wypisuje sumę liczb wynikowych (parz niżej) na stdout, usuwa utworzoną przez siebie pamięć współdzieloną i kończy działanie.

Program klienta jest wywoływany z dwoma parametrami - nazwą fragmentu pamięci serwera oraz `K` - liczbą  z zakresu od 0 do `N`-1. Klient otwiera fragment pamięci i odczytuje dwie liczby całkowite z `K`\*12 oraz `K`\*12+4. Następnie mnoży je przez siebie, zapisuje wynik w `K`\*12+8 i kończy działanie.

## Etapy

1. Serwer tworzy obszar pamięci współdzielonej, ustawia jego rozmiar i wypisuje jego nazwę. Po upływie dwóch sekund usuwa go i kończy działanie. Klient próbuje otworzyć obszar pamięci, wypisuje odpowiednio `Sukces` lub `Porażka` i kończy działanie. 
2. Serwer wypełnia pamięć współdzieloną zgodnie z treścią zadania. Klient odczytuje odpowiednie liczby i wypisuje wynik mnożenia na `stdout`. Na tym etapie program ignoruje wszystkie błędy.
3. Serwer kończy działanie po otrzymaniu `SIGINT`. Klient zapisuje wynik działania na odpowiednim adresie w pamięci współdzielonej.
4. Serwer wypisuje sumę obliczonych wyników. Pamięć współdzielona jest usuwana przy zamykaniu serwera. Pełna obsługa błędów.
