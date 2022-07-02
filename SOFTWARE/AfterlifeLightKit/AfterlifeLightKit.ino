#include "ConfigManager.h"
#include "Lights.h"

ConfigManager config;

MODES mode;
Lights lights;

void setup()
{
    Serial.begin(115200);
    config.init();
    lights.init(config);
}

void loop()
{
    lights.update();
}
