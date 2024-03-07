# Raspberry pi GPIO driver

A collage project developed for the Real-Time Operating Systems class. This project contains a driver for GPIO pins and a test app utilizing the provided driver.

## The Driver

GPIO controls 3 output pins with LEDs, and one input pin with push-button. When a command is written, one of the three LEDs transitions to one of three states: on, off, or blinking.

For example:

```
echo "R1" > /dev/sem_driver
```

This command brings the Red LE diode to the _on_ state. The other diodes are denoted as G (Green) and Y (Yellow).

States: 0 - _off_, 1 - _on_, 2 - _blinking_. LED blinking has 2 paramteres(T0 - _off time_, T1 - _on time_) both of type int, in milliseconds. These paramteres are passed to the driver during installation, with default values set to 250.

Driver also manages a push-button, counting the number of times it is pressed. The counting stops when the counter reaches a maximum number, M (maximum number of presses). The value of M is also passed as a parameter during controller installation, with a default value of 15.

To reset the push-button counter:

```
echo "C" > /dev/sem_driver
```

## Semaphore test app

The user-space test app utilizes read and write commands, simulating a semaphore with the following states:

1. Green on, others off. Duration - Ts1
2. Green blinking, others off. Duration - Ts2
3. Yellow on, others off. Duration - Ts3
4. Red on, others off. Duration - Ts4
5. Yellow and red on, green off. Duration - Ts5

After 5th state, the sequence repeats.

States change like this, unless during the green light is _on_ (1. state), the push-button is pressed M times. In that case, state automatically changes to green blinking state, and sequence continues normally. Button presses have no effect unless the green light is on.

Test app accepts parameters(Ts1, ... , Ts5) during startup.
