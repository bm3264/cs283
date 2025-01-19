#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len) {
    //TODO: #4:  Implement the setup buff as per the directions
    char *src = user_str;
    char *dest = buff;
    int count = 0;
    int space_added = 0;

    // Traverse user string and copy characters
    while (*src != '\0') {
        if (*src == ' ' || *src == '\t') {
            // Add a single space if no previous space was added
            if (!space_added) {
                if (count < len) {
                    *dest++ = ' ';
                    count++;
                    space_added = 1;
                } else {
                    return -1;  // Error: User string exceeds buffer size
                }
            }
        } else {
            if (count < len) {
                *dest++ = *src;
                count++;
                space_added = 0;
            } else {
                return -1;  // Error: User string exceeds buffer size
            }
        }
        src++;
    }

    // Fill remaining buffer with dots
    while (count < len) {
        *dest++ = '.';
        count++;
    }

    return (dest - buff);  // Return number of characters processed
}


void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len) {
    //YOU MUST IMPLEMENT
    int count = 0;
    int in_word = 0;
    char *ptr = buff;

    for (int i = 0; i < str_len; i++, ptr++) {
        if (*ptr != ' ') {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }

    return count;
}


//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
void reverse_string(char *buff, int str_len) {
    char *start = buff;
    char *end = buff + str_len - 1;
    char temp;

    while (start < end) {
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void print_words(char *buff, int str_len) {
    int word_count = 0;
    char *ptr = buff;
    char *word_start = NULL;
    int word_length = 0;

    printf("Word Print\n----------\n");

    for (int i = 0; i < str_len; i++, ptr++) {
        if (*ptr != ' ') {
            if (word_start == NULL) {
                word_start = ptr;
                word_count++;
            }
            word_length++;
        } else if (word_start) {
            printf("%d. ", word_count);
            while (word_start < ptr) {
                putchar(*word_start++);
            }
            printf(" (%d)\n", word_length);
            word_start = NULL;
            word_length = 0;
        }
    }

    if (word_start) {
        printf("%d. ", ++word_count);
        while (word_start < ptr) {
            putchar(*word_start++);
        }
        printf(" (%d)\n", word_length);
    }

    printf("\nNumber of words returned: %d\n", word_count);
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      This conditon checks if no flag is provided, preventing access to argv[1]
    //      This also ensures that the flag starts with '-'
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      The below code ensures that a string argument is provided after the flag,
    //      preventing faults due to missing inputs
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    
    // Allocate space for the buffer using malloc
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));

    // Check if allocation failed and exit with code 99
    if (buff == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(99);
    }



    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt) {
    case 'c':
        rc = count_words(buff, BUFFER_SZ, user_str_len);
        if (rc < 0) {
            printf("Error counting words, rc = %d\n", rc);
            exit(2);
        }
        printf("Word Count: %d\n", rc);
        break;
    
    case 'r':
        reverse_string(buff, user_str_len);
        printf("Reversed String: ");
        for (int i = 0; i < user_str_len; i++) {
            putchar(*(buff + i));
        }
        putchar('\n');
        break;

    case 'w':
        print_words(buff, user_str_len);
        break;

    default:
        usage(argv[0]);
        exit(1);
}

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          Providing both pointer and length ensures a few things.
//          Saftey through preventing buffer overflows, flexibility so functions can process only the important parts
//          of the buffer, and efficiency so that functions don't rely on null-terminated strings