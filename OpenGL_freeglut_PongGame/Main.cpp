#define _CRT_SECURE_NO_WARNINGS
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/freeglut_std.h>
#include <stdio.h>
#include <math.h>

// <----------------------------------- Global Variables ----------------------------------->

//player properties
int P1 = 0;

// play settings
#define kb_esc      27
static int mouse_pos_x = 0;

// window properties
static int Win_width;
static int Win_height;

// collision logic properties
#define coll_right	1
#define coll_left	2
#define coll_top	3
#define coll_bot	4

// ball properties
static float ball_x_speed = 4;
static float ball_y_speed = 4;
static float bounce = 1;

// output text
char string[30];

struct ba_pa
{
	float left;
	float pos;
	float thick;
	float size;
};

// like python lists
ba_pa ball = { 100, 100, 120, 120 };

ba_pa wall;

ba_pa paddle = { 0, 490, 40, 500 };


// draw paddle, ball
void Draw_Rectangle(ba_pa rect)
{
	glBegin(GL_QUADS);
	glVertex2f(rect.left, rect.size);
	glVertex2f(rect.thick, rect.size);
	glVertex2f(rect.thick, rect.pos);
	glVertex2f(rect.left, rect.pos);
	glEnd();
}

void Timer(int v)
{
	ball.left += ball_x_speed;
	ball.thick += ball_x_speed;
	ball.pos += ball_y_speed;
	ball.size += ball_y_speed;

	glutTimerFunc(1, Timer, 1);
}

void outtextxy(char* string, int x, int y)
{
	char* c;
	// push matrix down by one
	glPushMatrix();
	glTranslatef(x, y, 0);
	// producing nonuniform scaling along x, y, z
	glScalef(0.1, -0.1, 1);

	// moving between string indexes until index is empty
	for (c = string; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
	glPopMatrix();
}

void game_end()
{
	printf("GAME OVER");
	exit(0);
}

int ball_wall_collision(ba_pa ball, ba_pa wall)
{
	if (ball.thick >= wall.thick) return coll_right;
	if (ball.left <= wall.left) return coll_left;
	if (ball.pos <= wall.pos) return coll_top;
	if (ball.size >= wall.size) game_end();
	else return 0;
}

bool ball_paddle_collision(ba_pa ball, ba_pa paddle)
{
	if (ball.size >= paddle.pos
		&& ball.left >= paddle.left
		&& ball.thick <= paddle.size)
	{
		P1++;
		return true;
	}
	return false;
}

void kb_input(int kb_key, int x, int y)
{
	if (kb_key == kb_esc)
	{
		exit(0);
	}
}

// play settings
void mouse_motion(int x, int y)
{
	mouse_pos_x = x;
}

void gl_Settings()
{
	// specifying red, green, blue and alpha values used by
	// glClear to clear the color buffers
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// testing, idk
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Reshape(int width, int height)
{
	Win_width = width;
	Win_height = height;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// defines 2dim orthographic projection matrix
	gluOrtho2D(0, width, height, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int PCresult = 0;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	sprintf(string, "PC Result: %d", PCresult);
	outtextxy(string, 10, 80);
	sprintf(string, "Player Result: %d", P1);
	outtextxy(string, 10, 120);

	wall.left = wall.pos = 0;
	wall.thick = Win_width;
	wall.size = Win_height;

	// drawing ball
	Draw_Rectangle(ball);

	//collisions
	if (ball_wall_collision(ball, wall) == coll_right) ball_x_speed *= -(bounce);

	if (ball_wall_collision(ball, wall) == coll_left) ball_x_speed *= (bounce);

	if (ball_wall_collision(ball, wall) == coll_top) ball_y_speed *= (bounce);

	if (ball_wall_collision(ball, wall) == coll_bot)
	{
		ball_y_speed *= -(bounce);
		PCresult += 1;
	}

	Draw_Rectangle(paddle);
	paddle.left = mouse_pos_x - 20;
	paddle.thick = mouse_pos_x + 40;

	if (ball_paddle_collision(ball, paddle) == true)
	{
		ball_y_speed *= -(bounce);
		glutSwapBuffers();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE || GLUT_RGB);
	glutInitWindowSize(Win_width, Win_height);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Pong Game by Rawgreen");
	gl_Settings();
	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutTimerFunc(1, Timer, 1);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(kb_input);
	glutPassiveMotionFunc(mouse_motion);

	glutMainLoop();

}
