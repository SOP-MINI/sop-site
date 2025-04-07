---
title: "Task on network"
bookHidden: true
---

# Οἱ ἄγγελοι (The messengers)

The year is 480 B.C.  
The Persian king Xerxes (Old Persian: 𐎧𐏁𐎹𐎠𐎾𐏁𐎠, Khshayārsha) has just invaded the Greek city-states.  
This unfortunate event has caused many messengers traveling throughout Greece to begin acting as scouts.  
They visit a variety of cities. Some settlements are controlled by the Greeks, while others have been taken by the Persians.  

To gain an informational advantage, the citizens of Sparta (Doric Greek: Σπάρτα) decide to establish a library connected to the latest telecommunications network for messengers.  

Each messenger receives a TCP client application that can connect to the library server.  
The library contains a list of city-states and information about whether they are controlled by the Greeks or the Persians.  
Each messenger maintains a similar list locally, updating it when new information is received from the central library.  
There are 20 city-states. Each one is identified by a number in the range [1, 20].  
Your task is to **implement a library server** compatible with the provided client.  

The client program is delivered as a compiled executable. You can run it using:

```shell
./client <SERVER_ADDRESS> <PORT_NUMBER>
```

A compiled executable of a sample server implementation is also provided.

## Stages

1. Implement handling of a single client connecting to a TCP server.  
   The server accepts the port number as a command-line argument.  
   Example server execution:

   ```shell
   ./server 8888
   ```

   After starting, the server listens for incoming client connections.  
   Once connected to the first client, it waits to receive 4 bytes of data, prints them to stdout, closes the connection, and then exits.

2. Implement handling of multiple clients and receiving data.
   - When a new client connects, they are added to the current list of clients.
   - The maximum number of clients is 4.
   - If the maximum number of clients is reached, new connections should be rejected.
   - Whenever the server receives a 4-byte message from any client, it prints it to the terminal.
   Use the epoll function to implement this step  
   (alternatively, you may use pselect or ppoll).

   > ⚠️ **Note: In this step, you do not need to correctly handle client disconnections.**

3. Add to the server an array that stores information about which side each city belongs to.
   - Initially, all cities belong to the Greeks.
   - Each time the server receives a message in the format:
     ```
     pXX\n
     ```
     or
     ```
     gXX\n
     ```
     where `XX` is a two-digit number representing a city identifier, it indicates that city `XX` now belongs to the Persians or the Greeks, respectively.
   - If a city’s ownership hasn’t changed, take no action.
   - If the city’s owner has changed:
     - update the city ownership array,
     - broadcast the received message to all other connected clients.

   Assumptions:
   - You still do not need to handle client disconnections at this stage.
   - All incoming messages can be assumed to be in the correct format.

4. Upon receiving a `SIGINT` signal, the server should:
   - close all connections,
   - print out which side each city belongs to,
   - free resources and terminate.

   Add proper handling for client disconnections in the following cases:
   - Reading from a client descriptor returns a message of length 0.
   - Writing to a client descriptor raises an `EPIPE` error.
   - The message received from the client is not in the correct format or the city number is outside the [1, 20] range.
   - The server receives a Control-C (C-c).

   > ⚠️ **Note: Disconnected clients free up space for new incoming clients.**

## Client and server executables

- [sop2l8e3-4.zip](/files/sop2l8e3-4.zip)
