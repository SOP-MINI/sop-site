---
title: "Zadanie testowe z tematu pipe/FIFO"
date: 2022-02-01T19:32:59+01:00
bookHidden: true
---

## Zadanie
W tym zadaniu zasymulujemy Laboratorium SOP. Laboratorium składa się z `n` (`3 <= n <= 20`, jedyny parametr programu) studentów oraz jednego nauczyciela. Studenci rozwiązują zadanie podzielone na cztery etapy, warte odpowiednio 3, 6, 7 i 5 punktów. Każdy student ma poziom umiejętności `k` (losowa liczba całkowita, `3 <= k <= 9`), który określa jego zdolność do rozwiązania zadania.

## Etapy
1. **Tworzenie procesów:**
   - Proces główny (nauczyciel) tworzy `n` procesów potomnych (studentów).
   - Każdy student wypisuje swój identyfikator procesu (PID) i kończy działanie.

2. **Sprawdzanie obecności:**
   - Studenci komunikują się z nauczycielem za pomocą łączy nienazwanych.
   - Wszyscy studenci piszą do jednego, wspólnego łącza, natomiast nauczyciel komunikuje się ze studentami przez indywidualne łącza.
   - Nauczyciel sprawdza listę obecności, wysyłając do każdego studenta oraz na standardowe wyjście: `Teacher: Is [PID] here?`.
   - Studenci odpowiadają `Student [PID]: HERE` zarówno przez łącze, jak i na standardowe wyjście.
   - Po sprawdzeniu obecności wszyscy kończą działanie.

3. **Rozwiązywanie zadań:**
   - Po sprawdzeniu obecności studenci rozpoczynają pracę nad zadaniem. Każdy etap zadania przebiega według następujących kroków:
     1. Wylosowanie liczby całkowitej `t` z zakresu `[100, 500]`.
     2. Oczekiwanie `t` milisekund.
     3. Wylosowanie liczby `q` z zakresu `[1, 20]`, oznaczającej wynik ich próby.
     4. Obliczenie wyniku próby jako `k + q`.
     5. Wysłanie swojego PID-a i wyniku próby do nauczyciela.
     6. Nauczyciel porównuje wynik próby studenta z poziomem trudności (`d`), który jest sumą punktów za dany etap oraz losowej liczby z zakresu `[1, 20]`.
     7. Jeśli `k + q >= d`, student przechodzi etap pomyślnie; w przeciwnym razie go nie zalicza. Informacja o tym jest wysyłąna z powrotem do studenta.
     8. Nauczyciel wypisuje jeden z komunikatów:
        - `Teacher: Student [PID] finished stage [X]` (gdy zaliczył).
        - `Teacher: Student [PID] needs to fix stage [X]` (gdy nie zaliczył).
     9. Jeśli student ukończył wszystkie cztery etapy, wypisuje `Student [PID]: I NAILED IT!` i kończy działanie.

   - Nauczyciel oczekuje na zakończenie wszystkich studentów. Gdy wszyscy skończą, nauczyciel wypisuje `Teacher: IT'S FINALLY OVER!` i kończy działanie.

4. **Ograniczenie czasowe:**
   - Laboratorium może zakończyć się, jeśli skończy się czas przewidziany na zadanie. Obsługę tego przypadku realizuje funkcja `alarm(2)`, uruchamiana po sprawdzeniu obecności.
   - Po otrzymaniu sygnału `SIGALRM` nauczyciel wypisuje `Teacher: END OF TIME!`, przestaje oceniać studentów, wypisuje tabelę wyników, zwalnia zasoby i kończy pracę.
   - Każdy ze studentów wykrywa, że nauczyciel zakończył pracę i wypisuje `Student [PID]: Oh no, I haven't finished stage [X]. I need more time.` przed zakończeniem działania.

## Przykładowe wyjście

```
./sop-lab 5
Student: 47807
Student: 47808
Student: 47809
Student: 47810
Teacher: 47806
Teacher: Is 47807 here?
Student 47807: HERE!
Teacher: Is 47808 here?
Student: 47811
Student 47808: HERE!
Teacher: Is 47809 here?
Student 47809: HERE!
Teacher: Is 47810 here?
Student 47810: HERE!
Teacher: Is 47811 here?
Student 47811: HERE!
Teacher: Student 47808 finished stage 1
Teacher: Student 47809 finished stage 1
Teacher: Student 47811 finished stage 1
Teacher: Student 47807 needs to fix stage 1
Teacher: Student 47810 finished stage 1
Teacher: Student 47809 finished stage 2
Teacher: Student 47807 finished stage 1
Teacher: Student 47808 needs to fix stage 2
Teacher: Student 47810 finished stage 2
Teacher: Student 47811 needs to fix stage 2
Teacher: Student 47807 needs to fix stage 2
Teacher: Student 47809 finished stage 3
Teacher: Student 47810 needs to fix stage 3
Teacher: Student 47808 needs to fix stage 2
Teacher: Student 47807 needs to fix stage 2
Teacher: Student 47809 needs to fix stage 4
Teacher: Student 47811 needs to fix stage 2
Teacher: Student 47807 needs to fix stage 2
Teacher: Student 47809 needs to fix stage 4
Teacher: Student 47807 finished stage 2
Teacher: Student 47811 needs to fix stage 2
Teacher: Student 47810 needs to fix stage 3
Teacher: Student 47808 needs to fix stage 2
Teacher: Student 47809 needs to fix stage 4
Teacher: Student 47807 finished stage 3
Teacher: Student 47811 finished stage 2
Teacher: Student 47810 needs to fix stage 3
Teacher: Student 47808 needs to fix stage 2
Student 47807: I NAILED IT!
Teacher: Student 47807 finished stage 4
Teacher: END OF TIME!
Teacher: 47807 - 21
Teacher: 47808 - 3
Teacher: 47809 - 16
Teacher: 47810 - 9
Teacher: 47811 - 9
Teacher: IT'S FINALLY OVER!
Student 47809: Oh no, I haven't finished stage 4. I need more time.
Student 47810: Oh no, I haven't finished stage 3. I need more time.
Student 47811: Oh no, I haven't finished stage 3. I need more time.
Student 47808: Oh no, I haven't finished stage 2. I need more time.
```
