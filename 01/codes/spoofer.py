from scapy.all import *

# create IP header
a = IP()
a.dst = '10.9.0.5'

# set ICMP
b = ICMP()

# stack package
p = a/b

send(p)