---
title: "Example task 1 on low-level file operations and poll"
bookHidden: true
---

## Description

Princess Joanna's birthday is approaching in the **Shining Gem** castle. The court cooks want to bake the legendary **Cake**, but only the old grannies know the recipe. They gossip over tea in the castle towers and dictate the steps to the kitchen through **named pipes**. They talk with their mouths full, so the steps arrive in pieces, and when they get carried away gossiping, they mix up the step numbers. On top of that, Joanna visits every granny and spills tea and crumbles cookies over their notes, so the word `raspberries` is sometimes written as `raw~berries`.

Write a program `sop-cake recipe tower1 [tower2 ... tower5]`, where `recipe` is the path to the recipe file and `tower1...` are paths to the named pipes through which the grannies dictate.

The recipe is a 2048-byte file: 32 lines of 64 bytes each (63 characters padded with spaces + `\n`), line `NN` is step number `NN`. A granny sends messages `NN:content\n` (ASCII, at most 127 bytes without `\n`). A message is valid if there is only a number from 0 to 31 before the colon.

Grannies are created with the `granny.sh` script. The command `./granny.sh <tower> <number_of_steps>` creates the named pipe `<tower>`, waits until someone opens it for reading, then dictates the steps, printing them in its terminal, and finally removes the pipe. Start each granny in a separate terminal before you start the program:

```
    terminal 1:  $ ./granny.sh tower1 8
    terminal 2:  $ ./granny.sh tower2 8
    terminal 3:  $ ./sop-cake recipe.txt tower1 tower2
```

Files and descriptors must be handled only with low-level functions. *Busy waiting* is forbidden.

## Stages

1.  If the recipe file does not exist, the program creates it with the `write` function as 32 empty lines (each line is 63 spaces + `\n`). If it exists, the program opens it and checks with `lseek` whether it is exactly 2048 bytes long – if the size is different, it prints "This book is not a recipe!" on `stderr` and exits.

    Then the program opens the `tower1` pipe for reading. If such a file does not exist or is not a named pipe, the program prints "There is no granny in tower X" on `stderr`, where `X` is the given path, and exits. Finally, it reads data from the pipe until `EOF` and prints it unchanged on `stdout`.

    *Hint:* a named pipe is opened with the `open` function just like a regular file – see the [tutorial]({{< ref "/sop1/lab/l2" >}}#named-pipes-in-brief). You can check the file type with the `stat` or `fstat` function and the `S_ISFIFO` macro (`man 7 inode`). Open the recipe file with the `O_RDWR` flag from the start – `mmap` in stage 4 will need it.

    *To show:*
    ```
    terminal 1:  $ ./granny.sh tower1 8
    terminal 2:  $ ./sop-cake recipe.txt tower1
    terminal 2:  $ cat recipe.txt                     # the created file
    terminal 2:  $ ./sop-cake recipe.txt recipe.txt   # no granny
    terminal 2:  $ ./sop-cake granny.sh tower1        # wrong recipe
    ```
2.  Instead of printing the data from the pipe, the program splits it into messages terminated with `\n` – one message may arrive in several `read` calls. The program writes a valid step in the `NN:content` format into line `NN` of the recipe file. Only `content` goes into the file, without the `NN:` prefix, truncated to 63 characters and padded with spaces. Then the program prints "Step NN saved" on `stdout`, where `NN` always has two digits (e.g. `07`). Invalid messages are skipped and the program prints "Granny got carried away: <message>" on `stderr`.

    *To show:*
    ```
    terminal 1:  $ ./granny.sh tower1 8
    terminal 2:  $ ./sop-cake recipe.txt tower1
    terminal 2:  $ cat recipe.txt                     # the saved steps
    ```
3.  The program opens all given pipes (tower1 ... tower5) and waits for data from all of them at once using the `poll` function. When it detects the end of file on a pipe, the program closes its descriptor, prints "Granny from X finished dictating" on `stdout` (X is the path of the pipe) and removes it from the set monitored by `poll`. If no data arrives on any pipe for 3 seconds, the program prints "Grannies are pouring more tea..." on `stdout` and keeps waiting.

    *Hint:* pieces of messages from different pipes may arrive alternately, so each pipe needs its own buffer for the unfinished message.

    *To show:*
    ```
    terminal 1:  $ ./granny.sh tower1 8
    terminal 2:  $ ./granny.sh tower2 8
    terminal 3:  $ ./granny.sh tower3 8
    terminal 4:  $ ./sop-cake recipe.txt tower1 tower2 tower3
    ```
    The message about pouring tea is easiest to see when running only one granny.
4.  When all pipes are closed, the program maps the recipe file into memory with the `mmap` function. In memory, it replaces every occurrence of `raw~berries` with `raspberries`. Then it prints on `stdout`:
    - "Princess Joanna's stains cleaned: N", where N is the number of replaced occurrences,
    - every non-empty step (a line that does not consist of spaces only) in the `NN: content` format, in the order of step numbers, where `NN` always has two digits and `content` is printed without the padding spaces at the end of the line,
    - "Missing K steps, the cake will collapse!", where K is the number of empty lines, or "Cake ready!" if no line is empty.

    *To show:*
    ```
    terminal 1:  $ ./granny.sh tower1 8
    terminal 2:  $ ./granny.sh tower2 8
    terminal 3:  $ ./granny.sh tower3 8
    terminal 4:  $ ./sop-cake recipe.txt tower1 tower2 tower3
    terminal 4:  $ grep -c '~' recipe.txt             # should print 0
    ```

Remember to release all resources at the end by calling `munmap` and closing all open descriptors!


<em>code for the <b>granny.sh</b> file</em>
{{< includecode "granny.sh" >}}
