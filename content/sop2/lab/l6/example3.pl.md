---
title: "Zadanie testowe z tematu kolejek POSIX"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Głuchy Telefon  
> Gra "Głuchy Telefon" polega na tym, że gracze ustawiają się w linii i przekazują sobie wiadomość od jednej osoby do drugiej, co zazwyczaj skutkuje zabawnym zniekształceniem oryginalnej wiadomości, gdy trafia ona do ostatniego gracza.  

Napisz program, który symuluje grę w Głuchy Telefon, wykorzystując kolejki komunikatów POSIX.  

Program uruchamia się z trzema parametrami `P`, `T₁`, `T₂`, gdzie `0 ≤ P ≤ 100` oraz `100 ≤ T₁ < T₂ ≤ 6000`.  

Po uruchomieniu nazwy dzieci uczestniczących w grze są wprowadzane (każda w nowej linii) na standardowe wejście.  

Wpisanie polecenia `start "Very long sentence to pass"` rozpoczyna grę. Koordynator (główny proces) przekazuje podane zdanie w niezmienionej formie pierwszemu dziecku (w kolejności, w jakiej dołączyły do gry). Każde słowo jest wysyłane w osobnej wiadomości.  

Po zakończeniu gry koordynator wyświetla wynik, a wszystkie procesy zostają zakończone.  

Kolejka komunikatów może przechowywać jednocześnie tylko 2 słowa (wiadomości).  

Można założyć, że maksymalna długość jednej linii standardowego wejścia wynosi 255 znaków.  

**Uwaga:** Program powinien obsługiwać wiele instancji bez kolizji nazw kolejek i konfliktów w działaniu!  

---

## Etapy:  

1. Główny proces (koordynator gry) poprawnie tworzy nowe procesy na podstawie podanych nazw dzieci i oczekuje na ich zakończenie.  
   - Nie ma określonego limitu (poza zasobami systemowymi) liczby dzieci biorących udział w grze.  
   - Aby rozpocząć grę, musi być co najmniej jedno dziecko. Każde dziecko zna tylko swoje imię i PID dziecka, od którego odbiera wiadomości. W przypadku pierwszego dziecka jest to PID koordynatora.  
   - Dziecko wyświetla komunikat `[{PID}] {name} has joined the game!`, czeka losowy czas pomiędzy `T₁` a `T₂`, a następnie kończy działanie, wyświetlając komunikat `[{PID}] {name} has left the game!`.  

2. Dziecko, po dołączeniu do gry, tworzy nową kolejkę nazwaną `"sop_cwg_{PID}"`.  
   - Dziecko zna tylko PID poprzedniego dziecka – to informuje je, z której kolejki ma odbierać wiadomości.  
   - Wpisanie `start {message_to_pass}` rozpoczyna przekazywanie wiadomości słowo po słowie do pierwszego dziecka.  
   - Tylko pierwsze dziecko otrzymuje wiadomość od koordynatora na tym etapie.  
   - Po odebraniu słowa, dziecko wyświetla je na standardowym wyjściu: `[{PID}] {name} got the message: '{word}'`.  

3. Słowa są przekazywane między dziećmi, a następnie wracają do koordynatora.  
   - Dla każdej litery w słowie istnieje `P%` szansa, że zostanie ona zmieniona na losową literę alfabetu łacińskiego.  
   - Koordynator wyświetla odebraną wiadomość na standardowym wyjściu. Wszystkie użyte zasoby są prawidłowo zwalniane.  

4. Po otrzymaniu sygnału `SIGINT` wszystkie dzieci opuszczają grę.  
   - Koordynator wyświetla część wiadomości, którą już otrzymał, i wychodzi.  
   - Można użyć limitu czasowego na odbiór wiadomości, aby uniknąć zakleszczenia w tym etapie.  

---

### Przykładowe Wejście (`example_input.txt`):  

```plaintext
Anna
Frank
Simon
Katie
Tom
Matt
start "Systemy operacyjne 1 i 2 to moje ulubione kursy podczas studiów; nauczyłem się tutaj wielu ciekawych rzeczy!"
```

### Oczekiwane Wyjście:  

```plaintext
./sop-cwg-stage4 2 250 500 < example_input.txt
Chinese Whispers Game
Provide child name or type start to begin the game!
[29406] Anna has joined the game!
[29407] Frank has joined the game!
[29408] Simon has joined the game!
[29409] Katie has joined the game!
[29410] Tom has joined the game!
[29411] Matt has joined the game!
Game has started with 6 childrens!
Passing "Operating Systems 1 and 2 are my favorite courses during my studies; I have learned a lot of fun stuff here!" to the first child!
Passing word "Operating"
Passing word "Systems"
Passing word "1"
[29406] Anna got message: Operating
Passing word "and"
[29406] Anna got message: Systems
Passing word "2"
[29407] Frank got message: Operating
Passing word "are"
[29406] Anna got message: 1
[29407] Frank got message: Systems
[29408] Simon got message: Operating
Passing word "my"
[29407] Frank got message: 1
[29406] Anna got message: and
[29408] Simon got message: Systems
[29409] Katie got message: Operating
[29407] Frank got message: and
[29406] Anna got message: 2
Passing word "favorite"
[29410] Tom got message: Operating
[29408] Simon got message: 1
[29409] Katie got message: Systems
[29411] Matt got message: Operating
...
[29408] Simon has left the game!
[29411] Matt got message: fun
[29409] Katie got message: heraa
[29410] Tom got message: stucf
[29411] Matt got message: stucf
[29410] Tom got message: hewaa
[29411] Matt got message: hewaa
[29409] Katie has left the game!
[29410] Tom has left the game!
[29411] Matt has left the game!
Final message    : Operating Systems 1 and 2 lre my favorite courbes duhing my studies; I zave learned a lot ou fun stuuf hewaa
Original message : Operating Systems 1 and 2 are my favorite courses during my studies; I have learned a lot of fun stuff here!
Game has ended!
```