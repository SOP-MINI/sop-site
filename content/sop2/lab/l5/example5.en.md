---
title: "Task on pipe/FIFO"
date: 2022-02-01T19:32:59+01:00
bookHidden: true
---

## Task

Write a program to simulate "My Ship Sails" using processes that communicate via pipes.  
The program takes two arguments:  
`N` - number of players (`4 <= N <= 7`)  
`M` - number of cards per hand (`M >= 4` and `M ⋅ N <= 52`)  

## Rules

Players are dealt `M` cards each from a 52-card deck (cards are represented as integers `0-51`).  
Each turn, players simultaneously pass a card to their right neighbor.  
The game continues until a player collects `M` cards of the same suit and declares: `My ship sails!`.  
The first to declare wins.  
The suit of a card can be determined using `% 4` operation.  

## Stages

1. **Initialize:**
   - The server process creates `N` player processes.
   - It shuffles the deck and deals `M` cards to each player via pipes.
   - Each player prints their received hand with their process ID and exits.

2. **Gameplay:**
   - Players form a ring, passing cards via pipes (`nᵗʰ` player → `(n+1 % N)ᵗʰ` player).
   - A player who collects `M` cards of the same suit prints `[PID]: My ship sails!` (game runs endlessly).

3. **Winning Condition:**
   - The server creates a shared pipe for winners to announce victory.
   - A player who wins writes their PID to the pipe, prints `[PID]: My ship sails!`, and exits.
   - The server reads the PID, prints `Server: [PID] won!`, and exits.

4. **Termination:**
   - `Ctrl-C` instantly stops all processes and cleans up resources.
