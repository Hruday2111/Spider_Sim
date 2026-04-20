// ════════════════════════════════════════════════════════════
//  PROCEDURAL TEXTURE GENERATION
//  We create OpenGL 1D/2D textures from raw pixel data at init.
// ════════════════════════════════════════════════════════════
enum TexID {
    TEX_FLOOR=0, TEX_WALL, TEX_CEILING, TEX_WOOD_DARK,
    TEX_WOOD_LIGHT, TEX_FABRIC_BED, TEX_SPIDER_SKIN,
    TEX_PAINTING1,  // Mona Lisa style
    TEX_PAINTING2,  // Starry Night style
    TEX_RUG,
    TEX_COUNT
};
GLuint textures[TEX_COUNT];

// Noise helpers
static float noise2(int x,int y){ srand(x*374761+y*668265); return (rand()%1000)/1000.f; }
static float smoothNoise(float x,float y){
    int ix=(int)x, iy=(int)y;
    float fx=x-ix, fy=y-iy;
    float v00=noise2(ix,iy), v10=noise2(ix+1,iy);
    float v01=noise2(ix,iy+1), v11=noise2(ix+1,iy+1);
    float rx=3*fx*fx-2*fx*fx*fx, ry=3*fy*fy-2*fy*fy*fy;
    return v00*(1-rx)*(1-ry)+v10*rx*(1-ry)+v01*(1-rx)*ry+v11*rx*ry;
}

static void genFloorTex(int W,int H,unsigned char* p){
    // oak planks: horizontal stripes with grain noise
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        int plankW=W/6;
        int which=(x/plankW)%6;
        // base oak colour shifts per plank
        float br[6]={0.60f,0.58f,0.63f,0.57f,0.62f,0.59f};
        float bg[6]={0.38f,0.35f,0.40f,0.34f,0.39f,0.36f};
        float bb[6]={0.16f,0.14f,0.18f,0.13f,0.17f,0.15f};
        // grain noise
        float g=smoothNoise(x*0.12f,y*0.35f)*0.14f;
        // plank boundary dark line
        float edge=(x%plankW<2||x%plankW>plankW-3)?0.7f:1.0f;
        int r=(int)((br[which]+g)*edge*255); r=r<0?0:(r>255?255:r);
        int g2=(int)((bg[which]+g*0.5f)*edge*255); g2=g2<0?0:(g2>255?255:g2);
        int b=(int)((bb[which])*edge*255); b=b<0?0:(b>255?255:b);
        p[(y*W+x)*3+0]=r; p[(y*W+x)*3+1]=g2; p[(y*W+x)*3+2]=b;
    }
}
static void genWallTex(int W,int H,unsigned char* p){
    // warm cream/rose with subtle plaster noise
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        float n=smoothNoise(x*0.05f,y*0.05f)*0.06f;
        float r=0.88f+n, g=0.76f+n*0.6f, b=0.72f+n*0.4f;
        r=r>1?1:r; g=g>1?1:g; b=b>1?1:b;
        p[(y*W+x)*3+0]=(unsigned char)(r*255);
        p[(y*W+x)*3+1]=(unsigned char)(g*255);
        p[(y*W+x)*3+2]=(unsigned char)(b*255);
    }
}
static void genCeilingTex(int W,int H,unsigned char* p){
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        float n=smoothNoise(x*0.04f,y*0.04f)*0.04f;
        float v=0.92f+n;
        unsigned char c=(unsigned char)(v*255);
        p[(y*W+x)*3+0]=c; p[(y*W+x)*3+1]=c; p[(y*W+x)*3+2]=(unsigned char)((v*0.97f)*255);
    }
}
static void genDarkWoodTex(int W,int H,unsigned char* p){
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        float g=smoothNoise(x*0.20f,y*0.08f)*0.18f;
        float r=0.28f+g, gr=0.18f+g*0.5f, b=0.09f+g*0.2f;
        r=r>1?1:r;
        p[(y*W+x)*3+0]=(unsigned char)(r*255);
        p[(y*W+x)*3+1]=(unsigned char)(gr*255);
        p[(y*W+x)*3+2]=(unsigned char)(b*255);
    }
}
static void genLightWoodTex(int W,int H,unsigned char* p){
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        float g=smoothNoise(x*0.18f,y*0.07f)*0.12f;
        float r=0.82f+g, gr=0.68f+g*0.7f, b=0.48f+g*0.3f;
        r=r>1?1:r; gr=gr>1?1:gr; b=b>1?1:b;
        p[(y*W+x)*3+0]=(unsigned char)(r*255);
        p[(y*W+x)*3+1]=(unsigned char)(gr*255);
        p[(y*W+x)*3+2]=(unsigned char)(b*255);
    }
}
static void genFabricTex(int W,int H,unsigned char* p){
    // dusty rose bedsheet
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        float n=smoothNoise(x*0.3f,y*0.3f)*0.08f;
        float wx=(x%4<2)?0.04f:0.0f, wy=(y%4<2)?0.0f:0.04f;
        float r=0.72f+n+wx, g=0.48f+n*0.5f+wy, b=0.44f+n*0.3f;
        r=r>1?1:r; g=g>1?1:g; b=b>1?1:b;
        p[(y*W+x)*3+0]=(unsigned char)(r*255);
        p[(y*W+x)*3+1]=(unsigned char)(g*255);
        p[(y*W+x)*3+2]=(unsigned char)(b*255);
    }
}
static void genSpiderSkinTex(int W,int H,unsigned char* p){
    // dark brown chitinous look with iridescent highlight
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        float n=smoothNoise(x*0.4f,y*0.4f)*0.15f;
        float hi=smoothNoise(x*1.2f,y*1.2f)*0.10f;
        float r=0.10f+n+hi*0.3f;
        float g=0.07f+n*0.6f+hi*0.1f;
        float b=0.05f+n*0.3f+hi*0.5f;
        r=r>1?1:r; g=g>1?1:g; b=b>1?1:b;
        p[(y*W+x)*3+0]=(unsigned char)(r*255);
        p[(y*W+x)*3+1]=(unsigned char)(g*255);
        p[(y*W+x)*3+2]=(unsigned char)(b*255);
    }
}

// Procedural Mona Lisa (impressionistic)
static void genMonaLisa(int W,int H,unsigned char* p){
    // dark background, central figure silhouette, golden skin tones
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        float fx=(float)x/W, fy=(float)y/H;
        // landscape background (greenish dark)
        float r=0.20f,g=0.22f,b=0.15f;
        // sky band at top
        if(fy<0.35f){ r=0.28f+fx*0.08f; g=0.32f+fy*0.12f; b=0.38f+fy*0.10f; }
        // figure area centre
        float cdx=fx-0.5f, cdy=fy-0.55f;
        float dist=sqrtf(cdx*cdx+cdy*cdy*0.6f);
        if(dist<0.32f){
            // golden skin
            float t=1.0f-dist/0.32f;
            r=r*(1-t)+(0.75f+smoothNoise(x*0.3f,y*0.3f)*0.1f)*t;
            g=g*(1-t)+(0.60f+smoothNoise(x*0.3f,y*0.3f)*0.08f)*t;
            b=b*(1-t)+(0.40f+smoothNoise(x*0.3f,y*0.3f)*0.05f)*t;
        }
        // hair dark
        float hdx=fx-0.5f, hdy=fy-0.28f;
        if(hdx*hdx*1.8f+hdy*hdy<0.028f){
            r*=0.3f; g*=0.25f; b*=0.18f;
        }
        // dark robe at bottom
        if(fy>0.72f&&dist<0.38f){ r*=0.35f; g*=0.28f; b*=0.22f; }
        // noise
        float n=smoothNoise(x*0.15f,y*0.15f)*0.05f;
        r+=n; g+=n*0.7f; b+=n*0.4f;
        r=r>1?1:r; g=g>1?1:g; b=b>1?1:b;
        p[(y*W+x)*3+0]=(unsigned char)(r*255);
        p[(y*W+x)*3+1]=(unsigned char)(g*255);
        p[(y*W+x)*3+2]=(unsigned char)(b*255);
    }
}
// Procedural Starry Night
static void genStarryNight(int W,int H,unsigned char* p){
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        float fx=(float)x/W, fy=(float)y/H;
        // dark blue sky with swirls
        float swirl=sinf(fx*8.0f+fy*6.0f+smoothNoise(x*0.2f,y*0.2f)*4.0f)*0.5f+0.5f;
        float r=0.04f+swirl*0.08f+smoothNoise(x*0.1f,y*0.1f)*0.06f;
        float g=0.08f+swirl*0.15f;
        float b=0.28f+swirl*0.35f;
        // village silhouette at bottom
        if(fy>0.70f){
            float bldg=sinf(fx*18.0f)*0.05f+0.75f;
            if(fy>bldg){ r=0.06f+smoothNoise(x*0.4f,y*0.4f)*0.04f; g=0.06f; b=0.07f; }
        }
        // church spire
        if(fx>0.12f&&fx<0.18f&&fy>(0.5f+(fx-0.15f)*3.0f)&&fy<0.78f){ r=0.05f;g=0.05f;b=0.06f; }
        // stars (bright dots)
        float sn=noise2(x/4,y/4);
        if(sn>0.94f&&fy<0.65f){ float st=(sn-0.94f)/0.06f; r+=st*0.9f;g+=st*0.9f;b+=st*0.5f; }
        // moon (upper right)
        float mdx=(fx-0.78f), mdy=(fy-0.18f);
        if(mdx*mdx+mdy*mdy<0.006f){ r=0.98f;g=0.96f;b=0.70f; }
        r=r>1?1:r; g=g>1?1:g; b=b>1?1:b;
        p[(y*W+x)*3+0]=(unsigned char)(r*255);
        p[(y*W+x)*3+1]=(unsigned char)(g*255);
        p[(y*W+x)*3+2]=(unsigned char)(b*255);
    }
}
static void genRugTex(int W,int H,unsigned char* p){
    // Persian rug pattern with medallion
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        float fx=(float)x/W-0.5f, fy=(float)y/H-0.5f;
        // base burgundy
        float r=0.48f,g=0.12f,b=0.12f;
        // border pattern
        float bx=fabsf(fx), by=fabsf(fy);
        if(bx>0.42f||by>0.42f){ r=0.18f;g=0.12f;b=0.06f; }
        if((bx>0.38f&&bx<0.42f)||(by>0.38f&&by<0.42f)){ r=0.72f;g=0.62f;b=0.18f; }
        // medallion
        float d=sqrtf(fx*fx+fy*fy);
        if(d<0.12f){ r=0.70f;g=0.18f;b=0.10f; }
        if(d>0.09f&&d<0.12f){ r=0.72f;g=0.60f;b=0.15f; }
        // geometric diamonds
        float diam=fabsf(fx)+fabsf(fy);
        if(diam>0.25f&&diam<0.27f){ r=0.72f;g=0.58f;b=0.14f; }
        float n=smoothNoise(x*0.3f,y*0.3f)*0.05f;
        r+=n;g+=n*0.5f;b+=n*0.3f;
        r=r>1?1:r;g=g>1?1:g;b=b>1?1:b;
        p[(y*W+x)*3+0]=(unsigned char)(r*255);
        p[(y*W+x)*3+1]=(unsigned char)(g*255);
        p[(y*W+x)*3+2]=(unsigned char)(b*255);
    }
}

void buildTexture(GLuint id,int W,int H,void(*gen)(int,int,unsigned char*)){
    unsigned char* buf=new unsigned char[W*H*3];
    gen(W,H,buf);
    glBindTexture(GL_TEXTURE_2D,id);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,W,H,0,GL_RGB,GL_UNSIGNED_BYTE,buf);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,W,H,GL_RGB,GL_UNSIGNED_BYTE,buf);
    delete[] buf;
}

void initTextures(){
    glGenTextures(TEX_COUNT,textures);
    buildTexture(textures[TEX_FLOOR],    256,256, genFloorTex);
    buildTexture(textures[TEX_WALL],     256,256, genWallTex);
    buildTexture(textures[TEX_CEILING],  256,256, genCeilingTex);
    buildTexture(textures[TEX_WOOD_DARK],128,128, genDarkWoodTex);
    buildTexture(textures[TEX_WOOD_LIGHT],128,128,genLightWoodTex);
    buildTexture(textures[TEX_FABRIC_BED],128,128,genFabricTex);
    buildTexture(textures[TEX_SPIDER_SKIN],64,64, genSpiderSkinTex);
    buildTexture(textures[TEX_PAINTING1],128,192, genMonaLisa);
    buildTexture(textures[TEX_PAINTING2],192,128, genStarryNight);
    buildTexture(textures[TEX_RUG],      256,256, genRugTex);
}

void useTex(int id){ glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,textures[id]); }
void noTex(){ glDisable(GL_TEXTURE_2D); }
