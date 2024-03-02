//////////////////////////////////////////////////////////////////////////////////
//              @file   Main.cpp
///             @brief  Entry point function 後々はエンジン側に隠ぺいしますが, 当分先です.
///             @author Toide Yutaro
///             @date   2023_12_22
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CorePlatformMacros.hpp"

/*---------------------------------------------------------------
         Windowsの場合はwinmainで実行するため, windows.hを必要とします
-----------------------------------------------------------------*/
#if PLATFORM_OS_WINDOWS
#include <Windows.h>
#endif

/*---------------------------------------------------------------
         Debug時はメモリリークのチェックを行います.
-----------------------------------------------------------------*/
#ifdef _DEBUG
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
#endif

#include "MainGame/Core/Include/Application.hpp"
#include "GameUtility/Base/Include/GUCommandLine.hpp"
#include "GameUtility/Base/Include/GUParse.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_OS_WINDOWS
    #ifdef _DEBUG
        int main(const gu::int32 argumentCount, const gu::char8** argumentVector)
    #else
        int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCommandLine, _In_ int nShowCommand)
    #endif
#else
        int main(const gu::int32 argumentCount, const gu::char8** argumentVector)
#endif
/*---------------------------------------------------------------
            Main Functionの実装
-----------------------------------------------------------------*/
{   
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#if PLATFORM_OS_WINDOWS && !_DEBUG
    const gu::char8* constLine = const_cast<gu::char8*>(lpCommandLine);
    const auto argument = gu::CommandLine::BuildFromArgumentVector(1, &constLine);
#else
    const auto argument = gu::CommandLine::BuildFromArgumentVector(argumentCount, argumentVector);
#endif

    gu::CommandLine::SetUp(argument.CString());
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

