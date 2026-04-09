/*
 * dashboard.c
 *
 * TODO: Provide a high-level description of what is contained
 * in this file.
 *
 * Author: <TODO: Group Member Names>
 * Lab instructor: <TODO: Your lab instructor's name here>
 * Lecture instructor: <TODO: Your lecture instructor's name here>
*/

//You may include other original headers as you see fit
#include "DB.h"
#include "DB_impl.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    FILE *in_file;
    DB_create();
    if (argc != 2) {
        fprintf(stderr, "Usage: fcopy in_file\n");
        exit(EXIT_FAILURE);
    }
    in_file = fopen(argv[1], "r");
    if (in_file == NULL) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    importDB(argv[1]);
    
    int choice = 0;

    while (choice != 7) {
        printf("1. Export Database\n");
        printf("2. Count Entries\n");
        printf("3. Sort by\n");
        printf("4. Edit Entry\n");
        printf("5. Report\n");
        printf("6. Compress Database\n");
        printf("7. Exit\n");

        printf("Option: ");
        scanf("%d", &choice);   
        switch (choice) {
            case 1:
                if(choice == 1){
                    printf("Enter a filename: ");
                    scanf("%s", argv[1]);
                    exportDB(argv[1]);
                    break;
                }
            case 2:
                if(choice == 2){
                    int membercode;
                    char *member;
                    char Value[256];
                    printf("Enter member code(1. TT, 2. SM, 3. StM, 4. NID, 5. NN 6. W): \n");
                    scanf("%d", &membercode);
                    printf("Enter Value: \n");
                    scanf(" %[^\n]", Value);
                    
                    switch (membercode) {
                        case 1:
                            member = "Table Type";
                            break;
                        case 2:
                            member = "Surface Material";
                            break;
                        case 3:
                            member = "Structural Material";
                            break;
                        case 4:
                            member = "Neighborhood ID";
                            break;
                        case 5:
                            member = "Neighborhood Name";
                            break;
                        case 6:
                            member = "Ward";
                            break;
                        default:
                            fprintf(stderr, "Invalid member code\n");
                            exit(EXIT_FAILURE);
                    }
                
                    printf("%s <%s> appeared in the table %d time(s).\n", member, Value, countEntries(member, Value));

                
                }
                break;
            case 3:
                if (choice == 3){
                int sortcode;
                printf("Enter a criteria to sort by (1. TT, 2. SM, 3. StM, 4. NN, 5. W): \n");
                scanf("%d", &sortcode);
                char *sortmember;

                switch (sortcode) {
                        case 1:
                            sortmember = "Table Type";
                            break;
                        case 2:
                            sortmember = "Surface Material";
                            break;
                        case 3:
                            sortmember = "Structural Material";
                            break;
                        case 4:
                            sortmember = "Neighborhood Name";
                            break;
                        case 5:
                            sortmember = "Ward";
                            break;
                        default:
                            fprintf(stderr, "Invalid sort code\n");
                            exit(EXIT_FAILURE);
                    }

                    sortByMember(sortmember);
                    exportDB("sorted_out.csv");

                    printf("DB exported to %s.csv to verify", sortmember);


                }
                break;
            case 4:
                if (choice == 4) {
                    int table_id;
                    int membercode;
                    char *member;
                    char new_value[256];
                    printf("Enter a numeric table entry to edit: \n");
                    scanf("%d", &table_id);
                    printf("Enter member code (1. TT, 2. SM, 3. StM): \n");
                    scanf("%d", &membercode);
                    printf("Enter value: \n");
                    scanf(" %[^\n]", new_value);

                    switch (membercode) {
                        case 1:
                            member = "Table Type";
                            break;
                        case 2:
                            member = "Surface Material";
                            break;
                        case 3:
                            member = "Structural Material";
                            break;
                        default:
                            fprintf(stderr, "Invalid member code\n");
                            exit(EXIT_FAILURE);
                    }
                    editTableEntry(table_id, member, new_value);
                }
                
                break;
            case 5:
                //report();
                break;
            case 6:
                //compressDB();
                break;
            case 7:
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "Invalid choice\n");
                exit(EXIT_FAILURE);
            }

    }



   

    
    

    




    return 0;

}

