#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glut.h>

// Параметры поворота сцены: pitch — вверх/вниз (вокруг X), yaw — влево/вправо (вокруг Y)
GLfloat gPitch = 20.0f; 
GLfloat gYaw   = 0.0f;

static GLUquadric* gQuadric = nullptr;

void drawCylinderWithCaps(float baseR, float topR, float height, int slices, int stacks)
{
    gluQuadricNormals(gQuadric, GLU_SMOOTH);
    gluCylinder(gQuadric, baseR, topR, height, slices, stacks);

    glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(gQuadric, 0.0, baseR, slices, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, height);
    gluDisk(gQuadric, 0.0, topR, slices, 1);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(gPitch, 1.0f, 0.0f, 0.0f); // наклон вверх/вниз
    glRotatef(gYaw,   0.0f, 1.0f, 0.0f); // поворот влево/вправо

    // Тор
    {
        GLfloat magenta[] = { 0.8f, 0.1f, 0.6f, 1.0f };
        GLfloat spec[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, magenta);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);

        glPushMatrix();
        glTranslatef(-0.75f, 0.5f, 0.0f);
        glRotatef(90.0, 1.0f, 0.0f, 0.0f);
        glutSolidTorus(0.275, 0.85, 32, 64);
        glPopMatrix();
    }

    // Конус
    {
        GLfloat green[] = { 0.1f, 0.8f, 0.2f, 1.0f };
        GLfloat spec[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 30.0f);

        glPushMatrix();
        glTranslatef(-0.75f, -0.5f, 0.0f);
        glRotatef(270.0, 1.0f, 0.0f, 0.0f);
        glutSolidCone(1.0, 2.0, 64, 16);
        glPopMatrix();
    }

    // Шар
    {
        GLfloat red[] = { 0.85f, 0.15f, 0.15f, 1.0f };
        GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 40.0f);

        glPushMatrix();
        glTranslatef(0.75f, 0.0f, -1.0f);
        glutSolidSphere(1.0, 48, 48);
        glPopMatrix();
    }

    // Цилиндр
    {
        GLfloat blue[] = { 0.15f, 0.45f, 0.9f, 1.0f };
        GLfloat spec[] = { 0.6f, 0.6f, 0.8f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 35.0f);

        glPushMatrix();
        glTranslatef(0.75f, 0.6f, 0.5f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        drawCylinderWithCaps(0.35f, 0.35f, 1.2f, 48, 4);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void reshape(GLint w, GLint h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    GLfloat aspect = GLfloat(w) / GLfloat(h);
    glLoadIdentity();
    if (w <= h) {
        glOrtho(-2.5, 2.5, -2.5 / aspect, 2.5 / aspect, -10.0, 10.0);
    }
    else {
        glOrtho(-2.5 * aspect, 2.5 * aspect, -2.5, 2.5, -10.0, 10.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    GLfloat ambientLight[] = { 0.08f, 0.08f, 0.08f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 0.85f, 0.6f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    gQuadric = gluNewQuadric();
    gluQuadricDrawStyle(gQuadric, GLU_FILL);
    gluQuadricNormals(gQuadric, GLU_SMOOTH);
}

void handleKeys(unsigned char key, int, int)
{
    const float deltaAngle = 5.5f;
    switch (key) {
        case 'w': // наклон вверх
            gPitch += deltaAngle;
            if (gPitch > 89.0f) gPitch = 89.0f; // предотвратить "переворот"
            break;
        case 's': // наклон вниз
            gPitch -= deltaAngle;
            if (gPitch < -89.0f) gPitch = -89.0f;
            break;
        case 'a': // поворот влево (yaw положительный)
            gYaw += deltaAngle;
            break;
        case 'd': // поворот вправо
            gYaw -= deltaAngle;
            break;
    }

    glutPostRedisplay();
}

void runMainLoop(int)
{
    glutPostRedisplay();
    // Обновляем цикл (60 FPS)
    glutTimerFunc(1000 / 60, runMainLoop, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    // Включаем двойную буферизацию
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cyan Shapes in Yellow Light");

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeys);
    glutReshapeFunc(reshape);
    glutTimerFunc(1000 / 60, runMainLoop, 0);

    init();
    glutMainLoop();

    if (gQuadric) gluDeleteQuadric(gQuadric);
    return 0;
}