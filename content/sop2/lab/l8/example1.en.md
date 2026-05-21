---
title: "UDP Test Task"
bookHidden: true
---

## The Hundred Days

On March 1, 1815, to the surprise of the world, Napoleon Bonaparte landed with a small detachment at Juan in France.
Received enthusiastically by the French, he quickly regained power.
Thus began the period known today as “The Hundred Days”, which ended with the Battle of Waterloo.

In this task, we will simulate delivering reports to Napoleon’s headquarters during the battle.
Reports are carried by messengers.
In the chaos of battle, it often happens that a messenger fails to arrive.
Therefore, we will simulate report delivery using the UDP protocol.

For testing, you can use the `netcat` program with the `-u` flag.

## Stages

1. The server program takes one argument: the port number.  
The program waits for datagrams on the given port.
Messages have the form `<X> <Y> <P> <division name>`.
`X` and `Y` are map coordinates (natural numbers in the range from 0 to 99), while `<division name>` is text no longer than 128 characters.
`P` indicates the division’s allegiance and may be `0` (enemy) or `1` (allied).
The message means that the given division (allied or enemy) has moved to the given position.
After receiving a datagram, parse the message and print to the terminal a message of the form `<our/enemy> division <division name> was seen at position <X>:<Y>`.
If the message is malformed, print an error message, but do not terminate the program.

2. In such a fierce battle, there is considerable confusion even within headquarters itself.
Incoming messengers throw reports onto a stack by the entrance, from where they are picked up by four adjutants, who use them to update the headquarters maps.
Implement a thread pool of adjutants.
After receiving a message, the server adds it to the stack (of size `STACK_SIZE` equal to 16).
The adjutant threads wait for a new message — the one that receives it performs the parsing and prints the message as in the first stage.
Use a mutex for synchronization (to protect the stack) and a semaphore or condition variable (for the pointer to the top of the stack).

3. Add updates to the headquarters maps.
Create a shared array of division names among the threads, of size `DIVISION_NAMES_SIZE` equal to 128.
After receiving a new report, an adjutant works on it (that is, sleeps for 10 ms).
Then they check whether the division name is already in the array.
If not, they append it to the end (remember synchronization!).
Add a shared headquarters map — a two-dimensional array of size `100`x`100`, initially filled with `-1`.
The adjutant updates the division’s position on the map — that is, they look up its number, set its previous field to `-1`, and then write the division number (its index in the division names array) to the coordinates from the message.
To ensure synchronization, add one mutex per map row.

4. Add Napoleon’s thread.
Remember the addresses from which the last report for a given division arrived.
Every 30 ms, the Emperor of the French prints the state of the map.
Then he chooses a random allied division and sends it an order of the form `<X> <Y> <P> <division name>`.
