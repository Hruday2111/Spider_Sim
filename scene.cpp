// ════════════════════════════════════════════════════════════
//  FURNITURE  HELPERS
// ════════════════════════════════════════════════════════════

// Textured quad  (for paintings, switch plate, etc.)
void texQuad(float w,float h,int texID){
    useTex(texID);
    glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-w/2,-h/2,0);
    glTexCoord2f(1,0); glVertex3f( w/2,-h/2,0);
    glTexCoord2f(1,1); glVertex3f( w/2, h/2,0);
    glTexCoord2f(0,1); glVertex3f(-w/2, h/2,0);
    glEnd();
    noTex();
}

// Framed painting on +Z face
void drawPainting(float cx,float cy,float cz, float pw,float ph, int texID, bool onLeftWall=false){
    glPushMatrix();
    if(onLeftWall){
        glTranslatef(cz,cy,cx); // note: swap for left wall
        glRotatef(90,0,1,0);
    } else {
        glTranslatef(cx,cy,cz);
    }
    // Gold frame drawn as a flat border so no side slab appears from angled views.
    noTex();
    mat(0.30f,0.22f,0.05f, 0.55f,0.42f,0.08f, 0.90f,0.80f,0.45f,80);
    float fb=0.12f; // frame border
    float frameZ = 0.01f;
    glPushMatrix(); glTranslatef(0, ph/2 + fb/2, frameZ); box(pw + fb*2, fb, 0.02f); glPopMatrix();
    glPushMatrix(); glTranslatef(0,-ph/2 - fb/2, frameZ); box(pw + fb*2, fb, 0.02f); glPopMatrix();
    glPushMatrix(); glTranslatef(-pw/2 - fb/2,0, frameZ); box(fb, ph, 0.02f); glPopMatrix();
    glPushMatrix(); glTranslatef( pw/2 + fb/2,0, frameZ); box(fb, ph, 0.02f); glPopMatrix();

    // Canvas uses a neutral material so the imported image is not tinted by the gold frame.
    glPushMatrix();
    glTranslatef(0,0,0.03f);
    mat(0.90f,0.90f,0.90f, 1.0f,1.0f,1.0f, 0.05f,0.05f,0.05f,8);
    texQuad(pw,ph,texID);
    glPopMatrix();
    glPopMatrix();
}

// Wall light switch
void drawSwitch(float x,float y,float z){
    glPushMatrix();
    glTranslatef(x,y,z);
    // plate
    noTex();
    mat(0.80f,0.78f,0.75f, 0.90f,0.88f,0.85f, 0.40f,0.40f,0.38f,30);
    box(0.22f,0.32f,0.04f);
    // toggle
    mat(lightOn?0.90f:0.25f, lightOn?0.88f:0.25f, lightOn?0.80f:0.25f,
        lightOn?1.0f:0.30f,  lightOn?0.98f:0.30f, lightOn?0.88f:0.30f,
        0.5f,0.5f,0.5f,40);
    glTranslatef(0, lightOn?0.04f:-0.04f, 0.04f);
    box(0.10f,0.12f,0.04f);
    glPopMatrix();
}

// Wooden leg (round)
void woodLeg(float x,float y,float z,float r,float h){
    glPushMatrix();
    glTranslatef(x,y+h/2,z);
    useTex(TEX_WOOD_DARK);
    cylinder(r,h,10);
    noTex();
    glPopMatrix();
}

// ════════════════════════════════════════════════════════════
//  FURNITURE PIECES
// ════════════════════════════════════════════════════════════

void drawWardrobe(){
    // Positioned: right back corner
    float wx=RW-3.0f, wz=-RD+1.8f;

    // Main carcass – light wood
    useTex(TEX_WOOD_LIGHT);
    mat(0.34f,0.25f,0.16f, 0.66f,0.50f,0.32f, 0.36f,0.28f,0.18f,32);
    glPushMatrix(); glTranslatef(wx,5.5f,wz); box(5.8f,11.0f,3.2f,4.f,6.f); glPopMatrix();
    // Cornice
    glPushMatrix(); glTranslatef(wx,10.75f,wz+0.08f); box(6.0f,0.50f,3.4f,4.f,0.5f); glPopMatrix();
    // Base plinth
    glPushMatrix(); glTranslatef(wx,0.35f,wz); box(5.8f,0.70f,3.2f,4.f,0.4f); glPopMatrix();

    // Door panels (2 doors) – same wood but slightly different shade
    useTex(TEX_WOOD_LIGHT);
    mat(0.30f,0.22f,0.14f, 0.58f,0.43f,0.27f, 0.32f,0.24f,0.16f,28);
    glPushMatrix(); glTranslatef(wx-1.48f,5.8f,wz+1.62f); box(2.6f,9.8f,0.10f,2.f,5.f); glPopMatrix();
    glPushMatrix(); glTranslatef(wx+1.48f,5.8f,wz+1.62f); box(2.6f,9.8f,0.10f,2.f,5.f); glPopMatrix();
    // Door frame mouldings
    noTex();
    mat(0.20f,0.14f,0.09f, 0.42f,0.29f,0.17f, 0.28f,0.21f,0.14f,24);
    // Left door border strips
    for(int d=-1;d<=1;d+=2){
        float px=wx+d*1.48f;
        glPushMatrix(); glTranslatef(px,9.8f,wz+1.68f); box(2.2f,0.10f,0.06f); glPopMatrix();
        glPushMatrix(); glTranslatef(px,2.2f,wz+1.68f); box(2.2f,0.10f,0.06f); glPopMatrix();
        glPushMatrix(); glTranslatef(px+d*-1.00f,6.f,wz+1.68f); box(0.10f,7.2f,0.06f); glPopMatrix();
        glPushMatrix(); glTranslatef(px+d*1.00f, 6.f,wz+1.68f); box(0.10f,7.2f,0.06f); glPopMatrix();
    }
    // Centre strip
    glPushMatrix(); glTranslatef(wx,5.8f,wz+1.62f); box(0.12f,10.f,0.12f); glPopMatrix();
    // Gold handles
    mat(0.40f,0.32f,0.08f, 0.68f,0.55f,0.14f, 0.95f,0.88f,0.55f,110);
    glPushMatrix(); glTranslatef(wx-0.60f,6.0f,wz+1.74f); box(0.09f,0.55f,0.09f); glPopMatrix();
    glPushMatrix(); glTranslatef(wx+0.60f,6.0f,wz+1.74f); box(0.09f,0.55f,0.09f); glPopMatrix();
}

void drawBookshelf(){
    float bx=-RW+2.5f, bz=-RD+1.5f;
    // Main carcass
    useTex(TEX_WOOD_DARK);
    mat(0.18f,0.11f,0.06f, 0.38f,0.24f,0.13f, 0.22f,0.16f,0.10f,20);
    glPushMatrix(); glTranslatef(bx,5.5f,bz); box(4.8f,11.0f,2.8f,3.f,6.f); glPopMatrix();
    // Shelves (5 shelves)
    noTex();
    mat(0.20f,0.13f,0.07f, 0.42f,0.27f,0.14f, 0.18f,0.13f,0.08f,15);
    for(int s=0;s<5;s++){
        glPushMatrix(); glTranslatef(bx,1.2f+s*1.9f,bz); box(4.6f,0.12f,2.6f); glPopMatrix();
    }
    // Books on shelves (colourful spines)
    float bookColors[][3]={{0.7f,0.1f,0.1f},{0.1f,0.2f,0.7f},{0.1f,0.5f,0.2f},
                           {0.6f,0.5f,0.1f},{0.5f,0.1f,0.5f},{0.1f,0.5f,0.5f},
                           {0.7f,0.35f,0.1f},{0.8f,0.8f,0.2f},{0.3f,0.3f,0.3f}};
    float bw[9]={0.28f,0.22f,0.32f,0.24f,0.18f,0.26f,0.30f,0.20f,0.24f};
    for(int shelf=0;shelf<5;shelf++){
        float bX=bx-1.9f;
        for(int k=0;k<8&&bX<bx+2.0f;k++){
            int c=k%9;
            mat(bookColors[c][0]*0.3f,bookColors[c][1]*0.3f,bookColors[c][2]*0.3f,
                bookColors[c][0],bookColors[c][1],bookColors[c][2],
                0.25f,0.22f,0.18f,15);
            float bh=1.4f+noise2(shelf*10+k,shelf)*0.5f;
            glPushMatrix(); glTranslatef(bX+bw[k%9]/2+0.01f, 1.26f+shelf*1.9f+bh/2, bz);
            box(bw[k%9],bh,0.55f); glPopMatrix();
            bX+=bw[k%9]+0.02f;
        }
    }
}

void drawBed(){
    float bdX=-4.5f, bdZ=-RD+5.5f;
    // Lower the whole frame and extend the support structure so the bed sits on the floor.
    useTex(TEX_WOOD_DARK);
    mat(0.18f,0.11f,0.06f, 0.40f,0.25f,0.13f, 0.20f,0.14f,0.08f,16);
    glPushMatrix(); glTranslatef(bdX,0.34f,bdZ); box(7.9f,0.34f,9.4f,3.f,3.f); glPopMatrix();
    glPushMatrix(); glTranslatef(bdX-4.05f,0.63f,bdZ); box(0.30f,0.58f,9.6f,1.f,3.f); glPopMatrix();
    glPushMatrix(); glTranslatef(bdX+4.05f,0.63f,bdZ); box(0.30f,0.58f,9.6f,1.f,3.f); glPopMatrix();
    glPushMatrix(); glTranslatef(bdX,0.63f,bdZ+4.85f); box(8.1f,0.58f,0.30f,3.f,1.f); glPopMatrix();
    glPushMatrix(); glTranslatef(bdX,0.20f,bdZ); box(7.3f,0.18f,8.8f,2.f,2.f); glPopMatrix();
    // Mattress
    noTex();
    mat(0.62f,0.58f,0.55f, 0.84f,0.78f,0.74f, 0.18f,0.16f,0.14f,12);
    glPushMatrix(); glTranslatef(bdX,0.98f,bdZ+0.25f); box(7.55f,0.52f,8.65f); glPopMatrix();
    // Bedsheet (fabric texture)
    useTex(TEX_FABRIC_BED);
    mat(0.48f,0.30f,0.24f, 0.72f,0.47f,0.38f, 0.15f,0.10f,0.08f,8);
    glPushMatrix(); glTranslatef(bdX,1.30f,bdZ+1.0f); box(7.50f,0.10f,6.50f,2.f,2.f); glPopMatrix();
    // Blanket fold
    noTex();
    mat(0.44f,0.27f,0.20f, 0.66f,0.41f,0.30f, 0.12f,0.08f,0.06f,8);
    glPushMatrix(); glTranslatef(bdX,1.46f,bdZ+2.5f); glScalef(1,0.45f,1); box(7.5f,0.48f,1.5f); glPopMatrix();
    // Pillows x2
    mat(0.80f,0.76f,0.72f, 0.92f,0.88f,0.84f, 0.22f,0.20f,0.18f,18);
    glPushMatrix(); glTranslatef(bdX-1.8f,1.48f,bdZ-3.25f); box(3.0f,0.42f,1.50f); glPopMatrix();
    glPushMatrix(); glTranslatef(bdX+1.8f,1.48f,bdZ-3.25f); box(3.0f,0.42f,1.50f); glPopMatrix();
    // Headboard, scaled to realistic bedroom proportions.
    useTex(TEX_WOOD_DARK);
    mat(0.17f,0.10f,0.06f, 0.36f,0.23f,0.12f, 0.22f,0.16f,0.10f,20);
    glPushMatrix(); glTranslatef(bdX,2.15f,bdZ-5.02f); box(8.2f,3.2f,0.36f,5.f,2.f); glPopMatrix();
    // Headboard carved panel insets
    noTex();
    mat(0.13f,0.08f,0.05f, 0.28f,0.17f,0.09f, 0.16f,0.11f,0.07f,15);
    glPushMatrix(); glTranslatef(bdX-2.0f,2.25f,bdZ-4.83f); box(2.4f,2.0f,0.08f); glPopMatrix();
    glPushMatrix(); glTranslatef(bdX+2.0f,2.25f,bdZ-4.83f); box(2.4f,2.0f,0.08f); glPopMatrix();
    // Footboard
    useTex(TEX_WOOD_DARK);
    mat(0.17f,0.10f,0.06f, 0.36f,0.23f,0.12f, 0.20f,0.14f,0.08f,16);
    glPushMatrix(); glTranslatef(bdX,1.12f,bdZ+5.05f); box(8.1f,1.20f,0.34f,5.f,1.5f); glPopMatrix();
    // Bed legs (4 corners)
    noTex();
    mat(0.12f,0.07f,0.04f, 0.26f,0.16f,0.08f, 0.15f,0.10f,0.06f,18);
    float blx[4]={-3.5f,3.5f,-3.5f,3.5f}, blz[4]={-4.5f,-4.5f,4.5f,4.5f};
    for(int i=0;i<4;i++) woodLeg(bdX+blx[i],0,bdZ+blz[i],0.22f,0.34f);
    // Mid-span supports keep the centre from appearing to float.
    for(int i=0;i<2;i++) woodLeg(bdX-1.8f+i*3.6f,0,bdZ-0.2f,0.18f,0.30f);
}

void drawBedsideTable(){
    float btX=-10.5f, btZ=-RD+2.5f;
    // Body
    useTex(TEX_WOOD_DARK);
    mat(0.22f,0.10f,0.06f, 0.46f,0.24f,0.14f, 0.30f,0.18f,0.12f,24);
    glPushMatrix(); glTranslatef(btX,1.10f,btZ); box(2.5f,2.20f,2.5f,2.f,1.5f); glPopMatrix();
    // Drawer
    noTex();
    mat(0.28f,0.14f,0.08f, 0.55f,0.30f,0.18f, 0.24f,0.15f,0.10f,20);
    glPushMatrix(); glTranslatef(btX,1.20f,btZ+1.26f); box(2.30f,0.78f,0.08f); glPopMatrix();
    // Handle
    mat(0.38f,0.30f,0.08f, 0.65f,0.52f,0.12f, 0.92f,0.85f,0.52f,110);
    glPushMatrix(); glTranslatef(btX,1.20f,btZ+1.32f); box(0.08f,0.08f,0.06f); glPopMatrix();
    // Bedside lamp (base column)
    mat(0.18f,0.18f,0.22f, 0.30f,0.30f,0.45f, 0.40f,0.40f,0.55f,45);
    glPushMatrix(); glTranslatef(btX,2.62f,btZ); cylinder(0.09f,1.0f,12); glPopMatrix();
    // Lamp base disc
    mat(0.15f,0.15f,0.20f, 0.28f,0.28f,0.42f, 0.38f,0.38f,0.52f,40);
    glPushMatrix(); glTranslatef(btX,2.15f,btZ); cylinder(0.28f,0.08f,12); glPopMatrix();
    // Lampshade (cream fabric conical)
    noTex();
    mat(0.70f,0.62f,0.48f, 0.90f,0.82f,0.66f, 0.25f,0.22f,0.18f,20);
    glPushMatrix(); glTranslatef(btX,3.0f,btZ);
    // Conical shade: use truncated cone
    glBegin(GL_TRIANGLE_STRIP);
    int sl=16; float rBot=0.55f, rTop=0.22f, sh=0.75f;
    for(int i=0;i<=sl;i++){
        float a=i*2.f*(float)M_PI/sl, c=cosf(a), s=sinf(a);
        glNormal3f(c*0.8f,0.2f,s*0.8f);
        glVertex3f(rBot*c,-sh/2,rBot*s);
        glVertex3f(rTop*c, sh/2,rTop*s);
    }
    glEnd();
    glPopMatrix();
    // Decorative bulb only; illumination is handled exclusively by GL_LIGHT0.
    mat(0.18f,0.14f,0.08f, 0.95f,0.78f,0.45f, 1.f,0.85f,0.50f,10);
    glPushMatrix(); glTranslatef(btX,3.05f,btZ); sphere(0.10f,8,6); glPopMatrix();
    noEmit();
    // Items on table: book, glass
    noTex();
    mat(0.20f,0.05f,0.05f, 0.40f,0.10f,0.10f, 0.20f,0.10f,0.10f,10);
    glPushMatrix(); glTranslatef(btX+0.6f,2.28f,btZ-0.4f); glRotatef(15,0,1,0); box(0.60f,0.12f,0.45f); glPopMatrix();
    mat(0.38f,0.38f,0.50f, 0.55f,0.55f,0.72f, 0.70f,0.70f,0.80f,60);
    glPushMatrix(); glTranslatef(btX-0.5f,2.36f,btZ+0.3f); cylinder(0.09f,0.28f,10); glPopMatrix();
}

void drawDresser(){
    float dx=8.0f, dz=-RD+1.5f;
    useTex(TEX_WOOD_DARK);
    mat(0.24f,0.12f,0.07f, 0.48f,0.26f,0.15f, 0.34f,0.22f,0.14f,28);
    glPushMatrix(); glTranslatef(dx,2.5f,dz); box(3.8f,5.0f,2.8f,3.f,3.f); glPopMatrix();
    // Top surface
    mat(0.30f,0.16f,0.09f, 0.58f,0.32f,0.18f, 0.38f,0.25f,0.15f,32);
    glPushMatrix(); glTranslatef(dx,5.1f,dz+0.05f); box(4.0f,0.14f,2.9f,3.f,2.f); glPopMatrix();
    // 5 drawers
    noTex();
    mat(0.26f,0.13f,0.08f, 0.52f,0.28f,0.17f, 0.25f,0.16f,0.10f,20);
    for(int i=0;i<5;i++){
        glPushMatrix(); glTranslatef(dx,0.42f+i*0.90f,dz+1.41f); box(3.5f,0.78f,0.08f); glPopMatrix();
    }
    // Drawer handles (gold)
    mat(0.38f,0.30f,0.08f, 0.65f,0.52f,0.12f, 0.92f,0.85f,0.52f,110);
    for(int i=0;i<5;i++){
        glPushMatrix(); glTranslatef(dx,0.42f+i*0.90f,dz+1.48f); box(0.60f,0.09f,0.07f); glPopMatrix();
    }
    // Wall-mounted mirror kept close to the back wall so it does not jut into the scene.
    mat(0.25f,0.20f,0.14f, 0.45f,0.35f,0.24f, 0.30f,0.25f,0.18f,22);
    glPushMatrix(); glTranslatef(dx,7.15f,-RD+0.22f); box(2.45f,3.85f,0.08f); glPopMatrix();
    // Mirror glass
    mat(0.35f,0.40f,0.45f, 0.58f,0.62f,0.66f, 0.90f,0.92f,0.94f,120);
    glPushMatrix(); glTranslatef(dx,7.15f,-RD+0.27f); box(2.15f,3.55f,0.03f); glPopMatrix();
    // Perfume bottles on dresser
    mat(0.50f,0.35f,0.55f, 0.70f,0.50f,0.78f, 0.80f,0.75f,0.85f,80);
    glPushMatrix(); glTranslatef(dx-1.0f,5.42f,dz); cylinder(0.12f,0.50f,10); glPopMatrix();
    mat(0.55f,0.45f,0.20f, 0.75f,0.65f,0.30f, 0.85f,0.80f,0.60f,90);
    glPushMatrix(); glTranslatef(dx+0.6f,5.42f,dz+0.3f); cylinder(0.09f,0.38f,10); glPopMatrix();
}

void drawStudyDesk(){
    // Right wall
    float dkX=RW-2.0f, dkZ=2.0f;
    // Desktop surface
    useTex(TEX_WOOD_DARK);
    mat(0.20f,0.12f,0.08f, 0.42f,0.27f,0.18f, 0.30f,0.21f,0.14f,28);
    glPushMatrix(); glTranslatef(dkX-0.90f,3.30f,dkZ); box(2.0f,0.14f,6.8f,2.f,4.f); glPopMatrix();
    // Legs (4 sturdy wood legs)
    noTex();
    mat(0.15f,0.09f,0.06f, 0.33f,0.21f,0.13f, 0.24f,0.16f,0.10f,22);
    float dlz[4]={-3.0f,-3.0f,3.0f,3.0f};
    float dlx[4]={0.0f,-1.6f,0.0f,-1.6f};
    for(int i=0;i<4;i++) woodLeg(dkX+dlx[i]-0.05f,0,dkZ+dlz[i],0.16f,3.30f);
    // Side panel (modesty panel connecting right-wall side legs)
    useTex(TEX_WOOD_DARK);
    mat(0.18f,0.11f,0.07f, 0.38f,0.24f,0.15f, 0.24f,0.16f,0.10f,22);
    glPushMatrix(); glTranslatef(dkX-0.05f,1.62f,dkZ); box(0.12f,3.24f,6.8f,0.5f,3.f); glPopMatrix();
    // Laptop, rotated so the screen faces the centre of the room.
    noTex();
    glPushMatrix();
    glTranslatef(dkX-1.02f,3.47f,dkZ);
    glRotatef(-90,0,1,0);
    mat(0.42f,0.44f,0.46f, 0.68f,0.70f,0.72f, 0.82f,0.84f,0.86f,80);
    glPushMatrix(); glTranslatef(0,0,0.12f); box(2.35f,0.10f,1.45f); glPopMatrix();
    // Lid leans back slightly while its front face points toward the chair.
    glPushMatrix();
    glTranslatef(0,0.74f,-0.55f);
    glRotatef(-10,1,0,0);
    box(2.35f,1.45f,0.10f);
    mat(0.08f,0.11f,0.18f, 0.30f,0.42f,0.68f, 0.55f,0.65f,0.85f,60);
    glPushMatrix(); glTranslatef(0,0,0.06f); box(2.05f,1.15f,0.035f); glPopMatrix();
    glPopMatrix();
    noEmit();
    // Keyboard deck and trackpad are on the laptop base.
    mat(0.10f,0.11f,0.13f, 0.18f,0.20f,0.23f, 0.35f,0.38f,0.42f,35);
    glPushMatrix(); glTranslatef(0,0.08f,0.08f); box(1.80f,0.035f,0.42f); glPopMatrix();
    mat(0.08f,0.09f,0.11f, 0.14f,0.15f,0.18f, 0.28f,0.30f,0.34f,30);
    glPushMatrix(); glTranslatef(0,-0.005f,0.55f); box(0.70f,0.035f,0.34f); glPopMatrix();
    glPopMatrix();
    // Books stacked
    float bkc[][3]={{0.6f,0.1f,0.1f},{0.1f,0.4f,0.6f},{0.1f,0.5f,0.2f}};
    for(int b=0;b<3;b++){
        mat(bkc[b][0]*0.3f,bkc[b][1]*0.3f,bkc[b][2]*0.3f,
            bkc[b][0],bkc[b][1],bkc[b][2], 0.2f,0.18f,0.15f,12);
        glPushMatrix(); glTranslatef(dkX-1.0f,3.40f+b*0.14f,dkZ-2.2f); box(1.0f,0.12f,0.72f); glPopMatrix();
    }
    // Desk chair: opposite the laptop and facing the desk.
    float chX=dkX-3.20f, chZ=dkZ;
    glPushMatrix();
    glTranslatef(chX,0,chZ);
    glRotatef(90,0,1,0);
    noTex();
    mat(0.04f,0.16f,0.17f, 0.08f,0.36f,0.38f, 0.16f,0.28f,0.30f,24);
    glPushMatrix(); glTranslatef(0,2.42f,0); box(1.55f,0.28f,1.55f); glPopMatrix();
    glPushMatrix(); glTranslatef(0,3.22f,-0.70f); box(1.48f,1.70f,0.22f); glPopMatrix();
    // Rear supports attach the backrest to the rear edge of the seat.
    mat(0.08f,0.08f,0.09f, 0.15f,0.15f,0.17f, 0.35f,0.35f,0.38f,45);
    glPushMatrix(); glTranslatef(-0.58f,2.70f,-0.67f); box(0.14f,0.90f,0.14f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.58f,2.70f,-0.67f); box(0.14f,0.90f,0.14f); glPopMatrix();
    // Central gas lift and under-seat plate.
    glPushMatrix(); glTranslatef(0,1.16f,0); cylinder(0.12f,2.20f,12); glPopMatrix();
    glPushMatrix(); glTranslatef(0,2.24f,0); cylinder(0.36f,0.10f,16); glPopMatrix();
    // 5-point floor base with spokes, so the chair reads as one object.
    glPushMatrix(); glTranslatef(0,0.12f,0); cylinder(0.20f,0.24f,14); glPopMatrix();
    for(int i=0;i<5;i++){
        float a=i*2.f*(float)M_PI/5;
        glPushMatrix();
        glTranslatef(cosf(a)*0.45f,0.18f,sinf(a)*0.45f);
        glRotatef(-a*180.0f/(float)M_PI,0,1,0);
        box(1.05f,0.10f,0.16f);
        glPopMatrix();
        glPushMatrix(); glTranslatef(cosf(a)*1.02f,0.11f,sinf(a)*1.02f);
        sphere(0.11f,8,6); glPopMatrix();
    }
    glPopMatrix();
}

void drawWallClock(){
    // Elegant grandfather-clock-style wall clock on left wall
    float cx=-RW+0.12f, cy=6.0f, cz=-4.0f;
    glPushMatrix();
    glTranslatef(cx,cy,cz);
    glRotatef(90,0,1,0);

    // Clock case
    useTex(TEX_WOOD_DARK);
    mat(0.20f,0.09f,0.05f, 0.44f,0.20f,0.10f, 0.32f,0.18f,0.10f,30);
    // Lower trunk
    glPushMatrix(); glTranslatef(0,-2.0f,0); box(1.0f,4.0f,0.65f,1.f,3.f); glPopMatrix();
    // Hood (top section wider)
    glPushMatrix(); glTranslatef(0,0.8f,0); box(1.30f,2.4f,0.75f,1.f,2.f); glPopMatrix();
    // Bonnet top (arched)
    glPushMatrix(); glTranslatef(0,2.2f,0); glScalef(1.0f,0.5f,0.8f); sphere(0.68f,14,8); glPopMatrix();
    // Base / plinth
    glPushMatrix(); glTranslatef(0,-4.2f,0); box(1.15f,0.50f,0.80f,1.f,0.5f); glPopMatrix();

    // Clock face (cream)
    noTex();
    mat(0.84f,0.76f,0.60f, 0.96f,0.88f,0.70f, 0.38f,0.30f,0.20f,32);
    glPushMatrix(); glTranslatef(0,0.9f,0.43f); glRotatef(90,1,0,0); cylinder(0.52f,0.06f,24); glPopMatrix();
    // Hour markers (12 small boxes around dial)
    mat(0.12f,0.10f,0.08f, 0.22f,0.18f,0.15f, 0.15f,0.12f,0.10f,15);
    for(int hr=0;hr<12;hr++){
        float a=hr*2.f*(float)M_PI/12 - (float)M_PI/2;
        float mx=cosf(a)*0.40f, my=sinf(a)*0.40f;
        glPushMatrix(); glTranslatef(mx,0.9f+my,0.50f); box(0.04f,0.09f,0.035f); glPopMatrix();
    }
    // Hour hand
    mat(0.10f,0.08f,0.06f, 0.18f,0.14f,0.10f, 0.20f,0.16f,0.12f,25);
    glPushMatrix(); glTranslatef(0,0.9f,0.51f); glRotatef(-60,0,0,1);
    box(0.04f,0.28f,0.04f); glPopMatrix();
    // Minute hand
    mat(0.08f,0.06f,0.04f, 0.14f,0.10f,0.08f, 0.18f,0.14f,0.10f,28);
    glPushMatrix(); glTranslatef(0,0.9f,0.51f); glRotatef(-120,0,0,1);
    box(0.03f,0.38f,0.03f); glPopMatrix();
    // Front glass window and visible animated pendulum.
    mat(0.08f,0.05f,0.03f, 0.18f,0.10f,0.06f, 0.20f,0.14f,0.08f,22);
    glPushMatrix(); glTranslatef(0,-1.75f,0.48f); box(0.74f,2.45f,0.05f); glPopMatrix();
    mat(0.10f,0.14f,0.16f, 0.20f,0.28f,0.32f, 0.65f,0.75f,0.80f,80);
    glPushMatrix(); glTranslatef(0,-1.75f,0.52f); box(0.54f,2.10f,0.035f); glPopMatrix();
    mat(0.42f,0.32f,0.06f, 0.78f,0.60f,0.14f, 0.95f,0.86f,0.42f,120);
    glPushMatrix();
    glTranslatef(0,-0.75f,0.56f);
    glRotatef(pendAngle,0,0,1);
    glPushMatrix(); glTranslatef(0,-0.78f,0); box(0.035f,1.55f,0.035f); glPopMatrix();
    glPushMatrix(); glTranslatef(0,-1.62f,0); glRotatef(90,1,0,0); cylinder(0.22f,0.055f,24); glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

void drawArmchair(){
    float ax=-9.0f, az=5.0f;
    // Wooden support frame first, tucked into the cushion so pieces stay intact.
    useTex(TEX_WOOD_DARK);
    mat(0.18f,0.10f,0.06f, 0.38f,0.22f,0.13f, 0.30f,0.20f,0.12f,28);
    glPushMatrix(); glTranslatef(ax,1.18f,az); box(3.05f,0.22f,2.75f,2.f,2.f); glPopMatrix();
    float alx[4]={-1.12f,1.12f,-1.12f,1.12f}, alz[4]={-1.02f,-1.02f,1.02f,1.02f};
    for(int i=0;i<4;i++) woodLeg(ax+alx[i],0,az+alz[i],0.14f,1.28f);

    // Seat cushion
    noTex();
    mat(0.06f,0.20f,0.18f, 0.12f,0.42f,0.38f, 0.20f,0.30f,0.28f,24);
    glPushMatrix(); glTranslatef(ax,1.62f,az); box(2.90f,0.70f,2.70f); glPopMatrix();
    // Back cushion overlaps the rear of the seat instead of floating behind it.
    mat(0.05f,0.17f,0.16f, 0.10f,0.34f,0.32f, 0.18f,0.26f,0.25f,22);
    glPushMatrix(); glTranslatef(ax,2.85f,az-1.15f); box(2.95f,2.45f,0.70f); glPopMatrix();
    // Armrests overlap the cushion sides and sit on the frame.
    mat(0.04f,0.14f,0.14f, 0.08f,0.30f,0.30f, 0.16f,0.24f,0.24f,22);
    glPushMatrix(); glTranslatef(ax-1.55f,2.10f,az); box(0.62f,1.55f,2.80f); glPopMatrix();
    glPushMatrix(); glTranslatef(ax+1.55f,2.10f,az); box(0.62f,1.55f,2.80f); glPopMatrix();
}

void drawDoor(){
    // Door in front wall (left side), opens inward
    float dx=-RW+3.0f, dz=RD-0.1f;
    // Door frame
    noTex();
    mat(0.75f,0.72f,0.68f, 0.88f,0.85f,0.82f, 0.25f,0.24f,0.22f,18);
    glPushMatrix(); glTranslatef(dx,4.2f,dz);
    box(4.6f,8.4f+0.5f,0.30f); // frame surround
    glPopMatrix();
    // Door panel (hinged on left side, opens inward)
    // Hinge point is at dx-2.0f
    float hingeX=dx-2.0f;
    glPushMatrix();
    glTranslatef(hingeX,0,dz-0.15f);
    glRotatef(-doorAngle,0,1,0);  // rotate around hinge
    glTranslatef(2.0f,0,0);       // pivot offset
    // Door body
    useTex(TEX_WOOD_LIGHT);
    mat(0.50f,0.44f,0.36f, 0.82f,0.74f,0.62f, 0.28f,0.25f,0.20f,22);
    glPushMatrix(); glTranslatef(0,4.0f,0); box(4.0f,8.0f,0.18f,2.f,4.f); glPopMatrix();
    // Door panels (2 raised panels)
    noTex();
    mat(0.45f,0.38f,0.30f, 0.72f,0.64f,0.52f, 0.22f,0.18f,0.14f,18);
    glPushMatrix(); glTranslatef(0,2.8f,0.10f); box(3.2f,2.8f,0.08f); glPopMatrix();
    glPushMatrix(); glTranslatef(0,6.0f,0.10f); box(3.2f,2.8f,0.08f); glPopMatrix();
    // Doorknob
    mat(0.40f,0.32f,0.08f, 0.68f,0.55f,0.14f, 0.95f,0.88f,0.52f,120);
    glPushMatrix(); glTranslatef(1.6f,4.2f,0.18f); sphere(0.16f,12,8); glPopMatrix();
    glPushMatrix(); glTranslatef(1.6f,4.2f,-0.18f); sphere(0.16f,12,8); glPopMatrix();
    // Keyhole
    mat(0.10f,0.08f,0.06f, 0.18f,0.14f,0.10f, 0.20f,0.16f,0.12f,15);
    glPushMatrix(); glTranslatef(1.6f,3.9f,0.11f); cylinder(0.04f,0.05f,8); glPopMatrix();
    glPopMatrix(); // end door rotation
    // Door stop (small rubber)
    mat(0.12f,0.10f,0.10f, 0.22f,0.18f,0.18f, 0.10f,0.08f,0.08f,5);
    glPushMatrix(); glTranslatef(dx-1.6f,0.18f,dz-0.60f); cylinder(0.06f,0.20f,8); glPopMatrix();
}

void drawCeilingLight(){
    // Shallow diffuser mounted under the ceiling so the light reads as facing downward.
    float lx = LIGHTING.x;
    float ly = RH - 0.03f;
    float lz = LIGHTING.z;

    // Ceiling mount
    noTex();
    mat(0.16f,0.16f,0.17f, 0.30f,0.30f,0.32f, 0.20f,0.20f,0.22f,28);
    glPushMatrix(); glTranslatef(lx,ly,lz); cylinder(0.58f,0.08f,20); glPopMatrix();

    // Outer trim ring
    mat(0.12f,0.12f,0.13f, 0.22f,0.22f,0.24f, 0.26f,0.26f,0.28f,40);
    glPushMatrix(); glTranslatef(lx,ly-0.10f,lz); cylinder(1.08f,0.18f,24); glPopMatrix();

    // Warm diffuser on the underside.
    float gr = lightOn ? LIGHTING.diffuserOnColor[0] : LIGHTING.diffuserOffColor[0];
    float gg = lightOn ? LIGHTING.diffuserOnColor[1] : LIGHTING.diffuserOffColor[1];
    float gb = lightOn ? LIGHTING.diffuserOnColor[2] : LIGHTING.diffuserOffColor[2];
    mat(gr * LIGHTING.diffuserAmbientScale,
        gg * LIGHTING.diffuserAmbientScale,
        gb * LIGHTING.diffuserAmbientScale,
        gr, gg, gb,
        LIGHTING.diffuserSpecularColor[0],
        LIGHTING.diffuserSpecularColor[1],
        LIGHTING.diffuserSpecularColor[2],
        LIGHTING.diffuserShininess);
    glPushMatrix(); glTranslatef(lx,ly-0.18f,lz); glScalef(1.0f,0.22f,1.0f); sphere(0.92f,20,12); glPopMatrix();
    noEmit();
}

// ════════════════════════════════════════════════════════════
//  ROOM STRUCTURE
// ════════════════════════════════════════════════════════════
void tessellatedXZ(float x0,float x1,float z0,float z1,float y,
                   float nx,float ny,float nz,int sx,int sz,float tu,float tv)
{
    for(int ix=0;ix<sx;ix++){
        float xa=x0+(x1-x0)*(float)ix/sx;
        float xb=x0+(x1-x0)*(float)(ix+1)/sx;
        for(int iz=0;iz<sz;iz++){
            float za=z0+(z1-z0)*(float)iz/sz;
            float zb=z0+(z1-z0)*(float)(iz+1)/sz;
            float u0=tu*(float)ix/sx, u1=tu*(float)(ix+1)/sx;
            float v0=tv*(float)iz/sz, v1=tv*(float)(iz+1)/sz;
            glBegin(GL_QUADS);
            glNormal3f(nx,ny,nz);
            if(ny>0.0f){
                glTexCoord2f(u0,v1); glVertex3f(xa,y,zb);
                glTexCoord2f(u1,v1); glVertex3f(xb,y,zb);
                glTexCoord2f(u1,v0); glVertex3f(xb,y,za);
                glTexCoord2f(u0,v0); glVertex3f(xa,y,za);
            }else{
                glTexCoord2f(u0,v0); glVertex3f(xa,y,za);
                glTexCoord2f(u1,v0); glVertex3f(xb,y,za);
                glTexCoord2f(u1,v1); glVertex3f(xb,y,zb);
                glTexCoord2f(u0,v1); glVertex3f(xa,y,zb);
            }
            glEnd();
        }
    }
}

void tessellatedXY(float x0,float x1,float y0,float y1,float z,
                   float nx,float ny,float nz,int sx,int sy,float tu,float tv)
{
    for(int ix=0;ix<sx;ix++){
        float xa=x0+(x1-x0)*(float)ix/sx;
        float xb=x0+(x1-x0)*(float)(ix+1)/sx;
        for(int iy=0;iy<sy;iy++){
            float ya=y0+(y1-y0)*(float)iy/sy;
            float yb=y0+(y1-y0)*(float)(iy+1)/sy;
            float u0=tu*(float)ix/sx, u1=tu*(float)(ix+1)/sx;
            float v0=tv*(float)iy/sy, v1=tv*(float)(iy+1)/sy;
            glBegin(GL_QUADS);
            glNormal3f(nx,ny,nz);
            if(nz>0.0f){
                glTexCoord2f(u0,v0); glVertex3f(xa,ya,z);
                glTexCoord2f(u1,v0); glVertex3f(xb,ya,z);
                glTexCoord2f(u1,v1); glVertex3f(xb,yb,z);
                glTexCoord2f(u0,v1); glVertex3f(xa,yb,z);
            }else{
                glTexCoord2f(u0,v0); glVertex3f(xa,ya,z);
                glTexCoord2f(u0,v1); glVertex3f(xa,yb,z);
                glTexCoord2f(u1,v1); glVertex3f(xb,yb,z);
                glTexCoord2f(u1,v0); glVertex3f(xb,ya,z);
            }
            glEnd();
        }
    }
}

void tessellatedYZ(float y0,float y1,float z0,float z1,float x,
                   float nx,float ny,float nz,int sy,int sz,float tu,float tv)
{
    for(int iz=0;iz<sz;iz++){
        float za=z0+(z1-z0)*(float)iz/sz;
        float zb=z0+(z1-z0)*(float)(iz+1)/sz;
        for(int iy=0;iy<sy;iy++){
            float ya=y0+(y1-y0)*(float)iy/sy;
            float yb=y0+(y1-y0)*(float)(iy+1)/sy;
            float u0=tu*(float)iz/sz, u1=tu*(float)(iz+1)/sz;
            float v0=tv*(float)iy/sy, v1=tv*(float)(iy+1)/sy;
            glBegin(GL_QUADS);
            glNormal3f(nx,ny,nz);
            if(nx>0.0f){
                glTexCoord2f(u0,v0); glVertex3f(x,ya,za);
                glTexCoord2f(u0,v1); glVertex3f(x,yb,za);
                glTexCoord2f(u1,v1); glVertex3f(x,yb,zb);
                glTexCoord2f(u1,v0); glVertex3f(x,ya,zb);
            }else{
                glTexCoord2f(u0,v0); glVertex3f(x,ya,za);
                glTexCoord2f(u1,v0); glVertex3f(x,ya,zb);
                glTexCoord2f(u1,v1); glVertex3f(x,yb,zb);
                glTexCoord2f(u0,v1); glVertex3f(x,yb,za);
            }
            glEnd();
        }
    }
}

void drawRoom(){
    // ── FLOOR ──
    useTex(TEX_FLOOR);
    mat(0.35f,0.22f,0.10f, 0.62f,0.40f,0.18f, 0.18f,0.13f,0.07f,22);
    tessellatedXZ(-RW,RW,-RD,RD,0.0f, 0,1,0, 20,20, 4,4);

    // ── CEILING ──
    useTex(TEX_CEILING);
    mat(0.45f,0.44f,0.42f, 0.82f,0.80f,0.78f, 0.08f,0.08f,0.07f,5);
    tessellatedXZ(-RW,RW,-RD,RD,RH, 0,-1,0, 20,20, 10,10);

    // ── WALLS ──
    // Use the same plaster texture on every wall so the room reads consistently.
    useTex(TEX_WALL);
    mat(0.82f,0.80f,0.76f, 1.0f,0.99f,0.95f, 0.16f,0.15f,0.14f,10);
    // Back wall (-Z)
    tessellatedXY(-RW,RW,0,RH,-RD, 0,0,1, 20,20, 2.4f,1.0f);
    // Front wall (+Z) keeps the door cutout but now uses the same wall texture.
    // Left panel (door left side)
    tessellatedXY(-RW,-RW+1.0f,0,RH,RD, 0,0,-1, 4,20, 0.12f,1.0f);
    // Door left section
    tessellatedXY(-RW+1.0f,-RW+5.0f,0,RH,RD, 0,0,-1, 8,20, 0.48f,1.0f);
    // Right of door
    tessellatedXY(-RW+5.0f,RW,0,RH,RD, 0,0,-1, 16,20, 1.0f,1.0f);
    // Left wall (-X)
    tessellatedYZ(0,RH,-RD,RD,-RW, 1,0,0, 20,20, 2.4f,1.0f);
    // Right wall (+X)
    tessellatedYZ(0,RH,-RD,RD,RW, -1,0,0, 20,20, 2.4f,1.0f);

    // ── SKIRTING BOARDS (white trim) ──
    noTex();
    mat(0.68f,0.66f,0.64f, 0.88f,0.86f,0.84f, 0.20f,0.20f,0.18f,18);
    float sk=0.25f, st=0.055f;
    glPushMatrix(); glTranslatef(0,sk/2,-RD+st/2); box(RW*2,sk,st); glPopMatrix();
    glPushMatrix(); glTranslatef(0,sk/2, RD-st/2); box(RW*2,sk,st); glPopMatrix();
    glPushMatrix(); glTranslatef(-RW+st/2,sk/2,0); box(st,sk,RD*2); glPopMatrix();
    glPushMatrix(); glTranslatef( RW-st/2,sk/2,0); box(st,sk,RD*2); glPopMatrix();

    // ── CEILING COVING (crown moulding) ──
    mat(0.70f,0.68f,0.66f, 0.90f,0.88f,0.86f, 0.18f,0.18f,0.16f,15);
    float cv=0.22f;
    glPushMatrix(); glTranslatef(0,RH-cv/2,-RD+cv/2); glRotatef(45,1,0,0); box(RW*2,cv,cv); glPopMatrix();
    glPushMatrix(); glTranslatef(0,RH-cv/2, RD-cv/2); glRotatef(-45,1,0,0); box(RW*2,cv,cv); glPopMatrix();
    glPushMatrix(); glTranslatef(-RW+cv/2,RH-cv/2,0); glRotatef(45,0,0,1); box(cv,cv,RD*2); glPopMatrix();
    glPushMatrix(); glTranslatef( RW-cv/2,RH-cv/2,0); glRotatef(-45,0,0,1); box(cv,cv,RD*2); glPopMatrix();

}

// ════════════════════════════════════════════════════════════
//  PAINTINGS ON WALL
// ════════════════════════════════════════════════════════════
void drawPaintings(){
    // Mona Lisa on back wall (centre-right)
    drawPainting(3.0f,7.0f,-RD+0.15f, 3.5f,5.0f, TEX_PAINTING1);

    // City view on back wall (left side)
    drawPainting(-9.0f,7.8f,-RD+0.15f, 5.5f,3.8f, TEX_PAINTING2);
}

// ════════════════════════════════════════════════════════════
//  LIGHTING SETUP  (called every frame so brightness can animate)
// ════════════════════════════════════════════════════════════
void setupLighting(){
    float ceilingPower = lightOn ? lightBrightness : 0.0f;
    float lightIntensity = ceilingPower * LIGHTING.intensityScale;
    GLfloat gA[]={
        LIGHTING.globalAmbient * ceilingPower,
        LIGHTING.globalAmbient * ceilingPower,
        LIGHTING.globalAmbient * ceilingPower,
        1.0f
    };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gA);

    // Main warm ceiling light. Keep it strong enough to light the whole room
    // without the auxiliary corner fills.
    glEnable(GL_LIGHT0);
    GLfloat lp0[]={LIGHTING.x, RH - LIGHTING.yOffsetFromCeiling, LIGHTING.z, 1.0f};
    GLfloat ld0[]={
        lightIntensity * LIGHTING.diffuseColor[0],
        lightIntensity * LIGHTING.diffuseColor[1],
        lightIntensity * LIGHTING.diffuseColor[2],
        1.0f
    };
    GLfloat ls0[]={
        lightIntensity * LIGHTING.specularColor[0],
        lightIntensity * LIGHTING.specularColor[1],
        lightIntensity * LIGHTING.specularColor[2],
        1.0f
    };
    GLfloat la0[]={
        ceilingPower * LIGHTING.localAmbientColor[0],
        ceilingPower * LIGHTING.localAmbientColor[1],
        ceilingPower * LIGHTING.localAmbientColor[2],
        1.0f
    };
    glLightfv(GL_LIGHT0,GL_POSITION,lp0);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, ld0);
    glLightfv(GL_LIGHT0,GL_SPECULAR,ls0);
    glLightfv(GL_LIGHT0,GL_AMBIENT, la0);
    glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,  LIGHTING.constantAttenuation);
    glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,    LIGHTING.linearAttenuation);
    glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION, LIGHTING.quadraticAttenuation);

    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
}
