#ifndef COMMON_INCLUDES_H
#define COMMON_INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <set>
#include <map>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <memory>
#include <dirent.h>
#include <iterator>
#include <cfloat>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


typedef enum SocketType{
  unknown = -1,
  tcp = 0,
  udp = 1,
  icmp = 2,
  ospf = 3
} SocketType;

#endif /* COMMON_INCLUDES_H */
