#pragma once

#define INHERIT_GClass(className) public virtual ::GuelderEngine::Core::GClass<className>

/*
* The base class, which must be inherited from other core classes
* Currently it only for debugging purposes
*/
namespace GuelderEngine
{
    namespace Core
    {
        template<class T>
        class GClass
        {
        public:
            GClass() = default;
            virtual ~GClass() = default;

            static const char* GetClassName() { return typeid(T).name(); }
            static const char* GetClassRawName() { return typeid(T).raw_name(); }
        private:

        };
    }
}