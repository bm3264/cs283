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

@test "Built-in: cd with no args should not change directory" {
    run ./dsh <<EOF
pwd
cd
pwd
exit
EOF

    # Extract both pwd outputs
    pwd1=$(echo "$output" | grep -m 1 "^/" | head -n1)
    pwd2=$(echo "$output" | grep -m 2 "^/" | tail -n1)

    [ "$pwd1" = "$pwd2" ]
}

@test "Built-in: cd changes to subdirectory" {
    mkdir -p temp_test_dir
    run ./dsh <<EOF
cd temp_test_dir
pwd
exit
EOF

    # Check that we're in temp_test_dir
    echo "$output" | grep "temp_test_dir"
    rmdir temp_test_dir
}

@test "External: invalid command returns error" {
    run ./dsh <<EOF
thiscommanddoesnotexist
exit
EOF

    # Should include execvp error message
    echo "$output" | grep -i "execvp"
}

@test "External: echo with quoted string" {
    run ./dsh <<EOF
echo "hello   world"
exit
EOF

    echo "$output" | grep "hello   world"
}

@test "Built-in: exit command terminates shell" {
    run ./dsh <<EOF
exit
EOF

    [ "$status" -eq 0 ]
}
