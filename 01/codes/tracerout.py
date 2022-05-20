from scapy.all import *

cnt = 0
dst = 'google.com'

for i in range(30):
    cnt += 1

    a = IP()
    a.dst = dst
    a.ttl = cnt
    b = ICMP()
    p = sr1(a/b, iface='enp0s3', verbose=0, timeout=1)

    # notice that some routers between may drop our ICMP packet and not reply
    # so we need to set timeout
    if(p==None):
        continue

    if(p[IP].src==a.dst):
        break

temp = IP(dst=dst)

print('totally ', cnt, ' routers between ', temp.src, ' to ', temp.dst, '(', dst, ')')