| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |


# Queue in TCP Server example

We uses PC as a TCP client to generate command. This is WRITER.

The ESP32 works as a TCP server, which receives and filters commands as RECEPTION task, then push to QUEUE.

Moroever, There are 3 functional tasks, which will get commands from QUEUE and execute them if valid. These are READERs.


The application creates a TCP socket with the specified port number and waits for a connection request from the client. After accepting a request from the client, connection between server and client is established and the application waits for some data to be received from the client. Received data are filtered first by the WRTIER TASK, then push into the QUEUE. There are 3 READERs, including: `LED Task`, `Temperature and Humidity Task` and `Monitoring Task`.

Please check out my demo first: `https://youtu.be/a2zc2-b-34o`

## How to use example

### Hardware Required

This example should be able to run on any commonly available ESP32 development board.

### Software Required

I only guarantee this programme will run well on ``ESPRESSIF_IDE``.

In this lab, I'm using ``Hercules`` terminal to simulate a TCP client.

### Build and Flash

Build the project and flash it to the board. You can use any Terminal application to observe the results. I also recommend using built-in terminal of ``ESPRESSIF_IDE``.

(Replace PORT with the name of the serial port to use.)

(To exit the serial monitor, type ``Ctrl-]``.)

## Example Output

* Firstly, go to the source code, find `tcp_server.c`, which is considered as main file.

* From which, please find the function ```wifi_connect("SSID","Password")``` in ```void app_main(void)``` and change the SSID and Password of your Wifi. Then, build and flash into your ESP32. Once it is success, you will see an ``IP address`` in built-in Terminal.

* Take this address, Open ``Hercules Terminal``, in Tab TCP client, please enter the IP address above, the port is ``3333`` by default. Important to ensure your PC connects with the `same WIFI as ESP32`.

* At the result, you can send any command by using TCP client in Hercules. There are many valid command you can try below.

### LED TASK

Using to Control the LEDs

```
cmd:LED:all
cmd:LED:red
cmd:LED:green
cmd:LED:yellow
```

### Temp & Humi Task

Using to Read data from DHT11 sensor

```
cmd:DHT:all
cmd:DHT:temp
cmd:DHT:humi
```

### Monitoring Task

Using as cleaner trash or invalid command by default, it can be used to count the waiting task, or clear all of them.

```
cmd:CLN:clean
cmd:CLN:count
```