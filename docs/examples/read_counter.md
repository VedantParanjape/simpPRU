# Read hardware counter example

## Code

```python
start_counter();
while : read_counter() < 200000000 {
    digital_write(4, true);
}
stop_counter();
```

* Following code works on PocketBeagle, to use on other boards, please change the pins accordingly.

## Explaination

Since, PRU's hardware counter works at 200 MHz, it counts upto 2 x 10<sup>8</sup> cycles in 1 second. So, this can be reliably used to count time without using `delay`, as we can find exactly how much time 1 cycle takes. 

2 x 10<sup>8</sup> cycles/second.   
1 Cycles = 0.5 x 10<sup>-8</sup> seconds.

So, it can be used to count how many cycles have passed since, say we received a high input on pin 3. `start_counter` starts the counter, and `read_counter` reads the current state of the counter, and `stop_counter` stops the counter.
