#include "Debug.h"

Debug* Debug::instance = nullptr;;

Debug::Debug()
{
}

Debug* Debug::GetInstance()
{
    if (instance == nullptr) {
        instance = new Debug();
    }
    
    return instance;
}