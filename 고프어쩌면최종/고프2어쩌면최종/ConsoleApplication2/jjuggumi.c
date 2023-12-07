// 2023-2 ������α׷��� ����: �޲ٹ� ����
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low �̻� high ���� ������ �߻���Ű�� �Լ�
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1;
	}

	fscanf_s(fp, "%d", &n_player);
	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player[i];
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name),
			&(p->intel), &(p->str));
		p->stamina = 100;
		p->is_alive = true;
		p->hasitem = false;
	}

	fscanf_s(fp, "%d", &n_item);
	for (int i = 0; i < n_item; i++) {
		fscanf_s(fp, "%s%d%d%d",
			item[i].name, (unsigned int)sizeof(item[i].name),
			&(item[i].intel_buf),
			&(item[i].str_buf),
			&(item[i].stamina_buf));
	}

	fclose(fp);
	return 0;
}

void intro() {
	printf("\n");
	Sleep(300);
	printf("    ###      ###  ### ###   #####    #####   ### ###  ##   ##  #######  \n");
	Sleep(300);
	printf("    ###      ###  ### ###  #######  #######  ### ###  ### ###  #######  \n");
	Sleep(300);
	printf("    ###      ###  ### ###  ###      ###      ### ###  #######    ###     \n");
	Sleep(300);
	printf("### ###  ### ###  ### ###  #######  #######  ### ###  #######    ###    \n");
	Sleep(300);
	printf("### ###  ### ###  ### ###  ### ###  ### ###  ### ###  ### ###    ###    \n");
	Sleep(300);
	printf("#######  #######  #######  #######  #######  #######  ### ###  #######  \n");
	Sleep(300);
	printf(" #####    #####    #####    #####    #####    #####   ### ###  #######  \n");
	Sleep(300);
	printf("\n");
	Sleep(300);
	printf("��");
	Sleep(200);
	printf(" ���ӿ� ");
	Sleep(200);
	printf("�����Ͻðڽ��ϱ�?");
	Sleep(1500);
}

void ending1() {
	system("cls");
	printf("=========================================================================\n\n");
	printf("                       �޲ٹ� ������ ����ڴ� �ٷ�\n\n");
	printf("=========================================================================\n");
	Sleep(1000);

}
void ending2() {
	int winner = -1;

	for (int i = 0; i < n_player; i++)
		if (player[i].is_alive)
			winner = player[i].id;
	system("cls");

	printf("=========================================================================\n\n");
	printf("                       �޲ٹ� ������ ����ڴ� �ٷ�\n");
	printf("                     %d�� �÷��̾��Դϴ�! ���ϵ帳�ϴ�!\n\n", winner);
	printf("=========================================================================\n");
	Sleep(2000);

}

void ending3() {
	system("cls");
	printf("=========================================================================\n\n");
	printf("����ڸ� ������ ���߽��ϴ�.\n�����ڴ� ");
	for (int i = 0; i < n_player; i++) {
		if (player[i].is_alive) {
			printf("%d�� ", i);
			Sleep(1000);
		}
	}
	printf("�Դϴ�.\n\n");
	printf("=========================================================================\n");
	Sleep(3000);
}

void ending() {
	if (n_alive == 1) {
		ending1();
		ending2();
	}
	else {
		ending3();
	}
}

int main(void) {
	if (jjuggumi_init() == -1)
		return -1;
	intro();
	//sample();
	mugunghwa();
	nightgame();
	juldarigi();
	jebi();
	ending();
	return 0;
}
