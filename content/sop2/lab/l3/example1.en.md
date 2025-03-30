---
title: "Task on shared memory and mmap"
bookHidden: true
---

## Goal

The goal is to write a program that counts the occurrences of individual characters in a given file.
When writing the program, you can assume that each character is represented by a single byte.

To speed up computations, the program should use shared memory for synchronization between multiple processes.
For easier file processing, the file should be mapped into the process memory using the mmap function.

## Stages

1. Open the file using the mmap function in the parent process.  
   Print its contents to the standard output.  
   The use of streams and the read function is prohibited.  

2. Implement the logic for counting character occurrences in the file.  
   At the end of the program execution, print a summary of how many times each character appeared in the file.  

3. Distribute the workload across N child processes.  
   Move the file opening operation to the child process.  
   Each process should count characters independently of the others.  
   Use shared memory to transfer the computation results to the parent process.  
   The parent process should print the summary after all child processes have finished execution.  

4. Add handling for the case where a child process terminates unexpectedly.  
   In such a case, the parent process should skip printing the summary.  
   Instead, it should print a message stating that the computation failed.  
   Each child process should have a 3% chance of sudden termination when reporting results to the parent process (implemented using abort).  
