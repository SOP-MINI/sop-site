---
title: "L1 - Filesystem 1"
date: 2022-02-07T19:36:16+01:00
weight: 20
---

# Tutorial 1 - Filesystem

{{< hint info >}}
This tutorial contains the explanations for the used functions and their parameters.
It is still only a surface-level tutorial and it is **vital that you read the man pages** to familiarize yourself and understand all of the details.
{{< /hint >}}


## Browsing a directory

Browsing a directory makes us possible to know names and attributes of the files which that directory contains.
This task is accomplished e.g. by the terminal's command `ls -l`. 
However, in order to access this information from the C language, it is needed
to 'open' the directory using `opendir` function, and then read the subsequent records with `readdir` function.
The aforementioned functions are present in `<dirent.h>` header (`man 3p fdopendir`). Let us look at the definitions of these functions:

```
DIR *opendir(const char *dirname);
struct dirent *readdir(DIR *dirp);
```

As we can see, `opendir` returns a pointer to the object of `DIR` type, which we will use to read the directory content. The function `readdir` returns a pointer to the `dirent` structure, which contains (according to POSIX) the following fields (`man 0p dirent.h`):

```
ino_t  d_ino       -> file identifier (inode number, more: man 7 inode)
char   d_name[]    -> filename
```

The remaining file information can be read using `stat` or `lstat` functions from the `<sys/stat.h>` header (`man 3p fstatat`). 
Their definitions are as follows:

```
int  stat(const char *restrict path, struct stat *restrict buf);
int lstat(const char *restrict path, struct stat *restrict buf);
```
- `path` is here the path to the file,
- `buf` is the pointer to (already allocated) `stat` structure (not to be confused with the function name!), which contains the file information. 

In the manuals, the keyword `restrict` is often present in the definitions of function arguments. This is the declaration stating, that the given argument
has to be a block of memory separate from other arguments. 
In this case, passing the same block of memory (e.g. the same pointer) to more than one argument is a serious error and may cause a SEGFAULT or program malfunction.

The only difference between `stat` and `lstat` is the link handling. `stat` returns information about the file pointed by a link, while `lstat` return information of the link itself.

The `stat` structure contains, among others, information about the file size, owner, and last modification date. There are also some macros available, which can be used to check the file type. The important examples of that macros are as follows:
- Macros accepting `buf->st_mode` (field of type `mode_t`):
   - `S_ISREG(m)` -- checking if this is a regular file,
   - `S_ISDIR(m)` -- checking if this is a directory,
   - `S_ISLNK(m)` -- checking if this is a link.
- Macros of type `S_TYPE*(buf)` accepting the `buf` pointer, for identifying file types such as semaphores and shared memory (more in the next semester). 
The details can ba found in `man sys_stat.h`. It is worth familiarising yourself with all the attributes of the `stat` structure and macros, there is quite a lot of them.

After browsing the directory, one should (as good programmer and wanting to pass the course) remember to release resources using the `closedir` function.

### Technical information

In order to browse the entire directory, the `readdir` function should be called repeatedly until it returns `NULL`.
If an error occurs, both `opendir` and `readdir` return `NULL`. An important conclusion follows from this for the `readdir` function: before calling it, the `errno` variable should be set to `0`, 
and if `readdir` returns `NULL`, one should check that this variable has not been set to a non-zero value (indicating an error).
`errno` is a global variable used by system functions to indicate the code of an error encountered.
The `stat`, `lstat` and `closedir` functions return `0` if successful, any other value indicates an error.

### Exercise

Write a program counting objects (files, links, folders and others) in current working directory.

### Solution

New man pages:
```
man 3p fdopendir (only opendir)
man 3p closedir
man 3p readdir
man 0p dirent.h
man 3p fstatat (only stat and lstat)
man sys_stat.h
man 7 inode (first half of the "The file type and mode" section)
```

solution `l1-1.c`:
{{< includecode "l1-1.c" >}}

### Notes and questions 

- Run this program in the folder with some files but without sub-folders, it may be the folder you are working on this tutorial in. Is the folder count zero? Explain it.
{{< answer >}}
No, each folder has two special *hard-linked* folders -- `.` link to the folder itself and `..` the link to the parent folder, thus program counted 2 folders.  
{{< /answer >}}

- How to create a symbolic link for the tests?
{{< answer >}}
```shell
ln -s prog9.c prog_link.c
```
{{< /answer >}}

- What members are defined in `dirent` structure according to the LINUX (`man readdir`)? 
{{< answer >}}
Name, inode and 3 other not covered by the standard.
{{< /answer >}}

- Where the Linux/GNU documentation deviates from the standard, always follow the standard -- it will result in better
portability of your code.

- Error checks usually follow the same schema: `if(fun()) ERR();` (the `ERR` macro was declared and discussed before). You should
check for errors in all the functions that can cause problems, both system and library ones. In practice, nearly all
those function should be checked for errors. Most errors your code can encounter will result in program termination, some exceptions will be discussed in the
following tutorials.

- Pay attention to the use of `.` folder in the code, you do not need to know your current working folder, it's simpler
this way.

- Please notice that `errno`
is reset inside the loop, not before it. Also notice that in case of `NULL`, the program flows to the comparison of `errno`
through simple conditions only (no function calls).

- Why do we need to zero `errno` in first place? `readdir` could do it for us, right? The clou is that the POSIX says that
system function *could* zero `errno` on the the success but it is not obliged to do it.

- If you want to make assignments inside logical expressions in C, you should add parenthesis, its value will be equal to
the value assigned, see `readdir` call above.

## Working directory

The program from the previous excercise only allowed to scan the contents of the directory, which it was run in.
It would be much better to choose the directory to be scanned. As we can see, that it would be sufficient to replace the `opendir` argument to the path given e.g. in the positional parameter. 
Despite that, we won't modify the `scan_dir` function, in order to present the way to load and change the working directory from within the program code.

In order to get and change the working directory, we will make use of `getcwd` and `chdir` functions, present in the `<unistd.h>` header (`man 3p getcwd`, `man 3p chdir`). Their declarations, according to POSIX, are as follows:

```
char *getcwd(char *buf, size_t size);
```
- `buf` is the already allocated array of characters, that the **absolute** path to the working directory will be written into. This array should have at least `size` length,
- the function returns `buf` when successful. In case of failure, `NULL` is returned, and `errno` is set to the appropriate value.

```
int chdir(const char *path);
```
- `path` is a path to the new working directory (may be either relative or absolute),
- like many system functions that return `int`, the `chdir` function returns `0` on success and a different value on failure.

### Excercise

Use function from `l1-1.c` to write a program that will count objects in all the folders passed to the program as positional parameters.

### Solution

New man pages:
```
man 3p getcwd
man 3p chdir
```

solution `l1-2.c`:
{{< includecode "l1-2.c" >}}

### Notes and questions 

- Check how this program deals with:
   - non existing folders, 
   - no access folders, 
   - relative paths and absolute paths as parameters. 

- Why does this program store the initial working folder?
{{< answer >}}
This is the solution to the case when the user specifies several relative paths as parameters, e.g. 
`l1-2 dir1 dir2/dir3`. The program from the solution changes the working directory to the target directory before scanning. 
Thus, if we did not return to the starting directory each time after browsing the directory,
we would have tried to visit the `./dir1/` folder first (this is still correct) and then `./dir1/dir2/dir3/` instead of the 
the anticipated `./dir2/dir3/`.
{{< /answer >}}

- Is this true that the program should change the working directory back to the one which it was launched in?
{{< answer >}}
No, the working directory is a property of a single process. Changing CWD by a child process does not influence
the parent process, so there is no need for changing back.
{{< /answer >}} 

- Not all errors encountered in this program has to terminate it, what error can be handled in better way, how?
{{< answer >}} 
The `chdir` function may, for example, indicate an error for a non-existent directory. This could be handled with
`if(chdir(argv[i])) continue;`. It is the simplest solution, but it would be nice to add some message to it.
{{< /answer >}}

- Never ever code in this way: `printf(argv[i])`. What will be printed if somebody puts `%d` or other `printf` placeholders in
the arguments? This applies to any string, not only the one from arguments.

## File Operations

A large portion of programs interact with files on the disk. The simplest way to achieve this is:
1. Opening (or creating) a file using `fopen` (`man 3p fopen`),
2. Setting the file cursor with `fseek` (`man 3p fseek`),
3. Writing data with `fprintf`, `fputc`, `fputs`, `fwrite`, or reading it with `fscanf`, `fgetc`, `fgets`, `fread`,
4. Repeating steps 2-3 as necessary,
5. Closing the file using `fclose` (`man 3p fclose`).

The required functions are available in the `<stdio.h>` header.
```
FILE *fopen(const char *restrict pathname, const char *restrict mode);
```
- `pathname` specifies the path of the file to be opened,
- `mode` is the mode in which we want to open the file. The mode string can look as follows, enabling different ways of interacting with the file:
   - `r` - opens the file for reading,
   - `w` or `w+` - truncates the file to zero length (or creates it) and opens it for writing,
   - `a` or `a+` - allows appending data to the end of its existing content,
   - `r+` - opens the file for both reading and writing.

You can add `b` to each mode, which doesn’t affect the file descriptor on UNIX. It is allowed for C standard compatibility.

This function returns a pointer to an internal `FILE` structure, allowing control over the stream associated with the opened file. Following the wisdom of a comment by Pedro A. Aranda Gutiérrez in one `FILE` implementation in `<stdio.h>`:

> \* Some believe that nobody in their right mind should make use of the\
> \* internals of this structure.

we won’t delve into its internals. The structure's design depends on the specific system implementation, so we treat it as an opaque type, not reading nor setting any fields manually. We only store a pointer and use it by calling various functions on it.

The `fseek` function accepts a `FILE` pointer and lets us move to a specified position in the file. The exception is a file opened in "a" (append) mode, which always points to the end regardless of `fseek` calls. Otherwise, immediately after opening, the file cursor points to the first byte.

```
int fseek(FILE *stream, long offset, int whence);
```
- `stream` is the above-mentioned file stream identifier,
- `offset` specifies the number of bytes to move,
- `whence` defines the reference point for the move. It can have the following values:
   - SEEK_SET - the reference point is the beginning of the file, setting the cursor at the `offset`-th byte,
   - SEEK_CUR - a relative move from the current cursor position, moving `offset` bytes forward (or backward if negative),
   - SEEK_END - the reference point is the end of the file. The cursor points to data only if `offset` is negative.
      - For `offset` of `0`, the file cursor is positioned at the byte after the last byte, with `ftell` then returning the file’s exact size in bytes. This operation allows the programmer to allocate the exact number of bytes required to read the entire file.

Once the file cursor is set to the desired position, we can start reading data from or writing data to the file. `fprintf` and `fscanf` work analogously to the well-known `printf` and `scanf` functions for standard input. The other functions may initially seem less useful, although `fread` (`man 3p fread`) in particular is much more commonly used than its cousin `fscanf`. Custom implementation of conversion from raw file data to target data types offers much more control than library implementations.
```
size_t fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
```
- `ptr` - the buffer into which data will be written,
- `size` - the size of contiguous elements to read,
- `nitems` - the number of elements to read,
- `stream` - a pointer obtained from `fopen`.

The returned value indicates the number of successfully read elements. It will be smaller than `nitems` in the event of an error or file end. You might think splitting the data read into elements is an unnecessary complication, but it’s advantageous if we don’t want to split the data reading mid-record (e.g., 4-byte integer variables `int`). For a partial read, we don’t need to calculate the number of successfully read objects nor rewind the file cursor to read an incomplete record again. When the first call returns `n`, it's sufficient to call the function again with the buffer offset: `ptr+size*n` and a smaller number of elements: `nitems-n`.

It’s essential to release resources once finished using `fclose`.

If we need to delete a file, call `unlink` (`man 3p unlink`). If any process (including ours) still opens the `unlink`ed file, it’s removed from the file system but remains in memory. It's fully removed once the last process closes it.

### Task

Write a program that creates a new file with a name specified by the parameter (-n NAME), permissions (-p OCTAL), and size (-s SIZE). The file’s content should be about 10% random characters [A-Z], with the rest filled with zeros (null characters with code 0, not '0'). If the specified file already exists, delete it.

### Solution Outline

What the student needs to know:
- man 3p fopen
- man 3p fclose
- man 3p fseek
- man 3p rand
- man 3p unlink
- man 3p umask

glibc documentation on umask <a href="http://www.gnu.org/software/libc/manual/html_node/Setting-Permissions.html">link</a>

<em>code for file <b>prog12.c</b></em>
{{< includecode "prog12.c" >}}

### Notes and questions

- What bitmask is created by the expression `~perms&0777`?
{{< answer >}}
The inverse of permissions specified by the -p parameter, truncated to 9 bits. If unclear, review bitwise operations in C.
{{</ answer >}}

- How does character randomization work?
{{< answer >}}
Sequential alphabet characters are inserted at random locations. The characters go from A to Z, then loop back to A. The expression 'A'+(i%('Z'-'A'+1)) should be understandable; if not, study it further, as such randomization will recur.
{{</ answer >}}

- Run the program several times, view the output files with `cat` and `less`, and check sizes (ls -l). Are they always the specified parameter size? Explain the difference between small -s and larger sizes (>64K).
{{< answer >}}
Sizes are almost always different. This results from file creation: initially empty, then populated at random locations. The last position will not always have a character. Randomization is limited by the 2-byte RAND_MAX, so in large files, characters are placed up to RAND_MAX.
{{</ answer >}}

- Modify the program to always match the set size exactly.

- Why do we ignore one case in unlink error checking?
{{< answer >}}
ENOENT indicates a non-existent file, so we can’t delete it if it didn’t exist. Without this exception, we could only overwrite existing files, not create new ones.
{{</ answer >}}

- Pay attention to moving file-creation functions outside of `main`. The larger the code, the more critical the division into functions becomes. We will briefly outline the attributes of a good function:
   - Does only one thing at a time (short code)
   - Generalizes the problem as much as possible (e.g., adding percentage as a parameter)
   - Receives all input through parameters (no global variables)
   - Returns results via pointer parameters or return value (in this case, the file), not global variables.

- Why use types like `ssize_t`, `mode_t` instead of int in this program? This ensures type compatibility with system function prototypes.

- Why do we use `umask` here? The `fopen` function doesn’t set permissions, but `umask` allows restricting the default permissions given by `fopen`; low-level `open` gives better control over permissions.

- Why can’t we add `x` permissions? `fopen` assigns only 0666 rights, not full 0777; bitwise subtraction can't yield the missing 0111 component.

- We always check for errors, but not umask status. Why? `umask` doesn’t return errors, only the old mask.

- `umask` changes are local to our process and don’t affect the parent process, so restoring it is unnecessary.

- The `-p` text parameter was converted to octal permissions with `strtol`. Knowing such functions prevents "reinventing the wheel" for straightforward conversions.

- Why delete the file if the `w+` open mode overwrites it? If a file already existed under the given name, its permissions would persist, and we must assign ours. Also, it’s a pretext for deletion practice.

- POSIX systems don’t distinguish `b` mode; only binary access exists.

- Zeros automatically fill the file, since writing beyond the file end auto-fills gaps with zeros. Long zero sequences take up no disk sectors!

- If we unlink an open file in another program, it vanishes from the file system but remains accessible to interested processes until they’re done. Then it disappears.

- Call `srand` once with a unique seed; in this program the time in seconds is sufficient.

## Buffering of Standard Output

### Experiment

**Code for file** `prog13.c`
{{< includecode "prog13.c" >}}

- Try running this (very simple!) code from the terminal. What do you see in the terminal?  
{{< answer >}}  
As expected, a number appears every second.
{{</ answer >}}

- Now try running the code again, but this time redirect the output to a file with `./executable_file > output_file`. Then try to open the output file while the program is running, and then end the program with `Ctrl+C` and open the file again. What do you see this time?  
{{< answer >}}  
If you perform these steps quickly enough, the file will appear empty! This is because the standard library detects that the data isn’t going directly to the terminal and buffers it for efficiency, only writing it to the file once enough data has been gathered. This means that the data isn’t immediately available, and in case of an unexpected program termination (such as when using `Ctrl+C`), the data might even be lost. Of course, if we let the program finish, all data will be written to the file (please try this!). Buffering can be configured, but we don’t have to, as we’ll see in a moment.
{{</ answer >}}

- Run the code again, letting the output go to the terminal as in the first run, but this time remove the newline from the `printf` argument: `printf("%d", i);`. What do we see this time?  
{{< answer >}}  
Despite what was mentioned earlier, no output is visible even though the data is going directly to the terminal. Instead, the same behavior occurs as in the previous step. This is because the library buffers standard output even if data is directed to the terminal; the only difference is that it reacts to newline characters by flushing all the data collected in the buffer. This mechanism is why there was no unexpected behavior in the first step. This is also why you may sometimes find that `printf` doesn’t display anything on the screen; if we forget the newline character, the standard library won’t display anything until a newline appears in another printed string or the program ends successfully.
{{</ answer >}}

- Try repeating the previous three steps, but this time output the data to the standard error stream using `fprintf(stderr, /* parameters previously passed to printf */);`. What happens this time? To redirect standard error to a file, use `>2` instead of `>`.  
{{< answer >}}  
This time, nothing is buffered, and as expected, we see one digit every second. The standard library doesn’t buffer standard error because it’s often used for debugging.
{{</ answer >}}

- You may want to use `printf(...)` for debugging by adding calls to this function to check variable values or whether execution reaches certain points in the code. In such cases, use `fprintf(stderr, ...)` to output to standard error. Otherwise, data might be buffered and displayed later than expected—or, in extreme cases, not at all. If you’re not sure which stream to use, choose standard error. When writing real console applications, standard output is used only for displaying results, and standard error is used for everything else. For example, `grep` outputs matches to standard output, but any file access errors go to standard error. Even our `ERR` macro prints errors to the standard error stream.

## Example tasks
Do the example tasks. During the laboratory you will have more time and the starting code. However, if you finish the following tasks in the recommended time, you will know you're well prepared for the laboratory.

- [Task 1]({{< ref "/sop1/lab/l1/example1" >}}) ~75 minutes
- [Task 2]({{< ref "/sop1/lab/l1/example2" >}}) ~75 minutes
- [Task 3]({{< ref "/sop1/lab/l1/example3" >}}) ~120 minutes

## Source codes presented in this tutorial

{{% codeattachments %}}
