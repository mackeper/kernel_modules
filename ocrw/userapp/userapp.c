#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/ocrw"

int main()
{
        int i, fd;
        char ch, write_buf[100], read_buf[100];

        fd = open(DEVICE, O_RDWR); //open for read and write.

        if (fd < 0) {
                printf("File %s cannot be opened.\n", DEVICE);
        }

        printf("Enter 'r' to read or 'w' to write: ");
        scanf("%c", &ch);
        
        switch(ch) {
                case 'w':
                        printf("Write: ");
                        scanf(" %[^\n]", write_buf);
                        write(fd, write_buf, sizeof(write_buf));
                        break;
                case 'r':
                        read(fd, read_buf, sizeof(read_buf));
                        printf("read: %s\n", read_buf);
                        break;
                default:
                        printf("Command not recognized.\n");
                        break;
        }

        close(fd);

        return 0;
}
