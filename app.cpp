// ════════════════════════════════════════════════════════════
//  HUD
// ════════════════════════════════════════════════════════════
void drawHUD(){
    int W=glutGet(GLUT_WINDOW_WIDTH), H=glutGet(GLUT_WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    glOrtho(0,W,0,H,-1,1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f,0.0f,0.0f,0.60f);
    glBegin(GL_QUADS);
    glVertex2i(0,0); glVertex2i(620,0); glVertex2i(620,170); glVertex2i(0,170);
    glEnd(); glDisable(GL_BLEND);

    glColor3f(1.0f,0.94f,0.72f);
    const char* lines[]={
        "3D Spider Simulation  |  OpenGL/GLUT  |  Bedroom",
        "W/S : Move Forward/Back       A/D : Turn Left/Right",
        "Arrow UP/DOWN : Tilt Camera   Arrow L/R : Orbit",
        "Mouse Drag : Free Orbit       Scroll/+/- : Zoom",
        "L : Light Switch              O : Open/Close Door",
        "ESC : Quit",
    };
    const char* status = lightOn ? "[ LIGHT: ON ]" : "[ LIGHT: OFF ]";
    for(int i=0;i<6;i++){
        glRasterPos2i(14,22+i*24);
        for(const char*s=lines[i];*s;s++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*s);
    }
    glColor3f(lightOn?0.9f:0.45f, lightOn?0.9f:0.45f, lightOn?0.2f:0.45f);
    glRasterPos2i(14,170-18);
    for(const char*s=status;*s;s++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*s);

    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ════════════════════════════════════════════════════════════
//  DISPLAY
// ════════════════════════════════════════════════════════════
void display(){
    // Background: dark blue-grey when light off, warm when on
    if(lightOn)
        glClearColor(0.12f*lightBrightness,0.10f*lightBrightness,0.08f*lightBrightness,1.f);
    else
        glClearColor(0.02f,0.02f,0.04f,1.f);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera
    Vec3 sn=surfaceNormal(spSurface);
    float targetX=spX+sn.x*0.45f;
    float targetY=spY+sn.y*0.45f+0.45f;
    float targetZ=spZ+sn.z*0.45f;
    float ty=DEG2RAD(spYaw+camYawOfs);
    float pr=DEG2RAD(camPitch);
    float ex=targetX+camDist*sinf(ty)*cosf(pr);
    float ey=targetY+camDist*sinf(pr);
    float ez=targetZ+camDist*cosf(ty)*cosf(pr);
    const float CM=0.3f;
    if(ex<-RW+CM)ex=-RW+CM; if(ex>RW-CM)ex=RW-CM;
    if(ez<-RD+CM)ez=-RD+CM; if(ez>RD-CM)ez=RD-CM;
    if(ey<CM)ey=CM;          if(ey>RH-CM)ey=RH-CM;
    gluLookAt(ex,ey,ez, targetX,targetY,targetZ, 0,1,0);

    setupLighting();

    drawRoom();
    drawCeilingLight();
    drawPaintings();
    drawSwitch(-RW+0.12f, 4.0f, -8.0f);
    drawDoor();
    drawWardrobe();
    drawBookshelf();
    drawBed();
    drawBedsideTable();
    drawDresser();
    drawStudyDesk();
    drawWallClock();
    drawArmchair();
    drawSpiderShadow();
    drawSpider();
    drawHUD();

    glutSwapBuffers();
}

void reshape(int w,int h){
    if(!h)h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(55.0,(double)w/h,0.05,400.0);
    glMatrixMode(GL_MODELVIEW);
}

// ════════════════════════════════════════════════════════════
//  INPUT
// ════════════════════════════════════════════════════════════
void tryMove(float fwd,float turn){
    if(turn!=0.f) spYaw+=turn;
    if(fwd!=0.f){
        Vec3 f=surfaceForward(spSurface,spYaw);
        float nx=spX+f.x*fwd;
        float ny=spY+f.y*fwd;
        float nz=spZ+f.z*fwd;
        const float M=1.2f;
        bool moved=true;

        if(spSurface==SURF_FLOOR || spSurface==SURF_OBJECT_TOP){
            ny=spY;
            if(spSurface==SURF_OBJECT_TOP && spObjectIndex>=0 && !insideClimbBox(spObjectIndex,nx,nz,0.18f)){
                spSurface=SURF_FLOOR;
                spObjectIndex=-1;
                spY=0.f;
                ny=0.f;
            }
            if(nx<-RW+M){ spSurface=SURF_WALL_X_NEG; spObjectIndex=-1; spX=-RW+M; spY=ny; spZ=nz; spYaw=0.f; }
            else if(nx>RW-M){ spSurface=SURF_WALL_X_POS; spObjectIndex=-1; spX=RW-M; spY=ny; spZ=nz; spYaw=0.f; }
            else if(nz<-RD+M){ spSurface=SURF_WALL_Z_NEG; spObjectIndex=-1; spX=nx; spY=ny; spZ=-RD+M; spYaw=0.f; }
            else if(nz>RD-M){ spSurface=SURF_WALL_Z_POS; spObjectIndex=-1; spX=nx; spY=ny; spZ=RD-M; spYaw=0.f; }
            else {
                int c=climbBoxAt(nx,nz,0.20f);
                if(spSurface==SURF_FLOOR && c>=0){
                    spSurface=SURF_OBJECT_TOP;
                    spObjectIndex=c;
                    spX=nx; spZ=nz; spY=climbBoxes[c].top;
                } else if(spSurface==SURF_FLOOR && collides(nx,nz)) {
                    moved=false;
                } else {
                    spX=nx; spZ=nz; spY=ny;
                }
            }
        } else if(spSurface==SURF_CEILING){
            if(nx<-RW+M){ spSurface=SURF_WALL_X_NEG; spX=-RW+M; spY=RH-M; spZ=nz; spYaw=180.f; }
            else if(nx>RW-M){ spSurface=SURF_WALL_X_POS; spX=RW-M; spY=RH-M; spZ=nz; spYaw=180.f; }
            else if(nz<-RD+M){ spSurface=SURF_WALL_Z_NEG; spX=nx; spY=RH-M; spZ=-RD+M; spYaw=180.f; }
            else if(nz>RD-M){ spSurface=SURF_WALL_Z_POS; spX=nx; spY=RH-M; spZ=RD-M; spYaw=180.f; }
            else { spX=nx; spY=RH; spZ=nz; }
        } else {
            if(spSurface==SURF_WALL_X_NEG || spSurface==SURF_WALL_X_POS){
                nz=nz<-RD+M?-RD+M:(nz>RD-M?RD-M:nz);
                nx=(spSurface==SURF_WALL_X_NEG)?-RW+M:RW-M;
            } else {
                nx=nx<-RW+M?-RW+M:(nx>RW-M?RW-M:nx);
                nz=(spSurface==SURF_WALL_Z_NEG)?-RD+M:RD-M;
            }
            if(ny>=RH-M){ spSurface=SURF_CEILING; spX=nx; spY=RH; spZ=nz; spYaw=0.f; }
            else if(ny<=0.f){ spSurface=SURF_FLOOR; spObjectIndex=-1; spX=nx; spY=0.f; spZ=nz; spYaw=0.f; }
            else { spX=nx; spY=ny; spZ=nz; }
        }

        if(moved) walkPh+=fabsf(fwd)*4.0f;
        else spMoveVel=0.f;
    }
}

void keyboard(unsigned char k,int,int){
    switch(k){
        case'w':case'W': keyForward=true; break;
        case's':case'S': keyBack=true;    break;
        case'a':case'A': keyLeft=true;    break;
        case'd':case'D': keyRight=true;   break;
        case'l':case'L': lightOn=!lightOn; glutPostRedisplay(); break;
        case'o':case'O': doorOpening=!doorOpening; glutPostRedisplay(); break;
        case'+':case'=': camDist-=0.7f; if(camDist<DMIN)camDist=DMIN; glutPostRedisplay(); break;
        case'-':case'_': camDist+=0.7f; if(camDist>DMAX)camDist=DMAX; glutPostRedisplay(); break;
        case 27: exit(0);
    }
}
void keyboardUp(unsigned char k,int,int){
    switch(k){
        case'w':case'W': keyForward=false; break;
        case's':case'S': keyBack=false;    break;
        case'a':case'A': keyLeft=false;    break;
        case'd':case'D': keyRight=false;   break;
    }
}
void specialKey(int k,int,int){
    switch(k){
        case GLUT_KEY_UP:    camPitch+=2.5f; if(camPitch>82)camPitch=82; break;
        case GLUT_KEY_DOWN:  camPitch-=2.5f; if(camPitch<-15)camPitch=-15; break;
        case GLUT_KEY_LEFT:  camYawOfs-=4.f; break;
        case GLUT_KEY_RIGHT: camYawOfs+=4.f; break;
    }
    glutPostRedisplay();
}
void mouseBtn(int btn,int state,int x,int y){
    if(btn==GLUT_LEFT_BUTTON){mdown=(state==GLUT_DOWN);lmx=x;lmy=y;}
    if(btn==3){camDist-=0.7f;if(camDist<DMIN)camDist=DMIN;glutPostRedisplay();}
    if(btn==4){camDist+=0.7f;if(camDist>DMAX)camDist=DMAX;glutPostRedisplay();}
}
void mouseMove(int x,int y){
    if(!mdown)return;
    camYawOfs+=(x-lmx)*0.42f;
    camPitch  -=(y-lmy)*0.28f;
    if(camPitch>82)camPitch=82; if(camPitch<-15)camPitch=-15;
    lmx=x;lmy=y; glutPostRedisplay();
}

// ════════════════════════════════════════════════════════════
//  TIMER  (60 fps)
// ════════════════════════════════════════════════════════════
void timerFunc(int){
    int now=glutGet(GLUT_ELAPSED_TIME);
    dtSec=(now-lastMs)*0.001f;
    if(dtSec>0.05f)dtSec=0.05f;
    lastMs=now;

    // Light brightness animation (smooth transition)
    float target=lightOn?1.0f:0.0f;
    lightBrightness+=(target-lightBrightness)*dtSec*4.5f;

    // Door animation
    float targetDoor=doorOpening?90.f:0.f;
    doorAngle+=(targetDoor-doorAngle)*dtSec*3.5f;

    // Smooth continuous spider movement.
    float moveInput=(keyForward?1.f:0.f)-(keyBack?1.f:0.f);
    float turnInput=(keyLeft?1.f:0.f)-(keyRight?1.f:0.f);
    float targetMove=moveInput*MOVE_SPEED;
    float targetTurn=turnInput*TURN_SPEED;
    float moveEase=1.f-expf(-dtSec*8.0f);
    float turnEase=1.f-expf(-dtSec*10.0f);
    spMoveVel+=(targetMove-spMoveVel)*moveEase;
    spTurnVel+=(targetTurn-spTurnVel)*turnEase;
    if(fabsf(spMoveVel)<0.01f) spMoveVel=0.f;
    if(fabsf(spTurnVel)<0.01f) spTurnVel=0.f;
    tryMove(spMoveVel*dtSec, spTurnVel*dtSec);
    if(moveInput==0.f && fabsf(spMoveVel)>0.02f) walkPh+=fabsf(spMoveVel)*dtSec*0.8f;
    if(turnInput!=0.f && fabsf(spMoveVel)<0.02f) walkPh+=fabsf(spTurnVel)*dtSec*0.018f;

    // Pendulum (period ~1.8 seconds for realistic grandfather clock)
    pendAngle+=pendDir*55.0f*dtSec;
    if(pendAngle>22.f){pendAngle=22.f;pendDir=-1.f;}
    if(pendAngle<-22.f){pendAngle=-22.f;pendDir=1.f;}

    glutPostRedisplay();
    glutTimerFunc(16,timerFunc,0);
}
