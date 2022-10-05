---
title: "Wykład 11. Systemy plików"
date: 2022-02-04T22:17:03+01:00
---

## Zakres wykładu

  - Plik, blok kontrolny, i-węzeł, system plików, organizacja warstwowa.
  - Wirtualny system plików (VFS): cele, budowa, rozszerzalność. FUSE
  - Implementacje katalogów: liniowa lista, tablica, tablica rzadka, drzewo (B+ czy in.)
  - Podstawowe metody przydziału miejsca dla plików: ciągły, listowy, indeksowy. Modyfikacje: porcje nadmiarowe (extents), FAT, indeksy wielopoziomowe.
  - Zarządzanie wolną przestrzenią: mapy bitowe, lista wolnych bloków, mapy obszarów (ZFS)
  - Efektywność i wydajność. Techniki optymalizacji dostępu sekwencyjnego: wczesne zwalnianie bufora (free-behind) i czytanie z wyprzedzeniem (read-ahead).
  - Buforowanie stron, a buforowanie dla operacji wejścia/wyjścia. Zalety/wady podejścia jednolitego.
  - Systemy plików z dziennikiem

  
## Materiały

1.  Podręcznik: rozdz. 12 - "Implementacja systemu plików", podrozdz. 20.7 - "Systemy plików"; podrozdz. 21.5 - "System plików", podrozdz. 22.3.3.4 - "Zarządca wejścia-wyjścia", podrozdz. 22.5 - "System plików"
2.  Slajdy: [FS\_1.pdf]({{< resource "FS_1_3.pdf" >}})
