//////////////////////////////////////////////////////////////////////////////////
//              @file   Main.cpp
///             @brief  Entry point function ��X�̓G���W�����ɉB�؂����܂���, ������ł�.
///             @author Toide Yutaro
///             @date   2023_12_22
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CorePlatformMacros.hpp"

/*---------------------------------------------------------------
         Windows�̏ꍇ��winmain�Ŏ��s���邽��, windows.h��K�v�Ƃ��܂�
-----------------------------------------------------------------*/
#if PLATFORM_OS_WINDOWS
#include <Windows.h>
#endif

/*---------------------------------------------------------------
         Debug���̓��������[�N�̃`�F�b�N���s���܂�.
-----------------------------------------------------------------*/
#ifdef _DEBUG
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
#endif

#include "MainGame/Core/Include/Application.hpp"
#include "GameUtility/Container/Include/GUSortedMap.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_OS_WINDOWS
    #ifdef _DEBUG
        int main()
    #else
        int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCommandLine, _In_ int nShowCommand)
    #endif
#else
        int main()
#endif
/*---------------------------------------------------------------
            Main Function�̎���
-----------------------------------------------------------------*/
{   
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

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

