#pragma once

namespace GuelderEngine
{
    namespace Events
    {
        struct BaseEvent;
    }
}

//add to cmake
#define EVENT_STRUCT_TYPE(_type) static const EventType type = EventType::##_type;\
                                virtual const EventType GetType() const override { return type; }\
                                virtual const char* GetName() const override { return #_type; }

#define MAKE_STRUCT_EVENT(typeEvent) struct typeEvent ## Event : public ::GuelderEngine::Events::BaseEvent\
                                     {\
                                          typeEvent ## Event () = default;\
                                          EVENT_STRUCT_TYPE(##typeEvent)\
                                     }