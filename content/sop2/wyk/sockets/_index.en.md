---
title: "Sockets"
weight: 60
bookCollapseSection: true
---

# Sockets

## Scope

* Role of the transport layer`
* Concept of a socket and port
* Comparison of transport protocols: UDP vs TCP
* Structure of a UDP datagram header
* Features of UDP
* Creating sockets – `socket()` function
* Binding a socket to an address – `bind()` function and ephemeral ports
* Address structure families (`sockaddr`, `sockaddr_in`, `sockaddr_in6`)
* Network Byte Order vs Host Byte Order
* Conversion and helper functions (`inet_pton`, `inet_ntop`, `getsockname`, `getpeername`)
* Connectionless communication – `sendto()` and `recvfrom()` functions
* Byte stream model in the TCP protocol
* TCP connection identification (4-tuple)
* TCP segment structure and flag usage (`SYN`, `FIN`, `RST`, `PSH`, `ACK`)
* Sequence numbers (SEQ) and acknowledgment mechanism (ACK)
* Data management in the system kernel – transmit (Tx) and receive (Rx) buffers
* Establishing a TCP connection (3-way handshake, Initial Sequence Number)
* Listening sockets and connection queue (backlog, `listen()` function)
* Accepting connections on the server – `accept()` function
* Initiating connections by the client – `connect()` function
* TCP connection state diagram
* Reading and writing from a TCP stream – `send()` and `recv()` functions
* Closing: `close()` vs `shutdown()`
* `TIME_WAIT` state

## Reference

1. [Lecture Slides](slides)
2. [Code samples](code)
3. Old Slides: [Inet\_2en.pdf]({{< resource "Inet_2en_9.pdf" >}})
4. Chapter 16 in *the GNU C library* manual: [Sockets](http://www.gnu.org/software/libc/manual/html_node/#toc-Sockets-1)
5. Supplementary reading: W.R. Stevens, Unix Network Progamming, Vol. 1, 2nd Ed.

