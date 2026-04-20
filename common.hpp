/*
 * ╔══════════════════════════════════════════════════════════════════╗
 * ║   3D Spider Simulation  —  Final Professional Version           ║
 * ║   OpenGL (Fixed Pipeline) + GLUT                                ║
 * ╠══════════════════════════════════════════════════════════════════╣
 * ║  FEATURES:                                                       ║
 * ║  • Realistic organic spider (sphere-based body + tapered legs)  ║
 * ║  • Procedural textures (floor planks, walls, spider skin, etc.) ║
 * ║  • Ceiling light with on/off switch – key L                     ║
 * ║  • Projected stencil shadow on floor                            ║
 * ║  • Full bedroom: bed, wardrobe, desk, dresser, bookshelf        ║
 * ║  • Pendulum wall clock, bedside table + lamp                    ║
 * ║  • Bedroom door (opens/closes – key O)                          ║
 * ║  • Mona Lisa + Van Gogh framed paintings                        ║
 * ║  • Tight AABB collision – nothing overlaps                      ║
 * ║  • Smooth 60 fps timer loop                                     ║
 * ╠══════════════════════════════════════════════════════════════════╣
 * ║  CONTROLS:                                                       ║
 * ║  W/S           – move forward / backward                        ║
 * ║  A/D           – turn left / right                              ║
 * ║  Arrow UP/DN   – camera tilt                                    ║
 * ║  Arrow L/R     – camera orbit                                   ║
 * ║  Mouse drag    – free camera orbit                              ║
 * ║  Scroll/+/-    – zoom                                           ║
 * ║  L             – light switch (ceiling)                         ║
 * ║  O             – open/close door                                ║
 * ║  ESC           – quit                                           ║
 * ╠══════════════════════════════════════════════════════════════════╣
 * ║  COMPILE:                                                        ║
 * ║  Linux : g++ main.cpp -o spider -lGL -lGLU -lglut -lm          ║
 * ║  Mac   : g++ main.cpp -o spider -framework OpenGL               ║
 * ║                        -framework GLUT -lm                      ║
 * ║  Win   : g++ main.cpp -o spider -lfreeglut -lopengl32 -lglu32  ║
 * ╚══════════════════════════════════════════════════════════════════╝
 */

#ifdef _WIN32
  #include <windows.h>
#endif
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif
#define DEG2RAD(x) ((x)*(float)M_PI/180.0f)

// ════════════════════════════════════════════════════════════
//  ROOM  (all world units: 1 unit ≈ 1 foot for intuition)
// ════════════════════════════════════════════════════════════
const float RW = 20.0f;   // half-width  X  → room is 40 units wide
const float RH = 11.0f;   // full height Y
const float RD = 22.0f;   // half-depth  Z  → room is 44 units deep

// ════════════════════════════════════════════════════════════
//  GLOBAL STATE
// ════════════════════════════════════════════════════════════
// Spider
float spX=0.f, spY=0.f, spZ=5.f, spYaw=0.f;
float walkPh=0.f;
const float MSTEP=0.14f, TSTEP=2.5f;
bool keyForward=false, keyBack=false, keyLeft=false, keyRight=false;
float spMoveVel=0.f, spTurnVel=0.f;
const float MOVE_SPEED=4.2f, TURN_SPEED=120.f;
enum SurfaceType { SURF_FLOOR, SURF_CEILING, SURF_WALL_X_NEG, SURF_WALL_X_POS, SURF_WALL_Z_NEG, SURF_WALL_Z_POS, SURF_OBJECT_TOP };
SurfaceType spSurface=SURF_FLOOR;
int spObjectIndex=-1;

// Camera
float camPitch=20.f, camDist=9.f, camYawOfs=180.f;
const float DMIN=2.f, DMAX=40.f;
int lmx=-1,lmy=-1; bool mdown=false;

// Light switch
bool lightOn = true;
float lightBrightness = 1.0f;   // animated 0→1 on switch

// Door
float doorAngle = 0.f;           // 0=closed, 90=open
bool  doorOpening = false;

// Clock pendulum
float pendAngle = 0.f, pendDir = 1.f;
int   lastMs    = 0;
float dtSec     = 0.016f;

// ════════════════════════════════════════════════════════════
//  COLLISION  (AABB in XZ plane, y=floor)
// ════════════════════════════════════════════════════════════
struct AABB { float cx,cz,hw,hd; };
// Carefully measured so nothing touches anything
const int NC = 9;
AABB cols[NC] = {
    // wardrobe  (right back corner)
    { RW-3.0f,   -RD+1.8f,   3.0f,  1.8f },
    // tall bookshelf (left back)
    {-RW+2.5f,  -RD+1.5f,   2.5f,  1.5f },
    // bed (left-centre back)
    { -4.5f,    -RD+5.5f,   4.5f,  5.5f },
    // bedside table (left of bed)
    {-10.5f,   -RD+2.5f,   1.5f,  1.5f },
    // dresser / chest of drawers (right of centre)
    {  8.0f,   -RD+1.5f,   2.0f,  1.5f },
    // study desk (right wall)
    { RW-2.0f,   2.0f,      2.0f,  3.5f },
    // door zone (front wall left)  – blocked when door is closed
    {-RW+3.0f,   RD-1.5f,  2.5f,  1.5f },
    // armchair (left mid-room)
    { -9.0f,     5.0f,      1.8f,  2.0f },
    // rug (just decoration, no collision needed → dummy)
    {  0.0f,    99.0f,     0.1f,  0.1f },
};

struct ClimbBox { float cx,cz,hw,hd,top; };
const int NCLIMB = 6;
ClimbBox climbBoxes[NCLIMB] = {
    { -4.5f,    -RD+5.5f,   4.5f,  5.5f,  2.22f }, // bed
    { RW-3.0f,  -RD+1.8f,   3.0f,  1.8f, 10.95f }, // wardrobe / almirah
    {-RW+2.5f,  -RD+1.5f,   2.5f,  1.5f, 10.95f }, // bookshelf
    {  8.0f,    -RD+1.5f,   2.0f,  1.5f,  5.20f }, // dresser
    { RW-2.0f,   2.0f,      2.0f,  3.5f,  3.45f }, // study desk
    {-10.5f,    -RD+2.5f,   1.5f,  1.5f,  2.32f }, // bedside table
};

bool insideClimbBox(int i,float x,float z,float margin=0.f){
    return fabsf(x-climbBoxes[i].cx)<=climbBoxes[i].hw+margin &&
           fabsf(z-climbBoxes[i].cz)<=climbBoxes[i].hd+margin;
}

int climbBoxAt(float x,float z,float margin=0.f){
    for(int i=0;i<NCLIMB;i++) if(insideClimbBox(i,x,z,margin)) return i;
    return -1;
}

struct Vec3 { float x,y,z; };
Vec3 v3(float x,float y,float z){ Vec3 v={x,y,z}; return v; }
Vec3 vadd(Vec3 a,Vec3 b){ return v3(a.x+b.x,a.y+b.y,a.z+b.z); }
Vec3 vscale(Vec3 a,float s){ return v3(a.x*s,a.y*s,a.z*s); }
float vdot(Vec3 a,Vec3 b){ return a.x*b.x+a.y*b.y+a.z*b.z; }
Vec3 vcross(Vec3 a,Vec3 b){ return v3(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x); }
Vec3 vnorm(Vec3 a){
    float l=sqrtf(vdot(a,a));
    return l>0.0001f ? vscale(a,1.0f/l) : v3(0,1,0);
}

Vec3 surfaceNormal(SurfaceType s){
    switch(s){
        case SURF_CEILING:    return v3(0,-1,0);
        case SURF_WALL_X_NEG: return v3(1,0,0);
        case SURF_WALL_X_POS: return v3(-1,0,0);
        case SURF_WALL_Z_NEG: return v3(0,0,1);
        case SURF_WALL_Z_POS: return v3(0,0,-1);
        default:              return v3(0,1,0);
    }
}

Vec3 surfaceBaseForward(SurfaceType s){
    switch(s){
        case SURF_CEILING:    return v3(0,0,1);
        case SURF_WALL_X_NEG:
        case SURF_WALL_X_POS:
        case SURF_WALL_Z_NEG:
        case SURF_WALL_Z_POS: return v3(0,1,0);
        default:              return v3(0,0,-1);
    }
}

Vec3 surfaceForward(SurfaceType s,float yaw){
    Vec3 n=surfaceNormal(s);
    Vec3 f=surfaceBaseForward(s);
    Vec3 r=vnorm(vcross(f,n));
    float a=DEG2RAD(yaw), c=cosf(a), sn=sinf(a);
    return vnorm(vadd(vscale(f,c),vscale(r,sn)));
}

void surfaceBasis(SurfaceType s,float yaw,Vec3 &right,Vec3 &up,Vec3 &forward){
    up=surfaceNormal(s);
    forward=surfaceForward(s,yaw);
    right=vnorm(vcross(forward,up));
}

bool collides(float nx,float nz){
    const float SR=1.0f;
    for(int i=0;i<NC-1;i++){
        // skip door collision when door is open
        if(i==6 && doorAngle>45.f) continue;
        if(fabsf(nx-cols[i].cx)<cols[i].hw+SR &&
           fabsf(nz-cols[i].cz)<cols[i].hd+SR) return true;
    }
    return false;
}
void clampRoom(float &x,float &z){
    float M=1.2f;
    if(x<-RW+M)x=-RW+M; if(x>RW-M)x=RW-M;
    if(z<-RD+M)z=-RD+M; if(z>RD-M)z=RD-M;
}

// ════════════════════════════════════════════════════════════
//  MATERIAL  helper
// ════════════════════════════════════════════════════════════
void mat(float ar,float ag,float ab,
         float dr,float dg,float db,
         float sr,float sg,float sb,float sh,
         float er=0,float eg=0,float eb=0)
{
    // Scale diffuse by lightBrightness so lights feel physical
    float scale = lightOn ? lightBrightness : 0.08f;
    GLfloat a[]={ar*scale,ag*scale,ab*scale,1.f};
    GLfloat d[]={dr,dg,db,1.f};  // diffuse let light handle
    GLfloat s[]={sr,sg,sb,1.f};
    GLfloat e[]={er,eg,eb,1.f};
    (void)a; // ambient scaled separately via glLightModel
    // just set normally; scaling happens in light config
    GLfloat aa[]={ar,ag,ab,1.f};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,  aa);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,  d);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, s);
    glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,sh);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, e);
}
void noEmit(){GLfloat z[]={0,0,0,1};glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,z);}
