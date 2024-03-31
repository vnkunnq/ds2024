#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define SIZE 1024

int main() {
    char *ip = "127.0.0.1"; // ip
    char buffer[SIZE];
    struct sockaddr_in server_ip;
    int clientfd, e;
    FILE *file;

    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd < 0) {
        printf("[-] socket error\n");
        return 1;
    } else {
        printf("[+] Socket of server connected\n");
    }

    server_ip.sin_family = AF_INET; // any ipv4 connect to server 
    server_ip.sin_port = htons(PORT); // open port to listen
    server_ip.sin_addr.s_addr = inet_addr(ip); // server's ip 
    
    // sockaddr is structure for every kind of ip addr 
    e = connect(clientfd, (struct sockaddr*)&server_ip, sizeof(server_ip));
    if (e == -1) {
        printf("[-] error connection\n");
        return 1;
    }
    printf("[+] connect to server successfully\n");
    
    file = fopen("sent_file.txt", "r"); // open and read
  

    if (file == NULL) {
        printf("[-] error opening file\n");
        return 1;
    } else {
        ssize_t bytes_read;
        while ((bytes_read = fread(buffer, 1, SIZE, file)) > 0) {
            ssize_t bytes_sent = write(clientfd, buffer, bytes_read);
            if (bytes_sent == -1) {
                printf("[-] Error sending data to server\n");
                fclose(file); // Close the file descriptor
                close(clientfd); // Close the socket
                return 1;
            } else if (bytes_sent < bytes_read) {
                printf("[+] Data sent partially to server\n");
            } else {
                printf("[+] Data sent successfully to server\n");
            }
        }
        fclose(file);
    }

    // close socker
    close(clientfd);

    return 0;
}