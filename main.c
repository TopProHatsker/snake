#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "list.h"
#include "matrix.h"

#define G_SPD 150000
#define MAP_X 30
#define MAP_Y 30
#define SNK_C '*'
#define APL_C '@'


bool kbhit()
{
    struct termios term;
    tcgetattr(0, &term);

    struct termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}


int mygetch( ) {
    struct termios oldt,
    newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}


enum DIR {
    UP,
    DOWN,
    LEFT,
    RIGHT
};


void desk_print (struct matrix map) {
    if (map.c_num * map.r_num == 0 || map.str == NULL)
        return;

    //system("clear");
    //\033[L;Cf
    printf("\033[0;0f");

    printf(" ");
    for (uint64_t i = 0; i < map.c_num; i++) {
        printf("%2c", '-');
    }
    printf("\n");

    for (uint64_t i = 0; i < map.r_num; i++) {
        printf("|");
        for (uint64_t j = 0; j < map.c_num; j++) {
            printf("%2c", map.str[i][j]);
        }
        printf(" |\n");
    }

    printf(" ");
    for (uint64_t i = 0; i < map.c_num; i++) {
        printf("%2c", '-');
    }
    printf("\n");
}


void desk_print_fast (struct matrix map) {
    if (map.c_num * map.r_num == 0 || map.str == NULL)
        return;

    //system("clear");
    //\033[L;Cf
    printf(" ");
    for (uint64_t i = 0; i < map.c_num; i++) {
        printf("%c", '-');
    }
    printf("\n");

    for (uint64_t i = 0; i < map.r_num; i++) {
        printf("|%.*s |\n", MAP_X, map.str[i]);
    }

    printf(" ");
    for (uint64_t i = 0; i < map.c_num; i++) {
        printf("%c", '-');
    }
}



void ii_play (enum DIR* dir, struct point apple, list snk) {
    uint64_t hx = snk.start->pos.x;
    uint64_t hy = snk.start->pos.y;

    if (*dir == UP || *dir == DOWN) {
        if (apple.x < hx || (*dir == UP && apple.y > hy) || (*dir == DOWN && apple.y < hy)) {
            *dir = LEFT;
        }
        if (apple.x > hx) {
            *dir = RIGHT;
        }
    } else if (*dir == LEFT || *dir == RIGHT) {
        if (apple.y < hy || (*dir == LEFT && apple.x > hx) || (*dir == RIGHT && apple.x < hx)) {
            *dir = UP;
        }
        if (apple.y > hy) {
            *dir = DOWN;
        }
    }
}


bool snk_move (struct matrix* map, list* snk, enum DIR dir, struct point* apple) {
    if (map == NULL || snk == NULL || apple == NULL) {
        return 1;
    }

    uint64_t hx = snk->start->pos.x;
    uint64_t hy = snk->start->pos.y;

    bool need_move_tail = true;

    if (hx == apple->x && hy == apple->y) { // Eat an apple; not delete tail
        need_move_tail = false;
    }

    struct point new_head = {
        .x = hx,
        .y = hy
    };

    switch (dir) {
        case UP   : new_head.y--; break;
        case DOWN : new_head.y++; break;
        case LEFT : new_head.x--; break;
        case RIGHT: new_head.x++; break;
        default: return 1;
    }

    if (
            new_head.x < 0 ||
            new_head.y < 0 ||
            new_head.x >= MAP_X ||
            new_head.y >= MAP_Y
    ) {
        return 1;
    }

    l_add_first(snk, new_head);
    map->str[new_head.y][new_head.x] = SNK_C;

    if (need_move_tail) {
        map->str[snk->end->pos.y][snk->end->pos.x] = ' ';
        l_del(snk, snk->end);
    } else {
        // GEN NEW APPLE
        struct point new_apple = {
            .x = rand() % (MAP_X - 10) + 5,
            .y = rand() % (MAP_Y - 10) + 5,
        };
        *apple = new_apple;
        map->str[apple->y][apple->x] = APL_C;
    }

    return l_check_coll(snk);   // If all OK. Can check collision;
}

void play(bool is_auto) {
    srand(time(NULL));

    // INIT MAP
    struct matrix map = mtr_create();
    mtr_free(&map);
    mtr_init(&map, MAP_X, MAP_Y);

    // INIT SNAKE
    list snake = l_create();
    l_free(&snake);
    struct point start_point = {
        .x = MAP_X / 2,
        .y = MAP_Y / 2
    };
    l_add(&snake, start_point);
    map.str[start_point.y][start_point.x] = SNK_C;

    enum DIR dir = UP;

    struct point apple = {
        .x = MAP_X/3,
        .y = MAP_Y/3
    };
    map.str[apple.y][apple.x] = APL_C;


    while (!snk_move(&map, &snake, dir, &apple)) {
        desk_print(map);
        printf("\n\tScore: %lu", l_len(&snake));
        //desk_print_fast(map);
        fflush(stdout);
        usleep(G_SPD - 0.75 * G_SPD * is_auto);  // Boost, when auto play!

        if (is_auto) {
            ii_play(&dir, apple, snake);
        } else {
            if (kbhit()) {
                switch (mygetch()) {
                    case 'w': dir = UP; break;
                    case 'a': dir = LEFT; break;
                    case 's': dir = DOWN; break;
                    case 'd': dir = RIGHT; break;
                    default: break;
                }
            }
        }
    }

    printf("\nEND!\n");
    l_free(&snake);
    mtr_free(&map);

}


int main () {
    system("clear");
    printf("\n\n\t\tUse WASD no Strelachkas!\n\n");
    printf("  Select mode: [A]uto or [M]anual: ");
    char choice;
    scanf("%c", &choice);
    bool is_auto = false;
    if (choice == '\n' || choice == 'A') {
        is_auto = true;
    }
    system("clear");

    while (true) {
        system("clear");
        play(is_auto);
    }

    return 0;
}
