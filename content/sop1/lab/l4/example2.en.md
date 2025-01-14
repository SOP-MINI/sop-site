---
title: "Laboratory task 4: synchronization"
bookHidden: true
---

# Description

Write a program that simulates the course of "My Ship Sails" card game
using POSIX threads and appropriate synchronization primitives. The
program accepts a single parameter `n` indicating the table size
(`4 ≤ n ≤ 7`), specifying the number of players to wait for and
play concurrently in the "My Ship Sails" card game simulation.

## Game Rules

In the "My Ship Sails" game, players are initially dealt seven cards
each from a standard 52-card deck. Throughout the game, players take
turns simultaneously passing a card to the player on their right. The
card-passing sequence continues until a player accumulates seven cards
of the same suit and declares: "My ship sails!". If multiple players
satisfy the condition simultaneously, they all win.

## Task Description

The deck in this card game simulation should be represented as an
integer array with numbers ranging from 0 to 51. Each card's suit is
then determined by the modulo operation with 4, and its value is
calculated by dividing the card number by 4.

The simulation involves a host (main) thread and player threads. The
host thread functions as the central coordinator, managing the table,
dealing cards, inviting players to the table, and requesting players to
leave.

The host waits for the `SIGUSR1` signal as an indication of a new player's
arrival. Upon receiving this signal, the host thread attempts to seat
the player at the table. If there is available space at the table, the
player is seated (by creating a new thread dedicated to them), and cards
are dealt to the player. However, if the table is full, the host outputs
a message indicating that there is only room for `n` people at the
table.

After the player is seated, the player's thread prints its hand to
stdout and waits for other players.

When `n` players gather at the table, the game begins. Players
concurrently check for the win condition and then concurrently pick up a
random card from their hand and pass it to a player to their right. This
process continues until one of the players meets the win condition. Upon
meeting the criteria, the player prints a message to stdout stating:
"My ship sails!" along with their current hand and signals the other
players that the game has concluded.

After the game, all the players leave the table (player threads finish),
and new players might be seated. The host shuffles the deck from the
previous game when the next player enters.

When a SIGINT (`C-c`) signal arrives, the host signals all players to
exit immediately (waiting for the game or playing). All resources are
properly cleaned up, and the program exits.

# Stages:

- The host creates `n` threads. Each thread is dealt 7 cards. Each thread
prints its hand and exits. The host waits for all threads and exits.
- The host waits for `SIGUSR1` signal and properly tries to seat a new player.
The player prints their hand and waits for other players at the table.
When all the players gather, they all exit, and the table is available
for the subsequent group of players. 
- The players play the game according to the rules. **Hint**: Barrier could be used to synchronize the stages of the
game.
- Termination of the program by `C-c` works, all threads waiting or
playing immediately leave, and all resources are properly cleaned up.
**Hint**: A conditional variable could be used to notify players waiting at
the table to either leave or start the game.

# Starting code

- [sop1l4e2.zip](/files/sop1l4e2.zip)
