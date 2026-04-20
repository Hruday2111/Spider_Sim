// ════════════════════════════════════════════════════════════
//  SPIDER  —  Reference-style hierarchical spider
//
//  Ported from the reference Bone -> Leg -> Spider structure, but
//  scaled to this bedroom scene and using the local procedural texture.
// ════════════════════════════════════════════════════════════

void spiderBodyMat(float spec=0.25f){
    noTex();
    useTex(TEX_SPIDER_SKIN);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    mat(0.10f,0.07f,0.05f,  0.20f,0.14f,0.09f,  spec,spec*0.8f,spec*0.6f, 55);
}
void spiderLegMat(){
    noTex();
    mat(0.08f,0.05f,0.03f,  0.16f,0.11f,0.07f,  0.35f,0.30f,0.22f, 80);
}
void spiderJointMat(){
    noTex();
    mat(0.06f,0.04f,0.02f,  0.12f,0.08f,0.05f,  0.45f,0.40f,0.30f, 100);
}

void drawLegSegment(float x0,float y0,float z0,float x1,float y1,float z1,float r0,float r1){
    float dx=x1-x0, dy=y1-y0, dz=z1-z0;
    float len=sqrtf(dx*dx+dy*dy+dz*dz);
    if(len<0.0001f) return;
    float ax=dz, ay=0.f, az=-dx;
    float al=sqrtf(ax*ax+az*az);
    float ca=dy/len;
    if(ca>1.f) ca=1.f;
    if(ca<-1.f) ca=-1.f;
    float ang=acosf(ca)*180.0f/(float)M_PI;

    spiderLegMat();
    glPushMatrix();
    glTranslatef(x0,y0,z0);
    if(al>0.0001f) glRotatef(ang,ax/al,ay,az/al);
    taperedSeg(r0,r1,len,12);
    glPopMatrix();
}

void drawLegJoint(float x,float y,float z,float r){
    spiderJointMat();
    glPushMatrix();
    glTranslatef(x,y,z);
    sphere(r,10,8);
    glPopMatrix();
}

void drawReferenceSpiderLeg(int legIndex,int side,int pairIndex,float){
    static const float attachZ[4]={-0.44f,-0.18f,0.12f,0.42f};
    static const float reachZ[4]={-1.05f,-0.38f,0.38f,1.05f};
    static const float phaseOffset[8]={0.f,(float)M_PI,(float)M_PI,0.f,0.f,(float)M_PI,(float)M_PI,0.f};

    float p=walkPh+phaseOffset[legIndex];
    float swing=sinf(p);
    float lift=fmaxf(0.f,swing);
    float stride=cosf(p)*0.16f;

    float hipX=side*0.34f, hipY=0.46f, hipZ=attachZ[pairIndex];
    float coxaX=side*0.62f, coxaY=0.36f, coxaZ=hipZ+reachZ[pairIndex]*0.12f;
    float kneeX=side*(1.02f+lift*0.06f), kneeY=0.24f+lift*0.10f, kneeZ=hipZ+reachZ[pairIndex]*0.35f+stride;
    float ankleX=side*(1.50f+lift*0.08f), ankleY=0.13f+lift*0.08f, ankleZ=hipZ+reachZ[pairIndex]*0.72f-stride*0.45f;
    float footX=side*1.92f, footY=0.035f, footZ=hipZ+reachZ[pairIndex]+stride;

    drawLegJoint(hipX,hipY,hipZ,0.085f);
    drawLegSegment(hipX,hipY,hipZ,coxaX,coxaY,coxaZ,0.078f,0.070f);
    drawLegJoint(coxaX,coxaY,coxaZ,0.075f);
    drawLegSegment(coxaX,coxaY,coxaZ,kneeX,kneeY,kneeZ,0.070f,0.055f);
    drawLegJoint(kneeX,kneeY,kneeZ,0.065f);
    drawLegSegment(kneeX,kneeY,kneeZ,ankleX,ankleY,ankleZ,0.055f,0.040f);
    drawLegJoint(ankleX,ankleY,ankleZ,0.048f);
    drawLegSegment(ankleX,ankleY,ankleZ,footX,footY,footZ,0.040f,0.018f);

    mat(0.04f,0.03f,0.02f, 0.07f,0.05f,0.035f, 0.75f,0.65f,0.45f,120);
    glPushMatrix();
    glTranslatef(footX,footY,footZ);
    glRotatef(90,0,0,1);
    taperedSeg(0.018f,0.004f,0.12f,6);
    glPopMatrix();
}

void drawSpider(){
    float stride = fabsf(spMoveVel)/MOVE_SPEED;
    float bob = sinf(walkPh*2.f)*0.035f*stride;
    Vec3 right,up,forward;
    surfaceBasis(spSurface,spYaw,right,up,forward);

    glPushMatrix();
    Vec3 pos=vadd(v3(spX,spY,spZ),vscale(up,bob));
    GLfloat m[16]={
        right.x, right.y, right.z, 0.f,
        up.x, up.y, up.z, 0.f,
        -forward.x, -forward.y, -forward.z, 0.f,
        pos.x, pos.y, pos.z, 1.f
    };
    glMultMatrixf(m);
    glScalef(0.80f,0.80f,0.80f);

    spiderBodyMat(0.22f);
    glPushMatrix(); glTranslatef(0,0.55f,0.72f); glScalef(0.82f,0.72f,1.08f); sphere(0.54f,22,16); glPopMatrix();
    spiderBodyMat(0.32f);
    glPushMatrix(); glTranslatef(0,0.48f,-0.18f); glScalef(0.92f,0.62f,0.88f); sphere(0.48f,22,16); glPopMatrix();
    glPushMatrix(); glTranslatef(0,0.42f,0.28f); glScalef(0.52f,0.44f,0.70f); sphere(0.18f,12,8); glPopMatrix();
    glPushMatrix(); glTranslatef(0,0.47f,-0.66f); glScalef(0.74f,0.56f,0.68f); sphere(0.34f,18,12); glPopMatrix();

    mat(0.06f,0.04f,0.02f, 0.12f,0.08f,0.05f, 0.18f,0.14f,0.10f,45);
    glPushMatrix(); glTranslatef(0,0.83f,0.70f); glScalef(0.26f,0.10f,0.74f); sphere(0.50f,12,8); glPopMatrix();

    for(int i=0;i<4;i++){
        drawReferenceSpiderLeg(i*2, 1,i,walkPh);
        drawReferenceSpiderLeg(i*2+1,-1,i,walkPh+(float)M_PI);
    }

    noTex();
    GLfloat eyeAmb[]={0.6f,0.05f,0.0f,1.f};
    GLfloat eyeDif[]={1.0f,0.15f,0.0f,1.f};
    GLfloat eyeSpc[]={1.0f,0.8f,0.5f,1.f};
    GLfloat eyeEmi[]={0.75f,0.10f,0.0f,1.f};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,  eyeAmb);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,  eyeDif);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, eyeSpc);
    glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,110);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, eyeEmi);
    float eyeX[4]={-0.20f,-0.07f,0.07f,0.20f};
    for(int i=0;i<4;i++){
        glPushMatrix(); glTranslatef(eyeX[i],0.55f,-0.92f);
        glScalef(1.f,1.f,0.5f); sphere(0.045f,8,6); glPopMatrix();
    }
    float eyeX2[4]={-0.14f,-0.05f,0.05f,0.14f};
    eyeEmi[0]=0.60f; eyeEmi[1]=0.08f;
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,eyeEmi);
    for(int i=0;i<4;i++){
        glPushMatrix(); glTranslatef(eyeX2[i],0.63f,-0.87f);
        glScalef(1.f,1.f,0.5f); sphere(0.032f,8,6); glPopMatrix();
    }
    noEmit();

    for(int s=-1;s<=1;s+=2){
        mat(0.10f,0.07f,0.04f, 0.20f,0.14f,0.08f, 0.50f,0.45f,0.35f,90);
        glPushMatrix();
          glTranslatef(s*0.18f,0.36f,-0.90f);
          // main chelicera segment
          glRotatef(40,1,0,0);
          glPushMatrix(); glRotatef(-90,1,0,0);
          taperedSeg(0.070f,0.042f,0.32f,8); glPopMatrix();
          glTranslatef(0,-0.32f,0.0f);
          // fang (dark, curved tip)
          mat(0.04f,0.03f,0.02f, 0.06f,0.04f,0.03f, 0.80f,0.75f,0.60f,130);
          glRotatef(28,1,0,0);
          glPushMatrix(); glRotatef(-90,1,0,0);
          taperedSeg(0.035f,0.008f,0.20f,8); glPopMatrix();
        glPopMatrix();
    }

    for(int s=-1;s<=1;s+=2){
        float p=sinf(walkPh+s*(float)M_PI*0.25f)*0.025f;
        float x0=s*0.16f, y0=0.50f, z0=-0.88f;
        float x1=s*0.30f, y1=0.47f, z1=-1.12f+p;
        float x2=s*0.43f, y2=0.42f, z2=-1.38f+p*0.5f;
        drawLegSegment(x0,y0,z0,x1,y1,z1,0.052f,0.036f);
        drawLegJoint(x1,y1,z1,0.040f);
        drawLegSegment(x1,y1,z1,x2,y2,z2,0.034f,0.010f);
        drawLegJoint(x2,y2,z2,0.018f);
    }
    noTex();

    glPopMatrix();
}

// ════════════════════════════════════════════════════════════
//  STENCIL SHADOW PROJECTION
//  Project spider onto floor plane (y=0) from ceiling light
// ════════════════════════════════════════════════════════════
void drawSpiderShadow(){
    if(!lightOn) return;
    if(spSurface!=SURF_FLOOR && spSurface!=SURF_OBJECT_TOP) return;
    // Shadow matrix projecting from light position onto y=0 plane
    // Light at (0, RH-0.5, 0)
    float lx=0,ly=RH-0.5f,lz=0;
    // Projection matrix for planar shadow (ground plane y=0, normal (0,1,0))
    // M = (L·N - l_y) * I - L * N^T  where N=(0,1,0), L=(lx,ly,lz)
    float dot = ly; // L·N = ly (since N=(0,1,0))
    float sm[16]={
        dot-lx*0,  -ly*0,   -lz*0,  0,
        -lx*1,   dot-ly*1, -lz*1,  0,
        -lx*0,   -ly*0,   dot-lz*0,0,
        -lx*0,   -ly*0,   -lz*0,  dot
    };
    // OpenGL column-major
    float shadow[16]={
        dot,   0,   0, 0,
       -lx,   0,  -lz, 0,
        0,   0,   dot, 0,
        0,   0,   0,  dot
    };
    // Proper shadow matrix:
    // L=(lx,ly,lz,1) point light, plane y=0 → n=(0,1,0,0)
    float nd = ly; // n·L
    shadow[0]=nd-lx*0;  shadow[4]=-lx*1;   shadow[8]=-lx*0;  shadow[12]=-lx*0;
    shadow[1]=-ly*0;    shadow[5]=nd-ly*1; shadow[9]=-ly*0;  shadow[13]=-ly*0;
    shadow[2]=-lz*0;    shadow[6]=-lz*1;   shadow[10]=nd-lz*0;shadow[14]=-lz*0;
    shadow[3]=0;        shadow[7]=0;        shadow[11]=0;      shadow[15]=nd;

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.05f,0.04f,0.03f, lightBrightness*0.55f);
    glPushMatrix();
      glTranslatef(0,0.005f,0); // tiny offset above floor to avoid z-fight
      glMultMatrixf(shadow);
      glTranslatef(spX,0,spZ);
      glRotatef(spYaw,0,1,0);
      // Draw simplified shadow silhouette (just body + leg stubs)
      glScalef(1.44f,1.44f,1.44f);
      glutSolidSphere(0.45f,12,8);
      glScalef(1.5f,0.1f,1.0f);
      glutSolidSphere(0.52f,12,8);
    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}
