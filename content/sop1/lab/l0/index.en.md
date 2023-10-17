---
title: "L0 - POSIX program execution environment"
date: 2022-02-07T19:29:05+01:00
weight: 10
---

# Introduction Lab for OPS1

{{< hint info >}}

This laboratory does not require any preparation, its aim is to explain all the rules and answer all the question about graded labs and the classes schedule. Please carefully read everything about [GIT]({{< ref "../../git">}}), [syllabus]({{< ref "../../syllabus" >}}), [grading]({{< ref "../../zasady" >}}) and [schedule]({{< ref "../../harmonogram" >}}). Also have a look at the [reference]({{< ref "../../materialy" >}}).

On [reference]({{< ref "../../materialy" >}}) page you can find self preparation tutorials, you should do them at home before the graded labs. The aim of graded lab is to test how well you studied the topic at home. You can ask questions about the course topics via email at any time.

During the zeroth laboratory you will have to do the simple task to simulate real tasks from further laboratories.
You will receive half-finished code with some bugs. You will have to find those errors and finish its functionalities.
This task will challenge your environment and hopefully will help to find some troubles before first graded laboratories.
{{< /hint >}}

## Environment preparation

Unlike the previous classes, we do not require any particular IDE. However, a good editor should:

- Show live compilation errors (which allows us to learn about them and find relevant code quicker).
- Autocomplete function names (helpful while entering longer names).
- Give you ability to run debugger attached to your code.

A good choice would be Visual Studio Code (not to be confused with Visual Studio) or QT Creator, which is commonly chosen due to its beginner-friendliness. Another common choices are Emacs and Vim (as well as Neovim which is sadly not available in the lab by default), but those have a higher barrier to entry. All mentioned editors require additional configuration, which you should do before the first graded labs. Of course it would be a good idea to have a similar setup at home and during labs.

We suggest you shouldn't use larger IDEs, such as ~~CLion~~ if you do not know them well; otherwise their functionalities will hurt you more than help. Also do not use in-browser editors such as ~~<https://www.onlinegdb.com>~~; otherwise in case of PC instability you might lose your code (as it is only stored in browser) and need to begin from scratch.

{{< hint info >}}
If you want to use Visual Studio Code, you may encounter an issue in which the editor highlights some identifiers as unknown,
even though the program actually builds correctly (usually these are names related to signals, such as `sigset_t` and `SIG_BLOCK`).
In order to fix this, find in `C/C++: Edit Configurations (UI)` the `C standard` and change it from an option starting with `c`
to a corresponding option starting with `gnu`, for instance `gnu17` instead of `c17`.
{{< /hint >}}

## Tasks preparing to laboratory

{{< hint info >}}
Introduction notes:

- This tutorial is fairly easy and rather long, next ones will be harder and shorter
- Quick look at this material will not suffice, you should compile and run all the programs, check how they work, read
  additional materials like man pages. As you read the material please do all the exercises and questions. At the end
  you will find sample task similar to the one you will do during the labs, please do it at home.
- You will find additional information in yellow sections, questions and tasks in blue ones. Under the question you will
  find the answer, to see it you have to click. Please try to answer on you own before checking.
- Full programs' codes are placed as attachments at the bottom of this page.
- Codes, information and tasks are organized in logical sequence, in order to fully understand it you should follow this
  sequence. Sometimes former task makes context for the next one and it is harder to comprehend it without the study of
  previous parts.
- Tasks assume that you can write C code at some basic level (you can use dynamic allocated memory, parse strings, and so on). If you feel not confident with those operations, please consult with Programming 1 materials.
- Most of the exercises require command line to practice, I usually assume that all the files are placed in the current
  working folder and that we do not need to add path parts to file names.
- Quite often you will find $ sign placed before commands you should run in the shell, obviously you do not need to
  rewrite this sight to command line, I put it there to remind you that it is a command to execute.
- What you learn and practice in this tutorial will be required for the next ones. If you have a problem with this
  material after the graded lab you can still ask for help.
{{< /hint >}}

## Task 1 - stdout

Goal: Write and compile (using make program) the simplest program to write on the standard output.
What you need to know:
- man 3p stdin
- man 3p printf
- man stdlib.h
- man make
- Tutorial on gcc and make, <a href="{{< resource "tutorial.gcc_make.txt" >}}">link</a>

I'd like to discourage you from making google searches on API functions. Quite often you will find outdated man pages,
the ones related to different OSes, ones with mistakes and the pages not conforming to the standards like POSIX. It is
best to use man pages placed locally on the lab computers.

All man pages are organized in to numbered sections, they are described in manual page to the man program.

Make sure that you mostly use POSIX pages (section 3p) rather than the Linux/GNU implementation of those function
(section 2 and 3) because they can vary a lot. You are obliged to know and apply standards in your codes rather than
write Linux specific code.

How you can find documentation on man command?
{{< expand "Answer" >}}
```shell
man man
```
{{< /expand >}} 

If you use your own system to practice make sure you have POSIX man pages installed. 
If not, search google on how to install them in your Linux distribution and do it.

Why `man printf` will not help you to understand the `printf` function?
{{< expand "Answer" >}} 
If you do not specify the section, man system opens the lowest number section on given topic.
In this case it opens section 1 on printf program (used in bash). You have to supply section number: `man 3 printf`
to see Linux/Gnu version or better `man 3p printf` to see POSIX version.
{{< /expand >}}

<em>solution <b>prog1.c</b>:</em>
{{< includecode "prog1.c" >}}

Please notice that you should always return int value from main function. 
Although you can change main return value to void it opposes the POSIX that mandates all the processes 
to return success or failure int code on exit

New line symbol (`\n`) at the end of hello string should not be omitted. 
If you run this program without it next command prompt will be printed just after the hello message. 
It is a good practice to always place a new line at the end of text streams or text files. 
It gives you additional benefit of flushing the output stream - stdout and stderr are line buffered, it will be discussed later.

Instead of using numbers to return statuses (zero - success, everything else failure)
it is better to use macros predefined in stdlib.h: `EXIT_SUCCESS` and `EXIT_FAILURE`.

How do we know what header files to include in this program?
{{< expand "Answer" >}} `stdio.h` from `man 3 printf`, `stdlib.h` for status macros 
{{< /expand >}}

Compile the code with `make prog1`, it will deploy standard GNU make compilation template. Run the program.

Why this compilation method is not good enough?
{{< expand "Answer" >}}
Lack of `-Wall` flag, you will not see the code warnings. 
{{< /expand >}}

<em>solution <b>Makefile</b></em>
```makefile
all: prog1
prog1: prog1.c	
	gcc -Wall -fsanitize=address,undefined -o prog1 prog1.c
.PHONY: clean all
clean:
	rm prog1
```

Makefile file structure is described in the tutorial linked above. 
The most essential rules to remember include: tabulators prior to compilation description, 
first target is default (make == make all in above example), phony targets don't depend on physical files. 

Regular (not phony) targets must produce files of the same name as the target name, 
it is a common problem among students to not replace all the names when making copy of the makefile.

How can you remove the old executable with this Makefile?
{{< expand "Answer" >}}
```shell
make clean
```
{{< /expand >}}

How can you compile the code with this Makefile?
{{< expand "Answer" >}}
`make` or `make prog1`
{{< /expand >}}

How can you redirect the output of this program to the file?
{{< expand "Answer" >}}
```shell
./prog1 > file.txt
```
{{< /expand >}}

How to display this file?
{{< expand "Answer" >}}
```shell
cat file.txt
```
{{< /expand >}}

Make the copy of the source file, name it prog1b.c. Modify the Makefile so that it can compile this new source file to the binary named prog1b.
Make sure that the compilation is in deed using -Wall flag (it must be printed on the screen during compilation). 
If you succeed the first time try to deliberately break the Makefile by reverting some name changes (from prog1b back to prog1) in various configurations.

## Task 2 - stdin, stderr

Goal: Extend prog1 to print out the welcome message for the name from the standard input.
Names above 20 chars should generate error (message and immediate exit).
What you need to know:
- man 3p fscanf
- man 3p perror
- man 3p fprintf

For convenience lets add macro:
```c
#define ERR(source) (perror(source),\
		     fprintf(stderr, "%s:%d\n", __FILE__, __LINE__),\
		     exit(EXIT_FAILURE))

```

- Check on `__FILE__` and  `__LINE__` macros, they indicate the source code location in error message, it is C preprocessor feature.
- Usually we do not write semicolon at the end of macros.
- Error messages shall always be printed on stderr.
- On most errors program must terminate with exit function.

<em>code extension for <b>prog2.c</b></em>
```c
char name[22];
scanf("%21s", name);
if (strlen(name) > 20) ERR("Name too long");
printf("Hello %s\n", name);
```

- Modify the Makefile from the previous task to compile this code.
- Run and test this program. 

Why `scanf` reads up to 21 characters (`%21s`)?
{{< expand "Answer" >}}
If you limit it to 20 you will never know if user entered exactly 20 characters, or maybe he tried to enter more than the limit. 
{{< /expand >}}

Why we declare 22 chars sized array while storing at most 21 chars?
{{< expand "Answer" >}} 
In C all strings must end with zero code char.
scanf will add this terminator.
We must provide an array that can accommodate all the chars plus the terminating zero.
{{< /expand >}}

How can you run this program to hide error message from the screen?
{{< expand "Answer" >}} 
You can redirect stderr to `/dev/null`, e.g.: `./prog2 2>/dev/null`
{{< /expand >}}

When you provide too long name message "Name too long: Success" shows up. Why success? Variable errno is not set in case
of errors in our code, ERR is coded to work on errno value, it is meant for system and library functions.

If you provide two or more words as the input e.g. "Anna Maria" the program will use only the first word, that is due to
scanf formatting, you requested one word (%s) only.

The program is not displaying any prompt at the beginning it expects the input right on. This is the UNIX like attitude
to the user interface it is best suited for the scripts or batch processing (see next task). On the other end of the
scale we have very user friendly interactive programs (Windows like way) that will greet the user and instruct him what
to do. You should choose the interaction style based on the program purpose. You can mix the styles in one program if
you add the test weather the program runs in console or in batch mode (f.isatty).

In prog2.c we do not test the return value from scanf. It will not help us to determine if the string was too long or
not because (for %s) in both cases it will return 1.

In this code you can see so-called "magic numbers" (20,21,22). You should avoid such a direct notation because it is
hard to change the limit from 20 to something else without analyzing the code. Magic numbers are considered a bad coding
style. You should define macro (#define MAXL 20) instead and derive other numbers from it (MAXL+1 , MAXL+2).

To receive the input from scanf you must press ENTER, it is the result of terminal input line buffer. Immediate
keystroke reading is a bit complicated, you must get rid of terminal buffer (man 3p tcsetattr), add O_NONBLOCK flag on
the input descriptor and use descriptor instead of a stream. It is a bit easier with dedicated libraries like ncurses,
this is not a part of OPS course.

## Task 3 - stdin cd..

Goal: Extend prog2.c to print welcome message for each name given from the standard input. Program should consume lines
of text (up to 20 chars) and print on the standard output. The operation repeats until the end of stream (EOF)
condition (`C-s`). Lines above 20 chars should be truncated, but no error gets reported.

What you need to know:
- man 3p fgets

`C-d` (Ctrl d in Windows like notation) closes the stream/descriptor as if you execute close on it. When stream is at
the definite end of the data it can provide, it is in EOF (end of file) state. Please notice that it is something else
than temporary lack of data when program awaits "slow" user input from the keyboard. You must understand this
difference!

`C-d` works only after new line, text stream should always end with a new line.

`C-c` sends SIGINT to active process group - usually it gracefully ends the program.

`C-z` suspends the program (SIGSTOP), you can list stopped programs with jobs command and revive the process with command
%N where N is the number of the process on the list.

`C-\` sends SIGQUIT, terminates the program and dumps the core.

`C-s` freezes the terminal, it has no influence on the program until the terminal buffer is not full, once full the
process will wait on stdout output. Windows saving habit (Ctrl S) may get you in this situation pretty often, to
unfreeze the terminal press C-q

Above shortcuts work in bash (and some other shell programs) that we use in the labs.

<em>code for <b>prog3.c</b></em>
{{< includecode "prog3.c" >}}
<em>New makefile <b>Makefile</b> compiles multiple targets:</em>

```makefile
all: prog1 prog2 prog3
prog1: prog1.c
	gcc -Wall -fsanitize=address,undefined -o prog1 prog1.c
prog2: prog2.c
	gcc -Wall -fsanitize=address,undefined -o prog2 prog2.c
prog3: prog3.c
	gcc -Wall -fsanitize=address,undefined -o prog3 prog3.c
.PHONY: clean all
clean:
	rm prog1 prog2 prog3
```

Compile and run the program with above Makefile, how can you compile only one target at a time?
{{< expand "Answer" >}}
```shell
make prog3
```
{{< /expand >}} 

Check how this program works with 20 and 21 chars strings, explain the output.
{{< expand "Answer" >}} 
For 21 chars string array can not accommodate new line char, fgets truncates what can not fit the limit, 
in this case it is only trailing new line. 
{{< /expand >}} 

Why can we observe the welcoming messages in separated lines despite the fact that there is no new line in printf formatting string?
{{< expand "Answer" >}} 
fgets reads strings with trailing new line (if it fits the limit) and there is no need to add the second one, 
printf prints what is in the buffer including new lines.
{{< /expand >}}

Why buffer size id MAX_LINE+2?
{{< expand "Answer" >}}
It should accommodate string+new line+trailing zero marker.
{{< /expand >}}

- Please notice that fgets can work with any stream, not only the stdin.
- This program is free from magic numbers, it should always be like that.

<em>Text file <b>dane.txt</b></em>
```
Alice 
Marry Ann
Juan Luis
Tom
```

Create text file with above content.

How can you run the program in a such away that it will get the input from the file instead of the keyboard (two ways)?
{{< expand "Answer 1" >}} 
redirect file to stdin: `./prog3 < dane.txt` 
{{< /expand >}}
{{< expand "Answer 2" >}} pipeline: `cat dane.txt | ./prog3`
{{< /expand >}} 

## Task 4 - program parameters 1

Goal: Write code to display all the program execution parameters.
What you need to know:
- man 1 xargs

<em>code for <b>prog4.c</b></em>
{{< includecode "prog4.c" >}}

<em>Full <b>Makefile</b></em>

```makefile
CC=gcc
CFLAGS=-Wall -fsanitize=address,undefined
LDFLAGS=-fsanitize=address,undefined
```

To use GNU make compilation template you need to supply executable name as the parameter to the make command, in this
case it will be `make prog4`. Now compilation has required -Wall flag, the template was modified with the flags in
Makefile.

To supply multiple words argument it must be enclosed in apostrophes or all white characters must be escaped with `\`.

Zero argument is always the name of the binary itself!

Compile, run and test this program with various parameters.

How can you invoke `xargs` to have file dane.txt used as the source of parameters?
{{< expand "Answer 1" >}} 
each word as separated argument: `cat dane.txt | xargs ./prog4` 
{{< /expand >}}
{{< expand "Answer 2" >}} 
each line as an argument: `cat dane.txt |tr "\n" "\0"| xargs -0 ./prog4`
{{< /expand >}} 

When using xarg on larger files you must be aware that the command line length can be limited in OS. Xargs can split
data and invoke the command several times if you use appropriate flag.

## Task 5 - program parameters 2

Goal: 
Write a program that accepts 2 arguments: name and counter n &gt; 0, more than two parameters or invalid counter should stop the program. For correct parameters print "Hello NAME" n-times
What you need to know:
- man 3p exit
- man 3p atoi
- man 3p strtol

<em>Additional function in file <b>prog5.c</b></em>
```c
void usage(char *pname)
{
	fprintf(stderr, "USAGE:%s name times>0\n", pname);
	exit(EXIT_FAILURE);
}
```

- Please notice that exit function accepts the same statuses as you return from main.
- Printing usage syntax in case of missing or wrong arguments is a good programming practice.

<em>Code for <b>prog5.c</b> file:</em>
{{< includecode "prog5.c" >}}

Compile this program using universal Makefile from previous task.

How this program works if you supply incorrect value as the parameter, explain why??
{{< expand "Answer" >}}
It prints nothing as atoi in Linux returns 0 if it can not convert the string to the integer.
{{< /expand >}}

Why argc has to be 3, we expect two arguments?
{{< expand "Answer" >}}
`argc` is a counter of elements stored in `argv` which is storing two arguments and a name of the binary (`argv[0]`). in total 3 elements.
{{< /expand >}}

Please notice the reverted notation (0==j), how can it help us? If by mistake you write (0=j) compiler will return an
error, and you will know where the problem is. If you write (j=0) it will compile, and you will have some extra work
searching for the problem.

Older C standards disallowed variable declarations inside the code, but it is much easier to understand the code if
you declare variables at the point you start to use them instead of the block beginning.

What is returned when atoi can't convert? In practice, it is zero, but POSIX says that it is undefined. If you need more
control over the conversion and be able to tell error from real zero use strtol instead.

You can overwrite program arguments and name in the run time! It may be a hiding technique for the process or for the
passwords in arguments.

## Task 6 - program parameters 3

Goal:
Write a program that accepts any number of parameters of type -t x and any number of -n NAME parameters. 
Parameters can be mixed in any order.  Each occurrence of -n results in the printout of NAME x-times.
Initially x=1, each occurrence of -t changes x to the new value,
e.g.: `prog6 -n Anna -t 2 -n John -n Matt  -t 1 -n Danny` will print:
```
Hello Anna
Hello Jonh
Hello Jonh
Hello Matt
Hello Matt
Hello Danny
```

What you need to know:
- man 3p getopt
- GNU documentation on getopt <a href="http://www.gnu.org/software/libc/manual/html_node/Using-Getopt.html">link</a>

<em>Lets correct usage in <b>prog6.c</b>:</em>
```c
fprintf(stderr, "USAGE:%s ([-t x] -n Name) ... \n", pname);
```

<em>code for <b>prog6.c</b> file:</em>
{{< includecode "prog6.c" >}}

Test how this program deals with invalid options, spare positional (not preceded with option letter) parameters, missing
arguments and missing options.

In the code we compare c (int) with chars 't' and 'n' (char), those types can be easily compared as they are both
numeric, char is just a one byte number.

getopt parameter describes the syntax - mostly option letters with optional suffixes - `:` to denote mandatory parameter
and `::` for optional parameter.

Invalid option messages come from getopt function itself, to turn them off you need to zero global variable opterr.

Other global, extern type variables connected to getopt are optarg and optind. The meaning of the first one is obvious
(see the code). The second one denotes how many parameters from the command line has been processed up to now. Positional
parameters not related to the syntax are shifted to the end. Considering those two facts it should be easier to tell
what the last "if" in the code is for - it tests for spare (not defined in syntax) arguments that should be left at the
end of the list after the main loop.

Please notice that it may be a good idea to verify the parameters before processing them.

`optarg` is stored in static buffer, it is overwritten with next call to getopt, if you need to store values from this
buffer make copies (strcpy, memory allocation).

Long options like `--count 10` are recognized by getopt_long, however they are not part of the POSIX standard, it is
GNU extension, and we are not going to use them here.

## Task 7 - environment variables 1

Goal: List all the environmental variables of the process
What you need to know:
- man 3p environ
- man 7 environ

<em>code for <b>prog7.c</b> file:</em>
{{< includecode "prog7.c" >}}

You can add your own variable in the following way: `$ TVAR2="122345" ./prog7` , it will show up on the list but it will
not be stored in the parent environment i.e. next run `./prog7` will not list it.

You can also store it in the shell environment: `export TVAR1='qwert'` and invoke `./prog7` multiple times, it will
keep showing TVAR1 on the list.

If you start another shell from the menu and run this program inside will it list the variable exported in the first one?
{{< expand "Answer" >}}  
No, those two shells inherit variables from the program launcher and there is no "sideways" inheritance possible in UNIX. 
{{< /expand >}}

If I run the second shell from the first one and then run the program in 2nd one will it list the variable?
{{< expand "Answer" >}}
Yes, the second one inherits the environment with this variable from the first one as it is parent child relation of those two processes.
{{< /expand >}}

## Task 8 - environmental variables 2

Goal: 
Enhance prog3.c to multiply each welcome line of text as many times as environmental variable TIMES says. At the end of the program set RESULT environmental variable to "Done" value. 
What you need to know:
- man 3p getenv
- man 3p putenv
- man 3p setenv
- man 3 system (3p) is a bit confusing

<em>code for <b>prog8.c</b> file:</em>
{{< includecode "prog8.c" >}}

Please notice that environmental variable may be absent and that this code is prepared for this situation. Good
programmer always checks for errors. If you are in a hurry and skip those checks your must be aware that your code is
more vulnerable. It is terribly bad if you skip error checks due to laziness or ignorance.

The second check is at putenv, last one at system. As there are so many checks in the code it is reasonable to have
macro to deal with it (like ERR you have already seen).

There is a second function to modify the environment - setenv.

In this program system function is used to check what we already know.
If putenv was successful we do not expect
problems here, but it makes a good example tough.

Function "system" call is the same as running the given command in the shell as child process. The function shall return
the status of the command - not the output but what you normally get by calling "$%" (this time retype dollar as a part
of the command) in the shell.

System can run without shell program. In such a case the system function will not work! You will receive error code 127.

Run the program with different values for variable TIMES

How to check the RESULT value after the execution of the program? Will it be set?
{{< expand "Answer" >}}
`env|grep RESULT` - it will not show on the list. Changes in the environment are not propagated up the tree. 
The variable was set in the program process and, for a brief moment, in the shell executed by system function. 
After those processes have ended the variable vanished with them.
{{< /expand >}}

## Task 9 - error handling

Goal: Modify program prog7.c to add append new environmental variables passed by user and then list all them all.

What you need to know:
- man 3p errno

<em>code for <b>prog9.c</b> file:</em>
{{< includecode "prog9.c" >}}

Compile and run program in two ways:
```shell
$ ./prog9 VAR1 VAL1
``` 
```shell
$ ./prog9 VA=R1 VAL1
``` 

Second run should finish with error.
After reading `setenv` man page we can read, that environmental variable name cannot contain '=' sign.
In this case function return `-1` and sets `errno` to `EINVAL` error code.

It is common behaviour that system functions on error return special value and sets special variable `errno` with error code.
It helps to find out what gone wrong during function call.
When we know what this error code means in case of concrete function we can handle it specially.
In sample code finishes with special error description.
There are also cases when system function returning error shouldn't finish whole program.

In every man page of system function there is **ERRORS** section. 
I encourage you to read those section at all system function from this tutorial.
Consider what error codes shouldn't lead to finishing program.

What would happen, when we run program with only one argument?
{{< expand "Answer" >}}  
Program should finish at the beginning calling `usage` function.
We cannot set environmental variable when we don't know its value.
{{< /expand >}}

Why variables added by code are listed inside last loop?
{{< expand "Answer" >}} 
Modifying environmental variables it's just editing `char **environ` variable.
We can read in `man 3p setenv` that calling it modifies above variable.
{{< /expand >}}

## The task for IDE testing

Goal: Write a trivial program "hello world", compile and run it.

*What student has to know:*
- know one of available (in our labs) programmers environment for Linux
- know how to do basic compilation with gcc
- know how to use a command line interface

*What student should know:*
- fundamental know how about `git` program


*solution **prog1.c**:*
```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    printf("Hello world\n");
    return EXIT_SUCCESS;
}
```

*compilation:*

```shell
gcc -Wall -fsanitize=address,undefined -o prog1 prog1.c
```

Important note: use of `-Wall` compiler flag is compulsory.
Besides that each assignment will require usage of particular set of sanitizers.
[More information about sanitizers during the lab.]({{< ref "../sanitizers" >}})
{{< hint danger >}}
**ATTENTION** you can not freely reorder the switches of the gcc command, the `-o` switch has an argument (output filename).
{{< /hint >}}

*running:*

```shell
./prog1
```

## Instruction of using GIT on the laboratory

{{< hint info >}}

This section of tutorial has to be done during the laboratory on faculty.
Before starting you have to read section about [GIT]({{< ref "../../git" >}}) usage.
Steps below require access to server available only during laboratory.
I encourage you to read it and prepare before zeroth laboratory.

{{< /hint >}}

During laboratory every task will be done inside a GIT repository.
Credentials to remote access will be provided during the laboratory.
Your code has to be tracked by GIT during the laboratory.
Every stage has to be synchronized with server.
**If some code will not be sent to the server, you will not take points for it.** 

Before you start coding, you need your personal SSH key from [LeON](https://leon.pw.edu.pl).
On the OPS page you can download two files: `id_ed25519` and `id_ed25519.pub`.
You have to copy it to `~/.ssh` directory.
Every pair of keys is unique to student and has to be used by only one student.

Because private key is secret it shouldn't be readable to other users. You need to change permissions:
```shell
chmod 600 ~/.ssh/id_ed25519
```

First step on laboratory is copy remote repository to your local workstation with command 
```shell
$ git clone ssh://gitolite@vl01/l0en/name_surname
```

Command creates directory with name of repository and copies files to it.
Inside this directory you write your code.

The task consists of stages.
When you finish one stage, you should commit your change to repository.
To synchronize your code with server you have to run command 

```shell
$ git push
```

Please remember to send your code to the server as soon as possible.
Access to the server will be closed when the laboratory finishes.
Code can be graded if and only if it will be sent to this server.

The solution will be accepted by the server if and only if:
- Only solution files (`.c`) were modified - if you change any other files, e.g., makefile, commit will be rejected
- Solution files are correctly formatted. In the repository, there is a configuration file `.clang-format` for `clang-format` program, which is installed in the system. It allows to format source files - use `clang-format -i <filename.c>`. Many IDEs allow automatic formatting on save
- Solution is not too long - 600 lines by default, it should be more than sufficient for lab task
- Solution can be compiled without any warnings using makefile from the repository

If one of the rules is not met the server will reject the solution with a reject message. In that case, you need to fix your code, create a new commit, and push it. The server allows one push every minute.


## Source codes presented in this tutorial
{{% codeattachments %}}