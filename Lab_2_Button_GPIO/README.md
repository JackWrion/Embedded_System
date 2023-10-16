| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |

## GPIO functions:

| GPIO                         | Direction | Configuration                                                 |
| ---------------------------- | --------- | --------------------------------------------------------------|
| CONFIG_GPIO_OUTPUT_2         | output    | Using as LED                                                  |
| CONFIG_GPIO_INPUT_0          | input     | BUTTON pulled up, interrupt from rising edge and falling edge |

## Test:
 1. Connect CONFIG_GPIO_OUTPUT_2 with LED
 2. Connect CONFIG_GPIO_INPUT_0 with BUTTON connected to GND
 3. Click, Double Click or Holding whatever you want

## How to use example

* Firstly, check my demo video: https://youtube.com/shorts/VmXBnlwlUNM?feature=share


### Hardware Required

* A development board with any Espressif SoC (e.g., ESP32-DevKitC, ESP-WROVER-KIT, etc.)
* A USB cable for Power supply and programming
* A Button connected to GND and GPIO_0
* A Led connected to GPIO_2

### Software Required

* I am using Espressif-IDE, ESP32 environment built-in IDE, the newest release.
* Using Python 13 

### Build and Flash

* Only 3 files to consider: 
    
    gpio_example_main.c: main file

    ButtonLib.h and ButtonLib.c: Button Library I have built for re-use purpose, simple and stable.

* If you use Espressif-IDE, just clone this project from my github, then, it will be simply to run

## Example Output

As you run the example, you will see the following log:

```
GPIO[0] is HOLDING .....
1s: 2012215 and 2014550
GPIO[0] is HOLDING .....
GPIO[0] is: 1
1s: 2012215 and 2014550
1s: 2012215 and 2014550
1s: 2012215 and 2014550
GPIO[0] is: 0
GPIO[0] is CLICK ..... ESP32
GPIO[0] is: 1
GPIO[0] is: 0
GPIO[0] is CLICK ..... ESP32
GPIO[0] is DOUBLE CLICK .....
GPIO[0] is: 1
1s: 2012215 and 2014550
1s: 2012215 and 2014550
1s: 2012215 and 2014550
1s: 2012215 and 2014550
...
```

