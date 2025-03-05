#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suite in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Simple pipeline: ls | wc -l" {
    run ./dsh <<EOF
ls | wc -l
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ -n "$output" ]  # Ensure output is not empty
}

@test "Pipeline with grep: ls | grep .c" {
    run ./dsh <<EOF
ls | grep .c
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *".c"* ]]  # Check that output contains .c files
}

@test "Multiple piped commands: ls | sort | uniq" {
    run ./dsh <<EOF
ls | sort | uniq
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ -n "$output" ]  # Ensure output is not empty
}

@test "Handle non-existent command in pipeline" {
    run ./dsh <<EOF
ls | nonexistentcommand | wc -l
EOF

    # Assertions
    [ "$status" -ne 0 ]  # Should fail due to nonexistent command
}

@test "Pipeline with echo and cut" {
    run ./dsh <<EOF
echo "hello world" | cut -d' ' -f1
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "$output" = "hello" ]  # Ensure only "hello" is printed
}

@test "Pipelining with echo and tr (uppercase transformation)" {
    run ./dsh <<EOF
echo "hello" | tr 'a-z' 'A-Z'
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "$output" = "HELLO" ]  # Ensure transformation works
}

@test "Ensure correct handling of empty pipeline: | ls" {
    run ./dsh <<EOF
| ls
EOF

    # Assertions
    [ "$status" -ne 0 ]  # Should fail due to invalid syntax
}

@test "Exit command should terminate shell" {
    run ./dsh <<EOF
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"exiting..."* ]]
}