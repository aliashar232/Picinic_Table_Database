/* DB_impl.c
 *
 * Description: This file contains the implementation of the database functions declared in DB.h,
 it also containd implementation of helper functions designed to map string to ids, handling info of neighbourhood,
 comparison function for sortby function, helper fucntions to write csv and binary file.

 * Author: Ashar, Lyra, Theo
 * Lab instructor: Dharaben Wagh
 * Lecture instructor: Dharaben Wagh
 */

#include "DB.h"      /* Import the public database header. */
#include "DB_impl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Function: findOrAddToTable
Description: This helper function will search for a value in the table, if the value sis not found then it
adds into the table.
parameters: table points to Table, value
Pre: Parameters are not null
Post: value must exist 
Returns: index of teh value.
*/


int findOrAddToTable(Table *table, char *value) {
    int i = 0;
    for (; table->types[i] != NULL; i++) {
        if (strcmp(table->types[i], value) == 0)
            return i;
    }

    int newSize = i + 1;
    // resizing once for each value is not particularly efficient, but is acceptable here because our arrays are small arrays of pointers
    if (newSize >= INIT_SIZE) {
        table->types = realloc(table->types, (newSize + 1) * sizeof(char*));
    }

    table->types[i] = malloc(strlen(value) + 1);
    strcpy(table->types[i], value);
    table->types[i + 1] = NULL;
    return i;
}

size_t findOrAddNeighborhood(int id, char *value) {
    size_t i = 0;
    for (; Db->neighborhoodTable->ids[i] != -1; i++) {
        if (Db->neighborhoodTable->ids[i] == id)
            return i;
    }

    int newSize = i + 1;
    // resizing once for each value is not particularly efficient, but is acceptable here because our arrays are small arrays of pointers
    if (newSize >= INIT_SIZE) {
        Db->neighborhoodTable->names = realloc(Db->neighborhoodTable->names, (newSize + 1) * sizeof(char*));
        Db->neighborhoodTable->ids = realloc(Db->neighborhoodTable->ids, (newSize + 1) * sizeof(int));
    }

    if (value) {
        Db->neighborhoodTable->names[i] = malloc(strlen(value) + 1);
        strcpy(Db->neighborhoodTable->names[i], value);
    } else {
        Db->neighborhoodTable->names[i] = NULL;
    }
    Db->neighborhoodTable->ids[i] = id;
    Db->neighborhoodTable->names[i + 1] = NULL;
    Db->neighborhoodTable->ids[i + 1] = -1;
    return i;
}

#define COMPARE_BY_TABLE(NAME, TABLE, FIELD) int NAME(const void *a, const void *b) { \
    const PicnicTableEntry *picnicTable1 = a, *picnicTable2 = b; \
    return strcmp(Db->TABLE->types[picnicTable1->FIELD], Db->TABLE->types[picnicTable2->FIELD]); \
}

COMPARE_BY_TABLE(compareByTableType, tableTypeTable, tableTypeID);
COMPARE_BY_TABLE(compareBySurfaceMaterial, surfaceMaterialTable, surfaceMaterialID);
COMPARE_BY_TABLE(compareByStructuralMaterial, structuralMaterialTable, structuralMaterialID);

/*
Function: compareByNeighborhoodName compareByWard compareStringWithID
Description: comparison functions used for sorting purposes
parameters: a and b pointers of elements which are compared
Pre: valid pointers
Post: Elements ordered in correct order
Returns: Negative, zero, or positive value based on order.
*/

int compareByNeighborhoodName(const void *a, const void *b) {
    const PicnicTableEntry *picnicTable1 = a, *picnicTable2 = b;
    return strcmp(findNeighborhoodName(picnicTable1->neighborhoodID), findNeighborhoodName(picnicTable2->neighborhoodID));
}

int compareByWard(const void *a, const void *b) {
    const PicnicTableEntry *picnicTable1 = a, *picnicTable2 = b;
    return strcmp(picnicTable1->ward, picnicTable2->ward);
}

int compareStringWithID(const void *a, const void *b) {
    const StringWithID *tuple1 = a, *tuple2 = b;
    return strcmp(tuple1->value, tuple2->value);
}

/*
Function: writeEntryAsCSV
Description: This function writes picnic table entry into a csv file.
parameters: fp as file pointer, entry points to entry in picnic table
Pre: valid pointers
Post: Entry is written
Returns: None.
*/

void writeEntryAsCSV(FILE *fp, const PicnicTableEntry *entry) {
    char *TableType = Db->tableTypeTable->types[entry->tableTypeID];
    char *SurfaceMaterial = Db->surfaceMaterialTable->types[entry->surfaceMaterialID];
    char *StructuralMaterial = Db->structuralMaterialTable->types[entry->structuralMaterialID];

    fprintf(fp, "%d,%s,%s,%s,%s,%d,%s,%s,%s,%s,\"(%s, %s)\"\n",
        entry->siteID,
        TableType,
        SurfaceMaterial,
        StructuralMaterial,
        entry->streetAvenue,
        entry->neighborhoodID,
        findNeighborhoodName(entry->neighborhoodID),
        entry->ward,
        entry->latitude,
        entry->longitude,
        entry->latitude,
        entry->longitude
    );
}

/*
Function: findNeighbourhoodID
Description: This function find th id with respect to neighbourhood name.
parameters: neihbourhoodName points to name for searching.
Pre: valid pointers
Post: None
Returns: Neighbourhood id or -1 if not found
*/

int findNeighborhoodID(const char *neighborhoodName) {
    for (size_t i = 0; Db->neighborhoodTable->names[i] != NULL; i++) {
        if (strcmp(Db->neighborhoodTable->names[i], neighborhoodName) == 0) {
            return Db->neighborhoodTable->ids[i];
        }
    }
    return -1;
}

/*
Function: findNeighborhoodName
Description: This function find the name of neighborhood repsect to id.
parameters: id
Pre: None
Post: None
Returns: Neighbourhood name or null.
*/

char *findNeighborhoodName(int id) {
    for (size_t i = 0; Db->neighborhoodTable->ids[i] != -1; i++) {
        if (Db->neighborhoodTable->ids[i] == id) {
            return Db->neighborhoodTable->names[i];
        }
    }
    return NULL;
}

/*
Function: writeString
Description: This function writes the string to binary file.
parameters: str, fp
Pre: valid pointers
Post: string is written to file.
Returns: None
*/

void writeString(const char *str, FILE *fp) {
    if (!str) {
        fwrite("\0", sizeof(unsigned char), 1, fp);
        return;
    }
    unsigned char length = (unsigned char)strlen(str);
    fwrite(&length, sizeof(unsigned char), 1, fp);
    fwrite(str, 1, length, fp);
}

/*
Function: writeTable
Description: This function write table of strings.
parameters: table, fp
Pre: valid pointers
Post: table is written to file
Returns: None
*/

void writeTable(const Table *table, FILE *fp) {
    long lengthOffset = ftell(fp);
    unsigned short length = 0;
    fwrite(&length, sizeof(unsigned short), 1, fp);
    for (int i = 0; table->types[i] != NULL; i++) {
        writeString(table->types[i], fp);
        length++;
    }
    long endOffset = ftell(fp);
    fseek(fp, lengthOffset, SEEK_SET);
    fwrite(&length, sizeof(unsigned short), 1, fp);
    fseek(fp, endOffset, SEEK_SET);
}

/*
Function: readString
Description: This function reads a string form binary file.
parameters: fp
Pre: valid pointers
Post: string is read
Returns: allocates string
*/

char* readString(FILE *fp) {
    unsigned char length;
    fread(&length, 1, 1, fp);
    if (length == 0) return NULL;
    char *readStr = malloc(length + 1);
    fread(readStr, 1, length, fp);
    readStr[length] = 0;
    return readStr;
}

/*
Function: readTable
Description: This function read string form the table.
parameters: table, fp
Pre: valid pointers
Post: table is read
Returns: None
*/

void readTable(Table *table, FILE *fp) {
    unsigned short length;
    fread(&length, sizeof(unsigned short), 1, fp);
    table->types = realloc(table->types, (length + 1) * sizeof(char*));
    for (unsigned short i = 0; i < length; i++) {
        table->types[i] = readString(fp);
    }
    table->types[length] = NULL;
}

/*
Function: compressDB decompressDB
Description: This function reads and write database to a binary file with an option for compression and decompression. 
parameters: filename
Pre: valid pointers
Post: database is read and written to the file.
Returns: None
*/

void compressDB(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    writeTable(Db->tableTypeTable, fp);
    writeTable(Db->surfaceMaterialTable, fp);
    writeTable(Db->structuralMaterialTable, fp);

    // neighborhood table
    unsigned short neighborhoodTableLength = 0;
    long lengthOffset = ftell(fp);
    fwrite(&neighborhoodTableLength, sizeof(unsigned short), 1, fp);
    unsigned short id = 0;
    for (unsigned short i = 0; Db->neighborhoodTable->ids[i] != -1; i++) {
        id = Db->neighborhoodTable->ids[i];
        fwrite(&id, sizeof(unsigned short), 1, fp);
        neighborhoodTableLength++;
    }
    for (unsigned short i = 0; Db->neighborhoodTable->names[i]; i++) {
        writeString(Db->neighborhoodTable->names[i], fp);
    }
    long endOffset = ftell(fp);
    fseek(fp, lengthOffset, SEEK_SET);
    fwrite(&neighborhoodTableLength, sizeof(unsigned short), 1, fp);
    fseek(fp, endOffset, SEEK_SET);

    unsigned short lengthField = Db->picnicTableTable->count;
    fwrite(&lengthField, sizeof(unsigned short), 1, fp);

    for (unsigned short i = 0; i < Db->picnicTableTable->count; i++) {
        PicnicTableEntry *entry = &Db->picnicTableTable->entries[i];
        PicnicTableEntryCompressedMembers bitfields = {entry->siteID, entry->tableTypeID, entry->surfaceMaterialID, entry->structuralMaterialID, entry->neighborhoodID};
        fwrite(&bitfields, sizeof(bitfields), 1, fp);
        writeString(entry->streetAvenue, fp);
        writeString(entry->ward, fp);
        writeString(entry->latitude, fp);
        writeString(entry->longitude, fp);
    }
    fclose(fp);
}

void decompressDB(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    readTable(Db->tableTypeTable, fp);
    readTable(Db->surfaceMaterialTable, fp);
    readTable(Db->structuralMaterialTable, fp);

    // neighborhood table
    unsigned short neighborhoodTableLength = 0;
    fread(&neighborhoodTableLength, sizeof(unsigned short), 1, fp);
    Db->neighborhoodTable->ids = realloc(Db->neighborhoodTable->ids, (neighborhoodTableLength + 1) * sizeof(int));
    Db->neighborhoodTable->names = realloc(Db->neighborhoodTable->names, (neighborhoodTableLength + 1) * sizeof(char*));

    unsigned short id = 0;
    for (unsigned short i = 0; i < neighborhoodTableLength; i++) {
        fread(&id, sizeof(unsigned short), 1, fp);
        Db->neighborhoodTable->ids[i] = id;
    }
    for (unsigned short i = 0; i < neighborhoodTableLength; i++) {
        Db->neighborhoodTable->names[i] = readString(fp);
    }
    Db->neighborhoodTable->ids[neighborhoodTableLength] = -1;
    Db->neighborhoodTable->names[neighborhoodTableLength] = NULL;

    unsigned short lengthField = 0;
    fread(&lengthField, sizeof(unsigned short), 1, fp);
    Db->picnicTableTable->count = lengthField;
    Db->picnicTableTable->entries = realloc(Db->picnicTableTable->entries, lengthField * sizeof(PicnicTableEntry));

    for (unsigned short i = 0; i < lengthField; i++) {
        PicnicTableEntryCompressedMembers bitfields;
        fread(&bitfields, sizeof(bitfields), 1, fp);
        char *streetAvenue = readString(fp);
        char *ward = readString(fp);
        char *latitude = readString(fp);
        char *longitude = readString(fp);
        PicnicTableEntry entry = {i, bitfields.siteID, bitfields.tableTypeID, bitfields.surfaceMaterialID, bitfields.structuralMaterialID, streetAvenue, bitfields.neighborhoodID, ward, latitude, longitude};
        Db->picnicTableTable->entries[i] = entry;
    }
    fclose(fp);
}

/*
Function: freeTable
Description: This function Frees the memory of the allocated table.
parameters: table
Pre: valid pointers
Post: Memory is freed
Returns: None
*/

void freeTable(Table **table) {
    if (table) {
        if (*table) {
            for (int i = 0; (*table)->types[i]; i++) {
                free((*table)->types[i]);
            }
            free((*table)->types);
            free(*table);
        }
        *table = NULL;
    }
}
