#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/tcp.h>        // Provides declarations for tcp header
#include <netinet/ip.h>	        // Provides declarations for ip header
#include <arpa/inet.h>          // inet_addr 
#include <netinet/ether.h>         // Provides declarations for ip header
#include <netinet/ip_icmp.h>    // Provides declarations for icmp header
#include <sys/socket.h>
#include <string.h>

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

int sock;

void packet_handler(u_char *args, const struct pcap_pkthdr * header,
        const u_char * packet){

        // cast front part of the packet to ethheader
        struct ethhdr *eth = (struct ethhdr *) packet;
        if(ntohs(eth->h_proto) == 0x0800){
            struct iphdr * ip_header_ptr = (struct iphdr *) (packet+sizeof(struct ethhdr));
            struct in_addr source_addr;
            source_addr.s_addr = ip_header_ptr->saddr;

            printf("Packet from: %s ", inet_ntoa(source_addr));
            source_addr.s_addr = ip_header_ptr->daddr;
            printf("to: %s ", inet_ntoa(source_addr));

            if(ip_header_ptr->protocol == IPPROTO_TCP){
                printf("[TCP] ");
            }
            else if(ip_header_ptr->protocol == IPPROTO_UDP){
                printf("[UDP]\n");
            }
            else if(ip_header_ptr->protocol == IPPROTO_ICMP){
                printf("[ICMP]\n");

                // recalculate icmp checksum
                // struct icmphdr * icmp_header_ptr = (struct icmphdr *) (((char*) ip_header_ptr)+sizeof(struct iphdr));
                // unsigned short icmp_and_data_len = ntohs(ip_header_ptr->tot_len) - sizeof(struct iphdr);
                // icmp_header_ptr->checksum = 0;
                // unsigned short checksum_result = checksum(icmp_header_ptr, icmp_and_data_len);
                // icmp_header_ptr->checksum = htons(checksum_result);

                // reverse ip src and dst
                uint32_t temp = ip_header_ptr->saddr;
                ip_header_ptr->saddr = ip_header_ptr->daddr;
                ip_header_ptr->daddr = temp;

                ip_header_ptr->ttl = 123;

                // clean ip checksum field
                ip_header_ptr->check = 0;

                // send the raw socket out
                struct sockaddr_in dest_info;

                struct in_addr dest_ip_wrapper;
                dest_ip_wrapper.s_addr = ip_header_ptr->daddr;

                // destination information
                dest_info.sin_family = AF_INET;
                dest_info.sin_addr = dest_ip_wrapper;

                // send packet out
                printf("sending packet\n");
                if(sendto(sock, ip_header_ptr, ntohs(ip_header_ptr->tot_len), 0,
                    (struct sockaddr * ) &dest_info, sizeof(dest_info)
                )<0){
                        perror("PACKET NOT SEND\n");
                    }
                }
                else{
                    printf("[OTHERS]\n");
                }
            }
}

int main(){
    pcap_t * handler;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    char filter_exp[] = "icmp or tcp";
    bpf_u_int32 net;

    // create a raw socket
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    int enable = 1;

    // IP_HDRINCL tells OS that IP header is already included
    // enable = 1 means enable this option
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));


    // configure PCAP handler
    handler = pcap_open_live("br-8bdd88cc20ec", BUFSIZ, 1, 1000, errbuf);

    // translate and store filter expressed in filter_exp (human-readable) to fp (machine 
    // readable)
    pcap_compile(handler, &fp, filter_exp, 1, net);

    // set filter
    if(pcap_setfilter(handler, &fp)!=0){
        pcap_perror(handler, "Error:");
        exit(1);
    }

    // start looping and capture packets
    pcap_loop(handler, -1, packet_handler, NULL);

    pcap_close(handler);

    close(sock);

    return 0;
}
