module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Core;
import :ResourceManager;

import <fstream>;
import <sstream>;
import <regex>;
import <unordered_map>;

import GuelderEngine.Debug;

namespace GuelderEngine
{
    namespace Utils
    {
        //bool ResourcesHelper::HasDesiredCharsBeforeIndex(const std::string_view& str, std::size_t index, const std::string_view& desiredChars)
        //{
        //    if(index == 0 || index >= str.length()) {
        //        return false;  // Index out of range or at the beginning of the string
        //    }

        //    for(std::size_t i = index - 1; i >= 0; --i) {
        //        if(desiredChars.find(str[i]) != std::string::npos) {
        //            return true;  // Found a desired character before the given index
        //        }
        //    }

        //    return false;  // No desired characters found before the given index
        //}
        ResourcesHelper::vars ResourcesHelper::FindAllVariables(const std::string& resSource)
        {
            vars varsMap;

            const std::regex pattern(R"||(\s*var\s+(\w+)\s*=\s*"(.+)"\s*;)||"); // Regular expression pattern
            std::smatch matches;

            auto it = resSource.cbegin();
            while(std::regex_search(it, resSource.cend(), matches, pattern)) {
                varsMap[matches[1]] = matches[2];
                it = matches.suffix().first;
            }

            return varsMap;
        }
        ResourceManager::ResourceManager(const std::string_view& executablePath) :
            path(executablePath.substr(0, executablePath.find_last_of("/\\"))),
            m_Vars(ResourcesHelper::FindAllVariables(GetRelativeFileSource(resourcesPath)))
        {
        }
        std::string ResourceManager::GetRelativeFileSource(const std::string_view& relativeFilePath) const
        {
            std::ifstream file;
            file.open(path + "/" + relativeFilePath.data(), std::ios::in | std::ios::binary);
            //LOG("DEBUG " << path + "/" + relativeFilePath << " DEBUG");
            GE_CORE_CLASS_ASSERT(file.is_open(), "cannot open file at location: ", path, '/', relativeFilePath);
            std::stringstream source;//istringstream or stringstream?
            source << file.rdbuf();

            file.close();

            return source.str();
        }
        std::string ResourceManager::GetFileSource(const std::string_view& filePath)
        {
            std::ifstream file;
            file.open(filePath.data(), std::ios::in | std::ios::binary);
            //LOG("DEBUG " << path + "/" + relativeFilePath << " DEBUG");
            GE_CORE_CLASS_ASSERT(file.is_open(), "cannot open file at location: ", filePath);
            std::stringstream source;//istringstream or stringstream?
            source << file.rdbuf();

            file.close();

            return source.str();
        }
        std::string_view ResourceManager::FindResourcesVariableContent(const std::string_view& name) const
        {
            const auto found = m_Vars.find(name.data());
            GE_CORE_CLASS_ASSERT(found != m_Vars.end(), "cannot find \"", name, "\" variable, in \"", path, '/', resourcesPath, "\"");
            return found->second;
        }
        std::string ResourceManager::FindResourcesVariableFileContent(const std::string_view& name) const
        {
            return GetRelativeFileSource(FindResourcesVariableContent(name));
        }
        std::string ResourceManager::GetFullPathToRelativeFile(const std::string_view& relativePath) const
        {
            std::string filePath(path);
            filePath.append(relativePath);
            return filePath;
        }
        std::string ResourceManager::GetFullPathToRelativeFileViaVar(const std::string_view& varName) const
        {
            return GetFullPathToRelativeFile(FindResourcesVariableContent(varName));
        }
        const ResourcesHelper::vars& ResourceManager::GetRecourceVariables() const noexcept
        {
            return m_Vars;
        }
        /*const std::string ResourceManager::GetRelativeFileSource(const std::string& executablePath, const char* relativeFilePath)
        {
            std::ifstream file;
            file.open(executablePath + "/" + relativeFilePath, std::ios::in | std::ios::binary);

            ASSERT(file.is_open(), std::string(std::string("ResourceManager::GetRelativeFileSource: cannot open file at location: ") + std::string(relativeFilePath)).c_str());

            std::stringstream source;
            source << file.rdbuf();

            file.close();

            return source.str();
        }*/
        /*ResourceManager::ShaderProgram_ptr ResourceManager::LoadShaderProgram(const std::string_view& shaderProgramName, const std::string_view& vertexPath, const std::string_view& fragmentPath)
        {
            Debug::Logger::Assert(!GetRelativeFileSource(vertexPath).empty(), Debug::Logger::Format("ResourceManager::LoadShaders: vertex shader is empty, path: ", vertexPath));
            const std::string vertexSource = GetRelativeFileSource(vertexPath);

            Debug::Logger::Assert(!GetRelativeFileSource(fragmentPath).empty(), Debug::Logger::Format("ResourceManager::LoadShaders: vertex shader is empty, path: ", fragmentPath));
            const std::string fragmentSource = GetRelativeFileSource(fragmentPath);

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
        //std::string ResourceManager::GetResourceRelativePath(const std::string& varName) const
        //{
        //    const std::string allText = GetRelativeFileSource(resourcesPath);
        //    const Types::uint foundAt = allText.find(varName);

        //    //bool didntFound = foundAt != std::string::npos || 

        //    GE_CORE_CLASS_ASSERT(foundAt != std::string::npos, "cannot find a ", varName, " variable name");

        //    const Types::uint foundName = (Types::ushort)allText.find(varName);//finds index of first char of nameVar
        //    const Types::uint foundSemicolon = (Types::ushort)allText.substr(foundName, allText.size()).find_first_of(';');//finds the end of that line(; or semicon)

        //    const std::string foundLine = allText.substr(foundName, foundSemicolon);//cuts all file string to only line

        //    const Types::uint first = static_cast<Types::ushort>(foundLine.find_first_of('\"')) + 1/*finds first '"'*/;
        //    const Types::uint second = static_cast<Types::ushort>(foundLine.find_last_of('\"')) - static_cast<Types::ushort>(foundLine.find_first_of('\"')) - 1/*finds last '"'*/;

        //    const std::string shaderPath = foundLine.substr(first, second);

        //    return shaderPath;
        //}
        /*const ResourceManager::PresetShaderMap ResourceManager::GetPresetShader() const
        {
            const PresetShaderMap toReturn =
            {
                {
                ShadersPreset::Default,
                std::make_shared<ShaderProgram>(GetRelativeFileSource(GetResourceRelativePath("vertex")), GetRelativeFileSource(GetResourceRelativePath("fragment")))
                }
            };
            return toReturn;
        }
        const ResourceManager::ShaderProgram_ptr& ResourceManager::GetShaderProgram(const std::string_view& name) const
        {
            const auto& found = m_ShaderPrograms.find(name.data());
            if (found == m_ShaderPrograms.end())
                GE_THROW("ResourceManager::GetShaderProgram: cannot find shader, name: ", name);
            return found->second;
        }*/
    }
}