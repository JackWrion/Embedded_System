| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |

# FreeRTOS Real Time Stats Example

This lab will present how FreeRTOS drives tasks in some specific methods, including `Preemption with Time-slicing`, `Preemption without Time-slicing` and `No preepmtion`.

## How to use example

### Hardware Required

This example should be able to run on any commonly available ESP32 development board.

### Software Required

I only guarantee this programme will run well on ``ESPRESSIF_IDE``.

### Build and Flash

Build the project and flash it to the board. You can use any Terminal application to observe the results. I also recommend using built-in terminal of ``ESPRESSIF_IDE``.

(Replace PORT with the name of the serial port to use.)

(To exit the serial monitor, type ``Ctrl-]``.)


## Example Output

### `Preemption with Time-slicing`

#### Configuration

Don't need to config any thing in this example, because this is default configuration of FreeRTOS.

#### Output and explaination

```
...
----> The Highest on core 0 start at 0
----> The Highest on core 0 END at 20

The Idle task 1 on core 0 START at 20
The Idle task 2 on core 0 START at 20

The Idle task 1 on core 0 is CALLED at 21, work remain: 9
The Idle task 2 on core 0 is CALLED at 21, work remain: 9
The Idle task 1 on core 0 is CALLED at 22, work remain: 8
The Idle task 2 on core 0 is CALLED at 22, work remain: 8
The Idle task 1 on core 0 is CALLED at 23, work remain: 7
The Idle task 2 on core 0 is CALLED at 24, work remain: 7
The Idle task 1 on core 0 is CALLED at 25, work remain: 6
The Idle task 2 on core 0 is CALLED at 25, work remain: 6
The Idle task 1 on core 0 is CALLED at 26, work remain: 5

.....

The Idle task 1 on core 0 is CALLED at 119, work remain: 7
The Idle task 2 on core 0 is CALLED at 119, work remain: 7

----> The Highest on core 0 start at 120
----> The Highest on core 0 END at 140

The Idle task 1 on core 0 is CALLED at 140, work remain: 6
The Idle task 2 on core 0 is CALLED at 140, work remain: 6
...
```

The Highest task is allowed to run first. While it is running, no task is able to run. Moreover, while the IDLE task in running state, the HIGHEST task will preempt to run, once it leaves BLOCKING state. That is how ``Preemption`` works.

When Highest task is in blocking state, The 2 ILDE tasks will share their time to execute. That is how ``Time-slicng`` works.

### `Preemption without Time-slicing`

#### Configuration

Now, You need to find the file system called ``FreeRTOSConfig.h`` in your system library, in my term, It's easy to find by ``CTRL + LeftClick`` on the include header line. From which, find  and change `configUSE_TIME_SLICING` to 0.

#### Output and explaination

```
...
----> The Highest on core 0 start at 0
----> The Highest on core 0 END at 20

----> The Highest on core 0 start at 120
----> The Highest on core 0 END at 140

The Idle task 1 on core 0 START at 140

The Idle task 1 on core 0 is CALLED at 141, work remain: 9
The Idle task 1 on core 0 is CALLED at 142, work remain: 8
The Idle task 1 on core 0 is CALLED at 143, work remain: 7
The Idle task 1 on core 0 is CALLED at 144, work remain: 6
The Idle task 1 on core 0 is CALLED at 145, work remain: 5
The Idle task 1 on core 0 is CALLED at 146, work remain: 4
The Idle task 1 on core 0 is CALLED at 147, work remain: 3
The Idle task 1 on core 0 is CALLED at 148, work remain: 2
The Idle task 1 on core 0 is CALLED at 149, work remain: 1
The Idle task 1 on core 0 is CALLED at 150, work remain: 0

The Idle task 2 on core 0 START at 150

----> The Highest on core 0 start at 240
----> The Highest on core 0 END at 260
...
```

The Highest task is allowed to run first. While it is running, no task is able to run. Moreover, while the IDLE task in running state, the HIGHEST task will preempt to run, once it leaves BLOCKING state. That is how ``Preemption`` works.

When Highest task is in blocking state, There are 3 IDLE tasks in READY state. Only 1 of 3 IDLE task will be chosen by kernel, and take all the time to run until it ends. That presents ``WITHOUT Time-slicng`` works.

### `Preemption without Time-slicing`

#### Configuration

Now, You need to find the file system called ``FreeRTOSConfig.h`` in your system library, in my term, It's easy to find by ``CTRL + LeftClick`` on the include header line. From which, find  and change `configUSE_TIME_SLICING` to 0.

#### Output and explaination

```
----> The Highest on core 0 start at 0
----> The Highest on core 0 END at 20

The Idle task 1 on core 0 START at 20

The Idle task 1 on core 0 is CALLED at 21, work remain: 9
The Idle task 1 on core 0 is CALLED at 22, work remain: 8
The Idle task 1 on core 0 is CALLED at 23, work remain: 7
The Idle task 1 on core 0 is CALLED at 24, work remain: 6
The Idle task 1 on core 0 is CALLED at 25, work remain: 5
The Idle task 1 on core 0 is CALLED at 26, work remain: 4
The Idle task 1 on core 0 is CALLED at 27, work remain: 3
The Idle task 1 on core 0 is CALLED at 28, work remain: 2
The Idle task 1 on core 0 is CALLED at 29, work remain: 1
The Idle task 1 on core 0 is CALLED at 30, work remain: 0

The Idle task 2 on core 0 START at 30

The Idle task 2 on core 0 is CALLED at 31, work remain: 9
The Idle task 2 on core 0 is CALLED at 32, work remain: 8
.....
The Idle task 1 on core 0 is CALLED at 119, work remain: 3
The Idle task 1 on core 0 is CALLED at 120, work remain: 2
The Idle task 1 on core 0 is CALLED at 121, work remain: 1
The Idle task 1 on core 0 is CALLED at 122, work remain: 0

----> The Highest on core 0 start at 122
----> The Highest on core 0 END at 142
```

In this mode, `Time-slicing` makes no effect. If there is any task running, No task is able to preempt to run. Whether it is HIGHEST priority or not.

That is the reason why the HIGHEST has to wait until 122, while it has been already in READY state at 120. 