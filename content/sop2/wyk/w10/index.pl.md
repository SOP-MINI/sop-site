---
title: "Wykład 10. Pamięć wirtualna"
date: 2022-02-04T22:11:05+01:00
---

## Zakres wykładu

  - Pamięć wirtualna - podstawy:
      - Motywacja: mniejsze zapełnienie pamięci	⇒ lepsze wykorzystanie, większy stopień wieloprogramowości, mniejszy narzut na operacje wejścia/wyjścia; przestrzeń adresów logicznych może być dużo większa od przestrzeni adresów fizycznych i rzadka
      - Realizacja: stronicowanie na żądanie, segmentacja na żądanie
      - Memory management unit (MMU)
  - Stronicowanie na żądanie:
      - bit poprawności, błąd braku strony (*page fault*), wymiana stron
      - leniwa wymiana (*lazy swapping*), proces stronicowania (*pager*)
      - lokalność odwołań a częstość błędu braku strony
      - komplikacje związane ze wznawianiem instrukcji
      - średnia sprawność stronicowania na żądanie, średni czas dostępu do pamięci
      - algorytmy zastępowania stron i ramek: FIFO, OPT, LRU i przybliżenia; efekt Belady'ego
      - buforowanie ramek, a surowy tryb dostępu do urządzeń wej/wyj
      - przydział ramek do procesu: równy/priorytetowy, lokalny/globalny; niejednorodny dostęp (NUMA), powinowactwo procesora/rdzenia do procesu/watku (*CPU affinity*).
      - szamotanie (*thrashing*): przyczyny, objawy, środki zaradcze; model zbioru roboczego, częstość błędów braku strony, planista średnioterminowy
  - Środki poprawy wykorzystania dostępu do pamięci:
      - odwzorowanie plików w pamięci; odwzorowanie urządzeń wej/wyj
      - kopiowanie przy zapisie (COW) a efektywność realizaji funkcji `fork()`
      - przydział pamięci na potrzeby jądra; algorytm bliźniaków (*buddy algorithm*), algorytm płytkowy (*slab algorithm*)
      - Wpływ implementacji algorytmów działających z dużymi strukturami danych na lokalność odwołań i efektywność dostępu do pamięci
  - Przykłady realizacji pamięci wirtualnej: Windows, Unix, Linux

## Materiały

1.  Podręcznik, rozdz. 10: "Pamięć wirtualna"
2.  Slajdy: [VM.pdf]({{< resource "VM_5.pdf" >}})
3.  Dla zainteresowanych: [Linux MM Documentation](https://linux-mm.org/LinuxMMDocumentation), [Understanding the Linux Virtual Memory Manager](http://ptgmedia.pearsoncmg.com/images/0131453483/downloads/gorman_book.pdf)
