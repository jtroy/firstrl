/* generic monster/player defines and functions
 */

#include "main.h"
#include "item.h"
#define ACTOR_INV_SIZE	20

struct actor {
	int y;
	int x;
	struct item *inventory[ACTOR_INV_SIZE];
	int inv_items;
};

int move_player(struct actor*, int, int, int (*)[MAP_WIDTH]);
int pickup(struct actor*, struct item*);
