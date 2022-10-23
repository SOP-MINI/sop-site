---
title: "L0 - Introduction"
date: 2022-02-07T19:29:05+01:00
weight: 10
---

# Introduction Lab for OPS1

{{< hint info >}}

This laboratory does not require any preparation, it's aim is to explain all the rules and answer all the question about graded labs and the classes schedule. Please carefully read everything about [syllabus]({{< ref "../../syllabus" >}}), [grading]({{< ref "../../zasady" >}}) and [schedule]({{< ref "../../harmonogram" >}}). Also have a look at the [reference]({{< ref "../../materialy" >}}).

On [reference]({{< ref "../../materialy" >}}) page you can find self preparation tutorials, you should do them at home before the graded labs. During the graded lab you will have the chance to ask questions about material you studied (usually 15-20 minutes at the beginning of the lab is devoted for this), but we assume you are already familiar with it. The aim of graded lab is to test how well you studied the topic at home. You can ask questions about the course topics via email at any time.

Let's do a simple example to familiarize you with routines required during the graded labs.

{{< /hint >}}

### The task

Goal: Write a trivial program "hello world", compile it, run it, compress the source and copy to the required
destination. *What student has to know:*

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

*compressing (with check):*

```shell
tar -cjf $USER.tar.bz2 prog1.c
tar -tjf $USER.tar.bz2
```

{{< hint info >}}
You can type multiple files and folders instead of one file. For example:
```shell
tar -cjf $USER.tar.bz2 file1.c file2.c
```
will create an archive consisting of two files, `file1.c` and `file2.c`. Of course, it is also ok to archive a whole directory.
{{< /hint >}}

{{< hint danger >}}
**ATTENTION** you can not freely reorder the switches of the tar command, the `-f` switch has an argument (the archive).
{{< /hint >}}

{{< hint danger >}}
**ATTENTION** you can not freely reorder the tar arguments, e.g.:

~~`$ tar -cjf prog1.c $USER.tar.bz2`~~

will erase your source file\!\!\! Remember that the archive name should be right next to the `-f` switch.
{{< /hint >}}

*work upload (with check):*

```shell
cp $USER.tar.bz2 {{< teacherdir >}}
ls -l {{< teacherdir >}}$USER.tar.bz2
```

{{< hint info >}}
The destination folder will sometimes change, it will always be stated in your task.
{{< /hint >}}

### Environment preparation

Unlike the previous classes, we do not require any particular IDE. However, a good editor should:

- show live compilation errors (which allows us to learn about them and find relevant code quicker),
- autocomplete function names (helpful while entering longer names).

A good choice would be Visual Studio Code (not to be confused with Visual Studio), which is commonly chosen due to its beginner-friendliness. Another common choices are Emacs and Vim (as well as Neovim which is sadly not available in the lab by default), but those have a higher barrier to entry. All mentioned editors require additional configuration, which you should do before the first graded labs. Of course it would be a good idea to have a similar setup at home and during labs.

We suggest you shouldn't use larger IDEs, such as ~~CLion~~ if you do not know them well; otherwise their functionalities will hurt you more than help. Also do not use in-browser editors such as ~~<https://www.onlinegdb.com>~~; otherwise in case of PC instability you might lose your code (as it is only stored in browser) and need to begin from scratch.

### Before first graded labs

You should:

- read [syllabus]({{< ref "../../syllabus" >}}), [grading]({{< ref "../../zasady" >}}) and [schedule]({{< ref "../../harmonogram" >}}),
- prepare the editor to your liking (or be able to do it quickly at the start of labs),
- read [the relevant tutorial]({{< ref "../l1/index.en.html" >}}),
- complete [the example task]({{< ref "../l1-example.en.md" >}}),
- practice the operations you did on lab zero,
- read the relevant `man` pages and other entry test material.
