---
title: "Project"
---

The project lasts four weeks, from 25.11 to 22.12. During W3 (23.12), checking and grading will take place in accordance with the details described in task description.

The source code for grading should be send to the server, as during the laboratory - the path to the project is `p1en`. Please note some technical limitations:
- Unfortunately, the server is online only twice a week - on Mondays from 20:15 to 23:00 and on Tuesdays from 14:15 to 17:00. Only then is it possible to push your solution
- The server is only accessible from the faculty network. To push your solution from outside of the network, you can either:
  - copy it to the ssh server (scp to `ssh.mini.pw.edu.pl`) and push it from there as during the lab
  - use ssh tunneling (note - you will also need to change/add `remote` in the repository)

  Detailed instructions are provided below.

The server performs a preliminary check, similar to the laboratory, and in case of an error, it may reject the solution. However, since the project requires writing more code than during a regular lab, the rules are a bit more relaxed. It is acceptable (and recommended) to create and modify new .c and .h files. Only the initial `Makefile` and `.clang-format` cannot be modified. As always, source files should be formatted and the program should compile without any errors or warnings.

[The content of the task can be found here](/files/p1en.pdf)

If you have any questions, please feel free to ask after class or via email.

## SSH tunneling

SSH tunnel allows you to connect to a server that you do not have direct access to via an access server. In our case, the git server (`vl01`) is in the faculty's local network and cannot be accessed from outside. However, we can log in from outside to the department's ssh server (`ssh.mini.pw.edu.pl`), which has access to the local network.

To push your changes to a server from outside of the faculty, follow these steps:

1. Make sure that you have correctly configured ssh keys - as during the labs, they must be downloaded from LeON.

2. Confirm that you can log in to the ssh server - `ssh <login>@ssh.mini.pw.edu.pl` (use faculty credentials, i.e. those for lab computers)

3. Of course, connecting to the `vl01` server is only possible when it is turned on - so these steps below should be performed at the specific times described above.

4. Create a tunnel: 
```
ssh -L 22222:vl01:22 <login>@ssh.mini.pw.edu.pl
```
This command will cause ssh to route all network traffic from port 22222 on your computer through the faculty ssh server to the `vl01` server port 22 - i.e., the ssh port. Do not close this terminal or the ssh program - it is responsible for the redirection. Perform the next steps in a new terminal window.

5. To verify that we have a connection and that the key is configured correctly, we can try to log in to `vl01` using the command `ssh -p 22222 gitolite@localhost`. If everything is correct, we should see a short message confirming successful login. It should also be possible to download the repository using the new address `git clone ssh://gitolite@localhost:22222/p1en/<name_surname>`

6. Now we need to configure git in our repository to connect through the new tunnel. This step is not necessary if you have just downloaded the repository from the new address (using `git clone` as above) and want to use it. In the repository, use the command: 
```
git config url.“ssh://gitolite@localhost:22222”.insteadOf “ssh://gitolite@vl01”
```
At this point, the command
```
git remote -v
```
should return an address beginning with `ssh://gitolite@localhost:22222`, and all git commands such as `fetch`, `push`, or `pull` should work.

## Direct copy to ssh server

This method is not as elegant and convenient as the tunnel described above, but it is very simple:

1. Copy your solution to the ssh server:
```
scp -r <repository directory> <login>@ssh.mini.pw.edu.pl:
```

2. Log in to the ssh server:
```
ssh <login>@ssh.mini.pw.edu.pl
```

3. On the ssh server, enter the repository and `push` as usual.
