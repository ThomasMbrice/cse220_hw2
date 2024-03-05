#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 

int extractNumber(const char *str);
int checkmyCopy(const char *copy);
int checkmyPaste(const char *paste);
int checkmyMessaage(const char *message);
FILE ppm_to_sbu(int *colorarray, FILE *op, unsigned int len, int width, int length);
FILE sbu_to_ppm(signed char **bigarray, int *bigarraybrother, FILE *op, int length, int width, int colorlen, int super);
void my_Copy(const char* copy_arg, const char* paste_arg, int* colorarray, signed char **bigarray, int length, int width);
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

    else if ((op = fopen(output_file, "w")) == NULL){        //OUTPUT_FILE_UNWRITABLE
        printf("FILE UNWRITABLE \n");
        return OUTPUT_FILE_UNWRITABLE;
    }

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
    signed char **bigarray = NULL;
    int *colorarray = NULL;
    int *bigarraybrother = NULL;
    int index = 0;
    int width = 0, length = 0, max = 255, colorlen = 0;

    if(strstr(input_file, ".ppm") != NULL){         // from ppm       WORKS DO NOT EDIT
        fscanf(ip, "%s %d %d %d", header, &length, &width, &max);
        length *= 3;

        colorarray = malloc((width * length * 32));              //init memory

        for(int i = 0; i < width*length; i++){                 //does allocation
            fscanf(ip," %d", &colorarray[i]);
            //printf("%d ", colorarray[i]);
        }

    
    //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    if(copy_arg != NULL){
        my_Copy(copy_arg, paste_arg, colorarray, NULL, length, width);
    }

    //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        if(strstr(output_file, ".ppm") != NULL){         // to ppm       WORKS DO NOT EDIT
            fprintf(op, "%s\n%d %d\n%d\n", header, length/3, width, max);
            //printf("%s\n%d %d\n%d\n", header, length/3, width, max);
            //printf("\n wrong method \n");
            for (int i = 0; i < width * length; ++i) {
            fprintf(op, "%d ", colorarray[i]);
            //printf("%d ", colorarray[i]);
            }
        }
        else{
            *op = ppm_to_sbu(colorarray, op, (width * length * 32), width, length/3);
        }
        
        free(colorarray);

    }
    else{ 
        fscanf(ip, "%s %d %d %d", header, &length, &width, &colorlen);
        //printf("outlook %s %d %d %d\n", header, length, width, colorlen);
        colorlen *= 3;
        //printf("color len %d\n", colorlen);

        bigarray = malloc((width *length* 8));              //this is repeated

        for(int i = 0; i<width *length; i++){
            bigarray[i] = malloc(16);
        }
        
        if((bigarray == NULL))
            printf("major failure");

        bigarraybrother = (int *)malloc(colorlen * 32);         //colorarray

        for(int e = 0; e < colorlen; e++){
            fscanf(ip,"%d ", &bigarraybrother[e]);
        }   

        //printf("starting onboard %ld \n", sizeof(bigarray));
        while(fscanf(ip, " %s", bigarray[index]) == 1){                 //does allocation also repeated    
            //printf("%s, index :%d \n", bigarray[index], index); 
            index++;            
        }

    //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        if(strstr(output_file, ".sbu") != NULL){         // to sbu       WORKS DO NOT EDIT
            //printf("%c%c%c\n%d %d\n%d ", header[0], header[1], header[2], length, width, colorlen/3);
            fprintf(op, "%c%c%c\n%d %d\n%d ", header[0], header[1], header[2], length, width, colorlen/3);
        
            for(int z = 0; z < colorlen; z++){
                //printf("%d ", bigarraybrother[z]);
                fprintf(op,"%d ", bigarraybrother[z]);                   // colors 
            }        
        
            fprintf(op,"\n");
            //printf("\n");

            for(int i = 0; i < index; i++){
                fprintf(op,"%s ", bigarray[i]);
                //printf("%s ", bigarray[i]);
            }
        }
        else{
            *op = sbu_to_ppm(bigarray, bigarraybrother, op, length, width, colorlen, index);
        }

        free(bigarray);
        free(bigarraybrother);
    }


    fclose(ip);
    fclose(op);
    return 0;
}

FILE ppm_to_sbu(int *colorarray, FILE *op, unsigned int len, int width, int length){
    int *colors_of_SBU = malloc(len);
    int colorlen = 0, zeroifdup = 0;

    for(unsigned int i = 0; i < (len/32); i+=3){
        zeroifdup = 0;                              //assumes not duplicate
        for(unsigned int e = 0; e < i; e+=3){                //checks if duplicate
            if(((colors_of_SBU[e] == colorarray[i]) && (colors_of_SBU[e+1] == colorarray[i+1])
            && (colors_of_SBU[e+2] == colorarray[i+2])) | (i == 0))
            zeroifdup = 1;
        }
        if(zeroifdup == 0){                                     //copies if not duplicate
            colors_of_SBU[colorlen] = colorarray[i];
            colors_of_SBU[++colorlen] = colorarray[i+1];
            colors_of_SBU[++colorlen] = colorarray[i+2];
            colorlen++;
        }
    }

    fprintf(op, "%s\n%d %d\n%d ", "SBU", length, width, colorlen/3);
    //printf("%s\n%d %d\n%d ", "SBU", length, width, colorlen/3);

    for(int z = 0; z < colorlen; z++){
        //printf("%d ", colors_of_SBU[z]);
        fprintf(op,"%d ", colors_of_SBU[z]);                   // copy colors 
    }        
    fprintf(op,"\n");                                           //next copy actualy payload
    //printf("\n");
    
    int *bigarray_fraud = malloc(len);              //this is repeated

    int fruad_indexer = 0, e = 0;

    for(unsigned int i = 0; i< len/32; i+=3){                       //finds index which points to colors
        for(e = 0; e < colorlen; e+= 3){
            if(((colors_of_SBU[e] == colorarray[i]) && (colors_of_SBU[e+1] == colorarray[i+1])
            && (colors_of_SBU[e+2] == colorarray[i+2])) | (i == 0))
            break;
        }
        bigarray_fraud[fruad_indexer++] = (e/3);
        //printf("%d ", bigarray_fraud[fruad_indexer++]);
    }
    //printf("\n fruad %d \n", fruad_indexer);

    int repeater = 0, adder = 0;

    for(int i = 0; i < fruad_indexer; i++){
        if(bigarray_fraud[i] == bigarray_fraud[i+1]){
            repeater = bigarray_fraud[i];
            adder = 1;
            while(repeater == bigarray_fraud[i+adder] && i+adder < fruad_indexer){
                adder++;                                    //error happens here????
            }
            fprintf(op,"*%d %d ", adder, repeater);                   // copy colors 
            //printf("*%d %d ", adder, repeater);
            i+=adder-1;
        }
        else{
            fprintf(op,"%d ", bigarray_fraud[i]);                   // copy colors 
            //printf("%d ", bigarray_fraud[i]);
        }

    }

    free(bigarray_fraud);
    free(colors_of_SBU);

    return *op;
}

FILE sbu_to_ppm(signed char **bigarray, int *bigarraybrother, FILE *op, int length, int width, int colorlen, int super){
    
        fprintf(op, "%s\n%d %d\n%d\n", "P3", length, width, 255);
        printf("%s\n%d %d\n%d\n", "P3", length, width, 255);

        int astrix_multipli = 1, index = 0;                        //to deal with strix

        for(int i = 0; i < super; i++){

            astrix_multipli = 1;
            if(strstr((char *)bigarray[i], "*") != NULL) {          //gets astrix multipler for pixels
                astrix_multipli = extractNumber((char *)bigarray[i]);
                i++;
            }
            index = atoi(((char *)bigarray[i]))*3;              // get index
            //printf("array pos %d  - ", i);
            if(index+2 < colorlen){
                if(astrix_multipli > 1){
                    while(astrix_multipli > 0){
                        fprintf(op,"%d ", bigarraybrother[index]);
                                //printf("%d ", bigarraybrother[index]);
                        fprintf(op,"%d ", bigarraybrother[index+1]);
                                //printf("%d ", bigarraybrother[index+1]);
                        fprintf(op,"%d ", bigarraybrother[index+2]);
                                //printf("%d | ", bigarraybrother[index+2]);
                        astrix_multipli--;
                    }
                }
                else{
                fprintf(op,"%d ", bigarraybrother[index]);
                //                        printf("%d ", bigarraybrother[index]);
                fprintf(op,"%d ", bigarraybrother[index+1]);
                //                         printf("%d ", bigarraybrother[index+1]);
                fprintf(op,"%d ", bigarraybrother[index+2]);
                //                         printf("%d | ", bigarraybrother[index+2]);
                }
            }
            
        }

    return *op;
}

int extractNumber(const char *str) {
    str++;
    int num = atoi(str);
    return num;
}


void my_Copy(const char* copy_arg, const char* paste_arg, int* colorarray, 
             signed char **bigarray, int length, int width) {
    int copyx, copyy, pastex, pastey, copylegnth, copywidth;

    sscanf(copy_arg, "%d,%d,%d,%d", &copyx, &copyy, &copylegnth, &copywidth);
    sscanf(paste_arg, "%d,%d", &pastex, &pastey);

    copyy *= 3;
    pastey *= 3;
    copylegnth *= 3;

    if (bigarray == NULL) {       // for ppm
        int **temparr = malloc(width * sizeof(int*));

        for (int i = 0; i < width; i++) {
            temparr[i] = malloc(length * sizeof(int));
        }
        
        int index = 0;
        for (int i = 0; i < width; i++) {
            for (int e = 0; e < length; e+=3) {
                temparr[i][e] = colorarray[index];
                temparr[i][e+1] = colorarray[index+1];
                temparr[i][e+2] = colorarray[index+2];
                index+=3;
            }
        }

        int **copyarray = malloc(copywidth * sizeof(int*));
        for (int i = 0; i < copywidth; i++) {
            copyarray[i] = malloc(copylegnth * sizeof(int));
        }

        int tempcopy = copyy;
        for (int i = 0; i < copywidth; i++) {
            for (int e = 0; e < copylegnth; e += 3) {
                copyarray[i][e] = temparr[copyx][tempcopy];
                copyarray[i][e+1] = temparr[copyx][tempcopy+1];
                copyarray[i][e+2] = temparr[copyx][tempcopy+2];
                tempcopy += 3;
            }
        copyx++;
        tempcopy = copyy;
        }
        
        for(int i = 0; i < length*width;i+=3){
            printf("%d ", colorarray[i]);
            printf("%d ", colorarray[i+1]);
            printf("%d | ", colorarray[i+2]);
        }

        printf("\n");
        
        int temppastey = pastey;
        for (int i = 0; i < copywidth; i++) {
            for (int e = 0; e < copylegnth; e += 3) {
                temparr[pastex][temppastey] = copyarray[i][e];
                temparr[pastex][temppastey+1] = copyarray[i][e+1];
                temparr[pastex][temppastey+2] = copyarray[i][e+2];
                temppastey += 3;
            }
        pastex++;
        temppastey = pastey;
        }

        index = 0;
        for (int i = 0; i < width; i++) {
            for (int e = 0; e < length; e+=3) {
                colorarray[index] = temparr[i][e];
                colorarray[index+1] = temparr[i][e+1];
                colorarray[index+2] = temparr[i][e+2];
                index += 3;
            }
        }
        
        for(int i = 0; i < length*width;i+=3){
            printf("%d ", colorarray[i]);
            printf("%d ", colorarray[i+1]);
            printf("%d | ", colorarray[i+2]);
        }
        
        free(copyarray);
        free(temparr);
    } else {                       // for SBU
        // TODO
    }
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

/*
void my_Message(int z, unsigned char **bigarray,unsigned int length, unsigned int width, char message_arg){

}
*/
