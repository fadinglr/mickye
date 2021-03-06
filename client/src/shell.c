#include "shell.h"

#define SHELL_SHELL "/bin/bash"
#define SHELL_CMD ((char *[]){SHELL_SHELL, NULL})

/**
 *  Open a shell and get a Shell object
 */
Shell
shell_open()
{
    Shell shell;

    /**
     * File descriptors works like:
     *  - Index 0 : Reading
     *  - Index 1 : Writing
     */
    int parent2child_fd[2];
    int child2parent_fd[2];

    /* First, create a pipe and a pair of file descriptors for its both ends */
    pipe(parent2child_fd);
    pipe(child2parent_fd);

    /* Now fork in order to create process from we'll read from */
    shell.pid = fork();
    if (shell.pid == 0)
    {
        /* Child process */

        /**
         * Close child's writing and parent's reading because we don't need them
         * This apply only in this context (which is the child thread)
         */
        close(parent2child_fd[1]);
        close(child2parent_fd[0]);

        /**
         * Bind process STDIN and STDOUT to useful file descriptors used by the
         * parent's
         */
        dup2(parent2child_fd[0], STDIN_FILENO);
        dup2(child2parent_fd[1], STDOUT_FILENO);
        dup2(child2parent_fd[1], STDERR_FILENO);

        /* Execute command via shell - this will replace current process */
        execv(SHELL_SHELL, SHELL_CMD);

        /* It's useless to return it but the compiler will like it */
        return (Shell){0};
    }
    else
    {
        /* Parent */

        /* Close "write" end, not needed in this process */
        close(parent2child_fd[0]);
        close(child2parent_fd[1]);

        shell.stdin = parent2child_fd[1];
        shell.stdout = child2parent_fd[0];

        shell.fstdin = fdopen(shell.stdin, "w");
        shell.fstdout = fdopen(shell.stdout, "r");

        return shell;
    }
}

/**
 *  Close a shell
 */
void
shell_close(Shell shell)
{
    kill(shell.pid, SIGKILL);
    close(shell.stdin);
    close(shell.stdout);
}
