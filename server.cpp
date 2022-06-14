#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <utility>
#include <thread>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define PORT	 8080
#define MAXLINE 1024

int sockfd;
struct sockaddr_in servaddr, cliaddr;
unsigned int lenser, lencli;

vector<pair<int,vector<string>>> clients;

string format_numbers_server(int range,unsigned long number){
    string numb = to_string(number);
    int comp = range-numb.size();
    if(numb.size() < range){
        for(int i=0;i<comp;i++){
            numb.insert(0,"0");
        }
    }
    return numb;
}

struct validateLogin{
    char action;
    char validate[2];

    validateLogin(){
        action = 'L';
        strcpy(validate, "k");
    }

    string create_packet(){
        string packet = "";

        packet += action;
        packet += validate;

        return packet;
    }
};

struct list{
    char action;
    char username;

    list(){
        action = 'I';
    }

    string create_packet(){
        string packet = "";
        string clientsNumber = format_numbers_server(2, clients.size());

        packet += action;
        packet += clientsNumber;

        string userNames = "";

        for(int i = 0; i < clients.size(); i++){
            userNames += clients[i].second[0];
            packet += format_numbers_server(2, clients[i].second[0].size());
        }
        packet += userNames;

        return packet;
    }
};

//void listenConnections(int ConnectFD){
//    int n,t;
//    char big_buffer[1000];
//    while(1){
//        bzero(big_buffer,1000);
//        n = read(ConnectFD,big_buffer,1000);
//        if (n < 0){
//            cout<<"-----------Error reading from Socket---------------"<<endl;
//            break;
//        }
//        string user = getUser(ConnectFD);
//
//        if(big_buffer[0] == 'i'){
//            list users;
//            msg = users.create_packet();
//            t = write(ConnectFD,msg.c_str(),msg.size());
//            if(t < 0){
//                cout<<"-----------Didn't send a paket-------------"<<endl;
//                break;
//            }
//            bzero(big_buffer,1000);
//            msg.clear();
//        }
//        else{
//            errorManager(ConnectFD,"Invalid command");
//            bzero(big_buffer,1000);
//            msg.clear();
//        }
//    }
//}

// Driver code
int main() {

    char buffer[MAXLINE];
    char *hello = "Hello from server";


    // Creating socket file descriptor

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address

    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    int n;

    lencli = sizeof(cliaddr); //len is value/resuslt
    lenser = sizeof(servaddr); //len is value/resuslt

    validateLogin k;
    string msg = k.create_packet();
    char big_buffer[1000];

    while(1){
        bzero(big_buffer,1000);
        n = recvfrom(sockfd, big_buffer, 1000,
                     MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                     &lencli);
        buffer[n] = '\0';
        printf("Client : %s\n", buffer);


        if(big_buffer[0] == 'i'){
            list users;
            msg = users.create_packet();
            sendto(sockfd, msg.c_str(), msg.size(),
                   MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                   lencli);
            printf("Hello message sent.\n");

            bzero(big_buffer,1000);
            msg.clear();
        }
    }

    return 0;
}
