//////////////////////////////////////////////////////////////////////////////////
//              @file   ResourceManager.cpp
///             @brief  ResourceManager
///             @author Toide Yutaro
///             @date   2022/03/13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Core/Include/ResourceManager.hpp"
#include "GameCore/Audio/Include/AudioClip.hpp"
#include <unordered_map>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
struct ResourceManager::Implementation
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	/*-------------------------------------------------------------------
	-                        Exists
	---------------------------------------------------------------------*/
	bool ExistsAudioClip(const std::wstring& name) { return AudioTable.find(name) != AudioTable.end(); }
	
	/*-------------------------------------------------------------------
	-                        Clear
	---------------------------------------------------------------------*/
	void ClearAudioTable() { AudioTable.clear(); }
	void ClearAllResources();
	/*-------------------------------------------------------------------
	-                        Get
	---------------------------------------------------------------------*/
	AudioClip& GetAudioClip(const std::wstring& name) { return AudioTable[name]; }
	Implementation();
	~Implementation();
private:
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	std::unordered_map<std::wstring, AudioClip> AudioTable;

};
ResourceManager::Implementation::Implementation()
{

}
ResourceManager::Implementation::~Implementation()
{

}
void ResourceManager::Implementation::ClearAllResources()
{
	ClearAudioTable();
}

#pragma region Public Function
ResourceManager::ResourceManager()
{
	_implementation = std::make_unique<Implementation>();
}
ResourceManager::~ResourceManager()
{

}
#pragma region Exists
/****************************************************************************
*                       ExistsAudioClip
*************************************************************************//**
*  @fn        bool ResourceManager::ExistsAudioClip(const std::wstring& name)
*  @brief     Exists audio clip data
*  @param[in] std::wstring name
*  @return 　　bool
*****************************************************************************/
bool ResourceManager::ExistsAudioClip(const std::wstring& name)
{
	return _implementation->ExistsAudioClip(name);
}
#pragma endregion Exists
#pragma region Clear
/****************************************************************************
*                       ClearAllResources
*************************************************************************//**
*  @fn        void ResourceManager::ClearAllResources()
*  @brief     Clear all resources
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void ResourceManager::ClearAllResources()
{
	_implementation->ClearAllResources();
}
/****************************************************************************
*                       ClearAudioTable
*************************************************************************//**
*  @fn        void ResourceManager::ClearAudioTable()
*  @brief     Clear audio table
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void ResourceManager::ClearAudioTable()
{
	_implementation->ClearAudioTable();
}
#pragma endregion Clear
#pragma region Property
/****************************************************************************
*                       GetAudioClip
*************************************************************************//**
*  @fn        AudioClip& ResourceManager::GetAudioClip(const std::wstring& name)
*  @brief     Get audio clip
*  @param[in] void
*  @return 　　void
*****************************************************************************/
AudioClip& ResourceManager::GetAudioClip(const std::wstring& name)
{
	return _implementation->GetAudioClip(name);
}
#pragma endregion Property

#pragma endregion Public Function