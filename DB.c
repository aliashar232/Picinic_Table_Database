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
            if (strcmp(value, "Other Table") == 0) {
                if (Db->picnicTableTable->entries[i].tableTypeID == 0) {
                    count++;
                }
            }
            else if (strcmp(value, "Square Picnic Table") == 0) {
                if (Db->picnicTableTable->entries[i].tableTypeID == 1) {
                    count++;
                }
            }
            else if (strcmp(value, "Round Picnic Table") == 0) {
                if (Db->picnicTableTable->entries[i].tableTypeID == 2) {
                    count++;
                }
            }
                
                
            }
        
        else if (strcmp(memberName, "Surface Material") == 0) {
            if (strcmp(value, "Metal") == 0) {
                if (Db->picnicTableTable->entries[i].surfaceMaterialID == 0) {
                    count++;
                }
            }
            else if (strcmp(value, "Unknown") == 0) {
                if (Db->picnicTableTable->entries[i].surfaceMaterialID == 1) {
                    count++;
                }
            }
            else if (strcmp(value, "Wood") == 0) {
                if (Db->picnicTableTable->entries[i].surfaceMaterialID == 2) {
                    count++;
                }
            }
        }
        else if (strcmp(memberName, "Structural Material") == 0) {
            if (strcmp(value, "Aggregate") == 0) {
                if (Db->picnicTableTable->entries[i].structuralMaterialID == 0) {
                    count++;
                }
            }
            else if (strcmp(value, "Metal") == 0) {
                if (Db->picnicTableTable->entries[i].structuralMaterialID == 1) {
                    count++;
                }
            }
            else if (strcmp(value, "Unknown") == 0) {
                if (Db->picnicTableTable->entries[i].structuralMaterialID == 2) {
                    count++;
                }
            }
            else if (strcmp(value, "Wood") == 0) {
                if (Db->picnicTableTable->entries[i].structuralMaterialID == 3) {
                    count++;
                }
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

void importDB(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error, could not open file");
        exit(EXIT_FAILURE);
    }

    int index = 0;

    char line[256];
    fgets(line, sizeof(line), fp); // header skipped
    while (fgets(line, sizeof(line), fp) && index < INIT_SIZE) {
        char *token;
        //ID is Parsed
        token = strtok(line, ",");
        Db->picnicTableTable->entries[index].tableID = atoi(token);

        //Table Type ID is parsed
        token = strtok(NULL, ",");
        if (strcmp(token, "Other Table") == 0) {
            Db->picnicTableTable->entries[index].tableTypeID = 0;
        }
        else if (strcmp(token, "Square Picnic Table") == 0) {
            Db->picnicTableTable->entries[index].tableTypeID = 1;
        }
        else if (strcmp(token, "Round Picnic Table") == 0) {
            Db->picnicTableTable->entries[index].tableTypeID = 2;
        }
    
        // Surface Material ID is parsed
        token = strtok(NULL, ",");
        if (strcmp(token, "Metal") == 0) {
            Db->picnicTableTable->entries[index].surfaceMaterialID = 0;
        }
        else if (strcmp(token, "Unknown") == 0) {
            Db->picnicTableTable->entries[index].surfaceMaterialID = 1;
        }
        else if (strcmp(token, "Wood") == 0) {
            Db->picnicTableTable->entries[index].surfaceMaterialID = 2;
        }


        // Structural Material ID is parsed
        token = strtok(NULL, ",");
        if (strcmp(token, "Aggregate") == 0) {
            Db->picnicTableTable->entries[index].structuralMaterialID = 0;
        }
        else if (strcmp(token, "Metal") == 0) {
            Db->picnicTableTable->entries[index].structuralMaterialID = 1;
        }
        else if (strcmp(token, "Unknown") == 0) {
            Db->picnicTableTable->entries[index].structuralMaterialID = 2;
        }
        else if (strcmp(token, "Wood") == 0) {
            Db->picnicTableTable->entries[index].structuralMaterialID = 3;
        }


        // Street/Avenue is parsed
        token = strtok(NULL, ",");
        if (token != NULL) {
            Db->picnicTableTable->entries[index].streetAvenue = malloc(strlen(token) + 1);
            if (Db->picnicTableTable->entries[index].streetAvenue == NULL){
                fprintf(stderr, "Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
            strcpy(Db->picnicTableTable->entries[index].streetAvenue, token);
        }
        else {
            Db->picnicTableTable->entries[index].streetAvenue = NULL;
        }

        // Neighborhood ID is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].neighborhoodID = atoi(token);

        // Neighbourhood Name is parsed
        token = strtok(NULL, ",");
        if (token != NULL) {
            Db->neighborhoodTable->names[index] = malloc(strlen(token) + 1);
            if (Db->neighborhoodTable->names[index] == NULL){
                fprintf(stderr, "Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
            strcpy(Db->neighborhoodTable->names[index], token);
        }
        else{
        Db->neighborhoodTable->names[index] = NULL;
        }
        // Ward is parsed
        token = strtok(NULL, ",");
        if (token != NULL) {
            Db->picnicTableTable->entries[index].ward = malloc(strlen(token) + 1);
            if (Db->picnicTableTable->entries[index].ward == NULL){
                fprintf(stderr, "Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
            strcpy(Db->picnicTableTable->entries[index].ward, token);
        }
        else{
        Db->picnicTableTable->entries[index].ward = NULL;
        }
        // Latitude is parsed
        token = strtok(NULL, ",");
        if (token != NULL) {
            Db->picnicTableTable->entries[index].latitude = malloc(strlen(token) + 1);
            if (Db->picnicTableTable->entries[index].latitude == NULL){
                fprintf(stderr, "Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
            strcpy(Db->picnicTableTable->entries[index].latitude, token);
        }
        else {
        Db->picnicTableTable->entries[index].latitude = NULL;
        }
        // Longitude is parsed
        token = strtok(NULL, ",");
        if (token != NULL) {
            Db->picnicTableTable->entries[index].longitude = malloc(strlen(token) + 1);
            if (Db->picnicTableTable->entries[index].longitude == NULL){
                fprintf(stderr, "Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
            strcpy(Db->picnicTableTable->entries[index].longitude, token);
        }
        else {
        Db->picnicTableTable->entries[index].longitude = NULL;
        }
        printf("Read ID: %d\n", Db->picnicTableTable->entries[index].tableID);

        printf("Read TYPE ID: %d\n",  Db->picnicTableTable->entries[index].tableTypeID);

        printf("Read SurfaceMaterialID: %d\n", Db->picnicTableTable->entries[index].surfaceMaterialID);

        printf("Read StructuralMaterialID: %d\n", Db->picnicTableTable->entries[index].structuralMaterialID);

        printf("Read Street/Aveneue: %s\n",Db->picnicTableTable->entries[index].streetAvenue);

        printf("Read NeighbourhoodID: %d\n",Db->picnicTableTable->entries[index].neighborhoodID);

        printf("Read Neighbourhood Names: %s\n",Db->neighborhoodTable->names[index]);

        printf("Read Ward: %s\n", Db->picnicTableTable->entries[index].ward);

        printf("Read Latitude: %s\n",  Db->picnicTableTable->entries[index].latitude);

        printf("Read longitude: %s\n",Db->picnicTableTable->entries[index].longitude );






    

        index++;

        





    }
    fclose(fp);
}

void exportDB(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error, could not open file");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "ID,Table Type,Surface Material,Structural Material,Street/Avenue,Neighborhood ID,Neighborhood Name,Ward,Latitude,Longitude\n");

    

    for (int i = 0; i < INIT_SIZE; i++) {
        char *TableType;
        char *SurfaceMaterial;
        char *StructuralMaterial;

        if(Db->picnicTableTable->entries[i].tableTypeID == 0) {
            TableType = "Other Table";
        }
        else if(Db->picnicTableTable->entries[i].tableTypeID == 1) {
            TableType = "Square Picnic Table";
        }
        else if(Db->picnicTableTable->entries[i].tableTypeID == 2) {
            TableType = "Round Picnic Table";
        }

        if(Db->picnicTableTable->entries[i].surfaceMaterialID == 0) {
            SurfaceMaterial = "Metal";
        }
        else if(Db->picnicTableTable->entries[i].surfaceMaterialID == 1) {
            SurfaceMaterial = "Unknown";
        }
        else if(Db->picnicTableTable->entries[i].surfaceMaterialID == 2) {
            SurfaceMaterial = "Wood";
        }

        if(Db->picnicTableTable->entries[i].structuralMaterialID == 0) {
            StructuralMaterial = "Aggregate";
        }
        else if(Db->picnicTableTable->entries[i].structuralMaterialID == 1) {
            StructuralMaterial = "Metal";
        }
        else if(Db->picnicTableTable->entries[i].structuralMaterialID == 2) {
            StructuralMaterial = "Unknown";
        }
        else if(Db->picnicTableTable->entries[i].structuralMaterialID == 3) {
            StructuralMaterial = "Wood";
        }

        fprintf(fp, "%d,%s,%s,%s,%s,%d,%s,%s,%s,%s",
                Db->picnicTableTable->entries[i].tableID,
                TableType,
                SurfaceMaterial,
                StructuralMaterial,
                Db->picnicTableTable->entries[i].streetAvenue,
                Db->picnicTableTable->entries[i].neighborhoodID,
                Db->neighborhoodTable->names[i],
                Db->picnicTableTable->entries[i].ward,
                Db->picnicTableTable->entries[i].latitude,
                Db->picnicTableTable->entries[i].longitude
        );

    
        

        



        
    }
    
    

    fclose(fp);
}



