//////////////////////////////////////////////////////////////////////////////////
//              @file   Csv.cpp
///             @brief  csv loader and writer
///             @author 
///             @date   2022_04_09
//////////////////////////////////////////////////////////////////////////////////
// Copyright: (2012-2015) Ben Strasser <code@ben-strasser.net>
// License: BSD-3
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Include/Csv.hpp"
#include <cassert>
#include <cerrno>
#pragma warning(disable: 4996)
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace csv;
using namespace error;
using namespace detail;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Error
void WithFileName::SetFileName(const char* fileName)
{
	if (fileName != nullptr)
	{
		strncpy_s(this->_fileName, fileName, sizeof(this->_fileName));
		this->_fileName[sizeof(this->_fileName) - 1] = '\0';
	}
	else
	{
		this->_fileName[0] = '\0';
	}
}
void WithColumnName   ::SetColumnName   (const char* columnName)
{
	if (_columnName != nullptr)
	{
		strncpy_s(this->_columnName, columnName, MaxColumnNameLength);
		this->_columnName[MaxColumnNameLength] = '\0';
	}
	else
	{
		this->_columnName[0] = '\0';
	}
}
void WithColumnContent::SetColumnContent(const char* columnContent)
{
	if (_columnContent != nullptr)
	{
		strncpy_s(this->_columnContent, columnContent, MaxColumnContentLength);
		this->_columnContent[MaxColumnContentLength] = '\0';
	}
	else
	{
		this->_columnContent[0] = '\0';
	}
}
void error::CannotOpenFile          ::FormatErrorMessage() const
{
	if (_errorNoValue != 0)
	{
		std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer), "Can not open file \"%s\" because \"%s\".", _fileName, std::strerror(_errorNoValue));
	}
	else
	{
		std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer), "Can not open file \"%s\".", _fileName);
	}

}
void error::LineLengthLimitExceeded ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),
		"Line number %d in file \"%s\" exceeds the maximum length of 2^24-1.", _fileLine, _fileName);
}
void error::ExtraColumnInHeader     ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),
		R"(Extra column "%s" in header of file "%s".)"
		, _columnName, _fileName);
}
void error::MissingColumnInHeader   ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),
		R"(Missing column "%s" in header of file "%s".)"
		, _columnName, _fileName);
}
void error::DuplicatedColumnInHeader::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),R"(Duplicated column "%s" in header of file "%s".)", _columnName, _fileName);
}
void error::HeaderMissing           ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),"Header missing in file \"%s\".", _fileName);
}
void error::TooFewColumns           ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer), "Too few columns in line %d in file \"%s\".", _fileLine, _fileName);
}
void error::TooManyColumns          ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),
		"Too many columns in line %d in file \"%s\"."
		, _fileLine, _fileName);
}
void error::EscapedStringNotClosed  ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),
		"Escaped string was not closed in line %d in file \"%s\"."
		, _fileLine, _fileName);
}
void error::IntegerMustBePositive   ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),
		R"(The integer "%s" must be positive or 0 in column "%s" in file "%s" in line "%d".)"
		, _columnContent, _columnName, _fileName, _fileLine);
}
void error::NoDigit                 ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),
		R"(The integer "%s" contains an invalid digit in column "%s" in file "%s" in line "%d".)"
		, _columnContent, _columnName, _fileName, _fileLine);
}
void error::IntergerOverflow        ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),
		R"(The integer "%s" overflows in column "%s" in file "%s" in line "%d".)"
		, _columnContent, _columnName, _fileName, _fileLine);
}
void error::IntergerUnderflow       ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),
		R"(The integer "%s" underflows in column "%s" in file "%s" in line "%d".)"
		, _columnContent, _columnName, _fileName, _fileLine);
}
void error::InvalidSingleCharacter  ::FormatErrorMessage() const
{
	std::snprintf(ErrorMessageBuffer, sizeof(ErrorMessageBuffer),
		R"(The content "%s" of column "%s" in file "%s" in line "%d" is not a single character.)"
		, _columnContent, _columnName, _fileName, _fileLine);
}
#pragma endregion      Error
#pragma region ByteSource
/****************************************************************************
*							Read
*************************************************************************//**
*  @fn        int NonOwningStringByteSource::Read(char* buffer, int desiredByteCount)
*  @brief     Return copyByteCount
*  @param[in] char* buffer
*  @param[in] int desiredByteCount
*  @return 　　void
*****************************************************************************/
int NonOwningStringByteSource::Read(char* buffer, int desiredByteCount)
{
	int copyByteCount = desiredByteCount;
	if (_remainingByteCount < copyByteCount) { copyByteCount = static_cast<int>(_remainingByteCount); }
	std::memcpy(buffer, _string, copyByteCount);
	_remainingByteCount -= copyByteCount;
	_string             += copyByteCount;
	
	return copyByteCount;
}
#pragma endregion ByteSource
#pragma region Comment
bool EmptyLineComment::IsComment(const char* line)
{
	if (*line == '\0') { return true; }
	while (*line == ' ' || *line == '\t')
	{
		++line;
		if (*line == 0) { return true; }
	}
	return false;
}
#pragma endregion    Comment
#ifdef CSV_IO_NO_THREAD
#pragma region Synchronous Reader
#pragma endregion Synchronous Reader
#else
#pragma region Asynchronous Reader
AsynchronousReader::~AsynchronousReader()
{
	if (_byteSource != nullptr)
	{
		{
			std::unique_lock<std::mutex> guard(_lock);
			_terminationRequested = true; 
		}
		_readRequestedCondition.notify_one();
		_thread.join();
	}
}
/****************************************************************************
*							Initialize
*************************************************************************//**
*  @fn        void AsynchronousReader::Initialize(ByteSourcePtr byteSource)
*  @brief     Initialize 
*  @param[in] BytesourcePtr byteSource
*  @return 　　void
*****************************************************************************/
void AsynchronousReader::Initialize(ByteSourcePtr byteSource)
{
	std::unique_lock<std::mutex>guard(_lock);
	_byteSource           = std::move(byteSource);
	_desiredByteCount     = csv::INVALID_VALUE;
	_terminationRequested = false;
	/*-------------------------------------------------------------------
	-                  Execute Read Buffer 
	---------------------------------------------------------------------*/
	_thread = std::thread([&]
	{
		std::unique_lock<std::mutex>guard(_lock);
		try
		{
			while (true)
			{
				_readRequestedCondition.wait(guard, [&]
				{
					return _desiredByteCount != csv::INVALID_VALUE || _terminationRequested;
				});
				if (_terminationRequested) { return; }

				_readByteCount = _byteSource->Read(_buffer, _desiredByteCount);
				_desiredByteCount = csv::INVALID_VALUE;
				if (_readByteCount == 0) { break; }
				_readFinishedCondition.notify_one();
			}
		}
		catch (...)
		{
			_readError = std::current_exception();
		}
		_readFinishedCondition.notify_one();
	});
}
/****************************************************************************
*							StartRead
*************************************************************************//**
*  @fn        void AsynchronousReader::StartRead(char* buffer, int desiredByteCount)
*  @brief     Start file read thread
*  @param[in,out] char* buffer
*  @param[in]     int desiredByteCount
*  @return 　　int
*****************************************************************************/
void AsynchronousReader::StartRead(char* buffer, int desiredByteCount)
{
	std::unique_lock<std::mutex>guard(_lock);
	_buffer           = buffer;
	_desiredByteCount = desiredByteCount;
	_readByteCount    = csv::INVALID_VALUE;
	_readRequestedCondition.notify_one();
}
/****************************************************************************
*							FinishedRead
*************************************************************************//**
*  @fn        int AsynchronousReader::FinishRead()
*  @brief     Read Finished request
*  @param[in] void
*  @return 　　int
*****************************************************************************/
int AsynchronousReader::FinishRead()
{
	std::unique_lock<std::mutex> guard(_lock);
	_readFinishedCondition.wait(guard, [&]
	{
		return _readByteCount != csv::INVALID_VALUE || _readError;
	});
	if (_readError) { std::rethrow_exception(_readError); }
	else			{ return _readByteCount; }
}

#pragma endregion Asynchronous Reader
#endif
#pragma region LineReader
/****************************************************************************
*							OpenFile
*************************************************************************//**
*  @fn        FILE* OpenFile(const std::wstring& filePath)
*  @brief     Open FilePath
*  @param[in] const std::wstring& wString
*  @return 　　FILE*
*****************************************************************************/
LineReader::ByteSourcePtr LineReader::OpenFile(const std::string& fileName)
{
	FILE* filePtr = nullptr;
	fopen_s(&filePtr, fileName.c_str(), "rb");
	if (filePtr == nullptr)
	{
		int errorNo = errno;
		error::CannotOpenFile errorMessage;
		errorMessage.SetErrorNo(errorNo);
		errorMessage.SetFileName(fileName.c_str());
		throw errorMessage;
	}
	return ByteSourcePtr(new detail::OwningStdIOByteSourceBase(filePtr));
}
/****************************************************************************
*							Initialize
*************************************************************************//**
*  @fn        void LineReader::Initialize(ByteSourcePtr byteSource)
*  @brief     Initialize 
*  @param[in] ByteSourcePtr byteSource
*  @return 　　void
*****************************************************************************/
void LineReader::Initialize(ByteSourcePtr byteSource)
{
	_fileLine  = 0;
	_buffer    = std::unique_ptr<char[]>(new char[3 * BlockLength]);
	_dataBegin = 0;
	_dataEnd   = byteSource->Read(_buffer.get(), 2 * BlockLength);
	// Ignore UTF-8 BOM
	if (_dataEnd >= 3 && _buffer[0] == '\xEF' && _buffer[1] == '\xBB' && _buffer[2] == '\xBF') { _dataBegin = 3; }

	if (_dataEnd == 2 * BlockLength)
	{
		_reader.Initialize(std::move(byteSource));
		_reader.StartRead(_buffer.get() + 2 * BlockLength, BlockLength);
	}
}
/****************************************************************************
*							ProceedNextLine
*************************************************************************//**
*  @fn        char* LineReader::ProceedNextLine()
*  @brief     Initialize
*  @param[in] void
*  @return 　　char*
*****************************************************************************/
char* LineReader::ProceedNextLine()
{
	/*-------------------------------------------------------------------
	-                 DataEndCheck
	---------------------------------------------------------------------*/
	if (_dataBegin == _dataEnd) { return nullptr; }
	/*-------------------------------------------------------------------
	-                 Proceed next line
	---------------------------------------------------------------------*/
	++_fileLine;
	/*-------------------------------------------------------------------
	-                 Current data length check
	---------------------------------------------------------------------*/
	assert(_dataBegin < _dataEnd);
	assert(_dataEnd <= BlockLength * 2);

	if (_dataBegin >= BlockLength)
	{
		std::memcpy(_buffer.get(), _buffer.get() + BlockLength, BlockLength);
		_dataBegin -= BlockLength;
		_dataEnd   -= BlockLength;
		if (_reader.IsValid())
		{
			_dataEnd += _reader.FinishRead();
			std::memcpy(_buffer.get() + BlockLength, _buffer.get() + 2 * BlockLength, BlockLength);
			_reader.StartRead(_buffer.get() + 2 * BlockLength, BlockLength);
		}

	}
	/*-------------------------------------------------------------------
	-                 Line End
	---------------------------------------------------------------------*/
	int lineEnd = _dataBegin;
	while (lineEnd != _dataEnd && _buffer[lineEnd] != '\n')
	{
		++lineEnd;
	}

	if (lineEnd - _dataBegin + 1 > BlockLength)
	{
		error::LineLengthLimitExceeded err;
		err.SetFileName(_fileName.c_str());
		err.SetFileLine(_fileLine);
		throw err;
	}

	if (lineEnd != _dataEnd && _buffer[lineEnd] == '\n')
	{
		_buffer[lineEnd] = '\0';
	}
	else
	{
		// some files are missing the newline at the end of the last line
		++_dataEnd;
		_buffer[lineEnd] = '\0';
	}

	// handle windows \r\n-line breaks
	if (lineEnd != _dataBegin && _buffer[(int64_t)lineEnd - 1] == '\r') { _buffer[(int64_t)lineEnd - 1] = '\0'; }

	char* result = _buffer.get() + _dataBegin;
	_dataBegin = lineEnd + 1;
	return result;
}
#pragma endregion LineReader