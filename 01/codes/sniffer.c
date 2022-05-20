#include "packet_struct.h"

void packet_handler(u_char *args, const struct pcap_pkthdr * header,
        const u_char * packet){

        // cast front part of the packet to ethheader
        struct ethheader *eth = (struct ethheader *) packet;
        if(ntohs(eth->ether_type) == 0x0800){
            struct ipheader * ip = (struct ipheader *) (packet+sizeof(struct ethheader));
            printf("Packet from: %s ", inet_ntoa(ip->iph_sourceip));
            printf("to: %s ", inet_ntoa(ip->iph_destip));

            if(ip->iph_protocol == IPPROTO_TCP){
                printf("[TCP] ");
                struct tcpheader * tcp = (struct tcpheader *) (packet+sizeof(struct ethheader)+sizeof(struct ipheader));
                u_char * payload = (u_char *) (packet+sizeof(struct ethheader)+sizeof(struct ipheader)+tcp->th_offx2/4);
                printf("payload|%s|\n", payload);
            }
            else if(ip->iph_protocol == IPPROTO_UDP){
                printf("[UDP]\n");
            }
            else if(ip->iph_protocol == IPPROTO_ICMP){
                printf("[ICMP]\n");
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

    // configure PCAP handler
    handler = pcap_open_live("br-8bdd88cc20ec", BUFSIZ, 1, 1000, errbuf);

    // translate and store filter expressed in filter_exp (human-readable) to fp (machine 
    // readable)
    pcap_compile(handler, &fp, filter_exp, 0, net);

    // set filter
    if(pcap_setfilter(handler, &fp)!=0){
        pcap_perror(handler, "Error:");
        exit(1);
    }

    // start looping and capture packets
    pcap_loop(handler, -1, packet_handler, NULL);

    pcap_close(handler);
    return 0;
}
