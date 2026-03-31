---
title: "Slides"
type: presentation
layout: single
---

# Sockets

Operating Systems 2 Lecture

<small>Warsaw University of Technology<br/>Faculty of Mathematics and Information Science</small>

---

### The need for L4

**The Problem:**
When the packet arrives at its destination - to the host having IP address `10.0.0.2`,
it is received and accepted by the OS. How does the OS know which application should receive these bytes?

![l4_need.svg](/ops2/wyk/sockets/l4_need.svg)

---

### The Transport Layer

Network has to provide a process-process communication,
not only host to host. The Transport Layer (L4) does just that, building on top of L3
provides: global host addressing and unreliable delivery of packets.

![l4_net.svg](/ops2/wyk/sockets/l4_net.svg)

---

### Ports

The Transport Layer introduces addresses of applications running within a single host
known as **ports**.

In practice, those are 16-bit numbers uniquely identifying sender/receiver.

Pair `[ip]:[port]` like `10.0.0.2:1234` is a **globally unique address of a process**,
as IP was globally unique host address.

---

### L4 Protocols

There are two distinct, universally adopted protocols providing L4 functionality:

1. **UDP (User Datagram Protocol):**
    * Connectionless, fast, unreliable
    * Transmits discrete _messages_ known as **Datagrams**
2. **TCP (Transmission Control Protocol):**
    * Connection-oriented, reliable
    * Transmits a bidirectional, continuous "stream" of bytes

---

