---
title: "Introduction Lab for OPS1"
date: 2022-02-07T19:29:05+01:00
draft: false
---

<div class="dlaN">

This laboratory does not require any preparation , it's aim is to explain all the rules and answer all the question about graded labs and the classes schedule. Please carefully read everything about [syllabus]({{< ref "../../syllabus" >}}), [grading]({{< ref "../../zasady" >}}) and [schedule]({{< ref "../../harmonogram" >}}). Also have a look at the [reference]({{< ref "../../materialy" >}}).

On [reference]({{< ref "../../materialy" >}}) page you can find self preparation tutorials, you should do them at home before the graded labs. During the graded lab you will have the chance to ask questions about material you studied (usually 15-20 minutes at the beginning of the lab is devoted for this), but we assume you are already familiar with it. The aim of graded lab is to test how well you studied the topic at home. You can ask questions about the course topics via email at any time.

Let's do a simple example to familiarize you with routines required during the graded labs.

</div>

-----

### The task.

*Goal:* Write a trivial program "hello world", compile it, run it , compress the source and copy to the required destination . *What student has to know:*

  - know one of available (in our labs) programmers environment for Linux
  - know how to do basic compilation with gcc
  - know how to use a command line interface
  - know how to use tar compressing command
  - know how to copy files with cp command

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

    $ gcc -Wall -fsanitize=address,undefined -o prog1 prog1.c

Important note: use of -Wall compiler flag is compulsory. Besides that each assignment will require usage of particular set of sanitizers. [More information about sanitizers during the lab.]({{< ref "../sanitizers" >}})

*running:*

    $ ./prog1

*compressing (with check):*

    $ tar -cjf nowakj.tar.bz2 prog1.c
    $ tar -tjf nowakj.tar.bz2

Substitute nowakj with your MiNI account name.

<div class="dlaN">

  - You can type multiple files and folders instead of one file.

  - **ATTENTION** you can not freely reorder the switches of the tar command, the f switch has an argument (the archive).

  - **ATTENTION** you can not freely reorder the tar arguments, eg.:
    
    ~~`$ tar -cjf prog1.c nowakj.tar.bz2`~~
    
    will erase your source file \!\!\!

</div>

*work upload (with check):*

    $ cp nowakj.tar.bz2 {{< teacherdir >}}
    $ ls -l {{< teacherdir >}}nowakj.tar.bz2

Substitute nowakj with your MiNI account name.

<div class="dlaN">

The destination folder will sometimes change, it will always be stated in your task.

</div>
