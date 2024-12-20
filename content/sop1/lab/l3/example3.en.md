---
title: "Laboratory task 3: threads, mutexes, signals"
date: 2022-02-07T20:02:06+01:00
bookHidden: true
---

## Task

You need to write a program simulating a dog race. The program takes two
natural numbers as arguments: `n>20` - the length of the track, and
`m>2` - the number of dogs. The main thread of the program creates a
racetrack for dogs - an array of integers of length `n`. The array is
initially filled with zeros. The array will store the number of dogs at
each position on the track. After initializing the array, the program
creates `m` threads, each thread representing one dog.

The dog’s thread operates as follows: initially, the dog is outside the
track. At the beginning, the thread places the dog at position 0 and
increases the array value by 1. The direction of the dog’s movement is
towards increasing array indices. Then, in a loop, it performs the
following actions:

1.  Waits for a random time in the range `[200, 1520]` ms.
2.  Generates a random integer in the range `[1,5]` - the distance it
    will move.
3.  Checks if the randomly chosen position is within the array range. If
    not, it moves as far as possible, then changes the direction of
    movement.
4.  Checks if there is another dog at the position it has moved to. In
    such a situation, it prints the message `waf waf waf` and stays in
    place.
5.  Prints its number and new position on stdout, and optional
    information about reaching the end of the track.
6.  Updates the array values accordingly (decreases the value at the
    position it leaves and increases it at the position it arrives at).

After reaching the end of the array, the dog prints the order in which
it arrived at the finish line and finishes its work.

The main thread prints the state of the array every 1000 ms. When each
dog finishes the race, the program should print the three fastest dogs
and terminate. In response to the `SIGINT` signal, the main thread
cancels all dog threads, waits for their completion, and then the
program ends, printing information about the interrupted race.

Access to the array should be locked with a separate mutex for each
element. The counter of dogs that have reached the finish line should
also be locked with a mutex.

The program must not use any global variables.

## Graded stages

1. Read the arguments, initialize the array. Create dog threads. Each
   thread increases a random array cell by 1, prints its number, and
   finishes. After all dog threads finish, the main thread prints the final
   state of the track.
2. Implement the logic of the dog thread, initially without locking.
3. Implement locking using mutexes.
4. Implement program termination using SIGINT.
