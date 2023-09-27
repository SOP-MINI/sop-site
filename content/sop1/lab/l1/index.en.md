---
title: "L1 - Filesystem"
date: 2022-02-07T19:36:16+01:00
weight: 20
---

# Tutorial 1 - Filesystem

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
{{< includecode "prog10.c" >}}

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
{{< includecode "prog11.c" >}}

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
{{< includecode "prog12.c" >}}

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
{{< includecode "prog13.c" >}}

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
