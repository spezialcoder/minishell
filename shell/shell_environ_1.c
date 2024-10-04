#include "shell.h"

void update_shell_environ(t_shell *sc) {
    if(sc->envp != NULL)
        free_split_array(sc->envp);
    sc->envp = environ_to_array(&sc->environ);
}