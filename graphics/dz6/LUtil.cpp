#include "LUtil.h"
//Позиция камеры
GLfloat gCameraX = 0.f, gCameraY = 0.f;

bool initGL()
{
	glViewport(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glClearColor(0.f, 0.f, 0.f, 1.f);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL!\n");
		return false;
	}
	return true;
}
void update()
{
}
void render()
{
	//Очистить буфер
	glClear(GL_COLOR_BUFFER_BIT);
	//Восстановить матрицу по умолчанию
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	//Сохраним матрицу по умолчанию
	glPushMatrix();
	//Красный квадрат
	glBegin(GL_QUADS);
	glColor3f(1.f, 0.f, 0.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glEnd();
	//Движение вправо
	glTranslatef(SCREEN_WIDTH, 0.f, 0.f);
	//Зеленый квадрат
	glBegin(GL_QUADS);
	glColor3f(0.f, 1.f, 0.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glEnd();
	//Движение в нижний правый угол
	glTranslatef(0.f, SCREEN_HEIGHT, 0.f);
	//Синий квадрат
	glBegin(GL_QUADS);
	glColor3f(0.f, 0.f, 1.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glEnd();
	//Вниз экрана
	glTranslatef(-SCREEN_WIDTH, 0.f, 0.f);
	//Желтый квадрат
	glBegin(GL_QUADS);
	glColor3f(1.f, 1.f, 0.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glEnd();
	// белый квадрат в центре ///////////////////////////////////////////////////
	glTranslatef(SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f, 0.f);
	glBegin(GL_QUADS);
	glColor3f(1.f, 1.f, 1.f);
	glVertex2f(-50.f, -50.f);
	glVertex2f(50.f, -50.f);
	glVertex2f(50.f, 50.f);
	glVertex2f(-50.f, 50.f);
	glEnd();
	////////////////////////////////////////////////////////
	
	glTranslatef(gCameraX, gCameraY, 0.f);
	glBegin(GL_QUADS);
	glColor3f(0.f, 0.f, 1.f);
	glVertex2f(-50.f, -50.f);
	glVertex2f(50.f, -50.f);
	glVertex2f(50.f, 50.f);
	glVertex2f(-50.f, 50.f);
	glEnd();

	//Обновить экран
	glutSwapBuffers();
}
void handleKeys(unsigned char key, int x, int y)
{
	//Обработка нажатий клавиш
	if (key == 'w')
	{
		gCameraY -= 16.f;
	}
	else if (key == 's')
	{
		gCameraY += 16.f;
	}
	else if (key == 'a')
	{
		gCameraX -= 16.f;
	}
	else if (key == 'd')
	{
		gCameraX += 16.f;
	}


	//Восстановить матрицу
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glLoadIdentity();
	//Переместить камеру
	glTranslatef(-gCameraX, -gCameraY, 0.f);
	//Сохраним
	glPushMatrix();
}