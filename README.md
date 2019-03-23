# shell
These files create a simple Linux shell. This shell accepts user commands and then executes each command in a separate process. <br>
For now, the shell only take `exit, pwd, cd, type, history` <br>
* `exit` will exit the shell program.
* `pwd` display current working directory.
* `type` will return the type of command for instance:
    ```
    $ type cd
    'cd' is a shell300 builtin
    
    $ type ls
    'ls' is external to shell300
    ```
    * since `ls` is not implementing in the code
* `cd` will change the current directory
* `history` will display the most recently 10 entered commands and `!` command will run the commands directly from the history list.<br>
### `history`and `!` commands are not fully implemented, still have some bugs there
