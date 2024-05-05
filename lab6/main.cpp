#include <windows.h>
#include <gl/gl.h>
#include <iostream>
#include "libs/camera.h"
#include <cmath>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void Init_Material();

void WndResize(int x, int y){
    glViewport(0,0,x,y); //перестраивает размеры окна
    float k=x/(float)y; //соотношение сторон
    float sz = 0.1; //единица размера
    glLoadIdentity(); //загрузка единичной матрицы
    glFrustum(-k*sz, k*sz, -sz, sz, sz*2, 100); //установка перспективной проэкции
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
    Init_Material();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glPopMatrix();
}

    GLfloat light_position[] = { 10.0f, 10.0f, 5.0f, 1.0f }; //позиция источника
    GLfloat light_spot_direction[] = {0, 0, -1}; // позиция цели
    GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; //параметры
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //параметры
    GLfloat light_specular[] = { 0.2f, 0.2f, 0.2f, 32.0f }; //параметры
void Init_Light()
{
    glEnable(GL_LIGHTING); //общее освещения для всего пространства
    glShadeModel(GL_SMOOTH);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT1); // источник света LIGHT0
}


void Init_Material()
{
    glEnable(GL_COLOR_MATERIAL); //разрешения использования
    //материала
    glShadeModel(GL_SMOOTH); // сглаживает границы
    GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
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
    Init_Material();
    float vert[12]=
    {
        x,y,z,
        x+width,y,z,
        x+width,y+height,z,
        x,y+height,z
    };
    glVertexPointer(3, GL_FLOAT, 0, vert);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(r,g,b);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void ChessPlate(int n, float startX, float startY, float z)
{
    int plateW = 3;
    int plateH = 3;
    glEnableClientState(GL_NORMAL_ARRAY);
    float normal_vert[]={0,0,1, 0,0,1, 0,0,1, 0,0,1};
    glNormalPointer(GL_FLOAT,0,&normal_vert);
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
    glEnable(GL_DEPTH_TEST);
    bool INITEDLIGHT = false;
    Init_Light();
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
                glRotatef(theta,0,0,1);
                glLightfv(GL_LIGHT1, GL_POSITION, light_position);
                glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_spot_direction);
                Draw_Cube(10,10,5,0.1,0.1,0.1);
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
            ChessPlate(8,2,2,0);
            Draw_Cube(10, 10 ,0.5 ,0.5,0.5,0.5);
            Draw_Cube(10, 20 ,0.5 ,0.5,0.5,0.5);
            Draw_Cube(20, 10 ,0.5 ,0.5,0.5,0.5);
            Draw_Cube(20,20,0.5,0.5,0.5,0.5);
            glPushMatrix();
            glRotatef(theta, 0.0f, 0.0f, 1.0f);
            glBegin(GL_TRIANGLES);
                glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f,   1.0f);
                glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f,  -0.5f);
                glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);
            glEnd();
            glPopMatrix();
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

