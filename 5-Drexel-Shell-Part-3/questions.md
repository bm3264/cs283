1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation stores all child process PIDs in an array and calls waitpid() on each of them after forking. This ensures that the shell waits for all child processes to complete before continuing to prompt for new input. If we forgot to call waitpid(), the child processes would become zombie processes (defunct processes) because they would finish execution but remain in the process table until explicitly reaped by the parent.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necassary to close them to prevent descriptor leaks. If the pipes are left open, then processes might hang indefinitely because read() operations will not receive an EOF. Also the system might run out of file descriptors which would cause errors in the process.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

Unlike external commands that run in a child process, cd needs to change the shell’s working directory, which must happen inside the shell process itself.
If cd were an external process, the child process would change its directory but the parent (shell) would remain in the same directory, making cd ineffective.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To support an unlimited number of piped commands, we would need to dynamically allocate pipe file descriptors instead of using a fixed size array, use realloc() to resize command storage as needed, and manage memory effciently by freeing allocated resources after execution.

The trade offs with that is an increased amount of complexity due to everything needing to be done dynamically, performance issues due to creating and closing so many pipes, and potential operating system limits.