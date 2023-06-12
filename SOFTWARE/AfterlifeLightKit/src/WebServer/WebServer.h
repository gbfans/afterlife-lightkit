#ifndef WebServer_h
#define WebServer_h

#include <ESPAsyncWebServer.h>

class WebServer
{
    public:
        // Constructor
        WebServer();

        // Initialize WebServer
        void init();

        // Start the WebServer and WiFi
        void begin();

        // Stop the WebServer and WiFi
        void end();

        // Run in main loop
        void check();

        // Is the server started?
        bool isStarted();

    private:
        bool _serverStarted = false;
};

#endif
