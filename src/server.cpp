#include "server.h"

void server::setMaxConnections(int connectionsAllowed){
  this->connectionsAllowed = connectionsAllowed;
}

server::~server(){
  if(this->sockfd != -1){
    close(this->sockfd);
  }
}

server_tcp::server_tcp(){
  this->port = -1;
  this->host = NULL;
}
server_tcp::server_tcp(char* host, int port){
  this->port = port;
  this->host = host;
}
void server_tcp::runSysCallServer(){
  struct addrinfo hints, *serv_info;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  std::string temp = std::to_string(this->port);
  getaddrinfo(this->host, temp.c_str(), &hints, &serv_info);

  this->sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);

  if(bind(this->sockfd, serv_info->ai_addr, serv_info->ai_addrlen) == -1){
    perror("ERROR binding");
    exit(-1);
  }
  std::cout<<"Server accepting connections"<<std::endl;

  //plan on allowing more connections here with forks or a loop that has a timeout

  if(listen(this->sockfd, 1) == -1){
    perror("ERROR listening");
    exit(-1);
  }
  struct sockaddr_storage client_addr;
  socklen_t addr_size = sizeof(client_addr);
  int fd = accept(this->sockfd, (struct sockaddr* )&client_addr, &addr_size);
  if(fd == -1){
    perror("ERROR accepting connection");
    exit(-1);
  }

  if(fd != -1){
    char hoststr[NI_MAXHOST];
    char portstr[NI_MAXHOST];
    getnameinfo((struct sockaddr *)&client_addr, addr_size, hoststr, sizeof(hoststr), portstr, sizeof(portstr), NI_NUMERICHOST | NI_NUMERICSERV);
    std::cout<<"Server has connected with "<<hoststr<<":"<<portstr<<std::endl;
    this->processCommands(fd);
  }
  else{
    perror("ERROR in accepting client connection");
    exit(-1);
  }
  close(fd);
  close(this->sockfd);
  std::cout<<"Server is no longer active"<<std::endl;
}
void server_tcp::processCommands(int fd){
  std::string output;
  bool keepConnection = true;
  std::string command_str;
  char command[4096];
  int counter = 0;
  int bytes = 0;
  while(keepConnection && fd != -1){
    memset(&command[0], 0, 4096);
    bytes = recv(fd, command, 4096, 0);
    if(bytes == -1){
      perror("Error recieving message from client");
      break;
    }
    else if(counter > 10){
      std::cout<<"ERROR in connection - 10+ blank messages"<<std::endl;
      break;
    }
    else if(bytes == 0){
      counter++;
      continue;
    }
    else{
      output.clear();
      counter = 0;
      command_str = std::string(command);
      std::cout<<"-> "<<command_str<<std::endl;
      if(command_str == "q"){
        output = "...Goodbye...";
        keepConnection = false;
      }
      else{
        output = exec(command);
        std::cout<<output<<std::endl;
      }
      if(send(fd, output.c_str(), output.length(), 0) == -1){
        perror("ERROR sending message to client");
        exit(-1);
      }
    }
  }
}

server_udp::server_udp(){
  this->port = -1;
  this->host = NULL;
}
server_udp::server_udp(char* host, int port){
  this->port = port;
  this->host = host;
}
void server_udp::runSysCallServer(){
  struct addrinfo hints, *serv_info;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  std::string temp = std::to_string(this->port);
  getaddrinfo(this->host, temp.c_str(), &hints, &serv_info);

  this->sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);

  if(bind(this->sockfd, serv_info->ai_addr, serv_info->ai_addrlen) == -1){
    perror("ERROR binding");
    exit(-1);
  }
  std::cout<<"Server started"<<std::endl;

  this->processCommands();

  close(this->sockfd);
  std::cout<<"Server is no longer accepting connections"<<std::endl;
}
void server_udp::processCommands(){
  struct sockaddr_storage client_addr;
  socklen_t addr_size = sizeof(client_addr);
  std::string output;
  bool keepAlive = true;
  std::string command_str;
  char command[4096];
  int counter = 0;
  int bytes = -1;
  while(keepAlive){
    memset(&command[0], 0, 4096);
    bytes = recvfrom(this->sockfd, command, 4096, 0, (struct sockaddr*)&client_addr, &addr_size);
    if(bytes == -1){
      continue;
    }
    else if(counter > 10){
      std::cout<<"ERROR - 10+ blank messages"<<std::endl;
      break;
    }
    else if(bytes == 0){
      counter++;
      continue;
    }
    else{
      char hoststr[NI_MAXHOST];
      char portstr[NI_MAXHOST];
      counter = 0;
      getnameinfo((struct sockaddr *)&client_addr, addr_size, hoststr, sizeof(hoststr), portstr, sizeof(portstr), NI_NUMERICHOST | NI_NUMERICSERV);
      //std::string ip = inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr),hoststr,sizeof(hoststr));
      std::cout<<"Command recieved from "<<hoststr<<std::endl;

      command_str = std::string(command);
      std::cout<<"-> "<<command_str<<std::endl;
      if(command_str == "q"){
        output = "...Goodbye...";
        keepAlive = false;
      }
      else{
        output.clear();
        output = exec(command);
        std::cout<<output<<std::endl;
      }
      if(sendto(this->sockfd, output.c_str(), output.length(), 0,  (struct sockaddr*)&client_addr, addr_size) == -1){
        perror("ERROR sending message to client");
        exit(-1);
      }
    }
  }
}
