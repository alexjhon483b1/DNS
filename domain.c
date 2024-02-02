#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_DNS_SERVERS 4
#define TIMEOUT 5

char* get_domain(const char* ip_address, const char** dns_servers, int num_dns_servers) {
    struct sockaddr_in sa;
    inet_pton(AF_INET, ip_address, &(sa.sin_addr));

    for (int i = 0; i < num_dns_servers; ++i) {
        int dns_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (dns_socket == -1) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }

        struct timeval timeout;
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;
        if (setsockopt(dns_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
            perror("Failed to set socket timeout");
            close(dns_socket);
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in dns_server;
        memset(&dns_server, 0, sizeof(dns_server));
        dns_server.sin_family = AF_INET;
        dns_server.sin_port = htons(53);
        inet_pton(AF_INET, dns_servers[i], &(dns_server.sin_addr));

        if (connect(dns_socket, (struct sockaddr*)&dns_server, sizeof(dns_server)) < 0) {
            perror("Connection to DNS server failed");
            close(dns_socket);
            continue;
        }

        struct hostent* host_info = gethostbyaddr((const void*)&sa.sin_addr, sizeof(sa.sin_addr), AF_INET);
        if (host_info != NULL) {
            close(dns_socket);
            return strdup(host_info->h_name);
        }

        close(dns_socket);
    }

    return strdup("Unable to find domain for the given IP address.");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ip_address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("%s\n",argv[1]);
    const char* ip_address = argv[1];

    // List of DNS servers to try
    const char* dns_servers[MAX_DNS_SERVERS] = {"8.8.8.8", "8.8.4.4", "208.67.222.222", "208.67.220.220"};

    char* domain = get_domain(ip_address, dns_servers, MAX_DNS_SERVERS);
    printf("%s\n", domain);

    free(domain);

    return 0;
}
