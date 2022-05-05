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
