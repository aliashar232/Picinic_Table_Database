/* DB_impl.c
 *
 * TODO: Provide a high-level description of what is contained
 * in this file.
 *
 * Author: <TODO: Group Member Names>
 * Lab instructor: <TODO: Your lab instructor's name here>
 * Lecture instructor: <TODO: Your lecture instructor's name here>
 */

#include "DB.h"      /* Import the public database header. */
#include "DB_impl.h"
#include <stdio.h>
#include <string.h>

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

int findNeighborhoodID(const char *neighborhoodName) {
    for (size_t i = 0; Db->neighborhoodTable->names[i] != NULL; i++) {
        if (strcmp(Db->neighborhoodTable->names[i], neighborhoodName) == 0) {
            return Db->neighborhoodTable->ids[i];
        }
    }
    return -1;
}

char *findNeighborhoodName(int id) {
    for (size_t i = 0; Db->neighborhoodTable->ids[i] != -1; i++) {
        if (Db->neighborhoodTable->ids[i] == id) {
            return Db->neighborhoodTable->names[i];
        }
    }
    return NULL;
}

void writeString(const char *str, FILE *fp) {
    if (!str) {
        fwrite("\0", sizeof(unsigned char), 1, fp);
        return;
    }
    unsigned char length = (unsigned char)strlen(str);
    fwrite(&length, sizeof(unsigned char), 1, fp);
    fwrite(str, 1, length, fp);
}

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

char* readString(FILE *fp) {
    unsigned char length;
    fread(&length, 1, 1, fp);
    if (length == 0) return NULL;
    char *readStr = malloc(length + 1);
    fread(readStr, 1, length, fp);
    readStr[length] = 0;
    return readStr;
}

void readTable(Table *table, FILE *fp) {
    unsigned short length;
    fread(&length, sizeof(unsigned short), 1, fp);
    table->types = realloc(table->types, (length + 1) * sizeof(char*));
    for (unsigned short i = 0; i < length; i++) {
        table->types[i] = readString(fp);
    }
    table->types[length] = NULL;
}

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
