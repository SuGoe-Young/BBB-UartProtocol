#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include "uart_protocol.h"

int fd;

void uart_response_callback(const char *msg)
{
    write(fd, msg, strlen(msg));
}

int main()
{
    struct termios tty;
    fd = open("/dev/ttyS4", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) { perror("open"); return -1; }

    if (tcgetattr(fd, &tty) != 0) { perror("tcgetattr"); return -1; }

    uart_response_init(uart_response_callback);

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 1;
    tcsetattr(fd, TCSANOW, &tty);

    // === 這裡是重點 ===
    register_all_commands();

    printf("UART ready, listening...\n");

    char buf[256];
    while (1) {
        int n = read(fd, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("RX ← %s\n", buf);
            parse_command(buf);
        }
        usleep(100000);
    }

    close(fd);
    return 0;
}