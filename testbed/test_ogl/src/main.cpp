

#include <glut.h>
#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

void init ( void )
{
     GLfloat mat_specular [ ] = { 1.0, 1.0, 1.0, 1.0 };
     GLfloat mat_shininess [ ] = { 50.0 };
     GLfloat light_position [ ] = { 1.0, 1.0, 1.0, 0.0 };

     glClearColor ( 0.0, 0.0, 0.0, 0.0 );
     glShadeModel ( GL_SMOOTH );

     glMaterialfv ( GL_FRONT, GL_SPECULAR, mat_specular);
     glMaterialfv ( GL_FRONT, GL_SHININESS, mat_shininess);
     glLightfv ( GL_LIGHT0, GL_POSITION, light_position);

     glEnable (GL_LIGHTING);
     glEnable (GL_LIGHT0);
     glEnable (GL_DEPTH_TEST);
}

/*����GLUT����������һ����*/
void display ( void )
{
     glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glutSolidSphere (1.0, 40, 50);
     glFlush ();
}


/* ����GLUT��reshape������w��h�ֱ��ǵ�ǰ���ڵĿ�͸�*/
void reshape (int w, int h)
{
     glViewport (0, 0, (GLsizei) w, (GLsizei) h);
     glMatrixMode (GL_PROJECTION);
     glLoadIdentity ( );
     if (w <= h)
         glOrtho (-1.5, 1.5, -1.5 * ( GLfloat ) h / ( GLfloat ) w, 1.5 * ( GLfloat ) h / ( GLfloat ) w, -10.0, 10.0 );
     else
         glOrtho (-1.5 * ( GLfloat ) w / ( GLfloat ) h, 1.5 * ( GLfloat ) w / ( GLfloat ) h, -1.5, 1.5, -10.0, 10.0);
     glMatrixMode ( GL_MODELVIEW );
     glLoadIdentity ( ) ;
}


/* ����Լ��̵���Ӧ���� */
void keyboard ( unsigned char key, int x, int y)
{
     /*��Esc���˳�*/
     switch (key) 
     {
         case 27:
         exit ( 0 );
         break;
     }
}

int main(int argc, char **argv)
{
	     /* GLUT������ʼ��*/
    glutInit (&argc, argv);
     /* ��ʾģʽ��ʼ�� */
     glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
     /* ���崰�ڴ�С */
     glutInitWindowSize (300, 300);
     /* ���崰��λ�� */
     glutInitWindowPosition (100, 100);
    /* ��ʾ���ڣ����ڱ���Ϊִ�к����� */
     glutCreateWindow ( "HelloWorld" );
     /* ����OpenGL��ʼ������ */
     init ( );
     /* ע��OpenGL��ͼ���� */
     glutDisplayFunc ( display );
     /* ע�ᴰ�ڴ�С�ı�ʱ����Ӧ���� */
     glutReshapeFunc ( reshape );
     /* ע�������Ӧ���� */
     glutKeyboardFunc ( keyboard );
     /* ����GLUT��Ϣѭ������ʼִ�г��� */
     glutMainLoop( );

	printf("hello world\n");
}


