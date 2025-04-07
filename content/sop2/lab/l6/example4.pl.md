---
title: "Zadanie testowe z tematu kolejek POSIX"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Czat

Napisz serwer i klienta prostego czatu z użyciem kolejek POSIX.  
Gdy serwer się uruchamia, otwiera kolejkę o nazwie `"chat_{server_name}"`, gdzie `{server_name}` jest jedynym argumentem serwera. Ta kolejka jest używana do przychodzącej komunikacji z serwerem.  

Klient uruchamia się z dwoma argumentami: `{server_name}` i `{client_name}`. Łączy się z serwerem za pomocą kolejki serwera, otwiera własną kolejkę `"chat_{client_name}"` do odczytywania wiadomości od serwera i wysyła wszystkie wpisywane na standardowe wejście wiadomości do serwera.  

Istnieją trzy rodzaje wiadomości (użyj priorytetu wiadomości do odróżnienia typu):  
- **0** - połączenie  
- **1** - rozłączenie  
- **2** - zwykłe wiadomości  

Wiadomość o połączeniu wysłana przez klienta do serwera zawiera jego nazwę. Wiadomość o rozłączeniu to pusta wiadomość jednobajtowa sygnalizująca zakończenie działania klienta lub serwera.  
Tekstowe wiadomości przesyłane przez klientów do serwera zawierają jedynie surowe wiadomości jako C-string bez żadnych metadanych. Serwer rozsyła je z powrotem do wszystkich klientów sformatowane jako: `"[{sender_name}] {message}"`.  

Serwer może również wysyłać własne wiadomości wpisywane z wejścia standardowego; wtedy nazwa nadawcy to `SERVER`.  

Użyj `mq_notify` do odbierania wiadomości i odczytania `PID` wysyłającego procesu.  

---

## Etapy:

1. Serwer otwiera kolejkę `"chat_{server_name}"`, w pętli czyta z niej i wyświetla odebrane wiadomości na standardowym wyjściu jako: `"[{msg_prio}] {msg_content}"`.  
   Klient otwiera `"chat_{server_name}"` i wysyła tam swoje imię z odpowiednim priorytetem wiadomości.  

2. Klient tworzy swoją kolejkę `"chat_{client_name}"` po uruchomieniu.  
   Serwer otwiera kolejkę klienta po otrzymaniu nazwy klienta (wysłanej jako C-string z priorytetem 0) i przechowuje informacje o połączonych klientach (maksymalnie 8 klientów naraz).  
   Po połączeniu klienta, serwer wyświetla: `"Client `{client_name}` has connected!"` na standardowym wyjściu.  

3. Klient wysyła linie wprowadzone ze standardowego wejścia do serwera.  
   Serwer odbiera dane od klientów za pomocą `mq_notify`. Wyświetla je na standardowym wyjściu sformatowane jako: `"[{sender_name}] {message}"` i wysyła je z powrotem do wszystkich klientów w tej samej postaci.  
   Serwer pozwala również na wpisywanie własnych wiadomości za pomocą standardowego wejścia – są one wysyłane z `SERVER` jako nadawcą.  
   Klienci odbierają wiadomości od serwera i wyświetlają je jako: `"[{client_name}] {message}"`.  

4. Serwer obsługuje zamykanie za pomocą `Ctrl-C`, wysyłając informację o swoim zamknięciu poprzez "pustą" wiadomość z odpowiednim priorytetem.  
   W przypadku wiadomości o zamknięciu serwera, klienci wyświetlają: `"Server closed the connection"` i kończą działanie.  
   W przypadku wiadomości o zamknięciu klienta, serwer wyświetla: `"Client {client_name} disconnected!"`.  
   Wszystkie zasoby są prawidłowo zwalniane – kolejki są zamykane i usuwane przez ich twórców.  
