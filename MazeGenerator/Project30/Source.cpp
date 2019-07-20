#define _CRT_SECURE_NO_WARNINGS 

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <glut.h> 

int windowWidth;
int windowHeight;

int labyrinthWidth;
int labyrinthHeight;

int showSolve = 0;
int stepByStep = 0;
int playingMode = 0;

int prevPosition = 1;
int prevValue = 0;

int currentPosition = 1;

float bgColor[3] = { 0.93, 0.94, 0.95 };
float mazeColor[3] = { 0.42, 0.48, 0.54 };
float solveColor[3] = { 0.941, 0.204, 0.204 };
float snakeColor[3] = { 0.302, 0.686, 0.486 };

char separator[] = "--------------------------------------------------------------------------------";
char *maze;

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	for (int y = 0; y < labyrinthHeight; y++) {
		for (int x = 0; x < labyrinthWidth; x++) {
			if (maze[y * labyrinthWidth + x] == 1){
				glColor3f(mazeColor[0], mazeColor[1], mazeColor[2]);

				glVertex2f(-windowWidth / 2 + (x * 20), windowHeight / 2 - 20 - (y * 20));
				glVertex2f(-windowWidth / 2 + (x * 20), windowHeight / 2 - (y * 20));
				glVertex2f(-windowWidth / 2 + 20 + (x * 20), windowHeight / 2 - (y * 20));
				glVertex2f(-windowWidth / 2 + 20 + (x * 20), windowHeight / 2 - 20 - (y * 20));
			}
			else if (showSolve != 0 && (maze[y * labyrinthWidth + x] == 2 || maze[y * labyrinthWidth + x] == 4)) {
				glColor3f(solveColor[0], solveColor[1], solveColor[2]);

				glVertex2f(-windowWidth / 2 + (x * 20) + 5, windowHeight / 2 - 20 - (y * 20) + 5);
				glVertex2f(-windowWidth / 2 + (x * 20) + 5, windowHeight / 2 - (y * 20) - 5);
				glVertex2f(-windowWidth / 2 + 20 + (x * 20) - 5, windowHeight / 2 - (y * 20) - 5);
				glVertex2f(-windowWidth / 2 + 20 + (x * 20) - 5, windowHeight / 2 - 20 - (y * 20) + 5);
			}else if (maze[y * labyrinthWidth + x] == 5) {
				glColor3f(snakeColor[0], snakeColor[1], snakeColor[2]);

				glVertex2f(-windowWidth / 2 + (x * 20), windowHeight / 2 - 20 - (y * 20));
				glVertex2f(-windowWidth / 2 + (x * 20), windowHeight / 2 - (y * 20));
				glVertex2f(-windowWidth / 2 + 20 + (x * 20), windowHeight / 2 - (y * 20));
				glVertex2f(-windowWidth / 2 + 20 + (x * 20), windowHeight / 2 - 20 - (y * 20));
			}
		}
	}
	glEnd();
	glFlush();
}
void Initialize() {
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2, -1, 1);

}

void printMaze(int width, int height) {
	int x, y;
	printf("\n\nNext step:\n");
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			switch (maze[y * width + x]) {
			case 1:  printf("[]");  break;
			case 5:  printf("##");  break;
			default: printf("  ", y * width + x);  break;
			}
		}
		printf("\n");
	}
}
void carveMaze(int width, int height, int x, int y) {

	int x1, y1;
	int x2, y2;
	int dx, dy;
	int dir, count;

	dir = rand() % 4;
	count = 0;
	while (count < 4) {
		dx = 0; dy = 0;
		switch (dir) {
		case 0:  dx = 1;  break;
		case 1:  dy = 1;  break;
		case 2:  dx = -1; break;
		default: dy = -1; break;
		}
		x1 = x + dx;
		y1 = y + dy;
		x2 = x1 + dx;
		y2 = y1 + dy;
		if (x2 > 0 && x2 < width && y2 > 0 && y2 < height
			&& maze[y1 * width + x1] == 1 && maze[y2 * width + x2] == 1) {
			maze[y1 * width + x1] = 0;
			maze[y2 * width + x2] = 0;
			x = x2; y = y2;
			dir = rand() % 4;
			count = 0;

			if (stepByStep == 1) {
				printMaze(width, height);
				_getch();
			}

		}
		else {
			dir = (dir + 1) % 4;
			count += 1;
		}
	}
}
void generateLabyrint(int width, int height) {
	int x, y;

	for (x = 0; x < width * height; x++)
		maze[x] = 1;

	maze[1 * width + 1] = 0;
	srand(time(0));

	for (y = 1; y < height; y += 2) 
		for (x = 1; x < width; x += 2) {
			carveMaze(width, height, x, y);
		}
		
	maze[0 * width + 1] = 0;
	maze[(height - 1) * width + (width - 2)] = 4;

}
void solveMaze(int width, int height) {

	int dir, count;
	int x, y;
	int dx, dy;
	int forward;

	maze[0 * width + 1] = 1;
	maze[(height - 1) * width + (width - 2)] = 1;

	forward = 1;
	dir = 0;
	count = 0;
	x = 1;
	y = 1;
	while (x != width - 2 || y != height - 2) {
		dx = 0; dy = 0;
		switch (dir) {
		case 0:  dx = 1;  break;
		case 1:  dy = 1;  break;
		case 2:  dx = -1; break;
		default: dy = -1; break;
		}
		if ((forward  && maze[(y + dy) * width + (x + dx)] == 0)
			|| (!forward && maze[(y + dy) * width + (x + dx)] == 2)) {
			maze[y * width + x] = forward ? 2 : 3;
			x += dx;
			y += dy;
			forward = 1;
			count = 0;
			dir = 0;
		}
		else {
			dir = (dir + 1) % 4;
			count += 1;
			if (count > 3) {
				forward = 0;
				count = 0;
			}
		}
	}

	maze[1] = 2;
	maze[(height - 1) * width + (width - 2)] = 4;
	maze[(height - 2) * width + (width - 2)] = 2;

}

void settingsController() {

A:
	float _redBg = bgColor[0];
	float _greenBg = bgColor[1];
	float _blueBg = bgColor[2];

	float _redMaze = mazeColor[0];
	float _greenMaze = mazeColor[1];
	float _blueMaze = mazeColor[2];

	float _redSolve = solveColor[0];
	float _greenSolve = solveColor[1];
	float _blueSolve = solveColor[2];

	float _redSnake = snakeColor[0];
	float _greenSnake = snakeColor[1];
	float _blueSnake = snakeColor[2];

	system("cls");
	printf("%sLabyrinth Generator // Settings\n%s\n", separator, separator);

	printf("Choose, what you want:");
	printf("\n	1. Set background color");
	printf("\n	2. Set walls color");
	printf("\n	3. Set solve color");
	printf("\n	4. Set snake color");
	printf("\n	5. Step by step generation ");
	if (stepByStep == 1)
		printf("[Enabled]");
	else
		printf("[Disabled]");

	printf("\n\n\n	6. Return to menu");

	switch (_getch())
	{
	case '1':
		system("cls");
		printf("%sLabyrinth Generator // Settings // Background color\n%s\n", separator, separator);
		printf("Current background color (RBG):%3.2f,%3.2f,%3.2f", bgColor[0], bgColor[1], bgColor[2]);

		printf("\n\nSet new values (format - R, G, B):");
		scanf("%f, %f, %f", &_redBg, &_greenMaze, &_blueMaze);

		if ((_redBg < 0 || _redBg > 1) || (_greenMaze < 0 || _greenMaze > 1) || (_blueMaze < 0 || _blueMaze > 1)) {
			printf("\n\n%s!!! Value's range: from 0 to 1.\n%s", separator, separator);
			_getch();
			break;
		}

		bgColor[0] = _redBg;
		bgColor[1] = _greenMaze;
		bgColor[2] = _blueMaze;

		printf("\n\nNew values setted!\nPress any key to go back.");
		_getch();
		break;
	case '2':
		system("cls");
		printf("%sLabyrinth Generator // Settings // Maze color\n%s\n", separator, separator);
		printf("Current background color (RBG):%3.2f,%3.2f,%3.2f", mazeColor[0], mazeColor[1], mazeColor[2]);

		printf("\n\nSet new values (format - R, G, B):");
		scanf("%f, %f, %f", &_redMaze, &_greenMaze, &_greenMaze);

		if ((_redMaze < 0 || _redMaze > 1) || (_greenMaze < 0 || _greenMaze > 1) || (_blueMaze < 0 || _blueMaze > 1)) {
			printf("\n\n%s!!! Value's range: from 0 to 1.\n%s", separator, separator);
			_getch();
			break;
		}

		mazeColor[0] = _redMaze;
		mazeColor[1] = _greenMaze;
		mazeColor[2] = _greenMaze;

		printf("\n\nNew values setted!\nPress any key to go back.");
		_getch();
		break;
	case '3':
		system("cls");
		printf("%sLabyrinth Generator // Settings // Solve color\n%s\n", separator, separator);
		printf("Current solve color (RBG):%3.2f,%3.2f,%3.2f", solveColor[0], solveColor[1], solveColor[2]);

		printf("\n\nSet new values (format - R, G, B):");
		scanf("%f, %f, %f", &_redSolve, &_greenSolve, &_blueSolve);

		if ((_redSolve < 0 || _redSolve > 1) || (_greenSolve < 0 || _greenSolve > 1) || (_blueSolve < 0 || _blueSolve > 1)) {
			printf("\n\n%s!!! Value's range: from 0 to 1.\n%s", separator, separator);
			_getch();
			break;
		}

		solveColor[0] = _redSolve;
		solveColor[1] = _greenMaze;
		solveColor[2] = _blueSolve;

		printf("\n\nNew values setted!\nPress any key to go back.");
		_getch();
		break;
	case '4':
		system("cls");
		printf("%sLabyrinth Generator // Settings // Snake color\n%s\n", separator, separator);
		printf("Current solve color (RBG):%3.2f,%3.2f,%3.2f", snakeColor[0], snakeColor[1], snakeColor[2]);

		printf("\n\nSet new values (format - R, G, B):");
		scanf("%f, %f, %f", &_redSnake, &_greenSnake, &_blueSnake);

		if ((_redSnake < 0 || _redSnake > 1) || (_greenSnake < 0 || _greenSnake > 1) || (_blueSnake < 0 || _blueSnake > 1)) {
			printf("\n\n%s!!! Value's range: from 0 to 1.\n%s", separator, separator);
			_getch();
			break;
		}

		snakeColor[0] = _redSnake;
		snakeColor[1] = _greenSnake;
		snakeColor[2] = _blueSnake;

		printf("\n\nNew values setted!\nPress any key to go back.");
		_getch();
		break;
	case '5':
		switch (stepByStep)
		{
		case 0:
			stepByStep = 1;
			break;
		default:
			stepByStep = 0;
			break;
		}
		break;
	case '6':
		return;
		break;
	default:
		return;
		break;
	}
	goto A;
}
void developersController() {
	system("cls");
	printf("%sLabyrinth Generator // Settings\n%s\n", separator, separator);
	printf("Developed by Vadim Tregubenko 13656/3\n");

	printf("\n\n\nPress any key to back menu.");
	_getch();
}
void keyboardController(unsigned char key, int x, int y) {

	if (key == 'e')
		exit(0);

	if (key == 's') {
		switch (showSolve)
		{
		case 1:
			showSolve = 0;
			break;
		default:
			showSolve = 1;
			break;
		}
		glutSwapBuffers();
	}

}

int checkMove(int currentPosition) {
	if (currentPosition == labyrinthHeight * labyrinthWidth - 2) {
		system("cls");
		printf("%sLabyrinth Generator // Playing mode\n%s\n", separator, separator);

		printf("~####~~~####~~~####~~#####\n##~~~~~##~~##~##~~##~##~~##\n##~###~##~~##~##~~##~##~~##\n##~~##~##~~##~##~~##~##~~##\n~####~~~####~~~####~~#####\n\n######~~####~~#####~~~~###\n~~~~##~##~~##~##~~##~~~###\n~~~~##~##~~##~#####~~~~###\n##~~##~##~~##~##~~##\n~####~~~####~~#####~~~~###\n");
		printf("\n\nPress any key to exit.");
		_getch();
		exit(0);
	}
	if (maze[currentPosition] != 1 && maze[currentPosition] != 5)
		return 1;



	printf("\n\n!!! Illegal move. You can't go through the wall or through yourself.");
	return 0;
}
void checkGameOver(int currentPosition) {

	if ((maze[currentPosition + 1] == 1 || maze[currentPosition + 1] == 5) &&
		(maze[currentPosition - 1] == 1 || maze[currentPosition - 1] == 5) &&
		(maze[currentPosition + labyrinthWidth] == 1 || maze[currentPosition + labyrinthWidth] == 5) &&
		(maze[currentPosition - labyrinthWidth] == 1 || maze[currentPosition - labyrinthWidth] == 5)) {
		system("cls");
		printf("%sLabyrinth Generator // Playing mode\n%s\n", separator, separator);

		printf("~####~~~####~~##~~~#~#####\n##~~~~~##~~##~###~##~##\n##~###~######~##~#~#~####\n##~~##~##~~##~##~~~#~##\n~####~~##~~##~##~~~#~#####\n\n~####~~##~~##~#####~~#####~~~~###\n##~~##~##~~##~##~~~~~##~~##~~~###\n##~~##~##~~##~####~~~#####~~~~###\n##~~##~~####~~##~~~~~##~~##\n~####~~~~##~~~#####~~##~~##~~~###\n");
		printf("\n\nPress any key to exit.");
		_getch();
		exit(0);
	}
}
void playingController(int key, int x, int y) {
	if (playingMode == 0) {
		if (key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT || key == GLUT_KEY_UP) {
			playingMode = 1;
			system("cls");
			printf("%sLabyrinth Generator // Playing mode\n%s\n", separator, separator);
			printf("\n\nUse buttons UP, DOWN, LEFT, RIGHT to snake control.");
			maze[1] = 5;
			prevPosition = 1;
			prevValue = 2;
			glutSwapBuffers();
		}
	}
	else {
		if (key == GLUT_KEY_DOWN) {
			if (checkMove(currentPosition + labyrinthWidth) == 0) {
				checkGameOver(currentPosition);
				return;
			}

			maze[prevPosition] = prevValue;

			currentPosition += labyrinthWidth;

			prevPosition = currentPosition;
			prevValue = maze[currentPosition];
			maze[currentPosition] = 5;

			glutSwapBuffers();
		}
		if (key == GLUT_KEY_UP) {
			if (checkMove(currentPosition - labyrinthWidth) == 0) {
				checkGameOver(currentPosition);
				return;
			}
			maze[prevPosition] = prevValue;

			currentPosition -= labyrinthWidth;

			prevPosition = currentPosition;
			prevValue = maze[currentPosition];
			maze[currentPosition] = 5;

			glutSwapBuffers();
		}
		if (key == GLUT_KEY_RIGHT) {
			if (checkMove(currentPosition + 1) == 0) {
				checkGameOver(currentPosition);
				return;
			}
			maze[prevPosition] = prevValue;

			currentPosition++;

			prevPosition = currentPosition;
			prevValue = maze[currentPosition];
			maze[currentPosition] = 5;

			glutSwapBuffers();
		}
		if (key == GLUT_KEY_LEFT) {
			if (checkMove(currentPosition - 1) == 0) {
				checkGameOver(currentPosition);
				return;
			}
			maze[prevPosition] = prevValue;

			currentPosition--;

			prevPosition = currentPosition;
			prevValue = maze[currentPosition];
			maze[currentPosition] = 5;

			glutSwapBuffers();
		}
	}
}

int main(int argc, char ** argv) {

A:
	int _width = 0;
	int _height = 0;

	system("cls");
	printf("%sLabyrinth Generator\n%s\n", separator, separator);

	printf("Choose, what you want:");
	printf("\n	1. Generate Labyrinth");
	printf("\n	2. Settings");
	printf("\n	3. About Developers");

	printf("\n\n\n	4. Exit");

	switch (_getch())
	{
	case '1':

		_width = 0;
		_height = 0;

		system("cls");
		printf("%sLabyrinth Generator\n%s\n", separator, separator);
		printf("Set labyrinth's width: ");
		scanf("%d", &_width);
		printf("\n\nSet labyrinth's height: ");
		scanf("%d", &_height);

		if (_width < 3 || _height < 3) {
			printf("\n\n%s!!! Illegal maze size: min width - 3; min height - 3.\n%s", separator, separator);
			_getch();
			break;
		}

		_width--;
		_height--;

		labyrinthWidth = _width * 2 + 3;
		labyrinthHeight = _height * 2 + 3;

		maze = (char*)malloc(labyrinthWidth * labyrinthHeight * sizeof(char));
		if (maze == NULL) {
			printf("\n\n%s!!! Not enough memory.\n%s", separator, separator);
			free(maze);
			_getch();
			break;
		}

		printf("\n\n\n\nGenerating...\n\n");
		generateLabyrint(labyrinthWidth, labyrinthHeight);
		solveMaze(labyrinthWidth, labyrinthHeight);
		printf("Done!\nHINT: To show solve, press 'S'.\nHINT: To play, press UP / DOWN / LEFT / RIGHT button.\nHINT: To close window, press 'E'");

		windowWidth = labyrinthWidth * 20;
		windowHeight = labyrinthHeight * 20;

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(windowWidth, windowHeight);
		glutInitWindowPosition(600, 200);
		glutCreateWindow("Generated labyrinth");
		glutDisplayFunc(display);
		glutIdleFunc(display);
		Initialize();

		glutKeyboardFunc(keyboardController);
		glutSpecialFunc(playingController);

		glutMainLoop();

		free(maze);

		break;
	case '2':
		settingsController();
		break;
	case '3':
		developersController();
		break;
	case '4':
		return 1;
		break;
	default:
		break;
	}

	goto A;
}