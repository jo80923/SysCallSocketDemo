#include "io_util.h"

std::string exec(const char* command) {
  // run a process and create a streambuf that reads its stdout and stderr
  std::string output;
  redi::ipstream proc(command, redi::pstreams::pstdout | redi::pstreams::pstderr);
  std::string line;
  // read child's stdout
  while (std::getline(proc.out(), line)){
    output += (line + "\n");
  }
  // read child's stderr
  while (std::getline(proc.err(), line)){
    output += (line + "\n");
  }
  return output;
}

bool ipChecker(const char* ipAddress){
  struct sockaddr_in input;
  if(inet_pton(AF_INET, ipAddress, &(input.sin_addr)) || inet_pton(AF_INET6, ipAddress, &(input.sin_addr))){
    return true;
  }
  return false;
}

void parseNetworkArgs(int numArgs, char* args[], char* &host, int &port, bool &isServer, SocketType &type){
  if(numArgs < 3 || numArgs > 5){
    std::cout<<"Usage = ./SocketDemo <ip> <port> <-s or -c(server or client)> <optional - tcp or udp, defualt tcp>"<<std::endl;
    exit(-1);
  }
  std::string temp;

  host = args[1];
  temp = host;
  if(!ipChecker(temp.c_str())){
    std::cout<<"ERROR invalid IP address"<<std::endl;
    std::cout<<"Usage = ./SocketDemo <ip> <port> <-s or -c(server or client)> <optional - tcp or udp, defualt tcp>"<<std::endl;
    exit(-1);
  }


  port = std::stoi(args[2]);
  if(port < 1024 && port > 49151){
    std::cout<<"ERROR invalid port number"<<std::endl;
    std::cout<<"Usage = ./SocketDemo <ip> <port> <-s or -c(server or client)> <optional - tcp or udp, defualt tcp>"<<std::endl;
    exit(-1);
  }

  if(numArgs > 3){
    temp = args[3];
    if(temp == "-s" || temp == "-S"){
      isServer = true;
    }
    else if(temp == "-c" || temp == "-C"){
      isServer = false;
    }
    else{
      std::cout<<"Usage = ./SocketDemo <ip> <port> <-s or -c(server or client)> <optional - tcp or udp, defualt tcp>"<<std::endl;
      exit(-1);
    }
  }
  if(numArgs == 5){
    temp = args[4];
    std::transform(temp.begin(), temp.end(),temp.begin(), ::toupper);
    if(temp == "TCP"){
      type = tcp;
    }
    else if(temp == "UDP"){
      type = udp;
    }
    else{
      std::cout<<"ERROR unsupported socket type requested: "<<temp<<std::endl;
      std::cout<<"Usage = ./SocketDemo <ip> <port> <-s or -c(server or client)> <optional - tcp or udp, defualt tcp>"<<std::endl;
      exit(-1);
    }
  }
}

std::string keyboardInput(){
  std::string input;
  std::cout<<"-> ";
  getline(std::cin, input);
  return input;
}
