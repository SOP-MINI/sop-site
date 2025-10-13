---
title: "GIT version control system"
weight: 20
---

{{< hint info >}}

This page contains general information about GIT version control system.
After reading it see also [Using GIT during laboratory]({{< ref "sop1/lab/l0#instruction-of-using-git-on-the-laboratory" >}}).

{{< /hint >}}

## GIT - example of distributed version control system

Version control system (VCS) should help programmers (but not only) to track changes of files and help them to cooperate.
On our laboratory it helps to organize code sent by students.
This description shows fundamentals of using GIT version control system.

## GIT fundamentals - tracking file

Let's image that you received code made by other programmer.
Your task is to repair some bug inside.
Without VCS, you should make copy of received code and modify this copy.
When change is ready and tested you should replace original code with your modified copy.
This way you can roll back to old code if your change is wrong. 

When you are using VCS you already have this protection and even more.
When some file is tracked by GIT, you can roll back this file at every moment.
Additionally, you can see exact changes in form of diff file.
When your change is ready you can *commit* it to GIT.
Thanks to diff files and *patch* functionalities we can change **version** of every committed code with one command.
All our changes are held in history.

## GIT fundamentals - tracking changes

GIT basically holds every change in form of diff file.
Every committed change is represented by one differential file.
GIT remembers order of those differences and can get every state of file.
This mechanics gives us history of all changes and ability to select any version of file at any moment. 

## Distributed version control system

When you want to work with some other programmers you need some mechanism to share your work with others.
Generally there is some server which holds current application code.
With GIT every programmer has to make his/her local copy of **all** project files and history of changes.
Such set of files is named *repository* and server is called *remote*.

As a programmer you can work on your local copy without active internet connection.
Only when you want to share your code with other you need to synchronize with remote.

## How to use GIT?

After theoretical introduction let's do some exercise.
Let's create local repository with some sample code.
Then we will make changes to it.

Firstly you have to check if you have git already installed.

```shell
$ git
``` 

Let's create directory where you will create local GIT repository.
Change directory to newly created folder, then run command

```shell
$ git init
``` 

Above command creates directory with name `.git`.
There are all data used by GIT to track changes.
Let's create file `hello.c`, which will contain sample code from zeroth laboratory.

**hello.c**:
```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    printf("Hello world\n");
    return EXIT_SUCCESS;
}
```

This is file you want to track.
GIT doesn't track every file inside repository's directory by default.
Let's compile minimal sample with command

```shell
$ gcc -o hello hello.c
``` 

Now you have two files inside your directory.
Because file `hello` can be generated from `hello.c` at every moment, you don't want to track it.
There are some good reasons:
- Remember, every programmer has to make copy of whole repository before starting to work with it. Because of this minimalizing tracked files is a good practice.
- GIT uses functionalities of `diff` and `patch` programs to track changes. These programs work nicely with text files only. Tracking binary files means making a copy and swapping this copy between versions.

Now let's check how your repository looks like with command
```shell
$ git status
On branch master

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
	hello
	hello.c

nothing added to commit but untracked files present (use "git add" to track)
``` 
You can see, that two of your files are untracked.
Let's add `hello.c` to tracked files with command.

```shell
$ git add hello.c
```

This command makes two things: adds file `hello.c` to tracked files and adds same file to *stage*.
After changing status you can see, that under tracked files section is newly added file.
Let's save status of this file with command

```shell
$ git commit -m "Add hello.c to repository"
```

Above command saves state of files from stage to repository.
Now GIT remembers file at its history.
Parameter `-m` adds message to your commit.
To see list of all commits (changes) you have to run command

```shell
$ git log
```

## Pushing changes to remote server

As mentioned at the beginning, GIT is used to facilitate collaboration between developers. Therefore, the repository is rarely local only - an important feature is the ability to "push" your changes so that others can see them. This can be done using various methods and protocols, but recently, so-called "source forges" have become the most popular. These are servers that host and manage GIT repositories using a convenient web interface. Popular sites include [GitHub](https://github.com/), [GitLab](https://gitlab.com/), and [CodeBerg](https://codeberg.org/), but there are [many more](https://en.wikipedia.org/wiki/Comparison_of_source-code-hosting_facilities)

In this course we use our own system, but it is only activated during classes. It is still worth practicing pushing changes using one of the publicly available and free websites. To do this, first create an account (or log in if you already have one) on the website of your choice and create a new repository. The details vary between different software, but most often an empty repository is created and the website displays instructions on how to connect our local repository on the computer to a remote one. An example of what an empty remote repository looks like:

![](/img/empty-repo.png)

As you can see, this website provides two sets of instructions. The first one is about creating a new repository and largely covers what we have just practiced. The second set of instructions is for connecting an existing repository. A new element in both is setting the remote repository address with the `git remote` command and pushing your changes (commits) with the `git push` command. What should suffice is to use:

```shell
$ git remote add origin <adres zdalnego repozytorium>
```
which add to the repository a new "remote" named `origin` (the default name for "main" remote) with given address. Then you only need to use:

```shell
$ git push
```
to push your commits from current branch to default remote.

## Additional materials

Description above covers only fundamental functions of GIT.
It doesn't cover branching, merging and resolving conflicts.
Those problems are encountered in practice, but they are to complex to cover them here.
This [book](https://git-scm.com/book/en/v2) is fantastic source to learn more functions of GIT.
I recommend reading chapter 2 as supplement to this description.

{{< hint info >}}

This page contains general information about GIT version control system.
After reading it see also [Using GIT during laboratory]({{< ref "sop1/lab/l0#instruction-of-using-git-on-the-laboratory" >}}).
{{< /hint >}}
