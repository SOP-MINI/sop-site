---
title: "Task on POSIX message queues"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Chinese Whispers  

> The Chinese whispers game, also known as "Telephone," involves players forming a line and passing a message from one person to the next, usually resulting in a humorous distortion of the original message by the time it reaches the last player.

Write a program that simulates the game of Chinese whispers using POSIX message queues.  

The program launches with three parameters `P`, `T₁`, `T₂`, where `0 ≤ P ≤ 100` and `100 ≤ T₁ < T₂ ≤ 6000`.  
Upon launching, the names of children participating in the game are fed (on new lines) into the standard input.  

Entering the command `start "Very long sentence to pass"` starts the game. The coordinator (main process) passes the given sentence in its unchanged form to the first child (in the order they joined the game). Each word is sent in a separate message.  

After the game round is finished, the coordinator displays the result, and all processes are terminated.  

A message queue can store only 2 words (messages) at once.  

It can be assumed that the maximum length of a line of standard input is 255 characters.  

**Note:** The program should handle multiple instances without queue name collisions and program operation conflicts!  

## Stages:  

1. The main process (game coordinator) correctly creates new processes based on the given names of the children and waits for their termination.  
   - There is no predetermined limit (apart from system resources) on the number of children participating in the game.  
   - At a minimum, to start the game, there must be at least one child. Each child knows only their name and the child's PID from which they listen. In the case of the first child, it is the PID of the coordinator.  
   - A child displays the message `[{PID}] {name} has joined the game!`, waits a random time between `T₁` and `T₂`, and exits with the message `[{PID}] {name} has left the game!`.  

2. A child, upon joining the game, creates a new queue named `"sop_cwg_{PID}"`.  
   - The child knows only the PID of the preceding child - this tells them which queue to receive messages from.  
   - Typing `start {message_to_pass}` starts sending word-by-word the given message to the first child.  
   - Only the first child receives the message from the coordinator for this stage.  
   - Upon receiving a word, it prints it to standard output `[{PID}] {name} got the message: '{word}'`.  

3. Words are transmitted between the children and then returned back to the coordinator.  
   - For each letter of the word, there is a `P%` chance that the letter will be changed to some random Latin letter.  
   - The coordinator prints the received message on standard output. All used resources are correctly released.  

4. After receiving the `SIGINT` signal, all children quit the game.  
   - The coordinator displays the part of the message he has already received and exits.  
   - You can use a timeout for messages to avoid a deadlock in this stage.  

---

### Sample Input (`example_input.txt`):  

```plaintext
Anna
Frank
Simon
Katie
Tom
Matt
start "Operating Systems 1 and 2 are my favorite courses during my studies; I have learned a lot of fun stuff here!"
```

### Expected Output:

```plaintext
./sop-cwg-stage4 2 250 500 < example_input.txt
Chinese Whispers Game
Provide child name or type start to begin the game!
[29406] Anna has joined the game!
[29407] Frank has joined the game!
[29408] Simon has joined the game!
[29409] Katie has joined the game!
[29410] Tom has joined the game!
[29411] Matt has joined the game!
Game has started with 6 childrens!
Passing "Operating Systems 1 and 2 are my favorite courses during my studies; I have learned a lot of fun stuff here!" to the first child!
Passing word "Operating"
Passing word "Systems"
Passing word "1"
[29406] Anna got message: Operating
Passing word "and"
[29406] Anna got message: Systems
Passing word "2"
[29407] Frank got message: Operating
Passing word "are"
[29406] Anna got message: 1
[29407] Frank got message: Systems
[29408] Simon got message: Operating
Passing word "my"
[29407] Frank got message: 1
[29406] Anna got message: and
[29408] Simon got message: Systems
[29409] Katie got message: Operating
[29407] Frank got message: and
[29406] Anna got message: 2
Passing word "favorite"
[29410] Tom got message: Operating
[29408] Simon got message: 1
[29409] Katie got message: Systems
[29411] Matt got message: Operating
...
[29408] Simon has left the game!
[29411] Matt got message: fun
[29409] Katie got message: heraa
[29410] Tom got message: stucf
[29411] Matt got message: stucf
[29410] Tom got message: hewaa
[29411] Matt got message: hewaa
[29409] Katie has left the game!
[29410] Tom has left the game!
[29411] Matt has left the game!
Final message    : Operating Systems 1 and 2 lre my favorite courbes duhing my studies; I zave learned a lot ou fun stuuf hewaa
Original message : Operating Systems 1 and 2 are my favorite courses during my studies; I have learned a lot of fun stuff here!
Game has ended!
```