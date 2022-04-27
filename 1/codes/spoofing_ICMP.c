#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/tcp.h>  // Provides declarations for tcp header
#include <netinet/ip.h>	  // Provides declarations for ip header
#include <arpa/inet.h>    // inet_addr 
#include <netinet/udp.h>  // Provides declarations for udp header
#include <netinet/ip.h>   // Provides declarations for ip header
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <string.h>

#define PACKET_LEN 1024

unsigned short checksum(void *b, int len){    
    unsigned short *buf = b;
    unsigned int sum=0;
    unsigned short result;
  
    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    printf("%d\n", result);
    return ntohs(result);
}

int main(){
    
    // create a packet
    char buffer[PACKET_LEN];
    memset(buffer, 0, PACKET_LEN);

    struct iphdr * ip_header_ptr = (struct iphdr *) buffer;
    struct icmphdr * icmp_header_ptr = (struct icmphdr *) (buffer+sizeof(struct iphdr));
    char * data_payload = (char *) (buffer+sizeof(struct iphdr)+sizeof(struct icmphdr));

    // set up data payload
    char *msg = "123456789";
    int data_len = strlen(msg);
    strcpy(data_payload, msg);
    
    // set up icmp header
    icmp_header_ptr->code = 0;
    icmp_header_ptr->type = 8;
    icmp_header_ptr->un.echo.id = htons(10);
    icmp_header_ptr->un.echo.sequence = htons(1);
    
    short checksum_result = checksum(icmp_header_ptr, sizeof(struct icmphdr)+data_len);
    icmp_header_ptr->checksum = htons(checksum_result);

    // set up ip headear
    ip_header_ptr->version = 4;
    ip_header_ptr->ihl = 5;
    ip_header_ptr->ttl = 20;
    ip_header_ptr->saddr = inet_addr("10.9.0.2");
    ip_header_ptr->daddr = inet_addr("172.217.24.228");
    ip_header_ptr->protocol = 1;
    ip_header_ptr->tot_len = htons(sizeof(struct iphdr)+sizeof(struct icmphdr)+data_len);

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
