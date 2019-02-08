#ifndef IO_UTIL_H
#define IO_UTIL_H

#include "common_includes.h"
#include "pstream.h"

std::string exec(const char* command);
bool ipChecker(const char* ipAddress);
void parseNetworkArgs(int numArgs, char* args[], char* &host, int &port, bool &isServer, SocketType &type);
std::string keyboardInput();



#endif /* IO_UTIL_H */
