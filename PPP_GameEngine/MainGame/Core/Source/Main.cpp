#include <Windows.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "MainGame/Core/Include/Application.hpp"
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCommandLine, _In_ int nShowCommand)
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
    exit(EXIT_SUCCESS);
   
}

