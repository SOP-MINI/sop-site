---
title: "Using GIT during laboratory"
weight: 30
---


## Instruction of using GIT on the laboratory

During laboratory every task will be done inside a GIT repository.
Your code has to be tracked by GIT during the laboratory.
Every stage has to be synchronized with server.
**If some code will not be sent to the server, you will not take points for it.** 

### Facoulty's GIT server

During the laboratory repositories will be published on <https://sgit.mini.pw.edu.pl>.
[Here](https://sgit.mini.pw.edu.pl/git-tutorial) you can find info about accessing it and git configuration.
Authorisation using SSH keys is more efficient since it allows doing operations on remote repository without typing password every time, so we recommend it to everyone.


### Working with the repository during the lab

For the laboratory a custom git repo with the starting files will be published.
So the first step each time is to clone remote repo to the PC:

```shell
$ git clone ssh://git@192.168.137.60/OPS2_26L/w1_<surname_name> w1
```

This command creates directory with name `w1` and copies files to it.
The last param is the name of the directory - if you omit it, it will default to repo name (in this case `w1_surname_name`).
Inside this directory you write your code.
You don't need to type this address by hand - you can copy it from the server - the repository should be always present at the beginning of the list of all your repositories.

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
- Solution files are correctly formatted. In the repository, there is a configuration file `.clang-format` for `clang-format` program, which is installed in the system. It allows to format source files - use `clang-format -i <filename.c>`. Many IDEs allow automatic formatting on save (see [IDE configuration]({{< ref "info/IDE-configuration" >}}).).
- Solution is not too long - 600 lines by default, it should be more than sufficient for lab task
- Solution can be compiled without any warnings using makefile from the repository

If one of the rules is not met the server will reject the solution with a reject message. 
In that case, you need to read it carefully, fix your code, create a new commit, and push it.
The server allows one push every minute.
