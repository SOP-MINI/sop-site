---
title: "Zadanie testowe nr 2 z tematu Procesy, sygnały i deskryptory"
date: 2022-02-05T19:34:07+01:00
bookHidden: true
---

## Treść

Zadanie polega na napisaniu programu w standardzie POSIX, który symuluje rozprzestrzenianie się wirusa grypy w przedszkolu. Główny proces powinien odpowiadać za obsługę przedszkola, a `n` procesów potomnych za symulowanie dzieci przebywających w przedszkolu. Symulacja trwa `t` sekund.

Gdy dziecko jest chore, zaczyna kasłać co pewien czas (losowo, co 50-200 ms) i kaszle na wszystkie inne dzieci w przedszkolu – wysyłając sygnał SIGUSR1. Gdy zdrowe dziecko otrzyma wirusa (sygnał), istnieje prawdopodobieństwo `p`, że samo zachoruje i zacznie kaszleć. Po chore dziecko przyjeżdżają rodzice po `k` sekundach i je zabierają.

Program jako argumenty powinien przyjąć wartości t, k, n oraz p, gdzie:
* t - to czas symulacji w sekundach (1 - 100), 
* k - to czas po jakim przyjeżdżają rodzice po chore dziecko (1 - 100), 
* n - to liczba dzieci w przedszkolu (1 – 30),
* p - to wartość prawdopodobieństwa wyrażona w procentach, że dziecko po kontakcie z wirusem zachoruje (1 – 100).

Należy założyć, że zawsze jedno dziecko jest chore na starcie, dla uproszenia można przyjąć, że jest to pierwsze z listy.

Główny proces po czasie `t` sekund wysyła SIGTERM do wszystkich dzieci, który oznacza zakończenie symulacji. Dziecko po otrzymaniu sygnału, kończy swoje działanie i zwraca jako exit status liczbę kaszlnięć (aby przekazać ją do głównego procesu). W celu odliczania czasu użyj funkcji alarm.

W momencie odebrania dziecka przez rodziców, dziecko wyświetla o tym fakcie informację oraz kończy swoje działanie wracając odpowiedni exit code. Procesy dzieci, które zakończą się przed wysłaniem sygnału kończącego symulacją należy uznać za odebrane.

Gdy wszystkie procesy dzieci zakończą swoje działanie po czasie `t`, proces główny wyświetla listę wszystkich dzieci wraz z ich stanem, w poniższym formacie, gdzie Child ID to PID:

    No. | Child ID | Status
      1 |    17487 | Coughed 3 times and parents picked them up!
      2 |    17488 | Coughed 1 times and is still in the kindergarten!
      3 |    17489 | Coughed 2 times and is still in the kindergarten!
    2 out of 3 children stayed at in the kindergarten!

Uwaga 1: Należy zadbać, aby funkcje obsługujące sygnały były jak najlżejsze, zawierały jedynie proste operacje. W szczególności niedopuszczalne jest używanie funkcji z grupy signal-unsafe (patrz man 7 signal-safety).
Uwaga 2: Aby rozsyłać sygnały do wszystkich dzieci użyj ID grupy.
Uwaga 3: Aby uzyskać PID procesu z którego przyszedł sygnał użyj struktury siginfo_t.
Wykorzystaj poniższe komunikaty w swojej aplikacji: 

    Child[{PID}] starts day in the kindergarten, ill: {is_ill}
    Child[{PID}]: {PID} has coughed at me!
    Child[{PID}] get sick!
    Child[{PID}] is coughing {x}
    Child[{PID}] exits with {exit_code}
    KG[{PID}]: Alarm has been set for {t} sec
    KG[{PID}]: Simulation has ended!
    Coughed {x} times and parents picked them up!
    Coughed {x} times and is still in the kindergarten!
    {y} out of {z} children stayed in the kindergarten!

Przykład działania programu:

    ./sop-kg 10 2 4 10
    Child[10000] starts day in the kindergarten, ill: 1
    Child[10001] starts day in the kindergarten, ill: 0
    Child[10002] starts day in the kindergarten, ill: 0
    KG[9999]: Alarm has been set for 10 sec
    Child[10003] starts day in the kindergarten, ill: 0
    Child[10000] is coughing (1)
    Child[10003]: 10000 has coughed at me!
    Child[10002]: 10000 has coughed at me!
    Child[10001]: 10000 has coughed at me!
    Child[10000] is coughing (2)
    Child[10003]: 10000 has coughed at me!
    Child[10002]: 10000 has coughed at me!
    Child[10001]: 10000 has coughed at me!
    Child[10000] is coughing (3)
    Child[10003]: 10000 has coughed at me!
    Child[10002]: 10000 has coughed at me!
    Child[10001]: 10000 has coughed at me!
    Child[10002] get sick!
    Child[10000] is coughing (4)
    Child[10003]: 10000 has coughed at me!
    Child[10001]: 10000 has coughed at me!
    Child[10000] is coughing (5)
    Child[10003]: 10000 has coughed at me!
    Child[10001]: 10000 has coughed at me!
    Child[10000] is coughing (6)
    Child[10000] exits with 6
    Child[10001]: 10000 has coughed at me!
    Child[10003]: 10000 has coughed at me!
    Child[10002] is coughing (1)
    Child[10003]: 10002 has coughed at me!
    Child[10001]: 10002 has coughed at me!
    Child[10002] is coughing (2)
    Child[10003]: 10002 has coughed at me!
    Child[10001]: 10002 has coughed at me!
    Child[10002] is coughing (3)
    Child[10003]: 10002 has coughed at me!
    Child[10001]: 10002 has coughed at me!
    Child[10002] is coughing (4)
    Child[10002] exits with 4
    Child[10003]: 10002 has coughed at me!
    Child[10001]: 10002 has coughed at me!
    KG[9999]: Simulation has ended!
    Child[10003] exits with 0
    Child[10001] exits with 0
    No. | Child ID | Status
      1 |    10000 | Coughed 6 times and parents picked them up!
      2 |    10001 | Coughed 0 times and is still in the kindergarten!
      3 |    10002 | Coughed 4 times and parents picked them up!
      4 |    10003 | Coughed 0 times and is still in the kindergarten!
    2 out of 4 children stayed in the kindergarten!

## Oceniane etapy

1. Tworzone są poprawnie procesy dzieci, wypisują swój PID i zasypiają na losowy czas (300-1000 ms), główny proces czeka na ich zakończenie. **(4p)**
2. Proces główny wysyła po określonym czasie (alarm w procesie głównym) sygnał SIGTERM do wszystkich dzieci, a dzieci czekają na jego otrzymanie. **(3p)**
3. Działa mechanizm kaszlenia - roznoszenia wirusa. **(5p)**
4. Dzieci po określonym czasie zostają odebrane przez rodziców (alarm w procesach dzieci). **(4p)**
