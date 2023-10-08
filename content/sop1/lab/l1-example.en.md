---
title: "Graded task 1 on POSIX program environment"
date: 2022-02-07T20:01:59+01:00
bookHidden: true
---

## The problem:

Write a program that accepts the following positional parameters pairs:

  - path  
    path to the folder in the file system
  - bytes  
    limiting folder content size

The program appends the name of the folder given by "path" to the file out.txt only if the total sum of contained objects sizes is above the limit given by "bytes". The same procedure apply to all the folders given in parameters. Program sums sizes of all object types (folders,directories, links, etc.) in the folder, it does not count the sizes of sub-folders. If the folder can not be checked (does not exist or no access) proper message must be printed on stderr but the program should continue to check other folders. Example.:

    $./prog /etc 5000 /run 2000 /root 100
    No access to folder "root"
    $./cat out.txt
    /etc

In the above example size of all objects in /run is below 2000 bytes.

## Graded stages (3 points each, please report the stage as soon as you finish it).

1.  Program lists the names of objects in the working folder. *To show:* run the program without params
2.  Program lists the sizes (without names) of objects in the working folder, counts the sum of sizes and prints it on the screen. *To show:* run the program without params
3.  Extend the code to accept folders as parameters (sizes are ignored) and repeat the printout for every folder. *To show:* run the program with params: /etc 1000 /run 200
4.  Program prints only the names of folders if the content size is above the limit. *To show:* run the program with params: /etc 1000 /run 200, adjust the sizes so one folder will be printed and other not
5.  Move output to the file out.txt, add message on access problems. To show: run the program with params as in the example below the task.
