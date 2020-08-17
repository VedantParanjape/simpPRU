# Sending state of button using RPMSG

!!! info "Schematic"
    === "Pocket Beagle"
        ![](images/led_button_pocket_beagle.png)   

    === "BeagleBone Black / Beagle Bone Black Wireless"
        ![](images/led_button_beagle_bone_black.png)   

## Code

```python
init_message_channel();

while : true {
    if : digital_read(P1_29) {
        send_message(1);
    }
    else {
        send_message(0);
    }
    delay(100);
}
```

* Following code works on PocketBeagle, to use on other boards, please change the pins accordingly.

## Explaination

`init_message_channel` is needed to setup communication channel between ARM<->PRU. It only needs to be called once, before using RPMSG functions. 

`while : true` loop runs endlessly, inside this, we check for value of header pin P1_29, if it reads HIGH, 1 is sent to the ARM core using `send_message` and if it is LOW, 0 is sent to ARM core using `send_message`. Then PRU waits for 100ms, and repeats the steps again and again.
