#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "item.h"

/* generates a random list of n items and returns a pointer to that list */
struct item *generate_items(int n, int (*map)[MAP_WIDTH]) {
	int i; /*, y, x;*/
	struct item *head, *ip;

	/*head = (struct item*)malloc(sizeof(struct item));
	head->next = NULL;*/
	head = clone_item(&(item_base[rand() % NUM_ITEMS]));
	do {
		head->y = rand() % MAP_HEIGHT;
		head->x = rand() % MAP_WIDTH;
	} while(map[head->y][head->x] != GROUND);

	ip = head;
	for (i = 1; i < n; ++i) {
		/*ip->next = (struct item*)malloc(sizeof(struct item));
		ip = ip->next;*/
		ip->next = clone_item(&(item_base[rand() % NUM_ITEMS]));
		do {
			ip->next->y = rand() % MAP_HEIGHT;
			ip->next->x = rand() % MAP_WIDTH;
		} while(map[ip->next->y][ip->next->x] != GROUND);
		ip = ip->next;
	}
	return head;
}

struct item *clone_item(struct item *thing) {
	struct item *ip = (struct item*)malloc(sizeof(struct item));

	if (ip == NULL)
		return NULL;
	else {
		ip->y = thing->y;
		ip->x = thing->x;
		strcpy(ip->desc, thing->desc);
		ip->glyph = thing->glyph;
		ip->item_class = thing->item_class;
		ip->attr1 = thing->attr1;
		ip->next = thing->next;
		return ip;
	}
}

void destroy_item(struct item* ip) {
	free(ip);
	ip = NULL;
}

void free_item_list(struct item* head) {
	struct item *ip, *iptemp;
	ip = head;
	while (ip != NULL) {
		iptemp = ip->next;
		/*destroy_item(ip->thing);*/
		free(ip);
		ip = iptemp;
	}
	head = NULL;
}

#ifdef TEST
int main(int argc, char *argv[])
{
	if (argc < 2)
		return 1;
	else {
		int i, n = atoi(argv[1]);
		struct item *items = generate_items(n), *ip;

		for (ip = items; ip != NULL; ip = ip->next) {
			printf("y:\t\t%d\n", ip->y);
			printf("x:\t\t%d\n", ip->x);
			printf("description:\t%s\n", ip->desc);
			printf("glyph:\t\t%c\n", ip->glyph);
			printf("item class:\t%d\n", ip->item_class);
			printf("attribute 1:\t%d\n\n", ip->attr1);
		}

		free_item_list(items);
		return 0;
	}
}
#endif
