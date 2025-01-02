//gcc -fno-pie -no-pie -fno-stack-protector ./chall.c -o chall

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


void initialize()
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

}


void vuln()
{
    char buffer[16];
    char flag[16];
    FILE *file = fopen("./flag", "r");
    if (file == NULL) {
        perror("Error opening ./flag");
        return;
    }
    fscanf(file, "%15s", flag);
    printf("u give input, i print out your input\n");
    printf("Input: ");
    fgets(buffer, sizeof(buffer), stdin);
    printf("Your input is: ");
    printf(buffer);
    return;
}

int main()
{
    initialize();

    vuln();
    return 0;
}

