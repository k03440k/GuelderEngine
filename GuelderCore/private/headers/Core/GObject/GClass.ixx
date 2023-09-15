//export module GuelderEngine.Core.Types:GClass;
//
//import <typeinfo>;
//
//export namespace GuelderEngine
//{
//    const char* GetClassName();
//    /*
//    * The base class, which must be inherited from other core classes
//    * Currently it only for debugging purposes
//    */
//    template<class T>
//    class GClass
//    {
//    public:
//        GClass() = default;
//        virtual ~GClass() = default;
//        
//        //TODO: maybe delete this class and add to macroses, with CORE_CLASS, when printing, typeid(*this) and so this class then is uselesss
//
//        using Type = T;
//
//    protected:
//        friend const char* GetClassName();
//
//        static const char* GGetClassName() { return typeid(Type).name(); }//TODO: find better way of getting class name
//        static const char* GGetClassRawName() { return typeid(Type).raw_name(); }
//    };
//}