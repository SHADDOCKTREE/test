#include<windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include<vector>
#include<iostream>
#include <algorithm>
#include "onMidPointEllispe.h"

#define OFFSETx (xy1[0][0]+xy2[0][0])/2
#define OFFSETy (xy1[0][1]+xy2[0][1])/2
int menu=0,submenu1=0,submenu2;
using namespace std;
int iPointNum = 0;                                       //以确定点的数目
vector<vector<int> > xy1(100,vector<int>(2));
vector<vector<int> > xy2(100,vector<int>(2));
int n = 0;
int i=0;    //标记第i个点
int iMode=0; //菜单模式选择
int winWidth = 400, winHeight =300;                      //窗口大小


void Initial(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);                 //窗口背景颜色
}

void ChangeSize(int w, int h)
{
    winHeight = h;
    winWidth =w;
    glViewport(0,0,w,h);                                //指定窗口显示区域
    glMatrixMode(GL_PROJECTION);                        //设置窗口显示区域
    glLoadIdentity();                                   //调用单位矩阵, 去掉以前的投影参数设置
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);          //设置投影参数
}
void onMidpointEllispe(int a,int b)    //中点Bresenham算法画椭圆
{
    int x,y;
    float d1,d2;
    x = 0;y = b;
    d1=b*b+a*a*(-b+0.25);
    glPointSize(5);    //设置画笔尺寸

    glBegin(GL_POINTS);
    glVertex2i(OFFSETx+x,OFFSETy+y);
    glVertex2i(OFFSETx-x,OFFSETy-y);
    glVertex2i(OFFSETx-x,OFFSETy+y);
    glVertex2i(OFFSETx+x,OFFSETy-y);
    glEnd();

    while(b*b*(x+1) < a*a*(y-0.5)){
        if(d1<=0){
            d1+=b*b*(2*x+3);
            x++;
        }
        else{
            d1+=b*b*(2*x+3)+a*a*(-2*y+2);
            x++;
            y--;
        }
        glBegin(GL_POINTS);
        glVertex2i(OFFSETx+x,OFFSETy+y);
        glVertex2i(OFFSETx-x,OFFSETy-y);
        glVertex2i(OFFSETx-x,OFFSETy+y);
        glVertex2i(OFFSETx+x,OFFSETy-y);
        glEnd();
    }//while上半部分
    d2=b*b*(x+0.5)*(x+0.5)+a*a*(y-1)*(y-1)-a*a*b*b;
    while(y>0){
        if(d2<=0){
            d2+=b*b*(2*x+2)+a*a*(-2*y+3);
            x++,y--;
        }
        else{
            d2+=a*a*(-2*y+3);
            y--;
        }
        glBegin(GL_POINTS);
        glVertex2i(OFFSETx+x,OFFSETy+y);
        glVertex2i(OFFSETx-x,OFFSETy-y);
        glVertex2i(OFFSETx-x,OFFSETy+y);
        glVertex2i(OFFSETx+x,OFFSETy-y);
        glEnd();
    }
}
void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);                       //用当前背景色填充窗口
    glColor3f(1.0f, 0.0f, 0.0f);                        //指定当前绘图颜色
    if(iMode==1)
    {
        for(i=0;i<n;i++)
        {
            glBegin(GL_LINES);                              //维持
            glVertex2i(xy1[i][0],xy1[i][1]);

            glVertex2i(xy2[i][0],xy2[i][1]);

            glEnd();
        }

        if(iPointNum == 1)
        {
            glBegin(GL_LINES);
            glVertex2i(xy1[i][0],xy1[i][1]);

            glVertex2i(xy2[i][0],xy2[i][1]);

            glEnd();
        }
    }
    else if(iMode==2){
		glBegin(GL_LINE_LOOP);
		glVertex2i(xy1[0][0],xy1[0][1]);
		glVertex2i(xy2[0][0],xy1[0][1]);
		glVertex2i(xy2[0][0],xy2[0][1]);
		glVertex2i(xy1[0][0],xy2[0][1]);
		glEnd();
		glutPostRedisplay();
		onMidpointEllispe(abs(xy2[0][0]-xy1[0][0])/2,abs(xy2[0][1]-xy1[0][1])/2);
    }
    glutSwapBuffers();                                  //交换缓冲区

}

//鼠标实现
void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
    int a,b;
    if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
    {
        if(iPointNum == 0)
        {
            iPointNum = 1;
            xy1[n][0] = xMouse;
            xy1[n][1] = winHeight - yMouse;

        }
        else
        {
            xy2[n][0] = xMouse;
            int mod = glutGetModifiers();
			if (mod==GLUT_ACTIVE_CTRL)
            {
                if(winHeight-yMouse>xy1[n][1])
                    xy2[n][1] = xy1[n][1]+abs(xy2[n][0]-xy1[n][0]);
                else
                    xy2[n][1] = xy1[n][1]-abs(xy2[n][0]-xy1[n][0]);
            }
            else
                xy2[n][1] = winHeight - yMouse;
            n++;

            xy1[n][0] = xy2[n-1][0];
            xy1[n][1] = xy2[n-1][1];
            glutPostRedisplay();

        }
    }
    if(button == GLUT_MIDDLE_BUTTON && action == GLUT_DOWN)
    {
        iPointNum = 0;
        n=0;
        i=0;
        glutPostRedisplay();
    }
}

//键盘实现

void key(unsigned char key, int x, int y)
{
    int a,b;
    switch(key)
    {
        case 'p':
            if(iPointNum == 0)
            {
                iPointNum = 1;
                xy1[n][0] = x;
                xy1[n][1] = winHeight - y;                       //确定直线的第一个端点
            }
            else
            {
                xy2[n][0] = x;

                xy2[n][1] = winHeight - y;

                n++;

                xy1[n][0] = xy2[n-1][0];
                xy1[n][1] = xy2[n-1][1];
                glutPostRedisplay();
            }
            break;
        case 'c':
            {

                iPointNum = 0;
                n=0;
                i=0;
                glutPostRedisplay();
            }
            break;
        default: break;
    }
}

void processNormalKeys(unsigned char key, int x, int y)
{

}

void SpecialKeys(int key, int x, int y)
{

	if(key==GLUT_KEY_UP)
	{
		xy2[n][1]+=10;
	}
	if(key==GLUT_KEY_LEFT)
	{
		xy2[n][0]-=10;
	}
	if(key==GLUT_KEY_DOWN)
	{
		xy2[n][1]-=10;
	}
	if(key==GLUT_KEY_RIGHT)
	{
		xy2[n][0]+=10;
	}
	glutPostRedisplay();
}


void PassiveMouseMove(GLint xMouse, GLint yMouse)
{


    xy2[n][0] = xMouse;
    xy2[n][1] = winHeight - yMouse;
    glutPostRedisplay();
}

void ProcessMenu(int value)
{
    i = 0;
	iMode=value;
	//每次画完重新选择模式时都要清除上次画的数据，避免选择完直接绘制
    iPointNum = 0;
    n=0;
    glutPostRedisplay();
	glutPostRedisplay();
}
//菜单
void menufuc(void)
{
	glClearColor(1.0,1.0,1.0,1.0);
	//创建菜单
	glutCreateMenu(ProcessMenu);//菜单注册函数
	//在菜单中加入菜单项
		glutAddMenuEntry("LINE",1);//画直线
		glutAddMenuEntry("OVAL",2);//画椭圆
	glutAttachMenu(GLUT_RIGHT_BUTTON);//关联菜单和某鼠标按键
}

int main(int argc,char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);        //使用双缓存及RGB模型
    glutInitWindowSize(400,300);                       //指定窗口尺寸
    glutInitWindowPosition(100 ,100);                  //指定窗口在屏幕上的位置
    glutCreateWindow("椭圆的扫描转换算法的实现");
    glutDisplayFunc(Display);

    glutReshapeFunc(ChangeSize);                       //指定窗口再整形回调函数
    glutMouseFunc(MousePlot);                          //指定鼠标响应函数
    glutKeyboardFunc(key);                             //指定键盘响应函数
    glutSpecialFunc(SpecialKeys);                      //指定键盘移动响应函数
    glutPassiveMotionFunc(PassiveMouseMove);           //指定鼠标移动响应函数
    Initial();

    //菜单
    menufuc();

    glutMainLoop();
    return 0;
}
