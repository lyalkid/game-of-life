#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 25
#define COLUMNS 80
#define CELLULE 'o'
#define DEATH '.'
#define TIME 150000
void print_cel(int tmp);
void render(int array2D[ROWS][COLUMNS]);
void print_info(int generation, int speed);
void speed_control(int* speed, char ch, int* time);
void first_generation(int array2D[ROWS][COLUMNS], const int* array);
void read_numbers(int size, int* numbers);
void next_gen(int array2D[ROWS][COLUMNS], int* generation, int* cells);
int bound(int index, int size);
int search_ngh(int m[][COLUMNS], int i, int j);
int die_or_life(int array2D[ROWS][COLUMNS], int x, int y);

int main() {
    int cells = 0;
    int speed = 1;
    int flag = 1;
    int array[ROWS * COLUMNS] = {0};
    int array2D[ROWS][COLUMNS];
    read_numbers(ROWS * COLUMNS, array);
    first_generation(array2D, array);
    int generation = 0;

    if (freopen("/dev/tty", "r", stdin)) {
        initscr();
    }
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);
    int time = TIME;
    while (flag) {
        char key = getch();
        render(array2D);
        print_info(generation, speed);
        refresh();

        speed_control(&speed, key, &time);
        usleep(time);
        system("clear");
        clear();
        next_gen(array2D, &generation, &cells);
        if (key == 'x' || speed == 0 || cells == 2000) {
            flag = 0;
        }
    }
    endwin();
    if (cells == 2000) {
        printf("Жизнь остановилась на %d поколении.", generation);
    }
}

void speed_control(int* speed, char ch, int* time) {
    int dSpeed = 1;
    if (ch == '-' && *speed > 1) {
        *speed -= dSpeed;
        *time += 12500;
    } else if (ch == '=' && *time >= 10000) {
        *speed += dSpeed;
        *time -= 12500;
    }
}

void print_info(int generation, int speed) {
    printw(
        "Current generation: %d\nSpeed: %d\nTo exit, push: 'x' or 'ctrl+C' (if lagging)\nSpeed change: '-' "
        "down, '=' up\n",
        generation, speed);
}

void render(int array2D[ROWS][COLUMNS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            print_cel(array2D[i][j]);
        }
        printw("\n");
    }
}

void first_generation(int array2D[ROWS][COLUMNS], const int* array) {
    int count = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            array2D[i][j] = array[count];
            count++;
        }
    }
}

void read_numbers(int size, int* numbers) {
    for (int i = 0; i < size; i++) {
        scanf("%d", &numbers[i]);
    }
}

void print_cel(int tmp) {
    if (tmp == 1) {
        printw("%c ", CELLULE);
    } else {
        printw("%c ", DEATH);
    }
}

void next_gen(int array2D[ROWS][COLUMNS], int* generation, int* cells) {
    *cells = 0;
    int new_array2D[ROWS][COLUMNS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            new_array2D[i][j] = die_or_life(array2D, i, j);
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (array2D[i][j] == new_array2D[i][j]) {
                (*cells) += 1;
            };
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            array2D[i][j] = new_array2D[i][j];
        }
    }
    *generation += 1;
}
int bound(int index, int size) {
    int result = index;
    if (index < 0) {
        result = index + size;
    } else if (index > size) {
        result = index % size;
    }
    return result;
}

int search_ngh(int m[][COLUMNS], int i, int j) {
    int live_ngh = 0;
    for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++) {
            if (k == 0 && l == 0) continue;
            int x = (i + k + ROWS) % ROWS;
            int y = (j + l + COLUMNS) % COLUMNS;
            if (m[x][y] == 1) {
                live_ngh++;
            }
        }
    }
    return live_ngh;
}

int die_or_life(int array2D[ROWS][COLUMNS], int x, int y) {
    int neighboors = search_ngh(array2D, x, y);
    int res = 0;
    int cel = array2D[x][y];
     if (cel == 1 && (neighboors == 2 || neighboors == 3)) {
        res = 1;}

    else if (cel == 1 && neighboors < 2) {
        res = 0;
    } 
      else if (cel == 1 && neighboors > 3) {
        res = 0;
    } else if (cel == 0 && neighboors == 3) {
        res = 1;
    }
    return res;
}
