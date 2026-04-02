/*
 * dashboard.c
 *
 * TODO: Provide a high-level description of what is contained
 * in this file.
 *
 * Author: <TODO: Group Member Names>
 * Lab instructor: <TODO: Your lab instructor's name here>
 * Lecture instructor: <TODO: Your lecture instructor's name here>
*/

//You may include other original headers as you see fit
#include "DB.h"
#include "DB_impl.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    printf("Testing DB_create...\n");
    DB_create();

    importDB("PicnicTable.csv");
    exportDB("Output.csv");

     

    




    return 0;
}
