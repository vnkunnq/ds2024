#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h> 

#define PORT 8080
#define SIZE 1024*10 

int main() {
    char *ip = "127.0.0.1";
    char buffer[SIZE];
    struct sockaddr_in server_ip, client_ip;
    int serverfd, new_socket, e;
    socklen_t addr_len = sizeof(client_ip);

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0) {
        printf("[-] socket error\n");
        return 1;
    } else {
        printf("[+] Socket of server connected\n");
    }

    server_ip.sin_family = AF_INET; // any ipv4 connect to server 
    server_ip.sin_port = htons(PORT); // open port to listen
    server_ip.sin_addr.s_addr = inet_addr(ip); // server's ip 
    
    // sockaddr is structure for every kind of ip addr 
    e = bind(serverfd, (struct sockaddr*)&server_ip, sizeof(server_ip));
    if (e < 0) {
        printf("[-] Bind error\n");
        return 1;
    }
    printf("[+] bind successfully\n");

    e = listen(serverfd, 10);
    if (e == 0) {
        printf("[+] Listening for connections...\n");
    } else if (e < 0) {
        printf("[-] Listen error\n");
        return 1;
    }

    new_socket =  accept(serverfd, (struct sockaddr*)&client_ip, &addr_len);
    if (new_socket < 0) {
        printf("[-] Error accepting connection\n");
        return 1;
    } 

    int file_fd = open("received.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (file_fd == -1) {
        printf("[-] Error opening or creating file\n");
        return 1;
    }

    ssize_t bytes_received;
    while ((bytes_received = recv(new_socket, buffer, SIZE, 0)) > 0) {
        // write(1, buffer, bytes_received); // print data on the screen 
        ssize_t bytes_written = write(file_fd, buffer, bytes_received);
        if (bytes_written != bytes_received) {
            printf("[-] Error writing to file\n");
            close(file_fd);
            close(new_socket);
            return 1;
        }
    }

    if (bytes_received == -1) {
        printf("[-] Error receiving data from client\n");
        close(file_fd);
        close(new_socket);
        return 1;
    }

    printf("\n[+] Data received and written to 'received.txt' successfully\n");

    close(file_fd);
    close(new_socket);

    return 0;
}