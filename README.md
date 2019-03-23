# Shell
These files create a simple Linux shell. This shell accepts user commands and then executes each command in a separate process. <br>
The shell only takes `exit, pwd, cd, type, history` <br>
* `exit` exits the shell program.
* `pwd` display current working directory.
* `type` return the type of command for instance:
    ```
    $ type cd
    'cd' is a shell300 builtin
    ```
    ```
    $ type ls
    'ls' is external to shell300
    ```
    * since `ls` is not implementing in the code
* `cd` changes the current directory
* `history` displays the most recently 10 entered commands and `!` command runs the commands directly from the history list.
### important notice: `history`and `!` commands are not fully implemented, still have some bugs there
