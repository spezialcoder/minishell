# Minishell
--- ---
The shell header file is `shell/shell.h`.

All minishell source files are located in the `shell/` directory excluding `main.c`.

Functions to launch the shell are contained in `shell/shell.c`.

Shell prompt interface is contained in `shell/shell_prompt.c`

Shell cmd parser is contained in `shell/shell_parser.c`
--- ---
## Interface
- [x] *Prompt*
- [x] *Single quote*
- [x] *Double quote*
- [x] *Pipelining commands*
- [ ] *Environment variables*
- [ ] *$? last exit status*
- [ ] *Handle control signals (ctrl)*
- [x] *Handle redirections*
- [x] *Ignore \ or ;*

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
- [x] *Prompt parser*

## Miscellaneous
Feel free to write something here.

## Bugs
check var not starting with number


## Current
### Parser
- [x] pipe stacking
- [x] command parsing
- [x] parameter parsing
- [x] redirect parsing
- [x] Quote parsing
- [x] $ Expanding
## Notes
Parse redirect files, last one put into command is only to consider.

$(command) redirects output of command to bash or to command input

$var expands var and redirects to bash or command input

Zsh (default terminal in 42) behaves differently than bash regarding redirections

Redirection parsing example 
```python
def parse_r(prompt):
         rrout = []
         rout = []
         rin = []
         rrdel = []
         for op in ['<<', '>>', '<', '>']:
             tmp = prompt
             while(tmp.find(op) != -1):
                 tmp = tmp[tmp.find(op)+len(op):]
                 tmp = tmp.lstrip(' ')
                 idx = 0
                 tmpstr=""
                 while(idx < len(tmp) and tmp[idx] not in [' ','<','>','|']):
                     tmpstr += tmp[idx]
                     idx += 1
                 if op == '<': rin.append(tmpstr)
                 elif op == '>': rout.append(tmpstr)
                 elif op == '<<': rrdel.append(tmpstr)
                 elif op == '>>': rrout.append(tmpstr)
             prompt = prompt.replace(op,'')
         return (rrout, rout, rin, rrdel)
```

Parsing args and redirections (prob better approach)

```python
def parse_spec(prompt):
          rrout = []
          rout = []
          rin = []
          rrdel = []
          param = []
          tmp = prompt
          idx = 0
          tmpstr = ""
          while(idx < len(tmp)):
              if(tmp[idx:].find(">>") == 0 or tmp[idx:].find("<<") == 0
              or tmp[idx] in ['<','>']):
                  if(tmp[idx:].find(">>") == 0): op = ">>"
                  elif(tmp[idx:].find("<<") == 0): op = "<<"
                  else: op = tmp[idx]
                  idx += len(op);
                  while(tmp[idx] == ' '): idx += 1
                  tmpstr = ""
                  while(idx < len(tmp) and tmp[idx] not in [' ','<','>','|']):
                      tmpstr += tmp[idx]
                      idx += 1
                  if op == '<': rin.append(tmpstr)
                  elif op == '>': rout.append(tmpstr)
                  elif op == '<<': rrdel.append(tmpstr)
                  elif op == '>>': rrout.append(tmpstr)
                  tmpstr=""
                  continue
              elif tmp[idx] == ' ':
                  if(tmpstr != ''):
                      param.append(tmpstr)
                  tmpstr=""
              else:
                  tmpstr+=tmp[idx]
              idx += 1
          if(tmpstr != ''): param.append(tmpstr)
          return (rrout, rout, rin, rrdel,param)

```
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