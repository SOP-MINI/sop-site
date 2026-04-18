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

### Sockets and ports

L4 introduces addresses of application endpoints co-existing within a single host.
Those endpoints are known as **sockets** and are identified by numbers: **ports**.

![sockets.svg](/ops2/wyk/sockets/sockets.svg)

Pair `[ip]:[port]` like `10.0.0.2:1234` is a **globally unique address of an application endpoint**.

---

### L4 Protocols

There are two distinct, universally adopted protocols providing L4 functionality:

1. **UDP (User Datagram Protocol):**
    * Connectionless, fast, unreliable
    * Transmits discrete _messages_ known as **Datagrams**
2. **TCP (Transmission Control Protocol):**
    * Connection-oriented, reliable
    * Transmits a bidirectional, continuous _stream_ of bytes

---

### UDP Header

UDP datagram begins with an extremely simple, 8-byte header:

![udp_header.svg](/ops2/wyk/sockets/udp_header.svg)

All four fields are two bytes long. The first two fields are the **source port** and the **destination port**
identifying the host application endpoint.

Note those numbers are in network byte order (MSB first), that means: `0x00,0x01 = 1` and `0x01,0x00 = 256`.

Max datagram length is `2^16` bytes = `64KiB`.

---

### UDP Characteristics

* **Connectionless:** No handshake or teardown. The kernel implementation is mostly stateless.
    * Sender explicitly specifies the destination for *every single* message.
    * A single UDP socket can read incoming datagrams from *multiple different senders* multiplexed.
* **Multicast & Broadcast:** Inherently supports sending a single packet to many hosts (impossible in TCP).
* **Zero Reliability Guarantees:** The network or OS can (and will) cause:
    * ❌ **Packet Loss:** Datagrams can silently disappear.
    * ❌ **Duplication:** Receiving the exact same datagram twice.
    * ❌ **Reordering:** Datagram #2 might arrive before Datagram #1.

How to use: create a `socket()`, optionally `bind()` an address, and then straight to `sendto()` and `recvfrom()`.

---

### Creating sockets

To access **any** network, POSIX application must create a socket by calling `socket()` syscall.
It returns **fd** associated with newly created socket object.

```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
```

* `domain` specifies addressing type: `AF_INET` for IPv4 (others: `AF_INET6`, `AF_BLUETOOTH`, `AF_CAN`, `AF_PACKET`)
* `type`: specifies communication type: `SOCK_DGRAM` for datagrams (others: `SOCK_STREAM`, `SOCK_SEQPACKET`, `SOCK_RAW`)
* `protocol`: Usually `0` which tells the OS to choose the default protocol for the given domain/type pair:
  `AF_INET + SOCK_DGRAM = UDP`

If created successfully, the socket exists in the kernel, but it has no assigned address, no port, and is not capable of
receiving nor sending any traffic.
It's just an empty object with the protocol stack chosen.

---

### Claiming addresses

The `bind()` syscall assigns an address to a created socket.

```c
struct sockaddr_in my_addr;
// ... fill address structure ...
int bind(int sockfd, const struct sockaddr *addr, 
                     socklen_t addrlen);
```

Servers almost always calls `bind()`. These are expected to receive communication on a well-known port (e.g., port 53
for DNS).

Clients usually skip explicit `bind()`. The OS will automatically assign a random, unused (_ephemeral_) port the first
time the client sends something.

Socket address includes IP part. It may be the address of one of the available interfaces or a wildcard like `0.0.0.0` (
`INADDR_ANY`)
allowing for receiving traffic from all attached subnets.

---

### Address structures

Different addressing families use different addressing structures.
There's a single common _base class_ for addresses expected by `bind()`:

```c
struct sockaddr {
  sa_family_t sa_family;  /* Address family */
  char        sa_data[];  /* Socket address */
};
```

The first field determines what this address instance really is.
Example for `AF_INET` where `sa_data` has 6 bytes:

```c
struct sockaddr_in {
   sa_family_t     sin_family;     /* AF_INET */
   in_port_t       sin_port;       /* Port number */
   struct in_addr  sin_addr;       /* IPv4 address */
};
```

---

```c
struct sockaddr_in6 {
   sa_family_t     sin6_family;    // AF_INET6
   in_port_t       sin6_port;      // Port number
   uint32_t        sin6_flowinfo;   // IPv6 flow info 
   struct in6_addr sin6_addr;      // IPv6 address
   uint32_t        sin6_scope_id;  // Set of interface
};
```

```c
struct sockaddr_un {
   sa_family_t     sun_family;  // AF_UNIX
   char            sun_path[];  // Socket pathname
};
```

```c
struct sockaddr_rc {
    sa_family_t rc_family;   // AF_BLUETOOTH
    bdaddr_t    rc_bdaddr;   // 6-byte Bluetooth MAC
    uint8_t     rc_channel;  // RFCOMM Channel (1 to 30)
};
```

---

### Network Byte Order

Two machines talking to each other over the network may have totally different natural _endiannes_: order of storing
multibyte integers in memory.

![nbo.svg](/ops2/wyk/sockets/nbo.svg)

Conventionally used network format: **Network Byte Order** (NBO) = Big Endian.

Applications must perform conversions to/from network byte order manually. Kernel cannot possibly know how to interpret
bytes in L5+ application layers. Address structures also contain such mutlibyte integers. Sockets API expects them to be
in the Network Byte Order.

---

### Addressing helpers

* `inet_pton()` - converts text to binary address
* `inet_ntop()` - converts binary address to text

```c
const char* ip = "192.168.0.1";
struct in_addr addr; // uint32_t (NBO)
inet_pton(AF_INET, ip, &addr);
```

* `getsockname()` - returns address assigned to a socket
* `getpeername()` - returns address of remote, connected peer (TCP)

```c
struct sockaddr_in addr;
socklen_t len = sizeof(addr);
// Assumes AF_INET address is bound
getsockname(sockfd, (struct sockaddr *)&addr, &len);
```

---

### Sending and receiving

```c
ssize_t 
sendto(int sockfd, const void* buf, size_t len, int flags,
       const struct sockaddr* dstaddr, socklen_t addrlen);
```

* Sends a single datagram
* You must provide the destination address for every single datagram sent
* Returns number of bytes sent, or -1 on error (check `errno`)

```c
ssize_t
recvfrom(int sockfd, void* buf, size_t len, int flags,
         struct sockaddr* srcaddr, socklen_t* addrlen);
```

* Reads a single incoming datagram
* `addrlen` is an in/out parameter: you provide capacity, kernel returns length
* Returns number of bytes received into the `buf`

---

### Transmission Control Protocol

**TCP** is another, vastly different, more complex L4 protocol in use.

It attempts to provide reliable, ordered, bidirectional transmission of data between pairs of **connected** hosts.

![tcp_streams.svg](/ops2/wyk/sockets/tcp_streams.svg)

Connected pair is uniquely identified by a 4-tuple: (`IP`, `port`, `IP`, `port`).

Hosts are transmitting **byte streams**, not messages! TCP is a **stream-oriented protocol**.
Data written in a single `write()` call may be read in multiple reads.

---

### TCP Header

TCP is built on top of the unreliable IP layer (L3).
TCP messages encapsulated within IP packets are known as **segments**.
Segments contain a header and an optional application payload.

![tcp_header.svg](/ops2/wyk/sockets/tcp_header.svg)

---

### Sequence Numbers

![tcp_seq.svg](/ops2/wyk/sockets/tcp_seq.svg)

TCP byte stream must be chopped into segments handled by the IP layer.
To diagnose lost segments, reorderings and duplicates, the receiver must easily determine the
position of each segment within the byte stream.

Each segment carries a 4-byte **sequence number** identifying offset within the byte stream.
Sender increments and assigns the sequence number.

---

### Acknowledgement Numbers

In case of a lost segment, it needs to be retransmitted.
The receiver acknowledges the reception by sending an **acknowledgement number** back in its own segments.

![tcp_ack.svg](/ops2/wyk/sockets/tcp_ack.svg)

This tells the sender which segments have been successfully received and which ones still need to be
transmitted.

---

### Tx/Rx Buffers

Received segments may not be immediately delivered to the application 
in case preceding segments were lost or delivered out of order. 

Segments being sent may not be discarded immediately.
The sender must wait until the data is acknowledged.

Each connected socket maintains two kernel buffers: **receive (Rx) buffer** of incoming segments and **send (Tx) buffer** of outgoing segments.

![tcp_buffers.svg](/ops2/wyk/sockets/tcp_buffers.svg)

---

### Segment Flags

Each segment may contain a number of flags telling how to interpret the segment:

* `SYN` - start of new stream
* `FIN` - end of stream
* `RST` - reset connection
* `PSH` - regular data segment (seq no. tells which one)
* `ACK` - acknowledgement of previously received data
* `URG` - urgent data set (obsolete)

Combinations are possible, i.e. `PSH+ACK` segment does both:
transmits some new payload and acknowledges previously received bytes.

---

### Connection establishment

Both hosts must initially agree on their sequence numbers to establish a connection.
This is called a 3-way **handshake**.

![tcp_handshake.svg](/ops2/wyk/sockets/tcp_handshake.svg)

Each side sends a `SYN` segment to the other with chosen **initial sequence number**.
The receiver responds with a `ACK` confirming receipt of the `SYN` segment which counts 
as a single ghost byte within the byte stream.

Sequence counters for new connections have unpredictable initial values:

`ISN = Time + Hash(SrcIP, DstIP, SrcPort, DstPort, Secret)`

---

### Connection Backlog

TCP server constructs a *listening socket* which maintains the **connection backlog**. After `listen()` the OS starts to respond to the incoming handshakes.


![tcp_server.svg](/ops2/wyk/sockets/tcp_server.svg)

---

### Client Sockets

Established connections are queued and can be then obtained with `accept()` which returns a socket descriptor dedicated to a single client.

TCP server has at least one listening socket and `n` connected client sockets. All sockets share the same local address.
Client sockets are uniquely identified by the kernel stack using a 4-tuple, `localAddr,localPort,peerAddr,peerPort`
present in each received segment:

* `10.0.1.1,1234,peer:NULL` - listening socket
* `10.0.1.1,1234,10.0.2.100,5678` - client socket A
* `10.0.1.1,1234,10.0.2.200,1234` - client socket B

Each client socket maintains an independent connection state, Tx/Rx buffers and sequence and acknowledgement numbers.

---

### Establishing a connection 

Actively connecting side is called a **client**. It creates a socket using `SOCK_STREAM` type and calls `connect()` to establish a connection.

```c
int connect(int socket, const struct sockaddr *address,
            socklen_t address_len);
```

This usually blocks until server responds with `SYN+ACK` segment. 
The client OS automatically replies with `ACK` segment and returns the connected socket.

If `connect()` returns prematurely (e.g., due to signal delivery) the connection establishment process is still ongoing
in the background!

---

### TCP State Diagram (connection)

![tcp_state_conn.svg](/ops2/wyk/sockets/tcp_state_conn.svg)

Note it's the kernel memory where the connection state is maintained.
The network itself knows nothing about _connection_. It just passes segments around. 

---

### Closing a connection

Connection is closed independently both ways by sending a `FIN` segment to the other side which counts as last stream
byte needing acknowledgment.

![tcp_state_fin.svg](/ops2/wyk/sockets/tcp_state_fin.svg)

---

### Closing syscalls

Calling `close()` syscall tells the kernel two things:
* I won't send anything else
* I do not care about what remote side is still sending

`close()` **does not** block until Tx buffer is flushed out! It instructs the kernel to continue
flushing in the background. 

If `close()` is called with something not consumed from the Rx buffer, kernel treats it as an abnormal termination.
It will send `RST` segment to the other side.

You can also use `shutdown(fd, SHUT_WR)` to close output stream while maintaining the possibility to receive more data.

To close gracefully: use `shutdown()` -> read until EOF -> `close()`.

---

### Timed wait state

The side which closes the connection actively (sends the first `FIN`) enters the `TIME_WAIT` state after the 4-way teardown is complete.

The socket lingers in this state for `2 * MSL` (Maximum Segment Lifetime), which is strictly hardcoded to 60 seconds in Linux.
Why?

* If final `ACK` is lost, the remote side may retransmit the `FIN` segment.
* This could corrupt/break a new connection reusing the same port.

This frequently causes `bind()` to fail after program restart.

Client applications typically do not experience this problem due to ephemeral port usage.
**Construct protocols so that the client closes first!**

For quick iteration during developemnt `SO_REUSEADDR` before binding the listening socket!

---

### Data Transfer

Unlike UDP, TCP models bidirectional byte stream. Syscalls read and write **local kernel buffers**:

```c
ssize_t send(int fd, const void *buf, size_t len, int flgs);
ssize_t recv(int fd, void *buf, size_t len, int flgs);
```
Both can return `> 0` but `< len` if buffers are full/empty. Always use loops!
* **Graceful Disconnect (Received `FIN`)**
  * `recv()` returns 0 after stream was fully consumed.
  * `send()` will succeed! `FIN` does not mean the other side is done!
* **Abortive Disconnect (Received `RST`)**:
  * both `recv()` and `send()` may return `ECONNRESET`.
  * if `send()` is called **after** `RST`, it triggers a `SIGPIPE` signal!
