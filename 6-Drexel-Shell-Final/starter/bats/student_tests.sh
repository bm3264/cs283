#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Server starts and accepts connections" {
    ./dsh -s -i 127.0.0.1 -p 4567 &  # Start the server in the background
    sleep 1  # Allow time for server to start
    run ./dsh -c -i 127.0.0.1 -p 4567 <<EOF
exit
EOF
    [ "$status" -eq 0 ]  # Client should exit normally
    pkill -f "./dsh -s -i 127.0.0.1 -p 4567"  # Stop server
}

# Test if the server can execute a simple command
@test "Client executes 'ls' remotely" {
    ./dsh -s -i 127.0.0.1 -p 4568 &  # Start server
    sleep 1
    run ./dsh -c -i 127.0.0.1 -p 4568 <<EOF
ls
exit
EOF
    [[ "$output" == *"dsh4>"* ]]  # Ensure the prompt appears
    [[ "$output" == *"dsh_cli.c"* ]]  # Check that ls output includes a known file
    pkill -f "./dsh -s -i 127.0.0.1 -p 4568"
}

# Test command with arguments
@test "Client executes 'echo Hello World'" {
    ./dsh -s -i 127.0.0.1 -p 4569 &  
    sleep 1
    run ./dsh -c -i 127.0.0.1 -p 4569 <<EOF
echo Hello World
exit
EOF
    [[ "$output" == *"Hello World"* ]]
    pkill -f "./dsh -s -i 127.0.0.1 -p 4569"
}

# Test piped execution remotely
@test "Client executes 'ls | grep .c'" {
    ./dsh -s -i 127.0.0.1 -p 4570 &  
    sleep 1
    run ./dsh -c -i 127.0.0.1 -p 4570 <<EOF
ls | grep .c
exit
EOF
    [[ "$output" == *".c"* ]]
    pkill -f "./dsh -s -i 127.0.0.1 -p 4570"
}

# Test 'cd' built-in command remotely
@test "Client executes 'cd' command" {
    ./dsh -s -i 127.0.0.1 -p 4571 &
    sleep 1
    run ./dsh -c -i 127.0.0.1 -p 4571 <<EOF
cd /
pwd
exit
EOF
    [[ "$output" == *"/"* ]]
    pkill -f "./dsh -s -i 127.0.0.1 -p 4571"
}

# Test 'exit' command remotely
@test "Client exits normally" {
    ./dsh -s -i 127.0.0.1 -p 4572 &
    sleep 1
    run ./dsh -c -i 127.0.0.1 -p 4572 <<EOF
exit
EOF
    [ "$status" -eq 0 ]
    pkill -f "./dsh -s -i 127.0.0.1 -p 4572"
}

# Test 'stop-server' command
@test "Client stops server with 'stop-server'" {
    ./dsh -s -i 127.0.0.1 -p 4573 &
    sleep 1
    run ./dsh -c -i 127.0.0.1 -p 4573 <<EOF
stop-server
EOF
    sleep 1
    ! lsof -i :4573  # Server should not be running anymore
}

# Test invalid command handling
@test "Client handles invalid command" {
    ./dsh -s -i 127.0.0.1 -p 4574 &
    sleep 1
    run ./dsh -c -i 127.0.0.1 -p 4574 <<EOF
invalidcmd
exit
EOF
    [[ "$output" == *"command not found"* ]]
    pkill -f "./dsh -s -i 127.0.0.1 -p 4574"
}

# Test handling of server disconnect
@test "Client handles disconnected server" {
    ./dsh -s -i 127.0.0.1 -p 4575 &
    sleep 1
    pkill -f "./dsh -s -i 127.0.0.1 -p 4575"  # Kill server
    run ./dsh -c -i 127.0.0.1 -p 4575 <<EOF
ls
EOF
    [[ "$output" == *"server appeared to terminate"* ]]
}