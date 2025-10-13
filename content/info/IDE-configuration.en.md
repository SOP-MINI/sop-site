---
title: "Configuring IDE"
weight: 40
---

# Configuring IDE

No specific environment is required in the laboratories. You can choose any program installed on the faculty's computers and even (for advanced users) if you really miss your favorite program, try a local installation. However, a good code editor or IDE (Integrated Development Environment) should have several features that are very useful when solving lab tasks. These include:

- program syntax highlighting
- error reporting and highlighting
- hints and autocompletion (except for AI-based tools such as Copilot, these are prohibited)
- autoformatting on save

Several programs are installed on the lab computers that (when properly configured) meet all these requirements. These include (in alphabetical order):

- CLion
- Emacs
- Geany
- QtCreator
- Vim
- Visual Studio Code

Below is a sample IDE configuration tutorial, but it is worth noting that this is only an example dictated by the author's personal preferences. All of the programs listed above (with proper configuration) should work equally well, so if you already have a favorite tool, you can stick with it (although it is worth giving QtCreator a try, a very nice IDE ~ _author's personal note_).


# Configuring QtCreator for laboratories

First, to test program's features we create a simple project. Create a new directory (in our example we name it `test`) and two files in it: `main.c` and `Makefile`:

**main.c**:
```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Hello world\n");
    return EXIT_SUCCESS;
}
```

**Makefile**:
```
all: main
main: main.c	
	gcc -fsanitize=address,undefined -Wall -Wextra -Wshadow -Werror -o main main.c
.PHONY: clean all
clean:
	rm main
```

Now, calling `make` should correctly compile our small program, which after running should print `Hello world`.

We can now launch the QtCreator. After launching, select `File->New Project`. It is important to select `New Project` and not `Open File or Project` - if we simply open our `main.c` file, QtCreator will not know which flags we want to use to compile the program, and thus its operation may be unstable (e.g., reporting errors that do not actually exist). In the window that appears, select `Import project` and then `Import Existing Project`.

![](/img/qtcreator1.png) 

Next, we choose a name for our project (in the example, it is simply `test`) and the project directory from which we then add source files - in our case, `main.c`. After doing this, QtCreator will create a series of files named `test.<extension>` in the project directory and open our new project. It should look like this:

![](/img/qtcreator2.png) 

During the laboratory, these newly created files should not be added to git (the server will reject them) - this is only our local configuration; in practice, when several programmers work together, each of them can use different tools. From these files, we are now only interested in the `test.cflags`. In this file, we specify the compilation flags that QtCreator will use for its "smart" features, such as error reporting. It is important that the flags are the same as in the `Makefile`, so now you should paste there `-fsanitize=address,undefined -Wall -Wextra -Wshadow -Werror`.

At this point, all basic functionalities should work. As you write code, you should see a window with hints. When you make a mistake (try, for example, not adding a semicolon at the end of a line), the program will highlight the incorrect line in red.

The last step is to enable formatting on save - this will save us the hassle of calling `clang-format` before each commit. To do this, we need to enable the `beautifier` plugin. From the `Help` menu, click `About Plugins`, then find `Beautifier` in the list and activate it; then accept by clicking `Ok`. You will need to restart the IDE. In the project folder, create a configuration file for `clang-format` - during the lab, it will be included in the initial repository.

**.clang-format**:
```
BasedOnStyle: Google
IndentWidth: 4
ColumnLimit: 120
BreakBeforeBraces: Allman
BreakConstructorInitializersBeforeComma: false
AllowShortIfStatementsOnASingleLine: false
AllowShortBlocksOnASingleLine: false
AllowShortLoopsOnASingleLine: false
IncludeBlocks: Preserve
PointerAlignment: Left
InsertNewlineAtEOF: true
```

Note that the name of this file begins with a dot - it is invisible by default. Now go to the `Edit->Preferences` menu and select the `Beautifier` section. In the `General` tab, check `Automatic formatting on file save` and select `ClangFormat` from the `Tool` menu. In the `ClangFormat` tab, select `Use predefined style` and set it to `File`. Accept the settings by clicking `Ok`.

![](/img/qtcreator3.png) 

Now, every time you save a file (Ctrl+S), it will be automatically formatted according to configuration from config file. To check if this feature works correctly, try adding a few unnecessary spaces somewhere in the program and save the file.

Your IDE is now ready for the laboratories. It is still worth spending a few minutes reviewing and adjusting various options in the settings (`Edit->Preferences`), especially in the Environment section, where you can configure the appearance and keyboard shortcuts. By default, QtCreator has a very convenient search engine under the shortcut `ctrl+k` that allows you to find various object classes (files, functions, global variables, etc.) in the project. Other options and instructions can be found in the help and documentation.
