---
title: "Zadanie testowe z tematu Wątki muteksy i sygnały"
date: 2022-02-05T19:36:50+01:00
bookHidden: true
---

## Treść

Napisz program, który tworzy n wątków (n to parametr programu), każdy wątek losuje swój mnożnik M \[2-100\]. Wątek główny co 0,1 sekundy zwiększa licznik L o jeden (począwszy od 1). Wątki sprawdzają czy ich mnożnik dzieli licznik bez reszty, jeśli tak to wypisują na ekran krótką informację - „L jest podzielne przez M”. Należy zadbać o to aby dostęp do L był chroniony oraz o to aby wszystkie wątki zdążyły sprawdzić podzielność. Program kończy się po otrzymaniu SIGINT (C-c). W reakcji na sygnał wszystkie wątki poboczne (sprawdzające podzielność) mają się zakończyć a wątek główny ma je przyłączyć zanim sam się zakończy.

## Oceniane etapy (w ramach pokazania jak działa etap proszę uruchomić z parametrem 10)

1.  Tworzenie n wątków, każdy wątek wypisuje „\*” dla pokazania, że działa, wątek główny czeka na zakończenie wszystkich wątków
2.  Wątki poboczne losują swoje mnożniki i wypisują je na ekran
3.  Wątek główny wykonuje odliczanie, poboczne sprawdzają podzielność, całość bez synchronizacji/ochrony dostępu do licznika L, C-c kończy bez specjalnej obsługi
4.  Dodajemy ochronę L przed zmianą zanim wszystkie wątki poboczne nie sprawdzą podzielności. Sugestia: może się przydać licznik wątków, które już sprawdziły. Pokazujemy kod ochrony dostępu
5.  Dodajemy pełną obsługę C-c. Pokazujemy kod zamykania programu
