//////////////////////////////////////////////////////////////////////////////////
///             @file   WaveFile.hpp
///             @brief  Wave File Write and Read
///             @author Toide Yutaro
///             @date   2020_12_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WAVE_DECODER_HPP
#define WAVE_DECODER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <memory>
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************/
/* @class     WavFile
*  @brief     Wave File write and read
*****************************************************************************/
class WavDecoder
{
protected:
	using ByteArrayPtr = std::shared_ptr<BYTE[]>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool LoadFromFile(const std::wstring& filePath);

	const WAVEFORMATEX& GetFileFormatEx() const;
	const std::wstring& GetFilePath()     const;
	const std::shared_ptr<BYTE[]>& GetWaveData() const;
	const size_t                   GetWaveSize()  const;

	/****************************************************************************
	**                Public Property
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	WavDecoder();
	~WavDecoder() = default;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	bool Open(const std::wstring & filePath);
	bool CreateWaveFormatEx(size_t formatChunk);
	bool CreateWaveData(size_t dataSize);
	bool CheckChunk(MMCKINFO * chunkNext, MMCKINFO * chunkPrev, char c0, char c1, char c2, char c3, UINT flag);
	bool Close();
	/****************************************************************************
	**                Private Property
	*****************************************************************************/
	HMMIO        _handle   = nullptr;
	ByteArrayPtr _waveData = nullptr;
	size_t       _waveDataSize   = 0;
	std::wstring _filePath = L"";
	WAVEFORMATEX _waveFormatEx;

	char _padding[6];
};
#endif
