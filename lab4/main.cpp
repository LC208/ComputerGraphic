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
#include "collisionmanager.h"

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
Level test{};
RECT rct;
int Entity::all_entities;
std::string levelMap[10] =
{
    "BBBBBBBBBB",
    "BNNNNNNNNB",
    "BNNNNNNNNB",
    "BNNNNNNNNB",
    "BNNNNNNNNB",
    "BNNNNNNNNB",
    "BNNNNNNNNB",
    "BNNNNNNNNB",
    "BSNNNNNNNB",
    "FFFFFFFFFF",
};

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
    Entity::all_entities = 0;
    const float btnWidth = 200;
    const float btnHeight = 100;

   // std::map<EnttityAnimationState, std::array<int,3>> actionToLineAndTiles;
    //actionToLineAndTiles[EnttityAnimationState::STAYING] = {0, 0, 1};
   // actionToLineAndTiles[EnttityAnimationState::WALKING] = {0, 8, 1};
    //actionToLineAndTiles[EnttityAnimationState::JUMPING] = {1, 6, 0};
    /*Texture charac{std::string("spritelist.png")};
    MoveabelEntity* hero = new MoveabelEntity(TileSet{charac,80,80,{0,0}},20,100,80,80,1,actionToLineAndTiles);
    Texture testtex{std::string("jungle_pack_05.png")};
    Entity* floor = new Entity(TileSet{testtex,64*20,64,{128,128}},0,0);
    test.addEntity(hero);
    test.addEntity(floor);
    test.exportLevel("F:\lab3\level.dat");*/
    //Entity* floor = new Entity(TileSet{testtex,64*20,64,{128,128}},0,0);
    //addEntity(hero);

    mainMenu.addStartX((width - btnWidth)/2);
    mainMenu.createText("Program",btnWidth,btnHeight,nullptr, ElementType::Nonactive);
    mainMenu.createButton(btnWidth,btnHeight,"Start",[]()
    {
        ctl = &test;
        test.loadLevel("level.json");
        test.glInit(rct);
    });
    mainMenu.createButton(btnWidth,btnHeight,"Test",[](){std::cout<<"test"<<std::endl;});
    mainMenu.createButton(btnWidth,btnHeight,"Exit",[](){PostQuitMessage(0);});
    extraMenu.addStartX((width - btnWidth)/2 - btnWidth);
    //extraMenu.createImage(Texture{std::string("Tiles.png")},352,384,nullptr,ElementType::Nonactive);
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

            auto newTime = std::chrono::system_clock::now();
            elapsed_seconds = (newTime - appTime);
            appTime = newTime;
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ctl->renderAll();
            ctl->update(elapsed_seconds.count());
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
