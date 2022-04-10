#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>

/* <------------------COLORS------------------>
BLACK, BLUE, GREEN, CYAN, RED, MAGENT, BROWN, LIGHTGRAY, DARKGRAY,
LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
*/


/*<------------------Text Styles------------------>
DEFAULT_FONT, TRIPLEX_FONT, SMALL_FONT, SANS_SERIF_FONT, GOTHIC_FONT, SCRIPT_FONT,
SIMPLEX_FONT, TRIPLEX_SCR_FONT, COMPLEX_FONT, EUROPEAN_FONT, BOLD_FONT
*/


// <------------------global variables------------------>

//player properties
#define P1          0
#define P2          0

// keyboard layout for p1
#define kb_esc      27
#define up          'W'
#define up2         'w'
#define down        'S'
#define down2       's'

// keyboard layout for p2
#define p2_up       'O'
#define p2_up2      'o'
#define p2_down     'L'
#define p2_down2    'l'

// window properties
#define Win_x       800
#define Win_y       600
#define Win_color   BLACK

// left bar properties
#define lb_left     10
#define lb_pos      150
#define lb_thick    20
#define lb_size     100
#define lb_color    WHITE
#define lb_dir_y    15

// right bar properties
#define RB_left     790
#define RB_pos      250
#define RB_thick    870
#define RB_size     -100
#define RB_color    WHITE
#define RB_dir_y    15

// ball properties
#define Ball_pos_x  400
#define Ball_pos_y  300
#define Ball_radius 5
#define Ball_color  CYAN
#define Ball_move_dir_x 2
#define Ball_move_dir_y 2

// game header properties
#define rgb_color_red   255.0
#define rgb_color_green 255.0
#define rgb_color_blue  0.0

typedef struct
{
    int score_p1;
    int score_p2;
}scores;

typedef struct
{
    float rgb_red;
    float rgb_green;
    float rgb_blue;
}rgb;

typedef struct
{
    int win_size_x;
    int win_size_y;
    int win_bg_color;
}window;

typedef struct
{
    int rad;
    int pos_x;
    int pos_y;
    int dir_x;
    int dir_y;
    int color;
}ball;

typedef struct
{
    int bar_left;
    int bar_pos;
    int bar_thick;
    int bar_size;
    int bar_color;
    int bar_dir;
}side_bar;

typedef struct
{
    int rb_left;
    int rb_pos;
    int rb_thick;
    int rb_size;
    int rb_color;
    int rb_dir;
}side_bar2;

// constructing graph
void initGraphics(window* gw)
{
    int GraphDriver = 0, GraphMode = 0, errorcode;

    initwindow(Win_x, Win_y, "Pong Game", 0, 0);
    errorcode = graphresult();
    if (errorcode != grOk) {  /* an error occurred */
        printf("Graphics error: %s\n", grapherrormsg(errorcode));
        printf("Press any key to halt:");
        _getch();
        exit(1);               /* terminate with an error code */
    }
}

// initialize game window properties
void GameWindow(window* win, int win_x, int win_y, int win_color)
{
    // win-> manipulation through pointer
    win->win_size_x = win_x;
    win->win_size_y = win_y;
    win->win_bg_color = win_color;
}

// drawing left bar
void Draw_Left_Bar(int bar_left, int bar_pos, int bar_thick, int bar_size, int bar_color)
{
    setcolor(bar_color);
    setfillstyle(SOLID_FILL, bar_color);
    bar(bar_left, bar_pos, bar_left + bar_thick, bar_pos + bar_size);
}

void Draw_Right_Bar(int rb_left, int rb_pos, int rb_thick, int rb_size, int rb_color)
{
    setcolor(rb_color);
    setfillstyle(SOLID_FILL, rb_color);
    bar(rb_left, rb_pos, rb_thick + rb_size, rb_pos + rb_size);
}

// drawing ball
void Draw_Ball(int ball_pos_x, int ball_pos_y, int ball_radius, int ball_color)
{
    setcolor(ball_color);
    setfillstyle(SOLID_FILL, ball_color);

    // drawing colored and filled ellipse
    // using both radius params the same because we need a circle not actually an ellipse
    fillellipse(ball_pos_x, ball_pos_y, ball_radius, ball_radius);
}

// setting background color
void Set_Bg_Color(window* win)
{
    // draw a bar which covers all window
    Draw_Left_Bar(0, 0, win->win_size_x, win->win_size_y, win->win_bg_color);
}

// initialize left bar properties
void init_left_bar(side_bar* bar, int bar_left, int bar_pos, int bar_thick, int bar_size,
int bar_color, int bar_dir)
{
    bar->bar_left = bar_left;
    bar->bar_pos = bar_pos;
    bar->bar_thick = bar_thick;
    bar->bar_size = bar_size;
    bar->bar_color = bar_color;
    bar->bar_dir = bar_dir;

    Draw_Left_Bar(bar_left, bar_pos, bar_thick, bar_size, bar_color);
}

void init_right_bar(side_bar2* bar, int rb_left, int rb_pos, int rb_thick, int rb_size,
int rb_color, int rb_dir)
{
    bar->rb_left = rb_left;
    bar->rb_pos = rb_pos;
    bar->rb_thick = rb_thick;
    bar->rb_size = rb_size;
    bar->rb_color = rb_color;
    bar->rb_dir = rb_dir;

    Draw_Right_Bar(rb_left, rb_pos, rb_thick, rb_size, rb_color);
}

// initialize ball properties
void init_ball(ball* ball, int ball_pos_x, int ball_pos_y, int ball_radius, int ball_dir_x,
int ball_dir_y, int ball_color)
{
    ball->pos_x = ball_pos_x;
    ball->pos_y = ball_pos_y;
    ball->rad = ball_radius;
    ball->dir_x = ball_dir_x;
    ball->dir_y = ball_dir_y;
    ball->color = ball_color;

    Draw_Ball(ball->pos_x, ball->pos_y, ball->rad, ball->color);
}

// print game header
void Game_Header_Text(rgb* rgb, float red, float green, float blue)
{
    setcolor(COLOR(red, green, blue));
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(200, 10, "Pong Game by Rawgreen");

    while(red != 0 && green != 0)   // text will fade away in a few sec
    {
        delay(5);
        red -= 0.5f;
        green -= 0.5f;
        setcolor(COLOR(red, green, blue));
        settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
        outtextxy(200, 10, "Pong Game by Rawgreen");
    }
}

void init_rgb(rgb* rgb, float red, float green, float blue)
{
    rgb->rgb_red = red;
    rgb->rgb_green = green;
    rgb->rgb_blue = blue;

    Game_Header_Text(rgb, rgb->rgb_red, rgb->rgb_green, rgb->rgb_blue);
}

// counting and printing scores
void Game_Score(int p1, int p2)
{
    char Scores[3];
    sprintf(Scores, "%d : %d", p1, p2);
    outtextxy(375, 10, Scores);
}

void init_game_score(scores* scr, int p1, int p2)
{
    scr->score_p1 = p1;
    scr->score_p2 = p2;

    Game_Score(scr->score_p1, scr->score_p2);
}

// initialize game properties
void init_Game(window* win, ball* ball, side_bar* bar, scores* scr, rgb* rgb, side_bar2* bar2)
{
    initGraphics(win);
    GameWindow(win, Win_x, Win_y, Win_color);
    Set_Bg_Color(win);
    init_left_bar(bar, lb_left, lb_pos, lb_thick, lb_size, lb_color, lb_dir_y);
    init_right_bar(bar2, RB_left, RB_pos, RB_thick, RB_size, RB_color, RB_dir_y);
    init_ball(ball, Ball_pos_x, Ball_pos_y, Ball_radius, Ball_move_dir_x, Ball_move_dir_y,
     Ball_color);
    init_rgb(rgb, rgb_color_red, rgb_color_green, rgb_color_blue);
    init_game_score(scr, P1, P2);
}

// game ending
void Game_end()
{
    closegraph();
}

void ball_move(window* win, ball* ball, side_bar* bar, scores* scr, side_bar2* rb)
{

    // drawing initial ball
    Draw_Ball(ball->pos_x, ball->pos_y, ball->rad, win->win_bg_color);

    // bouncing from top/bottom lines
    if((ball->pos_y - ball->rad < 0) || (ball->pos_y + ball->rad > 600))
    {
        ball->dir_y *= -1;
        printf("Ball Bounced!! pos_y = %d\n", ball->pos_y);
    }
    // score p1
    else if(ball->pos_x + ball->rad > Win_x)
    {
        // score for p1
        scr->score_p1 += 1;
        scr->score_p2 = scr->score_p2;
        Game_Score(scr->score_p1, scr->score_p2);

        // reset ball pos and wait 1 sec
        ball->pos_x = 400;
        ball->pos_y = 300;
        delay(1000);
    }
    //score p2
    else if(ball->pos_x  - ball->rad < 0)
    {
        // score for p2
        scr->score_p2 += 1;
        scr->score_p1 = scr->score_p1;
        Game_Score(scr->score_p1, scr->score_p2);

        // reset ball pos and wait 1 sec
        ball->pos_x = 400;
        ball->pos_y = 300;
        delay(1000);
    }
    // bounce from left bar
    else if(ball->pos_x == bar->bar_left + bar->bar_thick
    && ball->pos_y + ball->rad >= bar->bar_pos
    && ball->pos_y <= bar->bar_pos + bar->bar_size)
    {
        ball->dir_x *= -1;
    }
    else if(ball->pos_x == rb->rb_thick
    && ball->pos_y + ball->rad <= rb->rb_pos
    && ball->pos_y >= rb->rb_pos + rb->rb_size)
    {
        ball->dir_x *= -1;
    }
    // ball movement
    ball->pos_x -= ball->dir_x;
    ball->pos_y -= ball->dir_y;

    // drawing processed ball
    Draw_Ball(ball->pos_x, ball->pos_y, ball->rad, ball->color);
}

// side bar move mechanic
void move_side_bar(window* win, side_bar* bar, int direction, int pl_min)
{
    /*-------------------------------------on development-------------------------------------
    if(bar->bar_pos == 0 || bar->bar_pos == 400)
    {
        direction = 0;
    }else{
        direction = direction;
    }
    */

    // draw initial bar
    Draw_Left_Bar(bar->bar_left, bar->bar_pos, bar->bar_thick, bar->bar_size, win->win_bg_color);

    // bar movement
    bar->bar_pos = (pl_min * direction) + bar->bar_pos;

    // draw processed bar
    Draw_Left_Bar(bar->bar_left, bar->bar_pos, bar->bar_thick, bar->bar_size, bar->bar_color);
    printf("Left bar pos: %d \n" ,bar->bar_pos);
}

void move_rightside_bar(window* win, side_bar2* rb, int direction, int pl_min)
{
    Draw_Right_Bar(rb->rb_left, rb->rb_pos, rb->rb_thick, rb->rb_size, win->win_bg_color);

    rb->rb_pos = (pl_min * direction) + rb->rb_pos;

    Draw_Right_Bar(rb->rb_left, rb->rb_pos, rb->rb_thick, rb->rb_size, rb->rb_color);
    printf("Right bar pos: %d \n" ,rb->rb_pos);
}

// keyboard input and bar movement control
bool kb_input_left_player(window* win, side_bar* bar, ball* ball, scores* scr, side_bar2* rb)
{
    int kb_code;
    delay(10);

    if(_kbhit())
    {
        kb_code = _getch();

        if(kb_code == kb_esc)
        {
            return true;
        }
        else if(kb_code == up || kb_code == up2)
        {
            move_side_bar(win, bar, lb_dir_y, -1);
        }
        else if(kb_code == down || kb_code == down2)
        {
            move_side_bar(win, bar, lb_dir_y, 1);
        }
    }
    ball_move(win, ball, bar, scr, rb);
    return false;
}

// i tried to take 2 different input and process them differently but there is some limitations
bool kb_input_right_player(window* win, side_bar2* rb, ball* ball, scores* scr)
{
    int kb_key;
    delay(10);

    if(_kbhit())
    {
        kb_key = _getch();
        if(kb_key == p2_up || kb_key == p2_up2)
        {
            move_rightside_bar(win, rb, RB_dir_y, -1);
        }
        else if(kb_key == p2_down || kb_key == p2_down2)
        {
            move_rightside_bar(win, rb, RB_dir_y, 1);
        }
    }
}

void game_play(window* win, side_bar* bar, ball* ball, scores* scr, side_bar2* rb)
{
    bool game_over = false;

    while(!game_over)
    {
        game_over = kb_input_left_player(win, bar, ball, scr, rb);
        kb_input_right_player(win, rb, ball, scr);
        Game_Score(scr->score_p1, scr->score_p2);
    }
}

int main()
{
    window      window;
    side_bar    bar;
    side_bar2   bar2;
    ball        ball;
    scores      scr;
    rgb         rgb;

    init_Game(&window, &ball, &bar, &scr, &rgb, &bar2);
    game_play(&window, &bar, &ball, &scr, &bar2);

    Game_end();
}
