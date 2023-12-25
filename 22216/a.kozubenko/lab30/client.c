#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int sock_fd;
    struct sockaddr_un sockaddr;

    char *dsock_file = "./dsock_file";

    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&sockaddr, 0, sizeof(struct sockaddr_un));
    sockaddr.sun_family = AF_UNIX;
    strcpy(sockaddr.sun_path, dsock_file);

    if (connect(sock_fd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFSIZ];
    ssize_t bytes_read;
    while ((bytes_read = read(0, buffer, BUFSIZ)) > 0) {
        if (write(sock_fd, buffer, bytes_read) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

