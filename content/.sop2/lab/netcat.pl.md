---
title: "Netcat"
date: 2022-06-07T10:46:18+02:00
---

# Wstęp

`netcat` jest programem, który potrafi nawiązywać różne rodzaje połączeń sieciowych (TCP, UDP i nie tylko) i przesyłać dane ze swojego standardowego wejścia do połączenia sieciowego oraz wypisywać na standardowe wyjście dane otrzymane z sieci. Ten program można wykorzystać do wysyłania wiadomości do programów, które komunikują się przez sieć z użyciem zwykłego tekstu. Na zajęciach z SOP2 netcat przyda może się przydać na laboratoriach, w trakcie których wykorzystywane są połączenia sieciowe ([L3]({{< ref "l3">}}), [L4]({{< ref "l4">}}) do testowania programów.

# Warianty programu netcat

Program `netcat` można spotkać w różnych wariantach, w zależności od używanego systemu operacyjnego. Wszystkie potrafią nawiązywać podstawowe połączenia TCP i UDP, jednak mogą się różnić w przypadku bardziej zaawansowanych funkcjonalności. W dystrybucjach Linuksa można spotkać trzy popularne implementacje (implementacji w przyrodzie jest dużo więcej), warto sprawdzić która implementacja jest zainstalowana w systemie:

 - [tradycyjna](https://nc110.sourceforge.io/),
 - [GNU](http://netcat.sourceforge.net/),
 - OpenBSD -- implementacja z systemu openbsd, posiada więcej funkcjonalności, między innymi potrafi nawiązywać połączenia w domenie ~unix~, czego nie potrafią pozostałe.

Dalsza część tego opisu będzie zakładać wersję ~netcat~ z OpenBSD, bo taka jest zainstalowana w laboratorium. 

# Użycie programu netcat

`netcat` jest szczegółowo opisany w podręczniku, poniżej podane są podstawowe wywołania przydatne na zajęciach.

W większości systemów `netcat` dostępny jest jako polecenie `nc`, rzadziej jako `netcat` lub `ncat`.

## TCP

Jeśli nie zostały podane żadne opcje specyfikujące protokół, który ma być użyty, to netcat domyślnie wykorzysta TCP:

 - `nc <host> <port>` --- nawiązuje połączenie kliencie z podanym serwerem TCP
 - `nc -l -s <adres> -p <port>` --- rozpoczyna nasłuchiwanie na podanym adresie i porcie, opcja `-s` jest opcjonalna
 
## UDP

Wszystkie wywołania wykorzystujące protokół UDP wymagają flagi `-u`:

 - `nc -u <host> <port>` --- tworzy socket udp na losowym porcie i wysyła wszystkie dane z stdin na podany adres,
 - `nc -u -l -s <adres> -p <port>` --- tworzy socket udp na podanym adresie i porcie, czeka na dane. Po otrzymaniu pakietu stdin jest wysyłane na adres, z którego przyszedł pakiet.

## UNIX

- `nc -U <ścieżka>` --- łączy się z podanym socketem w domenie unix (strumieniowo)
- `nc -l -U <ścieżka>` --- nasłuchuje na podanym sockecie w domenie unix (strumieniowo)
- `nc -U -u <ścieżka>` --- wysyła dane na podany socket w domenie unix (datagramy)
- `nc -l -U -u <ścieżka>` --- nasłuchuje na podanym sockecie w domenie unix (datagramy)
