#include <stdio.h>
#include <unistd.h>

int main()
{
        if(access("/bin/ls", X_OK)) {
                printf("Yes\n");
        } else {
                printf("No\n");
        }
        return 0;
}

