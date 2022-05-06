# BGP Exploration and Attack Lab

## Task 1: Stub Autonomous System

### Task 1.a: Understanding AS-155’s BGP Configuration


#### Task 1.a.1

it is easy to see that AS-155 peers with AS-2 (10.102.0.2)

#### Task 1.a.2

we can compare the result of several results:

---

first, disable all connected AS, that is, AS_2, AS_4 and AS_156, after a while, the route tables would shrink a lot:

![all AS disabled](Assets/imgs/task1a2_1.png)

---

only turn on **AS_4**
![only AS_4 turn on](Assets/imgs/task1a2_2.png)

---

only turn on **AS_2**
![only AS_2 turn on](Assets/imgs/task1a2_3.png)


### Task 1.b: Observing BGP UPDATE Messages

we only need to disable one peer of AS150 and enable it, and this would be sufficient enough to generate UPDATE message to withdraw and update. the reason is quite simple, since the best approach for AS150's neighbor to reach AS150 is access it through this direct bgp session. thus, once we disable bgp peering with the one neighbor, this neighbor would definitely have to adjust its best path to reach AS150 which is directly connecting to AS150.

---

operations in AS150:
![operations](Assets/imgs/task1a3_op.png)

---

withdraw message from AS2
![withdraw](Assets/imgs/task1a3_withdrawn.png)

---

update message from AS2

![update](Assets/imgs/task1a3_update.png)

### Task 1.c: Experimenting with Large Communities

to mimic service between AS156 and AS4 is broken, we could simply disable the bgp protocol in the AS156 side

![mimic service broken](Assets/imgs/task1c1_turndown_bgp.png)

---

change AS155's service to transit route to AS156
![AS155](Assets/imgs/task1c1_as155.png)

---

correspondingly, change AS156's settings to accept AS155
![AS156](Assets/imgs/task1c1_as156.png)

### Task 1.d: Configuring AS-180

first, we need to construct a bgp table in birdc and define several communitiy variables

![AS180 creates table](Assets/imgs/task1d1_AS2_set_up_bgp_table.png)

---

then, we could start to peer with AS171,
peer from AS180 to AS171
![peer from AS180 to AS171](Assets/imgs/task1d1_AS180_peer_with_AS171.png)
peer from AS171 to AS180
![peer from AS171 to AS180](Assets/imgs/task1d1_AS171_peer_with_AS180.png)

---

next, we could set up provider and customer relationship of AS180 and AS2 and AS3
AS180 accept AS2 and AS3's server
![AS180 accept AS2 and AS3's server](Assets/imgs/task1d1_AS180_accept_AS2_and_AS3_service.png)

---

also, we need to make sure AS2 and AS3 provide corresponding service to AS180
![AS2 provide service to AS180](Assets/imgs/task1d1_AS2_provide_service_to_AS180.png)
![AS3 provide service to AS180](Assets/imgs/task1d1_AS3_provide_service_to_AS180.png)

## Task 2: Transit Autonomous System

### Task 2.a: Experimenting with IBGP

in this task, we will check the behavior of the routing table of AS3 at IX103. we will ping from host 10.162.0.72(connect AS3 at IX103) to 10.164.0.71(connect AS3 at IX104). during the task, we will disable the ibgp3 which connecting AS3 at IX103 and at IX104. 

---

ping will stuck once disable ibgp3 at AS# at IX103
![ping stuck](Assets/imgs/task2a_ping.png)

---

actually, from the routing table at the AS3 at IX103, we could find that the route to the 10.164.0.0/24 will disappear

before disabling:
![before disabling](Assets/imgs/task2a_route_disable_before.png)

after disabling:
![after disabling](Assets/imgs/task2a_route_disable_after.png)

### Task 2.b: Experimenting with IGP

after disabling ospf1, the route of AS3 at IX103 would only contain information to all the subnets inside this IX103.
![route compare](Assets/imgs/task2b_route.png)

### Task 2.c: Configuring AS-5

---

![setting](Assets/imgs/task2c.png)
ibgp1 set the connection between AS5 IX101 with AS5 at IX103 while ibgp2 connect AS5 IX101 with AS5 at IX105

---
as mimic the peer configuration we done at task1, we could finish this task.
the following is the result from AS5 at IX101

![result](Assets/imgs/task2c_peer_set_up.png)

this task involing modification in many files, which i would not show screenshots here, however, you could check the files in the task2 folder.

---

as mimic the steps above, we could finish the task

![result](Assets/imgs/task2c_peer_set_up_with_as3.png)

like above, modification in files would not shown here, instead would be put in the folder task2

## Task 3: Path Selection

### Task 3.a.

in AS150, for 10.161.0.0/24, we can find there are 2 pathes available:
![all pathes](Assets/imgs/task3a_all_path.png)

these 2 pathes have same local preference but the first option has smaller AS path length, therefore, it is more preferable and we can see a * is put on the first line of it.

and it is indeed the chosen path passed to kernel

![selected path](Assets/imgs/task3a_chosen_path.png)


### Task 3.b.

basically, all necessary steps are shown in the section 8 of Prof. Du's book.

notice that, reconfigure is required to make modification take effect

---

for inbound traffic, we could lower the local preference for AS2

![modification](Assets/imgs/task3b_inbound_traffic.png)

before modification, we can find some route using AS2
![inbound before](Assets/imgs/task3b_before_change_preference.png)
after modification, we have all routes passing with AS3
![inbound before](Assets/imgs/task3b_after_change_preference.png)

---

for outbound traffic, we could prepend itself to route passing to AS2, like the followings:

![modification](Assets/imgs/task3b_outbound_traffic.png)

to check the result, we could check on the outside host, follow the book, i also checked the traffic on AS154

before modification, we can find it pass packets via AS2
![outbound before](Assets/imgs/task3b_outbound_original_path.png)
after modification, it choose another route
![outbound after](Assets/imgs/task3b_outbound_modified_path.png)

## Task 4: IP Anycast

after trying many host to ping 10.190.0.100, I found that they all ping to the 190host1. to better test the effect, I peer AS151 with AS190 directly at IX100.

![ping from AS151 to AS190](Assets/imgs/task4_AS151.png)
![ping from AS153 to AS190](Assets/imgs/task4_AS153.png)

## Task 5: BGP Prefix Attack

all the detailed operatoins are taught in section 12

### Task 5.a. Launching the Prefix Hijacking Attack from AS-161

attacker would conduct attack like the following in AS161
![modification](Assets/imgs/task5a_attacker_change.png)

before attack:
![before attack](Assets/imgs/task5a_before_attack.png)

after attack:
![after attack](Assets/imgs/task5a_after_attack.png)

---

### Task 5.b. Fighting Back from AS-154

to fight back, AS154 would need to advertise more specific ip:
![modification](Assets/imgs/task5b_fighting_attack_change.png)

after change, we have:
![fight back](Assets/imgs/task5b_fighting_attack.png)

---

### Task 5.c. Fixing the Problem at AS-3

for AS3, one of easy way to ban AS161's attack would simply is simply only allowing packet within 10.154.0.0/24 to go out from AS161

![modification](Assets/imgs/task5c_ban_illegal_packet.png)