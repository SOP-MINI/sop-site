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

Below are configuration tutorials for selected IDEs, but it is worth noting that these are only examples. All of the programs listed above (with proper configuration) should work equally well, so if you already have a favorite tool, you can stick with it (although it is worth giving QtCreator a try, a very nice IDE ~ _author's personal note_).

## First step - creating a sample project

As the first step in the configuration process for any of the following guides, we should create a sample project that we will later load/open using the selected IDE.

We create a simple project. Create a new folder with a name of your choice (in our case, it will be `test`) and place two files inside it: `main.c` and `Makefile`:

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
```make
all: main
main: main.c
	gcc -fsanitize=address,undefined -Wall -Wextra -Wshadow -Werror -o main main.c
.PHONY: clean all
clean:
	rm main
```

In the project folder, we also create a configuration file for `clang-format`; during the laboratory classes, this file will be included in the starter repository. **Important!** This file starts with a dot, so it is hidden by default.

**.clang-format**:
```yaml
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

Now we can proceed to configuring the selected IDE.

## Configuring QtCreator for laboratories

We assume that we have already created a sample project, as described [here](#first-step---creating-a-sample-project). Then, we navigate to the newly created directory.  

Now, calling `make` should correctly compile our small program, which after running should print `Hello world`.

We can now launch the QtCreator. After launching, select `File->New Project`. It is important to select `New Project` and not `Open File or Project` - if we simply open our `main.c` file, QtCreator will not know which flags we want to use to compile the program, and thus its operation may be unstable (e.g., reporting errors that do not actually exist). In the window that appears, select `Import project` and then `Import Existing Project`.

![](/img/qtcreator1.png) 

Next, we choose a name for our project (in the example, it is simply `test`) and the project directory from which we then add source files - in our case, `main.c`. After doing this, QtCreator will create a series of files named `test.<extension>` in the project directory and open our new project. It should look like this:

![](/img/qtcreator2.png) 

During the laboratory, these newly created files should not be added to git (the server will reject them) - this is only our local configuration; in practice, when several programmers work together, each of them can use different tools. From these files, we are now only interested in the `test.cflags`. In this file, we specify the compilation flags that QtCreator will use for its "smart" features, such as error reporting. It is important that the flags are the same as in the `Makefile`, so now you should paste there `-fsanitize=address,undefined -Wall -Wextra -Wshadow -Werror`.

At this point, all basic functionalities should work. As you write code, you should see a window with hints. When you make a mistake (try, for example, not adding a semicolon at the end of a line), the program will highlight the incorrect line in red.

The last step is to enable formatting on save - this will save us the hassle of calling `clang-format` before each commit. To do this, we need to enable the `beautifier` plugin. From the `Help` menu, click `About Plugins`, then find `Beautifier` in the list and activate it; then accept by clicking `Ok`. You will need to restart the IDE. In the project folder, create a configuration file for `clang-format` - during the lab, it will be included in the initial repository. There is a configuration file in our project directory .clang-format that we previously created.

Note that the name of this file begins with a dot - it is invisible by default. Now go to the `Edit->Preferences` menu and select the `Beautifier` section. In the `General` tab, check `Automatic formatting on file save` and select `ClangFormat` from the `Tool` menu. In the `ClangFormat` tab, select `Use predefined style` and set it to `File`. Accept the settings by clicking `Ok`.

![](/img/qtcreator3.png) 

Now, every time you save a file (Ctrl+S), it will be automatically formatted according to configuration from config file. To check if this feature works correctly, try adding a few unnecessary spaces somewhere in the program and save the file.

Your IDE is now ready for the laboratories. It is still worth spending a few minutes reviewing and adjusting various options in the settings (`Edit->Preferences`), especially in the Environment section, where you can configure the appearance and keyboard shortcuts. By default, QtCreator has a very convenient search engine under the shortcut `ctrl+k` that allows you to find various object classes (files, functions, global variables, etc.) in the project. Other options and instructions can be found in the help and documentation.

## Configuring Visual Studio Code

**Note!** VS Code comes in two main versions:
- **Visual Studio Code** -- the official version distributed by Microsoft (most likely the version you use on your own computer).
- **Code OSS** -- the open-source version developed by Microsoft and the community (this is the version we use in the lab).

Code OSS does not list all the extensions available in Microsoft's official version in its built-in extension search. Installing some of them therefore requires an extra step. In this tutorial, this applies to the **C/C++** extension, which makes working with C and C++ easier.

If you use the official Microsoft version of VS Code, you can install the **C/C++** extension directly in the editor. Open Visual Studio Code and go to the Extensions tab (keyboard shortcut: `Ctrl + Shift + X`).

![](/img/vsc1.png)

Search for the **C/C++** extension published by Microsoft (it should be among the first results), then click *Install*. Installation is automatic and usually does not require restarting the editor.

![](/img/vsc2.png)

With Code OSS, which we use in the lab, an extra step is needed. The **C/C++** extension is not available in Code OSS's default extension registry (Open VSX), so we need to download the installation file manually. Go to the [extension's official GitHub releases page](https://github.com/microsoft/vscode-cpptools/releases). Choose a release (preferably the latest stable version), expand the `Assets` section, and download the `.vsix` file for Linux on x86-64, such as `cpptools-linux-x64.vsix`.

![](/img/vsc1_OSS.png)

Next, install the downloaded extension in the editor. Open the Extensions tab (`Ctrl + Shift + X`), click the three dots (`...`) next to the *Extensions* heading in the upper-left corner, and select `Install from VSIX...`. Select the downloaded `.vsix` file, confirm the installation, and restart the editor if necessary.

![](/img/vsc2_OSS.png)

This installs an extension that makes working with C easier.  

The rest of the tutorial is the same for both versions of the editor.  

The next extension to install is **Makefile Tools**. It is also developed by Microsoft, but unlike C/C++, it is available in Code OSS's default extension registry. In both versions of the editor, install it as usual: open the Extensions tab (`Ctrl + Shift + X`), search for **Makefile Tools**, and click *Install*.

![](/img/vsc3.png) 

These are all the extensions needed for comfortable work with C code.   

In Visual Studio Code, click `File -> Open Folder` in the upper-left corner and select the `test` folder that we [created earlier](#first-step---creating-a-sample-project).  

With this configuration, running the `make` command compiles the program that prints `Hello world` to the standard output.  

**Note!** A very important step is changing the default C standard used by the editor. Otherwise, the editor may mark some names as unknown even though the program compiles correctly. To fix this, change the default standard from `c17` to `gnu17`. To do this, press `Ctrl + ,` (Control + comma), type `C standard` in the settings search bar, and change `c17` to `gnu17`. It should look something like this:

![](/img/vsc5.png) 

From this point on, the editor should analyze and highlight the code correctly.  

**Note!** It is necessary to ensure that no AI-assisted tools are active in the environment. In newer versions of VS Code, GitHub Copilot features are available built in, but using them requires activation and signing in. To be safe, they should be disabled. Installing additional extensions with AI agents is not allowed.

An easy way to disable AI-related features is to press `Ctrl + ,` (Control plus comma), type `disable ai features` in the settings search bar, and then enable the `Chat: Disable AI Features` option, which sets `chat.disableAIFeatures` to `true`.

![](/img/vsc14.png)

The next step is to enable error highlighting in the code. In theory, this option is enabled by default for the C language, but it often happens that, for some reason, it is disabled without any user intervention.  
Press `Ctrl + ,` (left Ctrl key and comma), then type `error squiggles` in the settings search bar and change the option to `enabled`.

![](/img/vsc6.png) 

Configure the IntelliSense engine in a similar way. Its default value should also be set to `default`, although this is not always the case. Press `Ctrl + ,` and type `IntelliSense engine`. Set the value of `C_Cpp.intelliSenseEngine` to `default`.

![](/img/vsc7.png)

The next step is to configure support for the `Makefile`, so that compilation uses the flags defined in it.

Press `Ctrl + ,` and type `configuration provider` in the search bar. In the `C_Cpp.default.configurationProvider` field, enter the following value: `ms-vscode.makefile-tools`. This allows VS Code to automatically use the settings provided in the `Makefile`.  

![](/img/vsc8.png)

Next, with the project open, press `Ctrl + Shift + P` and run the `Makefile: Configure` command. This will automatically configure Makefile support in VS Code.

Next, we will configure an optional but very useful feature: automatic code formatting when saving C files. During the labs, our project will include the following `.clang-format` file.

We want VS Code to use the above file as the formatting configuration. First, enable code formatting on save: press `Ctrl + ,`, type `format on save` in the search bar, and enable the `Format on File save` option.

![](/img/vsc9.png)

Next, search for `default formatter` and, in the `Default Formatter` setting, select `C/C++ ms-vscode.cpptools`.

![](/img/vsc10.png)

Then search for `C_Cpp formatting` and change the `C_Cpp: Formatting` setting to `clangformat`.

![](/img/vsc11.png)

Finally, make sure that the `C_Cpp: Clang_format_style` option (available through the search bar) is set to `file`. This should be the default value. Thanks to this setting, VS Code will use the formatting options defined in the `.clang-format` file.

![](/img/vsc12.png)

These are all the steps needed to make formatting run automatically when saving a file. This will speed up our work and make completing laboratory tasks much easier.  

A rather problematic editor feature may be the automatic insertion of `#include` directives into `.c` files. This can cause significant issues during laboratory classes. To disable it, press `Ctrl + ,`, type `Refactoring Include Header`, and set the `Include Header` option to `never`. VS Code should now no longer automatically add unnecessary `#include` directives to `.c` source files.

![](/img/vsc15.png)

As the final step, let us take care of hints and autocompletion. Most hint and autocompletion options should already be active, but we can verify this by pressing `Ctrl + ,` and searching for `parameter hints`. It is recommended to apply the settings shown in the screenshot below:

![](/img/vsc13.png)  

After completing the steps described above, Visual Studio Code should be ready for use during the labs. However, it is worth spending a moment getting familiar with a few additional editor features, because VS Code is a very flexible and highly customizable environment. The command palette, available under `Ctrl + Shift + P`, is especially useful because it allows you to run most editor functions without looking for them in the menu. It is also worth exploring the extensions panel, code formatting settings, and the integrated terminal, which can be opened with `Ctrl + ,`. More options and configuration examples can be found in the [Visual Studio Code documentation](https://code.visualstudio.com/docs).

## Configuring CLion

### Disable AI Plugins
Using AI assistance during laboratories is strictly prohibited. The recommended way to disable AI capabilities in CLion is to disable the relevant bundled plugins:

1. Open **Settings** (`Ctrl + Alt + S`).
2. Navigate to **Plugins** and select the **Installed** tab.
3. Find and disable (uncheck) the following plugins:
   - **Full Line Code Completion**
   - **Machine Learning in Search Everywhere**
4. Click **Apply**.

### Man Pages on Hover
To view Linux man pages directly when hovering over system/library functions:

1. While still in **Settings -> Plugins**, go to the **Marketplace** tab.
2. Install **C/C++ Language Support via Classic Engine**.
3. Switch back to the **Installed** tab, find **CLion C and C++**, and disable (uncheck) it.
4. Click **Apply** / **OK** and restart the IDE.

### Formatting on Save (.clang-format)
CLion natively detects and uses the `.clang-format` file located in the root of the project. To format code on save shortcut automatically:

1. Open **Settings** (`Ctrl + Alt + S`).
2. Go to **Tools -> Actions on Save**.
3. Check the **Reformat code** checkbox.

Using `Ctrl + S` shortcut will now automatically format the file according to the `.clang-format` rules.

### Opening the Project
1. Go to **File -> Open...** and select the project directory containing your `Makefile`.
2. In the **Load Project** dialog that appears, simply click **OK** so CLion can process the Makefile and its targets.
3. If you accidentally cancel the prompt or the project does not load, go to **Tools -> Makefile -> Clean and Reload Makefile Project** from the top menu.

### Running and Debugging
CLion detects build targets from the `Makefile`, but because binary names are often defined using variables (such as `NAME=sop-prog`), CLion cannot automatically infer the executable output. You must specify the executable manually:

1. Click the configuration dropdown in the top toolbar and select **Edit Configurations...** (or open **Run -> Edit Configurations...**).
2. Select your target configuration (e.g., `all`).
3. In the **Executable** field, enter manually the compiled binary path (e.g., `./sop-prog`).
4. To pass command-line arguments, enter them in the **Program arguments** field.
5. To define or override environment variables, enter them in the **Environment variables** field (or click the icon on the right to add key-value pairs).
6. Click **Apply** / **OK**.

Once configured, you can run (`Shift + F10`) or debug (`Shift + F9`) the code. You can also run `make` and execute programs directly using the built-in terminal (`Alt + F12`).

> #### Debugging with AddressSanitizer
> When launching the debugger on a binary compiled with `-fsanitize=address`, LeakSanitizer will crash with:
> ```
> LeakSanitizer has encountered a fatal error.
> HINT: LeakSanitizer does not work under ptrace (strace, gdb, etc)
> ```
> Because GDB attaches via `ptrace`, LeakSanitizer cannot run simultaneously. To resolve this, add the following to the **Environment variables** field in your configuration whenever you need to debug your code:
> ```
> ASAN_OPTIONS=detect_leaks=0
> ```
