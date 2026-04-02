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

int findOrAddToTable(Table *table, char *value) {
    int i = 0;
    for (; table->ids[i] != -1; i++) {
        if (strcmp(table->types[i], value) == 0)
            return table->ids[i];
    }

    int newSize = i + 1;
    // resizing once for each value is not particularly efficient, but is acceptable here because our arrays are small arrays of pointers
    if (newSize >= INIT_SIZE) {
        table->ids = realloc(table->ids, newSize);
        table->types = realloc(table->types, newSize);
    }

    table->types[i] = malloc(strlen(value) + 1);
    strcpy(table->types[i], value);
    table->ids[i] = i > 0 ? table->ids[i - 1] + 1 : 0;
    return table->ids[i];
}
