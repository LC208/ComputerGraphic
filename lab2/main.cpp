#include <windows.h>
#include <gl/gl.h>
#include <iostream>
#include "button.h"
#include <vector>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void ResizeElemenst(UINT newWidth, UINT newHeight);
std::vector<Button> buttons;
const int width = 800;
const int height = 500;
Button::ButtonLayoutFactory buttonF{width,height};

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
                          width,
                          height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);



    ShowWindow(hwnd, nCmdShow);
    EnableOpenGL(hwnd, &hDC, &hRC);
    RECT rct;
    GetClientRect(hwnd, &rct);
    int width = rct.right - rct.left;
    int height = rct.bottom - rct.top;
    buttonF.update(width,height);
    buttons.push_back(buttonF.createButton(100,100,"text",[](){std::cout<<"action"<<std::endl;}));
    buttons.push_back(buttonF.createButton(100,100,"text",[](){std::cout<<"action1"<<std::endl;}));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, rct.right, rct.bottom);
    glOrtho(0, rct.right, rct.bottom, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
            glClear(GL_COLOR_BUFFER_BIT);
            for(Button button : buttons)
            {
                button.render();
            }
            SwapBuffers(hDC);
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
        case WM_LBUTTONUP:
            {
                POINT cursorXY;
                if(GetCursorPos(&cursorXY) && ScreenToClient(hwnd,&cursorXY))
                {
                    for(Button btn : buttons)
                    {
                        if(cursorXY.x >= btn.getX() && cursorXY.x<= btn.getX() + btn.getWidth() && cursorXY.y >= btn.getY() && cursorXY.y <= btn.getY() + btn.getHeight())
                        {
                            btn();
                        }
                    }
                }
            }
            return 0;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_DESTROY:
            return 0;
        case WM_KEYDOWN:
            {
                switch (wParam)
                {
                    case VK_LBUTTON:
                        std::cout << "test";
                    return 0;
                }
            }
            return 0;
        case WM_SIZE:
            {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                buttonF.update(width, height);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glViewport(0, 0, width, height);
                glOrtho(0, width, height, 0, 0, 1);
            }
            return 0;
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
