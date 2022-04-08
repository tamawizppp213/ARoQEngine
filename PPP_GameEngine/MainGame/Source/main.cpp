#include <Windows.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include "GameUtility/Base/Include/Memory.hpp"
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
    gm::SharedPointer<int> a = gm::MakeShared<int>();
    std::cout << a.RefCount() << std::endl;
    /********************************************
    **         MainLoop
    *********************************************/
    
    /********************************************
    **         Check MemoryLeaks
    *********************************************/
    _CrtDumpMemoryLeaks();
    /********************************************
    **         Termination Process
    *********************************************/
    exit(EXIT_SUCCESS);
   
}

