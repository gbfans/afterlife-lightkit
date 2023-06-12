#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "ui/html_ui.h"

//#include <AsyncElegantOTA.h>

IPAddress apIP(192, 168, 4, 1);
AsyncWebServer server(80);

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

#ifndef APSSID
#define APSSID "GBF-LK-" + WiFi.macAddress()
#define APPSK  "ghostbusters"
#endif

// define flash strings once (saves flash memory)
static const char s_content_enc[] PROGMEM = "Content-Encoding";

const char* PARAM_MESSAGE = "message";

#include "Settings.h"
#include "Lights/Lights.h"
#include "GBFansControl/GBFansControl.h"

MODES mode;
Lights lights;
GBFansControl controls;

bool serverStarted = false;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void stopWebServer() {
    debugln(F("Stopping Web Server"));
    serverStarted = false;

    server.end();
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);
}

void startWebServer()
{
    if (serverStarted) {
        // Already started
        stopWebServer();
    }

    debugln(F("Starting Web Server"));
    serverStarted = true;

    WiFi.setSleepMode(WIFI_NONE_SLEEP); //Put this at the top of the setup function.

    WiFi.mode(WIFI_AP);
    WiFi.softAP(APSSID, APPSK);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

    debug("WiFi SSID: ");
    debugln(APSSID);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncWebServerResponse *response;

        response = request->beginResponse_P(200, "text/html", PAGE_index, PAGE_index_L);
        response->addHeader(FPSTR(s_content_enc),"gzip");
        request->send(response);
    });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
            if (message == "idle")
            {
                lights.setState(IDLE);
            } else if (message == "wifioff") {
                // Turn off server
                stopWebServer();
            } else {
                lights.setState(INACTIVE);
            }
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

    server.onNotFound(notFound);
    server.begin();
}

void setup()
{
    Serial.begin(115200);
    debugln(F("Starting up - This should only appear once"));

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    /**
     * Output Configuration
     */
    pinMode(SHIFT_PIN, OUTPUT); //For AJ's Logic level Shifter, this probably has to be initialized before the buttons.
    digitalWrite(SHIFT_PIN, HIGH);

    controls.init();

    lights.init();

    // Off by default
    lights.setState(INACTIVE);
    lights.update(true);

    dnsServer.start(DNS_PORT, "*", apIP);
}

void loop()
{
    controls.update();

    if (controls.isSequenceMatch("1:14:1:14:1:14:0")) {
        // We look for a sequence of "Startup => Powerdown" 3 times, which ends in "Off"
        // This can be triggered by switching the "Activate" switch On/Off 3 times.
        // (There shouldn't be a long enough pause between switches for the pack to finish Startup
        // and enter "Idle" State).
        if (serverStarted) {
            stopWebServer();
        } else {
            startWebServer();
        }
    }

    /**
     * NOTE: It is safe to keep setting the State over again here
     * as lights.setState() includes a check to prevent changing to the same State
     */
    if (controls.isState(STATE_OFF))
    {
        // Powered off
        //debugln(F("Off"));
        lights.setState(INACTIVE);
    }
    else if (controls.isState(STATE_POWER_UP))
    {
        // Starting up
        //debugln(F("Startup"));
        lights.setState(START);
    }
    else if (controls.isState(STATE_IDLE))
    {
        // Idling
        //debugln(F("Idle"));
        lights.setState(IDLE);
    }
    else if (
            controls.isState(STATE_FIRE_MOVIE) ||
            controls.isState(STATE_FIRE_TVG)
    ) {
        // Firing
        // NOTE: For the Pack Lights there is functionally no difference between MOVIE and TVG mode firing
        // TODO: Remove the duration check!
        if (controls.duration() > 5000)
        {
            // We have been firing for > 5 seconds
            //debugln(F("Overheating"));
            lights.setState(OVERHEATING);
        }
        else
        {
            //debugln(F("Firing"));
            lights.setState(FIRING);
        }
    }
    else if (controls.isState(STATE_POWER_DOWN))
    {
        //debugln(F("Powering Down"));
        lights.setState(SHUTDOWN);
    }

    // We should always update Lights AFTER the State has been set
    lights.update();

    if (serverStarted)
    {
        dnsServer.processNextRequest();
    }
}
