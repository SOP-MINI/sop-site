---
title: "Task on pipe/FIFO"
date: 2022-02-01T19:32:59+01:00
bookHidden: true
---

## Task

Simulation of a multiplayer game. The program receives two integer arguments - `2 ≤ N ≤ 5` and
`5 ≤ M ≤ 10`.

In the task we simulate a card game. The main process - which will be referred to as the server - creates `N` child processes - which will be referred to as players. The players and the server communicate through pipes. At the start every player has `M` cards numbered from 1 to `M`. The server cyclically announces the start of a new round to the players. Every player must then choose and send to the server one (let's assume random) card from the ones he has left (they can't use a card twice). The player who sends the highest card wins the round and gets `N` points. When a draw occurs, the points are divided evenly among the winners and rounded down (eg. when there are 5 players in the game and 2 of them win, they both get 2 points). After `M` rounds the game ends and the player with the most points wins.

1. The program (server) creates `N` player processes, every one is connected to the server by 2 pipes - one for sending and one for receiving messages. In the player processes **srand** is called to change the seed of the random number generator so that every player plays differently.

2. All the messages sent between the server and the players are always 16 bytes (padded by zeros when needed).

3. At the start of every round the server writes `NEW ROUND` to the terminal and sends the `new_round` message to every player.

4. Every player waits for the `new_round` message from the server and chooses one of their cards randomly. They send that card to the server in the form of a single number representing the value of that card.

5. After receiving a card from every player, the server chooses the winner(s) of the round based on the rules above. It sends back the number of points a given player has gained (in case of a loss, the player gets 0 points). The server process writes which player has played what card to the terminal.

6. After `M` rounds the player processes should terminate. The server process prints the table of results to the terminal, waits for the player processes to terminate and also terminates.

7. When the server can't send or receive a message to or from a player process, it prints an announcement to the terminal and continues the game without that player.

8. Every unused end of a pipe should be closed. Obviously, every other resource should be freed when they aren't necessary.

## Stages

1. Correct program initialization, creation of the player processes and the required pipes.

2. Every player process sends a random nonnegative integer `≤ M` to the server, for each the server process writes `Got number <X> from player <index>` to the terminal.

3. Implementation of rounds - the server cyclically sends to the players announcements that a new round has started. Players respond with a random card.

4. Full implementation of the game's rules, as above.

5. After a round starts, each player has a 5% chance of "failure" (premature termination). The server process correctly responds to such situation.
