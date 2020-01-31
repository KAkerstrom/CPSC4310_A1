#define PROGRAM_TITLE "CPSC 3710 A1 - Kyle Akerstrom"
#define DISPLAY_INFO "CPSC 3710 A1 - Kyle Akerstrom"

#include <stdlib.h>  // Useful for the following includes.
#include <stdio.h>    
#include <string.h>  // For spring operations.
#include <time.h>

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.

// Some global variables.
// Window IDs, window width and height.
int Window_ID;
int Window_Width = 600;
int Window_Height = 400;

// Cube position and rotation speed variables.
float X_Rot   = 0.0f;  // Initially only rotate around X-axis.
float Y_Rot   = 0.5f;  // Later on you can use control keys to change
float X_Speed = 0.05f;  // the rotation.
float Y_Speed = 0.5f;

float Z_Max = -3.0f;
float Z_Min = -99.0f;
float Z_Speed = 0.0f;
float Z_Max_Speed = 0.005f;
float Z_Off   = -5.0f;

//////////////////////////////////////////////////////////
// String rendering routine; leverages on GLUT routine. //
//////////////////////////////////////////////////////////
static void PrintString(void *font, char *str)
{
   int i,len=strlen(str);

   for(i=0;i < len; i++)
      glutBitmapCharacter(font,*str++);
}

struct float3
{
   float3()
   {
      X = 0;
      Y = 0;
      Z = 0;
   };
   float3(float x, float y, float z) : X(x), Y(y), Z(z){}
   float X;
   float Y;
   float Z;
};

struct rgba
{
   enum colors { c_black, c_white, c_red, c_green, c_blue, c_cyan, c_maroon, c_purple, c_teal, c_yellow };
   rgba()
   {
      red = 0;
      green = 0;
      blue = 0;
      alpha = 0;
   };

   rgba(colors preset, float a)
   {
      switch (preset)
      {
         case c_black:  red = 0.0; green = 0.0; blue = 0.0; break;
         case c_white:  red = 1.0; green = 1.0; blue = 1.0; break;
         case c_red:    red = 1.0; green = 0.0; blue = 0.0; break;
         case c_green:  red = 0.0; green = 0.5; blue = 0.0; break;
         case c_blue:   red = 0.0; green = 0.0; blue = 1.0; break;
         case c_yellow: red = 1.0; green = 1.0; blue = 0.0; break;
         case c_cyan:   red = 0.0; green = 1.0; blue = 1.0; break;
         case c_maroon: red = 0.5; green = 0.0; blue = 0.0; break;
         case c_purple: red = 0.5; green = 0.0; blue = 0.5; break;
         case c_teal:   red = 0.0; green = 0.5; blue = 0.5; break;
         default:       red = 0.0; green = 0.0; blue = 0.0; break;
      }
      alpha = a;
   }

   rgba(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a){}

   float red;
   float green;
   float blue;
   float alpha;
};

class Cube
{
   public:
      enum sides { top, bottom, left, right, front, back };
      Cube()
      {
         for (int i = 0; i < 6; i++)
            colors[(sides)i] = rgba(0, 0, 0, 1);
      };

      Cube(float3 _center, float _size) : center(_center), size(_size)
      {
         for (int i = 0; i < 6; i++)
            colors[(sides)i] = rgba(0, 0, 0, 1);
      };

      void create()
      {
         glTranslatef(center.X, center.Y, center.Z);

         // OK, let's start drawing our planer quads.
         glBegin(GL_QUADS); 

         // Back face
         glNormal3f( 0.0f, 0.0f, -1.0f); // facing back

         glColor4f(colors[back].red, colors[back].blue, colors[back].green, 1 - colors[back].alpha);
         glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size); // top left back
         glTexCoord2f(size, 0.0f); glVertex3f( size, -size, -size); // top right back
         glTexCoord2f(size, size); glVertex3f( size,  size, -size); // bottom right back
         glTexCoord2f(0.0f, size); glVertex3f(-size,  size, -size); // bottom left back

         // Right face
         glNormal3f( 1.0f, 0.0f, 0.0f);

         glColor4f(colors[right].red, colors[right].blue, colors[right].green, 1 - colors[right].alpha);
         glTexCoord2f(size, 0.0f); glVertex3f( size, -size, -size); // top right back
         glTexCoord2f(size, size); glVertex3f( size,  size, -size); // bottom right back
         glTexCoord2f(0.0f, size); glVertex3f( size,  size,  size); // bottom right front
         glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size,  size); // top right front

         // Front face
         glNormal3f( 0.0f, 0.0f, 1.0f);
         
         glColor4f(colors[front].red, colors[front].blue, colors[front].green, 1 - colors[front].alpha);
         glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size,  size); // top left front
         glTexCoord2f(size, 0.0f); glVertex3f( size, -size,  size); // top right front
         glTexCoord2f(size, size); glVertex3f( size,  size,  size); // bottom right front
         glTexCoord2f(0.0f, size); glVertex3f(-size,  size,  size); // bottom left front


         // Left Face
         glNormal3f(-1.0f, 0.0f, 0.0f);  
         
         glColor4f(colors[left].red, colors[left].blue, colors[left].green, 1 - colors[left].alpha);
         glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size); // top left back
         glTexCoord2f(size, 0.0f); glVertex3f(-size, -size,  size); // top left front
         glTexCoord2f(size, size); glVertex3f(-size,  size,  size); // bottom left front
         glTexCoord2f(0.0f, size); glVertex3f(-size,  size, -size); // bottom left back

         // Top Face
         glNormal3f(0.0f, -1.0f, .0f);  
         
         glColor4f(colors[top].red, colors[top].blue, colors[top].green, 1 - colors[top].alpha);
         glTexCoord2f(size, 0.0f); glVertex3f( size, -size, size); // right top front 
         glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size); // left top front
         glTexCoord2f(0.0f, size); glVertex3f(-size, -size,  -size); // left top back
         glTexCoord2f(size, size); glVertex3f( size, -size,  -size); // right top back

         // Bottom Face
         glNormal3f(0.0f, 1.0f, 0.0f);
         
         glColor4f(colors[bottom].red, colors[bottom].blue, colors[bottom].green, 1 - colors[bottom].alpha);
         glTexCoord2f(size, 0.0f); glVertex3f( size, size, size); // right top front 
         glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size); // left top front
         glTexCoord2f(0.0f, size); glVertex3f(-size, size,  -size); // left top back
         glTexCoord2f(size, size); glVertex3f( size, size,  -size); // right top back

         // All polygons have been drawn.
         glEnd();
      }

      void SetColor(sides side, rgba color) { colors[side] = color; }

      float size;
      float3 center;
      rgba colors[6];
};

/////////////////////////////////////////////////////////
// Routine which actually does the drawing             //
/////////////////////////////////////////////////////////
void CallBackRenderScene(void)
{
   char buf[80]; // For our strings.
   
   // Need to manipulate the ModelView matrix to move our model around.
   glMatrixMode(GL_MODELVIEW);

   // Reset to 0,0,0; no rotation, no scaling.
   glLoadIdentity(); 

   // Move the object back from the screen.
   glTranslatef(0.0f,0.0f,Z_Off);

   // Rotate the calculated amount.
   glRotatef(X_Rot,1.0f,0.0f,0.0f);
   glRotatef(Y_Rot,0.0f,1.0f,0.0f);

   // Clear the color and depth buffers.
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   float3 center(0.0, 0.0, 0.0);
   Cube c(center, 1.0);
   c.SetColor(Cube::sides::back,   rgba(rgba::colors::c_yellow, 0.0));
   c.SetColor(Cube::sides::front,  rgba(rgba::colors::c_red,    0.2));
   c.SetColor(Cube::sides::left,   rgba(rgba::colors::c_cyan,   0.4));
   c.SetColor(Cube::sides::right,  rgba(rgba::colors::c_maroon, 0.0));
   c.SetColor(Cube::sides::top,    rgba(rgba::colors::c_purple, 0.6));
   c.SetColor(Cube::sides::bottom, rgba(rgba::colors::c_teal,   0.4));
   c.create();

   // Move back to the origin
   glLoadIdentity();

   // We need to change the projection matrix for the text rendering.  
   glMatrixMode(GL_PROJECTION);

   // But we like our current view too; so we save it here.
   glPushMatrix();

   // Display a string
   // Now we set up a new projection for the text.
   glLoadIdentity();
   glOrtho(0,Window_Width,0,Window_Height,-1.0,1.0);
   glColor4f(0.6,1.0,0.6,1.00);
   sprintf(buf,"%s", DISPLAY_INFO); // Print the string into a buffer
   glRasterPos2i(2,2);                         // Set the coordinate
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);  // Display the string.

   
   // To ease, simply translate up.  Note we're working in screen
   // pixels in this projection.
   glTranslatef(6.0f, Window_Height - 14, 0.0f);

   // Done with this special projection matrix.  Throw it away.
   glPopMatrix();
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

   // We start with GL_DECAL mode.
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

   // All done drawing.  Let's show it.
   glutSwapBuffers();

   X_Rot += X_Speed;
   Y_Rot += Y_Speed;

   Z_Off += Z_Speed;
   if (Z_Off > Z_Max || Z_Off < Z_Min)
      Z_Off -= Z_Speed;
}

// Only used for exiting the program
void KeyPressed(unsigned char key, int x, int y)
{
   if (key == 27)
      printf("Escape key pressed. Exiting...\n");
      glutDestroyWindow(Window_ID);
      exit(1);
}

///////////////////////////////////////////////////////////////
// Callback routine executed whenever the window is resized. //
//////////////////////////////////////////////////////////////
void CallBackResizeScene(int Width, int Height)
{
   // Let's not core dump, no matter what.
   if (Height == 0)
      Height = 1;

   glViewport(0, 0, Width, Height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

   glMatrixMode(GL_MODELVIEW);

   Window_Width  = Width;
   Window_Height = Height;
}

void MouseClick(int button, int state, int x, int y)
{
   if(state == GLUT_DOWN)
      switch (button)
      {
         case GLUT_LEFT_BUTTON:
            Z_Speed = -Z_Max_Speed;
            break;
         
         case GLUT_RIGHT_BUTTON:
            Z_Speed = Z_Max_Speed;
            break;
         
         default:
            break;
      }
   else if (state == GLUT_UP)
      Z_Speed = 0;
}

////////////////////////////////////////////////////////
//   Setup your program before passing the control    //
//   to the main OpenGL event loop.                   //
////////////////////////////////////////////////////////
void MyInit(int Width, int Height) 
{
   // Color to clear color buffer to.
   glEnable(GL_BLEND);
   glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   

   // Depth to clear depth buffer to; type of test.
   glEnable(GL_DEPTH_TEST);
   glClearDepth(1.0);
   glDepthFunc(GL_LESS); 

   // Enables Smooth Color Shading; try GL_FLAT for (lack of) fun.
   glShadeModel(GL_SMOOTH);

   // Load up the correct perspective matrix; using a callback directly.
   CallBackResizeScene(Width,Height);
}

///////////////////////////////////////////////////
// main() function.                              //
//   There are general steps in making           //
//   an OpenGL application.                      //
//   Inits OpenGL.                               //
//   Calls our own init function                 //
//   then passes control onto OpenGL.            //   
///////////////////////////////////////////////////
int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(Window_Width, Window_Height);
   Window_ID = glutCreateWindow(PROGRAM_TITLE);

   glutDisplayFunc(&CallBackRenderScene);
   glutIdleFunc(&CallBackRenderScene);
   glutReshapeFunc(&CallBackResizeScene);
   glutMouseFunc(&MouseClick);
   glutKeyboardFunc(&KeyPressed); // Only for exiting the program

   MyInit(Window_Width, Window_Height);

   printf("\n%s\n\n", PROGRAM_TITLE);
   printf("CPSC 3710 Computer Graphics.\n");
   printf("Assignment 1 Programming\n");
   printf("Kyle Akerstrom - 001212544\n");

   glutMainLoop(); 
   return 1; 
}

