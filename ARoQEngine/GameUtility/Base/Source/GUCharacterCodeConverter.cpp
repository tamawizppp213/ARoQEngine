//////////////////////////////////////////////////////////////////////////////////
///  @file   GUCharacterCodeConverter.cpp
///  @brief  temp
///  @author toide
///  @date   2024/05/26 23:44:39
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUCharacterCodeConverter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     �����R�[�h��ϊ����܂�
*  @param[in] void
*  @return    bool �ϊ��ɐ��������ꍇ��true, ���s�����ꍇ��false
*************************************************************************/
gu::DynamicArray<uint8>& CharacterCodeConverter::Convert(const uint8* input, const uint64 inputByteSize)
{
	Check(input);
	Check(inputByteSize > 0);

	/*-------------------------------------------------------------------
	-            �ϊ�����̂ɕK�v�ȃo�C�g���ŗ̈���Z�o���܂�.
	---------------------------------------------------------------------*/
	const uint64 sourceMaxCharCount = inputByteSize / _sourceCharacterCode->GetMinByte();
	const uint64 requiredByteSize   = sourceMaxCharCount * _destinationCharacterCode->GetMaxByte() + _destinationCharacterCode->GetMinByte(); // �I�[�����p +1

	/*-------------------------------------------------------------------
	-           UTF32��ʂ��ĕϊ�
	---------------------------------------------------------------------*/
	gu::DynamicArray<uint8> tempBuffer1 = {};
	gu::DynamicArray<uint8> tempBuffer2 = {};
	tempBuffer1.Resize(requiredByteSize, false);
	tempBuffer2.Resize(requiredByteSize, false);

	CharacterDecodeResult decodeResult = {};
	CharacterEncodeResult encodeResult = {};

	if (_destinationCharacterCode->GetMaxByte() >= 4)
	{

		if (!_sourceCharacterCode->ToUTF32((const uint8*)input, inputByteSize, (uint32*)tempBuffer1.Data(), tempBuffer1.Size(), &decodeResult))
		{
			throw "Failed to convert character code to UTF32 (intermediate).";
		}

		if (!_destinationCharacterCode->FromUTF32((const uint32*)tempBuffer1.Data(), decodeResult.OutputCharacterCount, tempBuffer2.Data(), tempBuffer2.Size(), &encodeResult))
		{
			throw "Failed to convert character code. (output)";
		}
	}
	/*-------------------------------------------------------------------
	-           UTF16��ʂ��ĕϊ�
	---------------------------------------------------------------------*/
	else
	{
		if (!_sourceCharacterCode->ToUTF16((const uint8*)input, inputByteSize, (uint16*)tempBuffer1.Data(), tempBuffer1.Size(), &decodeResult))
		{
			throw "Failed to convert character code to UTF32 (intermediate).";
		}

		if (!_destinationCharacterCode->FromUTF16((const uint16*)tempBuffer1.Data(), decodeResult.OutputCharacterCount, tempBuffer2.Data(), tempBuffer2.Size(), &encodeResult))
		{
			throw "Failed to convert character code. (output)";
		}

	}

	_outputBuffer.Resize(encodeResult.OutputByteSize, false);
	Memory::Copy(_outputBuffer.Data(), tempBuffer2.Data(), encodeResult.OutputByteSize);

	return _outputBuffer;

}
#pragma endregion Public Function