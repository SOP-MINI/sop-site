---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Simple program compilation

```bash
make hello
./hello
```
[hello.c]({{< github_url "hello.c" >}})
[main.c]({{< github_url "main.c" >}})

### 4 compilation phases

```bash
gcc -E -o hello.i hello.c  # Stop after pre-processing
```

```bash
gcc -S -o hello.s hello.i  # Stop after compiler
```

```bash
gcc -c hello.s  # Stop after assembler
```

```bash
gcc -o hello hello.o  # Stop after linker
```

### Typical buildsystem behavior

```bash
#gcc -c main.c -o main.o
#gcc -c hello.c -o hello.o
#gcc -o hello main.o hello.o
make -B hello
```
[hello.c]({{< github_url "hello.c" >}})
[main.c]({{< github_url "main.c" >}})

### Inspect file types

```bash
file main.c
file main.o
file main
```

### ELF Object Files

```bash
readelf --file-header hello.o
```

```bash
readelf --sections hello.o
```

```bash
readelf --symbols hello.o
```

```bash
readelf -x .data hello.o
```

```bash
readelf -x .rodata hello.o
readelf -p .rodata hello.o
```

```bash
objdump --disassemble=f_defined hello.o
```

```bash
readelf --relocs hello.o
```

### ELF Executables

```bash
readelf -h hello
```

```bash
readelf --sections hello
```

```bash
readelf --symbols hello
```

```bash
objdump --disassemble=f_defined hello
objdump --disassemble=f_static hello
objdump --disassemble=f_external hello
```

#### Dynamic symbol resolution

```bash
objdump --disassemble=main hello
```

Note the call to `printf@plt`. Target address is in `.plt` section.

```bash
objdump -dj .plt hello
```

`.plt` contains short linker-generated trampoline to the value in `.got.plt` section:

```shell
readelf -x .got.plt hello
```

```bash
readelf --relocs hello
```

#### Loader view

```bash
readelf --segments hello
```

```bash
./hello block
```

```bash
cat /proc/$(pidof hello)/maps | grep hello
```

```bash
readelf --segments hello | grep LOAD -A1
```

#### Dynamic loader operation

```shell
readelf -p .interp main
```

```bash
readelf --dynamic main
```

```bash
LD_SHOW_AUXV=1 ./hello block
```

```bash
cat /proc/$(pidof hello)/maps | grep ld-linux
```

```bash
LD_DEBUG=all ./hello
```

### Dynamic library development

```shell
#gcc -Wall -Wextra -c hello.c -o hello.o
#gcc -Wall -Wextra -c main_shared.c -o main_shared.o
#gcc shared_utils.c -o libshared_utils.so -shared -fPIC
#gcc hello.o main_shared.o -o hello_shared -L. -lshared_utils
make -B hello_shared
```

```shell
file libshared_utils.so
```

```shell
readelf -h libshared_utils.so
```

```shell
readelf --dynamic hello_shared
```

```shell
readelf --relocs hello_shared
```

```shell
LD_LIBRARY_PATH=. ./hello_shared block
```

```bash
cat /proc/$(pidof hello_shared)/maps | grep libshared_utils.so
```

```shell
LD_DEBUG=all LD_LIBRARY_PATH=. ./hello_shared block
```

### Statically linked executable

```bash
#gcc -Wall -Wextra -c hello.c -o hello.o
#gcc -Wall -Wextra -c main.c -o main.o
#gcc -static hello.o main.o -o hello_static
make hello_static
```

```bash
hello_static
```

```bash
file hello_static
```

```bash
readelf -h hello_static
```

```bash
objdump --disassemble=f_defined hello_static
```
