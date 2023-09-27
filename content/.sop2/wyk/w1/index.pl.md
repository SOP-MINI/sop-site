---
title: "Łącza"
date: 2022-02-10T10:00:00+01:00
weight: 10
---

# Wykład 1 - Wprowadzenie do komunikacji międzyprocesowej (IPC), łącza

## Zakres wykładu
  
### Treści podstawowe

  - Procesy niezależne i współpracujące. Powody współpracy.
  - Dwa podstawowe modele komunikacji międzyprocesowej (IPC): pamięć wspólna, kolejki komunikatów.
  - Główne cechy komunikacji za pomocą pamięci wspólnej.
  - Główne cechy komunikacji wykorzystującej kolejki komunikatów.
  - POSIX i łącza
      - POSIX - definicja i semantyka łącz (pipes/FIFOs). Tworzenie/usuwanie, dostęp plikowy (za pośrednictwem deskryptorów plików) bez możliwości pozycjonowania, konsekwencje ograniczonego buforowania, dostęp w trybie blokującym i nieblokującym.
      - Łącza (anonimowe/nienazwane, pipes) - tworzenie: `pipe()`, usuwanie: `close()` dla wszystkich odniesień. Pipe i `fork()` - możliwości wykorzystania. Przykłady. Skrót: `popen()`, `pclose()`
      - FIFOs (kolejki FIFO, łącza nazwane) - tworzenie: `mkpipe()`, usuwanie: `unlink()` a `close()` dla wszystkich deskryptorów związanych z FIFO.
      - Powiadomienia: sygnał `SIGPIPE`, kody niepowodzenia funkcji interfejsu: wartości `errno` równe `EPIPE`, `ESPIPE`.
      - Możliwość wystąpienia zakleszczenia procesów przy otwieraniu FIFO w (domyślnym) trybie blokującym.

### Treści dodatkowe

  - Tryb nieblokujący obsługi FIFO: za i przeciw.
  - Łącza w innych systemach operacyjnych (w tym MS Win)

### Materiały

1.  Podręcznik: rozdz. 4.4 (Procesy współpracujące), 4.5 (Komunikacja międzyprocesowa).
2.  Slajdy: [IPC\_wstęp.pdf]({{< resource "IPC_wstęp.pdf" >}}), [POSIX-łącza.pdf]({{< resource "Łącza_POSIX.pdf" >}})
3.  Rozdział 15 w dokumentacji *the GNU C library*: [Pipes and FIFOs](http://www.gnu.org/software/libc/manual/html_node/Pipes-and-FIFOs.html#Pipes-and-FIFOs)
4.  Litera uzupełniająca: rozdz. 4 (Łącza i kolejki FIFO) z: W.R. Stevens, Unix programowanie usług sieciowych, t. 2, wyd. 2: Komunikacja międzyprocesowa
