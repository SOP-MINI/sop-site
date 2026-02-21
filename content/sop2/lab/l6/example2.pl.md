---
title: "Zadanie testowe z tematu pamięć dzielona i mmap"
bookHidden: true
math: true
---

## Wspólne całkowanie

Celem zadania jest napisanie programu do całkowania numerycznego metodą Monte Carlo funkcji:

{{< katex >}}\int_a^b{e^{-x^2}dx}{{< /katex >}}

Program powinien wykorzystać pamięć dzieloną do współpracy z większą ilością procesów.
Każdy kolejny uruchomiony program powinien przyłączyć się do obliczeń i je przyśpieszyć.

Obliczenia to prywatna część pamięci każdego procesu.
Po sprawdzeniu `N` wylosowanych próbek program powinien zaktualizować liczniki wylosowanych oraz trafionych próbek (`batch processing`).

W celu koordynacji obliczeń w pamięci dzielonej poza informacją o wyniku obliczeń należy przetrzymywać licznik współpracujących procesów. 
Przy rozpoczęciu pracy każdy proces powinien zwiększać licznik oraz zmniejszać go po prawidłowym zakończeniu pracy.
Kiedy proces zmniejsza go do zera, należy podsumować wyniki obliczeń i wypisać na ekran wynik aproksymacji.

## Etapy

1. Do współpracy między procesami wykorzystaj obiekt nazwanej pamięci dzielonej.  
   Przygotuj strukturę pamięci dzielonej zawierającą licznik procesów chronionym współdzielonym mutexem.

   Napisz procedurę inicjalizacji pamięci dzielonej z prawidłowa inkrementacją liczników przy uruchomieniu kolejnych procesów.  
   Do wyeliminowania wyścigu pomiędzy stworzeniem pamięci dzielonej a jej inicjalizacją, użyj semafora nazwanego.

   Po prawidłowej inicjalizacji pamięci dzielonej proces wypisuje ilość współpracujących procesów, śpi 2 sekundy, a następnie kończy się.  
   Przed zakończeniem procesu należy zniszczyć obiekt pamięci dzielonej w przypadku odłączania się ostatniego procesu.

2. Zaimplementuj obliczenia trzech paczek (ang. *batches*) obliczeń `N` punktów metodą Monte Carlo.  
   Pobierz parametry do programu w taki sposób jak opisuje funkcja `usage`.  
   Wykorzystaj dostarczoną funkcję `randomize_points` do obliczenia jednej paczki próbek.

   Dodaj do struktury pamięci dzielonej dwa liczniki opisujące ilość próbek wylosowanych oraz trafionych.  
   Po obliczeniu każdej paczki zaktualizuj liczniki oraz wyświetl ich stan na standardowe wyjście.  
   Po wykonaniu trzech iteracji obliczeń program powinien się skończyć z logiką niszczenia pamięci dzielonej jak w etapie pierwszym.

   > **W pamięci dzielonej warto przechować zakresy całkowania, aby uniknąć przyłączenia się procesu z innymi granicami całkowania. Taki scenariusz spowoduje, że wyniki aproksymacji nie będą miały sensu.**
3. Dodaj obsługę sygnału `SIGINT`, który przerwie obliczanie kolejnych paczek punktów.

   W tym etapie program powinien przybliżać całkę do momentu otrzymania sygnału.  
   Wystarczająco dobrą implementacją jest dokończenie aktualnie obliczanej paczki i zaniechanie wzięcia kolejnej.

   Jeżeli proces odłącza się ostatni z pamięci dzielonej, wypisz wynik na standardowe wyjście.

4. Dodaj obsługę śmierci procesu w momencie blokowania mutexu w pamięci dzielonej poza procedurą inicjalizacji.  
   Zmień mutexy, aby były typu robust oraz obsłuż sytuację śmierci właściciela.  
   Przy natrafieniu na taka sytuację załóż, że należy zdekrementować licznik procesów, aby nadal prawidłowo wykonać podsumowanie na końcu pracy programu.  
   Aby zasymulować nagłą śmierć procesu, użyj funkcji `random_death_lock`, która należy zablokować każdy mutex poza inicjalizacją pamięci dzielonej.
  
## Kod początkowy

{{< includecode "example2-code.c" >}}
