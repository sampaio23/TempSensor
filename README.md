# Temperature and Humidity Monitor

Implementation of a temperature and humidity monitor using the STM32F4 Discovery board with serial and Python communication.

### Concept and Motivation

Control of temperature and humidity is fundamental in many applications, from greenhouse maintenance or industrial and laboratory processes, conservation of consumer goods and automotive control to residential applications.
It is common to use air conditioning or vaporizers for small environments and refrigerants in large pipes for industries. However, the manager may want greater control along with a data monitoring system.

This project consists of the initial implementation of a functional prototype, using a microcontroller for monitoring and control of temperature and humidity. The user is able to perform a number of functions:
* Real-time temperature and humidity monitoring via serial communication.
* Control with functions **on** and **off** of an air conditioner and a vaporizer.
* Use of graphical user interface in a desktop application in Python.

### Block diagram

The STM32F407VGT6 microcontroller was used for the project implementation, as well as a DHT11 temperature and humidity sensor. As illustrated, for communication of the card with the computer, serial communication via USB was be used. The communication with the sensor is made by a single wire (bi-directional single-wire) and each sending consists of 40 bits that indicate temperature, humidity, and 1 byte of parity.

<p align="center">
  <img src="https://github.com/Microcontroladores2018/Sampaio/blob/master/images/blockdiagram.png">
</p>

### Pinout and Peripherals

The project used *GPIOs* and *USB*. Below you can see the pinout table and USB commands.

| **Pinout** | **Description**       |
| :--------: | :-------------------: |
| PA11       | USB DP                |
| PA12       | USB DM                |
| PB0        | Data In from sensor   |
| PD12       | Error LED             |
| PD13       | Relay Air Conditioner |
| PD14       | Relay Humidifier      |

```cpp
// Command: info
// Get sensor values
uint16_t cmd_info(uint16_t argc, uint8_t *argv8[]){
	const char **argv=(const char **)argv8;
	uint16_t size=0;
	char* buffer=(char*)argv[0];
	
	if(argc==1) size+=sprintf(buffer+size, "%d %d\r\n", temp ,umi);
    else size+=sprintf(buffer+size, "Syntax: info\r\n");

	return size;
}

// Command: ligarAr num
// If num equals 1, turns air conditioner on; else, turns off 
uint16_t cmd_ligarAr(uint16_t argc, uint8_t *argv8[]){
    if (argc==2){
        char* buffer = (char*) argv8[1];
        if (atoi(buffer) == 1) GPIO_SetBits(GPIOD, LED[1]);
        else GPIO_ResetBits(GPIOD, LED[1]);
    }
    return 0x0000;
}

// Command: ligarUmid num
// If num equals 1, turns humidifier on; else, turns off
uint16_t cmd_ligarUmid(uint16_t argc, uint8_t *argv8[]){
    if (argc==2){
        char* buffer = (char*) argv8[1];
        if (atoi(buffer) == 1) GPIO_SetBits(GPIOD, LED[2]);
        else GPIO_ResetBits(GPIOD, LED[2]);
    }
    return 0x0000;
}
```

### Firmware Flowchart

The flowchart consists of a *loop* that continuously reads the sensor. It contains error handling, and shows, by flashing the LED4, what type of error is happening in real time. If there are no errors, LED4 does not blink.

![screenshot 3](https://github.com/Microcontrollers2018/Sampaio/blob/master/images/flow.png)

When the user executes a command, USB interruption occurs, which performs the function that the user chose. The handling of the commands via USB can be represented by the flowchart below.

![screenshot 4](https://github.com/Microcontrollers2018/Sampaio/blob/master/images/usb.png)

### User Interface

The graphical interface was made in Python using the **Qt4** module. It has a panel that shows, in real time, the temperature and humidity measured by the sensor.

<p align="center">
  <img src="https://github.com/Microcontroladores2018/Sampaio/blob/master/images/gui.png">
</p>

To activate the measurement, you must choose the port on which the microcontroller is connected via USB, and click on the **Start** button. The program also allows the user to connect an air conditioner and/or a vaporizer using the *Air Conditioner ON* and *Humidifier ON* boxes.

To open the interface, simply execute:

```
python main.py
```

### Demo

For demonstration purposes, the pins connected to LEDs 3 and 5 were selected as pins of the relays. In this way, a *relay on* command also illuminates these LEDs.

[![screenshot 5](https://github.com/Microcontroladores2018/Sampaio/blob/master/images/thumb.png)](https://youtu.be/T4_lUNnVeBA)

### References

* Documentation
    * [Datasheet - DHT11](https://akizukidenshi.com/download/ds/aosong/DHT11.pdf)
    * [Reference Manual - STM32F4 Discovery](http://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf)

* Class Doubts
     * Prof. Renault - *Helped in and out of the classroom to solve all doubts.*
     * Queiroz (Class of 2018) - *Allowed me to use his code as a basis to start my project.*