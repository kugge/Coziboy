/* 
 * COZIBOY by Sofiane Djerbi (@Kugge)
 * Source code and licence available on Github.
 * https://github.com/Kugge/Coziboy
 */

#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    // Parse args
    if (argc != 2) {
        fprintf(stderr, "Usage: coziboy <filename>\n");
        return 1;
    }
    char *file_name = argv[1];
    // Check if file doesn't exists
    if (access(file_name, F_OK) != 0) {
        fprintf(stderr, "Error: File %s does not exists.\n", file_name);
        return 2;
    }

    fprintf(stdout,"Loading %s...\n", file_name);
    return 0;
}
