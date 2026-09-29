---
title: "Example task 3 on POSIX program environment"
bookHidden: true
---

# Description

Write a program simulating the creation of a virtual environment and
package management within the virtual environment. Upon successful
execution, the program will return the status `EXIT_SUCCESS`.

Creating an environment involves:

-   Creating a folder in the current directory with the name of the
    environment.

-   Adding a file named `requirements` to the created folder (initially
    empty).

The creation of a virtual environment is done using the `-c` option. The
`-v` option indicates the environment on which we perform the operation
(in this case, creating it). Example program invocation:
`./sop-venv -c -v <ENVIRONMENT_NAME>`.

Managing environments involves installing and uninstalling packages from
the environment. Installation is represented by:

-   Appending a line with the package name and its version (separated by
    a space) to the `requirements` file. The package can have any name
    and must have any version at this point.

-   Creating a file with the package name, filled with random content,
    and with read-only permissions for all user classes.

Uninstallation is done by removing the line with the package and the
file with the package name.

Command syntax:

-   Installation:
    `./sop-venv -v <ENVIRONMENT_NAME> -i <PACKAGE_NAME>==<VERSION>`

-   Uninstallation: `./sop-venv -v <ENVIRONMENT_NAME> -r <PACKAGE_NAME>`

The program should locate environments in the current directory. The
`-v` option can be used multiple times (except for creating an
environment), indicating the same operation on different environments
(installation or uninstallation). An error in one environment should not
stop the program. Options can be used in any order!

Additionally, the program should handle errors correctly (displaying a
message and returning the appropriate status) related to command syntax,
file operations, adding, removing packages, or operating on non-existing
environments. It is not allowed to add two identical packages, even with
different versions, or remove non-added ones.


## Example:

Examples of program usage:
```
    $ ../sop-venv -c new_environment
    $ ls ./
    new_environment
    $ ls new_environment/
    requirements
```
The above command will create a directory `new_environment` with a
`requirements` file inside it.
```
    $ ../sop-venv -v new_environment -i numpy==1.0.0
    $ ../sop-venv -v new_environment -i pandas==1.2.0
    $ cat new_environment/requirements
    numpy 1.0.0
    pandas 1.2.0
    $ ls -l new_environment/
    -r--r--r-- 1 user user 20 Oct 13 19:51 numpy
    -r--r--r-- 1 user user 30 Oct 13 19:51 pandas
    -rw-r--r-- 1 user user 17 Oct 13 19:50 requirements
    $ ../sop-venv -v new_environment -r numpy
    $ cat new_environment/requirements
    pandas 1.2.0
    $ ls -l new_environment/
    -r--r--r-- 1 user user 30 Oct 13 19:51 pandas
    -rw-r--r-- 1 user user 17 Oct 13 19:50 requirements
```
The above command will install two packages in `new_environment`, one
with a previously defined version, and then remove one of them.
```
    $ ../sop-venv -v non_existing_environment -i pandas
    sop-venv: the environment does not exist
    usage: ...
```
The above command on a non-existing environment will result in an error
and display the so-called `usage`.


## Stages

- Implementation of environment creation
- Implementation of package installation and handling described errors
- Ability to use the `-v` option multiple times
- Implementation of package removal
