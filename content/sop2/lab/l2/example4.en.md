---
title: "Task on POSIX message queues"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Chat

Write a server and a client for a simple chat using POSIX queues.  
When the server starts, it opens a queue named `"chat_{server_name}"` where `{server_name}` is the only server argument. This queue is used for incoming communication from clients to the server.  

The client starts with two arguments: `{server_name}` and `{client_name}`. It connects to the server using the server queue, opens its own queue `"chat_{client_name}"` for reading messages from the server, and sends all typed messages from standard input to the server.  

There are three types of messages (use message priority to distinguish the type):  
- **0** - connection  
- **1** - disconnection  
- **2** - regular messages  

The connection message sent to the server from the client contains its name. The disconnection message is an empty 1-byte message signaling client or server termination. Regular messages from clients contain raw messages sent as C-strings without any metadata. The server sends back to all clients messages formatted as: `"[{sender_name}] {message}"`.  

The server can also send its own messages typed from the standard input; in that case, the sender name is `SERVER`.  

Use `mq_notify` for obtaining messages and information about the sender `PID`.  

---

## Stages:

1. The server opens queue `"chat_{server_name}"`, reads from it, and displays in a loop a read message on the standard output as: `"[{msg_prio}] {msg_content}"`.  
   The client opens `"chat_{server_name}"` and sends its name there with the proper message priority.  

2. The client creates its queue `"chat_{client_name}"` at the start.  
   The server opens a client's queue after receiving the client's name (sent as a raw C-string with priority 0) and stores information about connected clients (max 8 clients connected at once).  
   On a client connection, the server displays: `"Client `{client_name}` has connected!"` on the standard output.  

3. A client sends lines from the standard input to the server.  
   The server receives data from clients using `mq_notify`. It displays them on the standard output formatted as: `"[{sender_name}] {message}"` and broadcasts them to all clients in the same format.  
   The server also supports typing messages, which are sent with `SERVER` as the sender name.  
   Clients receive messages from the server and display them as: `"[{client_name}] {message}"`.  

4. The server supports closing on `Ctrl-C`. It sends information about its closing using an "empty" message with the proper priority.  
   Upon receiving the server closing message, clients display: `"Server closed the connection"` and terminate.  
   Upon receiving the client closing message, the server displays: `"Client {client_name} disconnected!"`.  
   All resources are correctly released – queues are closed and removed by their creators.  
