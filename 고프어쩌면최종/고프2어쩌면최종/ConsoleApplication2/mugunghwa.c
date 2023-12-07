#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>
#pragma warning(disable:4996)

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void mugunghwa_init(void);
void move_manual(key_t key);
void move_random(int i, int dir);
void move_tail(int i, int nx, int ny);
void mugunghwa(void);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기

int row_m = 10;
int col_m = 40;

void mugunghwa_init(void) {
	clear_map(ROW_MAX, COL_MAX);
	map_init(row_m, col_m);

	int x, y;
	for (int i = 0; i < n_player; i++) {
		// 같은 자리가 나오면 다시 생성
		x = 2 + i;
		y = N_COL - 2;
		px[i] = x;
		py[i] = y;
		period[i] = randint(50, 100);

		back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
	}

	tick = 0;
}

void move_manual(key_t key) {
	// 각 방향으로 움직일 때 x, y값 delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 움직일 방향(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// 움직여서 놓일 자리
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!placable(nx, ny)) {
		return;
	}

	move_tail(0, nx, ny);

	// 스태미나 감소
	if (player[0].is_alive == true) {
		player[0].stamina = max(0, player[0].stamina - 1);
	}
}

// 0 <= dir < 4가 아니면 랜덤
void move_random(int playerIndex, int dir) {
	int p = playerIndex;  // 이름이 길어서...
	int nx, ny;  // 움직여서 다음에 놓일 자리

	// 움직일 공간이 없는 경우는 없다고 가정(무한 루프에 빠짐)	
	int move_x;
	int move_y;
	do {
		move_x = randint(0, 9);
		move_y = randint(0, 9);

		if (move_x == 0)
		{
			move_x = -1;
		}
		else if (move_x == 1)
		{
			move_x = 1;
		}
		else
		{
			move_x = 0;
		}

		if (move_y < 7)
		{
			move_y = -1;
		}
		else
		{
			move_y = 0;
		}

		nx = px[p] + move_x;
		ny = py[p] + move_y;

	} while (!placable(nx, ny));

	move_tail(p, nx, ny);

	// 스태미나 감소
	if (player[p].is_alive == true) {
		player[p].stamina = max(0, player[p].stamina - 1);
	}
}

// back_buf[][]에 기록
void move_tail(int player, int nx, int ny) {
	int p = player;  // 이름이 길어서...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

// 술래 앞에 도착했는지 판별 (1이면 도착 0이면 미도착)
int is_arrive(int p) {
	if (px[p] == 2 && py[p] == 1)
	{
		return 1;
	}
	else if (px[p] == 3 && py[p] == 2)
	{
		return 1;
	}
	else if (px[p] == 4 && py[p] == 2)
	{
		return 1;
	}
	else if (px[p] == 5 && py[p] == 2)
	{
		return 1;
	}
	else if (px[p] == 6 && py[p] == 1)
	{
		return 1;
	}

	return 0;
}

// 타이머에 맞춰 무궁화꽃이피었습니다 출력. 타이머 간격으로 시간 조절
void time_stop(int timer)
{
	if (timer == 50)
	{
		gotoxy(10, 0);
		printf("무 ");
	}
	else if (timer == 400)
	{
		gotoxy(10, 4);
		printf("궁 ");
	}
	else if (timer == 1000)
	{
		gotoxy(10, 8);
		printf("화 ");
	}
	else if (timer == 1700)
	{
		gotoxy(10, 12);
		printf("꽃 ");
	}
	else if (timer == 2500)
	{
		gotoxy(10, 16);
		printf("이 ");
	}
	else if (timer == 3250)
	{
		gotoxy(10, 20);
		printf("피 ");
	}
	else if (timer == 3750)
	{
		gotoxy(10, 24);
		printf("었 ");
	}
	else if (timer == 4100)
	{
		gotoxy(10, 28);
		printf("습 ");
	}
	else if (timer == 4350)
	{
		gotoxy(10, 32);
		printf("니 ");
	}
	else if (timer == 4500)
	{
		gotoxy(10, 36);
		printf("다 ");
	}
}

// 무궁화 꽃이 피었습니다 문구 지우기용
void clear_stop()
{
	for (int i = 0; i < 37; i += 4)
	{
		gotoxy(10, i);
		printf("  ");
	}
}

// 무궁화꽃이피었습니다 출력 후 3초 대기 시간 이전과 이후의 플레이어 좌표 비교, 다른 경우엔 각 플레이어에 해당하는 인덱스를 1로 바꾸어서 리턴함
int compare_loc(int temp_px[], int temp_py[])
{
	/*int* result = (int*)malloc(sizeof(int) * PLAYER_MAX);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		result[i] = 0;
	}*/

	int back = 0;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if ((px[i] != temp_px[i] || py[i] != temp_py[i]) || (px[i] != temp_px[i] && py[i] != temp_py[i]))
		{
			if (player[i].is_alive == true) {
				int alive = 0;
				for (int j = 0; j < PLAYER_MAX; j++) {
					if (player[j].is_alive == true) {
						if (px[i] == px[j] && py[i] > py[j]) {
							alive = 1;
							if (temp_px[i] == px[j] && temp_py[i] > py[j]) {
								if (py[j] > 1) {
									alive = 1;
								}
								else {
									alive = 0;
								}
							}
							else if (temp_px[i] != px[j] || temp_py[i] < py[j]) {
								alive = 0;
							}
						}
					}
				}
				if (alive == 0) {
					player[i].is_alive = false;
					n_alive--;
				}
				if (n_alive == 1)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

void reprint()
{
	for (int i = 0; i < n_player; i++)
	{
		if (player[i].is_alive == true)
		{
			gotoxy(px[i], py[i]);
			printf("%d", i);
		}
		else
		{
			back_buf[px[i]][py[i]] = ' ';
		}
	}

	for (int i = 0; i < N_ROW; i++)
	{
		gotoxy(i, 0);
		if (i == 0)
		{
			for (int j = 1; j < N_COL - 1; j++)
			{
				printf("*");
			}
		}
		else if (i == N_ROW - 1)
		{
			for (int j = 1; j < N_COL - 1; j++)
			{
				printf("*");
			}
		}
		else
		{
			printf("*");
			gotoxy(i, N_COL - 1);
			printf("*");
		}
	}
}

void mugunghwa(void) {
	mugunghwa_init();
	int flag = 0;

	int timer = 0;

	int temp_px[PLAYER_MAX];
	int temp_py[PLAYER_MAX];

	int* diff = (int*)malloc(sizeof(int) * PLAYER_MAX);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		temp_px[i] = px[i];
		temp_py[i] = py[i];
	}

	system("cls");
	display();

	key_t key;
	int player_log[PLAYER_MAX] = { 0 }; // dialog에 전에 탈락했던 플레이어 재출력 방지

	while (1) {
		// 술래 모양 출력
		gotoxy(3, 1);
		printf("#");
		gotoxy(4, 1);
		printf("#");
		gotoxy(5, 1);
		printf("#");
		// player 0만 손으로 움직임(4방향)
		key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			if (player[0].is_alive != false)
			{
				move_manual(key);
			}
		}

		int arrive_count = 0;
		for (int i = 0; i < n_player; i++)
		{
			if (is_arrive(i) == 1)
			{
				arrive_count++;
			}
		}

		if (n_alive == arrive_count)
		{
			break;
		}

		// player 1 부터는 랜덤으로 움직임(8방향)
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				if (is_arrive(i) == 1)
				{
					continue;
				}

				if (player[i].is_alive == true)
					move_random(i, -1);
			}
		}

		display();
		Sleep(10);
		timer += 10; // 매 반복마다 타이머 10씩 증가
		time_stop(timer);

		// 타이머가 4500에 도달한 경우 무궁화꽃이피었습니'다'가 출력되며 술래 모양이 바뀌고 움직이는 대상 감시
		if (timer >= 4500)
		{
			gotoxy(3, 1);
			printf("@");
			gotoxy(4, 1);
			printf("@");
			gotoxy(5, 1);
			printf("@");

			// 3초 동안 대기하며 무작위로 10퍼센트 확률로 이동시킴
			for (int i = 0; i < 300; i++)
			{

				for (int i = 0; i < PLAYER_MAX; i++)
				{
					temp_px[i] = px[i];
					temp_py[i] = py[i];
				}

				key = get_key();
				if (key == K_QUIT) {
					break;
				}
				else if (key != K_UNDEFINED) {
					if (player[0].is_alive != false)
					{
						move_manual(key);
					}
				}

				for (int i = 1; i < n_player; i++) {
					if (tick % period[i] == 0) {
						if (is_arrive(i) == 1)
						{
							continue;
						}

						int num = randint(0, 9);

						if (num == 0 && player[i].is_alive == true)
						{
							move_random(i, -1);
						}
						else
						{
							continue;
						}
					}
				}
				// 위치 비교
				if (!compare_loc(temp_px, temp_py))
					break;
				display();
				tick += 10;
				Sleep(10);
			}

			// 탈락한 플레이어가 있을시 플래그 표시 후 break
			for (int i = 0; i < n_player; i++)
			{
				if (player[i].is_alive == false && player_log[i] == 0)
				{
					diff[i] = 1;
					flag = 1;
				}
			}
			// 타이머 초기화 및 문구 지우기
			timer = 0;
			clear_stop();
		}
		// 탈락한 플레이어가 있는 경우
		if (flag == 1)
		{
			// 사망자 수 변수
			int die_count = 0;
			// 사망자 수 변수 확인하기, diff[i]가 1인 경우는 i 번째 플레이어가 위치가 달라져 색출되었다고 판별
			for (int i = 0; i < PLAYER_MAX; i++)
			{
				if (diff[i] == 1)
				{
					die_count++;
				}
			}
			printf("\n");

			// dialog에 문자열을 출력하기 위한 문자열 변수 선언
			char* str = (char*)malloc(sizeof(char) * 100); // 크기를 충분히 늘림
			char* str_ = (char*)malloc(sizeof(char) * 50); // 크기를 충분히 늘림
			if (!str || !str_) {
				// 메모리 할당 오류 처리
				exit(1);
			}
			sprintf(str, "player "); // sprintf 사용
			int init = 0;
			for (int i = 0; i < PLAYER_MAX; i++)
			{
				if (diff[i] == 1 && player_log[i] == 0)
				{
					if (init == 0)
					{
						sprintf(str_, "%d", i);
						init = 1;
						player_log[i] = 1;
					}
					else
					{
						char* temp = (char*)malloc(sizeof(char) * 50);
						sprintf(temp, ", %d", i);
						strcat(str_, temp);
						player_log[i] = 1;
					}
				}
			}
			strcat(str, str_);
			strcat(str, " dead!");
			dialog(str, row_m, col_m);
			flag = 0;

			reprint();

			free(str);
			free(str_);

			display();
		}

		tick += 10;
	}
}