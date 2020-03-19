#include <stdio.h>
#include <string.h>

int validateIP4Dotted(const char *s){
	int len = strlen(s);
    if (len < 7 || len > 15)
        return 0;

    char tail[16];
    tail[0] = 0;

    unsigned int d[4];
    int c = sscanf(s, "%3u.%3u.%3u.%3u%s", &d[0], &d[1], &d[2], &d[3], tail);

    if (c != 4 || tail[0])
    {
        return 0;
    }

	int i;
    for (i = 0; i < 4; i++)
        if (d[i] > 255)
            return 0;

    return 1;
}

int mainz(){
	char ipaddress[256];
	printf("Enter IP Adress: ");
	fgets(ipaddress, sizeof(ipaddress), stdin);
	if(validateIP4Dotted(ipaddress))
		printf("Valid IP Address\n");
	else
		printf("Invalid IP Address\n");
	return(0);
}