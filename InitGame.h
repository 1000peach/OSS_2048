#pragma once

#ifdef __linux__

int _getch();

#endif

char key;

int** setUp(int **arr, int size);
int** allocateArr(int **arr, int size);

void findEmpty();

void refreshGame(int **cur_board, int *cur_score, int size);
void undo(int **cur_board, int **pre_board, int size);
int isGameOver(int **cur_board, int size);

void save(int **cur_board, int **pre_board, int *cur_score, int *pre_score, int size);

void push(int **cur_board, char key, int size);

void merge(int **cur_board, char key, int size);

void spawnBlock(int **cur_board, int *emptyIndex, int size);

int checkMove(int **cur_board, int **pre_board, int size);

void printBoard(int **cur_board, int **pre_board, int *cur_score_p, int *pre_score_p, int *high_score_p, int size);

int move(char key, int **cur_board, int **pre_board, int size);

void updateScore(int **cur_board, int *cur_score, int *high_score, int size);

