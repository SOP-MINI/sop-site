# Przeznaczenie przedmiotu

Celem drugiej części przedmiotu *Systemy Operacyjne* jest zapoznanie studentów z zaawansowanymi problemami implementacji współczesnych systemów operacyjnych, a także kształtowanie umiejętności wykorzystania zaawansowanych mechanizmów systemowych do poprawnej realizacji (w środowisku POSIX/UNIX/Linux) aplikacji wieloprocesowych/wielowątkowych, wykorzystujących różne środki komunikacji międzyprocesowej i synchronizacji oraz komunikację sieciową. Wymagana jest znajomość języka C (przedmiot Programowanie 1) oraz podstawowych API POSIX (dotyczących plików, wątków, procesów i sygnałów nauczanych na przedmiocie Systemy Operacyjne 1)

# Prowadzenie zajęć

Przedmiot składa się z wykładu i laboratorium.

  - Wykład składa się z 30 godzin wykladowych w 2-godzinnych blokach. Zajęcia odbywają się zgodnie z harmonogramem podanym na stronie przedmiotu.
    - Na stronie przedmiotu znajdują się omawiane materiały wykładowe.
    - W semestrze odbędą się 2 sprawdziany wykładowe w terminach określonych w harmonogramie.
    - Każdy student ma prawo do jednej poprawy każdego ze sprawdzianów w dodatkowym terminie (terminie sprawdzianu poprawkowego). Obowiązuje ten sam zakres materiału co w pierwotnym terminie. Student musi zgłosić wykładowcy zamiar przystąpienia do tego sprawdzianu najpóźniej jedną dobę przed tym terminem. Ocena zdobyta na sprawdzianie poprawkowym zastępuje poprzednią - jeśli jest lepsza.
    - Suma punktów do zdobycia za sprawdziany wykładowe wynosi 100.
  - Laboratorium składa się z czterech zajęć oznaczonych kolejno L1, L2, L3, L4. Zajęcia odbywają się zgodnie z harmonogramem podanym na stronie przedmiotu. Na stronie przedmiotu znajdują się też materiały przygotowawcze do zajęć.
  - Zajęcia służą ocenie umiejętności w obszarze wskazanym w harmonogramie przedmiotu. Do każdych zajęć należy przygotować się samodzielnie, korzystając z materiałów na stronie przedmiotu i materiałów wykładowych. Zajęcia polegają na rozwiazaniu zadania programistycznego (130 minut).
  - Student ma prawo przystąpić do poprawy. Zadanie na laboratorium poprawkowym może dotyczyć wszystkich tematów omawianych w trakcie semestru. Wynik z poprawy zastępuje najgorszy wynik uzyskany przez studenta w semestrze.

# Materiały do przedmiotu

  - Materiały dostarczane w trakcie wykładu (np. slajdy, fragmenty kodu)
  - Literatura podana na wykładzie i dostępna na stronie przedmiotu
  - Opisy ćwiczeń laboratoryjnych na stronie przedmiotu
  - [The GNU C library manual](http://www.gnu.org/software/libc/manual/)
  - [The Single UNIX specification, Version 3](http://www.unix.org/version3/online.html) (zawiera IEEE Std 1003.1 and ISO/IEC 9945)
  - [POSIX FAQ](http://www.opengroup.org/austin/papers/posix_faq.html)


# Obecność na zajęciach

  - Obecność na wykładach nie jest obowiązkowa ale zalecana ze względu na ścisłe powiązanie omawianego materiału z laboratorium.
  - Obecność na wykładach na których odbywają się sprawdziany jest obowiązkowa. W przypadku nieobecności student otrzymuje 0 punktów za dany sprawdzian wykładowy. 
  - Obecność na zajęciach laboratoryjnych jest obowiązkowa. W przypadku nieobecności na zajęciach student otrzymuje 0 punktów za aktywności realizowane na zajęciach, na których był nieobecny.
  - Nieobecność na dwóch z zajęć laboratoryjnych ocenianych L1/L2/L3/L4 skutkuje niezaliczeniem przedmiotu.


# Weryfikacja osiągnięcia efektów uczenia się

Weryfikacja w trakcie wykładów i laboratorium.

## Wykład

W trakcie semestru są dwa sprawdziany wykładowe. 

  - Sprawdziany mają formę pisemną i składają się z kilku/kilkunastu pytań otwartych i zamkniętych dotyczących treści omawianych na wykładzie do danego testu. 
  - Pytania mogą dotyczyć również podstawowych treści omawianych na przedmiocie poprzedzającym - Systemy Operacyjne 1
  - Za dwa sprawdziany można otrzymać łącznie 100 punktów - punktacja za każde pytanie jest podana w treści zadań.

## Laboratorium

### Zadanie programistyczne

  - Za zadanie uzyskać można od 0 do 25 punktów
  - Na wykonanie zadania jest 130 minut
  - Zadania laboratoryjne rozwiązywane są z użyciem systemu Arch Linux na komputerach w laboratorium. Nie jest dozwolone rozwiązywanie zadań na własnym komputerze.
  - Zadanie programistyczne jest podzielone na etapy. Punkty za każdy etap podane są w treści zadania. Etapy muszą być wykonywane w wyznaczonej kolejności. Nie wolno przechodzić do następnego etapu bez wykonania poprzednich.
  - Każdy etap jest sprawdzany i oceniany na zajęciach. Oceniana jest umiejętność korzystania z właściwych narzędzi i prawidłowa (w tym zgodna z POSIX) implementacja wymaganej funkcjonalności.
  - Po ukończeniu każdego etapu studenci przesyłają rozwiązanie na serwer przy użyciu systemu kontroli git. Zgłoszenia podlegają następującym zasadom:
    - Program musi być napisany w języku C (nie C++).
    - Rozwiązania nie można zgłosić częściej niż co minutę.
    - Dozwolona jest modyfikacja jedynie plików rozwiązania.
    - Pliki muszą być odpowiednio sformatowane - zgodnie z konfiguracją programu clang-format znajdującą się w startowym repozytorium zadania (program jest dostępny na komputerach laboratoryjnych, jego działanie jest wyjaśniane podczas L0).
    - Program przy kompilacji przy użyciu pliku makefile zawartego w repozytorium nie powinien zwracać żadnych błędów.
     Jeżeli rozwiązanie nie spełnia któregoś z warunków, jest odrzucane, a student dostaje informację zwrotną o zaistniałym problemie. Przesłanie rozwiązania na serwer jest warunkiem wstępnym oceny danego etapu, rozwiązania nieprzesłane nie podlegają ocenie.
  - W trakcie laboratorium, na żądanie prowadzącego, studenci są zobowiązani uruchomić dostarczone z zadaniem oprogramowanie monitorujące ich pracę.

# Pomoce dopuszczone do użycia podczas weryfikacji osiągnięcia efektów uczenia się

  - W trakcie rozwiązywania testów wykładowych **nie można** korzystać z żadnych materiałów pomocniczych. Niedozwolona jest komunikacja z innymi.
  - W trakcie rozwiązywania wejściówki **nie można** korzystać z żadnych materiałów pomocniczych. Niedozwolona jest komunikacja z innymi.
  - W czasie rozwiązywania zadań programistycznych **można** korzystać z tutoriala, własnych materiałów, rozwiązań zadań przykładowych, dokumentacji systemowej oraz Internetu jednak tak, by oceniana praca była dziełem samodzielnym - w szczególności niedozwolone jest używanie rozwiązań oparty o AI, jak chat GPT, Github Copilot i podobnych.

# Zasady zaliczenia przedmiotu i wystawiania oceny końcowej

  - Do zaliczenia przedmiotu koniecznie jest uzyskanie w sumie przynajmniej 100 punktów (na 200 możliwych), z zastrzeżeniem dopuszczalnej liczby nieobecności na laboratorium. Wymagane jest uzyskanie co najmniej 50 punktów z części wykładowej i 50 punktów z części laboratoryjnej.
  - Ocena z poprawy zastępuje ocenę z poprawianych zajęć.
  - **Ocena końcowa z przedmiotu** wynika z sumy punktów uzyskanych przez studenta z laboratorium i wykładu:
    - Ocena 5.0 - wynik w przedziale [180, 200]
    - Ocena 4.5 - wynik w przedziale [160, 180)
    - Ocena 4.0 - wynik w przedziale [140, 120)
    - Ocena 3.5 - wynik w przedziale [120, 140)
    - Ocena 3.0 - wynik w przedziale [100, 120)
    - Ocena 2.0 - wynik w przedziale [0, 100)


# Termin i tryb ogłaszania ocen

  - Oceny cząstkowe
    - Liczba punktów uzyskana za testy wykładowe jest podawana najpóźniej 2 tygodnie po danym teście w systemie USOS.
    - Liczba punktów uzyskana za kolejne etapy zadania laboratoryjnego jest podawana w momencie sprawdzania przez prowadzącego i zapisywana na kartce z treścią zadania
    - Liczba punktów uzyskana za wejściówkę jest wyświetlana w moodle po zakończeniu testu
    - Punkty z obu części laboratorium są umieszczane w usosie najpóźniej 24 godziny po zajęciach. 
    - Liczba punktów wpisana w usosie determinuje ocenę końcową
  - Ocena końcowa
    - Wystawiana w USOSie w ciągu 24 godzin od opublikowania wyników ostatniego laboratorium L4 w USOSie lub od opublikowania wyników drugiego testu wykładowego (zależnie, który nastąpi później).
    - Korygowana w trakcie sesji egzaminacyjnej w ciągu 24 godzin od opublikowania wyników poprawek w USOSie


# Zasady powtarzania zajęć z powodu niezaliczenia przedmiotu

Osoba, która nie zaliczyła przedmiotu, musi powtórzyć całość przedmiotu i napisać przystąpić do wszystkich zaliczeń. Nie ma możliwości przepisywania punktów cząstkowych z poprzednich semestrów. 

# Inne 
