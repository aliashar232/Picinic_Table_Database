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
    Db->neighborhoodTable->ids = malloc(INIT_SIZE * sizeof(int));

    if (!Db->tableTypeTable->types || !Db->surfaceMaterialTable->types || !Db->structuralMaterialTable->types || !Db->neighborhoodTable->names || !Db->neighborhoodTable->ids) {
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
        Db->neighborhoodTable->ids[i] = -1;

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
    int targetID = -1;

    if (strcmp(memberName, "Table Type") == 0) {
        targetID = findOrAddToTable(Db->tableTypeTable, value);
    } else if (strcmp(memberName, "Surface Material") == 0) {
        targetID = findOrAddToTable(Db->surfaceMaterialTable, value);
    } else if (strcmp(memberName, "Structural Material") == 0) {
        targetID = findOrAddToTable(Db->structuralMaterialTable, value);
    } else if (strcmp(memberName, "Neighborhood ID") == 0) {
        targetID = atoi(value);
    } else if (strcmp(memberName, "Neighborhood Name") == 0) {
        targetID = findNeighborhoodID(value);
    }

    for (int i = 0; i < Db->picnicTableTable->count; i++) {
        if (strcmp(memberName, "Table Type") == 0) {
            if (Db->picnicTableTable->entries[i].tableTypeID == targetID) {
                count++;
            }
        } else if (strcmp(memberName, "Surface Material") == 0) {
            if (Db->picnicTableTable->entries[i].surfaceMaterialID == targetID) {
                count++;
            }
        } else if (strcmp(memberName, "Structural Material") == 0) {
            if (Db->picnicTableTable->entries[i].structuralMaterialID == targetID) {
                count++;
            }
        } else if (strcmp(memberName, "Neighborhood ID") == 0) {
            if (Db->picnicTableTable->entries[i].neighborhoodID == targetID) {
                count++;
            }
        } else if (strcmp(memberName, "Neighbourhood Name") == 0) {
            if (Db->picnicTableTable->entries[i].neighborhoodID == targetID) {
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
    
    char line[256];
    fgets(line, sizeof(line), fp); // header skipped
    
    size_t index = 0;
    size_t capacity = INIT_SIZE;

    while (fgets(line, sizeof(line), fp)) {
        char *token;
        // ID is Parsed
        token = strtok(line, ",");
        Db->picnicTableTable->entries[index].tableID = atoi(token);
        printf("Read ID: %d\n", Db->picnicTableTable->entries[index].tableID);

        //Table Type ID is parsed and cnoverted to integer accordingly.
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].tableTypeID = findOrAddToTable(Db->tableTypeTable, token);
        printf("Read TYPE ID: %d\n",  Db->picnicTableTable->entries[index].tableTypeID);

        // Surface Material ID is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].surfaceMaterialID = findOrAddToTable(Db->surfaceMaterialTable, token);
        printf("Read SurfaceMaterialID: %d %s\n", Db->picnicTableTable->entries[index].surfaceMaterialID, token);

        // Structural Material ID is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].structuralMaterialID = findOrAddToTable(Db->structuralMaterialTable, token);
        printf("Read StructuralMaterialID: %d %s\n", Db->picnicTableTable->entries[index].structuralMaterialID, token);

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
        printf("Read Street: %s\n", Db->picnicTableTable->entries[index].streetAvenue);

        // Neighborhood ID is parsed
        token = strtok(NULL, ",");
        Db->picnicTableTable->entries[index].neighborhoodID = atoi(token);

        // Neighbourhood Name is parsed
        token = strtok(NULL, ",");
        if (token != NULL) {
            findOrAddNeighborhood(Db->picnicTableTable->entries[index].neighborhoodID, token);
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
        printf("Read StructuralMaterialID: %d\n", Db->picnicTableTable->entries[index].structuralMaterialID);
        printf("Read Street/Aveneue: %s\n",Db->picnicTableTable->entries[index].streetAvenue);
        printf("Read NeighbourhoodID: %d\n",Db->picnicTableTable->entries[index].neighborhoodID);
        printf("Read Ward: %s\n", Db->picnicTableTable->entries[index].ward);
        printf("Read Latitude: %s\n",  Db->picnicTableTable->entries[index].latitude);
        printf("Read longitude: %s\n",Db->picnicTableTable->entries[index].longitude );

        index++;

        // automatically resizing array
        if (index >= capacity) {
            capacity *= 2;
            Db->picnicTableTable->entries = realloc(Db->picnicTableTable->entries, capacity * sizeof(PicnicTableEntry));
            if (!Db->picnicTableTable->entries) {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
        }
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
    fprintf(fp, "Id,Table Type,Surface Material,Structural Material,Street/Avenue,Neighborhood ID,Neighborhood Name,Ward,Latitude,Longitude,Location\n");

    for (size_t i = 0; i < Db->picnicTableTable->count; i++) {
        writeEntryAsCSV(fp, &Db->picnicTableTable->entries[i]);
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

void reportByNeighbourhood() {
    // store each picnic table along with its ward
    StringWithID *sortedPicnicTables = malloc(Db->picnicTableTable->count * sizeof(StringWithID));
    for (size_t i = 0; i < Db->picnicTableTable->count; i++) {
        StringWithID tuple = {findNeighborhoodName(Db->picnicTableTable->entries[i].neighborhoodID), i};
        sortedPicnicTables[i] = tuple;
    }
    qsort(sortedPicnicTables, Db->picnicTableTable->count, sizeof(StringWithID), compareStringWithID);

    int lastNeighborhoodID = -1;
    for (size_t i = 0; i < Db->picnicTableTable->count; i++) {
        if (lastNeighborhoodID == -1 || lastNeighborhoodID != Db->picnicTableTable->entries[sortedPicnicTables[i].id].neighborhoodID) {
            lastNeighborhoodID = Db->picnicTableTable->entries[sortedPicnicTables[i].id].neighborhoodID;
            printf("%s:\n", sortedPicnicTables[i].value);
        }
        writeEntryAsCSV(stdout, &Db->picnicTableTable->entries[sortedPicnicTables[i].id]);
    }
}

void reportByWard() {
    // store each picnic table along with its ward
    StringWithID *sortedPicnicTables = malloc(Db->picnicTableTable->count * sizeof(StringWithID));
    for (size_t i = 0; i < Db->picnicTableTable->count; i++) {
        StringWithID tuple = {Db->picnicTableTable->entries[i].ward, i};
        sortedPicnicTables[i] = tuple;
    }
    qsort(sortedPicnicTables, Db->picnicTableTable->count, sizeof(StringWithID), compareStringWithID);

    char *lastWard = NULL;
    for (size_t i = 0; i < Db->picnicTableTable->count; i++) {
        if (!lastWard || strcmp(sortedPicnicTables[i].value, lastWard) != 0) {
            lastWard = sortedPicnicTables[i].value;
            printf("%s:\n", lastWard);
        }
        writeEntryAsCSV(stdout, &Db->picnicTableTable->entries[sortedPicnicTables[i].id]);
    }
}
