---
title: "Example task 2 on POSIX program environment"
bookHidden: true
---

## Description

In this task, you will write a program that helps you scan a directory
by scanning the files in it. Your program should receive a list of
directories as its arguments and those directories will be scanned. As a
result we want to get the list of all the files with its size which are
in scanned directories.


Flag `-p` is used to provide each directory path. Flag `-d` means depth
of scanning as the scanning process is limited to some depth of
subdirectory search. Depth equal to 1 will only scan the current
directory, depth equal 2 will scan the current directory and its
immediate children and so on. During the scanning you should skip symlinks.
Flag `-e` is optional and when provided, the program should skip during
scanning all the files with the extension other than given with this
flag. If the `-o` option is provided and the environment variable
`L1_OUTPUTFILE` exists and is not empty, your program should output the
information about scanned files to the file named in this environment
variable; otherwise, your program should output this information to the
standard output. If the file already exists it should be overwritten.


## Example:

```
$ ./prog -p ./dir1 -p ./dir2 -d 2 -e txt -o
path: ./dir1
file1.txt 15
file2.txt 3315
path: ./dir2
file3.txt 23
path: ./dir2/dir3
file4.txt 44
file5.txt 12342
```

## Stages

- The program lists the names of the files of each provided directory on stdout, not skipping anything yet and ignoring other arguments. Run with `-d 1` flag
- Program lists files with their sizes and option `-e` works
- Option `-d` works
- Option `-o` works
