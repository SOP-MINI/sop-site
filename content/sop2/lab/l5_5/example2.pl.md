---
title: "Zadanie testowe z tematu kolejek POSIX"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Pracownicy

Stwórz system dystrybucji zadań dla procesów pracowników z wykorzystaniem kolejek wiadomości POSIX.  
System będzie symulować pracę przez dodawanie dwóch losowych liczb zmiennoprzecinkowych (zakres 0-100.0) oraz uśpienie procesu na losowy czas (500 ms - 2000 ms).  

Główny proces (serwer) co losowy czas (`T₁` - `T₂` ms, gdzie `100 <= T₁ < T₂ <= 5000`) dodaje nowe zadanie do kolejki.  
Każde zadanie to para losowych liczb zmiennoprzecinkowych z zakresu `[0.0, 100.0]`.  

Na początku tworzone są procesy dzieci (N pracowników, gdzie `2 <= N <= 20`), które rejestrują się w kolejce zadań o nazwie `"task_queue_{server_pid}"`.  

Pracownicy, oczekując na zadania, pobierają je z kolejki, gdy są dostępne i nie są zajęci.  
Każdy pracownik zwraca wyniki przez swoją własną kolejkę o nazwie `"result_queue_{server_pid}_{worker_id}"`.  

**Uwaga:** Program powinien obsługiwać wiele instancji bez kolizji nazw kolejek!  

### Etapy:

1. Główny proces tworzy N pracowników, którzy po wykonaniu losowego uśpienia (500 ms - 2000 ms) kończą pracę.  
   Na starcie, serwer wypisuje `"Server is starting..."` a po zakończeniu pracy wszystkich pracowników: `"All child processes have finished."`  
   Pracownicy przy starcie wypisują `"[{worker_pid}] Worker ready!"` i `"[{worker_pid}] Exits!"` przy zakończeniu.  

2. Serwer tworzy `5 * N` zadań, każde w losowych odstępach czasu (`T₁` do `T₂` ms), i dodaje je do kolejki.  
   Serwer informuje o dodaniu zadania: `"New task queued: [{v1}, {v2}]"` lub o pełnej kolejce: `"Queue is full!"`.   
   Pracownicy przy otrzymaniu zadania informują: `"[{worker_pid}] Received task [{v1}, {v2}]"` śpią losowo (500-2000ms), 
   wypisują wynik `"[{worker_pid}] Result [{result_value}]"` i oczekują na kolejne zadanie. Kończą pracę po wykonaniu 5 zadań.  

3. Pracownicy wysyłają wyniki do serwera przez indywidualne kolejki i informują o tym na standardowym wyjściu: `"[{worker_pid}] Result sent [{value}]"`  
   Serwer odbiera wyniki i wyświetla: `"Result from worker {worker_pid}: {value}"`  

4. Główny proces kontynuuje tworzenie zadań aż do otrzymania sygnału `SIGINT`, po czym informuje pracowników o zakończeniu pracy (poprzez kolejkę).  
   Serwer czeka na zakończenie aktualnych zadań pracowników, a następnie kończy działanie.  
   Pracownicy kończą pracę po otrzymaniu informacji o zakończeniu od serwera (doliczają do końca jedynie rozpoczęte zadania - zadania z kolejki są ignorowane).  
   Wszystkie zasoby są poprawnie zwalniane.  

### Przykładowe działanie programu:

```sh
./sop-qwp 3 500 2000
Server is starting...
[5831] Worker ready!
[5832] Worker ready!
New task queued: [84.65, 17.80]
[5831] Received task [84.65, 17.80]
[5833] Worker ready!
[5831] Result sent [102.46]
Result from worker 0: 102.46
New task queued: [25.87, 10.67]
[5832] Received task [25.87, 10.67]
New task queued: [84.04, 7.21]
[5833] Received task [84.04, 7.21]
[5832] Result sent [36.55]
Result from worker 1: 36.55
[5833] Result sent [91.25]
Result from worker 2: 91.25
New task queued: [83.46, 35.65]
[5831] Received task [83.46, 35.65]
New task queued: [38.44, 27.78]
[5832] Received task [38.44, 27.78]
[5831] Result sent [119.12]
Result from worker 0: 119.12
^CClosing server...
[5833] Exits
[5831] Exits
[5832] Result sent [66.23]
[5832] Exits
Result from worker 1: 66.23
All child processes have finished.
```
