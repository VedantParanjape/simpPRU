# IO Functions

## PRU<-->Header pin mappings

=== "PocketBeagle"
    === "PRU0"
        | R30 Register bit (Output) | R31 Register bit (Input) | Header pin |
        | :--------------: | :--------------: | :--------: |
        | - | 16 | P1_20 |
        | 7 | 7 | P1_29 |
        | 4 | 4 | P1_31 |
        | 1 | 1 | P1_33 |
        | 0 | 0 | P1_36 |
        | - | 15 | P2_18 |
        | 15 | - | P2_33 |
        | - | 14 | P2_22 |
        | 14 | - | P2_24 |
        | 6 | 6 | P2_28 |
        | 3 | 3 | P2_30 |
        | 2 | 2 | P2_32 |
        | 5 | 5 | P2_34 |

    === "PRU1"
        | R30 Register bit (Output) | R31 Register bit (Input) | Header pin |
        | :--------------: | :--------------: | :--------: |
        | 9 | 9 | P1_02 |
        | 11 | 11 | P1_04 |
        | 15 | 15 | P1_30 |
        | 14 | 14 | P1_32 |
        | 10 | 10 | P1_35 |
        | - | 16 | P2_31 |
        | 8 | 8 | P2_35 |

=== "BeagleBone Black"
    === "PRU0"
        | R30 Register bit (Output) | R31 Register bit (Input) | Header pin |
        | :--------------: | :--------------: | :--------: |
        | - | 15 | P8_15 |
        | 15 | - | P8_11 |
        | - | 14 | P8_16 |
        | 14 | - | P8_12 |
        | 7 | 7 | P9_25 |
        | 5 | 5 | P9_27 |
        | 3 | 3 | P9_28 |
        | 1 | 1 | P9_29 |
        | 2 | 2 | P9_30 |
        | 0 | 0 | P9_31 |
        | 6 | 6 | P9_41 |
        | 4 | 4 | P9_42 |

    === "PRU1"
        | R30 Register bit (Output) | R31 Register bit (Input) | Header pin |
        | :--------------: | :--------------: | :--------: |
        | 13 | 13 | P8_20 |
        | 12 | 12 | P8_21 |
        | 8 | 8 | **P8_27 |
        | 10 | 10 | **P8_28 |
        | 9 | 9 | **P8_29 |
        | 6 | 6 | **P8_39 |
        | 7 | 7 | **P8_40 |
        | 4 | 4 | **P8_41 |
        | 5 | 5 | **P8_42 |
        | 2 | 2 | **P8_43 |
        | 3 | 3 | **P8_44 |
        | 0 | 0 | **P8_45 |
        | 1 | 1 | **P8_46 |

        ** Before using these pins, you need to disable HDMI functionality.     
           You can read how to do this [here](https://archive.vn/D8Bzy)

=== "BeagleBone Black Wireless"
    === "PRU0"
        | R30 Register bit (Output) | R31 Register bit (Input) | Header pin |
        | :--------------: | :--------------: | :--------: |
        | - | 15 | P8_15 |
        | 15 | - | P8_11 |
        | - | 14 | P8_16 |
        | 14 | - | P8_12 |
        | 7 | 7 | P9_25 |
        | 5 | 5 | P9_27 |
        | 3 | 3 | P9_28 |
        | 1 | 1 | P9_29 |
        | 2 | 2 | P9_30 |
        | 0 | 0 | P9_31 |
        | 6 | 6 | P9_41 |
        | 4 | 4 | P9_42 |

    === "PRU1"
        | R30 Register bit (Output) | R31 Register bit (Input) | Header pin |
        | :--------------: | :--------------: | :--------: |
        | 13 | 13 | P8_20 |
        | 12 | 12 | P8_21 |
        | 8 | 8 | **P8_27 |
        | 10 | 10 | **P8_28 |
        | 9 | 9 | **P8_29 |
        | 6 | 6 | **P8_39 |
        | 7 | 7 | **P8_40 |
        | 4 | 4 | **P8_41 |
        | 5 | 5 | **P8_42 |
        | 2 | 2 | **P8_43 |
        | 3 | 3 | **P8_44 |
        | 0 | 0 | **P8_45 |
        | 1 | 1 | **P8_46 |

        ** Before using these pins, you need to disable HDMI functionality.      
           You can read how to do this [here](https://archive.vn/D8Bzy)

=== "BeagleBone AI"
    === "PRU0"
        | R30 Register bit (Output) | R31 Register bit (Input) | Header pin |
        | :--------------: | :--------------: | :--------: |
        | 3 | 3 | P8_12 |
        | 4 | 4 | P8_11 |
        | 5 | 5 | P9_15 |
        | 17 | 17 | P9_26 |

    === "PRU1"
        | R30 Register bit (Output) | R31 Register bit (Input) | Header pin |
        | :--------------: | :--------------: | :--------: |
        | 1 | 1 | P9_20 |
        | 2 | 2 | P9_19 |
        | 3 | 3 | P9_41 |
        | 5 | 5 | P8_18 |
        | 6 | 6 | P8_19 |
        | 7 | 7 | P8_13 |
        | 9 | 9 | P8_14 |
        | 10 | 10 | P9_42 |
        | 11 | 11 | P9_27 |
        | 14 | 14 | P9_14 |
        | 15 | 15 | P9_16 |
        | 16 | 16 | P8_15 |
        | 17 | 17 | P8_26 |
        | 18 | 18 | P8_16 |
    
    === "PRU2"
        | R30 Register bit (Output) | R31 Register bit (Input) | Header pin |
        | :--------------: | :--------------: | :--------: |
        | 10 | 10 | P8_33 |
        | 11 | 11 | P8_31 |
        | 6 | 6 | P8_38 |
        | 7 | 7 | P8_36 |
        | 20 | 20 | P8_08 |
        | 15 | 15 | P9_13 |
        | 3 | 3 | P8_39 |
        | 2 | 2 | P8_42 |
        | 9 | 9 | P8_35 |
        | 8 | 8 | P8_34 |
        | 5 | 5 | P8_37 |
        | 4 | 4 | P8_40 |
        | 17 | 17 | P8_28 |
        | 18 | 18 | P8_29 |
        | 19 | 19 | P8_30 |
        | 1 | 1 | P8_41 |
        | 0 | 0 | P8_44 |
        | 14 | 14 | P9_11 |

    === "PRU3"
        | R30 Register bit (Output) | R31 Register bit (Input) | Header pin |
        | :--------------: | :--------------: | :--------: | 
        | 0 | 0 | P8_32 |
        | 5 | 5 | P9_25 |
        | 6 | 6 | P8_09 |
        | 10 | 10 | P9_31 |
        | 8 | 8 | P9_18 |
        | 16 | 16 | P8_07 |
        | 15 | 15 | P8_10 |
        | 17 | 17 | P8_27 |
        | 20 | 20 | P8_43 |
        | 18 | 18 | P8_45 |
        | 19 | 19 | P8_46 |
        | 9 | 9 | P9_17 |
        | 13 | 13 | P9_28 |
        | 11 | 11 | P9_29 |
        | 12 | 12 | P9_30 |

        ** Before using these pins, you need to disable HDMI functionality.      
           You can read how to do this [here](https://archive.vn/D8Bzy)
           
* All Header pins are `constant integer variable` by default, with its value equal to respective R30/R31 register bit
  * Example: `P1_20` is an constant integer variable with value `16`, similary `P1_02` is an constant integer variable with value `9`

## Digital Write

`digital_write` is a function which enables PRU to write given logic level at specified output pin. It is a function with void return type and it's parameters are `integer` and `boolean`, first parameter is the pin number to write to or PRU R30 register bit and second parameter is `boolean` value to be written. `true` for HIGH and `false` for LOW.

### Syntax

`digital_write(pin_number, value);`

#### Parameters

* `pin_number` is an integer. It must be a header pin name which supports output, or PRU R30 Register bit.
* `value` is a boolean. It is used to set logic level of the output pin, `true` for HIGH and `false` for LOW.

#### Return Type

* `void` - returns nothing.

### Example

```python
int a := 32;

if : a < 32 {
    digital_write(P1_29, true);
}
else {
    digital_write(P1_29, false);
}
```

If the value of a < 32, then pin `P1_29` is set to `HIGH` or else it is set to `LOW`.

## Digital Read

`digital_read` is a function which enables PRU to read logic level at specified input pin. It is a function with return type `boolean` and it's parameter is a `integer` whose value must be the pin number to be read or PRU R31 register bit.

### Syntax

`digital_read(pin_number);`

#### Parameters

* `pin_number` is an integer. It must be a header pin name which supports input, or PRU R31 Register bit.

#### Return Type

* `boolean` - returns the logic level of the pin number passed to it. It returns `true` for HIGH and `false` for LOW.

### Example

```python
if digital_read(P1_20) {
    digital_write(P1_29, false);
}
else {
    digital_write(P1_29, true);
}
```

Logic level of pin `P1_20` is read. If it is HIGH, then pin `P1_29` is set to `LOW`, or else it is set to `HIGH`.

## Delay

`delay` is a function which makes PRU wait for specified milliseconds. When this is called PRU does absolutely nothing, it just sits there waiting.

### Syntax

`delay(time_in_ms);`

#### Parameters

* `time_in_ms` is an integer. It is the amount of time PRU should wait in milliseconds. (1000 milliseconds = 1 second).

#### Return Type

* `void` - returns nothing.

### Example

```python
digital_write(P1_29, true);
delay(2000);
digital_write(P1_29, false);
```

Logic level of pin `P1_29` is set to `HIGH`, PRU waits for *2000 ms* = *2 seconds*, and then sets the logic level of pin `P1_29` to `LOW`.

## Start counter

`start_counter` is a function which starts PRU's internal counter. It counts number of CPU cycles. So it can be used to count time elapsed, as it is known that each cycle takes 5 nanoseconds.

### Syntax 

`start_counter()`

#### Paramters

* n/a

#### Return Type

* `void` - returns nothing.

### Example

```python
start_counter();
```

## Stop counter

`stop_counter` is a function which stops PRU's internal counter.

### Syntax 

`stop_counter()`

#### Paramters

* n/a

#### Return Type

* `void` - returns nothing.

### Example

```python
stop_counter();
```

## Read counter

`read_counter` is a function which reads PRU's internal counter and returns the value. It counts number of CPU cycles. So it can be used to count time elapsed, as it is known that each cycle takes 5 nanoseconds.

### Syntax 

`read_counter()`

#### Parameters

* n/a

#### Return Type

* `integer` - returns the number of cycles elapsed since calling `start_counter`.

### Example

```python
start_counter();

while : read_counter < 200000000 {
    digital_write(P1_29, true);
}

digital_write(P1_29, false);
stop_counter();
```

while the value of hardware counter is less than 200000000, it will set logic level of pin `P1_29` to `HIGH`, after that it will set it to `LOW`. Here, 200000000 cpu cycles means 1 second of time, as CPU clock is 200 MHz. So, LED will turn on for 1 second, and turn off after.

## Init message channel

`init_message_channel` is a function which is used to initialise communication channel between PRU and the ARM core. It is sets up necessary structures to use RPMSG to communicate, it expects a init message from the ARM core to initialise. It is a necessary to call this function before using any of the message functions.

### Syntax

`init_message_channel()`

#### Parameters

* n/a

#### Return Type

* `void` - returns nothing

### Example

```python
init_message_channel();
```

## Receive message

`receive_message` is a function which is used to receive messages from ARM to the PRU, messages can only be `integers`, as only they are supported as of now. It uses RPMSG channel setup by `init_message_channel` to receive messages from ARM core.

### Syntax

`receive_message()`

#### Parameters

* n/a

#### Return Type

* `integer` - returns integer data received from PRU

### Example

```c
init_message_channel();

int emp := receive_message();

if : emp >= 0 {
    digital_write(P1_29, true);
}
else {
    digital_write(P1_29, false);
}
```

## Send message

There are six functions which are used to send messages to ARM core from PRU, messages can be `integers`, `characters`, `bools`, `integer arrays`, `character arrays`, and `boolean arrays`. It uses RPMSG channel setup by `init_message_channel` to send messages from PRU to the ARM core.

For sending arrays, arrays are automatically converted to a string, for example, [1, 2, 3, 4] would become "1 2 3 4".

### Syntax

* `send_int(expression)`
* `send_char(expression)`
* `send_bool(expression)`
* `send_ints(identifier)`
* `send_chars(identifier)`
* `send_bools(identifier)`

* `send_message` is an alias for `send_int` to preserve backwards compatibility.

#### Parameters

* For `send_int` and `send_char`, `expression` would be an arithmetic expression.
* For `send_bool`, `expression` would be a boolean expression
* For `send_ints`, `identifier` should be an identifier for an integer array.
* For `send_chars`, `identifier` should be an identifier for a character array.
* For `send_bools`, `identifier` should be an identifier for a boolean array.

### Example

```c
init_message_channel();

if : digital_read(P1_29) {
    send_bool(true);
}
else {
    send_int(0);
}
```