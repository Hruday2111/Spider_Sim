// ════════════════════════════════════════════════════════════
//  LOW-LEVEL GEOMETRY
// ════════════════════════════════════════════════════════════
// Box centred at origin with texture coords
void box(float w,float h,float d,float ts=1.f,float tt=1.f){
    float hw=w*0.5f,hh=h*0.5f,hd=d*0.5f;
    glBegin(GL_QUADS);
    // +Z face
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-hw,-hh, hd);
    glTexCoord2f(ts,0);glVertex3f( hw,-hh, hd);
    glTexCoord2f(ts,tt);glVertex3f( hw, hh, hd);
    glTexCoord2f(0,tt);glVertex3f(-hw, hh, hd);
    // -Z
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);glVertex3f( hw,-hh,-hd);
    glTexCoord2f(ts,0);glVertex3f(-hw,-hh,-hd);
    glTexCoord2f(ts,tt);glVertex3f(-hw, hh,-hd);
    glTexCoord2f(0,tt);glVertex3f( hw, hh,-hd);
    // -X
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0);glVertex3f(-hw,-hh,-hd);
    glTexCoord2f(ts,0);glVertex3f(-hw,-hh, hd);
    glTexCoord2f(ts,tt);glVertex3f(-hw, hh, hd);
    glTexCoord2f(0,tt);glVertex3f(-hw, hh,-hd);
    // +X
    glNormal3f(1,0,0);
    glTexCoord2f(0,0);glVertex3f( hw,-hh, hd);
    glTexCoord2f(ts,0);glVertex3f( hw,-hh,-hd);
    glTexCoord2f(ts,tt);glVertex3f( hw, hh,-hd);
    glTexCoord2f(0,tt);glVertex3f( hw, hh, hd);
    // +Y
    glNormal3f(0,1,0);
    glTexCoord2f(0,0);glVertex3f(-hw, hh, hd);
    glTexCoord2f(ts,0);glVertex3f( hw, hh, hd);
    glTexCoord2f(ts,tt);glVertex3f( hw, hh,-hd);
    glTexCoord2f(0,tt);glVertex3f(-hw, hh,-hd);
    // -Y
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0);glVertex3f(-hw,-hh,-hd);
    glTexCoord2f(ts,0);glVertex3f( hw,-hh,-hd);
    glTexCoord2f(ts,tt);glVertex3f( hw,-hh, hd);
    glTexCoord2f(0,tt);glVertex3f(-hw,-hh, hd);
    glEnd();
}

// Cylinder along Y axis
void cylinder(float r,float h,int sl=16){
    float hs=h*0.5f;
    glBegin(GL_TRIANGLE_STRIP);
    for(int i=0;i<=sl;i++){
        float a=i*2.0f*(float)M_PI/sl;
        float c=cosf(a),s=sinf(a);
        glNormal3f(c,0,s);
        glTexCoord2f((float)i/sl,0); glVertex3f(r*c,-hs,r*s);
        glTexCoord2f((float)i/sl,1); glVertex3f(r*c, hs,r*s);
    }
    glEnd();
    // caps
    for(int cap=-1;cap<=1;cap+=2){
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,cap,0);
        glTexCoord2f(0.5f,0.5f); glVertex3f(0,cap*hs,0);
        for(int i=0;i<=sl;i++){
            float a=i*2.f*(float)M_PI/sl;
            float c=cosf(a),s=sinf(a);
            glTexCoord2f(0.5f+0.5f*c*cap,0.5f+0.5f*s);
            glVertex3f(r*c,cap*hs,r*s);
        }
        glEnd();
    }
}

// Sphere wrapper (for spider body segments)
void sphere(float r,int sl=20,int st=14){
    glutSolidSphere(r,sl,st);
}

// Tapered segment (leg bone)
void taperedSeg(float r0,float r1,float len,int sl=10){
    glBegin(GL_TRIANGLE_STRIP);
    for(int i=0;i<=sl;i++){
        float a=i*2.f*(float)M_PI/sl;
        float c=cosf(a),s=sinf(a);
        float nx0=c,nz0=s; // approx normal
        glNormal3f(nx0,0,nz0); glVertex3f(r0*c,0,r0*s);
        glNormal3f(nx0,0,nz0); glVertex3f(r1*c,len,r1*s);
    }
    glEnd();
}
