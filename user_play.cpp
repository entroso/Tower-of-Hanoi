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
 

int n;
int flag;
struct point{
  int px,py;
} pts[1005];
struct color{
  int r,g,b;
} boundary;
int selected_disc;
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
        int from, to;
        cout<<"Enter the disc from which you want to move:";
        cin>>from;

        cout<<"Enter the disc from where you want to move:";
        cin>>to;

        int num_of_disc = list_discs[from].back();
        if(!list_discs[from].empty() and (list_discs[to].empty() or disc_info[list_discs[to].back()].second > disc_info[num_of_disc].second)) {
          moving = 1;
          cout<<"Here:"<<from<<" "<<to<<endl;

          anim_disc(num_of_disc,from,to);
          disc_info[num_of_disc].first.first = tox[to];
          disc_info[num_of_disc].first.second = toy[to];
          list_discs[to].push_back(list_discs[from].back());
          list_discs[from].pop_back();
          
          toy[from] -= 0.5;
          toy[to] += 0.5;
          cntr++;
          cout<<"Total number of moves done:"<<cntr<<endl;
          moving = 0;
        }
        break;
       }
       case 'x' : xAngle += 5;  break;
       case 'y' : yAngle += 5;  break;
       case 'z' : zAngle += 5;  break;
 
       default: printf ("   Keyboard %c == %d\n", key, key);
    }
 
    glutPostRedisplay();
}
 
void Draw_func (void)
{
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

  glTranslatef(0,0,-50);
  glutSolidCube(100);
  glTranslatef(0,0,50);

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
    cout<<"Displaying:"<<i<<" "<<selected_disc<<endl;
    if(i==disc_num_rot) {
      glTranslatef(disc_info[i].first.first, 0, disc_info[i].first.second);
      
      float vecx = disc_info[i].first.first - cencen.first;
      float vecy = disc_info[i].first.second - cencen.second;
      cout<<rot_mk.first<<" "<<rot_mk.second<<endl;
      glRotatef(rot_mk.first, rot_mk.second,0,0);

      if(i==selected_disc) {
        tmp_col[0] = disc_color[i].first.first * 0.1;
        tmp_col[1] = disc_color[i].first.second * 0.1;
        tmp_col[2] = disc_color[i].second * 0.1;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
        glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
      }
      else {
        tmp_col[0] = disc_color[i].first.first;
        tmp_col[1] = disc_color[i].first.second;
        tmp_col[2] = disc_color[i].second;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
        glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
      }
      glutSolidTorus(0.5,disc_info[i].second,50,50);
      glRotatef(-rot_mk.first, rot_mk.second,0,0);
      glTranslatef(-disc_info[i].first.first, 0, -disc_info[i].first.second);
    }
    else {
      glTranslatef(disc_info[i].first.first, 0, disc_info[i].first.second);
      if(i==selected_disc) {
        tmp_col[0] = disc_color[i].first.first * 0.1;
        tmp_col[1] = disc_color[i].first.second * 0.1;
        tmp_col[2] = disc_color[i].second * 0.1;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
        glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
      }
      else {
        tmp_col[0] = disc_color[i].first.first;
        tmp_col[1] = disc_color[i].first.second;
        tmp_col[2] = disc_color[i].second;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
        glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
      }
      glutSolidTorus(0.5,disc_info[i].second,50,50);
      glTranslatef(-disc_info[i].first.first, 0, -disc_info[i].first.second);
    }
  }
  glEnd();
}
int flll = 0;
void redraw (void)
{
    cout<<"I have come here"<<endl;
    int v;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_LIGHTING);
    glEnable (GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLoadIdentity();

    if((int)list_discs[3].size() == n and flll == 0) {
      cout<<"Andar aa gaya"<<endl;
      glClearColor(0.1,0.1,0.1,1);
      glClear  (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      flll = 1;
      /*glColor3f(1,1,1);
      char txt[] = "Your score : ";
      vector<int> to_num;
      int to_num_sz=0,st = strlen(txt);
      while(cntr) {
        to_num_sz++;
        to_num.push_back(cntr%10);
        cntr/=10;
      }
      reverse(to_num.begin(),to_num.end());
      for(int i=0;i<to_num_sz;i++) txt[st+i] = (char)(to_num[i]+'0');
      cout<<"Here:"<<txt<<endl;
      glRasterPos3f(10,10,10);
      for(int i=0;txt[i]!='\0';i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,txt[i]);
      glFlush(); */
      glutPostRedisplay();
    }
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
       if(flll == 0) Draw_func ();
    glPopMatrix ();
     glLoadIdentity();
     glDisable(GL_DEPTH_TEST);
     glDisable(GL_LIGHTING);
         if(flll == 0) {
          glColor3f(1,1,1);
             char txt[] = "Your score : ";
             vector<int> to_num;
             int to_num_sz=0,st = strlen(txt);
             if(cntr != 0) {
              int tmp_cntr = cntr;
              while(tmp_cntr) {
                to_num_sz++;
                to_num.push_back(tmp_cntr%10);
                tmp_cntr/=10;
              }
              reverse(to_num.begin(),to_num.end());
             }
             else {
              to_num_sz++;
              to_num.push_back(0);
             }
             for(int i=0;i<to_num_sz;i++) {
              cout<<"Attaching:"<<st<<" "<<to_num[i]<<endl;
               txt[st+i] = (char)(to_num[i]+'0');
             }
              cout<<"Here:"<<txt<<endl;

             glRasterPos3f(1,1,-2);
          for(int i=0;txt[i]!='\0';i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,txt[i]);
        }
         else {
          glColor3f(1,1,1);
          // glRasterPos3f(0,0,-2);

          char pp[] = "You won.";
          glRasterPos3f(-0.2,0.2,-2);
          for(int i=0;pp[i]!='\0';i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,pp[i]);

          char txt[] = "Your score is: ";
          vector<int> to_num;
          int to_num_sz=0,st = strlen(txt);
          if(cntr != 0) {
           int tmp_cntr = cntr;
           while(tmp_cntr) {
             to_num_sz++;
             to_num.push_back(tmp_cntr%10);
             tmp_cntr/=10;
           }
           reverse(to_num.begin(),to_num.end());
          }
          else {
           to_num_sz++;
           to_num.push_back(0);
          }
          for(int i=0;i<to_num_sz;i++) {
           cout<<"Attaching:"<<st<<" "<<to_num[i]<<endl;
            txt[st+i] = (char)(to_num[i]+'0');
          }
          txt[st+to_num_sz] = '\0';
             glRasterPos3f(-0.25,0,-2);
          for(int i=0;txt[i]!='\0';i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,txt[i]);

          char pp2[] = "Optimal score is: ";
          vector<int> to_num2;
          int to_num_sz2=0,st2 = strlen(pp2);
          int sccc = pow(2,n) - 1;
          if(sccc != 0) {
           int tmp_cntr = sccc;
           while(tmp_cntr) {
             to_num_sz2++;
             to_num2.push_back(tmp_cntr%10);
             tmp_cntr/=10;
           }
           reverse(to_num2.begin(),to_num2.end());
          }
          else {
           to_num_sz2++;
           to_num2.push_back(0);
          }
          for(int i=0;i<to_num_sz2;i++) {
           cout<<"Attaching:"<<st2<<" "<<to_num2[i]<<endl;
            pp2[st2+i] = (char)(to_num2[i]+'0');
          }
          pp2[st2+to_num_sz2] = '\0';
             glRasterPos3f(-0.25,-0.2,-2);
          for(int i=0;pp2[i]!='\0';i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,pp2[i]);

         }
         glEnable(GL_DEPTH_TEST);
         glEnable(GL_LIGHTING);
         glFlush(); 

//    glFlush(); 

    glutSwapBuffers();
}
int from,to;
void mouse_handler(int button, int state, int x, int y)
{
  if (moving) return;
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    int rod = -1;
    if(x<=550) rod = 1;
    else if(x>=1000) rod = 3;
    else rod = 2;

    cout<<x<<" "<<y<<" "<<from<<" "<<to<<" "<<rod<<endl;
    if(from==-1) {
      from = rod;
      selected_disc = list_discs[from].back();
    }
    else if(to==-1) {
        if(moving) return;
        to = rod;

        int num_of_disc = list_discs[from].back();
        if(!list_discs[from].empty() and (list_discs[to].empty() or disc_info[list_discs[to].back()].second > disc_info[num_of_disc].second)) {
          moving = 1;
          cout<<"Here:"<<from<<" "<<to<<endl;

          anim_disc(num_of_disc,from,to);
          disc_info[num_of_disc].first.first = tox[to];
          disc_info[num_of_disc].first.second = toy[to];
          list_discs[to].push_back(list_discs[from].back());
          list_discs[from].pop_back();
          
          toy[from] -= 0.5;
          toy[to] += 0.5;
          cntr++;
          cout<<"Total number of moves done:"<<cntr<<endl;
          moving = 0;
        }
      from = to = -1;
      selected_disc = 0;
    }
  }
  glutPostRedisplay();
}

float rr[] = {1,0,0,1,1,0,0.5};
float bb[] = {0,1,0,1,0,1,0.5};
float gg[] = {0,0,1,0,1,1,0.5};
int main (int argc, char **argv)
{
    system("canberra-gtk-play -f Leaving.wav");
    from = to = -1;
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
    glutMouseFunc(mouse_handler);

    glClearColor (0, 0.423, 0.6, 0.0);
 
    glMatrixMode   (GL_PROJECTION);
    gluPerspective (60, 1.5, 1, 10);
    glMatrixMode   (GL_MODELVIEW);
    glutMainLoop   ();
    glDisable(GL_LIGHTING);
    return 1;
}
 