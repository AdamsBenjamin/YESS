#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char yostr[] = ".yo";
    int size = sizeof(yostr);
    printf("%c %c %c", yostr[size - 2], yostr[size - 3], yostr[size - 4]);

    printf("%d", 'o' == yostr[size - 2]);

    return 0;
}
