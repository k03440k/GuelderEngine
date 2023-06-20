module;
#include "../src/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Core:ResourceManager;

import <string>;
import <string_view>;
import <map>;
import <memory>;

export namespace GuelderEngine
{
    namespace Utils
    {
        class ResourceManager : INHERIT_GClass(ResourceManager)
        {
        public:
            //using ShaderProgram_ptr = std::shared_ptr<Renderer::ShaderProgram>;

            /*enum class ShadersPreset : Utils::ubyte
            {
                Default
            };*/

            //using PresetShaderMap = std::map<ResourceManager::ShadersPreset, ResourceManager::ShaderProgram_ptr>;

            ResourceManager(const std::string_view & executablePath);
            ~ResourceManager() = default;

            ResourceManager(const ResourceManager&) = delete;
            ResourceManager(const ResourceManager&&) = delete;
            ResourceManager& operator=(const ResourceManager&) = delete;
            ResourceManager& operator=(const ResourceManager&&) = delete;

            //get file string
            std::string GetFileSource(const std::string_view & relativeFilePath) const;
            //static const std::string GetFileSource(const std::string& executablePath, const char* relativeFilePath);

            //ShaderProgram_ptr LoadShaderProgram(const std::string_view& shaderProgramName, const std::string_view& vertexPath, const std::string_view& fragmentPath);
            //ShaderProgram_ptr LoadShaderProgramByPreset(const ShadersPreset& presetToLoad);

            static constexpr char resourcesPath[] = "./Resources/resources.txt";

            /*
            * Gets default resource path in file "./Resources/resources.txt"
            */
            std::string GetResourceRelativePath(const std::string & resourceName) const;

            //const PresetShaderMap GetPresetShader() const;

            //const ShaderProgram_ptr& GetShaderProgram(const std::string_view& name = "default") const;

        private:
            const std::string m_Path;

            //std::map<std::string, ShaderProgram_ptr> m_ShaderPrograms;

            //using ShaderProgram = Renderer::ShaderProgram;
        };
    }
}