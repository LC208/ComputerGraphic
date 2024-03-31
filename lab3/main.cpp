#include <windows.h>
#include <gl/gl.h>
#include <iostream>
#include "menu.h"
#include "element.h"
#include <vector>
#include <algorithm>
#include "renderctl.h"
#include "texture.h"
#include <chrono>
#include <ctime>
#include "level.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void ResizeElemenst(UINT newWidth, UINT newHeight);
const int width = 1000;
const int height = 1000;
Menu mainMenu{width,height};
Menu extraMenu{width,height};
RenderCtl* ctl = &mainMenu;
int textures = 1;
auto appTime = std::chrono::system_clock::now();
std::chrono::duration<float> elapsed_seconds;
Entity* hero;
Level test{};
RECT rct;


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
                        WS_VISIBLE| WS_SYSMENU | WS_MINIMIZEBOX /*WS_OVERLAPPEDWINDOW*/,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
        width, height, NULL, NULL, hInstance, NULL);



    ShowWindow(hwnd, nCmdShow);
    EnableOpenGL(hwnd, &hDC, &hRC);
    GetClientRect(hwnd, &rct);
    ctl->glInit(rct);
    const float btnWidth = 200;
    const float btnHeight = 100;
    hero = new Entity(Texture{std::string("spritelist.png"),textures++},10,10,2);
    test.addEntity(hero);
    mainMenu.addStartX((width - btnWidth)/2);
    mainMenu.createText("Program",btnWidth,btnHeight,nullptr, ElementType::Nonactive);
    mainMenu.createButton(btnWidth,btnHeight,"Start",[](){ctl = &test; test.glInit(rct);});
    mainMenu.createButton(btnWidth,btnHeight,"Test",[](){std::cout<<"test"<<std::endl;});
    mainMenu.createButton(btnWidth,btnHeight,"Exit",[](){PostQuitMessage(0);});
    extraMenu.addStartX((width - btnWidth)/2 - btnWidth);
    extraMenu.createImage(Texture{std::string("spritelist.png"),textures++},640,240,nullptr,ElementType::Nonactive);
    extraMenu.addStartX(btnWidth);
    extraMenu.createButton(btnWidth,btnHeight,"Back",[](){std::swap(mainMenu,extraMenu);});
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
            auto newTime = std::chrono::system_clock::now();
            elapsed_seconds = newTime - appTime;
            appTime = newTime;
            ctl->update(elapsed_seconds.count());
            ctl->renderAll();
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
                mainMenu.handleClick(hwnd);
            }
            return 0;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_DESTROY:
            return 0;
        case WM_KEYDOWN:
            {
                switch(wParam)
                {
                case VK_LEFT:
                    {
                        hero->isWalking = true;
                        hero->setState(-2);
                    }
                    return 0;
                case VK_RIGHT:
                    {
                        hero->isWalking = true;
                        hero->setState(2);
                    }
                    return 0;
                case VK_UP:
                    if(!hero->inAir && !hero->isWalking)
                    {
                        hero->setState(3*std::abs(hero->getState())/hero->getState());
                    }
                    return 0;
                }
            }
            return 0;
        case WM_KEYUP:
            {
                if(wParam == VK_LEFT || wParam == VK_RIGHT)
                {
                    hero->setState(std::abs(hero->getState())/hero->getState());
                    hero->isWalking = false;
                }
                if(wParam == VK_UP)
                {
                    if(!hero->inAir)
                    {
                        hero->isJumping = true;
                    }
                }
            }
            return 0;
        case WM_SIZE:
            {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                ctl->reSize(width,height);
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
