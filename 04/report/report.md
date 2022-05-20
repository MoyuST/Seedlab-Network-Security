# TCP Attack

## Task 1: SYN Flooding Attack

<br>

### 1.1: Launching the Attack Using Python

<br>

**Notice** that you need to flush the tcp metrics in victim by "ip tcp_metrics flush", 
otherwise the connection could still be established from user in victim during SYN flooding  

**code**  
![code](./Assets/imgs/task1_1_codes.png)

<br>

**victim**  
![victim](./Assets/imgs/task1_1_victim.png)

<br>

**user1**  
![user1](./Assets/imgs/task1_1_user1.png)

### 1.2: Launching the Attack Using C

**code**  
![attacker](./Assets/imgs/task1_2_attacker.png)

<br>

**victim**  
![victim](./Assets/imgs/task1_2_victim.png)

<br>

**user1**  
![user1](./Assets/imgs/task1_2_user1.png)

comparing with using python the speed of using C is defintely faster.

### 1.3: Victim turn on SYNCookies

First, let's turn on the syncookies setting.

![turn on](Assets/imgs/task1_3_turn_on_SYNCookies.png)  

Then flush the tcp metrics in victim by "ip tcp_metrics flush" and start attacker.  

As we can see, victim's ip table is already full.  
![victim](Assets/imgs/task1_3_victim.png)  

However, the user1 could still connect to victim.  
![user1](Assets/imgs/task1_3_user1.png)

## Task 2: TCP RST Attacks on telnet Connections

codes:  
![codes](./Assets/imgs/task2_codes.png)

<br>
After we run the program on attacker, as long as we type anything (except enter) the connection will be reset automatically.  
![result](./Assets/imgs/task2_result.png)

## Task 3: TCP Session Hijacking

codes

![codes](./Assets/imgs/task3_codes.png)

![hijacking](./Assets/imgs/task3_hijacking.png)

![victim](./Assets/imgs/task1_3_victim.png)

<br>
After attacker sending out the packet, the user's terminal will freeze after typing 10 characters since user would never receiver ack = last_seq + 1


## Task 4: Creating Reverse Shell using TCP Session Hijacking

this task is pretty much the same as task 3 except that you need to replace the code in the spooffed packet and need to use netcat to open wait the redirect tcp traffic from victim sercer.

![codes](./Assets/imgs/task4_codes.png)

<br>

![hijacking](./Assets/imgs/task4_hijacking.png)

<br>

![redirect_traffic](./Assets/imgs/task4_redirect_TCP_traffic_to_attacker.png)