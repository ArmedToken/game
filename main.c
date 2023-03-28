#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

char CURRENT_PLAYER_NAME[80];
char PLAYER_NAMES[10][80];
char arr[20][20];
char SC_CHAR = 'P';
char SC_GOLD = 'G';
char SC_TRAP = 'T';
char SC_SURINGE = 'S';
char SC_KOBOLT = 'K';
char SC_CHEST = 'C';
char SC_SPACE = ' ';
char SC_WALL = 'W';
char SC_NEXTSTEP;
char SC_EXIT = 'E';
char SC_BOSS = 'B';
char SC_MERCHANT = 'M';
char *SC_LAST_EVENT = "";
char RATING_FILE[120];
char LEVEL1_FILE[120];
char MERCHANT_GOODS[4];

int POS_X = 1;
int POS_Y = 2;
int CURRENT_PLAYER_SCORE;
int PLAYER_SCORES[10];
int IF_EXIT = 0;
int CHAR_GOLD = 150;
int CHAR_HP = 200;
int CHAR_SOURCE = 10;
int CHAR_MAXHP = 250;
int chouse;
int atk = 15;
int arm = 5;
int m_atk = 10;
int NUMBER_OF_TURNS = 0;
unsigned int rating_line_count;


#define MAX_LINE_LENGTH 80

#define AC_WHITE "\x1b[38;2;250;250;250m"
#define AC_LIGHT_GRAY "\x1b[38;2;160;160;160m"
#define AC_GREEN "\x1b[32m"
#define AC_DARK_GRAY "\x1b[38;2;100;100;100m"
#define AC_NORMAL "\x1b[m"
#define AC_YELLOW "\x1b[33m"
#define AC_DARK_YELLOW "\x1b[38;2;245;127;23m"
#define AC_BLACK "\x1b[30m"
#define AC_BLUE "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN "\x1b[36m"
#define AC_RED "\x1b[31m"

int win(char n, char m) {
    if (n == m) {
        IF_EXIT = 1;
    }
}

/* https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way */
void trim(char * s) {
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}
void set_file_names(){
    char root_dir[PATH_MAX];
    getcwd(root_dir, sizeof(root_dir));


    sprintf(RATING_FILE, "%s%s", root_dir, "\\Rating");
    printf(RATING_FILE);
    sprintf(LEVEL1_FILE, "%s%s", root_dir, "\\level_1");
}

void read_Rating(){

    char line[MAX_LINE_LENGTH] = {0};


    FILE *file = fopen(RATING_FILE, "r");
    rating_line_count = 0;
    int record_count = 0;
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        if (record_count % 2 == 0){
            trim(line);
            strcpy(PLAYER_NAMES[rating_line_count], line);
        } else {
            PLAYER_SCORES[rating_line_count] = atoi(line);
            rating_line_count++;
        }
        record_count++;
    }

    fclose(file);
}

void show_Rating(){
    system("cls");
    printf("%sHALL OF FAME%s\n\n", AC_YELLOW, AC_NORMAL);
    for (int i = 0; i < rating_line_count; i++){
        printf("%d %s %d\n", i+1, PLAYER_NAMES[i], PLAYER_SCORES[i]);
    }
}

void merchant(){

};

void print_game_screen(char map[20][20]) {
    system("cls");

    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            if (map[x][y] == SC_CHAR) {
                printf("%s%c%s", AC_GREEN, SC_CHAR, AC_NORMAL);
            } else if (map[x][y] == SC_MERCHANT) {
                printf("%s%c%s", AC_GREEN, SC_MERCHANT, AC_NORMAL);
            } else if (map[x][y] == SC_GOLD) {
                printf("%s%c%s", AC_YELLOW, SC_GOLD, AC_NORMAL);
            } else if (map[x][y] == SC_WALL) {
                printf("%s%c%s", AC_DARK_GRAY, SC_WALL, AC_NORMAL);
            } else if (map[x][y] == SC_TRAP) {
                printf("%s%c%s", AC_BLACK, SC_TRAP, AC_NORMAL);
            } else if (map[x][y] == SC_KOBOLT) {
                printf("%s%c%s", AC_RED, SC_KOBOLT, AC_NORMAL);
            } else if (map[x][y] == SC_SURINGE) {
                printf("%s%c%s", AC_CYAN, SC_SURINGE, AC_NORMAL);
            } else if (map[x][y] == SC_BOSS) {
                printf("%s%c%s", AC_RED, SC_BOSS, AC_NORMAL);
            } else if (map[x][y] == SC_CHEST) {
                printf("%s%c%s", AC_DARK_YELLOW, SC_CHEST, AC_NORMAL);
            } else {
                printf("%c", map[x][y]);
            }
        }

        printf("%s", "\n");
    }
    printf("\n");

    if (SC_LAST_EVENT != "") {
        printf("%s\n\n", SC_LAST_EVENT);
    }

    printf("%s", "HP: ");
    printf("%d", CHAR_HP);
    printf("%s", " from ");
    printf("%d", CHAR_MAXHP);
    printf("%s", "\nYou have ");
    printf("%d", CHAR_GOLD);
    printf("%s", " gold \n \n");

    if (SC_LAST_EVENT != "") {
        Beep(500, 450);
    }

}

void load_level(char map[20][20], char filename[]) {
    char ch;
    int y = 0, x = 0;
    int counter = 0;
    FILE *fp;

    fp = fopen(filename, "r");

    do {
        ch = fgetc(fp);
        if ((ch == '\n') || (ch == '\r') || (ch == EOF)) {
            continue;
        }
        y = counter % 20;
        x = counter / 20;
        counter++;
        map[x][y] = ch;

    } while (ch != EOF);

}
void sort_rating_table(char * name, int score){
    for (int x = 0; x < 9; x++){
        if (score < PLAYER_SCORES[x] || PLAYER_SCORES[x] == 0){
            for (int i = 9; i > x; i--){
                PLAYER_SCORES[i] = PLAYER_SCORES[i-1];
                for(int j = 0; j < 80; j++){
                    PLAYER_NAMES[i][j] = PLAYER_NAMES[i-1][j];
                }
            }
            PLAYER_SCORES[x] = score;
            for(int g = 0; g < 80; g++){
                PLAYER_NAMES[x][g] = name[g];
            }
            if (rating_line_count < 10){
                rating_line_count++;
            }
            break;

        }
    }
}

void save_rating(){
    FILE *file = fopen(RATING_FILE, "w");
    for(int i = 0; i < rating_line_count; i++){
        char score[80];
        itoa(PLAYER_SCORES[i], score, 10);

        fputs(PLAYER_NAMES[i], file);
        fputs("\n", file);

        fputs(score, file);
        fputs("\n", file);
    }
    fclose(file);
}
void move_char(int delta_X, int delta_Y){
    if (arr[POS_X + delta_X][POS_Y + delta_Y] != SC_WALL) {
        SC_NEXTSTEP = arr[POS_X + delta_X][POS_Y + delta_Y];
        arr[POS_X + delta_X][POS_Y + delta_Y] = SC_CHAR;
        arr[POS_X][POS_Y] = SC_SPACE;
        POS_X = POS_X + delta_X;
        POS_Y = POS_Y + delta_Y;
        if (win(SC_NEXTSTEP, SC_EXIT) == 1) {
            return;
        }
        if (SC_NEXTSTEP == SC_GOLD) {
            CHAR_GOLD += 20;
            SC_LAST_EVENT = "You picked up a handful of gold";
        }
        if (SC_NEXTSTEP == SC_TRAP) {
            CHAR_HP -= 20;
            SC_LAST_EVENT = "You stepped into trap";
        }
        if (SC_NEXTSTEP == SC_SURINGE) {
            if (CHAR_HP + 100 < CHAR_MAXHP){
            CHAR_HP += 100;
            } else{
              CHAR_HP = CHAR_MAXHP;
            }
            SC_LAST_EVENT = "You picked up a healing suringe";
        }
        if (SC_NEXTSTEP == SC_CHEST) {
            SC_LAST_EVENT = "You found a chest. There was 200 G";
            CHAR_GOLD += 200;
        }
        if (SC_NEXTSTEP == SC_KOBOLT) {
            SC_LAST_EVENT = "You killed the kobolt";
            CHAR_GOLD += 25;
            CHAR_HP -= 50;
        }
        if (SC_NEXTSTEP == SC_BOSS) {
            SC_LAST_EVENT = "You killed boss. Congratulations";
            CHAR_GOLD += 250;
            CHAR_HP -= 100;
        }
        if (SC_NEXTSTEP == SC_MERCHANT){
            system("cls");

            printf("%s", "Welcome stranger. What are you buying? \n");
            printf("%s", "1 - Syringe. Cost: 50 G \n");
            printf("%s", "2 - Source potion. Cost: 100 G \n");
            scanf("%d", &chouse);
        }
    }
}
void run_game() {
    IF_EXIT = 0;
    NUMBER_OF_TURNS = 0;
    CHAR_GOLD = 150;
    CHAR_HP = 200;
    CHAR_MAXHP = 250;
    atk = 15;
    m_atk = 10;
    arm = 5;
    MERCHANT_GOODS[0] = 2;
    MERCHANT_GOODS[1] = 1;
    MERCHANT_GOODS[2] = 1;
    MERCHANT_GOODS[3] = 1;

    load_level(arr, LEVEL1_FILE);
    arr[1][2] = SC_CHAR;


    int key;

    system("cls");
    chouse = 0;
    printf("Insert tour name: \n");
    scanf("%s", CURRENT_PLAYER_NAME);

    system("cls");
    printf(
           "You are the brave hero %s of Ithilid and by order of the king you went into the dungeon to look for a magical artifact: the eye of Inos. The dungeon greeted you not very friendly. When you met an acid snake, you were sprayed with acid and your armor became unusable. Well, you have to continue the hike like this. \n\n", CURRENT_PLAYER_NAME);
    printf("%s", "You met a merchant \n\n");
    printf("%s", "Welcome stranger. What are you buying? \n");
    printf("%s", "1 - ring of health. Cost: 70 G \n");
    printf("%s", "2 - grindstone. Cost: 100 G \n");
    printf("%s", "3 - tome of magic. Cost: 110 G \n");
    printf("%s", "4 - leather armor. Cost: 90 G \n");
    scanf("%d", &chouse);
    if (chouse == 1) {
        printf("%s", "You picked ring of health. Your HP bar was increased by 50\n");
        CHAR_MAXHP += 50;
        CHAR_GOLD -= 70;
    }
    if (chouse == 2) {
        printf("%s", "You picked - grindstone. Your attack was increased by 10\n");
        atk += 10;
        CHAR_GOLD -= 100;
    }
    if (chouse == 3) {
        printf("%s", "You picked - tome of magic. Your magic attack was increased by 10\n");
        m_atk += 10;
        CHAR_GOLD -= 110;
    }
    if (chouse == 4) {
        printf("%s", "You picked - leather armor. Your armor points was increased by 10 \n");
        arm += 10;
        CHAR_GOLD -= 90;
    }

    do {
        print_game_screen(arr);

        key = getch();
        if (key == 224) {
            do {
                key = getch();
            } while (key == 224);
        }
        SC_LAST_EVENT = "";

        switch (key) {
            case 72: // UP
                move_char(-1, 0);
                break;

            case 75: // LEFT
                move_char(0, -1);
                break;

            case 77: // RIGHT
                move_char(0, 1);
                break;

            case 80:  // DOWN
                move_char(1, 0);
                break;
            default:
                Beep(1, 1);

        }

        NUMBER_OF_TURNS++;

    } while (IF_EXIT == 0 && CHAR_HP > 0);



    if (IF_EXIT == 1) {
        CURRENT_PLAYER_SCORE = NUMBER_OF_TURNS;
        printf("%s", "You have completed your journey\n");
        printf( "Your score is: %d\n", CURRENT_PLAYER_SCORE);
        sort_rating_table(CURRENT_PLAYER_NAME, CURRENT_PLAYER_SCORE);
        save_rating();
    }
    if (CHAR_HP < 1) {
        printf("%s", "You died in combat\n");
    }
    printf("Press any key to exit\n");
    getch();

}

void print_main_screen() {
    do {
        system("cls");
        printf("%sIN SEARCH OF ARTIFACT%s\n\n", AC_WHITE, AC_NORMAL);
        printf("%s(C) 2023, EFIMOV VSEVOLOD FROM THE GROUP M30-103C-22%s\n\n\n", AC_LIGHT_GRAY, AC_NORMAL);
        printf("%s1) Start play\n\n%s", AC_GREEN, AC_NORMAL);
        printf("%s2) Leaderboard\n\n%s", AC_YELLOW, AC_NORMAL);
        printf("%s3) Exit\n\n%s", AC_RED, AC_NORMAL);
        printf( "\n\n");

        scanf("%d", &chouse);
        if (chouse == 1) {
            run_game();

        }
        if (chouse == 2) {
            show_Rating();
            scanf("%d", &chouse);
        }
        if (chouse == 3) {
            break;
        }
    } while(TRUE);
}

int main() {
    set_file_names();
    read_Rating();
    print_main_screen();

}