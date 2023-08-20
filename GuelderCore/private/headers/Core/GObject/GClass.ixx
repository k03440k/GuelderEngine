export module GuelderEngine.Core.Types:GClass;

export namespace GuelderEngine
{
    namespace Core
    {
        /*
        * The base class, which must be inherited from other core classes
        * Currently it only for debugging purposes
        */
        template<class T>
        class GClass
        {
        public:
            GClass() = default;
            virtual ~GClass() = default;

            static const char* GGetClassName() { return typeid(T).name(); }
            static const char* GGetClassRawName() { return typeid(T).raw_name(); }
        private:

        };
    }
}