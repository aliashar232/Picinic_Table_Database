/* DB_impl.h
 *
 * For any "private"  declarations that should not be visible to the public
 * users of the database, but might want to be shared across various source
 * files in database implementation.
 * 
 * It is a common convention to append "_impl" to a private implementation of
 * some public interface, so we do so here.
 *
 * Author: <TODO: Group Member Names>
 * Lab instructor: <TODO: Your lab instructor's name here>
 * Lecture instructor: <TODO: Your lecture instructor's name here>
 */


#ifndef DB_IMPL_H
#define DB_IMPL_H

#include <stdio.h>

typedef struct {
    char *value;
    int id;
} StringWithID;

int compareStringWithID(const void*, const void*);

int findOrAddToTable(Table *table, char *value);
int compareByTableType(const void*, const void*);
int compareBySurfaceMaterial(const void*, const void*);
int compareByStructuralMaterial(const void*, const void*);
int compareByNeighborhoodName(const void*, const void*);
int compareByWard(const void*, const void*);

void writeEntryAsCSV(FILE *fp, const PicnicTableEntry *entry);

size_t findOrAddNeighborhood(int id, char *value);
int findNeighborhoodID(const char *neighborhoodName);
char * findNeighborhoodName(int id);

typedef struct {
    int siteID : 12;
    int tableTypeID : 12;
    int surfaceMaterialID : 12;
    int structuralMaterialID : 12;
    int neighborhoodID : 16;
} PicnicTableEntryCompressedMembers;

void writeString(const char* str, FILE *fp);
// reads a string of variable length, allocating caller-owned memory for it on the heap
// returns the newly allocated string on the heap
char *readString(FILE *fp);
void writeTable(const Table *table, FILE *fp);
// reads a table of heap-allocated strings from a file
// reallocates table->entries
void readTable(Table *table, FILE *fp);
void compressDB(const char *filename);
void decompressDB(const char *filename);

#endif
