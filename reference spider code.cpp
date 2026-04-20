// including desired libraries
#include <iostream>
// for Mac OS
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <chrono>
#include <thread>
// including soil library for image rendering
#include "soil/SOIL.h"

// IRRKlang audio library for sound effects management
// #include <irrKlang.h>

using namespace std;
// Using irrklang namespace for audio engine and sound interfaces
// using namespace irrklang;

// Global sound engine pointer - initialized at startup, used to create and play 2D sounds
// This is the main interface with the IRRKlang audio library
// ISoundEngine *engine;
// Global Control Variables
float ang = 5;
int xt = 1, p = 0, tr = 4; // change texture background

int flag = 0;
// spider texture
int sk = 3;
// spider length
float spiderLen = 2;
// rotating angle
int rz = 0;
// instruction background
int ibg1 = 12;

float tx = 0, ty = 0, tz = 0;
// For speed
int speed = 1;
// For room
bool is_room = false;
int fr = 6, Wr = 7;
int sw1 = 9;
int fw1 = 10;
int rf1 = 16;
int rc1 = 11;
int door1 = 14;

// for lightning
bool is_day = true;

// for zoom
float zooms = 1;
float zmoves = 0;

// for about
bool is_info = false;

// For Menu
bool menuFlag = true;
int Mr = 8;

int inMenuAlongZ = 0, inMenuAlongX = 0, inMenuAlongY = 0;

// 3D Objects
GLUquadric *quad;
GLUquadricObj *leg;
// Texture
GLuint texture[20];
// Perspective Cam
static GLfloat pers = 100.0;
// for perspective
float zNear = 1.0, zFar = 100;

void display();

// function for loading texture
int LoadGLTextures()
{
    // texture for spider
    texture[rc1] = SOIL_load_OGL_texture("img/rc1.jpeg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[2] = SOIL_load_OGL_texture("img/spider2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[3] = SOIL_load_OGL_texture("img/skin3.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[4] = SOIL_load_OGL_texture("img/background.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[5] = SOIL_load_OGL_texture("img/skin2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[6] = SOIL_load_OGL_texture("img/floor1.jpeg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[7] = SOIL_load_OGL_texture("img/wall1.jpeg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[8] = SOIL_load_OGL_texture("img/menu1.jpeg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[sw1] = SOIL_load_OGL_texture("img/sw2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[fw1] = SOIL_load_OGL_texture("img/fw1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[rf1] = SOIL_load_OGL_texture("img/rf1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[ibg1] = SOIL_load_OGL_texture("img/ibg1.jpeg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[door1] = SOIL_load_OGL_texture("img/door1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

    if (texture[0] == 0)
    {
        return false;
    }

    return true; // Return Success
}

// Mouse modifiers
float depth = 8;
float phi = 0, theta = 0;
float downX, downY;
bool leftButton = false, middleButton = false;

// IRRKlang audio function: plays click sound effect when user interacts with UI
// Uses ISound interface from irrklang to play 2D audio and manage sound properties
void clickSound()
{
    // Use sound engine to play 2D click sound from file
    // Parameters: (filename, looped=true, paused_initially=false, track_in_3d_space=true)
    // ISound *sound = engine->play2D("sound/click.wav", true, false, true);
    
    // Brief delay to allow sound to initialize
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Pause the sound to prevent continuous playback
    // sound->setIsPaused(true);
    
    // Release the sound resource via irrklang interface
    // sound->drop();
}

// IRRKlang audio function: plays warning sound when spider exceeds boundary limits
// Provides audio feedback to user when spider attempts to move out of bounds
void boundaryWarning()
{
    // Use sound engine to play 2D boundary warning sound from file
    // Parameters: (filename, looped=true, paused_initially=false, track_in_3d_space=true)
    // ISound *sound = engine->play2D("sound/out_of_boundary.wav", true, false, true);
    
    // Brief delay to allow sound to initialize
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Pause the sound to prevent continuous playback
    // sound->setIsPaused(true);
    
    // Release the sound resource via irrklang interface
    // sound->drop();
}

// for mouse call back
void mouseCallback(int button, int state, int x, int y)
{
    if (menuFlag)
    {
        // for mouse click
        if (button == GLUT_LEFT_BUTTON and state == GLUT_UP)
        {

            if (x >= 325 and x <= 475 and y >= 375 and y <= 425)
            {
                // Web - play click sound via IRRKlang audio engine
                clickSound();
                menuFlag = false;
                is_room = false;
                glutPostRedisplay();
            }
            else if (x >= 325 and x <= 475 and y >= 440 and y <= 490)
            {
                // Enter home mode - play click sound via IRRKlang audio engine
                clickSound();
                menuFlag = false;
                is_room = true;
                glutPostRedisplay();
            }
            else if (x >= 325 and x <= 475 and y >= 500 and y <= 550)
            {
                // Exit application - play click sound via IRRKlang audio engine before exit
                clickSound();
                exit(0);
            }
        }
    }
    else
    {
        if (is_info)
        {
            if (button == GLUT_LEFT_BUTTON and state == GLUT_UP)
            {

                if (x >= 778 and x <= 800 and y >= 0 and y <= 22)
                {
                    // Return from instructions - play click sound via IRRKlang audio engine
                    clickSound();
                    is_info = false;
                    glutPostRedisplay();
                }
            }
        }
        else
        {

            if (button == GLUT_LEFT_BUTTON and state == GLUT_UP)
            {
                if (x >= 778 and x <= 800 and y >= 0 and y <= 22)
                {
                    // Return to main menu - play click sound via IRRKlang audio engine
                    clickSound();
                    menuFlag = true;
                    is_room = false;
                    tx = ty = tz = 0;
                    theta = 0;
                    phi = 0;
                    rz = 0;
                    glutPostRedisplay();
                }
                else if (x >= 778 and x <= 800 and y >= 577 and y <= 600)
                {
                    // Enter instructions section - play click sound via IRRKlang audio engine
                    clickSound();
                    is_info = true;

                    glutPostRedisplay();
                }
            }
        }
    }
    // for updating mouse position
    downX = x;
    downY = y;
    leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));
    middleButton = ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN));
}

// to detect mouse motion
void motionCallback(int x, int y)
{
    if (menuFlag)
    {
        // for rotation angle for user view perspective
        phi = 0;
        theta = 0;

        if (leftButton)
        {
            // Rotation of spider on menu
            inMenuAlongY += (x - downX) / 4.0;
            inMenuAlongX += (downY - y) / 4.0;
            inMenuAlongZ += (x - downX) + (downY - y);
        }

        downX = x;
        downY = y;
    }
    else
    {
        if (leftButton)
        {
            // Rotation of view according to user
            phi += (x - downX) / 4.0;
            theta += (downY - y) / 4.0;
        }

        downX = x;
        downY = y;
    }

    glutPostRedisplay();
}

// Function for the web of the spider.
void spiderweb()
{
    float xmin = -54.5, xmax = 54.5, ymin = -40.5, ymax = 40.5;
    glPushMatrix();

    // Enables 2D texture mapping

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[tr]);
    glTranslatef(0, 0, -30);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);

    glVertex3f(xmin, ymin, 8.0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xmin, ymax, 8.0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xmax, ymax, 8.0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xmax, ymin, 8.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// for instruction background
void instructions_bg()
{
    float xmin = -54.5, xmax = 54.5, ymin = -40.5, ymax = 40.5;
    glPushMatrix();
    // to disable lighting
    glDisable(GL_LIGHTING);
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    // to bind a texture object to the current texture unit
    glBindTexture(GL_TEXTURE_2D, texture[ibg1]);
    glTranslatef(0, 0, -30);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xmin, ymin, 8.0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xmin, ymax, 8.0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xmax, ymax, 8.0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xmax, ymin, 8.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// Function for menuBackground
void menuBackground()
{
    glPushMatrix();
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[Mr]);
    glTranslatef(0, 0, -30);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    float xmin = -55.5, xmax = 55.5, ymin = -50.5, ymax = 50.5;
    glVertex3f(xmin, ymin, 8.0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xmin, ymax, 8.0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xmax, ymax, 8.0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xmax, ymin, 8.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// Function for the floor of the room.
void roomFloor()
{
    glPushMatrix();
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[fr]);
    glTranslatef(0, 0, -30);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    float xmin = -50.5, xmax = 50.5, ymin = -50.5, ymax = 50.5;
    glVertex3f(xmin, ymin, 8.0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xmin, ymax, 8.0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xmax, ymax, 8.0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xmax, ymin, 8.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// Function for the floor of the spider.
void roomWall()
{
    // front wall
    float xmin = -50.5, xmax = 50.5, ymin = -50.5, ymax = 50.5;

    glPushMatrix();
    glTranslatef(0, 0, -50);
    glPushMatrix();
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[fw1]);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xmin - 1, ymin - 1, 8.0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xmax + 1, ymin - 1, 8.0);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xmax + 1, ymax + 1, 8.0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xmin - 1, ymax + 1, 8.0);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // door
    glPushMatrix();
    float gate = 80.5;
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[door1]);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xmin - 1, ymin - 1, gate);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xmax + 1, ymin - 1, gate);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xmax + 1, ymax + 1, gate);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xmin - 1, ymax + 1, gate);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // right wall
    glPushMatrix();
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[sw1]);
    glTranslatef(xmax, 0, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    float zmin = 0, zmax = 80.5;
    ymin = -50.5, ymax = 50.5;
    glVertex3f(0, ymin, zmin);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0, ymax, zmin);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0, ymax, zmax);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0, ymin, zmax);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // left wall
    glPushMatrix();
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[sw1]);
    glTranslatef(xmin, 0, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    zmin = -1, zmax = 80.5;
    ymin = -51.5, ymax = 51.5;
    glVertex3f(0, ymin, zmin);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0, ymax, zmin);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0, ymax, zmax);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0, ymin, zmax);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // ceiling
    glPushMatrix();
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[rc1]);
    glTranslatef(0, ymax, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    zmin = -1, zmax = 80.5;

    glVertex3f(xmin - 1, 0, zmin);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xmax + 1, 0, zmin);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xmax + 1, 0, zmax);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xmin - 1, 0, zmax);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();
}

class Bone
{
public:
    // setting the width anf height of bone
    Bone(float a, float l) : width(l), height(a), connection(0) {}
    void draw();
    // connecting bone at certain angle
    void setconnection(Bone *Bone, float ang)
    {
        connection = Bone, angle = ang;
    }
    void setangle(float ang)
    {
        angle = ang;
    }

    float getangle()
    {
        return angle;
    }

protected:
    float width;
    float height;
    float angle;
    Bone *connection;
};

void Bone::draw()
{
    glPushMatrix();
    glTranslatef(0.0, height / 1.0, 0.0); // Bone center
    glPushMatrix();
    glScalef(width, height * 2, width); // changing the size of the Bone
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[sk]);
    glutSolidCube(0.7); // drawing bone
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glTranslatef(0.0, height / 2.0, 0.0);
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[sk]);
    gluQuadricTexture(quad, GL_TRUE);
    gluSphere(quad, 0.85 * width, 8, 8); // bone joints
    glDisable(GL_TEXTURE_2D);
    // making leg by connecting till all bones are connected
    if (connection)
    {
        glRotatef(angle, 1.0, 0.0, 0.0);
        connection->draw();
    }
    glPopMatrix();
};

class Leg
{
public:
    Leg(float length, float width);
    void draw()
    {
        a.draw();
    }
    void joint1_angle_set(float angles_array_0);

    void joint2_angle_set(float angles_array_0);

    float joint1_angle_get()
    {
        return a.getangle();
    }

    float joint2_angle_get()
    {
        return b.getangle();
    }

protected:
    Bone a, b, c;
};

Leg::Leg(float length, float width)
    : a(length * 0.7, width), b(length * 0.5, width), c(length * 0.4, width)
{
    a.setconnection(&b, 100.0); // Bone joint
    b.setconnection(&c, 35.0);  // Initial angle
}

void Leg::joint2_angle_set(float angles_array_0)
{
    b.setangle(angles_array_0);
}

void Leg::joint1_angle_set(float angles_array_0)
{
    a.setangle(angles_array_0);
}

class Spider
{
public:
    Spider(float size);
    void draw();
    void angle_set(int Legs, float curv);
    void joint1_angle_set(int Legs, float curv);
    void joint2_angle_set(int Legs, float curv);
    void leg_angle_set(int Legs, float curv);
    float getCurveto(int numLeg)
    {
        return angles_array_0[numLeg];
    }
    float leg_angle_get(int numLeg2)
    {
        return angles_array[numLeg2];
    }
    float joint1_angle_get(int numLeg)
    {
        switch (numLeg)
        {
        case 0:
            return pf1.joint1_angle_get();
            break;
        case 1:
            return pf2.joint1_angle_get();
            break;
        case 2:
            return pl3.joint1_angle_get();
            break;
        case 3:
            return pl4.joint1_angle_get();
            break;
        case 4:
            return pl5.joint1_angle_get();
            break;
        case 5:
            return pl6.joint1_angle_get();
            break;
        case 6:
            return pt7.joint1_angle_get();
            break;
        case 7:
            return pt8.joint1_angle_get();
            break;
        default:
            return -1;
            break;
        }
    }

    float joint2_angle_get(int numLeg)
    {
        switch (numLeg)
        {
        case 0:
            return pf1.joint2_angle_get();
            break;
        case 1:
            return pf2.joint2_angle_get();
            break;
        case 2:
            return pl3.joint2_angle_get();
            break;
        case 3:
            return pl4.joint2_angle_get();
            break;
        case 4:
            return pl5.joint2_angle_get();
            break;
        case 5:
            return pl6.joint2_angle_get();
            break;
        case 6:
            return pt7.joint2_angle_get();
            break;
        case 7:
            return pt8.joint2_angle_get();
            break;
        default:
            return -1;
            break;
        }
    }

    
    void walkLeg();
    void movP0();
    void movP1();
    void movP2();
    void movP3();
    void movP4();
    void movP5();
    void movP6();
    void movP7();
    void movP02();
    void movP12();
    void movP22();
    void movP32();
    void movP42();
    void movP52();
    void movP62();
    void movP72();
    void movP03();
    void movP13();
    void movP23();
    void movP33();
    void movP43();
    void movP53();
    void movP63();
    void movP73();

protected:
    float size;
    Leg pf1;
    Leg pf2;
    Leg pl3;
    Leg pl4;
    Leg pl5;
    Leg pl6;
    Leg pt7;
    Leg pt8;

    float angles_array_0[8];
    float angles_array[8];
};

Spider::Spider(float gros)
    : size(gros),
      pf1(4 * size, size),
      pf2(4 * size, size),
      pl3(4 * size, size),
      pl4(4 * size, size),
      pl5(4 * size, size),
      pl6(4 * size, size),
      pt7(4 * size, size),
      pt8(4 * size, size)
{

    for (int i = 0; i < 8; i++)
    {
        angles_array_0[i] = -115;
        angles_array[i] = 0;
    }
}

// Function for drawing the spider.
void Spider::draw()
{

    glPushMatrix();
    glTranslatef(0, 0, -10);

    glTranslatef(tx, ty, 0.0);
    glTranslatef(0, 0, -10);

    glPushMatrix();
    glTranslatef(-1, 2, 0);
    glScalef(zooms, zooms, zooms);

    glPushMatrix();
    glTranslatef(-1, 2, 0);
    glRotatef(rz, 0, 0, 1);
    glTranslatef(1, -2, 0);
    glRotatef(-90, 1, 0, 0);
    // Front Left

    glPushMatrix();
    glTranslatef(5.5, -1, 3);
    glRotatef(-75, 0, 1, 0);
    glRotatef(angles_array_0[0] * 0.9, 1.0, 0.0, 0.0);
    glRotatef(angles_array[0] * 0.9, 0.0, 0.0, 1.0);
    pf1.draw();
    glPopMatrix();

    // Front right
    glPushMatrix();
    glRotatef(170, 0, 1, 0);
    glTranslatef(-6, -1, -4);
    glRotatef(75, 0, 1, 0);
    glRotatef(0, 1, 0, 0);
    glRotatef(10, 0, 1, 0);
    glRotatef(angles_array_0[1] * 0.9, 1.0, 0.0, 0.0);
    glRotatef(angles_array[1] * 0.9, 0.0, 0.0, 1.0);
    pf2.draw();
    glPopMatrix();

    // Side Left 1
    glPushMatrix();
    glTranslatef(7, -1, 2);
    glTranslatef(-3 * size, 0.0, 0.0);
    glRotatef(-35, 0, 1, 0);
    glRotatef(angles_array_0[2] * 0.9, 1.0, 0.0, 0.0);
    glRotatef(angles_array[2] * 0.9, 0.0, 0.0, 1.0);
    pl3.draw();
    glPopMatrix();

    // Side Right 1
    glPushMatrix();
    glTranslatef(4, -1, 6);
    glRotatef(-145, 0, 1, 0);
    glRotatef(angles_array_0[3] * 0.9, 1.0, 0.0, 0.0);
    glRotatef(angles_array[3] * 0.9, 0.0, 0.0, 1.0);
    pl4.draw();
    glPopMatrix();

    // Back Left
    glPushMatrix();
    glRotatef(20, 0, 1, 0);
    glTranslatef(-3, -1, 1);
    glRotatef(angles_array_0[6] * 0.9, 1.0, 0.0, 0.0);
    glRotatef(angles_array[6] * 0.9, 0.0, 0.0, 1.0);
    pt7.draw();
    glPopMatrix();

    // Back Right
    glPushMatrix();
    glRotatef(160, 0, 1, 0);
    glTranslatef(0, -1, -7);
    glRotatef(angles_array[7] * 0.9, 0.0, 0.0, 1.0);
    glRotatef(angles_array_0[7] * 0.9, 1.0, 0.0, 0.0);
    pt8.draw();
    glPopMatrix();

    // Side Left 2
    glPushMatrix();
    glRotatef(10, 0, 1, 0);
    glTranslatef(-1, 0, 1);
    glTranslatef(2.5, -1, 2);
    glRotatef(angles_array_0[4] * 0.9, 1.0, 0.0, 0.0);
    glRotatef(angles_array[4] * 0.9, 0.0, 0.0, 1.0);
    pl5.draw();
    glPopMatrix();

    // Side Right 2
    glPushMatrix();
    glRotatef(170, 0, 1, 0);
    glTranslatef(-3, -1, -5);
    glRotatef(angles_array_0[5] * 0.9, 1.0, 0.0, 0.0);
    glRotatef(angles_array[5] * 0.9, 0.0, 0.0, 1.0);
    pl6.draw();
    glPopMatrix();

    // BODY
    glPushMatrix();
    glTranslatef(-2, -2, 4);
    glTranslatef(0, 0, 0);
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[sk]);
    gluQuadricTexture(quad, GL_TRUE);
    // sphere for spider body
    gluSphere(quad, 4.5, 8, 8);
    glDisable(GL_TEXTURE_2D);
    glRotatef(-70, 0, 1, 0);
    glRotatef(-10, 1, 0, 1);
    glRotatef(-115 * 0.9, 1.0, 0.0, 0.0);
    glRotatef(-115 * 0.9, 0.0, 0.0, 1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(4, -2, 4);
    glTranslatef(0, 0, 0);
    // Enables 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[sk]);
    gluQuadricTexture(quad, GL_TRUE);
    // sphere for spider head
    gluSphere(quad, 3, 8, 8);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

void Spider::angle_set(int numLeg, float curv)
{
    angles_array_0[numLeg] = curv;
}

void Spider::leg_angle_set(int numLeg2, float curv)
{
    angles_array[numLeg2] = curv;
}

void Spider::joint1_angle_set(int numLeg, float curv)
{
    switch (numLeg)
    {
    case 0:
        pf1.joint1_angle_set(curv);
        break;
    case 1:
        pf2.joint1_angle_set(curv);
        break;
    case 2:
        pl3.joint1_angle_set(curv);
        break;
    case 3:
        pl4.joint1_angle_set(curv);
        break;
    case 4:
        pl5.joint1_angle_set(curv);
        break;
    case 5:
        pl6.joint1_angle_set(curv);
        break;
    case 6:
        pt7.joint1_angle_set(curv);
        break;
    case 7:
        pt8.joint1_angle_set(curv);
        break;
    }
}

void Spider::joint2_angle_set(int numLeg, float curv)
{
    switch (numLeg)
    {
    case 0:
        pf1.joint2_angle_set(curv);
        break;
    case 1:
        pf2.joint2_angle_set(curv);
        break;
    case 2:
        pl3.joint2_angle_set(curv);
        break;
    case 3:
        pl4.joint2_angle_set(curv);
        break;
    case 4:
        pl5.joint2_angle_set(curv);
        break;
    case 5:
        pl6.joint2_angle_set(curv);
        break;
    case 6:
        pt7.joint2_angle_set(curv);
        break;
    case 7:
        pt8.joint2_angle_set(curv);
        break;
    }
}

// functions for the  various types of movement of the Legs of the spider

void Spider::movP0()
{
    angle_set(0, getCurveto(0) - 5);
    joint1_angle_set(0, joint1_angle_get(0) - 5);
}

void Spider::movP1()
{
    angle_set(1, getCurveto(1) - 5);
    joint1_angle_set(1, joint1_angle_get(1) - 5);
}
// changed from -5 to -10
void Spider::movP2()
{
    angle_set(2, getCurveto(2) - 5);
    joint1_angle_set(2, joint1_angle_get(2) - 5);
}

void Spider::movP3()
{
    angle_set(3, getCurveto(3) - 5);
    joint1_angle_set(3, joint1_angle_get(3) - 5);
}

void Spider::movP4()
{
    angle_set(4, getCurveto(4) - 5);
    joint1_angle_set(4, joint1_angle_get(4) - 5);
}

void Spider::movP5()
{
    angle_set(5, getCurveto(5) - 5);
    joint1_angle_set(5, joint1_angle_get(5) - 5);
}

void Spider::movP6()
{
    angle_set(6, getCurveto(6) - 5);
    joint1_angle_set(6, joint1_angle_get(6) - 5);
}

void Spider::movP7()
{
    angle_set(7, getCurveto(7) - 5);
    joint1_angle_set(7, joint1_angle_get(7) - 5);
}

// Movement 2
void Spider::movP02()
{
    leg_angle_set(0, leg_angle_get(0) - 10);
    display();
}

void Spider::movP12()
{
    leg_angle_set(1, leg_angle_get(1) + 10);
    display();
}

void Spider::movP22()
{
    leg_angle_set(2, leg_angle_get(2) - 10);
    display();
}

void Spider::movP32()
{
    leg_angle_set(3, leg_angle_get(3) + 10);
    display();
}

void Spider::movP42()
{
    leg_angle_set(4, leg_angle_get(4) - 10);
    display();
}

void Spider::movP52()
{
    leg_angle_set(5, leg_angle_get(5) + 10);
    display();
}

void Spider::movP62()
{
    leg_angle_set(6, leg_angle_get(6) - 10);
    display();
}

void Spider::movP72()
{
    leg_angle_set(7, leg_angle_get(7) + 10);
    display();
}

// Movement 3
void Spider::movP03()
{
    leg_angle_set(0, leg_angle_get(0) + 10);
    display();
}

void Spider::movP13()
{
    leg_angle_set(1, leg_angle_get(1) - 10);
    display();
}

void Spider::movP23()
{
    leg_angle_set(2, leg_angle_get(2) + 10);
    display();
}

void Spider::movP33()
{
    leg_angle_set(3, leg_angle_get(3) - 10);
    display();
}

void Spider::movP43()
{
    leg_angle_set(4, leg_angle_get(4) + 10);
    display();
}

void Spider::movP53()
{
    leg_angle_set(5, leg_angle_get(5) - 10);
    display();
}

void Spider::movP63()
{
    leg_angle_set(6, leg_angle_get(6) + 10);
    display();
}

void Spider::movP73()
{
    leg_angle_set(7, leg_angle_get(7) - 10);
    display();
}

// function for spider walking
// all the Legs and joints will move accordingly.

void Spider::walkLeg()
{

    int fmove1 = 1, fmove2 = 1;

    for (int i = 0; i <= fmove1; i++)
    {
        movP0();
        movP3();
        movP4();
        movP7();
        display();
    }

    for (int i = 0; i <= fmove2; i++)
    {

        angle_set(0, getCurveto(0) + 5);
        angle_set(3, getCurveto(3) + 5);
        angle_set(4, getCurveto(4) + 5);
        angle_set(7, getCurveto(7) + 5);
        display();
    }
    for (int i = 0; i <= fmove1; i++)
    {
        if (i == 0 || i == 2)
        {
            movP03();
            display();
            if (flag)
            {
                xt += 1;
            }
            else
            {
                xt -= 1;
            }
        }
        angle_set(0, getCurveto(0) - 5); // p0
        joint1_angle_set(0, joint1_angle_get(0) + 5);
        if (i == 0 || i == 2)
        {
            movP33();
            display();
        }
        angle_set(3, getCurveto(3) - 5); // p3
        joint1_angle_set(3, joint1_angle_get(3) + 5);
        if (i == 0 || i == 2)
        {
            movP43();
            display();
        }
        angle_set(4, getCurveto(4) - 5); // p4
        joint1_angle_set(4, joint1_angle_get(4) + 5);

        if (i == 0 || i == 2)
        {
            movP73();
            display();
        }
        angle_set(7, getCurveto(7) - 5); // p7
        joint1_angle_set(7, joint1_angle_get(7) + 5);

        angle_set(1, getCurveto(1) - 5); /// p1
        joint1_angle_set(1, joint1_angle_get(1) - 5);

        joint1_angle_set(2, joint1_angle_get(2) - 10);

        angle_set(5, getCurveto(5) - 5);
        joint1_angle_set(5, joint1_angle_get(5) - 5);

        angle_set(6, getCurveto(6) - 5);
        joint1_angle_set(6, joint1_angle_get(6) - 5);
        display();
        joint2_angle_set(1, joint2_angle_get(1) - 6);
        display();

        if (i == 1 || i == 3)
        {
            movP02();
            movP32();
            movP42();
            movP72();
        }
    }

    for (int i = 0; i <= fmove2; i++)
    {
        // returning p1 P2 P5 P6
        angle_set(1, getCurveto(1) + 5);
        display();
        // p2
        angle_set(2, getCurveto(2) + 5);
        display();
        // p5
        angle_set(5, getCurveto(5) + 5);
        display();
        // p6
        angle_set(6, getCurveto(6) + 5);
        display();
        if (i <= 3)
        {
            joint2_angle_set(1, joint2_angle_get(1) + 6);
            display();
        }

        display();
    }
    // legs returning
    for (int i = 0; i <= fmove1; i++)
    {
        // move
        // p1
        if (i == 0 || i == 2)
        {
            movP13();
            display();

            if (flag)
            {
                xt += 1;
            }
            else
            {
                xt -= 1;
            }
        }
        angle_set(1, getCurveto(1) - 5);
        joint1_angle_set(1, joint1_angle_get(1) + 5);
        // p2
        if (i == 0 || i == 2)
        {
            movP23();
            display();
        }
        angle_set(2, getCurveto(2) - 10);
        joint1_angle_set(2, joint1_angle_get(2) + 10);
        // p5
        if (i == 0 || i == 2)
        {
            movP53();
            display();
        }
        angle_set(5, getCurveto(5) - 5);
        joint1_angle_set(5, joint1_angle_get(5) + 5);
        // p6
        if (i == 0 || i == 2)
        {
            movP63();
            display();
        }
        angle_set(6, getCurveto(6) - 5);
        joint1_angle_set(6, joint1_angle_get(6) + 5);
        display();
        if (i == 1 || i == 3)
        {
            movP12();
            movP22();
            movP52();
            movP62();
        }
    }
    angles_array_0[0] = -115 * 10 / 9; // adjusting angle
    angles_array_0[1] = -115 * 10 / 9;
    angles_array_0[2] = -115 * 10 / 9;
    angles_array_0[3] = -115 * 10 / 9;
    angles_array_0[4] = -115 * 10 / 9;
    angles_array_0[5] = -115 * 10 / 9;
    angles_array_0[6] = -115 * 10 / 9;
    angles_array_0[7] = -115 * 10 / 9;
}


Spider s(1.0); // spider object

void init(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    GLfloat diffuseLight[] = {1.0f, 1.0f, 1.0f, 1.0};
    GLfloat ambientLight[] = {0.2f, 0.2f, 0.4f, 1.0};
    GLfloat lightPos[] = {-100.0f, 0.0f, 0.0f, 1.0f};
    // for lighting

    // Enable lighting calculations
    glEnable(GL_LIGHTING);

    // Set the ambient and diffuse lighting properties of the first light source
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, diffuseLight);

    // Set the position of the first light source
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Enable the first light source
    glEnable(GL_LIGHT0);

    // Enable per-vertex color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set the specular color of front and back material
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, diffuseLight);

    // Set the shininess of front and back material to 50
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 50);
    // for sphere object to make spider body
    quad = gluNewQuadric();
    leg = gluNewQuadric();
    LoadGLTextures();
    glClearColor(1.0, 1.0, 1.0, 1.0); // Background
    glClearDepth(1.0);                // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LEQUAL);           // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);          // Enables Depth Testing
    glShadeModel(GL_SMOOTH);          // Enables Smooth Color Shading
}

// for informations about how to use
void displayMenu()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    instructions_bg();
    glPushMatrix();

    glColor3f(0.0f, 0.0f, 0.0f);

    glRasterPos3f(-2, 10, 0.1);
    string text1 = "Instructions";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text1[i]);
    }
    float loc = 8;
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "h : Home";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "w : Web";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "e : Exit";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }

    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "a : 3rd joint Inwards of front left leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "A : 3rd joint Outwards of front left leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "b : 1st joint Inwards of front left leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "B : 1st joint Outwards of front left leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "c : 2nd joint Inwards of front left leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "C : 2nd joint Outwards of front left leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "d : 3rd joint Inwards of front right leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "D : 3rd joint Outwards of front right leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "e : 1st joint Inwards of front right leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "E : 1st joint Outwards of front right leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "f : 2nd joint Inwards of front right leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "F : 2nd joint Outwards of front right leg";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "l : switching between light and dark";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "u : scale up";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "U : scale down";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "w : Spider color will change to black";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "W : Spider color will change to Red";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "x : all the legs Inwards at 1st joint";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }

    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "X : all the legs Outwards at 1st joint";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "y : all the legs Inwards at 3rd joint";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "Y : all the legs Outwards at 3rd joint";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "z : all the legs Inwards at 2nd joint";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "Z : all the legs Outwards at 2nd joint";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "UP KEY : Move Forward";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "DOWN KEY : Move Backward";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "LEFT KEY : Rotate Anticlockwise";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "RIGHT KEY : Rotate Clockwise";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "> : for increasing the speed till 8 (initially the speed is 2, then it will become 4,6 and 8 after pressing the button consecutively)";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    glRasterPos3f(-15, loc, 0.1);
    loc -= 0.7;
    ;
    text1 = "< : for decreasing the speed till 2 (pressing the button consecutively will decrease the speed by 2, minimum speed is 2)";
    for (int i = 0; i < text1.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text1[i]);
    }
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// overall display function

void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!menuFlag)
    {

        if (is_info)
        {
            // display menu
            displayMenu();
        }
        else
        {

            glPushMatrix();
            glRotatef(-theta, 1.0, 0.0, 0.0);
            glRotatef(phi, 0.0, 1.0, 0.0);
            if (!is_room)
            {
                // for spider web
                glPushMatrix();
                if (is_day)
                {
                    glDisable(GL_LIGHTING);
                }

                spiderweb();
                glEnable(GL_LIGHTING);
                glPopMatrix();

                glPushMatrix();
                if (is_day)
                {
                    glDisable(GL_LIGHTING);
                }

                s.draw();
                glEnable(GL_LIGHTING);
                glPopMatrix();
            }
            else
            {
                // for room
                glPushMatrix();
                if (is_day)
                {
                    glDisable(GL_LIGHTING);
                }
                // for room wall
                roomWall();
                glEnable(GL_LIGHTING);

                glPushMatrix();

                glTranslatef(0, 0, -20);
                glRotatef(-90, 1, 0, 0);
                glPushMatrix();
                glTranslatef(0, 0, 2);
                glPushMatrix();

                if (is_day)
                {
                    glDisable(GL_LIGHTING);
                }
                // to draw spider
                s.draw();
                glEnable(GL_LIGHTING);
                glPopMatrix();
                glPopMatrix();
                if (is_day)
                {
                    glDisable(GL_LIGHTING);
                }
                // to draw room floor
                roomFloor();
                glEnable(GL_LIGHTING);

                glPopMatrix();
                glPopMatrix();
            }
            glPopMatrix();

            // writing i character for instructions
            float ixmin = 15, ixmax = 16, iymin = -12, iymax = -11;

            glColor3f(0.0f, 1.0f, 0.0f);
            glPushMatrix();
            glBegin(GL_QUADS);
            glVertex3f(ixmin, iymin, 0.0);
            glVertex3f(ixmax, iymin, 0.0);
            glVertex3f(ixmax, iymax, 0.0);
            glVertex3f(ixmin, iymax, 0.0);
            glEnd();
            glPopMatrix();

            glPushMatrix();
            glColor3f(0.0f, 0.0f, 1.0f);
            glRasterPos3f(ixmin + 0.2, iymin + 0.35, 0.1);

            const char *str1 = "i";
            const char *c;
            for (c = str1; *c != '\0'; c++)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
            }
            glPopMatrix();

            // writing co-ordinate travelled by spider
            glPushMatrix();
            glColor3f(1.0f, 1, 1.0f);
            glRasterPos3f(-15, 8.5, 0.1);
            float wxmin = -15.6, wxmax = -9.5, wymin = 8.2, wymax = 9.5;

            glColor3f(1, 0, 0);
            glBegin(GL_QUADS);

            glVertex3f(wxmin, wymin, 0.0);
            glVertex3f(wxmax, wymin, 0.0);
            glVertex3f(wxmax, wymax, 0.0);
            glVertex3f(wxmin, wymax, 0.0);
            glEnd();
            string text1 = "X " + to_string(tx);
            for (int i = 0; i < text1.length(); i++)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text1[i]);
            }

            glColor3f(1.0f, 1, 1.0f);
            glRasterPos3f(-15, 5.5, 0.1);
            float wxmin1 = -15.6, wxmax1 = -9.5, wymin1 = 5.2, wymax1 = 6.5;

            glColor3f(1, 0, 0);
            glBegin(GL_QUADS);

            glVertex3f(wxmin1, wymin1, 0.0);
            glVertex3f(wxmax1, wymin1, 0.0);
            glVertex3f(wxmax1, wymax1, 0.0);
            glVertex3f(wxmin1, wymax1, 0.0);
            glEnd();
            string text2 = "Y " + to_string(ty);
            for (int i = 0; i < text2.length(); i++)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text2[i]);
            }

            glColor3f(1.0f, 1, 1.0f);
            glRasterPos3f(-15, 2.5, 0.1);
            float wxmin2 = -15.6, wxmax2 = -9.5, wymin2 = 2.2, wymax2 = 3.5;

            glColor3f(1, 0, 0);
            glBegin(GL_QUADS);

            glVertex3f(wxmin2, wymin2, 0.0);
            glVertex3f(wxmax2, wymin2, 0.0);
            glVertex3f(wxmax2, wymax2, 0.0);
            glVertex3f(wxmin2, wymax2, 0.0);
            glEnd();
            string text3 = "Angle " + to_string(rz);
            for (int i = 0; i < text3.length(); i++)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text3[i]);
            }

            glPopMatrix();
        }

        // writing x character to move previous screen
        float cxmin = 15, cxmax = 16, cymin = 11, cymax = 12;

        glColor3f(1.0f, 0.2f, 0.0f);
        glPushMatrix();
        glBegin(GL_QUADS);
        glVertex3f(cxmin, cymin, 0.0);
        glVertex3f(cxmax, cymin, 0.0);
        glVertex3f(cxmax, cymax, 0.0);
        glVertex3f(cxmin, cymax, 0.0);
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.0f, 0.0f, 1.0f);
        glRasterPos3f(cxmin, cymin, 0.1);

        const char *str = "X";
        const char *c;
        for (c = str; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
        glPopMatrix();

        glColor3f(1.0f, 1.0f, 1.0f);
    }
    else
    {

        // menu screen

        // Draw Spider
        glPushMatrix();
        glTranslatef(1, 4, 0);

        glRotatef(inMenuAlongZ, 0, 0, 1);
        glTranslatef(-1, -4, 0);

        glPushMatrix();
        glTranslatef(0, 1, 0);
        if (is_day)
        {
            glDisable(GL_LIGHTING);
        }
        s.draw();
        glEnable(GL_LIGHTING);
        glPopMatrix();

        glPopMatrix();

        glPushMatrix();
        // Draw the title
        if (is_day)
        {
            glDisable(GL_LIGHTING);
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos3f(-1.5, 8, 0.1);
        string text = "SPIDER";
        for (int i = 0; i < text.length(); i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        }
        // glDepthMask(GL_TRUE);
        glEnable(GL_LIGHTING);

        glPopMatrix();

        // Web
        glDisable(GL_LIGHTING);
        glPushMatrix();

        float wxmin = -3, wxmax = 3, wymin = -5, wymax = -3;

        glColor3f(0.0, 0.8, 0.0);
        glBegin(GL_QUADS);

        glVertex3f(wxmin, wymin, 0.0);
        glVertex3f(wxmax, wymin, 0.0);
        glVertex3f(wxmax, wymax, 0.0);
        glVertex3f(wxmin, wymax, 0.0);
        glEnd();

        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos3f(wxmin + 2, wymin + 0.75, 0.1);
        text = "Web";
        for (int i = 0; i < text.length(); i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        }

        glPopMatrix();

        // Home
        glPushMatrix();

        float homeY = 2.5;

        glColor3f(0.0, 0.8, 0.0);
        glBegin(GL_QUADS);
        wymin -= homeY;
        wymax -= homeY;
        glVertex3f(wxmin, wymin, 0.0);
        glVertex3f(wxmax, wymin, 0.0);
        glVertex3f(wxmax, wymax, 0.0);
        glVertex3f(wxmin, wymax, 0.0);
        glEnd();

        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos3f(wxmin + 2, wymin + 0.75, 0.1);
        text = "Home";
        for (int i = 0; i < text.length(); i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        }
        glPopMatrix();

        // Exit
        glPushMatrix();

        float exitY = 2.5;

        glColor3f(0.0, 0.8, 0.0);
        glBegin(GL_QUADS);
        wymin -= exitY;
        wymax -= exitY;
        glVertex3f(wxmin, wymin, 0.0);
        glVertex3f(wxmax, wymin, 0.0);
        glVertex3f(wxmax, wymax, 0.0);
        glVertex3f(wxmin, wymax, 0.0);
        glEnd();

        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos3f(wxmin + 2, wymin + 0.75, 0.1);
        text = "Exit";
        for (int i = 0; i < text.length(); i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        }
        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
        glEnable(GL_LIGHTING);

        glPushMatrix();

        menuBackground();

        glPopMatrix();
    }

    glutSwapBuffers();
}

// if it moves away or towards us ,it takes care by reducing and increasing size accordingly
void reshape(int w, int h)
{
    // Set the viewport to the entire window
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // Set the current matrix to the projection matrix
    glMatrixMode(GL_PROJECTION);

    // Load the identity matrix to reset any previous transformations
    glLoadIdentity();

    // Set up a perspective projection with the given field of view, aspect ratio, and near/far clipping planes
    gluPerspective(pers, (GLfloat)w / (GLfloat)h, 1.0, 80.0);

    // Set the current matrix to the modelview matrix
    glMatrixMode(GL_MODELVIEW);

    // Load the identity matrix to reset any previous transformations
    glLoadIdentity();

    // Translate the scene along the z-axis to provide a view of the scene
    glTranslatef(0.0, 0.0, -10.0);
}

void specialKey(int key, int x, int y)
{
    if (menuFlag)
    {
    }
    else
    {
        switch (key)
        {
        // for spider movement
        case GLUT_KEY_UP:
            s.walkLeg();
            // for spider walking forward according to its speed
            for (int i = 1; i <= speed; i++)
            {
                float dy = (spiderLen * zooms) * (sin(rz * (M_PI / 180.0)));
                float dx = (spiderLen * zooms) * (cos(rz * (M_PI / 180.0)));
                if ((tx + dx) <= 34 and (tx + dx) >= -34 and ((!is_room and (ty + dy) <= 18) or (is_room and (ty + dy) <= 6)) and ((!is_room and (ty + dy) >= -28) or (is_room and (ty + dy) >= -36)))
                {
                    ty += dy;
                    tx += dx;
                }
                else
                {
                    // Spider moved out of bounds during forward movement - play warning sound via IRRKlang audio engine
                    boundaryWarning();
                }
            }

            break;
        case GLUT_KEY_DOWN:
            s.walkLeg();
            // for spider walking backward according to its speed
            for (int i = 1; i <= speed; i++)
            {
                float dy = (spiderLen * zooms) * (sin(rz * (M_PI / 180.0)));
                float dx = (spiderLen * zooms) * (cos(rz * (M_PI / 180.0)));
                if ((tx - dx) <= 34 and (tx - dx) >= -34 and ((!is_room and (ty - dy) <= 18) or (is_room and (ty - dy) <= 6)) and ((!is_room and (ty - dy) >= -28) or (is_room and (ty - dy) >= -36)))
                {
                    ty -= dy;
                    tx -= dx;
                }
                else
                {
                    // Spider moved out of bounds during backward movement - play warning sound via IRRKlang audio engine
                    boundaryWarning();
                }
                //  ty -= (spiderLen*zooms) * (sin(rz * (M_PI / 180.0)));
                //  tx -= (spiderLen*zooms) * (cos(rz * (M_PI / 180.0)));
            }
            break;
        // for spider rotation to its left
        case GLUT_KEY_LEFT:
            s.walkLeg();
            rz = (rz + 15) % 360;

            break;
        // for spider rotation to its right
        case GLUT_KEY_RIGHT:
            s.walkLeg();
            rz = (rz - 15 + 360) % 360;
            break;
        }
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{

    if (menuFlag)
    {
        // for menu function
        switch (key)
        {
        case 'h':
            menuFlag = false;
            is_room = true;
            break;
        case 'w':
            menuFlag = false;
            is_room = false;
            break;
        case 'e':
            exit(0);
        }
    }
    else
    {
        switch (key)
        {

        // for zoomIN
        case 'u':

            zooms = min(1.15, zooms + 0.05);
            cout << zooms << endl;
            break;
        // for zoomOUT
        case 'U':
            zooms = max(0.85, zooms - 0.05);
            cout << zooms << endl;
            break;

        // to increase speed
        case '.':
            speed = min(speed + 1, 4);
            break;
        // to decrease speed
        case ',':
            speed = max(speed - 1, 1);
            break;
        // for lighting effect
        case 'l':
            is_day = !is_day;
            break;
        case 'a':
            if (s.joint2_angle_get(0) > 100)
                break;
            s.joint2_angle_set(0, s.joint2_angle_get(0) + 5);
            break;

        case 'A':
            if (s.joint2_angle_get(0) < 0)
                break;
            s.joint2_angle_set(0, s.joint2_angle_get(0) - 5);

            break;
        case 'b':

            if (s.getCurveto(0) > 0)
                break;
            s.angle_set(0, s.getCurveto(0) + 5);
            break;
        case 'B':

            if (s.getCurveto(0) < -150)
                break;
            s.angle_set(0, s.getCurveto(0) - 5);
            break;
        case 'c':
            printf("%f\n", s.joint1_angle_get(0));
            if (s.joint1_angle_get(0) > 130)
                break;
            s.joint1_angle_set(0, s.joint1_angle_get(0) + 5);
            break;
        case 'C':
            printf("%f\n", s.joint1_angle_get(0));
            if (s.joint1_angle_get(0) < 15)
                break;
            s.joint1_angle_set(0, s.joint1_angle_get(0) - 5);
            break;

        // front right
        case 'd':
            if (s.joint2_angle_get(1) > 100)
                break;
            s.joint2_angle_set(1, s.joint2_angle_get(1) + 5);
            break;

        case 'D':
            if (s.joint2_angle_get(1) < 0)
                break;
            s.joint2_angle_set(1, s.joint2_angle_get(1) - 5);

            break;
        case 'e':
            printf("%f\n", s.getCurveto(1));
            if (s.getCurveto(1) > 0)
                break;
            s.angle_set(1, s.getCurveto(1) + 5);
            break;
        case 'E':
            printf("%f\n", s.getCurveto(1));
            if (s.getCurveto(1) < -150)
                break;
            s.angle_set(1, s.getCurveto(1) - 5);
            break;

        case 'f':
            printf("%f\n", s.joint1_angle_get(1));
            if (s.joint1_angle_get(1) > 130)
                break;
            s.joint1_angle_set(1, s.joint1_angle_get(1) + 5);
            break;
        case 'F':
            printf("%f\n", s.joint1_angle_get(1));
            if (s.joint1_angle_get(1) < 15)
                break;
            s.joint1_angle_set(1, s.joint1_angle_get(1) - 5);
            break;
        
        case 'y':
            for (int i = 0; i < 8; i++)
            {
                if (s.joint2_angle_get(i) > 100)
                    break;
                s.joint2_angle_set(i, s.joint2_angle_get(i) + 5);
            }
            break;

        case 'Y':
            for (int i = 0; i < 8; i++)
            {
                if (s.joint2_angle_get(i) < 0)
                    break;
                s.joint2_angle_set(i, s.joint2_angle_get(i) - 5);
            }
            break;

        case 'z':
            for (int i = 0; i < 8; i++)
            {
                if (s.joint1_angle_get(i) > 130)
                    break;
                s.joint1_angle_set(i, s.joint1_angle_get(i) + 5);
            }
            break;

        case 'Z':
            for (int i = 0; i < 8; i++)
            {
                if (s.joint1_angle_get(i) < 15)
                    break;
                s.joint1_angle_set(i, s.joint1_angle_get(i) - 5);
            }
            break;

        case 'x':
            for (int i = 0; i < 8; i++)
            {
                if (s.getCurveto(i) > 0)
                    break;
                s.angle_set(i, s.getCurveto(i) + 5);
            }
            break;

        case 'X':
            for (int i = 0; i < 8; i++)
            {
                if (s.getCurveto(i) < -150)
                    break;
                s.angle_set(i, s.getCurveto(i) - 5);
            }
            break;
        case 'w':
            sk = 5;
            display();
            break;
        case 'W':
            sk = 3;
            display();
            break;

        case '9':
            pers--;
            break;

        default:
            return;
        }
    }

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    // Initialize IRRKlang sound engine with default parameters
    // createIrrKlangDevice() creates and returns a global sound engine instance
    // engine = createIrrKlangDevice();

    // if (!engine)
    // {
    //     printf("Could not startup engine\n");
    //     return 0; // error starting up the engine
    // }

    glutInit(&argc, argv);

    // Set the display mode to RGBA color, double-buffering, depth buffering, and stencil buffering
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

    // Set the initial window size to 800x600 pixels
    glutInitWindowSize(800, 600);

    // Set the initial window position to (100, 100) on the screen
    glutInitWindowPosition(100, 100);

    // Create a new window with the title "Spider"
    glutCreateWindow("Spider");
    // Initialize the application
    init();

    // Register the display function to be called when the window needs to be redrawn
    glutDisplayFunc(display);

    // Register the reshape function to be called when the window is resized
    glutReshapeFunc(reshape);

    // Register the keyboard function to be called when a keyboard key is pressed
    glutKeyboardFunc(keyboard);

    // Register the mouse callback function to be called when a mouse button is pressed or released
    glutMouseFunc(mouseCallback);

    // Register the motion callback function to be called when the mouse is moved while a button is pressed
    glutMotionFunc(motionCallback);

    // Register the special key function to be called when a special key (like the arrow keys) is pressed
    glutSpecialFunc(specialKey);

    // Enter the GLUT event loop, which handles user input and updates the window
    glutMainLoop();

    if (!LoadGLTextures())
    {
        return 1; // If Texture Didn't Load Return FALSE
    }

    // Cleanup: Release IRRKlang sound engine resources and free allocated memory
    // drop() properly terminates the audio engine via irrklang interface
    // engine->drop();
    return 0;
}
