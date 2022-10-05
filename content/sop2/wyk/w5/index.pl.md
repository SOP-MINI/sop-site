---
title: "Wykład 5. Wprowadzenie do sieci TCP/IP"
date: 2022-02-03T19:14:33+01:00
---

## Zakres wykładu
  
### Treści podstawowe

  - Modele komunikacji sieciowej: 7-warstwowy OSI i 4-warstwowy DoD (TCP/IP). Rola warstw. Internet. Standardy IETF: RFC.
  - Terminy związane z komunikacją w sieci TCP/IP: stos protokołów, ruter, MTU, kapsułkowanie, fragmentacja/scalanie pakietów, unicast/multicast/broadcast/anycast
  - Adresowanie klasowe, a bezklasowe. Sieci prywatne i NAT.
  - Porty i zarządzanie nimi; porty ogólnie znane, rejestrowane, dynamiczne; rola IANA.
  - Podstawowe informacje o wybranych protokołach rodziny TCP/IP:
      - `IP` - adresowanie, fragmentacja/scalanie, TTL
      - `ICMP` - rola protokołu, wybrane funkcje.
      - `UDP` - adresowanie, struktura datagramu UDP
      - `TCP` - podstawowe własności (realizacja niezawodnej dupleksowej komunikacji strumieniowej/połączeniowej z kontrolą przepływu).  
        Terminy: potwierdzanie doręczenia segmentów, retransmisja, okno oferowane, adaptacja szybkości, algorytm Nagle'a, dane pilne (OOB), bufor odbiorczy i wysyłkowy, rozmiary i wskaźniki "low-water mark".
  - IPv6

### Treści dodatkowe

  - Formaty pakietów danych protokołów: `IP`, `TCP`, `ICMP`
  - Graf stanów gniazda. Zmiany stanu gniazda przy otwieraniu i zamykaniu połączenia (TCP).

## Materiały

1.  Podręcznik: z rozdziału 15 (Struktury systemów rozproszonych) podrozdziały: 15.2 (Topologia), 15.3 (Typy sieci), 15.4 (Komunikacja), 15.5 (Protokoły komunikacyjne), 15.8 (Przykład - praca w sieci).
2.  Slajdy: [Inet\_1.pdf]({{< resource Inet_1_3.pdf >}})
3.  Literatura uzupełniająca: W.R. Stevens, Unix programowanie usług sieciowych, t. 1, wyd. 2: API: gniazda i XTI  
    Rozdziały: 1 (Wprowadzenie), 2 (Warstwa transportowa: protokoły TCP i UDP)
