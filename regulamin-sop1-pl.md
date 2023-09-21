# Przeznaczenie przedmiotu

Celem pierwszej części przedmiotu *Systemy Operacyjne* jest zapoznanie studentów z podstawami działania i konstrukcji współczesnych systemów operacyjnych, a także kształtowanie umiejętności wykorzystania funkcji systemowych do poprawnej realizacji prostych aplikacji wieloprocesowych/wielowątkowych, wykorzystujących standardowe kanały wejścia/wyjścia i dostęp do plików.

# Prowadzenie zajęć

Przedmiot składa się z wykładu i laboratorium.

  - Wykłady mają wprowadzić studentów w tematykę przedmiotu, w szczególności w tematykę laboratorium.
    - Kurs składa się z 15 godzin wykładowych w 2-godzinnych blokach. Ostatni wykład trwa 1 godzinę. Zajęcia odbywają się zgodnie z harmonogramem podanym na stronie przedmiotu.
    - Na stronie przedmiotu znajdują się omawiane materiały wykładowe.
    - Nie ma sprawdzianów audytoryjnych (wykładowych). Treści wykładowe będą się jednak pojawiać na wejściówkach laboratoryjnych.

  - Laboratorium składa się z pięciu zajęć oznaczonych kolejno L0, L1, L2, L3, L4. Zajęcia odbywają się zgodnie z harmonogramem podanym na stronie przedmiotu. Na stronie przedmiotu znajdują się też materiały przygotowawcze do zajęć.
  - Zajęcia L0 to zajęcia poświęcone na przygotowanie środowiska. W trakcie tych zajęć studenci przypominają sobie umiejętności z przedmiotu Podstawy systemu unix i konfigurują środowisko programistyczne. Wszelkie braki w konfiguracji lub tych umiejętnościach (np. brak podpowiadania nazw funkcji, podkreślania błędów w edytorze, czy brak umiejętności poruszania się po katalogach) na wszystkich kolejnych zajęciach nie są okolicznością łagodzącą przy ocenianiu. Druga część L0 ma postać warsztatów na której prowadzący wyjaśnia podstawy systemu kontroli wersji git oraz oceniania kolejnych zajęć na podstawie prostego zadania przykładowego.
  - Zajęcia L1, L2, L3 i L4 służą ocenie umiejętności w obszarze wskazanym w harmonogramie przedmiotu. Do każdych zajęć należy przygotować się samodzielnie, korzystając z materiałów na stronie przedmiotu i materiałów wykładowych. Każde z zajęć składa się z dwóch części:
    - wejściówka (8 minut): rozwiązywanie testu w moodle
    - zadanie programistyczne (120 minut)
  - W sesji egzaminacyjnej jest organizowana poprawa, na której można poprawiać oceny uzyskane na dokładnie jednych zajęciach (L1, L2, L3, lub L4). Można poprawiać albo jedną część (wejściówkę, program) albo obie części. Zapisy na poprawy zostaną uruchomione od razu po ostatnich zajęciach laboratoryjnych w semestrze i potrwają **24 godziny**.


# Materiały do przedmiotu

  - Materiały dostarczane w trakcie wykładu (np. slajdy, fragmenty kodu)
  - Literatura podana na wykładzie i dostępna na stronie przedmiotu
  - Opisy ćwiczeń laboratoryjnych na stronie przedmiotu
  - [The GNU C library manual](http://www.gnu.org/software/libc/manual/)
  - [The Single UNIX specification, Version 3](http://www.unix.org/version3/online.html) (zawiera IEEE Std 1003.1 and ISO/IEC 9945)
  - [POSIX FAQ](http://www.opengroup.org/austin/papers/posix_faq.html)


# Obecność na zajęciach

  - Obecność na wykładach nie jest obowiązkowa ale zalecana ze względu na ścisłe powiązanie omawianego materiału z laboratorium.
  - Obecność na zajęciach laboratoryjnych jest obowiązkowa. W przypadku nieobecności na zajęciach student otrzymuje 0 punktów za aktywności realizowane na zajęciach, na których był nieobecny.
  - Nieobecność na dwóch z zajęć laboratoryjnych ocenianych L1/L2/L3/L4 skutkuje niezaliczeniem przedmiotu.


# Weryfikacja osiągnięcia efektów uczenia się

Weryfikacja w trakcie laboratorium. W trakcie semestru są cztery zajęcia laboratoryjne, realizujące 4 tematy, oznaczone skrótami L1, L2, L3 i L4.

Metody weryfikacji efektów nauczania na laboratorium:

### Wejściówka
  - Test weryfikujący wiedzę z wykładu i z materiałów przygotowawczych do laboratorium. Każda wejściówka obejmuje wszystkie wykłady poprzedzające i wszystkie poprzedzające tematy laboratorium oraz bieżący temat laboratorium.
  - Pytania zamknięte, wielokrotnego wyboru (poprawna może być dowolna liczba odpowiedzi większa od 0), wykonywany na platformie moodle. 
  - Wejściówka składa się z 4 pytań, za każde z pytań można uzyskać wynik z przedziału [0,2] punkty. W sumie, za cały test, maksymalnie 8 punktów. 
  - Wejściówka jest punktowana w następujący sposób. W każdym pytaniu wielokrotnego wyboru jest *n* poprawnych odpowiedzi i *m* niepoprawnych odpowiedzi (każda odpowiedź jest albo poprawna albo niepoprawna). Za każdą zaznaczoną poprawną odpowiedź jest 2/n punktów, a za każdą zaznaczoną niepoprawną odpowiedź jest -2/m punktów. Za każde pytanie przyznawana jest suma punktów wynikająca z zaznaczonych niepoprawnych i poprawnych odpowiedzi, o ile uzyskana suma nie jest ujemna. W takiej sytuacji przyznawane jest 0 punktów.
  - Wejściówka trwa 8 minut.

### Zadanie programistyczne

  - Za zadanie uzyskać można od 0 do 17 punktów
  - Na wykonanie zadania jest 120 minut
  - Zadania laboratoryjne rozwiązywane są z użyciem systemu Arch Linux na komputerach w laboratorium. Nie jest dozwolone rozwiązywanie zadań na własnym komputerze.
  - Zadanie programistyczne jest podzielone na etapy. Punkty za każdy etap podane są w treści zadania. Etapy muszą być wykonywane w wyznaczonej kolejności. Nie wolno przechodzić do następnego etapu bez wykonania poprzednich.
  - Każdy etap jest sprawdzany i oceniany na zajęciach. Oceniana jest umiejętność korzystania z właściwych narzędzi i prawidłowa (w tym zgodna z POSIX) implementacja wymaganej funkcjonalności. Poza L3 ocenie nie podlega estetyka kodu.
  - Prowadzący zajęcia dostarcza startowe pliki - wstępną wersję repozytorium git.
  - Po ukończeniu każdego etapu studenci przesyłają rozwiązanie na serwer przy użyciu systemu kontroli git, w sposób opisany przez prowadzącego na L0. Zgłoszenia podlegają następującym zasadom:
    - Rozwiązania nie można zgłosić częściej niż co minutę.
    - Dozwolona jest modyfikacja jedynie plików rozwiązania.
    - Pliki muszą być odpowiednio sformatowane - zgodnie z konfiguracją programu clang-format znajdującą się w startowym repozytorium zadania (program jest dostępny na komputerach laboratoryjnych, jego działanie jest wyjaśniane podczas L0).
    - Program przy kompilacji przy użyciu pliku makefile zawartego w repozytorium nie powinien zwracać żadnych błędów.
     Jeżeli rozwiązanie nie spełnia któregoś z warunków jest odrzucane, a student dostaje informację zwrotną o zaistniałym problemie. Przesłanie rozwiązania na serwer jest warunkiem wstępnym oceny danego etapu, rozwiązania nieprzesłane nie podlegają ocenie.
  - Rozwiązanie zadania L3 będzie również ocenione pod względem estetyki kodu. Ocena będzie wykonana po zajęciach. Za problemy w kodzie otrzymuje się punkty karne, które są odejmowane od punktów uzyskanych na zajęciach. Można stracić do 10 punktów, jednak nie więcej niż liczba punktów uzyskanych na zajęciach.


# Pomoce dopuszczone do użycia podczas weryfikacji osiągnięcia efektów nauczania

- W trakcie rozwiązywania wejściówki **nie można** korzystać z żadnych materiałów pomocniczych. Niedozwolona jest komunikacja z innymi.
- W czasie rozwiązywania zadań programistycznych **można** korzystać z tutoriala, własnych materiałów, rozwiązań zadań przykładowych, dokumentacji systemowej oraz Internetu jednak tak, by oceniana praca była dziełem samodzielnym - w szczególności niedozwolone jest używanie rozwiązań oparty o AI, jak chat GPT, Github Copilot i podobnych.

# Zasady zaliczenia przedmiotu i wystawiania oceny końcowej

 - Do zaliczenia przedmiotu koniecznie jest uzyskanie w sumie przynajmniej 50 punktów (na 100 możliwych), z zastrzeżeniem dopuszczalnej liczby nieobecności.
 - Ocena z poprawy zastępuje ocenę z poprawianych zajęć.
 - **Ocena końcowa z przedmiotu** wynika z sumy punktów uzyskanych przez studenta z laboratorium:
   - Ocena 5.0 - wynik w przedziale [90, 100]
   - Ocena 4.5 - wynik w przedziale [80, 90)
   - Ocena 4.0 - wynik w przedziale [70, 80)
   - Ocena 3.5 - wynik w przedziale [60, 70)
   - Ocena 3.0 - wynik w przedziale [50, 60)
   - Ocena 2.0 - wynik w przedziale [0, 50)


# Termin i tryb ogłaszania ocen

- Oceny cząstkowe

  - Liczba punktów uzyskana za kolejne etapy zadania laboratoryjnego jest podawana w momencie sprawdzania przez prowadzącego i zapisywana na kartce z treścią zadania
  - Liczba punktów uzyskana za wejściówkę jest wyświetlana w moodle po zakończeniu testu
  - Punkty z obu części laboratorium są umieszczane w usosie najpóźniej 24 godziny po zajęciach. Liczba punktów wpisana w usosie determinuje ocenę końcową
- Ocena końcowa
  - Wystawiana w USOSie w ciągu 24 godzin od opublikowania wyników ostatniego laboratorium L4 w USOSie
  - Korygowana w trakcie sesji egzaminacyjnej w ciągu 24 godzin od opublikowania wyników laboratorium poprawkowego w USOSie


# Zasady powtarzania zajęć z powodu niezaliczenia przedmiotu

Osoba, która nie zaliczyła przedmiotu, musi powtórzyć całość przedmiotu i przystąpić do wszystkich zaliczeń. Nie ma możliwości przepisywania punktów cząstkowych z poprzednich semestrów.

# Inne 
