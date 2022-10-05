---
title: "L1 - POSIX program execution environment"
date: 2022-02-07T19:36:16+01:00
weight: 20
---

# Tutorial 1 - POSIX program execution environment

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
- Tutorial on  gcc and make, <a href="{{< resource "tutorial.gcc_make.txt" >}}">link</a>

I'd like to discourage you from making google searches on API functions. Quite often you will find outdated man pages,
the ones related to different OS'es, ones with mistakes and the pages not conforming to the standards like POSIX. It is
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
{{% includecode "prog1.c" %}}

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
{{< expand "Answer" >}} `stdio.h` from  `man 3 printf`, `stdlib.h` for status macros 
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
first target is default (make == make all in above example), phony targets are not depending on physical files. 

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
If you succeed the first time try to deliberately break the Makefile by reverting some of the name changes (from  prog1b back to prog1) in various configurations.

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
If you limit it to 20 you will never know if user entered exactly 20 characters or maybe he tried to enter more than the limit. 
{{< /expand >}}

Why we declare 22 chars sized array while storing at most 21 chars?
{{< expand "Answer" >}} 
In C all strings must end with zero code char, scanf will add this terminator 
and we must provide an array that can accommodate all the chars plus the terminating zero.
{{< /expand >}}

How can you run this program to hide error message from the screen?
{{< expand "Answer" >}} 
You can redirect stderr to `/dev/null`, e.g.: `./prog2 2>/dev/null`
{{< /expand >}}

When you provide too long name message "Name too long: Success" shows up. Why success? Variable errno is not set in case
of errors in our code, ERR is coded to work on errno value, it is meant for system and library functions.

If you provide two or more words as the input eg. "Anna Maria" the program will use only the first word, that is due to
scanf formating, you requested one word (%s) only.

The program is not displaying any prompt at the beginning it expects the input right on. This is the UNIX like attitude
to the user interface it is best suited for the scripts or batch processing (see next task). On the other end of the
scale we have very user friendly interactive programs (Windows like way) that will greet the user and instruct him what
to do. You should choose the interaction style based on the program purpose. You can mix the styles in one program if
you add the test weather the program runs in console or in batch mode (f.isatty).

In prog2.c we do not test the return value from scanf. It will not help us to determine if the string was too long or
not because (for %s) in both cases it will return 1.

In this code you can see so called "magic numbers" (20,21,22). You should avoid such a direct notation because it is
hard to change the limit from 20 to something else without analyzing the code. Magic numbers are considered a bad coding
style. You should define macro (#define MAXL 20) instead and derive other numbers from it (MAXL+1 , MAXL+2).

To receive the input from scanf you must press ENTER, it is the result of terminal input line buffer. Immediate
keystroke reading is a bit complicated, you must get rid of terminal buffer (man 3p tcsetattr), add O_NONBLOCK flag on
the input descriptor and use descriptor instead of a stream. It is a bit easier with dedicated libraries like ncurses,
this is not a part of OPS course.

## Task 3 - stdin cd..

Goal: Extend prog2.c to print welcome message for each name given from the standard input. Program should consume lines
of text (up to 20 chars) and print on the standard output. The operation repeats until the end of stream (EOF)
condition (`C-s`). Lines above 20 chars should be truncated but no error gets reported.

What you need to know:
- man 3p fgets

`C-d` (Ctrl d in Windows like notation) closes the stream/descriptor as if you execute close on it. When stream is at
the definite end of the data it can provide, it is in EOF (end of file) state. Please notice that it is something else
than temporary lack of data when program awaits "slow" user input from the keyboard. You must understand this
difference!

`C-d` works only after new line, text stream should always end with a new line.

`C-c` sends SIGINT to active process group - usually it gracefully ends the program.

`C-z` suspends the program (SIGSTOP), you can list stopped programs with jobs command and revive the proces with command
%N where N is the number of the process on the list.

`C-\` sends SIGQUIT, terminates the program and dumps the core.

`C-s` freezes the terminal, it has no influence on the program until the terminal buffer is not full, once full the
process will wait on stdout output. Windows saving habit (Ctrl S) may get you in this situation pretty often, to
unfreeze the terminal press C-q

Above shortcuts work in bash (and some other shell programs) that we use in the labs.

<em>code for <b>prog3.c</b></em>
{{% includecode "prog3.c" %}}
<em>New makefile <b>Makefile</b> compiles multiple targetrs:</em>

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

Compile and run the program with above Makefie, how can you compile only one target at a time?
{{< expand "Answer" >}}
```shell
make prog3
```
{{< /expand >}} 

Check how this program works with 20 and 21 chars strings, explain the output.
{{< expand "Answer" >}} 
For 21 chars string  array can not accommodate new line char, fgets truncates what can not fit the limit, 
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

<em>text file <b>dane.txt</b></em>
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
{{% includecode "prog4.c" %}}

<em>Full <b>Makefile</b></em>

```makefile
CC=gcc
CFLAGS=-Wall -fsanitize=address,undefined
LDFLAGS=-fsanitize=address,undefined
```

To use GNU make compilation template you need to supply executable name as the parameter to the make command, in this
case it will be  `make prog4`. Now compilation has required -Wall flag, the template was modified with the flags in
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
{{% includecode "prog5.c" %}}

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
error and you will know where the problem is. If you write (j=0) it will compile and you will have some extra work
searching for the problem.

Older C standards disallowed variable declarations inside of the code, but it is much easier to understand the code if
you declare variables at the point you start to use them instead of the block beginning.

What is returned when atoi can't convert? In practice it is zero but POSIX says that it is undefined. If you need more
control over the conversion and be abel to tell error from real zero use strtol instead.

You can overwrite program arguments and name in the run time! It may be a hiding technique for the process or for the
passwords in arguments.

## Task 6 - program parameters 3

Goal:
Write a program that accepts any number of parameters of type -t x and any number of -n NAME parameters. 
Parameters can be mixed in any order.  Each occurrence of -n results in the printout of NAME x-times.
Initially x=1, each occurrence of -t changes x to the new value,
e.g.: `prog6 -n Anna -t 2 -n John -n Matt  -t 1 -n Danny`  will print:
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
- GNU  documentation on getopt <a href="http://www.gnu.org/software/libc/manual/html_node/Using-Getopt.html">link</a>

<em>lets correct usage in <b>prog6.c</b>:</em>
```c
fprintf(stderr, "USAGE:%s ([-t x] -n Name) ... \n", pname);
```

<em>code for <b>prog6.c</b> file:</em>
{{% includecode "prog6.c" %}}

Test how this program deals with invalid options, spare positional (not preceded with option letter) parameters, missing
arguments and missing options.

In the code we compare c (int) with chars 't' and 'n' (char), those types can be easily compared as they are both
numeric, char is just a one byte number.

Getopt parameter describes the syntax - mostly option letters with optional suffixes - `:` to denote mandatory parameter
and `::` for optional parameter.

Invalid option messages come from getopt function itself, to turn them off you need to zero global variable opterr.

Other global, extern type variables connected to getopt are optarg and optind. The meaning of the first one is obvious (
see the code). The second one denotes how many parameters from the command line has been processed up to now. Positional
parameters not related to the syntax are shifted to the end. Considering those two facts it should be easier to tell
what the last "if" in the code is for - it tests for spare (not defined in syntax) arguments that should be left at the
end of the list after the main loop.

Please notice that it may be a good idea to verify the parameters before processing them.

`optarg` is stored in static buffer, it is overwritten with next call to getopt, if you need to store values from this
buffer make copies (strcpy, memory allocation).

Long options like `--count 10`  are recognized by getopt_long, however they are not part of the POSIX standard, it is
GNU extension and we are not going to use them here.

## Task 7 - environment variables 1

Goal: List all the environmental variables of the process
What you need to know:
- man 3p environ
- man 7 environ

<em>code for <b>prog7.c</b> file:</em>
{{% includecode "prog7.c" %}}

You can add your own variable in the following way: `$ TVAR2="122345" ./prog7` , it will show up on the list but it will
not be stored in the parent environment i.e. next run `./prog7` will not list it.

You can also store it in the shell environment: `export TVAR1='qwert'` and invoke `./prog7` multiple times, it will
keep showing TVAR1 on the list.

If you start another shell from the menu and run this program inside will it list the variable exported in the first one?
{{< expand "Answer" >}}  
No, those two shells inherit variables form the program launcher and there is no "sideways" inheritance possible in UNIX. 
{{< /expand >}}

If I run the second shell from the first one and then run the program in 2nd one will it list the variable?
{{< expand "Answer" >}}
Yes, the second one inherits the environment with this variable from the first one as it is parent child relation of those two processes.
{{< /expand >}}

## Task 8 - environmental variables  2

Goal: 
Enhance prog3.c to multiply each welcome line of text as many times as environmental variable TIMES says. At the end of the program set RESULT environmental variable to "Done" value. 
What you need to know:
- man 3p getenv
- man 3p putenv
- man 3p setenv
- man 3 system (3p) is a bit confusing

<em>code for <b>prog8.c</b> file:</em>
{{% includecode "prog8.c" %}}

Please notice that environmental variable may be absent and that this code is prepared for this situation. Good
programmer always checks for errors. If you are in a harry and skip those checks your must be aware that your code is
more vulnerable. It is really bad If you skip error checks due to laziness or ignorance.

The second check is at putenv, last one at system. As there are so many checks in the code it is reasonable to have
macro to deal with it (like ERR you have already seen).

There is a second function to modify the environment - setenv.

In this program system function is used to check what we already know, if putenv was successful we do not expect
problems here, but it makes a good example tough.

Function "system" call is the same as running the given command in the shell as child process. The function shall return
the status of the command - not the output but what you normally get by calling "$%" (this time retype dollar as a part
of the command) in the shell.

System can run without shell program. In such a case the system function will not work! You will receive error code 127.

Run the program with different values for variable TIMES

How to check the RESULT value after the execution of the program? Will it be set?
{{< expand "Answer" >}}
`env|grep RESULT` - it will not show on the list however. Changes in the environment are not propagated up the tree. 
The variable was set in the program process and , for a brief moment, in the shell executed by system function. 
After those processes have ended the variable vanished with them.
{{< /expand >}}

## Task 9 - directories 1

Goal: 
Write a program counting objects (files, links, folders and others) in current working directory
What you need to know:
- man 3p fdopendir (only opendir)
- man 3p closedir
- man 3p readdir
- man 0p dirent.h
- man 3p fstatat (only stat and lstat)
- man 3p errno
- man 2 lstat (object type testing macros)

<em>function in <b>prog9.c</b> file:</em>

```c
void scan_dir()
{
	DIR *dirp;
	struct dirent *dp;
	struct stat filestat;
	int dirs = 0, files = 0, links = 0, other = 0;
	if (NULL == (dirp = opendir(".")))
		ERR("opendir");
	do {
		errno = 0;
		if ((dp = readdir(dirp)) != NULL) {
			if (lstat(dp->d_name, &filestat))
				ERR("lstat");
			if (S_ISDIR(filestat.st_mode))
				dirs++;
			else if (S_ISREG(filestat.st_mode))
				files++;
			else if (S_ISLNK(filestat.st_mode))
				links++;
			else
				other++;
		}
	} while (dp != NULL);

	if (errno != 0)
		ERR("readdir");
	if (closedir(dirp))
		ERR("closedir");
	printf("Files: %d, Dirs: %d, Links: %d, Other: %d\n", files, dirs, links, other);
}
```

Run this program in the folder with some files but without sub-folders, it may be the folder you are working on this tutorial in. Is the folder count zero? Explain it.
{{< expand "Answer" >}}
No, each folder has two special hard linked folders - "." link to the folder itself and ".." the link to the parent folder, thus program counted 2 folders.  
{{< /expand >}}

How to create symbolic link for the tests?
{{< expand "Answer" >}}
`ln -s prog9.c prog_link.c`
{{< /expand >}}

What is the difference between stat and lstat, if we replace lstat for stat in the code will it change the results?
{{< expand "Answer" >}} 
It will always count zero links because stat trawers the link and always test the real object instead.
{{< /expand >}}

What members are defined in dirent structure according to the POSIX?
{{< expand "Answer" >}}
file (object) name and its inode number. The rest of the properties can be read with stat/lstat 
{{< /expand >}}

What members are defined in dirent structure according to the LINUX (man readdir) ? 
{{< expand "Answer" >}}
Name, inode and 3 other not covered by the standard.
{{< /expand >}}

Where the Linux/GNU documentation deviates from the standard always follow the standard it will result in better
portability of your code.

Error checks usually follow the same schema if(fun()) ERR(); the ERR macro was declared and discussed before. You should
check for errors in all the functions that can cause problems, both system and library ones. In practice nearly all
those function should be checked for errors. I will keep repeating this as lack of those checks is mayor coders
weakness.

Most errors your code can encounter will result in program termination, some exceptions will be discussed in the
following tutorials.

Pay attention to the use of "." folder in the code, you do not need to know your current working folder, it's simpler
this way.

You must know about specific readdir error reporting. It is the same value as for the end of folder scanning, you can't
tell the difference in both cases it's NULL! How can we disambiguate this NULL?

You can use errno to check for real errors, to do it right you must zero errno before EACH call to readdir and check its
value before any other system and library function is called as they may reset the errno value. Please notice that errno
is reset inside the loop not before it. Also notice that in case of NULL the program flows to the comparison of errno
through simple conditions only (no function calls).

Why do we need to zero errno in first place? Readdir could do it for us, right? The clou is that the POSIX says that
system function could zero errno on the the success but it is not obliged to do it.

If you want to make assignments inside logical expressions in C you should add parenthesis, its value will be equal to
the value assigned, see readdir call above.

Good programmers always release resources, the open folder is a kind of resource. One open folder uses one file
descriptor in Linux, process can have limit on open descriptors. This gives you two good reasons not to forget closedir,
code checking teacher may be counted as third one :-).

## Task 10 - directories 2

Goal:
Use function form prog9.c to write a program that will count objects in all the folders passed to the program as positional parameters.

What you need to know:
- man 3p getcwd
- man 3p chdir

<em>code for <b>prog10.c</b> file:</em>
{{% includecode "prog10.c" %}}

How this program deals with non existing folders, no access folders, relative paths and absolute paths as parameters?

Why this program stores the initial working folder?
{{< expand "Answer" >}}
Paths given as parameters may be relative to the start working folder.
The program changes location in the tree of folders to "be" in folder pointed by the parameter but this automatically
invalidates all other relative paths (other parameters) to make other relative paths valid again program must go back to
initial position in the three. 
{{< /expand >}}

Is it true that program should terminate in the same working folder it was started with?
{{< expand "Answer" >}} No, the current working folder (CWD) is individual for each process (is a part of the
environment), any change in child process has no effect on the parent, there is no reason to go back just before
terminating the program.
{{< /expand >}}

Not all errors encountered in this program has to terminate it, what error can be handled in better way, how?
{{< expand "Answer" >}} 
`if(chdir(argv[i])) continue;` simplified solution, it would be nice to add some message to it.
{{< /expand >}}

Never ever code in this way: `printf(argv[i])`. What will be printed if somebody puts %d or other printf placeholders in
the arguments? This applies to any string not only the one from arguments.

## Task 11 - directories 3

Goal: 
Write a program that counts all occurrences of the files, folders, symbolic links and other objects in a sub-trees rooted at locations indicated by parameters.  
What you need to know:
- man 3p ftw
- man 3p nftw

<em>code for <b>prog11.c</b> file:</em>
{{% includecode "prog11.c" %}}

- Make sure you know how to declare and use pointers to functions in C
- Test how this program reacts on not available or not existing folders.

Why `FTW_PHYS` flag is applied?
{{< expand "Answer" >}}
Without it nftw trawers the links to real objects, so it can not count them, similar to stat and lstat functions.
{{< /expand >}}

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

## Task 12 - file access and operations 

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
{{% includecode "prog12.c" %}}

What bitmap is created with: `~perms&0777` ? 
{{< expand "Answer" >}} 
Reverted permission bits cut to 9 least significant bits. If you do not know how it works then learn C bit operations.
{{< /expand >}}

How char randomization works in this program? 
{{< expand "Answer" >}}
Program iterates through all the chars in sequence: A,B,C ... Z and then A again, For each char random position in the file is chosen. 
Expression  'A'+(i%('Z'-'A'+1)) should have no secrets for you, please make sure you know how it works.
Such a construction will return many times during our labs. 
{{< /expand >}}

Run the program few times and check the sizes of the files created ("$ ls -l"), are they always equal to the expected ones?
How can you explain the difference for small files sizes and how for big (64K+) ones?
{{< expand "Answer" >}}
Most of the time you will observe smaller files than expected. In both size ranges the problem has the same root 
file creation method. First of all the file is initially empty, we place chars at random positions, 
quite often not the last position in the file, and that's why smaller files are somehow smaller.
Big size range is also affected by the limit on the value of random number you can obtain from rand function. 
It can be at most RAND_MAX (the actual value can vary, expect 2 bytes number in Linux).
The highest position occupied by non zero character will not be larger than RAND_MAX 
thus the output file can not be larger than RAND_MAX. 
{{< /expand >}}

Modify the program to always produce files of expected size.

Why one case of unlink error is ignored?
{{< expand "Answer" >}} 
ENOENT means no file to remove, in this case it can't be considered a problem, there is nothing to delete. 
File removal is necessary only if the file exists.
{{< /expand >}}

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

## Task 13 – stdout buffering

Note that this topic is less about operating systems and more about general C programming, 
however we mention it for completeness as related issues were quite common in the past years.

<em>code for <b>prog13.c</b> file:</em>
{{% includecode "prog13.c" %}}

Try running the above (very simple!) code from the terminal. What do you see? 
{{< expand "Answer" >}} As expected, you just see one number each second, no funky business… yet.
{{< /expand >}}

Try running the above code again, but this time redirect it to the file: `./executable > file`. Afterwards,
try opening the file while the program is running, and then `Ctrl+C` the program and reopen the file again. What do you
see?
{{< expand "Answer" >}}
If you do this quickly enough, the file will be empty! That is because the standard library
detects that the output does not go to the terminal, and buffers it for performance, which means that the actual write
operation happens only when enough data accumulate. This means that the data is not available immediately, and can even
be lost in case the program ends abnormally (for instance when we use `Ctrl+C`). Of course, if you let the program run
until completion, it will output all the data in the end (try it out!). While this is configurable, you will not need to
do so as you will see in a second.
{{< /expand >}}

Try running the above code again, with output going to the terminal (like in the first step), but remove the newline
from the `printf` function argument: `printf("%d", i);`. What do you see?
{{< expand "Answer" >}}
Surprisingly, even though the output goes directly to the terminal, the same thing as in the second step happens and we
don't see any output. That is because the buffering happens even in this case; only now a newline tells the standard
library to flush all the data. This is why you didn't see anything weird happening in the first step. This is also why
you might find that your `printf` is not outputting anything; if you forget the newline at the end of the
string, the standard library will not do anything until there actually is a newline or the program ends correctly.
{{< /expand >}}

Retry the first three steps, but output to the standard error instead: 
`fprintf(stderr, /* parameters you passed to printf */);`. 
What happens now? Note that in order to redirect standard error to file, you need `>2` instead of `>`. 
{{< expand "Answer" >}}
Now there is no buffering: in all three cases you really see a number each second.
Indeed, the standard library will not buffer standard error, as this stream is meant for debugging. 
{{< /expand >}}

You might find yourself debugging your code using `printf(...)`, by adding calls to this function and
checking the values of parameters and/or seeing if those places in code are reached. When you do so, you should rather
use `fprintf(stderr, ...)` in order to output to standard error. Otherwise, as you have seen, the output data
is buffered by standard library and in some cases might never be output despite the call succeding. In general, if in
doubt, prefer standard error.

When writing actual console applications we tend to use standard output only to output actual results, and standard
error for everything else. For instance `grep` will output lines that contain the matched string to standard
output, but if it can't open the file, it will complain on the standard error. Note that our `ERR` macro also
outputs to standard error.

As an exercise do <a href="{{< ref "../l1-example" >}}">this</a> task. 
It was used in previous years in a bit different labs timing. 
It is 60 minutes task and if you can do it in this time it means you are prepared for the lab. 
In a new timing there is more time for the task and it will be slightly larger.

## Source codes presented in this tutorial

{{% codeattachments %}}
