---
title: "L1 - Filesystem"
date: 2022-02-07T19:36:16+01:00
weight: 20
---

# Tutorial 1 - Filesystem

{{< hint info >}}
This tutorial contains the explanations for the used functions and their parameters.
It is still only a surface-level tutorial and it is **vital that you read the man pages** to familiarize yourself and understand all of the details.
{{< /hint >}}


## Browsing a directory

Browsing a directory makes us possible to know names and attributes of the files which that directory contains.
This task is accomplished e.g. by the terminal's command `ls -l`. 
However, in order to access this information from the C language, it is needed
to 'open' the directory using `opendir` function, and then read the subsequent records with `readdir` function.
The aforementioned functions are present in `<dirent.h>` header (`man 3p fdopendir`). Let us look at the definitions of these functions:

```
DIR *opendir(const char *dirname);
struct dirent *readdir(DIR *dirp);
```

As we can see, `opendir` returns a pointer to the object of `DIR` type, which we will use to read the directory content. The function `readdir` returns a pointer to the `dirent` structure, which contains (according to POSIX) the following fields:

```
ino_t  d_ino       -> file identifier (inode number, more: man 7 inode)
char   d_name[]    -> filename
```

The remaining file information can be read using `stat` or `lstat` functions from the `<sys/stat.h>` header (`man 3p fstatat`). 
Their definitions are as follows:

```
int  stat(const char *restrict path, struct stat *restrict buf);
int lstat(const char *restrict path, struct stat *restrict buf);
```
- `path` is here the path to the file,
- `buf` is the pointer to (already allocated) `stat` structure (not to be confused with the function name!), which contains the file information. 

In the manuals, the keyword `restrict` is often present in the definitions of function arguments. This is the declaration stating, that the given argument
has to be a block of memory separate from other arguments. 
In this case, passing the same block of memory (e.g. the same pointer) to more than one argument is a serious error and may cause a SEGFAULT or program malfunction.

The only difference between `stat` and `lstat` is the link handling. `stat` returns information about the file pointed by a link, while `lstat` return information of the link itself.

The `stat` structure contains, among others, information about the file size, owner, and last modification date. There are also some macros available, which can be used to check the file type. The important examples of that macros are as follows:
- Macros accepting `buf->st_mode` (field of type `mode_t`):
   - `S_ISREG(m)` -- checking if this is a regular file,
   - `S_ISDIR(m)` -- checking if this is a directory,
   - `S_ISLNK(m)` -- checking if this is a link.
- Macros of type `S_TYPE*(buf)` accepting the `buf` pointer, for identifying file types such as semaphores and shared memory (more in the next semester). 
The details can ba found in `man sys_stat.h`. It is worth familiarising yourself with all the attributes of the `stat` structure and macros, there is quite a lot of them.

After browsing the directory, one should (as good programmer and wanting to pass the course) remember to release resources using the `closedir` function.

### Technical information

In order to browse the entire directory, the `readdir` function should be called repeatedly until it returns `NULL`.
If an error occurs, both `opendir` and `readdir` return `NULL`. An important conclusion follows from this for the `readdir` function: before calling it, the `errno` variable should be set to `0`, 
and if `readdir` returns `NULL`, one should check that this variable has not been set to a non-zero value (indicating an error).
`errno` is a global variable used by system functions to indicate the code of an error encountered.
The `stat`, `lstat` and `closedir` functions return `0` if successful, any other value indicates an error.

### Exercise

Write a program counting objects (files, links, folders and others) in current working directory.

### Solution

New man pages:
```
man 3p fdopendir (only opendir)
man 3p closedir
man 3p readdir
man 0p dirent.h
man 3p fstatat (only stat and lstat)
man sys_stat.h
man 7 inode (first half of the "The file type and mode" section)
```

solution `l1-1.c`:
{{< includecode "l1-1.c" >}}

### Notes and questions 

- Run this program in the folder with some files but without sub-folders, it may be the folder you are working on this tutorial in. Is the folder count zero? Explain it.
{{< answer >}}
No, each folder has two special *hard-linked* folders -- `.` link to the folder itself and `..` the link to the parent folder, thus program counted 2 folders.  
{{< /answer >}}

- How to create a symbolic link for the tests?
{{< answer >}}
```shell
ln -s prog9.c prog_link.c
```
{{< /answer >}}

- What members are defined in `dirent` structure according to the LINUX (`man readdir`)? 
{{< answer >}}
Name, inode and 3 other not covered by the standard.
{{< /answer >}}

- Where the Linux/GNU documentation deviates from the standard always follow the standard -- it will result in better
portability of your code.

- Error checks usually follow the same schema: `if(fun()) ERR();` (the `ERR` macro was declared and discussed before). You should
check for errors in all the functions that can cause problems, both system and library ones. In practice, nearly all
those function should be checked for errors. Most errors your code can encounter will result in program termination, some exceptions will be discussed in the
following tutorials.

- Pay attention to the use of `.` folder in the code, you do not need to know your current working folder, it's simpler
this way.

- Please notice that `errno`
is reset inside the loop, not before it. Also notice that in case of `NULL`, the program flows to the comparison of `errno`
through simple conditions only (no function calls).

- Why do we need to zero `errno` in first place? `readdir` could do it for us, right? The clou is that the POSIX says that
system function *could* zero `errno` on the the success but it is not obliged to do it.

- If you want to make assignments inside logical expressions in C, you should add parenthesis, its value will be equal to
the value assigned, see `readdir` call above.

## Working directory

The program from the previous excercise only allowed to scan the contents of the directory, which it was run in.
It would be much better to choose the directory to be scanned. As we can see, that it would be sufficient to replace the `opendir` argument to the path given e.g. in the positional parameter. 
Despite that, we won't modify the `scan_dir` function, in order to present the way to load and change the working directory from within the program code.

In order to get and change the working directory, we will make use of `getcwd` and `chdir` functions, present in the `<unistd.h>` header (`man 3p getcwd`, `man 3p chdir`). Their declarations, according to POSIX, are as follows:

```
char *getcwd(char *buf, size_t size);
```
- `buf` is the already allocated array of characters, that the **absolute** path to the working directory will be written into. This array should have at least `size` length,
- the function returns `buf` when successful. In case of failure, `NULL` is returned, and `errno` is set to the appropriate value.

```
int chdir(const char *path);
```
- `path` is a path to the new working directory (may be either relative or absolute),
- like many system functions that return `int`, the `chdir` function returns `0` on success and a different value on failure.

### Excercise

Use function from `l1-1.c` to write a program that will count objects in all the folders passed to the program as positional parameters.

### Solution

New man pages:
```
man 3p getcwd
man 3p chdir
```

solution `l1-2.c`:
{{< includecode "l1-2.c" >}}

### Notes and questions 

- Check, how this program deals with:
   - non existing folders, 
   - no access folders, 
   - relative paths and absolute paths as parameters?

- Why this program stores the initial working folder?
{{< answer >}}
Paths given as parameters may be relative to the start working folder.
The program changes location in the tree of folders to "be" in folder pointed by the parameter but this automatically
invalidates all other relative paths (other parameters) to make other relative paths valid again program must go back to
initial position in the three. 
{{< /answer >}}
This is the solution to the case where the user specifies several relative paths as parameters, e.g. 
`l1-2 dir1 dir2/dir3`. The program from the solution changes the working directory to the target directory before scanning. 
Thus, if we did not return to the starting directory each time after browsing the directory,
we would have tried to visit the `./dir1/` folder first (this is still correct) and then `./dir1/dir2/dir3/` instead of the 
the anticipated `./dir2/dir3/`.
{{< /answer >}}

- Not all errors encountered in this program has to terminate it, what error can be handled in better way, how?
{{< answer >}} 
The `chdir` function may, for example, indicate an error for a non-existent directory. This could be handled with
`if(chdir(argv[i])) continue;`. It is the simplest solution, but it would be nice to add some message to it.
{{< /answer >}}

- Never ever code in this way: `printf(argv[i])`. What will be printed if somebody puts `%d` or other `printf` placeholders in
the arguments? This applies to any string, not only the one from arguments.

## Browsing directories and subdirectories (recursive)

TODO
Goal: 
Write a program that counts all occurrences of the files, folders, symbolic links and other objects in a sub-trees rooted at locations indicated by parameters.  
What you need to know:
- man 3p ftw
- man 3p nftw

<em>code for <b>prog11.c</b> file:</em>
{{< includecode "prog11.c" >}}

- Make sure you know how to declare and use pointers to functions in C
- Test how this program reacts on not available or not existing folders.

Why `FTW_PHYS` flag is applied?
{{< answer >}}
Without it nftw trawers the links to real objects, so it can not count them, similar to stat and lstat functions.
{{< /answer >}}

How other flags modify nftw algorithm?

Declaration of `_XOPEN_SOURCE` has to be placed prior to the include if you wish to use nftw, otherwise you can only use
obsolete ftw.

Global variables are known to be the root of all evil in coding. They are very easy to use and have many negative
consequences on code portability, re-usability and last but not least on how easily you can understand the code. They
create indirect code dependencies that are sometimes really hard to trace. Unfortunately there are exceptional
situations where we are forced to use them. This is one of such a cases because nftw callback function has no better way
to return its results to the main code. Other exceptions will be discussed soon. You are allowed to use globals ONLY in
those defined cases.

`nftw` descriptor limit is a very useful feature if you have to restrict the descriptor resources consumed. Take into
account that the limit should be no less that the depth of the scanned tree otherwise you run out of the descriptors
before reaching the bottom of the tree. In Linux, descriptor limit is not defined but administrator can limit individual
processes.

## Task 4 - file access and operations 

Goal:
Create a new file with name, permissions and size specified by parameters (-n NAME, -p OCTAL, -s SIZE). Content of the
file is 10% of random [A-Z] chars at random positions, the rest is filled with zeros (not "0", ascii code zero). If the
given name is occupied, erase the old file.

What you need to know:
- man 3p fopen
- man 3p fclose
- man 3p fseek
- man 3p rand
- man 3p unlink
- man 3p umask
- [Glibc documentation on umask](http://www.gnu.org/software/libc/manual/html_node/Setting-Permissions.html)

<em>code for <b>prog12.c</b> file:</em>
{{< includecode "prog12.c" >}}

What bitmask is created with: `~perms&0777` ? 
{{< answer >}} 
Reverted permission bits cut to 9 least significant bits. If you do not know how it works then learn C bit operations.
{{< /answer >}}

How char randomization works in this program? 
{{< answer >}}
Program iterates through all the chars in sequence: A,B,C ... Z and then A again, For each char random position in the file is chosen. 
Expression  'A'+(i%('Z'-'A'+1)) should have no secrets for you, please make sure you know how it works.
Such a construction will return many times during our labs. 
{{< /answer >}}

Run the program few times and check the sizes of the files created ("$ ls -l"), are they always equal to the expected ones?
How can you explain the difference for small files sizes and how for big (64K+) ones?
{{< answer >}}
Most of the time you will observe smaller files than expected. In both size ranges the problem has the same root 
file creation method. First of all the file is initially empty, we place chars at random positions, 
quite often not the last position in the file, and that's why smaller files are somehow smaller.
Big size range is also affected by the limit on the value of random number you can obtain from rand function. 
It can be at most RAND_MAX (the actual value can vary, expect 2 bytes number in Linux).
The highest position occupied by non zero character will not be larger than RAND_MAX 
thus the output file can not be larger than RAND_MAX. 
{{< /answer >}}

Modify the program to always produce files of expected size.

Why one case of unlink error is ignored?
{{< answer >}} 
ENOENT means no file to remove, in this case it can't be considered a problem, there is nothing to delete. 
File removal is necessary only if the file exists.
{{< /answer >}}

The larger code the more functions it should contain. File creation code is a good candidate to write a good function.
Let's make sure you know how a good function looks like:

- It does one thing (short code)
- it is as general as possible, solves a range of problems not only one case, it can be reused in many places in the
  code in various programs (in this case it has percentage parameter)
- All the data function requires to work is passed as parameters (no globals).
- All results are passed back as return value or as output parameters. In this program the results are stored in a file,
  but again no globals.

Numeric types like `ssize_t`, `mode_t` are used to avoid casting when retrieving data from system functions.

Why umask is called here? `fopen` does not allow us to control permissions of newly created file, umask can narrow down
the default ones selected by `fopen`. Descriptor level function `open` offers better control over the permissions.

Why `x` permissions can not be set in this program? Default permissions on a new file created with `fopen` is `0666` 
(not full `0777`) we can not increase it by subtracting bits.

You were asked to check for all errors but in this example I do not check umask errors, why? 
Umask function can not report errors, it returns the former value of umask.

Umask value is a process property, as a part of environment it is inherited by child processes 
but does not propagate up the process tree.

To parse text containing octal numer (properties -p) you can use `strtol`, 
there is a very long list of C functions worth knowing.

Why to erase the file if we know that `w+` opening mode will wipe the content of the old one?
Old set of permissions will be preserved and we need to set a new ones.
Try to comment the erasing statement and see for yourself. Additionally, you can practice unlinking.

Opening mode `b` has no real effect o POSIX system, all files in this standard are binary.

In the program we do not need to fill the file with zeros, 
it happens automatically every time we write something past the current end of the file, the gap is filled with zeros.
You should know that if a sequence of zeros is long enough to fill full sector of the disk then this sector is not physically stored on the drive.

If you unlink a file it will disappear from the folder but it will still be accessible for the process that is using this file. 
Once closed by all the process it will be erased for good.

You should call `srand` only once per process and make sure it gets unique seed, in this program time in seconds is sufficient.

## Task 5 – stdout buffering

Note that this topic is less about operating systems and more about general C programming, 
however we mention it for completeness as related issues were quite common in the past years.

<em>code for <b>prog13.c</b> file:</em>
{{< includecode "prog13.c" >}}

Try running the above (very simple!) code from the terminal. What do you see? 
{{< answer >}} As expected, you just see one number each second, no funky business… yet.
{{< /answer >}}

Try running the above code again, but this time redirect it to the file: `./executable > file`. Afterwards,
try opening the file while the program is running, and then `Ctrl+C` the program and reopen the file again. What do you
see?
{{< answer >}}
If you do this quickly enough, the file will be empty! That is because the standard library
detects that the output does not go to the terminal, and buffers it for performance, which means that the actual write
operation happens only when enough data accumulate. This means that the data is not available immediately, and can even
be lost in case the program ends abnormally (for instance when we use `Ctrl+C`). Of course, if you let the program run
until completion, it will output all the data in the end (try it out!). While this is configurable, you will not need to
do so as you will see in a second.
{{< /answer >}}

Try running the above code again, with output going to the terminal (like in the first step), but remove the newline
from the `printf` function argument: `printf("%d", i);`. What do you see?
{{< answer >}}
Surprisingly, even though the output goes directly to the terminal, the same thing as in the second step happens and we
don't see any output. That is because the buffering happens even in this case; only now a newline tells the standard
library to flush all the data. This is why you didn't see anything weird happening in the first step. This is also why
you might find that your `printf` is not outputting anything; if you forget the newline at the end of the
string, the standard library will not do anything until there actually is a newline or the program ends correctly.
{{< /answer >}}

Retry the first three steps, but output to the standard error instead: 
`fprintf(stderr, /* parameters you passed to printf */);`. 
What happens now? Note that in order to redirect standard error to file, you need `>2` instead of `>`. 
{{< answer >}}
Now there is no buffering: in all three cases you really see a number each second.
Indeed, the standard library will not buffer standard error, as this stream is meant for debugging. 
{{< /answer >}}

You might find yourself debugging your code using `printf(...)`, by adding calls to this function and
checking the values of parameters and/or seeing if those places in code are reached. When you do so, you should rather
use `fprintf(stderr, ...)` in order to output to standard error. Otherwise, as you have seen, the output data
is buffered by standard library and in some cases might never be output despite the call succeding. In general, if in
doubt, prefer standard error.

When writing actual console applications we tend to use standard output only to output actual results, and standard
error for everything else. For instance `grep` will output lines that contain the matched string to standard
output, but if it can't open the file, it will complain on the standard error. Note that our `ERR` macro also
outputs to standard error.

## Task 6 - low-level file access

Write a simple file-copying program.
It should accept two file paths as arguments, and copy the file from the first path to the second one.

This time to implement reading and writing we will use low-level functions, i.e. ones which are not defined by the standard C library, but which are exposed by the operating system itself. They are trickier to use, but are more universal. You can use them for e.g. network communications, which we will consider in the next semester.

<em>What you need to know:</em> 
- man 3p open
- man 3p close
- man 3p read
- man 3p write
- man 3p mknod (only open new file permissions constants)
- macro TEMP_FAILURE_RETRY description <a href="http://www.gnu.org/software/libc/manual/html_node/Interrupted-Primitives.html">here</a>

<em>code for <b>prog14.c</b> file:</em>
{{< includecode "prog14.c" >}}

For a program to see `TEMP_FAILURE_RETRY` macro you must first define `GNU_SOURCE` and then include header
file `unistd.h`. You don't have to understand it fully for now, it will get more important during the next laboratory when we tackle signals.

Why are `bulk_read` and `bulk_write` functions used in the above program?
Would not it suffice to just call `read` or `write`?
{{< answer >}}
According to the specification, `read` and `write` functions can return before reading/writing the amount of data the caller asked for.
You will learn more about this aspect in the tutorial for the next laboratory.
In theory this does not matter in this task (we are not using signals), but getting used to this pattern now is a recommended idea.
{{< /answer >}}

Could the above program use C library functions instead of low-level IO? (`fopen`, `fprintf`, ...)
{{< answer >}}
Yes, everything done in this program could be achieved using functions introduced earlier.
{{< /answer >}}

Can you write data to a descriptor returned by `open` using `fprintf`?
{{< answer >}}
No! `fprintf`, `fgets`, `fscanf` etc. function accept a variable of type `FILE*` as their argument, a descriptor on the other hand is just a number of type `int` used by the operating system to identify an open file.
{{< /answer >}}

## Example tasks

Do the example tasks. During the laboratory you will have more time and a starting code. If you do following tasks in the allotted time, it means that you are well-prepared.

- [Task 1]({{< ref "/sop1/lab/l1/example1" >}}) ~75 minutes
- [Task 2]({{< ref "/sop1/lab/l1/example2" >}}) ~75 minutes
- [Task 3]({{< ref "/sop1/lab/l1/example3" >}}) ~120 minutes

## Source codes presented in this tutorial

{{% codeattachments %}}
