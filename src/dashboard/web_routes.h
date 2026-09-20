#ifndef POMODORO_WEB_ROUTES_H
#define POMODORO_WEB_ROUTES_H

#include <WebServer.h>

extern WebServer server;

void setupRoutes();

void handleRoot();
void handleToggle();
void handleReset();
void handleSkip();
void handleMode();
void handleCustomTimer();
void handleStatus();

#endif
