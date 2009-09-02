#include <stdlib.h>
#include <math.h>

#include "actor.h"
#include "main.h"

int move_player(struct actor *player, int new_y, int new_x, int (*map)[MAP_WIDTH]) {
	if (abs(new_y - player->y) > 1 || abs(new_x - player->x) > 1)
		return 1; /* do pathfinding or something... figure this out later */
	else if (map[new_y][new_x] > 0) {
		player->y = new_y;
		player->x = new_x;
		return 0;
	} else
		return -1;
}

/*int pickup(struct actor *player, struct item *ground_items) {
	if (player->inv_items < ACTOR_INV_SIZE) { inventory not full
		struct item *ip, *iptemp = NULL;
		for (ip = ground_items; ip != NULL; ip = ip->next) {
			if (ip->y == player->y && ip->x == player->x) {
				//if (ip == ground_items)
					
				if (iptemp == NULL)  {
					//iptemp = ip->next;
					ground_items = ground_items->next;
					ip->next = NULL;
					player->inventory[player->inv_items] = ip;
					//player->inventory[player->inv_items]->next = NULL;
					++player->inv_items;
					//ground_items = iptemp;
					return 0;
				} else {
					iptemp->next = ip->next;
					ip->next = NULL;
					player->inventory[player->inv_items] = ip;
					++player->inv_items;
					ip = iptemp;
					return 0;
				}
			}
			iptemp = ip;
		}
	} 
	return 1;
}*/

int pickup(struct actor *player, struct item *ground_items) {
	if (player->inv_items < ACTOR_INV_SIZE) { /*inventory not full*/
		struct item *ip, *iptemp = NULL;
		for (ip = ground_items; ip->next != NULL; ip = ip->next) {
			if (ip->next->y == player->y && ip->next->x == player->x) {
				iptemp = ip->next;
				ip = ip->next->next;
				iptemp->next = NULL;
				player->inventory[player->inv_items] = iptemp;
				++player->inv_items;
				return 0;
			} else if (ip->y == player->y && ip->x == player->x) {
				ip = ground_items;
				ground_items = ground_items->next;
				ip->next = NULL;
				player->inventory[player->inv_items] = ip;
				++player->inv_items;
				return 0;
			}
		}
	} 
	return 1;
}
