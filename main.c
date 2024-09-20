#include "shell/shell.h"

int main(int argc, char **argv, char **envp) {
    minishell_boot(envp);
    return 0;
}