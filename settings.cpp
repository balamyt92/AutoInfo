#include "settings.h"

Settings * Settings::instance = 0;
Destroyer Settings::destroyer;

Destroyer::~Destroyer()
{
    instance->sync();
    delete instance;
}

void Destroyer::inicialize(Settings *p)
{
    instance = p;
}

Settings *Settings::getInstance()
{
    if(!instance)
    {
        instance = new Settings();
        destroyer.inicialize(instance);
    }
    return instance;
}
