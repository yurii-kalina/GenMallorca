#include "http_json.h"
#include "web_server.h"


void sendJson(int httpCode, const std::function<void(JsonDocument&)>& fill) {
  JsonDocument store;
  JsonDocument d = store;

  fill(d);

  String out;
  serializeJson(d, out);
  server.send(httpCode, "application/json", out);
}
