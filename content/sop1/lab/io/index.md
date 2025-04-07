---
title: "Input/Output"
---

# Input

## getline

1.  Not part of the POSIX standard! It's a GNU extension.
2.  Reads a newline-terminated string.
3.  Allocates memory for us, resizes the buffer if it's too small (via realloc).
4.  No overflows!
5.  Returns -1 on error, string length on success.

```c
char *buffer = NULL;
size_t buf_size = 0;
ssize_t str_len = getline(&buffer, &buf_size, stdin);

if(str_len == -1) ERR("getline");
buffer[str_len - 1] = '\0'; // Optionally insert a null char in 
                            // place of the new line char at the end
free(buffer);
```

## fgets

1.  Reads newline-terminated string.
2.  Uses a user-supplied buffer.
3.  No overflows... but if the buffer is not big enough, it will read only up to its size.
4.  Returns NULL on error, buffer pointer on success.

```c
#define BUF_SIZE 256
// ...
char buffer[BUF_SIZE];
if(NULL == fgets(buffer, BUF_SIZE, stdin))
    ERR("fgets");
size_t len = strlen(buffer);
if (len > 0 && buffer[len - 1] == '\n') // Optionally remove new line char if present
    buffer[len - 1] = '\0';
puts(buffer);
```
    
## fgetc

1.  Reads a single character.
2.  Returns EOF on error, character on success.
3.  There might still be other characters in the stdin.
4.  If the user input was \"ABC\", there are still 'B', 'C' and '\\n' characters in the stdin you might want to read. Consider reading the whole line and taking one character from it.

```c
int c = fgetc(stdin);
if(c == EOF) ERR("fgetc");
```

# Output

## puts

1.  Outputs a string to stdout and a trailing newline character.
2.  Use it when there is no string formatting.
3.  Returns EOF on error.

```c
char *str = "Hello!";
int ret = puts(str);
if(ret == EOF) ERR("puts");
```

## fputs

1.  Outputs a string without adding a trailing newline character.
2.  Use it when there is no string formatting.
3.  Returns EOF on error.

```c
char *str = "Hello!";
int ret = fputs(str, stdout);
if(ret == EOF) ERR("fputs");
```


## fputc

1.  Outputs a single character.
2.  Returns EOF on error.
3.  See also: `putc`, `putchar`.

```c
char c = 'H';
int ret = fputc(c, stdout);
if(ret == EOF) ERR("putc");
```

## printf

1.  Outputs a formatted string to stdout.
2.  Use when there is some string formatting involved.
3.  Never print user-inputted text using `printf`!
4.  Returns the number of characters printed on success, negative value on failure.
5.  See also: `fprintf`.

```c
const char *str = "Hello!";
int n = 4;
/* print at most first four characters */
int ret = printf("%.*s\n", n, str);
if(ret < 0) ERR("printf");
```
