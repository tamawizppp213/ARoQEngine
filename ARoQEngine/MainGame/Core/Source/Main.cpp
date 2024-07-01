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
#include "GameUtility/Base/Include/GUCommandLine.hpp"
#include "GameUtility/Base/Include/GUParse.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"

#include "GameUtility/File/Private/Json/Include/JsonReader.hpp"
// 
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_OS_WINDOWS
    #ifdef _DEBUG
        int main(const gu::int32 argumentCount, const char** argumentVector)
    #else
        int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCommandLine, _In_ int nShowCommand)
    #endif
#else
        int main(const gu::int32 argumentCount, const char** argumentVector)
#endif
/*---------------------------------------------------------------
            Main Function�̎���
-----------------------------------------------------------------*/
{   
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    /*---------------------------------------------------------------
                CommandLine�̎擾
    -----------------------------------------------------------------*/
#if PLATFORM_OS_WINDOWS && !_DEBUG
    const char* constLine = const_cast<char*>(lpCommandLine);
    const auto argument = gu::CommandLine::BuildFromArgumentVector(1, &constLine);
#else
    const auto argument = gu::CommandLine::BuildFromArgumentVector(argumentCount, argumentVector);
#endif

    gu::CommandLine::SetUp(argument.CString());

    /*---------------------------------------------------------------
				Json�t�@�C���̓ǂݍ���
    -----------------------------------------------------------------*/
   /* gu::file::json::JsonReader reader;
    reader.Read(SP("Resources/test.json"), false);*/

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

