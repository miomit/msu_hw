#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include "LUtil.h"

void runMainLoop(int val);
/*
Предварительное условие:
  -Инициализированный freeGLUT
Состояние:
  -Вызывает функции основного цикла и устанавливает обратный вызов через 1000 / SCREEN_FPS миллисекунд.
Побочные эффекты:
  -Устанавливает функцию GlutTimerFunc
*/

int main(int argc, char* args[])
{
    //FreeGLUT
    glutInit(&argc, args);

    //OpenGL context
    glutInitContextVersion(2, 1);

    //Окно с двойным буфером
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("OpenGL");

    //Ошибка
    if (!initGL())
    {
        printf("Unable to initialize graphics library!\n");
        return 1;
    }

    //Обработчик событий клавиатуры
    glutKeyboardFunc(handleKeys);

    //Отрисовка
    glutDisplayFunc(render);

    //Основной цикл
    glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);

    //Start
    glutMainLoop();

    return 0;
}

void runMainLoop(int val)
{
    //Что делаем?
    update();
    render();

    //Обработка по таймеру
    glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, val);
}