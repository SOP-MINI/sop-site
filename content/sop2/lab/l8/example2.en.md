---
title: "Task on network"
bookHidden: true
---

## Imperial Election

It is the year 1519.  
The Holy Roman Emperor Maximilian I has passed away,  
and the time has come for the election of a new emperor.  

There are 3 candidates:

1. Francis I, King of France  
2. Charles V, Archduke of Austria and King of Spain  
3. Henry VIII, King of England

The election is decided by 7 electors from different Holy Roman Empire member states:

1. Mainz  
2. Trier  
3. Cologne  
4. Bohemia  
5. Palantinate  
6. Saxony  
7. Brandenburg

This year, to facilitate the election process,  
it has been decided to set up a TCP server handling and counting the votes.  
Your task is to write this server.  
Each elector will connect to it,
cast their vote,
possibly change their vote,
and disconnect.  
You can use `netcat` as a client program.

## Stages

1. Implement accepting a single connection to the server.  
   The server accepts one argument: the port number.  
   
   Example server execution:
   
   ```shell
   ./sop-hre 8888 
   ```
   
   After starting, the server waits for a single TCP connection.  
   After establishing a connection,  
   the server prints `Client connected`, closes the connection, and ends.
   
2. Implement connections from multiple clients.  
   
   After a client connects,  
   send them back a message: `Welcome, elector!`  
   Then, print each message the client sends to the server on `stdout`.  
   Use `epoll` to implement this stage (or, alternatively, `ppoll` or `pselect`).  
   
   Remember to correctly handle disconnecting clients.
3. Implement the voting process and keep a list of connected electors.  
   
   When a new client connects, wait for a single digit (in the range [1, 7]).  
   This digit is the number of the elector connecting to the server.  
   If a different character is received from the client,
   close their connection.  
   
   While the server is waiting for this identification message,  
   other clients should still be able to connect.  
   
   If another client identifying as elector E tries to connect to the server,  
   they should be disconnected.  
   
   The message sent back to the client should now be sent after identification and should say:  
   `Welcome, elector of X!`,  
   where `X` is the member state the elector is from.  
   
   After receiving the identification message,  
   the connected client should be able to cast votes in the election  
   by sending a number in the range [1, 3], signifying the preferred candidate.  
   Other characters should be ignored.  
   Any elector can change their mind and vote multiple times.  
   Successive votes override previous ones.  
   
   Correctly handle clients disconnecting from the server,  
   including electors disconnecting and reconnecting!
   
4. Implement an additional thread sending out UDP messages.  
   
   The program should now accept two arguments in total:  
   - the TCP server port  
   - the UDP client port
   
   Example server execution:
   
   ```shell
   ./sop-hre 8888 9999 
   ```
   
   All functionality from the previous stages should still work.  
   
   Upon starting the server,  
   create an additional thread with a UDP client that sends a message  
   with the current election results each second to the given port  
   on `localhost`.  
   
   Remember to protect the election results with a mutex,  
   or avoid a data race in some other way.
   
5. Handle the `SIGINT` signal.  
   
   After receiving it:
   
   - Close all active connections with the electors  
   - Free all resources, including the UDP thread  
   - Count and print the votes for each candidate.
