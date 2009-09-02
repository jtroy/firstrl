#include <stdio.h>

enum tiles {    START_POS = -3,
		WALL,
		CLOSED_DOOR,
		NOTHING,
		GROUND,
		OPEN_DOOR};

const int MAP_HEIGHT = 60;
const int MAP_WIDTH = 120;

int main()
{
	int i, j;
	char c;
	for (j = 0; j < MAP_HEIGHT; j++) {
		for (i = 0; i <= MAP_WIDTH; i++) 
			switch (c = getchar()) {
				case '.':
					printf("%d ", GROUND);
					break;
				case '#':
					printf("%d ", WALL);
					break;
				case ' ':
					printf("%d ", NOTHING);
					break;
				case 's':
					printf("%d ", START_POS);
					break;
				case '+':
					printf("%d ", CLOSED_DOOR);
					break;
				case '|':
					printf("%d ", OPEN_DOOR);
					break;
			}
		printf("\n");
	}

	return 0;
}
