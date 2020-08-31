#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BLOCKSIZE 512

bool isJpgHeader(uint8_t buffer[]) //make a function rather that read into everytime.
{
    return buffer[0] == 0xff
        && buffer[1] == 0xd8
        && buffer[2] == 0xff
        && (buffer[3] & 0xf0) == 0xe0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) //check user input
    {
        printf("Usage ./recover image\n");
        return 1;
    }

    char* inputFile = argv[1]; //need a pointer to argv 1
    if(inputFile == NULL) //always check to see if Null
    {
        printf("Usage ./recover image\n");
        return 1;
    }

    FILE* inputPtr = fopen(inputFile, "r"); //point to a file and open. the input file we need.
    if(inputPtr == NULL) //check values
    {
        printf("Unable to open file: %s\n", inputFile);
        return 1;
    }

    char filename[8]; //.jpg + '\0' thats why 8.
    FILE* outputPtr = NULL; //need an output pointer
    uint8_t buffer[BLOCKSIZE]; //use file to read into buffer. use buffer to read into ouput file.
    int jpgCounter = 0; //where to write the jpg size

    while(fread(buffer, sizeof(uint8_t), BLOCKSIZE, inputPtr) || feof(inputPtr) == 0)
    { //while we read from input file, and while input file is not the last one..
        if(isJpgHeader(buffer)) //if we read a jpg header
        {
            if(outputPtr != NULL)
            {
                fclose(outputPtr);
            }
            sprintf(filename, "%03i.jpg", jpgCounter); //create a file name. % because we need a 3 digit number.
            outputPtr = fopen(filename, "w");
            jpgCounter++; //keep count, so we know what to document
        }
        if(outputPtr != NULL) //if we find an output file we want to write to it..
        {
            fwrite(buffer, sizeof(buffer), 1, outputPtr); //from buffer, size of, move 1 size, write to output file
        }
    }

    if(inputPtr == NULL) //anything we create we have to close.
    {
        fclose(inputPtr);
    }

    if(outputPtr == NULL)
    {
        fclose(outputPtr);
    }

    return 0;
}
