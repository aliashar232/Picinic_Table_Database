/* DB.h
 * 
 * Public interface to the specification for your database implementation
 *
 * Please do not modify any code in this file!  We will be testing your code
 * under the assumption that the public interface matches this interface.
 *
 * Author: <TODO: Group Member Names>
 * Lab instructor: <TODO: Your lab instructor's name here>
 * Lecture instructor: <TODO: Your lecture instructor's name here>
 */

#ifndef DB_H
#define DB_H

#include <stdlib.h>
/*
 * DECLARE AND TYPEDEF HERE THE STRUCTS Table, NeighbourhoodTable, AND PicnicTable
 * WHICH WILL BE FIELDS IN THE STRUCT DataBase BELOW. YOU MAY DECLARE ADDITIONAL
 * STRUCTS AS NEEDED.
 */

/*
 * The INIT_SIZE is only relevant if you are using arrays for your data structures. 
 * If you are using linked lists you will not need it
 */
#define INIT_SIZE 5  

typedef struct {
    char **types;
} Table;

typedef struct {
    char **names;
} NeighbourhoodTable;

typedef struct{
    int tableID;
    int siteID;
    int tableTypeID;
    int surfaceMaterialID;
    int structuralMaterialID;
    char *streetAvenue;
    int neighborhoodID;
    char *ward;
    char *latitude;
    char *longitude;
} PicnicTableEntry;

typedef struct{
    size_t count;
    PicnicTableEntry *entries;
} PicnicTable;


/*
 * You may change the internal details of the struct below,
 * only keep it typedef'ed to DataBase
 */
typedef struct {
    // You can add anything you see fit here
    Table *tableTypeTable;
    Table *surfaceMaterialTable;
    Table *structuralMaterialTable;
    NeighbourhoodTable *neighborhoodTable;
    PicnicTable *picnicTableTable;
} DataBase;

/* Declare a global DataBase variable*/
/* That should be the only global variable declared*/
/* DB.c should have the definition of this variable*/
extern DataBase *Db;


/*
 * Take the name of a member of the picnicTable entry and a value for that member 
 * as parameters and return the number of entries in the picnicTable which have 
 * that value for the memberName.
 * 
 * This is to be implemented for the following members:
 *  1- Table Type
 *  2- Surface Material
 *  3- Structural Material
 *  4- Neighborhood ID
 *  5- Neighborhood Name
 *  6- Ward
 */

int countEntries(char *memberName, char * value);

/*
 * Take the name of a member of the picnicTable entry as an argument 
 * and sort the table in ascending order of the entry values of that member. 
 * The function must be implemented for:
 *   1- Table Type
 *   2- Surface Material
 *   3- Structural Material
 *   4- Neighborhood Name
 *   5- Ward
 */
void sortByMember(char *memberName);

/*
 * Take a tableID, the name of a member of the picnicTable entry and a value for that 
 * member as parameters, and find the entry which has that tableID and 
 * change its memberName value to newValue. 
 * Members that can be modified:
 *  1- Table Type
 *  2- Surface Material
 *  3- Structural Material 
 * If the new value is not found in the existing tables, this value must be added to the 
 * corresponding table.
 */
void editTableEntry(int tableID, char *memberName, char *value);



/*
 * print a listing of picnic tables grouped by neigbourhoods in ascending 
 * alphabetical order.
 */
void reportByNeighbourhood();

/*
 * print a listing of picnic tables grouped by wards in ascending order.
 */
void reportByWard();

/*
 * Frees all dynamic memory associated with each table upon exit. 
 */
void freeDB();

void DB_create();

void importDB(const char *filename);
void exportDB(const char *filename);

#endif
