#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/tcp.h>  // Provides declarations for tcp header
#include <netinet/ip.h>	  // Provides declarations for ip header
#include <arpa/inet.h>    // inet_addr 
#include <netinet/udp.h>  // Provides declarations for udp header
#include <netinet/ip.h>   // Provides declarations for ip header
#include <sys/socket.h>
#include <string.h>

#define PACKET_LEN 1024

int main(){
    
    // create a packet
    char buffer[PACKET_LEN];
    memset(buffer, 0, PACKET_LEN);

    struct iphdr * ip_header_ptr = (struct iphdr *) buffer;
    struct udphdr * udp_header_ptr = (struct udphdr *) (buffer + sizeof(struct iphdr));
    char * data_payload = (char *) (buffer+sizeof(struct iphdr)+sizeof(struct udphdr));

    // set up data payload
    char *msg = "test 1\n";
    int data_len = strlen(msg);
    strcpy(data_payload, msg);
    
    // set up udp header
    udp_header_ptr->uh_dport = htons(9999);
    udp_header_ptr->uh_sport = htons(1234);
    udp_header_ptr->len = htons(sizeof(struct udphdr)+data_len);
    // udp checksum is not mandatory
    // set to 0 to tell receiver no need to check
    udp_header_ptr->check = 0;

    // set up ip headear
    ip_header_ptr->version = 4;
    ip_header_ptr->ihl = 5;
    ip_header_ptr->ttl = 20;
    ip_header_ptr->saddr = inet_addr("10.9.0.1");
    ip_header_ptr->daddr = inet_addr("10.9.0.5");
    ip_header_ptr->protocol = IPPROTO_UDP;
    ip_header_ptr->tot_len = htons(sizeof(struct iphdr)+sizeof(struct udphdr)+data_len);
    // set ip check sum to 0 to ask OS to calculate it
    ip_header_ptr->check = 0;


    // send the raw socket out
    
    struct sockaddr_in dest_info;
    int enable = 1;

    // create a raw socket
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    // IP_HDRINCL tells OS that IP header is already included
    // enable = 1 means enable this option
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));

    struct in_addr dest_ip_wrapper;
    dest_ip_wrapper.s_addr = ip_header_ptr->daddr;

    // destination information
    dest_info.sin_family = AF_INET;
    dest_info.sin_addr = dest_ip_wrapper;

    // send packet out
    printf("sending packet\n");
    if(sendto(sock, buffer, ntohs(ip_header_ptr->tot_len), 0,
        (struct sockaddr * ) &dest_info, sizeof(dest_info)
    )<0){
        perror("PACKET NOT SEND\n");
        return EXIT_FAILURE;
    }

    close(sock);
    return EXIT_SUCCESS;
}
