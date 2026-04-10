#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../DB.h"
#include "../DB_impl.h"

void test_db_create() {
    DB_create();
    if (Db != NULL) {
        printf("DB_CREATE: PASS\n");
    } else {
        printf("DB_CREATE: FAIL\n");
    }
}

void test_db_import() {
    DB_create();
    importDB("testing_strategy/test.csv");
    if (Db->picnicTableTable->count > 0) {
        printf("DB_IMPORT: PASS\n");
    } 
    else {
        printf("DB_IMPORT: FAIL\n");
    }
}

void test_db_export() {
    DB_create();
    importDB("testing_strategy/test.csv");
    exportDB("testing_strategy/test_out.csv");
    FILE* fp = fopen("testing_strategy/test_out.csv", "r");
    if (fp != NULL) {
        printf("DB_EXPORT: PASS\n");
        fclose(fp);
    } else {
        printf("DB_EXPORT: FAIL\n");
    }
}

void test_countEntries() {
    DB_create();
    importDB("testing_strategy/test.csv");
    int expected_count = 7; 


    int actual_count = countEntries("Table Type", "Square Picnic Table");
    if (actual_count == expected_count) {
        printf("COUNT_ENTRIES: PASS\n");
    } 
    else {
        printf("COUNT_ENTRIES: FAIL\n");
    }


    


    

}

void test_sortByMember() {
    DB_create();
    importDB("testing_strategy/test.csv");
    sortByMember("Table Type");
    exportDB("testing_strategy/sorted_output.csv");
}

void test_editTableEntry() {
    DB_create();
    importDB("testing_strategy/test.csv");
    int before_result_1 = Db->picnicTableTable->entries[0].tableTypeID;
    int before_result_2 = Db->picnicTableTable->entries[0].surfaceMaterialID;
    int before_result_3 = Db->picnicTableTable->entries[0].structuralMaterialID;

    editTableEntry(0, "Table Type", "Changed Table");
    editTableEntry(0, "Surface Material", "Changed Surface Material");
    editTableEntry(0, "Structural Material", "Changed Structural Material");

    exportDB("testing_strategy/editTableEntry_output.csv");

    int after_result_1 = Db->picnicTableTable->entries[0].tableTypeID;
    int after_result_2 = Db->picnicTableTable->entries[0].surfaceMaterialID;
    int after_result_3 = Db->picnicTableTable->entries[0].structuralMaterialID;

    if (before_result_1 != after_result_1 && before_result_2 != after_result_2 && before_result_3 != after_result_3) {
        printf("EDIT_TABLE_ENTRY: PASS\n");
    } else {
        printf("EDIT_TABLE_ENTRY: FAIL\n");
    }
}

    



void test_reportByNeighbourhood() {
    DB_create();
    importDB("testing_strategy/test.csv");
    reportByNeighbourhood();
    
}

void test_reportByWard() {
    DB_create();
    importDB("testing_strategy/test.csv");
    reportByWard();
    
}

int main() {
    test_db_create();
    test_db_import();
    test_db_export();
    test_countEntries();
    test_sortByMember();
    test_editTableEntry();
    test_reportByNeighbourhood();
    test_reportByWard();

    return 0;
}

