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
        entry->tableID,
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
