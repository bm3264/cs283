1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  fork allows the parent process (shell) to continue running while the child executes the command. Without fork, execvp would replace the shell itself, preventing further command execution.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() fails, it returns -1. My implementation checks this and prints an error message using perror("fork"), preventing the shell from executing an invalid process.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() searches for the executable in directories listed in the PATH environment variable.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  wait() ensures the parent process waits for the child to finish, preventing zombie processes. Without it, the child process would remain in a "defunct" state until the system cleans it up.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS(status) extracts the exit code of the child process. It helps determine if the command executed successfully or encountered an error.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  It treats quoted strings as single arguments, preserving spaces within them. This is necessary to correctly parse commands like echo "hello world" as one argument instead of two.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I removed pipe handling and refactored the command parsing to focus on a single command per line. Handling quoted arguments required additional logic to maintain correct argument structure.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals allow processes to asynchronously communicate, unlike pipes or shared memory, which require explicit message passing.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**: 
        SIGKILL (9) – Immediately terminates a process, cannot be caught or ignored.
        SIGTERM (15) – Politely asks a process to terminate, allowing cleanup.
        SIGINT (2) – Interrupts a process (e.g., Ctrl+C in a terminal).

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**: SIGSTOP pauses a process immediately and cannot be caught or ignored, as it is handled by the kernel to ensure reliable process suspension.