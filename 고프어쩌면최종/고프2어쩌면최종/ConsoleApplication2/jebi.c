#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define JEBI_MAP_ROW 5
#define JEBI_MAP_COL 18

char jebies[PLAYER_MAX];
int fail, cursor;
int round = 1, turn;

void jebi_init(void) {
	system("cls");
	clear_map(ROW_MAX, COL_MAX);
	map_init(JEBI_MAP_ROW, JEBI_MAP_COL);

	int back_idx = 1;
	for (int i = 0; i < n_alive; i++) {
		back_buf[2][back_idx++] = ' ';
		back_buf[2][back_idx++] = '?';
	}

	cursor = 2;
	back_buf[2][cursor] = '@';

	turn = n_alive;
	fail = randint(1, n_alive);
}

void jebi_init_turn(void) {
	back_buf[2][turn * 2] = ' ';

	if (cursor != turn * 2)
		back_buf[2][cursor] = '?';
	turn--;
	cursor = 2;
	back_buf[2][cursor] = '@';

	fail = randint(1, turn);
}

void jebi_init_round(void) {
	back_buf[2][n_alive * 2] = ' ';
	
	int back_idx = 1;
	n_alive--;
	for (int i = 0; i < n_alive; i++) {
		back_buf[2][back_idx++] = ' ';
		back_buf[2][back_idx++] = '?';
	}
	cursor = 2;
	back_buf[2][cursor] = '@';

	fail = randint(1, n_alive);
	turn = n_alive;
	round++;
}

void print_jebi_status(int playerIndex) {
	int p = playerIndex;

	gotoxy(JEBI_MAP_ROW, 0);
	printf("round %d, turn: player %d\n", round, p);
}

void erase_cursor(void) {
	back_buf[2][cursor] = '?';
}

void draw_cursor(void) {
	back_buf[2][cursor] = '@';
}

void move_cursor_left(void) {
	erase_cursor();

	if (cursor == 2)
		cursor = turn * 2;
	else
		cursor -= 2;

	draw_cursor();
}

void move_cursor_right(void) {
	erase_cursor();

	if (cursor == turn * 2)
		cursor = 2;
	else
		cursor += 2;

	draw_cursor();
}

bool check_answer(int playerIndex) {
	int p = playerIndex;

	char message_pass[] = "player 0 pass!";
	char message_fail[] = "player 0 fail!";

	if (cursor / 2 != fail) {
		message_pass[7] = p + '0';
		dialog(message_pass, JEBI_MAP_ROW, JEBI_MAP_COL);
	}
	else {
		message_fail[7] = p + '0';
		dialog(message_fail, JEBI_MAP_ROW, JEBI_MAP_COL);
		player[p].is_alive = false;
		jebi_init_round();
		display();
		return true;
	}
	return false;
}

void jebi(void) {
	jebi_init();
	print_jebi_status(0);

	while (1) {
		for (int i = 0; i < n_player; i++) {
			if (!player[i].is_alive)
				continue;

			if (i == 0) {
				while (1) {
					key_t key;
					key = get_key();

					if (key == K_QUIT) {
						return;
					}
					else if (key == K_LEFT) {
						move_cursor_left();
					}
					else if (key == K_RIGHT) {
						move_cursor_right();
					}
					else if (key == K_CHOICE) {
						if (check_answer(0)) {
							if (n_alive == 1)
								return;
							break;
						}
						jebi_init_turn();
						display();
						Sleep(1000);
						break;
					}

					display();
					print_jebi_status(0);
				}
			}
			else {
				while (1) {
					key_t key;
					key = get_key();

					if (key == K_QUIT) {
						return;
					}

					print_jebi_status(i);

					int command = randint(1, 5);
					if (turn == 1 || command == 1) {
						if (check_answer(i)) {
							if (n_alive == 1)
								return;
							break;
						}
						jebi_init_turn();
						display();
						Sleep(1000);
						break;
					}
					else if (command <= 3)
						move_cursor_left();
					else if (command <= 5)
						move_cursor_right();
					
					display();
					Sleep(1000);
				}
			}
			if (n_alive == 1)
				return;
		}
	}
}