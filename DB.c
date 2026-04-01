/* DB.c
 *
 * TODO: Provide a high-level description of what is contained
 * in this file.
 *
 * Author: <TODO: Group Member Names>
 * Lab instructor: <TODO: Your lab instructor's name here>
 * Lecture instructor: <TODO: Your lecture instructor's name here>
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "DB.h"       /* Import the public database header. */
#include "DB_impl.h"  /* Import the private database header */

DataBase *Db;


void DB_create() {
    //Error checking for malloc of Db.
    Db = malloc(sizeof(DataBase));
    if (Db == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    //Allocating the memory for general tables.

    Db->tableTypeTable = malloc(sizeof(Table));
    Db->surfaceMaterialTable = malloc(sizeof(Table));
    Db->structuralMaterialTable = malloc(sizeof(Table));
    Db->neighborhoodTable = malloc(sizeof(NeighbourhoodTable));
    Db->picnicTableTable = malloc(sizeof(PicnicTable));

    //Allocating memeory for internal arrays of the tables.

    Db->tableTypeTable->types = malloc(INIT_SIZE * sizeof(char *));
    Db->tableTypeTable->ids = malloc(INIT_SIZE * sizeof(int));

    Db->surfaceMaterialTable->types = malloc(INIT_SIZE * sizeof(char *));
    Db->surfaceMaterialTable->ids = malloc(INIT_SIZE * sizeof(int));

    Db->structuralMaterialTable->types = malloc(INIT_SIZE * sizeof(char *));
    Db->structuralMaterialTable->ids = malloc(INIT_SIZE * sizeof(int));

    Db->neighborhoodTable->names = malloc(INIT_SIZE * sizeof(char *));
    Db->neighborhoodTable->ids = malloc(INIT_SIZE * sizeof(int));

    Db->picnicTableTable->entries = malloc(INIT_SIZE * sizeof(PicnicTableEntry));

    // Initaializing the internal arrays of the table to NULL and 0.
    for (int i = 0; i < INIT_SIZE; i++) {
        Db->tableTypeTable->types[i] = NULL;
        Db->tableTypeTable->ids[i] = 0;

        Db->surfaceMaterialTable->types[i] = NULL;
        Db->surfaceMaterialTable->ids[i] = 0;

        Db->structuralMaterialTable->types[i] = NULL;
        Db->structuralMaterialTable->ids[i] = 0;

        Db->neighborhoodTable->names[i] = NULL;
        Db->neighborhoodTable->ids[i] = 0;

        Db->picnicTableTable->entries[i].tableID = 0;
        Db->picnicTableTable->entries[i].siteID = 0;
        Db->picnicTableTable->entries[i].tableTypeID = 0;
        Db->picnicTableTable->entries[i].surfaceMaterialID = 0;
        Db->picnicTableTable->entries[i].structuralMaterialID = 0;
        Db->picnicTableTable->entries[i].streetAvenue = NULL;
        Db->picnicTableTable->entries[i].neighborhoodID = 0;
        Db->picnicTableTable->entries[i].ward = NULL;
        Db->picnicTableTable->entries[i].latitude = NULL;
        Db->picnicTableTable->entries[i].longitude = NULL;

    }
    // final error checking for general tables.
    if (Db->tableTypeTable == NULL || Db->surfaceMaterialTable == NULL|| 
        Db->structuralMaterialTable == NULL || 
        Db->neighborhoodTable == NULL || Db->picnicTableTable == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
    }


    
}

int countEntries(char *memberName, char * value){
    int count = 0;
    for (int i = 0; i < INIT_SIZE; i++) {
        if (strcmp(memberName, "Table Type") == 0) {
            if (Db->tableTypeTable->types[i] && strcmp(Db->tableTypeTable->types[i], value) == 0) {
                count++;
            }
        }
        else if (strcmp(memberName, "Surface Material") == 0) {
            if (Db->surfaceMaterialTable->types[i] && strcmp(Db->surfaceMaterialTable->types[i], value) == 0) {
                count++;
            }
        }
        else if (strcmp(memberName, "Structural Material") == 0) {
            if (Db->structuralMaterialTable->types[i] && strcmp(Db->structuralMaterialTable->types[i], value) == 0) {
                count++;
            }
        }
        else if (strcmp(memberName, "Neighborhood ID") == 0) {
            if (Db->picnicTableTable->entries[i].neighborhoodID == atoi(value)) {
                count++;
            }
        }

        else if (strcmp(memberName, "Neighbourhood Name") == 0) {
            if (Db->neighborhoodTable->names[i] && strcmp(Db->neighborhoodTable->names[i], value) == 0) {
                count++;
            }
        }

        else if (strcmp(memberName, "Ward") == 0) {
            if (Db->picnicTableTable->entries[i].ward && strcmp(Db->picnicTableTable->entries[i].ward, value) == 0) {
                count++;
            }
        }
        else {
            fprintf(stderr, "INVALID MEMBER.\n");
            exit(EXIT_FAILURE);
        }
    }
    return count;
}

