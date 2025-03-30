---
title: "Zadanie testowe z tematu kolejek POSIX"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---


## Kanał danych

Twoim zadaniem na dzisiejszych laboratoriach jest zaimplementowanie struktury kanału wewnątrz pamięci współdzielonej.  
Jest to miejsce, gdzie jeden proces *Producent* może umieszczać dane, a jeden z wielu procesów *Konsumentów* może je odczytywać.  
Jest to coś podobnego do rury z wodą w rzeczywistym świecie.

![Schemat kanału](/channel_schematic.png)

Ta struktura ma dwie podstawowe funkcje: `produce()` i `consume()`.  
Wywołanie `consume()` w momencie, gdy nie ma danych do przetworzenia, zablokuje wywołujący proces.  
Z drugiej strony, wywołanie `produce()` w momencie, gdy w kanale wciąż znajdują się dane, również zablokuje proces.  
Jeśli jeden proces wywoła `produce()` z jednej strony, a inny `consume()` z drugiej strony, dane zaczną *płynąć* (tak jak dane w `łączu` z L5).

Na naszych laboratoriach kanał może przechowywać **pojedynczy** ciąg znaków.  
Aby umieścić nowy ciąg znaków, poprzedni musi zostać odczytany przez konsumenta.  
Maksymalna długość przechowywanego ciągu wynosi 4096 bajtów.  
Twoim zadaniem jest zaimplementowanie programu (`ops-double-processor.c`), który:
1. Pobiera dane z kanału wejściowego (ta operacja może blokować, jeśli w kanale nie ma danych)
2. Podwaja każdy znak wejściowy (np. `"abc cda" -> "aabbcc  ccddaa"`)
3. Umieszcza zmodyfikowane dane w kanale wyjściowym (ta operacja może blokować, jeśli w kanale są już dane)

Dla ułatwienia masz dostęp do trzech programów: `ops-generator`, `ops-printer` i `ops-cleanup`.  
Pierwszy z nich odczytuje plik linia po linii i przesyła dane do kanału wyjściowego.  
Drugi pobiera dane z kanału wejściowego i wypisuje je na standardowe wyjście.  
Ostatni usuwa semafory i kanały o podanych nazwach.  
Uruchomienie tych programów bez argumentów pokazuje, jak ich używać.

## Etapy
1. Zaimplementuj funkcje otwierania (`channel_open`) i zamykania (`channel_close`) kanału (obiektu pamięci współdzielonej).  
   Jeśli obiekt pamięci współdzielonej o danej nazwie nie istnieje, powinien zostać utworzony i poprawnie zainicjalizowany.  
   Aby wyeliminować wyścig między `shm_open` a inicjalizacją kanału, użyj nazwanego semafora.

   > **Sprawdzenie:** `$ ./ops-double-process ch1 ch1 && ./ops-cleanup ch1 && ls /dev/shm`
   
   > **Wskazówka:** Po `mmap` struktury kanału możesz sprawdzić pole `status`, czy ma wartość  
   > `CHANNEL_UNINITIALIZED`, aby dowiedzieć się, czy wymaga inicjalizacji.

2. Zaimplementuj funkcję `channel_consume()`.  
   Powinna ona oczekiwać na `consumer_cv`, dopóki kanał nie zmieni statusu na `CHANNEL_OCCUPIED`.  
   Następnie kopiować dane z kanału do lokalnej pamięci procesu i sygnalizować inne procesy poprzez `producer_cv`.  
   Ta funkcja powinna zwracać `0`, jeśli dane zostały poprawnie odczytane.  
   Jeśli kanał jest wyczerpany (`status == CHANNEL_DEPLETED`), powinna zwrócić `1`.  
   Wypisz odebrane dane na standardowe wyjście.

   > **Sprawdzenie:** `$ ./ops-generator ops-generator.c ch1 & ./ops-double-processor ch1 ch1`

   > **Wskazówka:** Powinno to wyglądać niemal identycznie jak uruchomienie:  
   > `$ ./ops-generator ops-generator.c ch1 & ./ops-printer ch1`

3. Zaimplementuj funkcję `channel_produce()`.  
   Powinna ona oczekiwać na `producer_cv`, dopóki kanał nie zmieni statusu na `CHANNEL_EMPTY`.  
   Następnie skopiować dane z prywatnej pamięci procesu do pola `data` i zasygnalizować jeden proces przez `consumer_cv`.

   Teraz zamiast wypisywać na standardowe wyjście, umieść otrzymane dane w kanale wyjściowym.  
   Po pobraniu ostatniego elementu z kanału wejściowego (`consume` zwróciło wyczerpanie kanału wejściowego)  
   oznacz kanał wyjściowy jako wyczerpany.

   > **Sprawdzenie:** `$ ./ops-generator ops-generator.c ch1 & ./ops-double-processor ch1 ch2 & ./ops-printer ch2`

   > **Wskazówka:** Po oznaczeniu kanału wyjściowego jako wyczerpanego pamiętaj o sygnalizacji wszystkich konsumentów na drugim końcu.

4. Zaimplementuj logikę duplikacji danych wejściowych.

   > **Sprawdzenie:** 
   > ```
   > $ ./ops-generator ops-generator.c ch1 & ./ops-double-processor ch1 ch2 & \
   >   ./ops-printer ch2
   > $ ./ops-generator ops-generator.c ch1 & ./ops-double-processor ch1 ch2 & \
   >   ./ops-printer ch2 & ./ops-printer ch2 & ./ops-printer ch2
   > ```

   > **Wskazówka:** Pamiętaj o ograniczeniach rozmiaru kanału.  
   > Wynik duplikacji będzie dwa razy dłuższy niż wejście.  
   > Rozważ podział na więcej wywołań `produce`.

## Kod początkowy

- [sop2l3e3.zip](/files/sop2l3e3.zip)
