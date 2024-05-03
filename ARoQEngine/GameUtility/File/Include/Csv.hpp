//////////////////////////////////////////////////////////////////////////////////
///             @file   Csv.hpp
///             @brief  Csv
///             @author Partially edit by Toide
///         Reference : https://github.com/ben-strasser/fast-cpp-csv-parser/blob/master/csv.h
///                     https://cpp-learning.com/fast-cpp-csv-parser/
///             @date   2022_04_10
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

#pragma once
#ifndef CSV_HPP
#define CSV_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             HowTo
//////////////////////////////////////////////////////////////////////////////////
// ① 列データの変数定義
// ②　CSVReaderの呼び出し.
// ③　CSVカラムと同じ名前で登録. 
// ④ CSVReader.ReadRowでデータ読み込み
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#ifndef CSV_IO_NO_THREAD
#include <mutex>
#include <thread>
#endif
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace csv
{
	constexpr int INVALID_VALUE = -1;
	

	enum class IgnoreColumn : unsigned int
	{
		IgnoreNoColumn,
		IgnoreExtraColumn,
		IgnoreMissingColumn
	};
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace csv
{
	namespace error
	{
		struct Base : std::exception
		{
			virtual void FormatErrorMessage() const = 0;
			const char* what() const noexcept override
			{
				FormatErrorMessage();
				return ErrorMessageBuffer;
			}
			mutable char ErrorMessageBuffer[512] = {0};
		};
		/*-------------------------------------------------------------------
		-               Line Reader
		---------------------------------------------------------------------*/
		const int MaxFileNameLength = 255;
		struct WithFileName
		{
		public:
			WithFileName() { std::memset(_fileName, 0, sizeof(_fileName)); }
			void SetFileName(const char* fileName);
		protected:
			char _fileName[MaxFileNameLength + 1];
		};
		struct WithFileLine
		{
		public:
			WithFileLine() { _fileLine = csv::INVALID_VALUE; }
			void SetFileLine(int fileLine) { this->_fileLine = fileLine; }
		protected:
			int _fileLine;
		};
		struct WithErrorNo
		{
		public:
			WithErrorNo() { _errorNoValue = 0; }
			void SetErrorNo(int errorValue) { this->_errorNoValue = errorValue; }
		protected:
			int _errorNoValue;
		};
		struct CannotOpenFile : public Base, public WithFileName, public WithErrorNo
		{
			void FormatErrorMessage() const override;
		};
		struct LineLengthLimitExceeded : public Base, public WithFileLine, public WithFileName
		{
			void FormatErrorMessage() const override;
		};
		/*-------------------------------------------------------------------
		-               CSV Reader
		---------------------------------------------------------------------*/
		const int MaxColumnNameLength = 63;
		const int MaxColumnContentLength = 63;
		struct WithColumnName
		{
		public:
			WithColumnName() { std::memset(_columnName, 0, (int64_t)MaxColumnNameLength + 1); }
			void SetColumnName(const char* columnName);

		protected:
			char _columnName[MaxColumnNameLength + 1] = { NULL };
		};
		struct WithColumnContent
		{
		public:
			WithColumnContent() { std::memset(_columnContent, 0, (int64_t)MaxColumnContentLength + 1); }
			void SetColumnContent(const char* columnContent);
		protected:
			char _columnContent[MaxColumnContentLength + 1];
		};
		struct ExtraColumnInHeader : public Base, public WithFileName, public WithColumnName
		{
			void FormatErrorMessage() const override;
		};
		struct MissingColumnInHeader : public Base, public WithFileName, public WithColumnName
		{
			void FormatErrorMessage() const override;
		};
		struct DuplicatedColumnInHeader : public Base, public WithFileName, WithColumnName
		{
			void FormatErrorMessage() const override;
		};
		struct HeaderMissing : public Base, public WithFileName
		{
			void FormatErrorMessage() const override;
		};
		struct TooFewColumns : public Base, public WithFileName, public WithFileLine
		{
			void FormatErrorMessage() const override;
		};
		struct TooManyColumns : public Base, public WithFileName, public WithFileLine
		{
			void FormatErrorMessage() const override;
		};
		struct EscapedStringNotClosed : public Base, public WithFileName, public WithFileLine
		{
			void FormatErrorMessage() const override;
		};
		struct IntegerMustBePositive : public Base, public WithFileName, public WithFileLine, public WithColumnName, public WithColumnContent
		{
			void FormatErrorMessage() const override;
		};
		struct NoDigit : public Base, public WithFileName, public WithFileLine, public WithColumnName, public WithColumnContent
		{
			void FormatErrorMessage() const override;
		};
		struct IntergerOverflow : public Base, public WithFileName, public WithFileLine, public WithColumnName, public WithColumnContent
		{
			void FormatErrorMessage() const override;
		};
		struct IntergerUnderflow : public Base, public WithFileName, public WithFileLine, public WithColumnName, public WithColumnContent
		{
			void FormatErrorMessage() const override;
		};
		struct InvalidSingleCharacter : public Base, public WithFileName, public WithFileLine, public WithColumnName, public WithColumnContent
		{
			void FormatErrorMessage() const override;
		};
	}
	class ByteSourceBase
	{
	public:
		virtual int Read(char* buffer, int bufferSize) = 0;
		virtual ~ByteSourceBase() {};
	};
	namespace detail
	{
		/*-------------------------------------------------------------------
		-              ByteSource
		---------------------------------------------------------------------*/
		class OwningStdIOByteSourceBase : public ByteSourceBase
		{
		public:
			explicit OwningStdIOByteSourceBase(FILE* file) : _file(file) { std::setvbuf(_file, 0, _IONBF, 0); }
			~OwningStdIOByteSourceBase() { std::fclose(_file); }
			int Read(char* buffer, int bufferSize) override { return static_cast<int>(std::fread(buffer, 1, bufferSize, _file)); }

		private:
			FILE* _file;
		};
		class NonOwningIStreamByteSource : public ByteSourceBase
		{
		public:
			explicit NonOwningIStreamByteSource(std::istream& in) : _in(in) {};
			int Read(char* buffer, int bufferSize) override
			{
				_in.read(buffer, bufferSize); return static_cast<int>(_in.gcount());
			}
			~NonOwningIStreamByteSource() {};
		private:
			std::istream& _in;
		};
		class NonOwningStringByteSource : public ByteSourceBase
		{
		public:
			NonOwningStringByteSource(const char* string, long long size) : _string(string), _remainingByteCount(size) {}
			int Read(char* buffer, int desiredByteCount);

		private:
			const char* _string;
			long long _remainingByteCount;
		};
		/*-------------------------------------------------------------------
		-              TrimChars
		---------------------------------------------------------------------*/
		template<char... TrimCharList>
		struct TrimChars
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			static void Trim(char*& stringBegin, char*& stringEnd)
			{
				while (stringBegin != stringEnd && IsTrimChar(*stringBegin, TrimCharList...)) { ++stringBegin; }
				while (stringBegin != stringEnd && IsTrimChar(*(stringEnd - 1), TrimCharList...)) { --stringEnd; }
				*stringEnd = '\0';
			}
		private:
			/****************************************************************************
			**                Private Function
			*****************************************************************************/
			constexpr static bool IsTrimChar(char) { return false; }
			template<class ...OtherTrimChars>
			constexpr static bool IsTrimChar(char c, char trimChar, OtherTrimChars... otherTrimChars)
			{
				return c == trimChar || IsTrimChar(c, otherTrimChars...);
			}
		};
		/*-------------------------------------------------------------------
		-              Comment
		---------------------------------------------------------------------*/
		struct NoComment
		{
			static bool IsComment(const char*) { return false; }
		};
		template<char... CommentStartCharList>
		struct SingleLineComment
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			static bool IsComment(const char* line) { return IsCommentStartChar(*line, CommentStartCharList...); }
		private:
			/****************************************************************************
			**                Private Function
			*****************************************************************************/
			constexpr static bool IsCommentStartChar(char) { return false; }
			template<class... OtherCommentStartChars>
			constexpr static bool IsCommentStartChar(char c, char commentStartChar, OtherCommentStartChars... otherCommentStartChars)
			{
				return c == commentStartChar || IsCommentStartChar(c, otherCommentStartChars...);
			}
	};
		struct EmptyLineComment
		{
			static bool IsComment(const char* line);
		};
		template<char... CommentStartCharList>
		struct SingleAndEmptyLineComment
		{
			static bool IsComment(const char* line)
			{
				return SingleLineComment<CommentStartCharList...>::IsComment(line) || EmptyLineComment::IsComment(line);
			}
		};
		/*-------------------------------------------------------------------
		-              QuoteEscape
		---------------------------------------------------------------------*/
		template<char Separate>
		struct NoQuoteEscape
		{
			static const char* FindNextColumnEnd(const char* columnBegin)
			{
				while (*columnBegin != Separate && *columnBegin != '\0') { ++columnBegin; }
				return columnBegin;
			}
			static void UnEscape(char*&, char*&) {};
		};
		template<char Separate, char Quote>
		struct DoubleQuoteEscape
		{
			static const char* FindNextColumnEnd(const char* columnBegin);
			static void UnEscape(char*& columnBegin, char*& columnEnd);
		};
		/*-------------------------------------------------------------------
		-              Parse policy
		---------------------------------------------------------------------*/
		template <class QuotePolicy> void ChopNextColumn(char*& line, char*& columnBegin, char*& columnEnd);
		template <class TrimPolicy, class QuotePolicy> void ParseLine(char* line, char** sortedColumn, const std::vector<int>& columnOrder);
		
		template <unsigned int ColumnCount,class TrimPolicy, class QuotePolicy>
		void ParseHeaderLine(char* line, std::vector<int>& columnOrder, const std::string* columnName, IgnoreColumn ignorePolicy);
		template<class OverflowPolicy> void Parse(char* column, char& x);
		template<class OverflowPolicy> void Parse(char* column, std::string& x) { x = column; }
		template<class OverflowPolicy> void Parse(char* column, const char*& x) { x = column; }
		template<class OverflowPolicy> void Parse(char* column, char*& x)       { x = column; }
		
		template<class OverflowPolicy, class T> void ParseUnsignedInteger(const char* column, T& x);
		template<class OverflowPolicy> void Parse(char* column, unsigned char     & x) { ParseUnsignedInteger<OverflowPolicy>(column, x); }
		template<class OverflowPolicy> void Parse(char* column, unsigned short    & x) { ParseUnsignedInteger<OverflowPolicy>(column, x); }
		template<class OverflowPolicy> void Parse(char* column, unsigned int      & x) { ParseUnsignedInteger<OverflowPolicy>(column, x); }
		template<class OverflowPolicy> void Parse(char* column, unsigned long     & x) { ParseUnsignedInteger<OverflowPolicy>(column, x); }
		template<class OverflowPolicy> void Parse(char* column, unsigned long long& x) { ParseUnsignedInteger<OverflowPolicy>(column, x); }

		template<class OverflowPolicy, class T> void ParseSignedInteger(const char* column, T& x);
		template<class OverflowPolicy> void Parse(char* column, signed char     & x) { ParseSignedInteger<OverflowPolicy>(column, x); }
		template<class OverflowPolicy> void Parse(char* column, signed short    & x) { ParseSignedInteger<OverflowPolicy>(column, x); }
		template<class OverflowPolicy> void Parse(char* column, signed int      & x) { ParseSignedInteger<OverflowPolicy>(column, x); }
		template<class OverflowPolicy> void Parse(char* column, signed long     & x) { ParseSignedInteger<OverflowPolicy>(column, x); }
		template<class OverflowPolicy> void Parse(char* column, signed long long& x) { ParseSignedInteger<OverflowPolicy>(column, x); }

		template<class T> void ParseFloat(const char* column, T& x);
		template<class OverflowPolicy> void Parse(char* column, float      & x) { ParseFloat(column, x); }
		template<class OverflowPolicy> void Parse(char* column, double     & x) { ParseFloat(column, x); }
		template<class OverflowPolicy> void Parse(char* column, long double& x) { ParseFloat(column, x); }

		template<class OverflowPolicy, class T>void Parse(char* column, T& x)
		{
			(void)column; (void)x;
			static_assert(sizeof(T) != sizeof(T), "Can not parse this type. Only buildin integrals, floats, char, char*, const char* and std::string are supported");
		}
#ifdef CSV_IO_NO_THREAD
		/****************************************************************************
		*				  			SynchronousReader
		*************************************************************************//**
		*  @class     SynchronousReader
		*  @brief     Synchronous file read
		*****************************************************************************/
		class SynchronousReader
		{
			using ByteSourcePtr = std::unique_ptr<ByteSourceBase>;
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			void Initialize(ByteSourcePtr byteSource) { _byteSource = std::move(byteSource); }
			bool IsValid() const { return _byteSource != nullptr; }
			void StartRead(char* buffer, int desiredByteCount)
			{
				_buffer = buffer;
				_desiredByteCount = desiredByteCount;
			}
			int  FinishRead() { return _byteSource->Read(_buffer, _desiredByteCount); }
			/****************************************************************************
			**                Public Property
			*****************************************************************************/

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
		private:
			/****************************************************************************
			**                Private Function
			*****************************************************************************/

			/****************************************************************************
			**                Private Property
			*****************************************************************************/
			ByteSourcePtr _byteSource;
			char* _buffer;
			int _desiredByteCount;
		};
#else
		/****************************************************************************
		*				  			AsynchronousReader
		*************************************************************************//**
		*  @class     AsynchronousReader
		*  @brief     Asynchronous csv read class
		*****************************************************************************/
		class AsynchronousReader
		{
			using ByteSourcePtr = std::unique_ptr<ByteSourceBase>;
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			void Initialize(ByteSourcePtr byteSource);
			bool IsValid() const { return _byteSource != nullptr; }
			void StartRead(char* buffer, int desiredByteCount);
			int  FinishRead();

			/****************************************************************************
			**                Public Property
			*****************************************************************************/

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			AsynchronousReader() = default;
			~AsynchronousReader();
		private:
			/****************************************************************************
			**                Private Function
			*****************************************************************************/

			/****************************************************************************
			**                Private Property
			*****************************************************************************/
			/*-------------------------------------------------------------------
			-                        Buffer
			---------------------------------------------------------------------*/
			ByteSourcePtr _byteSource = nullptr;
			char* _buffer = nullptr;
			int           _desiredByteCount = 0;
			int           _readByteCount = 0;

			bool          _terminationRequested = false;
			/*-------------------------------------------------------------------
			-                   Asynchronous Process
			---------------------------------------------------------------------*/
			std::mutex _lock;
			std::thread _thread;
			std::condition_variable _readFinishedCondition;
			std::condition_variable _readRequestedCondition;
			std::exception_ptr      _readError;
		};
#endif
	}
	/****************************************************************************
	*				  			LineReader
	*************************************************************************//**
	*  @class     LineReader
	*  @brief     Csv row data reader
	*****************************************************************************/
	class LineReader
	{
		using ByteSourcePtr = std::unique_ptr<ByteSourceBase>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		static ByteSourcePtr OpenFile(const std::string& fileName);
		void  Initialize(ByteSourcePtr byteSource);
		char* ProceedNextLine();
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		inline void         SetFileName(const char* fileName)
		{
			if (fileName != nullptr) { _fileName = std::string(fileName); }
		}
		inline void         SetFileName(const std::string& fileName) { _fileName = fileName; }
		inline void         SetFileLine(unsigned int fileLine) { _fileLine = fileLine; }
		inline unsigned int GetFileLine() const { return _fileLine; }
		inline const char* GetTruncatedFileName() const { return _fileName.c_str(); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		LineReader() = delete;
		LineReader(const LineReader&) = delete;
		LineReader& operator=(const LineReader&) = delete;
		LineReader(const char* fileName, ByteSourcePtr byteSource) { _fileName = std::string(fileName); Initialize(std::move(byteSource)); }
		LineReader(const std::string& fileName, ByteSourcePtr byteSource) { _fileName = fileName;              Initialize(std::move(byteSource)); }
		LineReader(const char* fileName, const char* dataBegin, const char* dataEnd) { _fileName = std::string(fileName); Initialize(ByteSourcePtr(new detail::NonOwningStringByteSource(dataBegin, dataEnd - dataBegin))); }
		LineReader(const std::string& fileName, const char* dataBegin, const char* dataEnd) { _fileName = fileName;              Initialize(ByteSourcePtr(new detail::NonOwningStringByteSource(dataBegin, dataEnd - dataBegin))); }
		LineReader(const char* fileName, FILE* file) { _fileName = std::string(fileName); Initialize(ByteSourcePtr(new detail::OwningStdIOByteSourceBase(file))); }
		LineReader(const std::string& fileName, FILE* file) { _fileName = fileName;              Initialize(ByteSourcePtr(new detail::OwningStdIOByteSourceBase(file))); }
		LineReader(const char* fileName, std::istream& in) { _fileName = std::string(fileName); Initialize(ByteSourcePtr(new detail::NonOwningIStreamByteSource(in))); }
		LineReader(const std::string& fileName, std::istream& in) { _fileName = fileName; Initialize(ByteSourcePtr(new detail::NonOwningIStreamByteSource(in))); }
		explicit LineReader(const char* fileName) { _fileName = std::string(fileName); Initialize(OpenFile(fileName)); }
		explicit LineReader(const std::string& fileName) { _fileName = fileName;              Initialize(OpenFile(fileName)); }

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		std::string  _fileName;
		unsigned int _fileLine;

		std::unique_ptr<char[]> _buffer;
		int _dataBegin;
		int _dataEnd;
#ifdef CSV_IO_NO_THREAD
		SynchronousReader _reader;
#else
		detail::AsynchronousReader _reader;
#endif
		static const int BlockLength = 1 << 20;
	};


#pragma region Overflow
	struct ThrowOnOverflow
	{
		template<class T> static void OnOverflow (T&) { throw error::IntergerOverflow(); }
		template<class T> static void OnUnderflow(T&) { throw error::IntergerUnderflow(); }
	};
	struct IgnoreOverflow
	{
		template<class T> static void OnOverflow(T&) {};
		template<class T> static void OnUnderflow(T&) {};
	};
	struct SetToMaxOnOverflow
	{
		template<class T>static void OnOverflow(T& x) { x = (std::numeric_limits<T>::max)(); }
		template<class T>static void OnUnderflow(T& x) { x = (std::numeric_limits<T>::min)(); }
	};
#pragma endregion Overflow
	/****************************************************************************
	*				  			CSVReader
	*************************************************************************//**
	*  @class     CSVReader
	*  @brief     CSV reader
	*****************************************************************************/
	template <unsigned int columnCount,
		class TrimPolicy     = detail::TrimChars<' ', '\t'>,
		class QuotePolicy    = detail::NoQuoteEscape<','>,
		class OverflowPolicy = ThrowOnOverflow,
		class CommentPolicy  = detail::NoComment>
	class CSVReader
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		char* NextLine() { return _reader.ProceedNextLine(); }
		template<class... ColumnNameList> void ReadHeader(IgnoreColumn ignorePolicy, ColumnNameList... columnNameList);
		template<class... ColumnNameList> void SetHeader (ColumnNameList... columnNameList);
		template<class... ColumnType>     bool ReadRow(ColumnType&... columns);
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		bool HasColumn(const std::string& columnName) const;
		inline const char* GetTruncatedFileName() const { return _reader.GetTruncatedFileName(); }
		inline void  SetFileName(const std::string& fileName) { _reader.SetFileName(fileName); }
		inline void  SetFileName(const char*        fileName) { _reader.SetFileName(fileName); }
		inline unsigned int GetFileLine() const               { return _reader.GetFileLine(); }
		inline void         SetFileLine(unsigned int fileLine) { _reader.SetFileLine(fileLine); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CSVReader() = delete;
		CSVReader(const CSVReader&) = delete;
		CSVReader& operator=(const CSVReader&) = delete;
		template<class... Args> explicit CSVReader(Args&&... args);
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		template<class... ColumnNameList> void SetColumnNames(std::string string, ColumnNameList... columnNameList)
		{
			_columnNameList[columnCount - sizeof...(ColumnNameList) - 1] = std::move(string);
			SetColumnNames(std::forward<ColumnNameList>(columnNameList)...);
		}
		void                                SetColumnNames() {};
		void ParseHelper(std::size_t) {};
		template<class T, class... ColumnType >
		void ParseHelper(std::size_t row, T& t, ColumnType&... column);
		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		LineReader _reader;
		char* _row[columnCount];
		std::string      _columnNameList[columnCount];
		std::vector<int> _columnOrder;

	};
#pragma region DoubleQuoteEscape
	/****************************************************************************
	*                       FindNextColumnEnd
	*************************************************************************//**
	*  @fn        template<char Separate, char Quote>
				  const char* DoubleQuoteEscape<Separate, Quote>::FindNextColumnEnd(const char* columnBegin)
	*  @brief     FindNextColumnEnd
	*  @param[in] const char* columnBegin
	*  @return 　　const char*
	*****************************************************************************/
	template<char Separate, char Quote>
	const char* detail::DoubleQuoteEscape<Separate, Quote>::FindNextColumnEnd(const char* columnBegin)
	{
		while (*columnBegin != Separate && *columnBegin != '\0')
		{
			if (*columnBegin != Quote) { ++columnBegin; }
			else
			{
				do
				{
					++columnBegin;
					while (*columnBegin != Quote)
					{
						if (*columnBegin == '\0') { throw error::EscapedStringNotClosed(); }
						++columnBegin;
					}
					++columnBegin;
				} while (*columnBegin == Quote);
			}
		}
	}
	/****************************************************************************
	*                       UnEscape
	*************************************************************************//**
	*  @fn        template<char Separate, char Quote>
				  void DoubleQuoteEscape<Separate, Quote>::UnEscape(char*& columnBegin, char*& columnEnd)
	*  @brief     UnEscape
	*  @param[in] const char* columnBegin
	*  @param[in] const char* columnEnd
	*  @return 　　void
	*****************************************************************************/
	template<char Separate, char Quote>
	void detail::DoubleQuoteEscape<Separate, Quote>::UnEscape(char*& columnBegin, char*& columnEnd)
	{
		if (columnEnd - columnBegin < 2) { return; }
		if (*columnBegin == Quote && *(columnEnd - 1) == Quote)
		{
			++columnBegin; --columnEnd;
			char* out = columnBegin;
			for (char* in = columnBegin; in != columnEnd; ++in)
			{
				if (*in == Quote && (in + 1) != columnEnd && *(in + 1) == Quote) { ++in; }
				*out = *in;
				++out;
			}
			columnEnd = out;
			*columnEnd = '\0';
		}
	}
#pragma endregion DoubleQuoteEscape
#pragma region Parse
	/****************************************************************************
	*                       ChopNextColumn
	*************************************************************************//**
	*  @fn        template <class QuotePolicy> void detail::ChopNextColumn(char*& line, char*& columnBegin, char*& columnEnd)
	*  @brief     Get current column string
	*  @param[in,out] char*& line
	*  @param[in,out] char*& columnBegin
	*  @param[in,out] char*& columnEnd
	*  @return 　　void
	*****************************************************************************/
	template <class QuotePolicy> void detail::ChopNextColumn(char*& line, char*& columnBegin, char*& columnEnd)
	{
		if (line == nullptr) { return; }
		/*-------------------------------------------------------------------
		-             Proceed next column
		---------------------------------------------------------------------*/
		columnBegin = line;
		columnEnd   = columnBegin + (QuotePolicy::FindNextColumnEnd(columnBegin) - columnBegin);
		if (*columnEnd == '\0') { line = nullptr; }
		else
		{
			*columnEnd = '\0';
			line       = columnEnd + 1;
		}
	}
	/****************************************************************************
	*                       ParseLine
	*************************************************************************//**
	*  @fn        template <class TrimPolicy, class QuotePolicy> void detail::ParseLine(char* line, char** sortedColumn, const std::vector<int>& columnOrder)
	*  @brief     Parse line 
	*  @param[in,out] char*& line
	*  @param[in,out] char** sortedColumn
	*  @param[in] const std::vector<int>& columnOrder
	*  @return 　　void
	*****************************************************************************/
	template <class TrimPolicy, class QuotePolicy> void detail::ParseLine(char* line, char** sortedColumn, const std::vector<int>& columnOrder)
	{
		for (int i : columnOrder)
		{
			if (line == nullptr) { throw error::TooFewColumns(); }
			
			char* columnBegin = nullptr; char* columnEnd = nullptr;
			detail::ChopNextColumn<QuotePolicy>(line, columnBegin, columnEnd);

			if (i != csv::INVALID_VALUE)
			{
				TrimPolicy::Trim(columnBegin, columnEnd);
				QuotePolicy::UnEscape(columnBegin, columnEnd);
				sortedColumn[i] = columnBegin;
			}
		}
		if (line != nullptr) { throw error::TooManyColumns(); }
	}
	template <unsigned int ColumnCount, class TrimPolicy, class QuotePolicy >
	void detail::ParseHeaderLine(char* line, std::vector<int>& columnOrder, const std::string* columnName, IgnoreColumn ignorePolicy)
	{
		columnOrder.clear();

		bool found[ColumnCount] = {};
		std::fill(found, found + ColumnCount, false);
		while (line)
		{
			char* columnBegin = nullptr; char* columnEnd = nullptr;
			ChopNextColumn<QuotePolicy>(line, columnBegin, columnEnd);

			TrimPolicy::Trim(columnBegin, columnEnd);
			QuotePolicy::UnEscape(columnBegin, columnEnd);

			for (unsigned i = 0; i < ColumnCount; ++i)
				if (columnBegin == columnName[i])
				{
					if (found[i])
					{
						error::DuplicatedColumnInHeader err;
						err.SetColumnName(columnBegin);
						throw err;
					}
					found[i] = true;
					columnOrder.push_back(i);
					columnBegin = 0;
					break;
				}
			if (columnBegin)
			{
				if ((unsigned int)ignorePolicy & (unsigned int)IgnoreColumn::IgnoreExtraColumn)
					columnOrder.push_back(-1);
				else
				{
					error::ExtraColumnInHeader err;
					err.SetColumnName(columnBegin);
					throw err;
				}
			}
		}
		if (!((unsigned int)ignorePolicy & (unsigned int)IgnoreColumn::IgnoreMissingColumn))
		{
			for (unsigned i = 0; i < ColumnCount; ++i)
			{
				if (!found[i])
				{
					error::MissingColumnInHeader err;
					err.SetColumnName(columnName[i].c_str());
					throw err;
				}
			}
		}
	}
	/****************************************************************************
	*                       Parse (Char)
	*************************************************************************//**
	*  @fn        template<class OverflowPolicy> void detail::Parse(char* column, char& x)
	*  @brief     Parse char 
	*  @param[in,out] char* column
	*  @param[out] char& x
	*  @return 　　void
	*****************************************************************************/
	template<class OverflowPolicy> void detail::Parse(char* column, char& x)
	{
		if (!*column) { throw error::InvalidSingleCharacter(); }
		x = *column; ++column;
		if (*column) { throw error::InvalidSingleCharacter(); }
	}
	/****************************************************************************
	*                       Parse (Unsigned int )
	*************************************************************************//**
	*  @fn        template<class OverflowPolicy> void detail::Parse(char* column, char& x)
	*  @brief     Parse unsigned int
	*  @param[in] char* column
	*  @param[out] T& x
	*  @return 　　void
	*****************************************************************************/
	template<class OverflowPolicy, class T> void detail::ParseUnsignedInteger(const char* column, T& x)
	{
		x = 0;
		while (*column != '\0')
		{
			if ('0' <= *column && *column <= '9')
			{
				T y = *column - '0';
				if (x > ((std::numeric_limits<T>::max)() - y) / 10)
				{
					OverflowPolicy::OnOverflow(x);
					return;
				}
				x = 10 * x + y;
			}
			else
				throw error::NoDigit();
			++column;
		}
	}
	template<class OverflowPolicy, class T> void detail::ParseSignedInteger(const char* column, T& x)
	{
		if (*column == '-')
		{
			++column;

			x = 0;
			while (*column != '\0')
			{
				if ('0' <= *column && *column <= '9')
				{
					T y = *column - '0';
					if (x < ((std::numeric_limits<T>::min)() + y) / 10)
					{
						OverflowPolicy::OnUnderflow(x);
						return;
					}
					x = 10 * x - y;
				}
				else
					throw error::NoDigit();
				++column;
			}
			return;
		}
		else if (*column == '+')
			++column;
		ParseUnsignedInteger<OverflowPolicy>(column, x);
	}
	template<class T> void detail::ParseFloat(const char* column, T& x)
	{
		bool isNeg = false;
		/*-------------------------------------------------------------------
		-               Signed check
		---------------------------------------------------------------------*/
		if      (*column == '-') { isNeg = true; ++column; }
		else if (*column == '+') { ++column; }
		/*-------------------------------------------------------------------
		-               Integer part
		---------------------------------------------------------------------*/
		x = 0;
		while ('0' <= *column && *column <= '9')
		{
			int y = *column - '0';
			x *= 10;
			x += y;
			++column;
		}

		if (*column == '.' || *column == ',')
		{
			++column;
			T pos = 1;
			while ('0' <= *column && *column <= '9')
			{
				pos /= 10;
				int y = *column - '0';
				++column;
				x += y * pos;
			}
		}

		if (*column == 'e' || *column == 'E')
		{
			++column;
			int e;

			ParseSignedInteger<SetToMaxOnOverflow>(column, e);

			if (e != 0)
			{
				T base;
				if (e < 0)
				{
					base = T(0.1);
					e = -e;
				}
				else
				{
					base = T(10);
				}

				while (e != 1)
				{
					if ((e & 1) == 0)
					{
						base = base * base;
						e >>= 1;
					}
					else
					{
						x *= base;
						--e;
					}
				}
				x *= base;
			}
		}
		else
		{
			if (*column != '\0')
				throw error::NoDigit();
		}

		if (isNeg)
			x = -x;
	}

#pragma endregion Parse
#pragma region CSVReader
	template <unsigned int columnCount, class TrimPolicy, class QuotePolicy, class OverflowPolicy, class CommentPolicy>
	template<class... Args> CSVReader<columnCount, TrimPolicy, QuotePolicy, OverflowPolicy, CommentPolicy>::CSVReader(Args&&... args) : _reader(std::forward<Args>(args)...)
	{
		std::fill(_row, _row + columnCount, nullptr);
		_columnOrder.resize(columnCount);
		for (unsigned int i = 0; i < columnCount; ++i) { _columnOrder[i] = i; }
		for (unsigned int i = 1; i <= columnCount; ++i) { _columnNameList[i - 1] = "col" + std::to_string(i); }
	}
	/****************************************************************************
	*                       ReadHeader
	*************************************************************************//**
	*  @fn        template<class... ColumnNameList> void CSVReader::ReadHeader()
	*  @brief     ReadHeader
	*  @param[in] void
	*  @return 　　void
	*****************************************************************************/
	template <unsigned int columnCount, class TrimPolicy, class QuotePolicy, class OverflowPolicy, class CommentPolicy>
	template<class... ColumnNameList> void CSVReader<columnCount, TrimPolicy, QuotePolicy, OverflowPolicy, CommentPolicy>::ReadHeader(IgnoreColumn ignorePolicy, ColumnNameList... columnNameList)
	{
		// sizeof...で可変引数にいくつ変数を持っているかを調べる https://qiita.com/kaizen_nagoya/items/8e7abc71f8c6516fc9f4
		static_assert(sizeof...(columnNameList) >= columnCount, "Not enough colum names");
		static_assert(sizeof...(columnNameList) <= columnCount, "Too many column names");
		try
		{
			/*-------------------------------------------------------------------
			-               Set column names
			---------------------------------------------------------------------*/
			SetColumnNames(std::forward<ColumnNameList>(columnNameList)...);
			/*-------------------------------------------------------------------
			-               Set column names
			---------------------------------------------------------------------*/
			char* line = nullptr;
			do
			{
				line = _reader.ProceedNextLine();
				if (!line) { throw error::HeaderMissing(); }
			} while (CommentPolicy::IsComment(line));

			detail::ParseHeaderLine<columnCount, TrimPolicy, QuotePolicy>(line, _columnOrder, _columnNameList, ignorePolicy);

		}
		catch (error::WithFileName& error)
		{
			error.SetFileName(_reader.GetTruncatedFileName()); throw;
		}
	}
	/****************************************************************************
	*                       SetHeader
	*************************************************************************//**
	*  @fn        template <unsigned int columnCount, class TrimPolicy, class QuotePolicy, class OverflowPolicy, class CommentPolicy>
				  template<class... ColumnNameList> void CSVReader<columnCount, TrimPolicy, QuotePolicy, OverflowPolicy, CommentPolicy>::SetHeader(ColumnNameList... columnNameList)
	*  @brief     SetHeader
	*  @param[in] ColumnNameList... columnNameList
	*  @return 　　void
	*****************************************************************************/
	template <unsigned int columnCount, class TrimPolicy, class QuotePolicy, class OverflowPolicy, class CommentPolicy>
	template<class... ColumnNameList> void CSVReader<columnCount, TrimPolicy, QuotePolicy, OverflowPolicy, CommentPolicy>::SetHeader(ColumnNameList... columnNameList)
	{
		static_assert(sizeof...(columnNameList) >= columnCount, "Not enough colum names");
		static_assert(sizeof...(columnNameList) <= columnCount, "Too many column names");
		SetColumnNames(std::forward<ColumnNameList>(columnNameList)...);
		std::fill(_row, _row + columnCount, nullptr);
		_columnOrder.resize(columnCount);
		for (unsigned int i = 0; i < columnCount; ++i) { _columnOrder[i] = i; }
	}
	/****************************************************************************
	*                       HasColumn
	*************************************************************************//**
	*  @fn        template <unsigned int columnCount, class TrimPolicy, class QuotePolicy, class OverflowPolicy, class CommentPolicy>
				  bool CSVReader<columnCount, TrimPolicy, QuotePolicy, OverflowPolicy, CommentPolicy>::HasColumn(const std::string& columnName) const
	*  @brief     Has column
	*  @param[in] const std::string& columnName
	*  @return 　　bool
	*****************************************************************************/
	template <unsigned int columnCount, class TrimPolicy, class QuotePolicy, class OverflowPolicy, class CommentPolicy>
	bool CSVReader<columnCount, TrimPolicy, QuotePolicy, OverflowPolicy, CommentPolicy>::HasColumn(const std::string& columnName) const
	{
		return _columnOrder.end() != std::find(
			_columnOrder.begin(), _columnOrder.end(),
			std::find(std::begin(_columnNameList), std::end(_columnNameList), name) - std::begin(_columnNameList));
	}
	/****************************************************************************
	*                       ReadRow
	*************************************************************************//**
	*  @fn        template <unsigned int columnCount, class TrimPolicy, class QuotePolicy, class OverflowPolicy, class CommentPolicy>
				  template<class... ColumnType> bool CSVReader<columnCount, TrimPolicy, QuotePolicy, OverflowPolicy, CommentPolicy>::ReadRow(ColumnType&... columns)
	*  @brief     Read row
	*  @param[in] ColumnType&... columns
	*  @return 　　bool
	*****************************************************************************/
	template <unsigned int columnCount, class TrimPolicy, class QuotePolicy, class OverflowPolicy, class CommentPolicy>
	template<class... ColumnType> bool CSVReader<columnCount, TrimPolicy, QuotePolicy, OverflowPolicy, CommentPolicy>::ReadRow(ColumnType&... columns)
	{
		static_assert(sizeof...(ColumnType) >= columnCount, "not enough columns specified");
		static_assert(sizeof...(ColumnType) <= columnCount, "too many columns specified");
		try
		{
			try
			{

				char* line = nullptr;
				do
				{
					line = _reader.ProceedNextLine();
					if (!line) { return false; }
				} while (CommentPolicy::IsComment(line));

				detail::ParseLine<TrimPolicy, QuotePolicy> (line, _row, _columnOrder);

				ParseHelper(0, columns...);
			}
			catch (error::WithFileName& err)
			{
				err.SetFileName(_reader.GetTruncatedFileName());
				throw;
			}
		}
		catch (error::WithFileLine& err)
		{
			err.SetFileLine(_reader.GetFileLine());
			throw;
		}

		return true;
	}
	
	template <unsigned int columnCount, class TrimPolicy, class QuotePolicy, class OverflowPolicy, class CommentPolicy>
	template<class T, class... ColumnType > void CSVReader<columnCount, TrimPolicy, QuotePolicy, OverflowPolicy, CommentPolicy>::ParseHelper(std::size_t row, T& t, ColumnType&... column)
	{
		if (_row[row])
		{
			try
			{
				try
				{
					detail::Parse<OverflowPolicy>(_row[row], t);
				}
				catch (error::WithColumnContent& err)
				{
					err.SetColumnContent(_row[row]);
					throw;
				}
			}
			catch (error::WithColumnName& err)
			{
				err.SetColumnName(_columnNameList[row].c_str());
				throw;
			}
		}
		ParseHelper(row + 1, column...);
	}
#pragma endregion CSVReader
}
#endif