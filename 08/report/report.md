# DNS Rebinding Attack Lab

## before task

After appending the local DNS server in the system, we could find browse the attacker32.com successfully.

![attacker.com](Assets/imgs/preTask.png)

## Task 1

"seediot32.com/change" would work while "attacker32.com/change" would not work which is resulted from the same-policy-protocal.

For "seediot32.com/change", it sends request to "seediot32.com/password" which is under the same origin "seediot32.com" so the browser would not block this request. However, request will be blocked for "attacker32.com/change" web.

![successful request from seediot32.com/change](Assets/imgs/Task1_successful_request.png)
![successful request from attacker32.com/change](Assets/imgs/Task1_failed_request.png)

## Task 2

### Step 1: Modify the JavaScript code

After modification and restart the containre, error message could still be captured in the console. However, this error is not the same-origin-policy but the request is not allowed. Maybe the attacker's web just simply not set up this Post request.

![error message still occured after modified codes](Assets/imgs/Task2_1.png)

## Step 2: Conduct the DNS rebinding

Just need to modify the filed of **www** in the attacker nameserver will work. Also, modify the **TTL** field would make it work faster.
![modify attack's nameserver](Assets/imgs/Task3_modifed_nameserver_of_attacker.png)

Before it works, we need to reload the attacker's nameserver. And we could also flush the local dns server to make it takes effect faster.
![reload and flush](Assets/imgs/Task3_reload_nameserver_and_flush_local_dns.png)

Then we reopen the web of attacker32.com/change it will work.
![result](Assets/imgs/Task3_attack_result.png)

## Task 3

This task may be a little confused. Notice that using the automated attack web page means loading its codes to local to let it works, which means we could load the codes of page inside our browser and not close it, then modify the attacker's nameserver. Since we loaded the automated attack web page already, the codes inside would takes effect as long as we keep the page open. This is quite critical to keep the page open, since after we change the attacker's nameserver, next time we open the attacker32.com it will bind to the **new binded ip** we just set.

Steps:
**!!!Notice: after changing attacker's nameserver's settings, you need to reload it!**

![change back](Assets/imgs/Task4_change_configure_file_back.png)
<br>
![load page](Assets/imgs/Task4_load_automated_attack_page.png)
<br>
![change nameserver](Assets/imgs/Task4_change_the_attackers_nameserver.png)
<br>
![wait until it works](Assets/imgs/Task4_wait_until_it_works.png)
