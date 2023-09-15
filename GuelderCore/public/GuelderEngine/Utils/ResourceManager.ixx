module;
#include "../private/headers/Core/GObject/GClass.hpp"
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

        class ResourcesHelper
        {
        public:
            DELETE_COPY_AND_MOVING(ResourcesHelper);
            
            using vars = std::unordered_map<std::string, std::string>;

            /*
             *@brief Use only for resources.txt
             *@brief finds by the '=' symbol and if the var is not inside of '"'
             *@brief delcare all variables with "var" before the name
            */
            static vars FindAllVariables(const std::string& resSource);
        };
        class ResourceManager// : INHERIT_GClass(ResourceManager)
        {
        public:

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

            const std::string path;
        private:
            //variables from resources.txt
            const ResourcesHelper::vars m_Vars;
        };
    }
}