#include <windows.h>
#include <gl/gl.h>
#include <iostream>
#include "libs/camera.h"
#include <cmath>

#define PI 3.14159265

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void Init_Material();

void WndResize(int x, int y){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //загрузка единичной матрицы
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glFrustum(-k*sz, k*sz, -sz, sz, sz*2, 100); //установка перспективной проэкции
}


void Make_Foundation(float * verticies, GLuint * indices, float d ,int n, float zOffset)
{
    float angle_step = 2*PI/n;
    for (int i = 0;i < n;i++)
    {
        verticies[3*i] = d*cos(angle_step*i);
        verticies[3*i+1] = d*sin(angle_step*i);
        verticies[3*i+2] = zOffset;
    }

    for (int i = 0;i < n;i++)
    {
        indices[i]=i;
    }
}

void Make_Wall_From_Found(float * verticies, GLuint * indices, float * found1, float * found2, int n)
{
    for (int i = 0;i<n;i++)
    {
        verticies[i*3] = found1[i*3];
        verticies[i*3+1] = found1[i*3+1];
        verticies[i*3+2] = found1[i*3+2];

        verticies[3*n+i*3] = found2[i*3];
        verticies[3*n+i*3+1] = found2[i*3+1];
        verticies[3*n+i*3+2] = found2[i*3+2];

        indices[4*i] = i;
        indices[4*i+1] = n+i;
        if(n+i+1 >= 2*n)
        {
            indices[4*i+2] = n;
        }
        else
        {
            indices[4*i+2] = n+i+1;
        }
        if(i+1 >= n)
        {
            indices[4*i+3] = 0;
        }
        else
        {
            indices[4*i+3] = i+1;
        }
    }
}


void Draw_Second_Type(float d1, float d2, int dist, int n, float x, float y, float z)
{
    float * verticiesD1 = new float[3*n];
    GLuint * indicesD1 = new unsigned int[n];
    Make_Foundation(verticiesD1,indicesD1,d1,n,dist);
    float * verticiesD2 = new float[3*n];
    GLuint * indicesD2 = new unsigned int[n];
    Make_Foundation(verticiesD2,indicesD2,d2,n,0);
    float * verticiesF = new float[6*n];
    GLuint * indicesF = new unsigned int[4*n];
    Make_Wall_From_Found(verticiesF,indicesF,verticiesD1,verticiesD2,n);
    glPushMatrix();
    glTranslatef(x,y,z);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, verticiesD2);
    glNormalPointer(GL_FLOAT, 0, verticiesD2);
    glDrawElements(GL_TRIANGLE_FAN, n, GL_UNSIGNED_INT, indicesD2);
    glVertexPointer(3, GL_FLOAT, 0, verticiesD1);
    glNormalPointer(GL_FLOAT, 0, verticiesD1);
    glDrawElements(GL_TRIANGLE_FAN, n, GL_UNSIGNED_INT, indicesD1);
    glVertexPointer(3, GL_FLOAT, 0, verticiesF);
    glNormalPointer(GL_FLOAT, 0, verticiesF);
    glDrawElements(GL_QUADS, 4*n, GL_UNSIGNED_INT, indicesF);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glPopMatrix();
    delete[] verticiesD1;
    delete[] indicesD1;
    delete[] verticiesD2;
    delete[] indicesD2;
}

void Draw_Cube(float x,float y,float z,float width,float height,float depth){
    GLfloat vertices[] = {
     x-width, y-depth, z-height,
     x+width, y-depth, z-height,
     x+width, y+depth, z-height,
     x-width, y+depth, z-height,
     x-width, y-depth, z+height,
     x+width, y-depth, z+height,
     x+width, y+depth, z+height,
     x-width, y+depth, z+height
    };
    GLuint indices[] = {
     0, 1, 2,
     2, 3, 0,
     1, 5, 6,
     6, 2, 1,
     7, 6, 5,
     5, 4, 7,
     4, 0, 3,
     3, 7, 4,
     4, 5, 1,
     1, 0, 4,
     3, 2, 6,
     6, 7, 3
    };
    GLfloat normals[] = {
     0.0f, 0.0f, -1.0f,
     0.0f, 0.0f, -1.0f,
     0.0f, 0.0f, -1.0f,
     0.0f, 0.0f, -1.0f,
     0.0f, 0.0f, 1.0f,
     0.0f, 0.0f, 1.0f,
     0.0f, 0.0f, 1.0f,
     0.0f, 0.0f, 1.0f,
     -1.0f, 0.0f, 0.0f,
     -1.0f, 0.0f, 0.0f,
     -1.0f, 0.0f, 0.0f,
     -1.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f,
     0.0f, -1.0f, 0.0f,
     0.0f, -1.0f, 0.0f,
     0.0f, -1.0f, 0.0f,
     0.0f, -1.0f, 0.0f,
     0.0f, 1.0f, 0.0f,
     0.0f, 1.0f, 0.0f,
     0.0f, 1.0f, 0.0f,
     0.0f, 1.0f, 0.0f
    };
    glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glPopMatrix();
}

GLfloat light_position[] = {10, 10, 5.0f, 1.0f }; //позиция источника
GLfloat light_spot_direction[] = {0, 0, -1}; // позиция цели
GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; //параметры
GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //параметры
GLfloat light_specular[] = { 0.2f, 0.2f, 0.2f, 32.0f }; //параметры


void Init_Material()
{
    glEnable(GL_COLOR_MATERIAL); //разрешения использования
    //материала
    glShadeModel(GL_SMOOTH); // сглаживает границы
    GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 0.5f };
    GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.5f };
    GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
    GLfloat material_shininess[] = { 50.0f }; //блеск материала
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
}



void MoveCamera(){
    Camera_MoveDirectional(GetKeyState('W') < 0 ? 1 : (GetKeyState('S')< 0 ? -1 : 0) ,
                           GetKeyState('D')< 0 ? 1 : (GetKeyState('A')< 0 ? -1 : 0) ,
                            0.1);
    Camer_AutoMoveByMouse(400,400,1);
}

void Rectangle(float width, float height, float x, float y, float z,float r , float g, float b)
{
    glPushMatrix();
    float vert[12]=
    {
        x,y,z,
        x+width,y,z,
        x+width,y+height,z,
        x,y+height,z
    };
    float normal_vert[]={-1,-1,3, 1,-1,3, 1,1,3, -1,1,3};
    glNormalPointer(GL_FLOAT,0,&normal_vert);
    glVertexPointer(3, GL_FLOAT, 0, vert);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor4f(r,g,b,1);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void ChessPlate(int n, float startX, float startY, float z)
{
    int plateW = 3;
    int plateH = 3;
    glEnableClientState(GL_NORMAL_ARRAY);
    for (int i = 0; i < n;i++)
    {
        for(int j = 0; j<n;j++)
        {
            if ((i+j) % 2 == 0)
            {

                Rectangle(plateW,plateH,startX + plateW*i,startY+plateH*j,z,0.1,0.1,0.1);
            }
            else
            {
                Rectangle(plateW,plateH,startX + plateW*i,startY+plateH*j,z,1,1,1);
            }
        }
    }
    glDisableClientState(GL_NORMAL_ARRAY);
}


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1000,
                          1000,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    RECT rct;
    GetClientRect(hwnd,&rct);
    WndResize(rct.right,rct.bottom);
    SetCursorPos(400,400);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    Init_Material();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            if (GetForegroundWindow()==hwnd) MoveCamera();
            glPushMatrix();
                Camera_Apply();
                glPushMatrix();
                    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_spot_direction);
                    //glTranslatef(4*sin(theta/100),4*cos(theta/100),0);
                    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
                    //Draw_Cube(light_position[0],light_position[1],light_position[2],0.1,0.1,0.1);
                glPopMatrix();
                glBegin(GL_LINES);
                    glColor3f(1.0f, 0.0f, 0.0f);
                    glVertex3f(0.0f, 0.0f, 0.0f);
                    glVertex3f(1.0f, 0.0f, 0.0f);
                    glColor3f(0.0f, 1.0f, 0.0f);
                    glVertex3f(0.0f, 0.0f, 0.0f);
                    glVertex3f(0.0f, 0.0f, 1.0f);
                    glColor3f(0.0f, 0.0f, 1.0f);
                    glVertex3f(0.0f, 0.0f, 0.0f);
                    glVertex3f(0.0f, 1.0f, 0.0f);
                glEnd();
                glPushMatrix();
                    glScalef(0.5,0.5, 1);
                    Draw_Second_Type(2,3,1,13, 10, 10, 0);
                glPopMatrix();
                ChessPlate(8,2,2,0);
                float a = 1;
                glTranslatef(10,10,0);
                for(int i = 0;i<270;i+=45)
                {
                    glColor4f(0,0,0.5,a);
                    a-=0.2;
                    Draw_Cube(4*sin(i), 4*cos(i) ,0.51 ,0.5,0.5,0.5);

                }
            glPopMatrix();

            SwapBuffers(hDC);
            theta += 1.0f;
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_SIZE:
        {
            RECT rct;
            GetClientRect(hwnd,&rct);
            WndResize(rct.right,rct.bottom);
        }
        break;
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}



void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

