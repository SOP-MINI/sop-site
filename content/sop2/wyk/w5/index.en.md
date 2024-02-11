---
title: "TCP/IP networking"
date: 2022-02-03T19:14:17+01:00
weight: 50
---

# Lecture 5 - Introduction to TCP/IP networking

## Scope
  
### Basic content

  - OSI communication architecture. Functionality of layers.
  - Internetworks, Internet. 4-layer architecture of TCP/IP based networks.
  - Communication in TCP/IP network; virtual layer to layer communication. Encapsulation. Roles of router and bridge. Example protocol stack.
  - Types of data transmissions/routing schemes: unicast, multicast, broadcast, anycast, geocast
  - IPv4 addressing: classful, classless (CIDR). Private networks and NAT.
  - Port concept, IANA, assignment of ports: well-known, registered, ephemeral.
  - Basic TCP/IP protocols:
      - IP: addressing, fragmentation/reassembly of data packets (datagrams). IP datagram structure.
      - ICMP: carrier of (mostly error) messages.
      - UDP: basis of connection-less (datagram) Internet communication.
      - TCP: basis of connection-oriented (stream) reliable Internet communication.  
        Concepts: segement retransmission, acknowledgement, offered window, adaptation of transmission speed, Nagle algorithm, input/output queues (buffers) and their size, low-water marks, urgent/out-of-band (OOB) data.
  - IPv6

### Auxiliary content

  - TCP/IP state diagram.
  - Auxiliary protocols: ARP, RARP, BOOTP, DHCP

## Reference

1.  Textbook: chapter 16 (Distributed Operating Systems): sec 16.3 (Network Structure), sec. 16.4 (Network Topology), sec. 16.5 (Communication Structure), sec. 16.6 (Communication Protocols), sec. 16.9 (An Example: Networking)
2.  Slides: [Inet\_1en.pdf]({{< resource "Inet_1en_5.pdf" >}})
3.  Supplementary reading: W.R. Stevens, Unix Network Progamming, Vol. 1, 2nd Ed.  
    Chapters: 1 (Introduction), 2 (The Transport Layer: TCP and UDP).
