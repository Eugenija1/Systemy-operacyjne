//zamiana lseek() i read() na pread() jest równoważna, bo ostatni parametr f-cji ustawia
//wskaźnik na wybrane off_t offset i zatem odczytuje się count bajtów z deskryptora.


#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

/*[backward]*/
void backward(char *path)
{
    char s[256], c;
    int i, fd;
    off_t where;

    fd = open(path, O_RDONLY);
    where = lseek(fd, 1, SEEK_END);
    i = sizeof(s) - 1;
    s[i] = '\0';
    while (where > 0) {
        switch (pread(fd, &c, 1, where - 1)) {
            case 1:
                if (c == '\n') {
                    printf("%s", &s[i]);
                    i = sizeof(s) - 1;
                }
                if (i <= 0) {
                    errno = E2BIG;
                    /* handle the error */
                }
                s[--i] = c;
                break;
            case -1:
                /* handle the error */
                break;
            default: /* impossible */
                errno = 0;
                /* handle the error */
        }

        where--;
    }
    printf("%s", &s[i]);
    close(fd);
}

int main(int argc, char *argv[]){
    backward(argv[1]);
}
