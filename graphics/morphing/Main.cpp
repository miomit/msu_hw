#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

using namespace std;

static float anim_t = 0.0f;
static const int timer_ms = 50;
static const float step = 0.01f;
static bool anim_running = true;

static std::vector<glm::vec2> polygonContourFromVerts(const std::vector<glm::vec2>& verts, int samples)
{
    std::vector<glm::vec2> pts;
    if (samples <= 0 || verts.size() < 2) return pts;
    pts.reserve(static_cast<size_t>(samples));

    const size_t m = verts.size();
    std::vector<float> lens(m);
    float L = 0.0f;
    for (size_t i = 0; i < m; ++i) {
        float l = glm::length(verts[(i + 1) % m] - verts[i]);
        lens[i] = l;
        L += l;
    }
    if (L <= 0.0f) return pts;

    for (int si = 0; si < samples; ++si) {
        float s = (static_cast<float>(si) / static_cast<float>(samples)) * L;
        float acc = 0.0f;
        glm::vec2 p(0.0f);
        for (size_t i = 0; i < m; ++i) {
            if (s <= acc + lens[i] || i == m - 1) {
                float local = (lens[i] > 0.0f) ? ((s - acc) / lens[i]) : 0.0f;
                p = verts[i] + local * (verts[(i + 1) % m] - verts[i]);
                break;
            }
            acc += lens[i];
        }
        pts.push_back(p);
    }

    return pts;
}

static std::vector<glm::vec2> triangleContour(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, int N)
{
    std::vector<glm::vec2> pts;
    pts.reserve(N);

    auto len = [](const glm::vec2& u, const glm::vec2& v) {
        return glm::length(v - u);
    };

    float l1 = len(A, B);
    float l2 = len(B, C);
    float l3 = len(C, A);
    float L = l1 + l2 + l3;
    if (L <= 0.0f || N <= 0) return pts;

    for (int i = 0; i < N; ++i) {
        float s = (static_cast<float>(i) / static_cast<float>(N)) * L;
        glm::vec2 p;
        if (s <= l1) {
            float tt = (l1 > 0.0f) ? (s / l1) : 0.0f;
            p = A + tt * (B - A);
        } else if (s <= l1 + l2) {
            s -= l1;
            float tt = (l2 > 0.0f) ? (s / l2) : 0.0f;
            p = B + tt * (C - B);
        } else {
            s -= (l1 + l2);
            float tt = (l3 > 0.0f) ? (s / l3) : 0.0f;
            p = C + tt * (A - C);
        }
        pts.push_back(p);
    }

    return pts;
}

static std::vector<glm::vec2> ellipseContour(const glm::vec2& center, float rx, float ry, int N)
{
    std::vector<glm::vec2> pts;
    if (N <= 0) return pts;
    pts.reserve(static_cast<size_t>(N));
    const float twoPi = glm::two_pi<float>();
    for (int i = 0; i < N; ++i) {
        float a = twoPi * (static_cast<float>(i) / static_cast<float>(N));
        pts.emplace_back(center.x + rx * cosf(a), center.y + ry * sinf(a));
    }
    return pts;
}

static std::vector<glm::vec2> starVerts(const glm::vec2& center, float outerR, float innerR, int points)
{
    std::vector<glm::vec2> verts;
    if (points < 2) return verts;
    verts.reserve(static_cast<size_t>(points * 2));
    const float twoPi = glm::two_pi<float>();
    for (int i = 0; i < points; ++i) {
        float a = twoPi * (static_cast<float>(i) / static_cast<float>(points));
        verts.emplace_back(center.x + outerR * cosf(a), center.y + outerR * sinf(a));
        float a2 = a + glm::pi<float>() / static_cast<float>(points);
        verts.emplace_back(center.x + innerR * cosf(a2), center.y + innerR * sinf(a2));
    }
    return verts;
}

static std::vector<glm::vec2> rectVerts(const glm::vec2& center, float halfW, float halfH)
{
    return {
        { center.x - halfW, center.y - halfH },
        { center.x + halfW, center.y - halfH },
        { center.x + halfW, center.y + halfH },
        { center.x - halfW, center.y + halfH }
    };
}

static std::vector<glm::vec2> regularPolygonVerts(const glm::vec2& center, float radius, int sides)
{
    std::vector<glm::vec2> verts;
    if (sides < 3) return verts;
    verts.reserve(static_cast<size_t>(sides));
    const float twoPi = glm::two_pi<float>();
    for (int i = 0; i < sides; ++i) {
        float a = twoPi * (static_cast<float>(i) / static_cast<float>(sides));
        verts.emplace_back(center.x + radius * cosf(a), center.y + radius * sinf(a));
    }
    return verts;
}

static std::vector<glm::vec2> circleContour(const glm::vec2& center, float radius, int N)
{
    std::vector<glm::vec2> pts;
    pts.reserve(N);
    if (N <= 0) return pts;

    const float twoPi = glm::two_pi<float>();
    for (int i = 0; i < N; ++i) {
        float a = twoPi * (static_cast<float>(i) / static_cast<float>(N));
        pts.emplace_back(center.x + radius * cosf(a), center.y + radius * sinf(a));
    }
    return pts;
}

static void drawCorrespondences_midpoint(const std::vector<glm::vec2>& a, const std::vector<glm::vec2>& b, float t, float r, float g, float bcol)
{
    if (a.size() != b.size()) return;
    glColor3f(r, g, bcol);
    glPointSize(3.f);
    glBegin(GL_POINTS);
    for (size_t i = 0; i < a.size(); ++i) {
        glm::vec2 m = (1.0f - t) * a[i] + t * b[i];
        glVertex2f(m.x, m.y);
    }
    glEnd();
}

void timerFunc(int)
{
    if (anim_running) {
        anim_t += step;
        if (anim_t >= 1.0f) {
            anim_t = 1.0f;
            anim_running = false;
        } else {
            glutTimerFunc(timer_ms, timerFunc, 0);
        }
        glutPostRedisplay();
    }
}

void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glm::vec2 A(0.6f, -0.4f), B(0.0f, 0.6f), C(-0.6f, -0.4f);
    glm::vec2 center(0.5f, 0.5f);
    float radius = 0.3f;
    const int N = 1328;

    auto star = polygonContourFromVerts(starVerts({ -0.5f,-0.5f }, 0.28f, 0.12f, 5), N);

    auto tri = triangleContour(A, B, C, N);
    auto circ = circleContour(center, radius, N);

    auto ell = ellipseContour({ 0.0f,0.0f }, 0.6f, 1.0f, N);

    auto rect = polygonContourFromVerts(rectVerts({ 0.5f,0.5f }, 0.52f, 0.18f), N);

    auto pent = polygonContourFromVerts(regularPolygonVerts({ 0.0f,0.0f }, 0.65f, 5), N);

    drawCorrespondences_midpoint(star, circ, anim_t, 1.f, 1.f, 1.f);

    glFlush();
}

int main(int argc, char** argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Triangle <-> Circle");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutDisplayFunc(displayMe);

    glutTimerFunc(timer_ms, timerFunc, 0);

    glutMainLoop();
    return 0;
}
