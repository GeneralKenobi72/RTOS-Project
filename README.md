# LEDs example driver for Raspberry PI

Collage project for Real Time Operating Systems class.
It consists of driver for LEDs and a test app, using the given driver.

## The Driver

Writing command brings one of three LEDs to one of three states: on, off, blinking.

```
echo "R1" > /dev/sem_driver
```

brings Red LE diode to _on_ state. 2 Other diodes are G(Green), and Y(Yellow).

States: 0 - _off_, 1 - _on_, 2 - _blinking_. LED blinking has 2 paramteres(T0 - _off time_, T1 - _on time_, both of type int, in milliseconds) that are passed to driver upon installing it, both default values are set to 250.

Driver also handles one push-button in a way that counts how many times the button is pressed. The counting stops when the counter reaches a maximum number, M (maximum number of presses). The value of M is also passed as a parameter during controller installation, with a default value of 15.

```
echo "C" > /dev/sem_driver
```

Sets push-button counter back to 0.

## Semaphore test app

User space test app uses read and write commands, simulating a semaphore with following states:

1. Green on, others off. Duration - Ts1
2. Green blinking, others off. Duration - Ts2
3. Yellow on, others off. Duration - Ts3
4. Red on, others off. Duration - Ts4
5. Yellow and red on, green off. Duration - Ts5

After 5th state, repeat.

States are changing like this unless, during the green light on(1. state), the push-button is pressed M times. In that case, state automatically changes to green blinking state, and sequence continues normally. Button presses have no effect unless the green light is on.

Test app accepts parameters(Ts1, ..., Ts5) during startup.
