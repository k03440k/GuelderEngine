#include "../../../includes/GuelderEngine/Utils/ResourceManager.hpp"

#include <fstream>
#include <sstream>

namespace GuelderEngine
{
    namespace Utils
    {
        ResourceManager::ResourceManager(const std::string_view& executablePath) : 
            m_Path(executablePath.substr(0, executablePath.find_last_of("/\\")))
        {
            //const size_t found = executablePath.find_last_of("/\\");
            //m_Path = executablePath.substr(0, found);
        }
        const std::string ResourceManager::GetFileSource(const std::string_view& relativeFilePath) const
        {
            std::ifstream file;
            file.open(m_Path + "/" + relativeFilePath.data(), std::ios::in | std::ios::binary);
            //LOG("DEBUG " << m_Path + "/" + relativeFilePath << " DEBUG");
            Debug::Logger::Assert(file.is_open(), Debug::Logger::Format("ResourceManager::GetFileSource: cannot open file at location: ", relativeFilePath));
            std::stringstream source;
            source << file.rdbuf();

            file.close();

            return source.str();
        }
        /*const std::string ResourceManager::GetFileSource(const std::string& executablePath, const char* relativeFilePath)
        {
            std::ifstream file;
            file.open(executablePath + "/" + relativeFilePath, std::ios::in | std::ios::binary);

            ASSERT(file.is_open(), std::string(std::string("ResourceManager::GetFileSource: cannot open file at location: ") + std::string(relativeFilePath)).c_str());

            std::stringstream source;
            source << file.rdbuf();

            file.close();

            return source.str();
        }*/
        /*ResourceManager::ShaderProgram_ptr ResourceManager::LoadShaderProgram(const std::string_view& shaderProgramName, const std::string_view& vertexPath, const std::string_view& fragmentPath)
        {
            Debug::Logger::Assert(!GetFileSource(vertexPath).empty(), Debug::Logger::Format("ResourceManager::LoadShaders: vertex shader is empty, path: ", vertexPath));
            const std::string vertexSource = GetFileSource(vertexPath);

            Debug::Logger::Assert(!GetFileSource(fragmentPath).empty(), Debug::Logger::Format("ResourceManager::LoadShaders: vertex shader is empty, path: ", fragmentPath));
            const std::string fragmentSource = GetFileSource(fragmentPath);

            const ShaderProgram_ptr& newShader = std::make_shared<Renderer::ShaderProgram>(vertexSource, fragmentSource);

            Debug::Logger::Assert(newShader->IsCompiled(), "ResourceManager::LoadShaderProgram: cannot compile ShaderProgram");

            m_ShaderPrograms.emplace(shaderProgramName, newShader);

            return newShader;
        }
        ResourceManager::ShaderProgram_ptr ResourceManager::LoadShaderProgramByPreset(const ShadersPreset& presetToLoad)
        {
            m_ShaderPrograms.emplace("default", GetPresetShader().find(presetToLoad)->second);
            return GetPresetShader().find(presetToLoad)->second;
        }*/
        std::string ResourceManager::GetResourceRelativePath(const std::string& shaderName) const
        {
            const std::string allText = GetFileSource(resourcesPath);
            const Utils::ubyte foundAt = (Utils::ubyte)allText.find(shaderName);

            Debug::Logger::Assert(foundAt != std::string::npos, "ResourceManager::GetResourceRelativePath: cannot find a such variable name");

            const Utils::ushort foundName = (Utils::ushort)allText.find(shaderName);//finds index of first char of nameVar
            const Utils::ushort foundSemicon = (Utils::ushort)allText.substr(foundName, allText.size()).find_first_of(';');//finds the end of that line(; or semicon)

            const std::string foundLine = allText.substr(foundName, foundSemicon);//cutts all file string to only line

            const Utils::ushort first = (Utils::ushort)foundLine.find_first_of('\"') + 1/*finds first '"'*/;
            const Utils::ushort second = (Utils::ushort)foundLine.find_last_of('\"') - (Utils::ushort)foundLine.find_first_of('\"') - 1/*finds last '"'*/;

            const std::string shaderPath = foundLine.substr(first, second);

            return shaderPath;
        }
        /*const ResourceManager::PresetShaderMap ResourceManager::GetPresetShader() const
        {
            const PresetShaderMap toReturn =
            {
                {
                ShadersPreset::Default,
                std::make_shared<ShaderProgram>(GetFileSource(GetResourceRelativePath("vertex")), GetFileSource(GetResourceRelativePath("fragment")))
                }
            };
            return toReturn;
        }
        const ResourceManager::ShaderProgram_ptr& ResourceManager::GetShaderProgram(const std::string_view& name) const
        {
            const auto& found = m_ShaderPrograms.find(name.data());
            if (found == m_ShaderPrograms.end())
                GE_ELOG("ResourceManager::GetShaderProgram: cannot find shader, name: ", name);
            return found->second;
        }*/
    }
}