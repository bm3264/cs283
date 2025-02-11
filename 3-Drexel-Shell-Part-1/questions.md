1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice because it reads inputs safely, reads one full line at a time, handles EOF properly and unlike scanf(), it allows whitespace characters in the input which is important for parsing commands

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  We use malloc() because it allows us to dynamically allocate memory at runtime, adapt to different shell environments and handles user input flexibly. Using a fixed-size array could work for smaller applications but in a big shell like this, dynamic allocation is more scalable and flexible


3. In `dshlib.c`, the function `build_cmd_list()` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  This is necessary because leading spaces can cause the executable name to be empty or misidentified. Trailing spcaes can cause unnecessary arguments or empty strings in the command list, and the extra spaces around pipes might lead to parsing errors. If we don't trim the spaces, " ls " might be stored as " ls" or "ls", causing execution errors.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Redirecting STDOUT to a file could pose a challenge by having to implement file handling in our shell to create/write files
    Redirecting STDERR to a file could pose a challenge by having to handle different file descriptors correctly
    Redirecting both as a file could pose a challenge by having to implement file descriptor merging properly

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection redirects inputs or outputs to/from files, we use this when saving or reading command outputs. Piping passes output of one command as the input to another, this is used when chaining commands dynamically.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Keeping them separate allows scripts to process outputs independently.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Display STDERR separately so users can distinguish errors from regular outputs. Return proper exit codes so scripts using our shell can detect failures