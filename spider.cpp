// ════════════════════════════════════════════════════════════
// SPIDER — Realistic hierarchical spider
//
// Fixes vs original:
//   • Eyes now sit flush on the head sphere surface (no floating)
//   • Legs fan properly from cephalothorax sides with arched
//     "knees up" posture — looks like a real spider, not a bug
//   • Leg attachment points spread along cephalothorax length
//   • Each leg: coxa→femur(up)→tibia(down)→metatarsus→tarsus
//     giving the characteristic angular profile
//   • Narrow pedicel (waist) visually separates abdomen from
//     cephalothorax
// Everything else (materials, texture calls, shadow, globals) unchanged.
// ════════════════════════════════════════════════════════════

void spiderBodyMat(float spec=0.25f){
    useTex(TEX_SPIDER_SKIN);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(3.2f,2.4f,1.0f);
    glMatrixMode(GL_MODELVIEW);
    mat(0.24f,0.20f,0.18f, 0.92f,0.88f,0.84f, spec,spec*0.8f,spec*0.6f, 55);
}

void spiderLegMat(){
    noTex();
    mat(0.08f,0.05f,0.03f, 0.16f,0.11f,0.07f, 0.35f,0.30f,0.22f, 80);
}

void spiderJointMat(){
    noTex();
    mat(0.06f,0.04f,0.02f, 0.12f,0.08f,0.05f, 0.45f,0.40f,0.30f, 100);
}

// ── Low-level segment/joint drawers (logic unchanged from original) ────────

void drawLegSegment(float x0,float y0,float z0,float x1,float y1,float z1,float r0,float r1){
    float dx=x1-x0, dy=y1-y0, dz=z1-z0;
    float len=sqrtf(dx*dx+dy*dy+dz*dz);
    if(len<0.0001f) return;
    float ax=dz, ay=0.f, az=-dx;
    float al=sqrtf(ax*ax+az*az);
    float ang=acosf(dy/len)*180.0f/(float)M_PI;
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

// ── Realistic spider leg ───────────────────────────────────────────────────
//
//  Real spider side profile:
//
//              KNEE (high)
//             /           \
//  body──coxa              tibia──foot (floor)
//
//  femur goes UP and OUT from body → knee is highest point
//  tibia goes DOWN and forward/backward to the foot
//  This arch is what makes it look like a spider, not a bug.
//
void drawSpiderLeg(int pairIndex, int side, float walkPhase)
{
    // Cephalothorax sits at (0, 0.44, -0.18).
    // Legs attach along its length: pair 0 front, pair 3 rear.
    static const float attachZ[4] = { 0.08f, -0.08f, -0.24f, -0.40f };
    const float attachY  = 0.44f;  // hip height (on side of cephalothorax)
    const float attachX  = 0.42f;  // lateral offset from centre

    // Alternating-tripod gait phase
    static const float pairPhase[4] = { 0.f, (float)M_PI, 0.f, (float)M_PI };
    float p      = walkPhase + pairPhase[pairIndex];
    float lift   = fmaxf(0.f, sinf(p))  * 0.20f;   // foot height off floor
    float stride = cosf(p) * 0.24f;                 // fore-aft swing

    // Front legs (0,1) reach forward; rear legs (2,3) reach backward.
    // Sign makes each pair point outward in its natural direction.
    float dirZ = (pairIndex < 2) ? 1.0f : -1.0f;

    // ── Hip ───────────────────────────────────────────────────────────────
    float hX = side * attachX;
    float hY = attachY;
    float hZ = attachZ[pairIndex];

    // ── Coxa: short, goes sideways and slightly in leg direction ──────────
    float cX = side * (attachX + 0.22f);
    float cY = attachY - 0.04f;
    float cZ = hZ + dirZ * 0.10f;

    // ── Femur: goes UP and far OUT — this creates the "knee arch" ─────────
    float kX = side * (attachX + 0.70f);
    float kY = attachY + 0.38f + lift;   // knee is highest point
    float kZ = cZ + dirZ * 0.28f + stride * 0.35f;

    // ── Tibia: drops steeply DOWN and continues outward ───────────────────
    float aX = side * (attachX + 1.18f);
    float aY = 0.18f + lift * 0.5f;
    float aZ = kZ + dirZ * 0.45f + stride * 0.40f;

    // ── Metatarsus + foot: final reach, grazes the floor ─────────────────
    float fX = side * (attachX + 1.55f);
    float fY = 0.03f + lift;
    float fZ = aZ + dirZ * 0.28f + stride * 0.25f;

    // ── Draw ──────────────────────────────────────────────────────────────
    drawLegJoint   (hX, hY, hZ,  0.086f);
    drawLegSegment (hX, hY, hZ,  cX, cY, cZ,  0.076f, 0.068f);

    drawLegJoint   (cX, cY, cZ,  0.072f);
    drawLegSegment (cX, cY, cZ,  kX, kY, kZ,  0.068f, 0.054f);  // femur rises

    drawLegJoint   (kX, kY, kZ,  0.060f);
    drawLegSegment (kX, kY, kZ,  aX, aY, aZ,  0.054f, 0.038f);  // tibia drops

    drawLegJoint   (aX, aY, aZ,  0.044f);
    drawLegSegment (aX, aY, aZ,  fX, fY, fZ,  0.038f, 0.016f);  // metatarsus

    // Tarsal claw
    mat(0.04f,0.03f,0.02f, 0.07f,0.05f,0.035f, 0.75f,0.65f,0.45f, 120);
    glPushMatrix();
        glTranslatef(fX, fY, fZ);
        glRotatef(side * 15.f, 0, 1, 0);
        glRotatef(35.f, 1, 0, 0);
        glRotatef(-90.f, 1, 0, 0);
        taperedSeg(0.014f, 0.003f, 0.12f, 6);
    glPopMatrix();
}

// ── Main spider model ──────────────────────────────────────────────────────

void drawSpiderModel(){

    // ── ABDOMEN ─────────────────────────────────────────────────────────
    // Large rear oval, sits high and behind
    spiderBodyMat(0.22f);
    glPushMatrix();
        glTranslatef(0, 0.54f, 0.70f);
        glScalef(0.80f, 0.72f, 1.05f);
        sphere(0.54f, 22, 16);
    glPopMatrix();

    // ── PEDICEL (waist) — narrow connector ──────────────────────────────
    spiderBodyMat(0.16f);
    glPushMatrix();
        glTranslatef(0, 0.43f, 0.21f);
        glScalef(0.28f, 0.24f, 0.34f);
        sphere(0.28f, 10, 8);
    glPopMatrix();

    // ── CEPHALOTHORAX ───────────────────────────────────────────────────
    // Flatter, leg-bearing section
    spiderBodyMat(0.30f);
    glPushMatrix();
        glTranslatef(0, 0.44f, -0.18f);
        glScalef(0.90f, 0.52f, 0.96f);
        sphere(0.48f, 22, 16);
    glPopMatrix();

    // ── HEAD (forward lobe) — eyes live here ────────────────────────────
    // Centre: (0, 0.50, -0.66)   effective radius: ~0.32 * scale
    spiderBodyMat(0.32f);
    glPushMatrix();
        glTranslatef(0, 0.50f, -0.66f);
        glScalef(0.66f, 0.58f, 0.62f);
        sphere(0.32f, 18, 12);
    glPopMatrix();

    // ── EYES — on the head sphere surface ───────────────────────────────
    // Head centre = (0, 0.50, -0.66)
    // Front of head (z) = -0.66 - 0.32*0.62 ≈ -0.858
    // We embed eyes slightly into the surface so they sit flush.
    noTex();
    {
        GLfloat eyeAmb[]={0.55f,0.04f,0.0f,1.f};
        GLfloat eyeDif[]={0.90f,0.12f,0.0f,1.f};
        GLfloat eyeSpc[]={1.0f, 0.8f, 0.5f,1.f};
        GLfloat eyeEmi[]={0.70f,0.08f,0.0f,1.f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   eyeAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   eyeDif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  eyeSpc);
        glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 120);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  eyeEmi);

        // Front face of head: z ≈ -0.858, centre y = 0.50
        // Eye rows sit just in front of the face centre (inset by ~eye radius)
        const float eyeZ  = -0.848f;   // flush with head front face
        const float eyeY  =  0.50f;    // primary row — at face centre height
        const float eyeY2 =  0.528f;   // secondary row — slightly above

        // Primary row: 4 eyes, spread ±0.15 of head width (head hw ≈ 0.21)
        const float r1 = 0.036f;
        float pX[4] = {-0.150f, -0.052f, 0.052f, 0.150f};
        for(int i=0;i<4;i++){
            glPushMatrix();
                glTranslatef(pX[i], eyeY, eyeZ);
                glScalef(1.f, 1.f, 0.50f);
                sphere(r1, 8, 6);
            glPopMatrix();
        }

        // Secondary row: slightly smaller, tighter cluster, higher
        eyeEmi[0] = 0.55f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, eyeEmi);
        const float r2 = 0.026f;
        float sX[4] = {-0.100f, -0.036f, 0.036f, 0.100f};
        for(int i=0;i<4;i++){
            glPushMatrix();
                glTranslatef(sX[i], eyeY2, eyeZ + 0.012f);
                glScalef(1.f, 1.f, 0.50f);
                sphere(r2, 8, 6);
            glPopMatrix();
        }
    }
    noEmit();

    // ── 4 PAIRS OF LEGS ─────────────────────────────────────────────────
    // Each pair: one right (+1) and one left (-1).
    // Left side offset by half a cycle (π) for alternating gait.
    for(int pair=0; pair<4; pair++){
        drawSpiderLeg(pair,  1, walkPh);
        drawSpiderLeg(pair, -1, walkPh + (float)M_PI);
    }

    // ── COMPACT MOUTHPARTS ───────────────────────────────────────────────
    for(int s=-1; s<=1; s+=2){
        float sway = sinf(walkPh + s*(float)M_PI*0.18f) * 0.008f;

        // Short pedipalp tucked near the mouth.
        float p0x=s*0.10f, p0y=0.41f, p0z=-0.84f;
        float p1x=s*0.15f, p1y=0.33f, p1z=-0.94f+sway;
        float p2x=s*0.18f, p2y=0.24f, p2z=-1.03f+sway*0.5f;
        spiderLegMat();
        drawLegSegment(p0x,p0y,p0z, p1x,p1y,p1z, 0.028f,0.018f);
        drawLegJoint(p1x,p1y,p1z, 0.018f);
        drawLegSegment(p1x,p1y,p1z, p2x,p2y,p2z, 0.018f,0.006f);

        // Small downward fang.
        mat(0.03f,0.025f,0.018f, 0.06f,0.05f,0.04f, 0.60f,0.56f,0.48f,120);
        glPushMatrix();
            glTranslatef(s*0.07f, 0.33f, -0.90f);
            glRotatef(18.f*s, 0, 1, 0);
            glRotatef(78.f, 1, 0, 0);
            glPushMatrix();
                glRotatef(-90.f, 1, 0, 0);
                taperedSeg(0.014f, 0.003f, 0.12f, 8);
            glPopMatrix();
        glPopMatrix();
    }

    noTex();
}

void drawSpider(){
    float stride = fabsf(spMoveVel)/MOVE_SPEED;
    float bob    = sinf(walkPh*2.f)*0.030f*stride;
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
    drawSpiderModel();
    glPopMatrix();
}

// ════════════════════════════════════════════════════════════
// STENCIL SHADOW PROJECTION
// ════════════════════════════════════════════════════════════
void drawSpiderShadow(){
    if(!lightOn) return;

    float stride = fabsf(spMoveVel)/MOVE_SPEED;
    float bob    = sinf(walkPh*2.f)*0.030f*stride;
    Vec3 right,up,forward;
    surfaceBasis(spSurface,spYaw,right,up,forward);

    const float lx = LIGHTING.x;
    const float ly = RH - LIGHTING.yOffsetFromCeiling;
    const float lz = LIGHTING.z;

    const float shadow[16]={
        ly,  0.f, 0.f, 0.f,
        -lx, 0.f, -lz, -1.f,
        0.f, 0.f, ly, 0.f,
        0.f, 0.f, 0.f, ly
    };

    Vec3 pos=vadd(v3(spX,spY,spZ),vscale(up,bob));
    GLfloat spiderMatrix[16]={
        right.x, right.y, right.z, 0.f,
        up.x, up.y, up.z, 0.f,
        -forward.x, -forward.y, -forward.z, 0.f,
        pos.x, pos.y, pos.z, 1.f
    };

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glColor4f(0.05f, 0.04f, 0.03f, lightBrightness*0.55f);

    glPushMatrix();
        glTranslatef(0, 0.005f, 0);
        glMultMatrixf(shadow);
        glMultMatrixf(spiderMatrix);
        glScalef(0.80f,0.80f,0.80f);
        drawSpiderModel();
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}
