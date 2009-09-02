#ifndef ITEM_H_
#define ITEM_H_

#define NUM_ITEMS	5
#define DESC_STR_LEN	20

enum item_classes { FOOD,
		    CORPSE,
		    SWORD,
		    SHIELD };

struct item {
	int y;
	int x;
	char desc[DESC_STR_LEN];
	char glyph;
	int item_class;
	int attr1;
	struct item *next; 
};

static struct item item_base[NUM_ITEMS] = {
	{ -1, -1, "banana", 		'%', FOOD, 	5,	NULL},
	{ -1, -1, "dwarf corpse",	'%', CORPSE, 	0,	NULL},
	{ -1, -1, "shortsword",		')', SWORD, 	10,	NULL},
	{ -1, -1, "longsword",		')', SWORD,	15,	NULL},
	{ -1, -1, "kite shield",	'[', SHIELD,	10,	NULL}
};

/*struct ilist {
	struct item *thing;
	struct ilist *next;
};*/

struct item *generate_items(int, int (*)[MAP_WIDTH]);
struct item *clone_item(struct item*);
void destroy_item(struct item*);
void free_item_list(struct item*);

#endif
