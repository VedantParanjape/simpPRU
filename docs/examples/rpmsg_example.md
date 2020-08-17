# Using RPMSG to communicate with ARM core

## Code

```python
init_message_channel();

int count := receive_message();

while : true {
    send_message(count);
    count := count + 1;
    delay(1000);
}
```

* Following code works on PocketBeagle, to use on other boards, please change the pins accordingly.

## Explaination

PRU has a functionality to communicate with the ARM core, it is called RPMSG. This examples show how to use RPMSG functionality to communicate with ARM core using RPMSG.

`init_message_channel` is needed to setup communication channel between ARM<->PRU. It only needs to be called once, before using RPMSG functions. 

`int count := receive_message();` waits for a message from ARM Core, we need to send some integer to PRU with which to start the counting. So, say we send 3, then int variable count will be equal to 3.

After this, there is `while : true` block which runs endlessly. Inside the block there is a `send_message` call, this sends message back to the ARM Core.

So, inside the for loop we are sending value of count variable, after this we increase value of count by 1. Then we wait for 1000ms, and repeat the above steps again and again.
