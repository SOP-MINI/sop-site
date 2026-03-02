---
title: "Synchronizacja"
date: 2022-02-10T10:00:00+01:00
weight: 20
bookCollapseSection: true
---

# Synchronizacja

## Zakres wykładu

- Problem sekcji krytycznej
- Trzy warunki poprawności: Wzajemne wykluczanie, Postęp, Ograniczone oczekiwanie
- Algorytm Petersona
- Brak spójności widoku pamięci (Cache, Store Buffers)
- Sprzętowa zmiana kolejności instrukcji 
- Sprzętowe modele pamięci
- Wpływ procesu translacji
- Model pamięci C11+
- Bariery pamięciowe
- Sprzętowe wsparcie synchronizacji: operacje Read-Modify-Write
- Atomic Swap (XCHG)[_index.en.md](_index.en.md)
- Test-and-Set (TAS) 
- Compare-and-Swap (CAS / cmpxchg)
- Spinlocki i problem aktywnego czekania
- Semafory systemowe Dijkstry
- Linux Futex (Fast Userspace Mutex)

## Materiały

1. [Slajdy wykładowe](slides)
2. [Programy przykładowe](code)
3. Podręcznik: rozdz. 7 (Synchronizacja procesów)
4. Stare slajdy: [Synchro_3.pdf]({{< resource "Synchro_3.pdf" >}})
