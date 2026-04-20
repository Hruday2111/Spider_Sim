/*
 * 3D Spider Simulation - modular entry point.
 *
 * Compile commands remain unchanged because the implementation modules are
 * included below in the same order they originally appeared in main.cpp.
 *
 * Linux : g++ main.cpp -o spider -lGL -lGLU -lglut -lm
 * Mac   : g++ main.cpp -o spider -framework OpenGL -framework GLUT -lm
 * Win   : g++ main.cpp -o spider -lfreeglut -lopengl32 -lglu32
 */

#include "common.hpp"
#include "textures.cpp"
#include "geometry.cpp"
#include "spider.cpp"
#include "scene.cpp"
#include "app.cpp"

// ════════════════════════════════════════════════════════════
//  MAIN
// ════════════════════════════════════════════════════════════
int main(int argc,char**argv){
    srand((unsigned)time(nullptr));

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1280,800);
    glutInitWindowPosition(30,20);
    glutCreateWindow("3D Spider Simulation  |  Final  |  OpenGL/GLUT Bedroom");
    glutIgnoreKeyRepeat(1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

    // Fog for depth (subtle)
    glEnable(GL_FOG);
    GLfloat fogColor[]={0.08f,0.07f,0.06f,1.0f};
    glFogfv(GL_FOG_COLOR,fogColor);
    glFogf(GL_FOG_MODE,GL_EXP2);
    glFogf(GL_FOG_DENSITY,0.0055f);

    initTextures();
    lastMs=glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKey);
    glutMouseFunc(mouseBtn);
    glutMotionFunc(mouseMove);
    glutTimerFunc(16,timerFunc,0);

    glutMainLoop();
    return 0;
}
