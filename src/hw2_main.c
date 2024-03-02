#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 

int checkmyCopy(const char *copy);
int checkmyPaste(const char *paste);
int checkmyMessaage(const char *message);
void my_Copy(int z, unsigned char **bigarray, unsigned int length, unsigned int width, const char *copy_arg, const char *paste_arg);
//void my_Message(int zero_if_ppm, char ** bigarray,unsigned int length, unsigned int width, char message_arg);


int main(int argc, char **argv) {
    const char *input_file = NULL;
    const char *output_file = NULL;
    const char *copy_arg = NULL;
    const char *paste_arg = NULL;
    const char *message_arg = NULL;
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
    /*
    printf("I  %s\n", input_file);
    printf("O  %s\n", output_file);
    printf("P %s\n", paste_arg);
    printf("R %s\n", message_arg);
    printf("Rindex %d\n", repeatmessageflag);
    */

    if((input_file == NULL) | (output_file == NULL) 
    | (repeatcopyflag != 0 && checkmyCopy(copy_arg) == 1)
    | (repeatpastflag != 0 && checkmyCopy(paste_arg) == 1)
    | (repeatmessageflag != 0 && checkmyCopy(message_arg) == 1) 
    ){                               //mising args
        return MISSING_ARGUMENT;
    }
    else if (unrecflag != 0){
        return UNRECOGNIZED_ARGUMENT;
    }
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
    

//________________________________________________________________________________________________end error handling


    char header[3] = {'i','i','i'};
    unsigned char *bigarray = NULL;
    unsigned int *colorarray = NULL;  
    int width = 0, length = 0, max = 255, colorlen = 0;

    if(strstr(input_file, ".ppm") != NULL){         // from ppm
        fscanf(ip, "%s %d %d %d", header, &length, &width, &max);
        length *= 3;

        colorarray = malloc(width * length * 32);              //init memory

        for(int i = 0; i < width*length; i++){                 //does allocation
                fscanf(ip," %u", &colorarray[i]);
        }
    }
    else{ 
        fscanf(ip, "%s %d %d %d", header, &length, &width, &colorlen);
        printf("outlook %s %d %d %d", header, length, width, colorlen);
        colorlen *= 3;
        printf("color len %d\n", colorlen);

        colorarray = malloc(colorlen * 32);         //colorarray

        for(int e = 0; e < colorlen; e++){
            fscanf(ip,"%u", &colorarray[e]);
        }   

        bigarray = malloc(width *length* 8);              //this is repeated
        
        if(bigarray == NULL)
            printf("major failure");

        printf("starting onboard %ld \n", sizeof(bigarray));
        int index = 0;
        while(fscanf(ip, " %c", &bigarray[index]) == 1){                 //does allocation also repeated    
            printf("%c, index :%d \n", bigarray[index], index); 
            index++;            
        }
        printf("finished onboard\n");
    }
//________________________________________________________________________________________________ end data copy
  
    
    //if(copy_arg != NULL)
    //    my_Copy(zero_if_ppm, &bigarray, length, width, copy_arg, paste_arg);
    //if(message_arg != NULL)
        //my_Message(zero_if_ppm, &bigarray, length, width, *message_arg);

    //printf("index 0 %u \n", colorarray[0]);
    
    if(strstr(output_file, ".ppm") != NULL){         // from ppm
        //printf("%s\n%d %d\n%d\n", header, length/3, width, max);
        fprintf(op, "%s\n%d %d\n%d\n", header, length/3, width, max);
        for(int i = 0; i < width * length; i++){
                printf("%d ", colorarray[i]);
        }
    }
    else{
        //printf("%s %d %d %d", header, length/2, width, 100);
        fprintf(op, "%s\n%d %d\n %d", header, length/2, width, colorlen);
        printf("printhoe");
        for(int i = 0; i < colorlen; i++){
            printf("%d", colorarray[i]);
            fprintf(op,"%d ", colorarray[i]);                   // colors 
        }        
        
        fprintf(op,"\n");

        for(int i = 0; i < width; i++)
            fprintf(op,"%d ", bigarray[i]);
            
    }


    free(bigarray);
    free(colorarray);
    fclose(ip);
    fclose(op);
    return 0;
}


void my_Copy(int z, unsigned char **bigarray, unsigned int length, unsigned int width, const char *copy_arg, const char *paste_arg){

    unsigned int *paste_cordx = 0, *paste_cordy = 0, *copy_cordx = 0, *copy_cordy = 0, *copylen = 0, *copyheight = 0;
     if(sscanf(paste_arg, "%u,%u", paste_cordx, paste_cordy) == 2)
     printf("show me printer %u %u\n" , *paste_cordx, *paste_cordy);

    if(sscanf(copy_arg, "%u,%u,%u,%u", copy_cordx,copy_cordy,copylen,copyheight) == 4)
         printf("show me copy %u %u\n", *copy_cordx, *copy_cordy);
    
    (void)z;
    (void)**bigarray;
    (void)length;
    (void)width;

}


void BacktoNums(){

}



int checkmyCopy(const char *copy){        // error handling
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

int checkmyPaste(const char *paste){              //error handling
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

int checkmyMessaage(const char *message){         //error check
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

/*
void my_Message(int z, unsigned char **bigarray,unsigned int length, unsigned int width, char message_arg){

}
*/
