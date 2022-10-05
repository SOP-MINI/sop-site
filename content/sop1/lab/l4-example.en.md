---
title: "Laboratory task 4: AIO"
date: 2022-02-07T20:02:11+01:00
bookHidden: true
---

## Task

The program takes 2 parameters: the filename f and the number of blocks n. The file size must be divided into n blocks of equal size and the reminder part of the file R. Program starts asynchronous read operation for each block in turn. The AIO read operation must start the thread on the completion. The thread must replace all not alphabetical characters in the currently processed block with spaces, then the thread should write the block synchronously at the same position in the file and terminate. After processing all the n blocks the program must process the R part of the file in the same way. The file f can be opened only once in the code. To synchronize the end of block processing and the main thread, the signal SIGIO must be sent from the working thread, which should wait for it synchronously. NOTICE: to do it correctly the working thread must know TID of main thread and use pthread\_kill (as signal masks of aio started threads are undefined).

## Stages

1.  Analyse the size of the file and print all the blocks offsets and sizes, prepare all the aiocb structures you need in the program (4p)
2.  Start one aio\_read operation on the first block, thread should start and the buffer read from the file must be processed as described in the task. The result should be printed on the screen, main thread should wait 3sec. for the working thread and exit. (5p)
3.  Add threads synchronization with signal, main thread should wait for the signal and then exits (4p)
4.  Write the block processing result into the file. Process all the blocks not just one (4p)
