//////////////////////////////////////////////////////////////////////////////////
///             @file   WaveDecoder.cpp
///             @brief  Wave File Write and Read
///             @author Toide Yutaro
///             @date   2022_03_12
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Audio/Include/WavDecoder.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "winmm.lib")

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
WavDecoder::WavDecoder()
{
	ZeroMemory(&_waveFormatEx, sizeof(_waveFormatEx));
}
#pragma region Public Function
/****************************************************************************
*                       LoadFromFile
*************************************************************************//**
*  @fn        bool WavDecoder::LoadFromFile(const std::wstring& filePath)
*  @brief     Open Wav File
*  @param[in] const std::wstring& filePath
*  @return 　　bool
*****************************************************************************/
bool WavDecoder::LoadFromFile(const std::wstring& filePath)
{
	MMCKINFO riffChunk, fmtChunk, dataChunk;

	/*-------------------------------------------------------------------
	-              File open
	---------------------------------------------------------------------*/
	if (!Open(filePath)) { return false; }
	/*-------------------------------------------------------------------
	-              Check Wave Header
	---------------------------------------------------------------------*/
	if (!CheckChunk(&riffChunk, nullptr, 'W', 'A', 'V', 'E', MMIO_FINDRIFF)) { return false; }
	/*-------------------------------------------------------------------
	-              Check FMT Chunk
	---------------------------------------------------------------------*/
	if (!CheckChunk(&fmtChunk, &riffChunk, 'f', 'm', 't', ' ', MMIO_FINDCHUNK)) { return false; }
	/*-------------------------------------------------------------------
	-              Read data to WAVEFORMATEX
	---------------------------------------------------------------------*/
	if (!CreateWaveFormatEx(fmtChunk.cksize)) { return false; }
	/*-------------------------------------------------------------------
	-              Return the reading position to the top.
	---------------------------------------------------------------------*/
	mmioAscend(_handle, &fmtChunk, SEEK_SET);
	/*-------------------------------------------------------------------
	-              Check DATA Chunk
	---------------------------------------------------------------------*/
	if (!CheckChunk(&dataChunk, &riffChunk, 'd', 'a', 't', 'a', MMIO_FINDCHUNK)) { return false; }
	/*-------------------------------------------------------------------
	-              Read WAVE Data
	---------------------------------------------------------------------*/
	if (!CreateWaveData(dataChunk.cksize)) { return false; }

	Close();
	return true;
}
#pragma region Property
const WAVEFORMATEX& WavDecoder::GetFileFormatEx() const
{
	return this->_waveFormatEx;
}
const std::wstring& WavDecoder::GetFilePath() const
{
	return this->_filePath;
}
const std::shared_ptr<BYTE[]>& WavDecoder::GetWaveData() const
{
	return _waveData;
}

const size_t WavDecoder::GetWaveSize() const
{
	return this->_waveDataSize;
}
#pragma endregion Property
#pragma endregion Public Function
#pragma region Private Function
/****************************************************************************
*                       Open
*************************************************************************//**
*  @fn        bool WavDecoder::Open(const std::wstring& filePath)
*  @brief     Open Wav File
*  @param[in] const std::wstring& filePath
*  @return 　　bool
*****************************************************************************/
bool WavDecoder::Open(const std::wstring& filePath)
{
	MMIOINFO mmioInfo;
	memset(&mmioInfo, 0, sizeof(MMIOINFO));

	/*-------------------------------------------------------------------
	-              Is Wav File Path
	---------------------------------------------------------------------*/
	std::wstring extension = file::FileSystem::GetExtension(filePath);
	if (extension != L"wav")
	{
		MessageBox(NULL, L"can't read wavFile", L"Warning", MB_ICONWARNING);
		return false;
	}

	/*-------------------------------------------------------------------
	-              File Open
	---------------------------------------------------------------------*/
	LPWSTR path = (LPWSTR)filePath.c_str();
	if (!(_handle == mmioOpen(path, &mmioInfo, MMIO_READ)))
	{
		MessageBox(NULL, L"can't create MMIO.", L"Warning", MB_ICONWARNING);
		return false;
	}

	_filePath = filePath;
	return true;
}
/****************************************************************************
*                       CreateWaveFormatEx
*************************************************************************//**
*  @fn        bool WavDecoder::CreateWaveFormatEx(size_t formatChunk)
*  @brief     Create Wave Format Ex
*  @param[in] size_t formatChunk
*  @return 　　bool
*****************************************************************************/
bool WavDecoder::CreateWaveFormatEx(size_t formatChunk)
{
	LONG readSize = -1;
	readSize = static_cast<LONG>(mmioRead(_handle, reinterpret_cast<HPSTR>(&_waveFormatEx), static_cast<LONG>(formatChunk)));

	if (readSize != (LONG)formatChunk)
	{
		MessageBox(NULL, L"Loaded sizes do not match", L"Warning", MB_ICONWARNING);
		return false;
	}

	return true;
}
/****************************************************************************
*                       CreateWaveData
*************************************************************************//**
*  @fn        bool WavDecoder::CreateWaveData(size_t dataSize)
*  @brief     CreateWaveData
*  @param[in] size_t dataSize
*  @return 　　bool
*****************************************************************************/
bool WavDecoder::CreateWaveData(size_t dataSize)
{
	_waveDataSize = dataSize;
	std::unique_ptr<BYTE[]> data = std::make_unique<BYTE[]>(dataSize);

	if (mmioRead(_handle, reinterpret_cast<HPSTR>(data.get()), (LONG)_waveDataSize) != static_cast<LONG>(_waveDataSize))
	{
		MessageBox(NULL, L"couldn't read wave data.", L"Warning", MB_ICONWARNING);
		return false;
	}

	_waveData = std::move(data);
	return true;
}
/****************************************************************************
*                       CheckChunk
*************************************************************************//**
*  @fn        bool WavDecoder::CheckChunk()
*  @brief     Check Chunk
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool WavDecoder::CheckChunk(MMCKINFO* chunkNext, MMCKINFO* chunkPrev, char c0, char c1, char c2, char c3, UINT flag)
{
	FOURCC* fourcc = nullptr;
	if (flag == MMIO_FINDRIFF)
	{
		fourcc = &chunkNext->fccType;
		*fourcc = mmioFOURCC(c0, c1, c2, c3);
	}
	if (flag == MMIO_FINDCHUNK)
	{
		fourcc = &chunkNext->ckid;
		*fourcc = mmioFOURCC(c0, c1, c2, c3);
	}
	if (fourcc == nullptr)
	{
		MessageBox(NULL, L"different chunk format.", L"Warning", MB_ICONWARNING);
		return false;
	}

	MMRESULT result = mmioDescend(_handle, chunkNext, chunkPrev, flag);
	if (result != MMSYSERR_NOERROR)
	{
		return false;
	}
	return true;
}
/****************************************************************************
*                       Close
*************************************************************************//**
*  @fn        bool WavDecoder::Close()
*  @brief     Close Wav File
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool WavDecoder::Close()
{
	if (_handle == nullptr) { return true; }
	if (mmioClose(_handle, 0) == MMIOERR_CANNOTWRITE) { return false; }

	_handle = nullptr;
	return true;
}
#pragma endregion Private Function