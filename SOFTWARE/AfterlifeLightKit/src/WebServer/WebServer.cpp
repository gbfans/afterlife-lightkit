#include "WebServer.h"
#include "Settings.h"

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "ui/html_ui.h"

//#include <AsyncElegantOTA.h>

IPAddress apIP(192, 168, 4, 1);
AsyncWebServer asyncServer(80);

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

#ifndef APSSID
#define APSSID "GBF-LK-" + WiFi.macAddress()
#define APPSK  "ghostbusters"
#endif

// define flash strings once (saves flash memory)
static const char s_content_enc[]
PROGMEM = "Content-Encoding";

const char *PARAM_MESSAGE = "message";

WebServer::WebServer()
{

}

void WebServer::init()
{
    dnsServer.start(DNS_PORT, "*", apIP);

    asyncServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response;

        response = request->beginResponse_P(200, "text/html", PAGE_index, PAGE_index_L);
        response->addHeader(FPSTR(s_content_enc), "gzip");
        request->send(response);
    });

    // Send a GET request to <IP>/get?message=<message>
    asyncServer.on("/get", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE))
        {
            message = request->getParam(PARAM_MESSAGE)->value();
        }
        else
        {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);

        if (message == "wifioff")
        {
            // Turn off server
            this->end();
        }
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    asyncServer.on("/post", HTTP_POST, [](AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE, true))
        {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        }
        else
        {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

    asyncServer.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Not found");
    });
}

void WebServer::begin()
{
    if (_serverStarted)
    {
        // Already started
        end();
    }

    debugln(F("Starting Web Server"));
    _serverStarted = true;

    WiFi.setSleepMode(WIFI_NONE_SLEEP); //Put this at the top of the setup function.

    WiFi.mode(WIFI_AP);
    WiFi.softAP(APSSID, APPSK);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

    debug("WiFi SSID: ");
    debugln(APSSID);

    asyncServer.begin();
}

void WebServer::end()
{
    debugln(F("Stopping Web Server"));
    _serverStarted = false;

    asyncServer.end();
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);
}


bool WebServer::isStarted()
{
    return _serverStarted;
}

void WebServer::check()
{
    if (_serverStarted)
    {
        dnsServer.processNextRequest();
    }
}

