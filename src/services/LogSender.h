#pragma once
#include <Arduino.h>

/**
 * Надсилає лог на бекенд-сервер (або кешує, якщо немає інтернету).
 * @param type — тип логування (info, error, warn)
 * @param message — повідомлення
 */
void sendLogToServer(const String& type, const String& message);
