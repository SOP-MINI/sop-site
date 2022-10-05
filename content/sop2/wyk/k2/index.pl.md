---
title: "Zakres kolokwium 2"
date: 2022-02-05T16:04:15+01:00
---

- SOP1.W8: [Zarządzanie procesami i wątkami.]({{<ref "../w8">}})
- SOP2.W9: [Zarządzanie pamięcią operacyjną.]({{<ref "../w9">}})
- SOP2.W10: [Pamięć wirtualna.]({{<ref "../w10">}})
- SOP2.W11: [Systemy plików.]({{<ref "../w11">}})
- SOP2.W12: [Realizacje systemów plików.]({{<ref "../w12">}}) \[Temat "Rozproszone systemy plików" nie wchodzi w zakres kolokwium.\]
- SOP2.W13: [Systemy czasu rzeczywistego.]({{<ref "../w13">}})
- SOP2.W14: [Ochrona i bezpieczeństwo.]({{<ref "../w14">}})

Poza pytaniami otwartymi oraz pytaniami wielokrotnego wyboru kolokwium może zawierać zadania "obliczeniowe", np. podobne do poniższych. W związku z tym w czasie kolokwium należy mieć dostęp do kalkulatora.

  - W pewnym systemie komputerowym ze stronicowaniem na żądanie tablica stron jest przechowywana w szybkich rejestrach. Czas bezpośredniego dostępu do pamięci wynosi ....sekund. Czas obsługi błędu braku strony wynosi ... sekund, jeśli usuwanej strony nie trzeba zapisać, a ... sekund - jeśli potrzebny jest zapis. Można przyjąć, że w ...% przypadków potrzebny jest zapis wymienionej strony. Jaka jest maksymalna *częstość* błędów braku strony (liczba błędów na sekundę), która skutkuje średnim czasem dostępu: ...... sekund?
  - W pewnym systemie są aktualnie dostępne wolne obszary pamięci RAM o rozmiarach podanych w załączonej tabli. Określ w jaki sposób te obszary będą wykorzystane w trakcie realizacji *kolejnych* zamówień o wielkości ......, jeżeli przydział będzie wykonywany zgodnie z algorytmami: a) first-fit (FF), b) best-fit (BF), c) worst-fit (WF), d) buddy allocator (BA)
  - Ile transmisji dyskowych jest wymaganych do: 1) otwarcia dostępu do pliku systemu ext2, o ścieżce dostępu `/a/file` oraz 2) do odczytu 1MiB jego zawartości? Załóż, że blok alokacji dysku ma rozmiar 1KiB, blok indeksowy zajmuje cały klaster, katalog `a` zajmuje 4KiB, a adresy klastrów mają długość 4B. Przyjmij, że kontroler dysku przesyła do pamięci jeden klaster na raz i że kopia katalogu głównego znajduje się zawsze w pamięci jądra.
  - Zakładając, że ciąg odniesień do pamięci, to: 1,2,3,... wyznacz liczbę błędów braku strony przy zastosowaniu następujących algorytmów zastępowania stron: a) LRU, b) FIFO, c) optymalny. Przyjmij, że wszystkie N ramek jest początkowo pustych. N=.....
  - Procesy P1,P2,P3,P4 staną się gotowe do wykonania w podanych chwilach czasowych. W tablicy podano też długość fazy procesora każdego procesu i priorytet procesów. Narysuj diagramy Gantta odpowiadające szeregowaniu zgodnie z algorytmami omawianymi na wykładzie SOP1.W8 (FIFO, SJF, SRJF, RR, priorytetowy,...) Oblicz średni czas oczekiwania (albo średni czas obrotu, czy maksymalny czas oczekiwania) dla każdego z algorytmów.
