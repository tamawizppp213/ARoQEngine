#include <Windows.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "MainGame/Include/Application.hpp"
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCommandLine, int nShowCommand)
#endif
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    /********************************************
    **         Initialize
    *********************************************/
    Application::Instance().StartUp();
    /********************************************
    **         MainLoop
    *********************************************/
    Application::Instance().Run();
    /********************************************
    **         Termination Process
    *********************************************/
    Application::Instance().ShutDown();
    /********************************************
    **         Check MemoryLeaks
    *********************************************/
    _CrtDumpMemoryLeaks();
    exit(EXIT_SUCCESS);
   
}

