#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
#define  RADDEG  57.29577951f
#define PI          3.14159265358979323
float XUP[3] = {1,0,0}, XUN[3] = {-1, 0, 0},
      YUP[3] = {0,1,0}, YUN[3] = { 0,-1, 0},
      ZUP[3] = {0,0,1}, ZUN[3] = { 0, 0,-1},
      ORG[3] = {0,0,0};
 
GLfloat viewangle = 0, tippangle = 0, traj[120][3];
 
GLfloat d[3] = {0.1, 0.1, 0.1};
 
GLfloat  xAngle = 0.0, yAngle = 0.0, zAngle = 0.0;
//  Use arrow keys to rotate entire scene !!!
 
GLuint tex;
GLuint texture[2];
struct Image {

  unsigned long sizeX;

  unsigned long sizeY;

  char *data;

};
typedef struct Image Image;
#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];
void makeCheckImage(void){        //draw texture on the ground-plane
  int i, j, c;
  for (i = 0; i < checkImageWidth; i++) {
    for (j = 0; j < checkImageHeight; j++) {
      if(((i%21)<10) xor ((j%21)<10))
      {
        checkImage[i][j][0] = (GLubyte) 255;
        checkImage[i][j][1] = (GLubyte) 255 ;
        checkImage[i][j][2] = (GLubyte) 120;
      }
      else
      {
        checkImage[i][j][0] = (GLubyte) 0;
        checkImage[i][j][1] = (GLubyte) 120;
        checkImage[i][j][2] = (GLubyte) 0;
      }
    }
  }
}
int ImageLoad(char *filename, Image *image)  //loading iamge for texture
{
  FILE *file;
    unsigned long size; // size of the image in bytes.
    unsigned long i; // standard counter.
    unsigned short int planes; // number of planes in image (must be 1)
    unsigned short int bpp; // number of bits per pixel (must be 24)
    char temp; // temporary color storage for bgr-rgb conversion.
    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL){
      printf("File Not Found : %s\n",filename);
      return 0;
    }
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);
    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
      printf("Error reading width from %s.\n", filename);
      return 0;
    }
    //printf("Width of %s: %lu\n", filename, image->sizeX);
    // read the height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
      printf("Error reading height from %s.\n", filename);
      return 0;
    }
    //printf("Height of %s: %lu\n", filename, image->sizeY);
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;
    // read the plane
    if ((fread(&planes, 2, 1, file)) != 1) {
      printf("Error reading planes from %s.\n", filename);
      return 0;
    }
    if (planes != 1) {
      printf("Planes from %s is not 1: %u\n", filename, planes);
      return 0;
    }
    // read the bitsperpixel
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
      printf("Error reading bpp from %s.\n", filename);
      return 0;
    }
    if (bpp != 24) {
      printf("Bpp from %s is not 24: %u\n", filename, bpp);
      return 0;
    }
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);
    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
      printf("Error allocating memory for color-corrected image data");
      return 0;
    }
    if ((i = fread(image->data, size, 1, file)) != 1) {
      printf("Error reading image data from %s.\n", filename);
      return 0;
    }
    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
      temp = image->data[i];
      image->data[i] = image->data[i+2];
      image->data[i+2] = temp;
    }
    // we're done.
    return 1;
  }
  Image * loadTexture(){
    Image *image1;
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
      printf("Error allocating space for image");
      exit(0);
    }
    if (!ImageLoad("d.bmp", image1)) {
     exit(1);
   }
   return image1;
 }
 void initGL() {                  //initialinzing graphics parameters
   glClearColor(0.19128f, 0.1571f ,0.8235f, 1.0f);
   glClearDepth(1.0f);   
   Image *image1 = loadTexture();
   if(image1 == NULL){
    printf("Image was not returned from loadTexture\n");
    exit(0);
  }
  makeCheckImage();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Create Texture
  glGenTextures(2, texture);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,
      GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth,
      checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,&checkImage[0][0][0]);
    // glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
  }
int n;
int flag;
struct point{
  int px,py;
} pts[1005];
struct color{
  int r,g,b;
} boundary;
vector<float> list_discs[105]; 
vector<pair<pair<float,float>,float> >  disc_info;
vector<pair<pair<float,float>,float> > disc_color;
int cntr = 0;
float tox[105],toy[105]; //Shows the x and y coordinates of top 
vector<pair<int,int> > moves;
void dfs(int n,int from,int to,int aux) {
  if(n==1) {
    moves.push_back(make_pair(from,to));
    return ;
  }
  dfs(n-1,from,aux,to);
  moves.push_back(make_pair(from,to));
  dfs(n-1,aux,to,from);
}
void redraw (void);
int moving;

int kailas=0;
   // Light source position
GLfloat qaLightPosition[]    = {0, -5, 9.5, 1};// Positional Light
GLfloat qaLightDirection[]    = {1, 1, 1, 0};// Directional Light

void changelightposition (int key, int x, int y)
{
    switch (key) {
 
       case GLUT_KEY_LEFT :  qaLightPosition[0] -= 0.5;  break;
       case GLUT_KEY_RIGHT:  qaLightPosition[0] += 0.5;  break;
       case GLUT_KEY_UP   :  qaLightPosition[1] -= 0.5;  break;
       case GLUT_KEY_DOWN :  qaLightPosition[1] += 0.5;  break;
 
       default: printf ("   Special key %c == %d\n", key, key);
    }
 
    glutPostRedisplay();
}


void changelightdirection (int key, int x, int y)
{
    switch (key) {
 
       case GLUT_KEY_LEFT :  qaLightDirection[0] -= 5;  break;
       case GLUT_KEY_RIGHT:  qaLightDirection[0] += 5;  break;
       case GLUT_KEY_UP   :  qaLightDirection[1] -= 5;  break;
       case GLUT_KEY_DOWN :  qaLightDirection[1] += 5;  break;
 
       default: printf ("   Special key %c == %d\n", key, key);
    }
 
    glutPostRedisplay();
}

void Special_Keys (int key, int x, int y)
{
  if(kailas==1)changelightposition(key,x,y);
    else if(kailas==2)changelightdirection(key,x,y);
    else {
    switch (key) {
       case GLUT_KEY_LEFT :  viewangle -= 5;  break;
       case GLUT_KEY_RIGHT:  viewangle += 5;  break;
       case GLUT_KEY_UP   :  tippangle -= 5;  break;
       case GLUT_KEY_DOWN :  tippangle += 5;  break;
 
       default: printf ("   Special key %c == %d\n", key, key);
    }
  }
 
    glutPostRedisplay();
}
 
GLdouble radius=1.5;
GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0}; //White Color
GLfloat qaRed[] = {1.0, 0.0, 0.0, 1.0}; //Red Color
GLfloat tmp_col[] = {0.0,0.0,0.0,1.0}; 
    // Set lighting intensity and color
GLfloat qaAmbientLight[]    = {0.1, 0.1, 0.1, 1.0};
GLfloat qaDiffuseLight[]    = {1, 1, 1, 1.0};
  
void initLighting()
{
    // Add smooth
    glShadeModel(GL_SMOOTH);
    // Enable lighting
     glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

     // Set lighting intensity and color
       // glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    ////////////////////////////////////////////////
    //glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2f );
     // glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION , 0.5f );
    //glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION , 0.009f );
}
pair<float,float> rot_mk;
pair<float,float> cencen;
float disc_num_rot;
bool mkkk;
void anim_disc(int disc_num,int st,int en) {
  float stp = 0.5;
  float tot_steps = 0;
  float cx = tox[st], cy = toy[st];
  float ex = tox[en], ey = toy[en];
  cout<<cx<<" "<<cy<<" "<<ex<<" "<<ey<<endl;
  for(;cy<7.25;cy+=stp) {
    mkkk++;
    cout<<"Debug 1 "<<tox[st]<<" "<<cy<<endl;
    disc_info[disc_num].first.first = tox[st];
    disc_info[disc_num].first.second = cy;
    redraw();
    tot_steps++;
    // sleep(1);
  }
  float cenx = (tox[st] + tox[en])*0.5;
  float ceny = cy;
  float rad = fabs(tox[st]-tox[en]) * 0.5;
  if(tox[st] < tox[en]) {
    float theta = 0;
    for(;theta<=170;theta+=(72/tot_steps)) {
      mkkk++;
      cout<<"Debug 2"<<" "<<cx<<" "<<cy<<endl;
      cx = cenx - rad * cos((theta * PI) / 180);
      cy = ceny + rad * sin((theta * PI) / 180);
      disc_info[disc_num].first.first = cx;
      disc_info[disc_num].first.second = cy;
      disc_num_rot = disc_num;
      rot_mk.first = theta;
      rot_mk.second = -1; 
      cencen.first = cenx;
      cencen.second = ceny;
      redraw();
    // sleep(1);
    }
  }
  else {
    float theta = 0;
    for(;theta<=170;theta+=(72/tot_steps)) {
      mkkk++;
      cout<<"Debug 2"<<" "<<cx<<" "<<cy<<endl;
      cx = cenx + rad * cos((theta * PI) / 180);
      cy = ceny + rad * sin((theta * PI) / 180);
      disc_info[disc_num].first.first = cx;
      disc_info[disc_num].first.second = cy;
      disc_num_rot = disc_num;
      rot_mk.first = -theta;
      rot_mk.second = 1;
      cencen.first = cenx;
      cencen.second = ceny;
      redraw();
    // sleep(1);
    }
  }
  disc_num_rot = 0;
  for(;cy>=ey;cy-=stp) {
    mkkk = 3 - mkkk;
    cout<<"Debug 3"<<" "<<ex<<" "<<cy<<endl;
    disc_info[disc_num].first.first = ex;
    disc_info[disc_num].first.second = cy;
    redraw();
    // sleep(1);
  }
  disc_info[disc_num].first.first = ex;
  disc_info[disc_num].first.second = ey;
  redraw();
  cout<<endl<<endl<<endl;
}
void Keyboard (unsigned char key, int x, int y)
{
    switch (key) {
       case 'j' : d[0] += 0.1;  break;
       case 'k' : d[1] += 0.1;  break;
       case 'l' : d[2] += 0.1;  break;

       case 'b' : d[0] -= 0.1;  break;
       case 'n' : d[1] -= 0.1;  break;
       case 'm' : d[2] -= 0.1;  break;

       case 'e' : kailas=(kailas==1)?0:1;  break;
       case 'f' : kailas=(kailas==2)?0:2;  break;

 
       case 'a' : {
        if(moving) break;
        moving = 1;
        int tmp = (int)moves.size();
        if(cntr != tmp) {
          int from = moves[cntr].first, to = moves[cntr].second;

          int num_of_disc = list_discs[from].back();
          list_discs[to].push_back(list_discs[from].back());
          list_discs[from].pop_back();
          // cout<<"Here:"<<from<<" "<<to<<endl;

          anim_disc(num_of_disc,from,to);
          disc_info[num_of_disc].first.first = tox[to];
          disc_info[num_of_disc].first.second = toy[to];

          toy[from] -= 0.5;
          toy[to] += 0.5;
          cntr++;
        }
        moving = 0;
        break;
       }
       case 'x' : xAngle += 5;  break;
       case 'y' : yAngle += 5;  break;
       case 'z' : zAngle += 5;  break;
 
       default: printf ("   Keyboard %c == %d\n", key, key);
    }
 
    glutPostRedisplay();
}
void Draw_face(int a,int b)
{
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glBegin(GL_POLYGON);
  glTexCoord2f(0,0);
  int gz=1;
  
  glVertex3f(-a,-b,0);
 glTexCoord2f(1,0);
  
  glVertex3f(a,-b,0);
  
  glTexCoord2f(1,1);
  
  glVertex3f(a,b,0);
  
  glTexCoord2f(0,1);
  
  glVertex3f(-a,b,gz);
  glEnd();
}
void Draw_func (void)
{
  cout<<"I have come here"<<endl;
  tmp_col[0] = 0.5, tmp_col[1] = 0.35, tmp_col[2] = 0.05;
  glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
  /*glBegin(GL_QUADS);
  // glNormal3f(0.0f, 0.0f, 1.0f);
  
  glVertex3f(100,100 ,-0.05);
  glVertex3f(-100 ,100,-0.05);
  glVertex3f(-100 ,-100 ,-0.05);
  glVertex3f(100,-100,-0.05);  
  glEnd();

  glBegin(GL_QUADS);
  // glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(100,100 ,-100 );
  glVertex3f(-100 ,100,-100);
  glVertex3f(-100 ,-100 ,-100 );
  glVertex3f(100,-100,-100);  
  glEnd();

  glBegin(GL_QUADS);
  // glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(100,100 ,-0.05);
  glVertex3f(-100 ,100,-0.05);
  glVertex3f(-100 ,100,-100);
  glVertex3f(100,100 ,-100 );  
  glEnd();

  glBegin(GL_QUADS);
  // glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-100 ,100,-0.05);
  glVertex3f(-100 ,-100 ,-0.05);
  glVertex3f(-100 ,-100 ,-100 );
  glVertex3f(-100 ,100,-100);  
  glEnd();

  glBegin(GL_QUADS);
  // glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-100 ,-100 ,-0.05);
  glVertex3f(100,-100,-0.05);
  glVertex3f(100,-100,-100);
  glVertex3f(-100 ,-100 ,-100 );  
  glEnd();

  glBegin(GL_QUADS);
  // glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(100,-100,-0.05);
  glVertex3f(100,100 ,-0.05);
  glVertex3f(100,100 ,-100 );
  glVertex3f(100,-100,-100);  
  glEnd();*/
glEnable(GL_TEXTURE_2D);
  // glTranslatef(0,0,-50);

  // glutSolidCube(100);
  // glTranslatef(0,0,50);   

  glDisable(GL_LIGHTING);
  glTranslatef(0,0,-100);
  Draw_face(50,50);
  glTranslatef(0,0,+100);
  Draw_face(50,50);


  glTranslatef(50,0,-50);
  glRotatef(90,0,1,0);
  Draw_face(50,50);

  glRotatef(-90,0,1,0);

  glTranslatef(-50,0,50);


  glTranslatef(-50,0,-50);
  glRotatef(90,0,1,0);
  Draw_face(50,50);

  glRotatef(-90,0,1,0);

  glTranslatef(50,0,50);


  glTranslatef(0,-50,-50);
  glRotatef(90,1,0,0);
  Draw_face(50,50);

  glRotatef(-90,1,0,0);

  glTranslatef(0,50,50);


  glTranslatef(0,50,-50);
  glRotatef(90,1,0,0);
  Draw_face(50,50);

  glRotatef(-90,1,0,0);

  glTranslatef(0,-50,50);

 glEnable(GL_LIGHTING);

  glDisable(GL_TEXTURE_2D);
  // glBegin(GL_POLYGON);

    GLfloat mat_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

  tmp_col[0] = 1, tmp_col[1] = 1, tmp_col[2] = 1;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_white);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_white);
  glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
  // glTranslatef(-10.0,-10.0,0);
  glutSolidCone(1,7,50,50);
  
  glTranslatef(15.0,0,0);
  glutSolidCone(1,7,50,50);
  glTranslatef(-15.0,0,0);

  glTranslatef(-15.0,0,0);
  glutSolidCone(1,7,50,50);
  glTranslatef(15.0,0,0);

  for(int i=1;i<=n;i++) {
    tmp_col[0] = disc_color[i].first.first;
    tmp_col[1] = disc_color[i].first.second;
    tmp_col[2] = disc_color[i].second;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
    glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
    if(i==disc_num_rot) {
      glTranslatef(disc_info[i].first.first, 0, disc_info[i].first.second);
      
      float vecx = disc_info[i].first.first - cencen.first;
      float vecy = disc_info[i].first.second - cencen.second;
      cout<<rot_mk.first<<" "<<rot_mk.second<<endl;
      glRotatef(rot_mk.first, rot_mk.second,0,0);
      glutSolidTorus(0.5,disc_info[i].second,50,50);
      glRotatef(-rot_mk.first, rot_mk.second,0,0);
      glTranslatef(-disc_info[i].first.first, 0, -disc_info[i].first.second);
    }
    else {
      glTranslatef(disc_info[i].first.first, 0, disc_info[i].first.second);
      glutSolidTorus(0.5,disc_info[i].second,50,50);
      glTranslatef(-disc_info[i].first.first, 0, -disc_info[i].first.second);
    }
  }
  glEnd();
}

void drawBitmapText(char *string,float x,float y,float z) {  
  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) 
  {
    // cout<<"Putting:"<<(*c)<<endl;
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }
}

void redraw (void)
{
    int v;
 
    glClear  (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable (GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLoadIdentity ();
    char str[] = "Heregesdfgfvxdv";
    drawBitmapText(str,0.5,0.5,0);
    glTranslatef (0, 0, -3) ;
    glRotatef (tippangle, 1,0,0);  // Up and down arrow keys 'tip' view.
    glRotatef (viewangle, 0,1,0);  // Right/left arrow keys 'turn' view.
 
    // glDisable (GL_LIGHTING);
    glScalef(0.5,0.5,0.5);
 
    glPushMatrix ();
       glTranslatef (d[0], d[1], d[2]);    // Move box down X axis.
       glScalef (0.2, 0.2, 0.2);
       glRotatef (zAngle, 0,0,1);
       glRotatef (yAngle, 0,1,0);
       glRotatef (xAngle, 1,0,0);
       Draw_func ();
    glPopMatrix ();
 
    glutSwapBuffers();
}

/*void mouse_handler(int button, int state, int x, int y)
{
  if (moving) return;
  int from = -1, to = -1;
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    int rod = -1;
    if(x<=550) rod = 1;
    else if(x>=1000) rod = 3;
    else rod = 2;

    if(from==-1) from = rod;
    else if(to==-1) {

      from = to = -1;
    }
  }
  glutPostRedisplay();
}
*/
float rr[] = {1,0,0,1,1,0,0.5};
float bb[] = {0,1,0,1,0,1,0.5};
float gg[] = {0,0,1,0,1,1,0.5};
int main (int argc, char **argv)
{
    // system("canberra-gtk-play -f Leaving.wav"); 
    cout<<"Enter number of discs:";
    cin>>n;
    disc_info.resize(n+5);
    disc_color.resize(n+5);
    bool re = 0;
    float rad = 1 + n * 0.5;
    for(int i=1;i<=n;i++) {
      list_discs[1].push_back(i),re^=1;
      disc_info[i] = make_pair(make_pair(-15,0.5 + 0.5*(i-1)),rad);
      disc_color[i].first.first = rr[i-1];
      disc_color[i].first.second = bb[i-1];
      disc_color[i].second = gg[i-1];
      rad -= 0.5;
    }
    tox[1] = -15, toy[1] = 0.5 + 0.5*n;
    tox[2] = 0, toy[2] = 0.5;
    tox[3] = 15, toy[3] = 0.5;
    dfs(n,1,3,2);
    glutInit               (&argc, argv);
    glutInitWindowSize     (1500, 1500);
    glutInitWindowPosition (300, 300);
    glutInitDisplayMode    (GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB | GLUT_DOUBLE);
    initLighting(); 
    // glEnable(GL_LIGHTING);
    glutCreateWindow ("Trying");
    glutDisplayFunc  (   redraw   );
    glutKeyboardFunc (  Keyboard  );
    glutSpecialFunc  (Special_Keys);
    // glutMouseFunc(mouse_handler);

    glClearColor (0, 0.423, 0.6, 1.0);
 
    glMatrixMode   (GL_PROJECTION);
    gluPerspective (60, 1.5, 1, 50);
    glMatrixMode   (GL_MODELVIEW);
    initGL();
    glutMainLoop   ();
    glDisable(GL_LIGHTING);
    return 1;
}
 