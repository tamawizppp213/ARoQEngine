//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueObject.cpp
///  @brief  ObjectŒ^‚ÌJson‚Ì’l‚ðˆµ‚¤ƒNƒ‰ƒX
///  @author toide
///  @date   2024/06/10 0:36:59
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonValueObject.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"

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
/*!**********************************************************************
*  @brief     JsonObjectŒ^‚Ì’l‚ðŽæ“¾‚µ‚Ü‚·.
*  @param[in] void
*  @return    JsonObject
* ************************************************************************/
SharedPointer<JsonObject> JsonValueObject::GetObject()
{
	Checkf(_type == JsonValueType::Object, "Json type is not object type");

	return _object;
}

/*!**********************************************************************
*  @brief     JsonObjectŒ^‚Ì’l‚ðŽæ“¾‚µ‚Ü‚·.
*  @param[in] void
*  @return    JsonObject
* ************************************************************************/
const SharedPointer<JsonObject> JsonValueObject::GetObject() const
{
	Checkf(_type == JsonValueType::Object, "Json type is not object type");

	return _object;
}
#pragma endregion Public Function