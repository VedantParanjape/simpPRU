# Using RPMSG to implement a simple calculator on PRU

## Code

```python
init_message_channel();

while : true {
    int option := receive_message();
    int a := receive_message();
    int b := receive_message();

    if : option == 1 {
        send_message(a+b);
    }
    elif : option == 2 {
        send_message(a-b);
    }
    elif : option == 3 {
        send_message(a*b);
    }
    elif : option == 4 {
        if : b != 0 {
            send_message(a/b);
        }
        else {
            send_message(a);
        }
    }
    else
    {
        send_message(a+b);
    }
}
```

* Following code works on PocketBeagle, to use on other boards, please change the pins accordingly.

## Explaination

`init_message_channel();` starts the message channel for communication with ARM <-> PRU cores. Then `while : true` loops runs endlessly. 

`int option := receive_message();` receives which operator to be executed and stores it in option variable. 1 for addition, 2 for subtractions, 3 for multiplication and 4 for division. `int a := receive_message();` receives the value of first operand, and `int b := receive_message();` receives the value of second operand.

if-elseif ladder checks if value of option is 1, 2, 3 or 4 and accordingly sends the value of operation back to ARM core using `send_message`. While division, it makes sure that divisor is not 0. If value of option is anything other than 1, 2, 3, 4, then it defaults to else condition, that is a+b.

This runs endlessly since it is inside a `while : true` loop. 

