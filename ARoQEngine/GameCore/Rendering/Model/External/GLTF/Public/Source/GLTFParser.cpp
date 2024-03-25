//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFParser.cpp
///             @brief  GLTF File Parser
///             @author Toide Yutaro
///             @date   2022_05_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFParser.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFSerialize.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFDeserialize.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFResourceReader.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFResourceWriter.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFExceptions.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFMeshPrimitiveUtils.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include <fstream>
#include <sstream>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
using namespace gltf::detail::define;
using namespace gltf::MeshPrimitiveUtils;
namespace
{
	class StreamReader : public gltf::detail::IStreamReader
	{
	public:
		StreamReader();
		std::shared_ptr<std::istream> GetInputStream(const std::string& fileName) const override
		{
			auto stream = std::make_shared<std::ifstream>(fileName.c_str(), std::ios_base::binary);
			if (!stream)
			{
				throw std::runtime_error("Unable to create a valid input stream for uri" + fileName);
			}
			return stream;
		}

	};
	StreamReader::StreamReader() {}

	class StreamWriter : public gltf::detail::IStreamWriter
	{
	public:
		StreamWriter();
		std::shared_ptr<std::ostream> GetOutputStream(const std::string& fileName) const override
		{
			auto stream = std::make_shared<std::ofstream>(fileName.c_str(), std::ios_base::binary);
			if (!stream)
			{
				throw std::runtime_error("Unable to create a valid input stream for uri" + fileName);
			}
			return stream;
		}

	};
	StreamWriter::StreamWriter() {}
}
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
void GLTFFile::Load(const std::string& filePath)
{
	/*-------------------------------------------------------------------
	-                 Get Extension and Directory
	---------------------------------------------------------------------*/
	std::string extension = file::FileSystem::GetExtension(filePath);
	std::string directory = file::FileSystem::GetDirectory(filePath);
	/*-------------------------------------------------------------------
	-                      Prepare Resource Reader
	---------------------------------------------------------------------*/
	std::string manifest = "";
	std::unique_ptr<StreamReader>          streamReader = std::make_unique<StreamReader>();
	std::unique_ptr<GLTFResourceReader>    resourceReader = nullptr;
	if (extension == "gltf")
	{
		auto gltfStream = streamReader->GetInputStream(filePath);
		auto gltfResourceReader = std::make_unique<GLTFResourceReader>(std::move(streamReader));

		std::stringstream manifestStream;
		manifestStream << gltfStream->rdbuf();
		manifest = manifestStream.str();

		resourceReader = std::move(gltfResourceReader);
	}
	else if (extension == "glb")
	{
		auto glbStream = streamReader->GetInputStream(filePath);
		auto glbResourceReader = std::make_unique<GLBResourceReader>(std::move(streamReader), std::move(glbStream));

		manifest = glbResourceReader->GetJson();
		resourceReader = std::move(glbResourceReader);
	}
	else
	{
		throw gltf::detail::error::GLTFException("Invalid extension type.");
	}

	/*-------------------------------------------------------------------
	-                      Deserialize
	---------------------------------------------------------------------*/
	try
	{
		Document = Deserialize(manifest);
		resourceReader = std::move(resourceReader);
	}
	catch (const detail::error::GLTFException& exception)
	{
		throw std::runtime_error(exception.what());
	}

}

void GLTFFile::Save(const std::string& filePath, SerializeFlags flags)
{
	/*-------------------------------------------------------------------
	-                 Get Extension and Directory
	---------------------------------------------------------------------*/
	std::string extension = file::FileSystem::GetExtension(filePath);
	std::string directory = file::FileSystem::GetDirectory(filePath);
	/*-------------------------------------------------------------------
	-                      Prepare Resource Reader
	---------------------------------------------------------------------*/
	std::string manifest = "";
	std::unique_ptr<StreamWriter>          streamWriter = std::make_unique<StreamWriter>();
	std::unique_ptr<GLTFResourceWriter>    resourceWriter = nullptr;
	if (extension == "gltf")
	{
		resourceWriter = std::make_unique<GLTFResourceWriter>(std::move(streamWriter));
	}
	else if (extension == "glb")
	{
		resourceWriter = std::make_unique<GLBResourceWriter>(std::move(streamWriter));
	}
	else
	{
		throw gltf::detail::error::GLTFException("Invalid extension type.");
	}

	/*-------------------------------------------------------------------
	-                      Serialize
	---------------------------------------------------------------------*/
	try
	{
		manifest = Serialize(Document, flags);
	}
	catch (const detail::error::GLTFException& exception)
	{
		throw std::runtime_error(exception.what());
	}

	if (extension == "gltf")
	{
		resourceWriter->WriteExternal(filePath, manifest);
	}
	else if (extension == "glb")
	{
		GLBResourceWriter* writer = dynamic_cast<GLBResourceWriter*>(resourceWriter.get());
		writer->Flush(manifest, filePath);
	}
	else
	{
		throw gltf::detail::error::GLTFException("Invalid extension type.");
	}
	ResourceWriter = std::move(resourceWriter);
}