#include <stdio.h> 
#include <conio.h>
#include <winsock2.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>

#define BUFFER_SIZE 104857600

#pragma comment(lib, "ws2_32.lib");

char *read_file(){

    FILE *file;
    char *buffer;
    int bufferSize = 1024; // Initial buffer size
    int position = 0; // Position in the buffer
    int ch; // Character read from file

     // Open the file in reading mode
    file = fopen("home.html", "r");
    if (!file) {
        fprintf(stderr, "Unable to open file %s", "home.html");
        return "";
    }

    // // Open the HTML file
    //     int file_fd = open("home.html", O_RDONLY);
    //     if(file_fd == -1){
    //         perror("Error opening file");
    //     }
    //     printf("Hello1");
    
     // Allocate memory for the buffer
    buffer = (char *)malloc(bufferSize);
    if (!buffer) {
        fprintf(stderr, "Memory error!");
        fclose(file);
        return "";
    }

    // Read the file, character by character
    while ((ch = fgetc(file)) != EOF) {
        // Check if buffer needs to be expanded
        if (position == bufferSize - 1) {
            bufferSize *= 2; // Double the buffer size
            buffer = (char *)realloc(buffer, bufferSize);
            if (!buffer) {
                fprintf(stderr, "Memory error!");
                fclose(file);
                return "";
            }
        }

        // Store the character in the buffer
        buffer[position] = ch;
        position++;
    }
    //printf(buffer);

    return buffer;


}

int main(){

    char * filedate = read_file();
    printf(filedate);
}