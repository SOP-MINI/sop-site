---
title: "Using GIT during laboratory"
weight: 30
---


## Instruction of using GIT on the laboratory

{{< hint info >}}

This section of tutorial has to be done during the laboratory on faculty (W1).
Before starting you have to read section about [GIT]({{< ref "/info/git" >}}) usage.
Steps below require access to server available only during laboratory.
I encourage you to read it and prepare before zeroth laboratory.

{{< /hint >}}

### ssh configuration

During laboratory every task will be done inside a GIT repository.
Credentials to remote access will be provided during the laboratory.
Your code has to be tracked by GIT during the laboratory.
Every stage has to be synchronized with server.
**If some code will not be sent to the server, you will not take points for it.** 

Before you start coding, you need your personal SSH key from [LeON](https://leon.pw.edu.pl).
On the OPS page you can download two files: `id_ed25519` and `id_ed25519.pub`.
You have to copy it to `~/.ssh` directory.
Every pair of keys is unique to student and has to be used by only one student.

Because the private key is secret, it shouldn't be readable to other users. You need to change permissions:
```shell
chmod 600 ~/.ssh/id_ed25519
```

#### Managing multiple ssh keys

It is possible that in the `~/.ssh` directory there are arleady some keys (e.g. for another course). In such case the best solution is to tell `ssh` which one use for which host. Change the name of downloaded keys, e.g. to `id_ed25519_ops` and `id_ed25519_ops.pub` and place in `~/.ssh` directory (remember about correct permission, `chmod 600`). Now, create a new file `~/.ssh/config` with content:

```
IdentityFile ~/.ssh/id_ed25519
Host vl01
  IdentityFile ~/.ssh/id_ed25519_ops
```

Configuration file like this tells `ssh` to first look at default key `id_ed25519` (like before) but when we are connecting to host `vl01` (this is server we use during laboratories) to use key `id_ed25519_ops`. You can use this method to add more keys to other servers.

### Working with the repository

First step on laboratory is copy remote repository to your local workstation with command 
```shell
$ git clone ssh://gitolite@vl01/w1en/name_surname
```

Command creates directory with name of repository (`name_surname`) and copies files to it.
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
