#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PACKET_SIZE 1519
#define NUM_PACKETS 1000000
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

void swap_mac_addresses(unsigned char *packet) {
    unsigned char temp[6];
    memcpy(temp, packet, 6);
    memcpy(packet, packet + 6, 6);
    memcpy(packet + 6, temp, 6);
}

void generate_packet(unsigned char *packet) {
    // Generate a dummy packet with random data
    for (int i = 0; i < PACKET_SIZE; i++) {
        packet[i] = rand() % 256;
    }
}

void process_packet(unsigned char *packet) {
    // Process the packet (swap MAC addresses)
    swap_mac_addresses(packet);
}

void client() {
    int sockfd;
    struct sockaddr_in server_addr;
    unsigned char packet[PACKET_SIZE];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect to server");
        exit(1);
    }

    // Send packets to the server
    for (int i = 0; i < NUM_PACKETS; i++) {
        generate_packet(packet);
        send(sockfd, packet, PACKET_SIZE, 0);
        recv(sockfd, packet, PACKET_SIZE, 0);
    }

    // Close the socket
    close(sockfd);
}

void server() {
    int sockfd, client_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    unsigned char packet[PACKET_SIZE];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind socket");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, 1) < 0) {
        perror("Failed to listen");
        exit(1);
    }

    // Accept a client connection
    client_len = sizeof(client_addr);
    client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sockfd < 0) {
        perror("Failed to accept connection");
        exit(1);
    }

    // Process packets from the client
    while (1) {
        ssize_t len = recv(client_sockfd, packet, PACKET_SIZE, 0);
        if (len <= 0) {
            break;
        }
        process_packet(packet);
        send(client_sockfd, packet, len, 0);
    }

    // Close the sockets
    close(client_sockfd);
    close(sockfd);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Failed to fork");
        exit(1);
    } else if (pid == 0) {
        client();
    } else {
        server();
    }

    return 0;
}