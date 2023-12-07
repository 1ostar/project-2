#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "jjuggumi.h"
#include "canvas.h"

#define DIALOG_DURATION_SEC		4

void draw(void);
void print_status(void);

// (zero-base) row행, col열로 커서 이동
void gotoxy(int row, int col) {
	COORD pos = { col,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// row행, col열에 ch 출력
void printxy(char ch, int row, int col) {
	gotoxy(row, col);
	printf("%c", ch);
}

void map_init(int n_row, int n_col) {
	// 두 버퍼를를 완전히 비우기
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		back_buf[i][0] = back_buf[i][N_COL - 1] = '*';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '*' : ' ';
		}
	}
}

void clear_map(int n_row, int n_col) {
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}
}

// back_buf[row][col]이 이동할 수 있는 자리인지 확인하는 함수
bool placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

// 상단에 맵을, 하단에는 현재 상태를 출력
void display(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // 추가로 표시할 정보가 있으면 맵과 상태창 사이의 빈 공간에 출력
}

void draw(void) {
	for (int row = 0; row < N_ROW; row++) {
		for (int col = 0; col < N_COL; col++) {
			if (front_buf[row][col] != back_buf[row][col]) {
				front_buf[row][col] = back_buf[row][col];
				printxy(front_buf[row][col], row, col);
			}
		}
	}
}

void dialog(char message[], int row, int col) {
	int msg_len = strlen(message);
	char temp_buf[ROW_MAX][COL_MAX];
	for (int i = row / 2 - 1; i < row / 2 + 2; i++) {
		for (int j = col / 5; j < msg_len + 15; j++) {
			temp_buf[i][j] = back_buf[i][j];
		}
	}

	char msg[COL_MAX] = "";
	for (int i = 0; i < msg_len; i++) {
		if (message[i] == '\0')
			break;
		msg[i] = message[i];
	}
	for (int i = DIALOG_DURATION_SEC; i > 0; i--) {
		gotoxy(row / 2 - 1, col / 5);
		for (int i = 0; i < msg_len + 5; i++) {
			printf("*");
		}
		gotoxy(row / 2, col / 5);
		printf("*%d %s *\n", i, msg);
		gotoxy(row / 2 + 1, col / 5);
		for (int i = 0; i < msg_len + 5; i++) {
			printf("*");
		}
		Sleep(1000);
	}

	for (int i = row / 2 - 1; i < row / 2 + 2; i++) {
		for (int j = col / 5; j < msg_len + 15; j++) {
			gotoxy(i, j);
			printf("%c", temp_buf[i][j]);
		}
	}
	draw();
}
