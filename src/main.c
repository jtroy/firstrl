/*
 * main.c
 * Roguelike-like multiplayer (eventually) rpg frontend (eventually)
 *
 * John Troy
 */

#include <stdlib.h>
#include <curses.h>
#include <string.h>

#include "main.h"
#include "item.h"
#include "actor.h"

#define MSG_STR_LEN 80

struct message {
	char text[MSG_STR_LEN];
	struct message * next;
} *msgcurr = NULL, *msghead = NULL;

int action_door(int, int, int (*)[MAP_WIDTH]);

void add_message(char*);
void draw_message();
void finish(char*);

int main()
{
	initscr(); cbreak(); noecho(); curs_set(0);
	if (COLS < 80 || LINES < 24) {
		finish("Console must be at least 80x24; quitting.");
	}

	msghead = (struct message *)malloc(sizeof(struct message));
	msghead->next = NULL;
	msgcurr = msghead;

	int j, i;
	struct actor player;

	/* read the map */
	int map[MAP_HEIGHT][MAP_WIDTH];
	int viewport[FIELD_Y][FIELD_X];
	FILE *fp;
	if ((fp = fopen("map", "r")) == NULL)
		finish("Couldn't open map file.");


	for (j = 0; j < MAP_HEIGHT; j++)
		for (i = 0; i < MAP_WIDTH; i++)
			if (fscanf(fp, "%d", &map[j][i]) == EOF)
				finish("Map size doesn't match");
	if (fclose(fp) != 0)
		finish("Couldn't close map file");

	/* get the player's starting position */
	for (i = 0; i < MAP_WIDTH; i++)
		for (j = 0; j < MAP_HEIGHT; j++)
			if (map[j][i] == START_POS) {
				player.y = j;
				player.x = i;
				player.inv_items = 0;
				map[j][i] = GROUND;
			}
		
	/* generate the items for the level */
	struct item *ip, *ground_items = generate_items(21, map);
	
	/* single item test
	 * struct item *ip, *ground_items = (struct item *)malloc(sizeof(struct item));
	ground_items->y = 55;
	ground_items->x = 30;
	ground_items->glyph = ']';
	ground_items->next = NULL;
	*/
				
	char key = getch();
	int view_y, view_x;
	for (;;) {
		/* create map viewport for current player position and current map*/
		for (i = 0; i < FIELD_X; i++) {
			for (j = 0; j < FIELD_Y; j++) {
				view_y = player.y - FIELD_Y/2 + j;
				view_x = player.x - FIELD_X/2 + i;
				if (view_y < 0 || view_x < 0)
					viewport[j][i] = 0;
				else if (view_y >= MAP_HEIGHT || view_x >= MAP_WIDTH)
					viewport[j][i] = 0;
				else
					viewport[j][i] = map[view_y][view_x];
			}
		}

		/* draw the viewport we just made */
		for (i = 0; i < FIELD_X; i++) {
			for (j = 0; j < FIELD_Y; j++) {
				switch (viewport[j][i]) {
					case NOTHING:
						mvaddch(j + OFFSET_Y, i + OFFSET_X, ' ');
						break;
					case GROUND:
						mvaddch(j + OFFSET_Y, i + OFFSET_X, '.');
						break;
					case WALL:
						if ((i - 1 >= 0 && viewport[j][i - 1] == WALL) ||
						    (i + 1 < FIELD_X && viewport[j][i + 1] == WALL))
						    	mvaddch(j + OFFSET_Y, i + OFFSET_X, '-');
						else
							mvaddch(j + OFFSET_Y, i + OFFSET_X, '|');
						break;
					case CLOSED_DOOR:
						mvaddch(j + OFFSET_Y, i + OFFSET_X, '+');
						break;
					case OPEN_DOOR:
						if ((i - 1 >= 0 && viewport[j][i - 1] == WALL) ||
						    (i + 1 < FIELD_X && viewport[j][i + 1] == WALL))
						    	mvaddch(j + OFFSET_Y, i + OFFSET_X, '|');
						else
							mvaddch(j + OFFSET_Y, i + OFFSET_X, '-');
						break;
					/* default:  it's an item or monster 
						mvaddch(j + OFFSET_Y, i + OFFSET_X, banana->glyph);
						break; */
				}
			}
		}
		for (ip = ground_items; ip != NULL; ip = ip->next)
			if (ip->y > player.y - FIELD_Y/2 &&
			    ip->y < player.y + FIELD_Y/2 &&
			    ip->x > player.x - FIELD_X/2 &&
			    ip->x < player.x + FIELD_X/2)
			    	mvaddch(ip->y - player.y + FIELD_Y/2 + OFFSET_Y, 
					ip->x - player.x + FIELD_X/2 + OFFSET_X, 
					ip->glyph);
		
		/* put the player in its spot and draw it all onscreen */
		mvaddch(FIELD_Y/2 + OFFSET_Y, FIELD_X/2 + OFFSET_X, '@');
		mvprintw(1, 0, "%3d\t%3d", player.y, player.x);
		/* blank message line */
		for (i = MESSAGE_X; i < MSG_STR_LEN; i++)
			mvaddch(MESSAGE_Y, i, ' ');
		draw_message();
		refresh();
		
		/* grab and parse keypresses*/
		switch (key = getch()) {
			case 'q':
				finish("");
				break;
			case 'h':
				if (player.x > 0)
					move_player(&player, player.y, player.x - 1, map);
				break;
			case 'j':
				if (player.y < MAP_HEIGHT - 1)
					move_player(&player, player.y + 1, player.x, map);
				break;
			case 'k':
				if (player.y > 0)
					move_player(&player, player.y - 1, player.x, map);
				break;
			case 'l':
				if (player.x < MAP_WIDTH - 1)
					move_player(&player, player.y, player.x + 1, map);
				break;
			case 'y':
				if (player.y > 0 && player.x > 0)
					move_player(&player, player.y - 1, player.x - 1, map);
				break;
			case 'u':
				if (player.y > 0 && player.x < MAP_WIDTH - 1)
					move_player(&player, player.y - 1, player.x + 1, map);
				break;
			case 'b':
				if (player.y < MAP_HEIGHT - 1 && player.x > 0)
					move_player(&player, player.y + 1, player.x - 1, map);
				break;
			case 'n':
				if (player.y < MAP_HEIGHT - 1 && player.x < MAP_WIDTH - 1)
					move_player(&player, player.y + 1, player.x + 1, map);
				break;
			case '.':
				move_player(&player, player.y, player.x, map);
				break;
			case ',':
				pickup(&player, ground_items);
				break;
			case 'o':
				add_message("Open door in which direction?");
				draw_message();
				refresh(); /* remove after making a redraw function or cleaning this up */
				int door_y = player.y, door_x = player.x;
				switch (key = getch()) {
					case 'h':
						door_x = player.x - 1;
						break;
					case 'j':
						door_y = player.y + 1;
						break;
					case 'k':
						door_y = player.y - 1;
						break;
					case 'l':
						door_x = player.x + 1;
						break;
					default:
						add_message("Invalid direction.");
						break;
				}
				switch(action_door(door_y, door_x, map)) {
					case -1: /*no door*/
						add_message("That's not a door!");
						break;
					case 0: /*opened it*/
						add_message("You open the door.");
						break;
					case 1: /*closed it*/
						add_message("You close the door.");
						break;
				}
				break;
			case 'm':	/* draw lots of messages for debugging */
				add_message("1");
				add_message("2");
				add_message("3");
				add_message("4");
				add_message("5");
				add_message("6");
				add_message("7");
				add_message("8");
				add_message("9");
				add_message("10");
				break;
		}
	}
}


int action_door(int y, int x, int (*map)[MAP_WIDTH]) {
	if ((y >= 0 && x >= 0 && y < MAP_HEIGHT && x < MAP_WIDTH) && map[y][x] == CLOSED_DOOR) {
		map[y][x] = OPEN_DOOR;
		return 0;
	} else if ((y >= 0 && x >= 0 && y < MAP_HEIGHT && x < MAP_WIDTH) && map[y][x] == OPEN_DOOR) {
		map[y][x] = CLOSED_DOOR;
		return 1;
	} else
		return -1;
}
		
void add_message(char new_message[]) {
	for (msgcurr = msghead; msgcurr->next != NULL; msgcurr = msgcurr->next)
		; /* traverse to the end */

	msgcurr->next = (struct message *)malloc(sizeof(struct message));
	strncpy(msgcurr->next->text, new_message, MSG_STR_LEN);
	msgcurr->next->next = NULL;
}

void draw_message() {
	/*char message[MSG_STR_LEN] = '\0';
	for (msgcurr = msghead->next; msgcurr != NULL; msgcurr = msgcurr->next) {
		if (strlen(message) + strlen(msgcurr->text) + 1 < MSG_STR_LEN) {
			strcat(message, msgcurr->text);
			msghead->next = msgcurr->next
		} else
			mvprintw(MESSAGE_Y, MESSAGE_X, message);
	*/

	/* make this mess look more like C linked list idioms */
	msgcurr = msghead->next;
	if (msgcurr != NULL) {
		mvprintw(MESSAGE_Y, MESSAGE_X, msgcurr->text);
		msghead->next = msgcurr->next;
		free(msgcurr);
		msgcurr = NULL;
	}
}

void finish(char message[]) {
	endwin();
	printf("%s\n", message);
	exit(0);
}
