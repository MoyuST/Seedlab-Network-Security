# VPN Lab: The Container Version

## Task 2: Create and Configure TUN Interface

### Task 2.a: Name of the Interface

Just modify the command passing to the file descriptor, which is tun, then we could set the name.

---

![code modification](Assets/imgs/task2a_code_modification.png)

---

![result](Assets/imgs/task2a_set_customized_name.png)

---

### Task 2.b: Set up the TUN Interface

Modify the codes just as the instruction said, then we could get the expected result.

---

![code modification](Assets/imgs/task2b_code_modification.png)

---

![result](Assets/imgs/task2b_result.png)

---

### Task 2.c: Read from the TUN Interface

After modification, the program could print out the packets it received via tun interface.

---
**ping 192.168.53.00/24**

![result 1](Assets/imgs/task2c_result1.png)

---

**ping 192.168.60.00/24**

![result 2](Assets/imgs/task2c_result2.png)

---

from above results, we can find that ICMP packets to 192.168.53.00/24 would be successfully received while those from 192.168.60.00/24 would not. the reason is quite straightfore, since when do configuration for tun interface, we only use it to capture the packets in the 192.168.53.00/24 subnet. this is also reflected in the routing table:

![routing table](Assets/imgs/task2c_route.png)

---

### Task 2.d: Write to the TUN Interface

just follow the instructions and modify slightly, we could get the result.

---

spoof ICMP reply:

![code modification1](Assets/imgs/task2d_code_modification1.png)

![result1](Assets/imgs/task2d_result1.png)

---

wrting arbitrary data:

![code modification](Assets/imgs/task2d_code_modification2.png)

![resut2](Assets/imgs/task2d_result2.png)

---

## Task 3: Send the IP Packet to VPN Server Through a Tunnel

For the server part, we just create a file inside the volumes folder like the instruction said, and then executed.

---

**ping 192.168.53.0/24**

![code 1](Assets/imgs/task3_code1.png)
![result 1](Assets/imgs/task3_result1.png)

---

**ping 192.168.60.5/24 before adding new route rule**

![result 2](Assets/imgs/task3_result2.png)

---

**ping 192.168.60.5/24 after adding new route rule**

![result 3](Assets/imgs/task3_result3.png)

![code 3](Assets/imgs/task3_code3.png)

---

## Task 4: Set Up the VPN Server

this task is simply combine the former code of server and client

---

![code modification](Assets/imgs/task4_code_modification.png)

---

![result](Assets/imgs/task4_result.png)

---

## Task 5: Handling Traffic in Both Directions

i find something tricky in this task. it seems that TUN can only write the packet with valid ip within the TUN interface to the network stack. anyway, instead of writing the upd packet inside the network stack and send it out, we could directly send out upd packets using sockets.

---

![result](Assets/imgs/task5_result.png)

---

Notice that is the server part, I use a simple dict to make it kind of reasonable cause server could not dedicated to server one client. however, it is far more insufficient to use the inside packets' ips to denote their ports. i donnot further improve it cause it is not the most important issue in the lab

![client code](Assets/imgs/task5_client_codes.png)
![server code](Assets/imgs/task5_server_codes.png)

---

## Task 6: Tunnel-Breaking Experiment

---

if stopping client and server after connection, the telnet program will kind of stuck there

![break connection](Assets/imgs/task6_break.png)

---

after resume the connection, once hit the keyboard, the telnet program would then send all the command typed during the breaking of client and server program, and then resumed to work normally.

![reconnect](Assets/imgs/task6_reconnect.png)

---

## Task 7: Routing Experiment on Host V

after deleting the default routing rule, the ICMP reply would disappear immediately

![ICMP disappear after deleting default routing rule](Assets/imgs/task7_no_reply_after_delete_default.png)

---

then we could add a specific routing rule to allow 192.168.00.00/24 passing through eth0, but anyway, i guess deleting default rule is generally not a good idea for usage

![ICMP reply resumed after adding rule](Assets/imgs/task7_reply_after_adding_specific_rule.png)

---

## Task 8: VPN Between Private Networks

after setting the enviroment using `docker-compose -f docker-compose2.yml build; docker-compose -f docker-compose2.yml up`, we just need to modify the ip and setting up the route tables for server, clien, hostU and hostV then it will works properlly.

--- 

the setting for VPN client is changed, so we only need to modify its part like following:

![modify VPN client](Assets/imgs/task8_code_modification.png)

--- 

now we test ping from host U to host V, notice that the routing table is set in as the first few lines in each pane shown in the screenshot:

![ping from hostU to hostV](Assets/imgs/task8_U_to_V.png)

---

after the ip of client VPN is cached in my server VPN program, that is, the ip and ports stored in the simple dictionary in codes, then we could ping from hostV back to hostU

![ping both directions](Assets/imgs/task8_U_to_V_and_V_to_U.png)

---

## Task 9: Experiment with the TAP Interface

this task is just get some feeling about the tap, just follow the instruction and we will get the expected result

![tap result](Assets/imgs/task9_tap.png)