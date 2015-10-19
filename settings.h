#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include "constants.h"

class Settings;

class Destroyer
{
private:
    Settings * instance;
public:
    ~Destroyer();
    void inicialize(Settings * p);
};

class Settings : public QSettings
{
private:
    static Settings* instance;
    static Destroyer destroyer;
protected:
    Settings() : QSettings(CONFIG_FILE, QSettings::IniFormat)   { }

    Settings (const Settings&);
    Settings& operator=(Settings&);
    ~Settings(){}
    friend class Destroyer;

public:
    static Settings *getInstance();
    QString getBaseType();
};

#endif // SETTINGS_H
