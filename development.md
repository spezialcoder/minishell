# Minishell
--- ---
The shell header file is `shell/shell.h`.

All minishell source files are located in the `shell/` directory excluding `main.c`.

Functions to launch the shell are contained in `shell/shell.c`.
--- ---
## Interface
- [ ] *Prompt*
- [ ] *Single quote*
- [ ] *Double quote*
- [ ] *Pipelining commands*
- [ ] *Environment variables*
- [ ] *$? last exit status*
- [ ] *Handle control signals (ctrl)*
- [ ] *Handle redirections*
- [ ] *Ignore \ or ;*

> Interface related functions are found in `shell_interface_{module number}.c` files.

## Builtins
- [ ] *echo*
- [ ] *cd*
- [ ] *pwd*
- [ ] *export*
- [ ] *unset*
- [ ] *env*
- [ ] *exit*

> Builtins are found in `shell_builtins_{modules_number}.c` files.

## Backend
- [ ] *Binary finder and launcher* 

## Miscellaneous
Feel free to write something here.

## Bugs
No bugs yet
--- ---
> Allowed functions `readline, rl_clear_history, rl_on_new_line,
rl_replace_line, rl_redisplay, add_history,
printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
getcwd, chdir, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs`