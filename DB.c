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

    // error checking for general tables.
    if (Db->tableTypeTable == NULL || Db->surfaceMaterialTable == NULL|| 
        Db->structuralMaterialTable == NULL || 
        Db->neighborhoodTable == NULL || Db->picnicTableTable == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
    }

    //Allocating memeory for internal arrays of the tables.
    Db->tableTypeTable->types = malloc(INIT_SIZE * sizeof(char *));
    Db->surfaceMaterialTable->types = malloc(INIT_SIZE * sizeof(char *));
    Db->structuralMaterialTable->types = malloc(INIT_SIZE * sizeof(char *));
    Db->neighborhoodTable->names = malloc(INIT_SIZE * sizeof(char *));

    if (!Db->tableTypeTable->types || !Db->surfaceMaterialTable->types || !Db->structuralMaterialTable->types || !Db->neighborhoodTable->names) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
 
    Db->picnicTableTable->count = 0;
    Db->picnicTableTable->entries = malloc(INIT_SIZE * sizeof(PicnicTableEntry));

    // Initaializing the internal arrays of the table to NULL and 0.
    for (int i = 0; i < INIT_SIZE; i++) {
        Db->tableTypeTable->types[i] = NULL;
        Db->surfaceMaterialTable->types[i] = NULL;
        Db->structuralMaterialTable->types[i] = NULL;
        Db->neighborhoodTable->names[i] = NULL;

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
}

int countEntries(char *memberName, char * value){
    int count = 0;
    for (int i = 0; i < INIT_SIZE; i++) {
        if (strcmp(memberName, "Table Type") == 0) {
            if (Db->tableTypeTable->types[i] && strcmp(Db->tableTypeTable->types[i], value) == 0) {
                count++;
            }
        } else if (strcmp(memberName, "Surface Material") == 0) {
            if (Db->surfaceMaterialTable->types[i] && strcmp(Db->surfaceMaterialTable->types[i], value) == 0) {
                count++;
            }
        } else if (strcmp(memberName, "Structural Material") == 0) {
            if (Db->structuralMaterialTable->types[i] && strcmp(Db->structuralMaterialTable->types[i], value) == 0) {
                count++;
            }
        } else if (strcmp(memberName, "Neighborhood ID") == 0) {
            if (Db->picnicTableTable->entries[i].neighborhoodID == atoi(value)) {
                count++;
            }
        } else if (strcmp(memberName, "Neighbourhood Name") == 0) {
            if (Db->neighborhoodTable->names[i] && strcmp(Db->neighborhoodTable->names[i], value) == 0) {
                count++;
            }
        } else if (strcmp(memberName, "Ward") == 0) {
            if (Db->picnicTableTable->entries[i].ward && strcmp(Db->picnicTableTable->entries[i].ward, value) == 0) {
                count++;
            }
        } else {
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
        // ID is Parsed
        token = strtok(line, ",");
        Db->picnicTableTable->entries[index].tableID = atoi(token);

        //Table Type ID is parsed and cnoverted to integer accordingly.
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].tableTypeID = findOrAddToTable(Db->tableTypeTable, token);

        // Surface Material ID is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].surfaceMaterialID = findOrAddToTable(Db->surfaceMaterialTable, token);

        // Structural Material ID is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].structuralMaterialID = findOrAddToTable(Db->structuralMaterialTable, token);

        // Street/Avenue is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].streetAvenue = strdup(token);

        // Neighborhood ID is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].neighborhoodID = atoi(token);

        // Neighbourhood Name is parsed
        token = strtok(NULL, ",");
        Db->neighborhoodTable->names[index] = strdup(token);

        // Ward is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].ward = strdup(token);

        // Latitude is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].latitude = strdup(token);

        // Longitude is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].longitude = strdup(token);

        //I have printed all of this to check if the parsing is done correctly, and the integer values are printed according
        //to theri respective string values.

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
    Db->picnicTableTable->count = index;
    fclose(fp);
}

void exportDB(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error, could not open file");
        exit(EXIT_FAILURE);
    }

    //header is printed to the output file.
    fprintf(fp, "ID,Table Type,Surface Material,Structural Material,Street/Avenue,Neighborhood ID,Neighborhood Name,Ward,Latitude,Longitude\n");

    for (int i = 0; i < INIT_SIZE; i++) {
        char *TableType = Db->tableTypeTable->types[Db->picnicTableTable->entries[i].tableTypeID];
        char *SurfaceMaterial = Db->surfaceMaterialTable->types[Db->picnicTableTable->entries[i].surfaceMaterialID];
        char *StructuralMaterial = Db->structuralMaterialTable->types[Db->picnicTableTable->entries[i].structuralMaterialID];

        fprintf(fp, "%d,%s,%s,%s,%s,%d,%s,%s,%s,%s\n",
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

void sortByMember(char *memberName) {
    if (strcmp(memberName, "Table Type") == 0) {
        qsort(Db->picnicTableTable->entries, Db->picnicTableTable->count, sizeof(PicnicTableEntry), compareByTableType);
    } else if (strcmp(memberName, "Surface Material") == 0) {
        qsort(Db->picnicTableTable->entries, Db->picnicTableTable->count, sizeof(PicnicTableEntry), compareBySurfaceMaterial);
    } else if (strcmp(memberName, "Structural Material") == 0) {
        qsort(Db->picnicTableTable->entries, Db->picnicTableTable->count, sizeof(PicnicTableEntry), compareByStructuralMaterial);
    } else if (strcmp(memberName, "Neighborhood Name") == 0) {
        qsort(Db->picnicTableTable->entries, Db->picnicTableTable->count, sizeof(PicnicTableEntry), compareByNeighborhoodName);
    } else if (strcmp(memberName, "Ward") == 0) {
        qsort(Db->picnicTableTable->entries, Db->picnicTableTable->count, sizeof(PicnicTableEntry), compareByWard);
    }
}

void editTableEntry(int tableID, char *memberName, char *value) {
    PicnicTableEntry *entry = &Db->picnicTableTable->entries[tableID];
    if (strcmp(memberName, "Table Type") == 0) {
        entry->tableTypeID = findOrAddToTable(Db->tableTypeTable, value);
    } else if (strcmp(memberName, "Surface Material") == 0) {
        entry->surfaceMaterialID = findOrAddToTable(Db->surfaceMaterialTable, value);
    } else if (strcmp(memberName, "Structural Material") == 0) {
        entry->structuralMaterialID = findOrAddToTable(Db->structuralMaterialTable, value);
    }
}
