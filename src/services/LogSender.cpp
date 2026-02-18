#include <HTTPClient.h>
#include <WiFi.h>
#include "EEPROMHandler.h"
#include "BackEndServer.h"

#define MAX_PENDING_LOGS 20
#define PATH_GENERATOR_SEND_LOG "/api/logs"

struct LogEntry { String type; String message; };

static LogEntry logQueue[MAX_PENDING_LOGS];
static int logQueueSize = 0;

static void queueLog(const String& type, const String& message) {
    if (logQueueSize < MAX_PENDING_LOGS) {
        logQueue[logQueueSize++] = { type, message };
    }
}

static bool sendSingleLog(const IPAddress& ip, uint16_t port, const String& type, const String& message) {
    String url = "http://" + ip.toString() + ":" + String(port) + PATH_GENERATOR_SEND_LOG;

    HTTPClient http;
    http.setTimeout(2000);                 // короткий таймаут
    if (!http.begin(url)) return false;
    http.addHeader("Content-Type", "application/json");

    String safeMessage = message; safeMessage.replace("\"", "\\\"");
    String payload = "{\"type\":\"" + type + "\",\"message\":\"" + safeMessage + "\"}";

    int rc = http.POST(payload);
    http.end();
    return (rc > 0 && rc < 400);
}

void sendLogToServer(const String& type, const String& message) {
    if (WiFi.status() != WL_CONNECTED) { queueLog(type, message); return; }

    // спроба надіслати поточний
    if (!sendSingleLog(targetIP, targetPort, type, message)) {
        queueLog(type, message);
        return;
    }

    // спроба злити чергу; якщо якась відправка впала — перериваємо (не втрачаємо)
    int i = 0;
    while (i < logQueueSize) {
        if (!sendSingleLog(targetIP, targetPort, logQueue[i].type, logQueue[i].message)) break;
        ++i;
        delay(30);
    }

    // зсунемо невідправлене на початок
    if (i < logQueueSize) {
        const int remain = logQueueSize - i;
        for (int k = 0; k < remain; ++k) logQueue[k] = logQueue[i + k];
        logQueueSize = remain;
    } else {
        logQueueSize = 0;
    }
}
