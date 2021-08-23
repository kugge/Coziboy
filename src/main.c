/*
 * COZIBOY by Sofiane Djerbi (@Kugge)
 * Source code and license available on Github.
 * https://github.com/Kugge/Coziboy
 */

#include <stdbool.h>
#include <stdio.h>
#include "hardware/cpu.h"


// Return true if the file exists
bool file_exists(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        fclose(fp);
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    // Parse args
    if (argc != 2) {
        fprintf(stderr, "Usage: coziboy <filename>\n");
        return 1;
    }
    char *file_name = argv[1];
    // Check if file exists
    if (!file_exists(file_name)) {
        fprintf(stderr, "Error: File %s does not exists.\n", file_name);
        return 2;
    }

    fprintf(stdout,"Loading file \"%s\"...\n", file_name);

    // TODO: Call emulator and load rom + GUI and debug tools

    return 0;
}
