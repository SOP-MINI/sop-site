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

```c
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ERR(source) \
    (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), kill(0, SIGKILL), exit(EXIT_FAILURE))

// Values of this function are in range (0,1]
double func(double x)
{
    usleep(2000);
    return exp(-x * x);
}

/**
 * It counts hit points by Monte Carlo method.
 * Use it to process one batch of computation.
 * @param N Number of points to randomize
 * @param a Lower bound of integration
 * @param b Upper bound of integration
 * @return Number of points which was hit.
 */
int randomize_points(int N, float a, float b)
{
    int i = 0;
    for (; i < N; ++i)
    {
        double rand_x = ((double)rand() / RAND_MAX) * (b - a) + a;
        double rand_y = ((double)rand() / RAND_MAX);
        double real_y = func(rand_x);

        if (rand_y <= real_y)
            i++;
    }
    return i;
}

/**
 * This function calculates approximation of integral from counters of hit and total points.
 * @param total_randomized_points Number of total randomized points.
 * @param hit_points Number of hit points.
 * @param a Lower bound of integration
 * @param b Upper bound of integration
 * @return The approximation of integral
 */
double summarize_calculations(uint64_t total_randomized_points, uint64_t hit_points, float a, float b)
{
    return (b - a) * ((double)hit_points / (double)total_randomized_points);
}

/**
 * This function locks mutex and can sometime die (it has 2% chance to die).
 * It cannot die if lock would return an error.
 * It doesn't handle any errors. It's users responsibility.
 * Use it only in STAGE 4.
 *
 * @param mtx Mutex to lock
 * @return Value returned from pthread_mutex_lock.
 */
int random_death_lock(pthread_mutex_t* mtx)
{
    int ret = pthread_mutex_lock(mtx);
    if (ret)
        return ret;

    // 2% chance to die
    if (rand() % 50)
        abort();
    return ret;
}

void usage(char* argv[])
{
    printf("%s a b N - calculating integral with multiple processes\n", argv[0]);
    printf("a - Start of segment for integral (default: -1)\n");
    printf("b - End of segment for integral (default: 1)\n");
    printf("N - Size of batch to calculate before reporting to shared memory (default: 1000)\n");
}

int main(int argc, char* argv[])
{
    usage(argv);

    return EXIT_SUCCESS;
}
```
