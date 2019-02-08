#include "client.h"


client::~client(){
  if(this->sockfd != -1){
    close(this->sockfd);
  }
}
client_tcp::client_tcp(){
  this->host = NULL;
  this->port = -1;
  this->active = false;
  this->sockfd = -1;
}
client_tcp::client_tcp(char* host, int port){
  this->host = host;
  this->port = port;

  struct addrinfo hints, *serv_info;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  std::string temp = std::to_string(this->port);
  getaddrinfo(this->host, temp.c_str(), &hints, &serv_info);

  this->sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);
  if(this->sockfd == -1){
    perror("ERROR creating socket");
    exit(-1);
  }
  if(connect(this->sockfd, serv_info->ai_addr, serv_info->ai_addrlen)!= -1){
    this->active = true;
    std::cout<<"Connected to server "<<this->host<<" successfully"<<std::endl;
  }
  else{
    perror("Connection error");
    exit(-1);
  }
}
void client_tcp::executeSysCall(std::string command){
  if(!this->active){
    std::cout<<"ERROR no connection to server has been estabilished"<<std::endl;
  }
  char output[4096];
  memset(&output[0], 0, 4096);

  if(send(this->sockfd, command.c_str(), command.length(), 0) != -1){
    if(recv(this->sockfd, output, 4096, 0) == -1){
      perror("ERROR recieving message from server");
      exit(-1);
    }
    std::cout<<output<<std::endl;
  }
  else{
    perror("ERROR sending command");
    exit(-1);
  }
  if(command == "q"){
    close(this->sockfd);
    this->active = false;
    std::cout<<"Connection has been closed gracefully"<<std::endl;
  }
}

client_udp::client_udp(){
  this->host = NULL;
  this->port = -1;
  this->active = false;
  this->sockfd = -1;
}
client_udp::client_udp(char* host, int port){
  this->host = host;
  this->port = port;

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  std::string temp = std::to_string(this->port);
  getaddrinfo(this->host, temp.c_str(), &hints, &(this->serv_info));

  this->sockfd = socket(this->serv_info->ai_family, this->serv_info->ai_socktype, this->serv_info->ai_protocol);
  if(this->sockfd == -1){
    perror("ERROR creating socket");
    exit(-1);
  }

  this->active = true;

}
void client_udp::executeSysCall(std::string command){
  char output[4096];
  memset(&output[0], 0, 4096);

  if(sendto(this->sockfd, command.c_str(), command.length(), 0, this->serv_info->ai_addr, this->serv_info->ai_addrlen) != -1){
    if(recvfrom(this->sockfd, output, 4096, 0, this->serv_info->ai_addr, &(this->serv_info->ai_addrlen)) == -1){
      perror("ERROR recieving message from server");
      exit(-1);
    }
    std::cout<<output<<std::endl;
  }
  else{
    perror("ERROR sending command");
    exit(-1);
  }
  if(command == "q"){
    close(this->sockfd);
    this->active = false;
    std::cout<<"Client deactivated gracefully"<<std::endl;
  }
}
