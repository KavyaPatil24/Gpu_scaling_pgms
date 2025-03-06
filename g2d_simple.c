#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define G2D_DEVICE "/dev/g2d"

int main() {
    int g2d_fd;

    // Open the G2D device
    g2d_fd = open(G2D_DEVICE, O_RDWR);
    if (g2d_fd < 0) {
        perror("Failed to open G2D device");
        return -1;
    }
    printf("G2D device opened successfully.\n");

    // Close the G2D device
    if (close(g2d_fd) < 0) {
        perror("Failed to close G2D device");
        return -1;
    }
    printf("G2D device closed successfully.\n");

    return 0;
}

