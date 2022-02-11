---
title: "GCC Sanitizers"
date: 2022-02-05T17:31:35+01:00
draft: false
---

Oprócz podstawowej diagnostyki w trakcie kompilacji (opcja -Wall kompilatora, która włącza sporo komunikatów ostrzegawczych, sugerujących problemy z kodem), na zajęciach wymagamy użycia sanitizera (w wolnym tłumaczeniu uzdrowiciela) wbudowanego w nowsze kompilatory. Mechanizm ten powoduje, że kod wynikowy programu zostanie rozbudowany o dodatkowe sprawdzenia poprawności i w przypadku niezgodności zostanie wygenerowany błąd. Po dokładną listę elementów, które można w ten sposób sprawdzić, odsyłamy do dokumentacji kompilatora.

Na Systemach operacyjnych będziemy korzystać przede wszystkim z opcji -fsanitize=address i -fsanitize=undefined. Pierwszy zestaw odpowiada za sprawdzanie różnych problemów z dostępem do pamięci, a druga sygnalizuje wykonywanie niektórych z operacji, których wyniku standard nie definiuje lub definiuje wynik nieokreślony. W przeciwieństwie do opcji -Wall, która wpływa tylko na komunikaty w procesie kompilacji, *uzdrowiciel* zmienia kod wynikowy generowany przez kompilator. Uzyskany program będzie miał gorszą wydajność ze względu na dodatkowe sprawdzenia, w związku z tym w wersjach programu kompilowanych dla użytkowników końcowych, należy rozważnie dobierać te opcje. Na szczęście na laboratorium nie jest to problemem.

Ogromnym zyskiem z używania *uzdrowicieli* jest to, że możemy wykrywać problemy, których wykrycie z użyciem analizy w trakcie kompilacji byłoby albo niemożliwe (konkretnie nieobliczalne, o definicji takich problemów będzie Teorii automatów i języków formalnych) albo bardzo kosztowne. Z drugiej strony sprawdzenia te zależą od tego jak wykonuje się program (konkretne dane wejściowe wpływają na przebieg programu).

Przykładowy program z niebezpiecznym czytaniem z stdin:
```c
#include <stdio.h>

int main(int argc, char** argv) {
  int x=5;
  char name[10];
  scanf("%s", name); // Reading stirng of unknown (probably exceeding 9) length
  printf("You typed: %s\nThe number is %d\n", name, x);
  return 0;
}
```

Jeśli skompilujemy ten program bez opcji -fsanitize, to podanie napisu dłuższego niż 9 znaków spowoduje nadpisanie pamięci, gdzie znajduje się liczba x. Oczywiście w tym programie łatwo znaleźć problem, ale w ogólności nadpisywanie pomięci poza zakresem bufora powoduje mnóstwo problemów. Teraz skompilujmy nasz program z takimi opcjami:  
`  gcc -Wall -fsanitize=address,undefined array-problem.c -o array-problem `  
Teraz podanie na stdin napisu dłuższego niż bufor powoduje zgłoszenie błędu (warto nauczyć się czytać te błędy przez zajęciami). Pewną niedogodnością jest fakt, że w podanym stosie wywołania programu brakuje numerów linii (załóżmy, że prawdziwe programy mają jednak więcej niż 3 linie). Informacje debugowe, m.in. numery linii, można dołączyć opcją -g kompilatora `  gcc -g -Wall -fsanitize=address,undefined array-problem.c -o array-problem `

*Do każdego z zadań na laboratorium będzie podana lista opcji -fsanitize, którą należy użyć. Uwaga: nie można włączyć wszystkich możliwych, ponieważ część opcji jest ze sobą w konflikcie.*

