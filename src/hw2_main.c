#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 

int checkmyCopy(char *copy);
int checkmyPaste(char *paste);
int checkmyMessaage(char *message);


int main(int argc, char **argv) {
    char *input_file = NULL;
    char *output_file = NULL;
    char *copy_arg = NULL;
    char *paste_arg = NULL;
    char *message_arg = NULL;
    FILE *ip, *op;
    int repeatedoutflag = 0, repeatedinflag = 0, repeatcopyflag = 0, 
    repeatpastflag = 0, repeatmessageflag =0, unrecflag = 0; 

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {

        if ((strcmp(argv[i], "-i") == 0) && (i + 1 < argc)                              // checks for -i
        && (strstr(argv[i + 1], ".ppm") != NULL || strstr(argv[i + 1], ".sbu") != NULL)) {  // check if it is a file

            input_file = argv[++i];
            repeatedinflag++;
        } else if ((strcmp(argv[i], "-o") == 0) && (i + 1 < argc)                       //checks for -o
        && (strstr(argv[i + 1], ".ppm") != NULL || strstr(argv[i + 1], ".sbu") != NULL)) {  // check if it is a file

            output_file = argv[++i];
            repeatedoutflag++;

        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            copy_arg = argv[++i];
            repeatcopyflag++;

        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            paste_arg = argv[++i];
            repeatpastflag++;

        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            repeatmessageflag++;
            message_arg = argv[++i];

        }
        else
            unrecflag++;               //UNRECOGNIZED_ARGUMENT
    }

    printf("I  %s\n", input_file);
    printf("O  %s\n", output_file);
    printf("P %s\n", paste_arg);
    printf("R %s\n", message_arg);
    printf("Rindex %d\n", repeatmessageflag);


    if((input_file == NULL) | (output_file == NULL) 
    | (repeatcopyflag != 0 && checkmyCopy(copy_arg) == 1)
    | (repeatpastflag != 0 && checkmyCopy(paste_arg) == 1)
    | (repeatmessageflag != 0 && checkmyCopy(message_arg) == 1) 
    ){                               //mising args
        return MISSING_ARGUMENT;
    }
    else if (unrecflag != 0)
        return UNRECOGNIZED_ARGUMENT;

    else if((repeatedinflag > 1) | (repeatedoutflag > 1) | (repeatcopyflag > 1) 
    | (repeatpastflag > 1) |(repeatmessageflag > 1))    //DUPLICATE_ARGUMENT
        return DUPLICATE_ARGUMENT;

    else if ((ip = fopen(input_file, "r")) == NULL)          //INPUT_FILE_MISSING
        return INPUT_FILE_MISSING;

    else if ((op = fopen(output_file, "w")) == NULL)         //OUTPUT_FILE_UNWRITABLE
        return OUTPUT_FILE_UNWRITABLE;

    else if((repeatpastflag == 1) && (repeatcopyflag == 0))
        return C_ARGUMENT_MISSING;

    else if(repeatcopyflag != 0 && checkmyCopy(copy_arg) == 2)           // c arg invalid 
        return C_ARGUMENT_INVALID;
    
    else if(repeatpastflag != 0 && checkmyPaste(paste_arg) == 2)          // p arg invalid 
        return P_ARGUMENT_INVALID;
    
    else if(repeatmessageflag != 0 && checkmyMessaage(message_arg) == 2)         //r arg invalid
        return R_ARGUMENT_INVALID;
    


    return 0;
}


int checkmyCopy(char *copy){        // error handling
    int commaCount = 0;
    int numberCount = 0;

    for (int i = 0; copy[i] != '\0'; i++) {
        if (copy[i] == ',') {
            commaCount++;
        } 
        else if (copy[i] >= '0' && copy[i] <= '9') {
            while (copy[i] >= '0' && copy[i] <= '9') {
                i++;
            }
            numberCount++;
            i--;
        }
    }

    if(numberCount < 1)
        return 1;                               //missing arg
    else if((numberCount != 4) | (commaCount != 3))
        return 2;                               //invalid carg
    else
        return 0;
}

int checkmyPaste(char *paste){              //error handling
    int commaCount = 0;
    int numberCount = 0;

    for (int i = 0; paste[i] != '\0'; i++) {
        if (paste[i] == ',') {
            commaCount++;
        } 
        else if (paste[i] >= '0' && paste[i] <= '9') {
            while (paste[i] >= '0' && paste[i] <= '9') {
                i++;
            }
            numberCount++;
            i--;
        }
    }
    if(numberCount < 1)
        return 1;                               //missing arg
    else if((numberCount != 2) | (commaCount != 1))
        return 2;                               //invalid carg
    else
        return 0;
}

int checkmyMessaage(char *message){
    int commaCount = 0;
    int numberCount = 0;
    //int validformater = 0;

    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] == ',') {                  //checks commas
            commaCount++;
        } 
        else if (message[i] >= '0' && message[i] <= '9') {          //checks how many nums
            while (message[i] >= '0' && message[i] <= '9') {
                i++;
            }
            numberCount++;
            i--;
        }
    }

    if(numberCount < 1)
        return 1;                               //missing arg
    else if((numberCount != 3) | (commaCount != 4))
        return 2;                               //invalid carg
    else
        return 0;

}



