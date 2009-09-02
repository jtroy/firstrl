#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp;
	int bin[9];	
	if((fp = fopen("smallermap", "r")) == NULL) {
		printf("Couldn't read file");
		exit(0);
	}


	int i, temp;
	for(i = 0; i < 9; i++) {
		fscanf(fp, "%d", &bin[i]);
		//bin[i] = temp;
		printf("%d \n", bin[i]);
	}

	fclose(fp);
	return(0);
}
