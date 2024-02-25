#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 


int main(int argc, char **argv) {
    char *input_file = NULL;
    char *output_file = NULL;
    char *copy_arg = NULL;
    char *paste_arg = NULL;
    char *message_arg = NULL;
    //FILE *ip, *op;
    int repeatedoutflag = 0, repeatedinflag = 0, repeatcopyflag = 0, 
    repeatpastflag = 0, repeatmessageflag =0; 

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            input_file = argv[++i];
            repeatedinflag++;

        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
            printf("how mant runs.  %s \n", output_file);
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
    }

    if((input_file == NULL) | (output_file == NULL))    //MISSING_ARGUMENT
        return MISSING_ARGUMENT;
    //UNRECOGNIZED_ARGUMENT
    if((input_file != NULL && repeatedinflag != 1) | (output_file != NULL && repeatedoutflag != 1) |
    (copy_arg != NULL && repeatcopyflag != 1) | (paste_arg != NULL && repeatpastflag != 1) |
    (message_arg != NULL && repeatmessageflag != 1))    //DUPLICATE_ARGUMENT
        return DUPLICATE_ARGUMENT;
    //if ((ip = fopen(input_file, "r")) == NULL)          //INPUT_FILE_MISSING
    //    return INPUT_FILE_MISSING;
    //if ((op = fopen(output_file, "w")) == NULL)         //OUTPUT_FILE_UNWRITABLE
    //    return OUTPUT_FILE_UNWRITABLE;
    if((repeatpastflag == 1) && (repeatcopyflag == 0))
        return C_ARGUMENT_MISSING; 
    // C_ARGUMENT_INVALID
    //P_ARGUMENT_INVALID
    //R_ARGUMENT_INVALID

    return 0;
}

    