# LED blink on button press example

!!! info "Schematic"
    === "Pocket Beagle"
        ![](images/led_button_pocket_beagle.png)   

    === "BeagleBone Black / Beagle Bone Black Wireless"
        ![](images/led_button_beagle_bone_black.png)   

## Code

```python
while : true {
    if : digital_read(P1_29) {
        digital_write(P1_31, false);
    }
    else {
        digital_write(P1_31, true);
    }
}
```

* Following code works on PocketBeagle, to use on other boards, please change the pins accordingly.

## Explaination

This code runs a never ending loop, since it is `while : true`. Inside `while` if header pin P1_29 is HIGH, then header pin P1_31 is set to HIGH, waits for 1000ms, then sets header pin P1_31 to LOW, then again it waits for 1000ms. This loop runs endlessly as long as header pin P1_29 is HIGH, so we get a Blinking output if one connects a LED to output pin.
