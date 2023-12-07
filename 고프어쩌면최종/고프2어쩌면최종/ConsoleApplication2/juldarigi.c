#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define JULDARIGI_MAP_ROW 3
#define JULDARIGI_MAP_COL 23

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

double str;
int tick = 0;
int center = JULDARIGI_MAP_COL / 2;
bool player2[PLAYER_MAX] = { false };
bool lld, rld;

void juldarigi_init(void) {
	clear_map(ROW_MAX, COL_MAX);
	map_init(JULDARIGI_MAP_ROW, JULDARIGI_MAP_COL);
	
	back_buf[0][center] = ' ';
	back_buf[2][center] = ' ';

	back_buf[1][center] = '-';
	back_buf[1][center-1] = '-'; 
	back_buf[1][center+1] = '-';

	int dist = 2;
	for (int i = 0; i < n_player; i++) {
		if (i % 2 == 0)
			back_buf[1][center - dist] = '0' + i;
		else {
			back_buf[1][center + dist] = '0' + i;
			dist++;
		}
		player2[i] = true;
	}
}

double calculate_str(void) {
	double str = 0;
	for (int i = 0; i < n_player; i++) {
		if (i % 2 == 0) {
			if (player2[i])
				str -= player[i].str;
		}
		else {
			if (player2[i])
				str += player[i].str;
		}
	}
	return str;
}

void calculate_lying_down_str(int team) {
	for (int i = 0; i < n_player; i++)
		if (i % 2 == team)
			if (player2[i])
				player[i].str *= 2;
}

void end_lying_down_str(int team) {
	for (int i = 0; i < n_player; i++)
		if (i % 2 == team)
			if (player2[i])
				player[i].str /= 2;
}

void win(int playerIndex) {
	int p = playerIndex;

	if (!player[p].is_alive) {
		player[p].is_alive = true;
	}
}

void lose(int playerIndex) {
	int p = playerIndex;

	if (player[p].is_alive) {
		player[p].hasitem = false;
		player[p].item = nullItem;
		player[p].str /= 2;
		player[p].intel /= 2;
	}
	else {
		n_alive--;
	}
}

bool move_left(void) {
	char temp_buf[JULDARIGI_MAP_COL - 2];

	if (back_buf[1][center + 1] != '-') {
		int i = center + 1;
		
		if (back_buf[1][i] != ' ') {
			player2[back_buf[1][i] - '0'] = false;
			lose(back_buf[1][i] - '0');
		}
		else {
			for (int i = 0; i < n_player; i++)
				if (player2[i])
					win(i);
			return true;
		}

		for (; back_buf[1][i] != ' '; i++)
			back_buf[1][i] = back_buf[1][i + 1];
	}
	else {
		for (int i = 0; i < JULDARIGI_MAP_COL - 2; i++)
			temp_buf[i] = back_buf[1][i + 1];

		for (int i = 1; i < JULDARIGI_MAP_COL - 2; i++) {
			back_buf[1][i] = temp_buf[i];
		}
	}
	return false;
}

bool move_right(void) {
	char temp_buf[JULDARIGI_MAP_COL - 2];

	if (back_buf[1][center - 1] != '-') {
		int i = center - 1;
		
		if (back_buf[1][i] != ' ') {
			player2[back_buf[1][i] - '0'] = false;
			lose(back_buf[1][i] - '0');
		}
		else {
			for (int i = 0; i < n_player; i++)
				if (player2[i])
					win(i);
			return true;
		}

		for (; back_buf[1][i] != ' '; i--)
			back_buf[1][i] = back_buf[1][i - 1];
	}
	else {
		for (int i = 0; i < JULDARIGI_MAP_COL - 2; i++)
			temp_buf[i] = back_buf[1][i + 1];

		for (int i = 0; i < JULDARIGI_MAP_COL - 3; i++) {
			back_buf[1][i + 2] = temp_buf[i];
		}
	}
	return false;
}


void update_str(void) {
	gotoxy(12, 0);
	printf("str:                        \n");
	gotoxy(12, 0);
	printf("str:\t\t%.1f\n", str);
}

void print_juldarigi_status(void) {
	gotoxy(14, 0);
	printf("no. of players left: %d\n", n_alive);

	for (int p = 0; p < n_player; p++) {
		if (player[p].is_alive)
			printf("player %2d: alive\n", p);
		else
			printf("player %2d: alive*\n", p);
	}
}

void juldarigi(void) {
	juldarigi_init();

	system("cls");
	display();
	update_str();
	print_juldarigi_status();

	key_t key;
	str = calculate_str();

	bool gameover = false;
	while (!gameover) {
		tick++;

		if (back_buf[1][center - 1] != ' ' && back_buf[1][center + 1] != ' ') {
			key = get_key();
		}
		if (key == K_QUIT) {
			break;
		}
		else if (key == K_LPULL) {
			str--;
		}
		else if (key == K_RPULL) {
			str++;
		}
		else if (key == K_LLD) {
			char message[] = "Left team lying down.";
			dialog(message, JULDARIGI_MAP_ROW, JULDARIGI_MAP_COL);

			lld = true;
			calculate_lying_down_str(0);
			str = calculate_str();
			update_str();

			if (str < 0) {
				gameover = move_left();
				gameover = move_left();
			}
			else {
				gameover = move_right();
				gameover = move_right();
			}

			for (int i = 0; i < n_player; i++)
				if (i % 2 == 0) {
					if (player[i].stamina > 30)
						player[i].stamina -= 30;
					else
						player[i].stamina = 0;
				}
			end_lying_down_str(0);
		}
		else if (key == K_RLD) {
			char message[] = "Right  team lying down.";
			dialog(message, JULDARIGI_MAP_ROW, JULDARIGI_MAP_COL);

			rld = true;
			calculate_lying_down_str(1);
			str = calculate_str();
			update_str();

			if (str < 0) {
				gameover = move_left();
				gameover = move_left();
			}
			else {
				gameover = move_right();
				gameover = move_right();
			}

			for (int i = 0; i < n_player; i++)
				if (i % 2 == 1) {
					if (player[i].stamina > 30)
						player[i].stamina -= 30;
					else
						player[i].stamina = 0;
				}
			end_lying_down_str(1);
		}

		
		if (tick % 100 == 0) {
			if (lld || rld) {
				display();
				update_str();
				print_juldarigi_status();

				Sleep(10);
				lld = false;
				rld = false;
				continue;
			}

			if (str < 0) {
				gameover = move_left();
			}
			else {
				gameover = move_right();
			}
			str = calculate_str();
		}
;
		display();
		update_str();
		print_juldarigi_status();

		Sleep(10);
	}
}