//////////////////////////////////////////////////////////////////////////////////
//              @file   ModelConverter.cpp
///             @brief  Model file Converter Module
///             @author Toide Yutaro
///             @date   2022_06_07
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GameModelConverter.hpp"
#include "../Include/GameModel.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
//namespace
//{
//
//#pragma region ModelConverter
//	class IModelConverter
//	{
//	public :
//		virtual bool ToModel(const std::wstring& filePath, Model* model) = 0;
//		//virtual bool FromModel(const std::wstring& filePath, const Model* model) = 0;
//	};
//	class ModelConverterPMD : public IModelConverter
//	{
//	public:
//		bool ToModel(const std::wstring& filePath, Model* model) override { std::wcout << filePath << std::endl;  return true; };
//		//bool FromModel(const std::wstring& filePath, const Model* model) override { return true; };
//	private:
//		void ConvertMaterial(Material& buffer, const pmx::PMXFile& file) { std::cout << buffer.GetMaterialByteSize() << std::endl; std::cout << file.Directory << std::endl; };
//	};
//	
//	class ModelConverterPMX : public IModelConverter
//	{
//	public:
//		bool ToModel(const std::wstring& filePath, Model* model) override;
//		//bool FromModel(const std::wstring& filePath, const Model* model) override { return true; };
//	private:
//		void ConvertMaterial(Material& buffer, const pmx::PMXFile& file);
//	};
//
//	class ModelConverterGLTF : public IModelConverter
//	{
//	public:
//		bool ToModel(const std::wstring& filePath, Model* model) override;
//		//bool FromModel(const std::wstring& filePath, const Model* model) override { return true; };
//	};
//
//
//	bool ModelConverterPMX::ToModel(const std::wstring& filePath, Model* model)
//	{
//		/*-------------------------------------------------------------------
//		-            PMXFile Load
//		---------------------------------------------------------------------*/
//		pmx::PMXFile file;
//		if (!file.Load(filePath)) { return false; }
//		/*-------------------------------------------------------------------
//		-            Convert Vertices
//		---------------------------------------------------------------------*/
//		auto& meshes = model->GetMesh();
//		
//
//		/*-------------------------------------------------------------------
//		-            Convert Material
//		---------------------------------------------------------------------*/
//		auto& materialBuffer = model->GetMaterialBuffer();
//		
//		
//		return true;
//	}
//
//	void ModelConverterPMX::ConvertMaterial(MaterialBuffer& buffer, const pmx::PMXFile& file)
//	{
//		std::cout << file.Directory << std::endl;
//		gu::SharedPointer<UploadBuffer> uploadBuffer = buffer.GetBufferPtr();
//		uploadBuffer->CopyStart();
//		
//		
//		uploadBuffer->CopyEnd();
//	}
//	
//
//	bool ModelConverterGLTF::ToModel(const std::wstring& filePath, Model* model)
//	{
//		/*-------------------------------------------------------------------
//		-            Load
//		---------------------------------------------------------------------*/
//		gltf::GLTFFile file;
//		file.Load(unicode::ToUtf8String(filePath));
//		model = nullptr;
//		std::wcout << filePath;
//		//const auto& document = file.Document;
//		//auto&       reader   = file.ResourceReader;
//		///*-------------------------------------------------------------------
//		//-            Convert
//		//---------------------------------------------------------------------*/
//		//for (const auto& mesh : document.Meshes.Elements())
//		//{
//
//		//}
//		return true;
//	}
//
//}
//#pragma endregion ModelConverter Interface
//#pragma region Model Converter class
//bool ModelConverter::ToModel(const std::wstring& filePath, Model* model)
//{
//	/*-------------------------------------------------------------------
//	-            Get Extension
//	---------------------------------------------------------------------*/
//	std::wstring& extension = file::FileSystem::GetExtension(filePath);
//	/*-------------------------------------------------------------------
//	-            Choice IModel Converter
//	---------------------------------------------------------------------*/
//	std::unique_ptr<::IModelConverter> converter = nullptr;
//	if      (extension == L"pmd")  { converter = std::make_unique<ModelConverterPMD>(); }
//	else if (extension == L"pmx")  { converter = std::make_unique<ModelConverterPMX>(); }
//	else if (extension == L"glb")  { converter = std::make_unique<ModelConverterGLTF>(); }
//	else if (extension == L"gltf") { converter = std::make_unique<ModelConverterGLTF>(); }
//	else
//	{
//		std::cerr << "Invalid extension. Please check the extension type. " << std::endl;
//		return false;
//	}
//	/*-------------------------------------------------------------------
//	-            Model Load
//	---------------------------------------------------------------------*/
//	converter->ToModel(filePath, model);
//	return true;
//}
//bool ModelConverter::FromModel(const std::wstring& filePath, const Model* model)
//{
//	/*-------------------------------------------------------------------
//	-            Get Extension
//	---------------------------------------------------------------------*/
//	std::wstring& extension = file::FileSystem::GetExtension(filePath);
//	/*-------------------------------------------------------------------
//	-            Choice IModel Converter
//	---------------------------------------------------------------------*/
//	std::unique_ptr<::IModelConverter> converter = nullptr;
//	if      (extension == L"pmd")  { converter = std::make_unique<ModelConverterPMD>(); }
//	else if (extension == L"pmx")  { converter = std::make_unique<ModelConverterPMX>(); }
//	else if (extension == L"glb")  { converter = std::make_unique<ModelConverterGLTF>(); }
//	else if (extension == L"gltf") { converter = std::make_unique<ModelConverterGLTF>(); }
//	else
//	{
//		std::cerr << "Invalid extension. Please check the extension type. " << std::endl;
//		return false;
//	}
//	/*-------------------------------------------------------------------
//	-            Model Load
//	---------------------------------------------------------------------*/
//	std::wcout << model->GetName() << std::endl;
//	//converter->FromModel(filePath, model);
//	return true;
//}
//#pragma endregion Model Converter class