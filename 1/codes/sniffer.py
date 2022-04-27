#!/usr/bin/env python3

from scapy.all import *

def print_pkt(pkt):
    pkt.show()

# br-8bdd88cc20ec is retrived from information of ipconfig
# iface = ['br-8bdd88cc20ec', 'enp0s3'] to sniff multiple NIC
pkt = sniff(iface='br-8bdd88cc20ec', filter='icmp', prn=print_pkt)