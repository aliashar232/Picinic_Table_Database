/* DB.c
 *
 * TODO: Provide a high-level description of what is contained
 * in this file.
 *
 * Author: <TODO: Group Member Names>
 * Lab instructor: <TODO: Your lab instructor's name here>
 * Lecture instructor: <TODO: Your lecture instructor's name here>
 */

#include "DB.h"       /* Import the public database header. */
#include "DB_impl.h"  /* Import the private database header */

Database *Db;


void DB_create() {
    Db = malloc(sizeof(DataBase));
    if (Db == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for
    Db->tableTypeTable = NULL;
    Db->surfaceMaterialTable = NULL;
    Db->structuralMaterialTable = NULL;
    Db->neighborhoodTable = NULL;
    Db->picnicTableTable = NULL;

    
}

int countEntries(char *memberName, char *value) {
    int count = 0;
    if (strcmp(memberName, "T"))