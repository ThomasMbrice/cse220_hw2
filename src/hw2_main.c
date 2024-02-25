#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 

int findmyargs(char *input, char *output, char *copy, char *paste, char *message);

int main(int argc, char **argv) {
    char *input_file = NULL;
    char *output_file = NULL;
    char *copy_arg = NULL;
    char *paste_arg = NULL;
    char *message_arg = NULL;
    FILE *ip, *op;
    int repeatedoutflag = 0, repeatedinflag = 0, repeatcopyflag = 0, 
    repeatpastflag = 0, repeatmessageflag =0; 

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            input_file = argv[++i];
            repeatedinflag++;

        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
            repeatedoutflag++;

        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            copy_arg = argv[++i];
            repeatcopyflag++;

        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            paste_arg = argv[++i];
            repeatpastflag++;

        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            message_arg = argv[++i];
            repeatmessageflag++;

        }
        else
            return UNRECOGNIZED_ARGUMENT;               //UNRECOGNIZED_ARGUMENT
    }


    if((repeatedinflag > 1) | (repeatedoutflag > 1) | (repeatcopyflag > 1) 
    | (repeatpastflag > 1) |(repeatmessageflag > 1))    //DUPLICATE_ARGUMENT
        return DUPLICATE_ARGUMENT;

    else if ((ip = fopen(input_file, "r")) == NULL)          //INPUT_FILE_MISSING
        return INPUT_FILE_MISSING;

    else if ((op = fopen(output_file, "w")) == NULL)         //OUTPUT_FILE_UNWRITABLE
        return OUTPUT_FILE_UNWRITABLE;

    else if((repeatpastflag == 1) && (repeatcopyflag == 0))
        return C_ARGUMENT_MISSING; 


    int checker = findmyargs(input_file, output_file, copy_arg, paste_arg, message_arg);
    if(checker == 1){               //mising args
        return MISSING_ARGUMENT;
    }

    // other methods will be inserted here after debugging

    else if(checker == 2){          // c arg invalid 
        return C_ARGUMENT_INVALID;
    }
    else if(checker == 3){          // p arg invalid 
        return P_ARGUMENT_INVALID;
    }
    else if(checker == 4){          //r arg invalid
        return R_ARGUMENT_INVALID;
    }
    


    return 0;
}

int findmyargs(char *input, char *output, char *copy, char *paste, char *message){
    if((input == NULL) | (output == NULL)){
        return 1;
        printf("input fail %s\n", input);
        printf("output fail %s\n", output);

    }
        printf("extra fail %s\n", copy);
        printf("extra fail %s\n", paste);
        printf("extra fail %s\n", message);

    return 0;
}



/*
int temp = 0;
            for(unsigned long e = 0; e < strlen(copy_arg); e++){        //checks copyarg is valid
                if(strcmp(&paste_arg[e], ",") == 0){
                    temp++;
                    if(!isdigit(&paste_arg[e+1])){
                        temp = -1;
                        break;
                    }
                }
            }
            if(temp != 1)                                        // causes error to throw later
                copy_arg = NULL;
*/
