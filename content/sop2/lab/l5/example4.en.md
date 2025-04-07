---
title: "Task on pipe/FIFO"
date: 2022-02-01T19:32:59+01:00
bookHidden: true
---

## Task

Write a roulette simulator using processes communicating through pipes.
The program takes two parameters: number of players `N` (`N ≥ 1`) and player starting money `M` (`M ≥ 100`).

Players bet on a single number `[0, 36]` with a `35:1` payout. The winning number is chosen randomly from `[0, 36]`.

## Stages

1. The dealer (main process) creates `N` player processes. Each player prints:
   `[process id]: I have [amount] and I'm going to play roulette`.

2. Players communicate with the dealer via pipes. Each player sends a bet amount (within their balance) and a chosen number.
   - The dealer prints: `Dealer: [process id] placed [amount] on [number]` after receiving a bet.
   - The dealer draws a random number and announces: `Dealer: [number] is the lucky number`.
   - After one round, all players and the dealer exit.

3. The game continues as long as at least one player has money.
   - If a player runs out of money, they print: `[process id]: I'm broke` and exit.
   - If a player wins, they print: `[process id]: I won [amount]`.
   - Once all players exit, the dealer prints: `Dealer: Casino always wins` and exits.

4. Each round, a player has a 10% chance to leave with their remaining money. They print: `[process id]: I saved [amount left]` and exit.
