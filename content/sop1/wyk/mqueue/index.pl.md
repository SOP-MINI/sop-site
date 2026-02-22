---
title: "Kolejki komunikatów"
date: 2022-02-10T10:00:00+01:00
weight: 100
---
  
# Kolejki komunikatów

## Zakres wykładu
  
- Wiadomości podstawowe o interfejsie IPC w standardzie POSIX.
    - Trzy mechanizmy IPC: kolejki komunikatów, pamięć wspólna, semafory.
    - Dwie rodziny interfejsów: `UNIX System V IPC`, `POSIX IPC`
    - Trwałość obiektów IPC: jądra, procesu, systemu plików.
    - Przestrzenie nazw, widoczność obiektów (w szczególności w systemie plików)
- Kolejki komunikatów i pamięć wspólna - interfejs `POSIX IPC`:
    - Przestrzeń nazw, trwałość, widoczność obiektów
    - Komunikat, priorytet komunikatu, przesyłanie komunikatów (buforowanie o ograniczonej pojemności, dostęp do komunikatów kolejki (rola priorytetu), asynchroniczne powiadamianie o wstawieniu komunikatu do pustej kolejki), usuwanie kolejki i tego skutki.
    - Blok pamięci współdzielonej: tworzenie, dołączanie/odłączanie, tryby dostępu i konsekwencje naruszenia ograniczeń, zaznaczanie do usunięcia, a usuwanie bloku z systemu.
    - Wywołanie funkcji systemowych `fork()`, `exec()`, `_exit()`, a obiekty IPC.
    - (\*\*) Budowa i wykorzystanie interfejsów kolejki komunikatów i pamięci wspólnej POSIX:
        - Obserwacja obiektów IPC w systemie plików
        - Funkcje systemowe kolejek komunikatów (`mq_*()`) i pamięci wspólnej(`shm_*()`, `ftruncate()`, `mmap()` i in.)
        - Wykorzystanie asynchronicznego powiadamianie o wstawieniu komunikatu do pustej kolejki.
- Kolejki komunikatów i pamięć wspólna - interfejs `UNIX System V IPC`
    - Przestrzeń nazw, trwałość, widoczność obiektów
    - Komunikat, typ komunikatu, przesyłanie komunikatów (buforowanie o ograniczonej pojemności, dostęp wybiórczy do komunikatów: porządek FIFO albo wg typu komunikatu), usuwanie kolejki i tego skutki.
    - Segment pamięci współdzielonej: tworzenie, dołączanie/odłączanie segmentu, tryby dostępu i konsekwencje naruszenia ograniczeń, zaznaczanie do usunięcia, a usuwanie segmentu z systemu.
    - (\*) Budowa i wykorzystanie interfejsu kolejki komunikatów i pamięci wspólnej
        - Obserwacja obiektów IPC w systemie; polecenia systemowe (`ipcs`, `ipcrm`)
        - Funkcje systemowe kolejek komunikatów (`msg*()`) i pamięci wspólnej (`shm*()`) i niektóre struktury danych interfejsu.

### Materiały

1.  Slajdy: [IPC\_1.pdf]({{< resource "IPC_1.pdf" >}})
2.  Litera uzupełniająca: W.R. Stevens, Unix programowanie usług sieciowych, t. 2, wyd. 2: Komunikacja międzyprocesowa Rozdziały: 1 (Podstawowe określenia), 2 (Posiksowa komunikacja międzyprocesowa), 3 (Komunikacja międzyprocesowa w Systemie V), 5 (Kolejki komunikatów standardu POSIX), 6 (Kolejki komunikatów w Systemie V), 12 (Podstawy na temat pamięci wspólnej), 13 (Posiksowa pamięć wspólna), 14 (Pamięć wspólna Systemu V)
