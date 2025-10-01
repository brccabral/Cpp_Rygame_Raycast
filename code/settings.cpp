#include "settings.hpp"

Settings *Settings::instance = nullptr;

Settings *Settings::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Settings();
    }
    return instance;
}

void Settings::Destroy()
{
    delete instance;
}
