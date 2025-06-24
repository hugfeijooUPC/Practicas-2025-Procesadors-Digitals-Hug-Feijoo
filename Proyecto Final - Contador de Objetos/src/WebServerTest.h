#ifndef WEBSERVERTEST_H
#define WEBSERVERTEST_H

#include "WebServerManager.h"

void setupWebServerTest(WebServerManager& server, int* counterPtr);
void loopWebServerTest(WebServerManager& server);

#endif // WEBSERVERTEST_H