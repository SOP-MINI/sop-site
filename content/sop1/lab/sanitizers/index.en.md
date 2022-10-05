---
title: "GCC Sanitizers"
date: 2022-02-05T17:31:35+01:00
---

Besides compiler diagnostics provided by -Wall compilation option during the class it is require to use sanitizers that are build in sufficiently new compilers. Sanitizers cause that emitted code is instrumented with many runtime checks that provide additional program verification layer. For more details please refer to the compiler documentation.

During the Operating systems class we will mostly use -fsanitize=address and -fsanitize=undefined. The first set enables various checks regarding memory access and the second checks behaviours that are undefined by standard. In contrast to -Wall option, which only emits messages during the compilation, sanitizers change the output program code. The resulting code will be less efficient because of additional checks and therefore the use of sanitizers in the release version for final users should be carefully considered. During the lab the release version is not a problem.

The advantage of using sanitizers is ability to find code problems that are either impossible of very expensive (in terms of computation costs) to detect during the compilation. On the other hand only problems that actually happened during the execution will be detected (which may depend on e.g. input data).

The example of unsafe stdin reading:
```
#include <stdio.h>

int main(int argc, char** argv) {
  int x=5;
  char name[10];
  scanf("%s", name); // Reading stirng of unknown (probably exceeding 9) length
  printf("You typed: %s\nThe number is %d\n", name, x);
  return 0;
}
```

Compiling the program without -fsanitize and typing a string longer than 9 characters on stdin will end up with messing the x variable value. In small program such problem is easy to locate, but in actual lab code...  
Now try compiling it with sanitizers:  
`  gcc -Wall -fsanitize=address,undefined array-problem.c -o array-problem `  
Again try to type strings longer and shorter than 9. This time longer string caused a descriptive error message (please get familiar with those messages before the first lab). There is one inconvenience tough, the part regarding our program in stack trace (the main function) is missing the line number. We can ask compiler to emit debug info (in particular line numbers) to the output program by passing -g option: `  gcc -g -Wall -fsanitize=address,undefined array-problem.c -o array-problem `

*Along the assignment for each lab you will be provided with a set of mandatory sanitizers for given lab. Note that enabling all the sanitizers is not possible as some of them are mutually exclusive.*
