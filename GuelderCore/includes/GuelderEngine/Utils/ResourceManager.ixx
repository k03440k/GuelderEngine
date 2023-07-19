module;
#include "../src/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Core:ResourceManager;

import <string>;
import <string_view>;
import <unordered_map>;
import <memory>;
import <vector>;

export namespace GuelderEngine
{
    namespace Utils
    {
        extern constexpr char resourcesPath[] = "Resources/resources.txt";

        class ResourcesHelper : INHERIT_GClass(ResourcesHelper)
        {
        public:
            ResourcesHelper() = default;
            ~ResourcesHelper() = default;
            
            using vars = std::unordered_map<std::string, std::string>;

            /*
             *@brief Use only for resources.txt
             *@brief finds by the '=' symbol and if the var is not inside of '"'
             *@brief delcare all variables with "var" before the name
            */
            static vars FindAllVariables(const std::string& resSource);
        };
        class ResourceManager : INHERIT_GClass(ResourceManager)
        {
        public:
            //using ShaderProgram_ptr = std::shared_ptr<Renderer::ShaderProgram>;

            /*enum class ShadersPreset : Utils::ubyte
            {
                Default
            };*/

            //using PresetShaderMap = std::map<ResourceManager::ShadersPreset, ResourceManager::ShaderProgram_ptr>;

            ResourceManager(const std::string_view& executablePath);
            ~ResourceManager() = default;

            DELETE_COPY_AND_MOVING(ResourceManager);

            //get file string
            std::string GetRelativeFileSource(const std::string_view& relativeFilePath) const;
            static std::string GetFileSource(const std::string_view& filePath);
            /*
             *@brief Finds variable content, for example: "var i = "staff"; then it will return staff"
            */
            std::string_view FindResourcesVariableContent(const std::string_view& name) const;
            /*
             *@brief Finds file content by variable content in resources.txt
            */
            std::string FindResourcesVariableFileContent(const std::string_view& name) const;
            std::string GetFullPathToRelativeFile(const std::string_view& relativePath) const;
            std::string GetFullPathToRelativeFileViaVar(const std::string_view& varName) const;

            const ResourcesHelper::vars& GetRecourceVariables() const noexcept;
            //static const std::string GetFileSource(const std::string& executablePath, const char* relativeFilePath);

            //ShaderProgram_ptr LoadShaderProgram(const std::string_view& shaderProgramName, const std::string_view& vertexPath, const std::string_view& fragmentPath);
            //ShaderProgram_ptr LoadShaderProgramByPreset(const ShadersPreset& presetToLoad);

            /*
            * Gets default resource path in file "./Resources/resources.txt"
            */
            //std::string GetResourceRelativePath(const std::string & resourceName) const;

            //const PresetShaderMap GetPresetShader() const;

            //const ShaderProgram_ptr& GetShaderProgram(const std::string_view& name = "default") const;

            const std::string path;
        private:
            //variables from resources.txt
            const ResourcesHelper::vars m_Vars;
            //std::map<std::string, ShaderProgram_ptr> m_ShaderPrograms;

            //using ShaderProgram = Renderer::ShaderProgram;
        };
    }
}