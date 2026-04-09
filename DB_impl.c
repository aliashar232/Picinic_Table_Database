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
        table->types = realloc(table->types, newSize + 1);
    }

    table->types[i] = malloc(strlen(value) + 1);
    strcpy(table->types[i], value);
    table->types[i + 1] = NULL;
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
    return strcmp(Db->neighborhoodTable->names[picnicTable1->neighborhoodID], Db->neighborhoodTable->names[picnicTable2->neighborhoodID]);
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
        Db->neighborhoodTable->names[entry->neighborhoodID],
        entry->ward,
        entry->latitude,
        entry->longitude,
        entry->latitude,
        entry->longitude
    );
}
