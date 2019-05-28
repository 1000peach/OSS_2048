#include "InitGame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__

#include <termios.h>

int _getch() {
  	int ch;

	struct termios buf;
	struct termios save;

	tcgetattr(0, &save);
	buf = save;

	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;

	tcsetattr(0, TCSAFLUSH, &buf);

	ch = getchar();

	tcsetattr(0, TCSAFLUSH, &save);

	return ch;
}

#endif //_getch가 정의된 conio 라이브러리가 없기 때문에 _getch 함수를 직접 정의

int** allocateArr(int **arr, int size) {
	int i = 0, j = 0;

	arr = (int **)malloc(sizeof(int *)*size);
	for (i = 0; i < size; i++) {
		arr[i] = (int *)malloc(sizeof(int)*size);
	}
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			arr[i][j] = 0;
		}
	}
	return arr;
}//arr에 size만큼 메모리 할당하고 0으로 초기화

int** setUp(int **arr, int size) {
	int k = 0, i = 0, j = 0;
	int random = 0;

	key = 0;
	arr = allocateArr(arr, size);
	for (k = 0; k < 2; k++) {
		i = rand() % size;
		j = rand() % size;
		random = ((rand() % 2) + 1) * 2;
		arr[i][j] = random;
	}
	return arr;
}//메모리 할당된 arr에 랜덤 수 2개 생성

void findEmpty() {}

void refreshGame(int **cur_board, int *cur_score, int size) {
  int i = 0, j = 0;
  cur_score = 0;

  for (i = 0; i <= size - 1; i++) {
    for (j = 0; j <= size - 1; j++) {
      cur_board[i][j] = 0;
    }
  }
} //현재 GameBoard를 초기화하고 게임을 재 시작
void undo(int **cur_board, int **pre_board, int size) {
  int i = 0, j = 0;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      cur_board[i][j] = pre_board[i][j];
    }
  }
} //최근 방향키 움직임을 취소하고 이전의 상태로 복원
int isGameOver(int **cur_board, int size) {
  int i = 0, j = 0;

  for (i = 0; i <= size - 1; i++) {
    for (j = 0; j <= size - 1; j++) {
      if (cur_board[i][j] == 0) {
        return 0;
      }
    }
  }
  for (i = 0; i <= size - 2; i++) {
    for (j = 0; j <= size - 2; j++) {
      if (i == size - 1 && j != size - 1 && cur_board[i][j] == cur_board[i][j + 1]) {
        return 0;
      }
      if (j == size - 1 && i != size - 1 && cur_board[i][j] == cur_board[i + 1][j]) {
        return 0;
      }
      if (cur_board[i][j] == cur_board[i + 1][j] || cur_board[i][j] == cur_board[i][j + 1]) {
        return 0;
      }
    }
  }

  return 1;
} //반환값이 1이면 게임 오버되어 키 입력 불가

void save(int **cur_board, int **pre_board, int *cur_score_p, int *pre_score_p, int size) {
	int i = 0;
	int j = 0;

	*pre_score_p = *cur_score_p;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			pre_board[i][j] = cur_board[i][j];
		}
	}
}

void push(int **cur_board, char key, int size) {
	int k = 0;
	int i = 0;
	int j = 0;

	switch (key) {
	case 'w':
	case 'W':
		for (k = 0; k <= size - 2; k++) {
			for (j = 0; j <= size - 1; j++) {
				for (i = 0; i < size - 1; i++) {
					if (cur_board[i][j] == 0 && cur_board[i + 1][j] != 0) {
						cur_board[i][j] = cur_board[i + 1][j];
						cur_board[i + 1][j] = 0;
					}
				}
			}
		}

		break;
	case 'a':
	case 'A':
		for (k = 0; k <= size - 2; k++) {
			for (i = 0; i <= size - 1; i++) {
				for (j = size - 1; j > 0; j--) {
					if (cur_board[i][j] != 0 && cur_board[i][j - 1] == 0) {
						cur_board[i][j - 1] = cur_board[i][j];
						cur_board[i][j] = 0;
					}
				}
			}
		}

		break;
	case 's':
	case 'S':
		for (k = 0; k <= size - 2; k++) {
			for (j = 0; j <= size - 1; j++) {
				for (i = size - 1; i > 0; i--) {
					if (cur_board[i][j] == 0 && cur_board[i - 1][j] != 0) {
						cur_board[i][j] = cur_board[i - 1][j];
						cur_board[i - 1][j] = 0;
					}
				}
			}
		}
		break;
	case 'd':
	case 'D':
		for (k = 0; k <= size - 2; k++) {
			for (i = 0; i <= size - 1; i++) {
				for (j = 0; j < size - 1; j++) {
					if (cur_board[i][j] != 0 && cur_board[i][j + 1] == 0) {
						cur_board[i][j + 1] = cur_board[i][j];
						cur_board[i][j] = 0;
					}
				}
			}
		}
		break;
	}
}

void merge(int **cur_board, char key, int size) {

	int i = 0;
	int j = 0;

	switch (key) {
	case 'w':
	case 'W':
		for (j = 0; j <= size - 1; j++) {
			for (i = 0; i < size - 1; i++) {
				if (cur_board[i][j] == cur_board[i + 1][j] && cur_board[i][j] != 0) {
					cur_board[i][j] = cur_board[i + 1][j] + cur_board[i][j];
					cur_board[i + 1][j] = 0;
					i++;
				}
			}
		}
		break;
	case 'a':
	case 'A':
		for (i = 0; i <= size - 1; i++) {
			for (j = 0; j <= size - 2; j++) {
				if (cur_board[i][j] == cur_board[i][j + 1] && cur_board[i][j] != 0) {
					cur_board[i][j] = cur_board[i][j + 1] + cur_board[i][j];
					cur_board[i][j + 1] = 0;
					j++;
				}
			}
		}
		break;
	case 's':
	case 'S':
		for (j = 0; j <= size - 1; j++) {
			for (i = size - 1; i > 0; i--) {
				if (cur_board[i][j] == cur_board[i - 1][j] && cur_board[i][j] != 0) {
					cur_board[i][j] = cur_board[i - 1][j] + cur_board[i][j];
					cur_board[i - 1][j] = 0;
					i--;
				}
			}
		}
		break;
	case 'd':
	case 'D':
		for (i = 0; i <= size - 1; i++) {
			for (j = size - 1; j > 0; j--) {
				if (cur_board[i][j] == cur_board[i][j - 1] && cur_board[i][j] != 0) {
					cur_board[i][j - 1] = cur_board[i][j - 1] + cur_board[i][j];
					cur_board[i][j] = 0;
					j--;
				}
			}
		}
		break;
	}
}

int checkMove(int **cur_board, int **pre_board, int size) {
	int i = 0;
	int j = 0;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (cur_board[i][j] != pre_board[i][j])
				return 0;
		}
	}
	return 1;
}//보드에 변경사항이 있는지 검사하고 변경사항이 있으면 조기리턴한다.

void printBoard(int **cur_board, int **pre_board, int *cur_score_p, int *pre_score_p, int *high_score_p, int size) {
	int i = 0;
	int j = 0;

	system("CLS");
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (j == 0)
				printf("|");
			printf(" %4d   |", pre_board[i][j]);
		}
		printf("\n");
	}
	printf("\nThe previous state\n");
	printf("Score:%d\nHigh Score:%d\n", *pre_score_p, *high_score_p);
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (j == 0)
				printf("|");
			printf(" %4d   |", cur_board[i][j]);
		}
		printf("\n");
	}
	updateScore(cur_board, cur_score_p, high_score_p, size);
	printf("\nThe current state\n");
	printf("Score:%d\nHigh Score:%d", *cur_score_p, *high_score_p);
}

void spawnBlock(int **cur_board, int *emptyIndex, int size) {

	int emptyTile = 0;
	int randomIndex = 0;
	int randomNum = 0;
	int i = 0;
	int j = 0;

	for (i = 0; i <= size - 1; i++) {
		for (j = 0; j <= size; j++) {
			if (cur_board[i][j] == 0) {
				emptyIndex[emptyTile] = 10 * i + j;
				emptyTile = emptyTile + 1;
			}
		}
	}
	randomIndex = rand() % emptyTile;
	i = emptyIndex[randomIndex] / 10;
	j = emptyIndex[randomIndex] % 10;
	randomNum = ((rand() % 2) + 1) * 2;
	cur_board[i][j] = randomNum;

}

int move(char key, int **cur_board, int **pre_board, int size) {
	push(cur_board, key, size);

	merge(cur_board, key, size);

	if (checkMove(cur_board, pre_board, size)) {
		return 1;
	}//블록이 전혀 움직이지 않은 경우 1을 반환하고 함수를 종료. 움직이지 않았을때 다시 키를 받도록 한다.

	push(cur_board, key, size);

}//블록을 옮기고 붙어있는 블럭을 합치고 다시 한 번 블록을 옮긴다. merge 이후 생기는 틈을 매꾼다.

void updateScore(int **cur_board, int *cur_score_p, int *high_score_p, int size) {
	int i = 0;
	int j = 0;
	int sum = 0;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			sum += cur_board[i][j];
		}
	}
	*cur_score_p = sum;
	if (*cur_score_p > *high_score_p) {
		*high_score_p = *cur_score_p;
	}
}

