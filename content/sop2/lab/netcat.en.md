---
title: "Netcat"
---

# Introduction

`netcat` is a program that can be used to perform a variety of network connections (TCP, UDP and others) and send data read from `stdin` to the connection as well as printing the data from the network connection to `stdout`. You can use netcat to communicate with programs that use plaintext based network protocol. During OPS2 classes `netcat` is a useful tool during lab that cover networking: ([L8]({{< ref "l8">}})). This way you can test your programs.

# netcat versions

The `netcat` program comes in several different version depending on the operating system installed. All popular versions can communicate using TCP and UCP sockets, although they differ in case of some advanced operations. In major Linux distributions you usually find one of three of the following versions (it's worth to check which version is available in your system):

 - [traditional](https://nc110.sourceforge.io/),
 - [GNU](http://netcat.sourceforge.net/),
 - OpenBSD -- an implementation taken from OpenBSD system, probably the most versatile of all of them. It can use unix domain socket (traditional and GNU versions cannot do that).

The remainder of this page describes `netcat` OpenBSD version as this is the version installed in the lab computers.

# netcat usage

The full description of `netcat` is available in the manual. Below we present the most common usage examples.

In majority of systems `netcat` is available under the command `nc`, sometimes also `netcat` or `ncat`.

## TCP

By default if there are no switches specifying the protocol to be used `netcat` operates in TCP mode:

 - `nc <host> <port>` --- connects as a TCP client to the server given as the argument
 - `nc -l -s <address> -p <port>` --- starts a server socket listening on the given port, `-s` is optional
 
## UDP

To use `netcat` in UCP mode option `-u` is used:

 - `nc -u <host> <port>` --- creates a UDP socket bound to a random port and sends all data from `stdin` to the address given,
 - `nc -u -l -s <address> -p <port>` --- creates udp socket bound to the specified address and port and waits for data from the network. `stdin` data is send to the address where the network datagram come from.

## UNIX

- `nc -U <path>` --- connects to a given unix domain socket (stream)
- `nc -l -U <path>` --- waits for connections on the given unix domain socket (stream)
- `nc -U -u <path>` --- sends data to a given unix domain socket (datagram)
- `nc -l -U -u <path>` --- waits for data on the given unix domain socket (datagram)
