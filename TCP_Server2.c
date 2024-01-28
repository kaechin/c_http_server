// https://www.binarytides.com/winsock-socket-programming-tutorial/
// https://github.com/JeffreytheCoder/Simple-HTTP-Server/blob/master/server.c

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

    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    char *message;

    printf("Intiialising Winsock\n");
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0){
        printf("Failed. Error code: %d", WSAGetLastError());

        // Typically, 1 means failed in C world
        return 1;
    }
    printf("Initialised.\n");

    // Creating Socket
    // SOCK_STREAM = TCP, SOCK_DGRAM = UDP
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        printf("Could not create socket : %d", WSAGetLastError());
    }

    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Binding Socket
    if(bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR){
        printf("Bind failed with error code: %d", WSAGetLastError());
    }

    puts("Bind done");

    //Listen to incoming connection
    listen(s,3);

    //Accepting and incoming connection
    puts("Waiting for incoming connections");

    c = sizeof(struct sockaddr_in);

    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
	{
		puts("Connection accepted");

        size_t fileLength = 0;
        char *fileData = read_file("home.html", &fileLength);
        printf(fileData);
        if(!fileData){
            perror("Error reading file");
            continue;
        }
		
	

        // Send HTTP header
        const char *headerFormat = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        char sendData[1000];
        strcpy(sendData, headerFormat);
        strcat(sendData, fileData);
        strcat(sendData, "\r\n");

        
        send(new_socket, sendData, strlen(sendData),0);

        //free(sendData);
        //free(fileData);
	}
	
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d" , WSAGetLastError());
		return 1;
	}


    getchar();
	
	puts("Closing socket");

	closesocket(s);
	WSACleanup();

    return 0;
}