---
title: "Regulamin i zasady oceniania"
date: 2022-02-10T10:00:00+01:00
draft: false
layout: single
menu: sop1
---

## Ocena

**Ocena z przedmiotu** wynika z sumy punktów uzyskanych przez studenta z laboratorium (maksymalnie 100p):

| Suma punktów | Ocena |
| ------------ | ----- |
| s≥90         | 5     |
| 80≤ s \<90   | 4.5   |
| 70≤ s \<80   | 4     |
| 60≤ s \<70   | 3.5   |
| 50≤ s \<60   | 3     |
| s\<50        | 2     |

### Wykład

  - Wykłady mają wprowadzić studentów w tematykę przedmiotu, w szczególności w tematykę laboratorium. Dlatego spotkania w blokach 2-godzinnych odbywają się w pierwszej części semestru.
  - Nie ma sprawdzianów audytoryjnych (wykładowych). Treści wykładowe będą się jednak pojawiać na wejściówkach laboratoryjnych.

### Laboratorium

  - *UWAGA*, zajęcia laboratoryjne służą głownie ocenie samodzielnego przygotowania studenta z tematyki wskazanej na wykładzie i w materiałach przygotowawczych.
  - Wszystkie oceniane prace muszą być wykonywane **samodzielnie**.
  - Laboratorium składa się z nieocenianych zajęć wprowadzających (L0), **czterech** zajęć ocenianych (L1, L2, L3, L4) oraz terminu dodatkowego (L5).
  - Zadania laboratoryjne rozwiązywane są z użyciem systemu Arch Linux na komputerach w laboratorium. Nie jest dozwolone pokazywanie rozwiązań na własnym komputerze.
  - Zajęcia oceniane (L1/L2/L3/L4) rozpoczynają się punktowaną wejściówką przeprowadzaną w systemie Moodle PW. Wejściówka sprawdza wiedzę techniczną wymaganą na zajęciach - wykład, man, tutoriale i inne podane materiały. Następnie publikowana jest treść zadania programistycznego. Czas na jego wykonanie i przesłanie rozwiązania wynosi 90 minut.
  - **Za każde z zajęć ocenianych można zdobyć łącznie 25 punktów w tym 8 punktów za wejściówkę i 17 punktów za zadanie programistyczne.**
  - W trakcie rozwiązywania wejściówki **nie można** korzystać z żadnych materiałów pomocniczych. Niedozwolona jest komunikacja z innymi.
  - Zadanie programistyczne jest podzielone na etapy. Punkty za każdy etap podane są w treści zadania. Etapy muszą być wykonywane w wyznaczonej kolejności. Nie wolno przechodzić do następnego etapu bez wykonania poprzedników.
  - W czasie rozwiązywania zadań programistycznych **można** korzystać z tutoriala, własnych materiałów, rozwiązań zadań przykładowych, dokumentacji systemowej oraz Internetu jednak tak, by oceniana praca była dziełem samodzielnym.
  - Każdy etap jest sprawdzany i oceniany na zajęciach. Oceniana jest umiejętność korzystania z właściwych narzędzi i prawidłowa (w tym zgodna z POSIX) implementacja wymaganej funkcjonalności. Poza L3 ocenie nie podlega estetyka kodu.
  - Na koniec zajęć wersja kodu zawierająca wszystkie oceniane etapy musi zostać skopiowana do udziału sieciowego (więcej informacji na końcu strony). Brak implementacji którejś z funkcjonalności w wysłanym kodzie oznacza wyzerowanie punktów za dany etap.
  - Rozwiązanie zadania L3 będzie również ocenione pod względem estetyki kodu. Ocena będzie wykonana po zajęciach. Za problemy w kodzie otrzymuje się punkty karne, które są odejmowane od punktów uzyskanych na zajęciach. Można stracić do 10 punktów, jednak nie więcej niż liczba punktów uzyskanych na zajęciach.
  - Nieobecność na dwóch z zajęć ocenianych L1/L2/L3/L4 skutkuje niezaliczeniem przedmiotu.
  - Stwierdzenie przez prowadzącego niesamodzielności rozwiązania zadania laboratoryjnego skutkuje niezaliczeniem przedmiotu.
  - Zajęcia L5 przeznaczone są na poprawę ocen z laboratoriów. Odbywają się w taki sam sposób, jak pozostałe laboratoria, tzn. wejściówka+zadanie. Każdy student może poprawiać jeden wybrany przez siebie temat. Punkty z poprawy *zastępują* punkty z wybranego laboratorium. Można poprawiać tylko jedną część, tzn. tylko wejściówkę lub tylko program lub obie części (z tego samego tematu). **Na terminie poprawkowym nie obowiązuje podział na grupy tygodnia patrzystego/niepatrzystego. Zamiast tego na każdym z terminów będą poprawiane inne tematy.**
  - Zajęcia odbywają się (bez przerw) wg [harmonogramu]({{<ref "harmonogram" >}}).

## Forma oddawania zadania laboratoryjnego

  - Zadanie musi być wykonane w języku *C* (nie C++)
  - Rozwiązanie każdego zadania musi być przesłane w formie archiwum w formacie .tar.gz, .tar.xz lub .tar.bz2 tak aby po rozpakowaniu archiwum z flagami -xjf, -xJf lub -xzf utworzył się podkatalog o nazwie $USER zawierający wszystkie wymagane pliki. $USER jest loginem wydziałowym.
  - Archiwum musi zawierać wyłącznie pliki źródłowe (.c,.h) oraz plik Makefile
  - Plik Makefile ma pozwalać na kompilację całego rozwiązania pojedynczą komendą *make*
  - Podczas kompilacji gcc *obowiązkowo* ma być użyta flaga *-Wall* i inne flagi wymagane w zadaniu
  - Gotowe archiwum należy wgrać do katalogu na udziale sieciowym wskazanego w treści zadania.

</div>
