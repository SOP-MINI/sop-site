---
title: "Sockets interface"
date: 2022-02-03T19:18:47+01:00
weight: 60
---

# Lecture 6 - Sockets interface

## Scope
  

  - Sockets and UNIX file system: socket/file descriptors, read/write/close, error handling, socket API belongs to kernel, Unix domain (local) sockets - special files.
  - Models of simple (request-reply) client-server communication using connection-oriented (TCP) and datagram (UDP) communication.
  - Socket creation: `socket()`, protocol families, address familiesw, socket options
  - Socket addressing structures: generic `sockaddr`, IPv4 domain `sockaddr_in`, Unix domain `sockaddr_un`.
  - Network order of bytes; order conversion functions: `htons()`, `htonl()`, `ntohs()`, `ntohl()`
  - Address format conversion functions: `inet_addr()`, `inet_ntoa()`, `inet_aton()`, `inet_ntop()`, `inet_pton()`
  - Using host names: structure `hostent` and functions `gethostbyname()`, `gethostbyaddr()`; structure `addrinfo` and functions: `getaddrinfo()`, `freeaddrinfo()`, `gai_strerror()`, `getnameinfo()`
  - TCP socket addressing (naming): explicit (`bind()`), implicit (`connect()`)
  - Opening connection: passive side (`listen()`, `accept()`), active side (`connect()`)
  - Data transfer via TCP connection: `read()`, `recv()`, `readv()`,`recvmsg`, `write()`, `send()`, `writev()`, `sendmsg`; `SIGPIPE` signal.
  - Connection closing: orderly (`shutdown()`) and careless (`close()`). Lingering (*linger*).
  - UDP communication: addressing (sender, recepient), basic communication functions: `recvfrom()`, `sendto()`
  - Connected UDP sockets: why and how. Asynchronous communication errors of UDP communication.

### Note

During lecture tests knowledge and understanding of general concepts and socket interface properties are expected. Understanding the role of basic socket functions in connection-oriented (stream) or connection-less (datagram) communication can be tested, but not detailed knowledge of socket interface functions (e.g. argument list).  
  
## Reference

1.  Slides: [Inet\_2en.pdf]({{< resource "Inet_2en_9.pdf" >}})
2.  Chapter 16 in *the GNU C library* manual: [Sockets](http://www.gnu.org/software/libc/manual/html_node/#toc-Sockets-1)
3.  Supplementary reading: W.R. Stevens, Unix Network Progamming, Vol. 1, 2nd Ed.

