#include "BackEndServer.h"
#include <ArduinoJson.h>
#include "EEPROMHandler.h"
#include "LogSender.h"
#include "api/web_handlers.h"

IPAddress targetIP;
uint16_t targetPort = 0;

void initBackEndServerFromEEPROM() {
    char ipBuffer[16] = {0};
    uint16_t port = 0;

    if (loadEepromBackEndServerURL(ipBuffer, sizeof(ipBuffer), &port)) {
        IPAddress tmp;
        if (tmp.fromString(ipBuffer) && port > 0) {
            targetIP = tmp;
            targetPort = port;
            sendLogToServer("info", "Loaded BackEnd IP/Port from EEPROM: " + String(ipBuffer) + ":" + String(port));
            return;
        }
    }

    // fallback
    const char* fallbackIP = "192.168.121.22";
    const uint16_t fallbackPort = 5023;
    targetIP.fromString(fallbackIP);
    targetPort = fallbackPort;
    sendLogToServer("warn", "Fallback IP/Port used: " + String(fallbackIP) + ":" + String(fallbackPort));
}

void setBackEndServerURL() {
    if (!server.hasArg("plain")) {
        server.send(400, "application/json", "{\"error\":\"No JSON body\"}");
        sendLogToServer("error", "setBackEndServerURL: Missing JSON body");
        return;
    }

    JsonDocument doc;
    if (deserializeJson(doc, server.arg("plain"))) {
        server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        sendLogToServer("error", "setBackEndServerURL: Invalid JSON");
        return;
    }

    const char* ip = doc["ip"];
    uint16_t port = doc["port"] | 0;

    IPAddress parsed;
    if (!ip || !parsed.fromString(ip) || port == 0) {
        server.send(400, "application/json", "{\"error\":\"Bad ip/port\"}");
        sendLogToServer("error", "setBackEndServerURL: Bad ip/port");
        return;
    }

    saveEepromBackEndServer(ip, port);
    targetIP = parsed;
    targetPort = port;

    JsonDocument resp;
    resp["ok"] = true;
    resp["ip"] = targetIP.toString();
    resp["port"] = targetPort;

    String out; serializeJson(resp, out);
    sendLogToServer("info", "Back-end server URL updated to: " + targetIP.toString() + ":" + String(targetPort));
    server.send(200, "application/json", out);
}

void getBackEndServerURL() {
    char ipFromEEPROM[16] = {0};
    uint16_t portFromEEPROM = 0;

    JsonDocument doc;

    if (loadEepromBackEndServerURL(ipFromEEPROM, sizeof(ipFromEEPROM), &portFromEEPROM)) {
        doc["eeprom"]["ip"] = ipFromEEPROM;
        doc["eeprom"]["port"] = portFromEEPROM;
    } else {
        doc["eeprom"]["error"] = "Failed to load from EEPROM";
    }

    doc["current"]["ip"] = targetIP.toString();
    doc["current"]["port"] = targetPort;

    String response; serializeJson(doc, response);
    server.send(200, "application/json", response);
}
