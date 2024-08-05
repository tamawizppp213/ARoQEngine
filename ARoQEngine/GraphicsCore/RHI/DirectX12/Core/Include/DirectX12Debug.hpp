//////////////////////////////////////////////////////////////////////////////////
//              Title:  DirectX12Debug.hpp
//            Content:  DirectX12 Debugger Helper
//             Author:  Toide Yutaro
//             Create:  2020_11_10
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_DEBUG_HPP
#define DIRECTX12_DEBUG_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <comdef.h>
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         HResult Exception Class
//////////////////////////////////////////////////////////////////////////////////
namespace debug
{
	inline gu::wstring AnsiToWString(const gu::string& str)
	{
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, str.CString(), -1, buffer, 512);
		return gu::wstring(buffer);
	}

	inline gu::string WStringToString(gu::wstring wString)
	{
		int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, wString.CString(), -1, (char*)NULL, 0, NULL, NULL);
		CHAR* cpMultiByte = new CHAR[iBufferSize];

		WideCharToMultiByte(CP_OEMCP, 0, wString.CString(), -1, cpMultiByte, iBufferSize, NULL, NULL);

		gu::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);
		delete[]    cpMultiByte;

		return(oRet);
	}

}


class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hresult, const gu::wstring& functionName, const gu::wstring& filename, int lineNumber)
	{
		ErrorCode    = hresult;
		FunctionName = functionName;
		Filename     = filename;
		LineNumber   = lineNumber;

		// Get the string description of the error code.
		MessageBoxA(NULL, debug::WStringToString(ToString()).CString(), "Error", MB_ICONERROR);
	}

	inline gu::wstring ToString()
	{
		// Get the string description of the error code.
		_com_error err(ErrorCode);
		gu::wstring msg = err.ErrorMessage();
		gu::wstring allErrorMessage = FunctionName + L" failed in " + Filename + L"; line " + gu::wstring::FromNumber(LineNumber) + L"; error: " + msg;

		return allErrorMessage;
	}

	HRESULT      ErrorCode = S_OK;
	gu::wstring FunctionName;
	gu::wstring Filename;
	int          LineNumber = -1;
};

#define SAFE_RELEASE(p) {if(p){(p)->Release(); p = nullptr;}}

//////////////////////////////////////////////////////////////////////////////////
//                         ThrowIfFailed
//                Always use this function when hresult is returned 
//////////////////////////////////////////////////////////////////////////////////
#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    gu::wstring wfn = debug::AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

#endif
