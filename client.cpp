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
struct sockaddr_in	 servaddr;
string command = "";
unsigned int len;

struct list{
    char action;

    list(){
        action = 'i';
    }

    string create_packet(){
        string packet = "";
        packet += action;
        return packet;
    }
};

bool getList(char* buffer){
    if(buffer[0] != 'I'){
        cout<<"List Expected"<<endl;
        return false;
    }

    string msg(buffer);
    vector<int> sizes;
    string username = "";
    string size_u, num_cli;
    int size_user, num_clients;
    string users = "";
    string listed = " - ";

    msg.erase(0,1);

    num_cli = msg.substr(0,2);
    num_clients = stoi(num_cli);

    msg.erase(0,2);

    for(int i = 0; i < num_clients; i++){
        size_u = msg.substr(0,2);
        size_user = stoi(size_u);
        sizes.push_back(size_user);
        msg.erase(0,2);
    }

    for(int i = 0; i < num_clients; i++){
        username = msg.substr(0, sizes[i]);
        users += listed;
        users += username;
        users += "\n";
        msg.erase(0, sizes[i]);
    }
    cout<<users<<endl;
    return true;
}

void listenCommands(){
    char buffer[1000];
    bzero(buffer, 1000);
    int n;
    while(1){
        n = recvfrom(sockfd, buffer, 1000,
                     MSG_WAITALL, (struct sockaddr *) &servaddr,
                     &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);

        if(command == "--list"){
            getList(buffer);
            cout<<endl;
            bzero(buffer, 1000);
        }
        else{
            bzero(buffer, 1000);
        }
        command.clear();
    }
}



// Driver code
int main() {
    char buffer[MAXLINE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n;



    string msg = "";

    while(1){

        getline(cin, command);




        if( command == "--list"){
            list lst;
            msg = lst.create_packet();

            sendto(sockfd, msg.c_str(), msg.size(),
                   MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                   sizeof(servaddr));
            printf("Hello message sent.\n");


        }

        listenCommands();
    }

    close(sockfd);
    return 0;
}
