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
    }
}