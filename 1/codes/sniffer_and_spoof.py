#!/usr/bin/env python3

from types import new_class
from scapy.all import *

def spoof_pkt(pkt):
    if ICMP in pkt and pkt[ICMP].type == 8:
        print('forging data ', pkt[IP].dst, ' to ', pkt[IP].src)
        ip = IP(src=pkt[IP].dst, dst=pkt[IP].src, ihl=pkt[IP].ihl)
        ip.ttl = 111
        icmp = ICMP(type=0, id=pkt[ICMP].id, seq=pkt[ICMP].seq)

        if pkt.haslayer(Raw):
            data = pkt[Raw].load
            newpkt = ip/icmp/data
        else:
            newpkt = ip/icmp
        
        send(newpkt, verbose=0)

    
# br-8bdd88cc20ec is retrived from information of ipconfig
# iface = ['br-8bdd88cc20ec', 'enp0s3'] to sniff multiple NIC
pkt = sniff(iface='br-8bdd88cc20ec', filter='icmp and net 10.9.0.0/24', prn=spoof_pkt)