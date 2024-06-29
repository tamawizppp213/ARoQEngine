//////////////////////////////////////////////////////////////////////////////////
///  @file   Json.cpp
///  @brief  Json�̃V���A���C�Y��f�V���A���C�Y���s���N���X
///  @author toide
///  @date   2024/06/29 17:57:17
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Include/Json.hpp"
#include "GameUtility/File/Private/Json/Include/JsonReader.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu::file::json;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function

#pragma endregion Public Function

#pragma region Private Function
/*!**********************************************************************
*  @brief     �V���A���C�Y���s���܂�.�@�V���A���C�Y�Ƃ̓I�u�W�F�N�g��Json�`���̕�����ɕϊ����邱�Ƃł�.
*  @param[in] const gu::SharedPointer<JsonWriter>& json���������ރN���X
*  @param[in] const gu::SharedPointer<JsonObject>& �V���A���C�Y�������I�u�W�F�N�g
*  @return    bool �����������ǂ���
*************************************************************************/
bool JsonSerializer::Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::SharedPointer<Element>& startElement)
{
	return false;
}


/*!**********************************************************************
*  @brief     �f�V���A���C�Y���s���܂�.�@�f�V���A���C�Y�Ƃ�Json�`���̕�������I�u�W�F�N�g�ɕϊ����邱�Ƃł�.
*  @param[in] const gu::SharedPointer<JsonReader>& json��ǂݍ��ރN���X
*  @param[in] const gu::DynamicArray<gu::SharedPointer<JsonValue>>& �f�V���A���C�Y�������I�u�W�F�N�g
*  @return    bool �����������ǂ���
*************************************************************************/
bool JsonSerializer::Deserialize(const gu::SharedPointer<JsonReader>& reader, State& state)
{
	/*-------------------------------------------------------------------
	-          State�̏���
	---------------------------------------------------------------------*/
	// Object��Array�Ƃ����������̗v�f���ꎞ�I�ɒ��߂Ă���
	DynamicArray<SharedPointer<State>> stackState = {};

	// ���݂̓ǂݎ����
	SharedPointer<State> currentState = nullptr;

	// ���݂̓ǂݎ��l
	SharedPointer<JsonValue> newValue = nullptr;

	// �ǂݎ���Ă���Notation
	JsonNotation notation = JsonNotation::None;

	/*-------------------------------------------------------------------
	-         �ǂݎ��
	

	return false;
}
#pragma endregion Private Function