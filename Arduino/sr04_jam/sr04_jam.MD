# Ultrasonic Jamming 
Ultrasonic sensors are a non-contact distance measuring device that operate on the same principal as radar or sonar, namely time of flight. Polaroid cameras employed this sensor to provide an approximate distance gauge for photographer. The cheapest commercial market sonar is the HC-SR04 sensor which sends out a modulated signal at 40 kHz. The NewPing Library for Arduino provides the easiest interface to use these sensors. The basic operation is explained below

###Software Loop for Jamming
```c
    digitalWrite(_triggerPin, LOW);   // Set the trigger pin low, should already be low, but this will make sure it is.
    delayMicroseconds(4);             // Wait for pin to go low.
    digitalWrite(_triggerPin, HIGH);  // Set trigger pin high, this tells the sensor to send out a ping.
    delayMicroseconds(10);            // Wait long enough for the sensor to realize the trigger pin is high. Sensor specs say to wait 10uS.
    digitalWrite(_triggerPin, LOW);   // Set trigger pin back to low.
```
### EEureka!
Notice that the trigger is merely a clock signal!
Electrical engineering knowledge *triggered*!
Jamming can be done in **HARDWARE**.

Time to revisit my first Digital Logic Design project, creating an astable timer. A good reference detailing the 55 timer and astable circuits can be found [here](http://electronicsclub.info/555astable.htm)

### Designing the Hardware circuit
Our constraints come from the datasheet of the sonar sensor. The trigger signal is a 10uS pulse which sets off 8 pulses at 40 kHz. Each period is 2.5e-5 seconds. The waiting period should be 0.2 milliseconds as it transmits. To be safe our on time will be 20uS and our off time will be 230 uS. 

Design Parameter | Value
---------------- | -----
Frequency        | 4 kHz
Duty Cycle       | 8%

Circuit Parameter | Value
----------------- | -----
R1                | 302 Ohm
R2                | 28.8 Ohm
C                 | .001 uF

![Astable Circuit](http://electronicsclub.info/images/555astable.gif)
