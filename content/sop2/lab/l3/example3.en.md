---
title: "Task on shared memory and mmap"
bookHidden: true
---

## Data Channel

Your task for today's laboratory is to implement channel structure inside shared memory.
It is a place where one process *Producer* can put data and one of many processes *Consumers* can read it.
It is something similar to pipe with water in real world.

![Channel schematic](/channel_schematic.png)

This structure has two basic functions:`produce()` and `consume()`.  
Calling `consume()` when there is no data to process will block the calling process.  
From the other side, calling `produce()` when there is still data in channel will also block the process.  
When there is one process calling `produce()` from one side and  
another one calling `consume()` from the other side data would *flow* (like data in `pipe` from L5).

On our laboratory channel can hold **single** string.  
To push another string first one has to be read by consumer.  
Maximum length of held string is 4096 bytes.  
Your task is to implement program (`ops-double-processor.c`) which:
1. Grabs data from input channel (this operation can block if there is no data in channel)
2. Doubles every character from input (for e.g. `"abc cda" -> "aabbcc  ccddaa"`)
3. Puts modified data to output channel (this operation can block if there is any data in channel)

To make it easier you have access to three programs: `ops-generator`, `ops-printer` and `ops-cleanup`.  
The first one is reading a file line by line and pushing data to some output channel.  
The second one grabs data from input channel and prints it to standard output.  
The last one unlinks semaphores and channels by given names.  
Calling those programs without arguments shows how to use them.

## Stages
1. Implement functions for opening (`channel_open`) and closing (`channel_close`) the channel (a shared memory object).  
   If there is no shared memory object with given name, it should be created and initialized correctly.  
   To eliminate race between `shm_open` and initialization of channel use named semaphore.

   > **Check:** `$ ./ops-double-process ch1 ch1 && ./ops-cleanup ch1 && ls /dev/shm`
   
   > **Hint:** After `mmap` of channel structure you can check `status` field if it is  
   > `CHANNEL_UNINITIALIZED` to find out that it requires initialization.

2. Implement function `channel_consume()`.  
   It should wait on `consumer_cv` until channel will change status to `CHANNEL_OCCUPIED`.  
   Then copy data from channel to local memory of process and signal other processes through `producer_cv`.  
   This function should return `0` when it read data correctly.  
   If channel is depleted (`status == CHANNEL_DEPLETED`) `1` should be returned.  
   Print received data to standard output.

   > **Check:** `$ ./ops-generator ops-generator.c ch1 & ./ops-double-processor ch1 ch1`

   > **Hint:** It should look almost the same as running: `$ ./ops-generator ops-generator.c ch1 & ./ops-printer ch1`

3. Implement function `channel_produce()`.
   It should wait on `producer_cv` until channel will change status to `CHANNEL_EMPTY`.
   Then copy data from private process memory to `data` field and signal one process through `consumer_cv`.

   Now instead of printing to standard output put received data to output channel.
   After consuming last element from input channel (`consume` function returned depletion of input channel)
   mark output channel depleted.

   > **Check:** `$ ./ops-generator ops-generator.c ch1 & ./ops-double-processor ch1 ch2 & ./ops-printer ch2`

   > **Hint:** After marking output channel as depleted remember about signaling all consumers at the other end.

4. Implement duplication logic of input data.

   > **Check:** 
   > ```
   > $ ./ops-generator ops-generator.c ch1 & ./ops-double-processor ch1 ch2 & \
   >   ./ops-printer ch2
   > $ ./ops-generator ops-generator.c ch1 & ./ops-double-processor ch1 ch2 & \
   >   ./ops-printer ch2 & ./ops-printer ch2 & ./ops-printer ch2
   > ```

   > **Hint:** Remember about channel size limits.  
   > Result of duplication will be two times longer that input.  
   > Consider dividing it to more `produce` calls.

## Starting code

- [sop2l3e3.zip](/files/sop2l3e3.zip)
