# Embedded Metronome Project

<table>
  <tr>
    <td>
      Digital metronome project written in C, utilizing TivaWare for C and the Texas Instruments ARM® Cortex®-M4F Based MCU TM4C123G LaunchPad.     
    </td>
    <td>
      <img src="https://github.com/user-attachments/assets/15d0db88-140d-4386-acb2-4a65ba2b6665" width="300"/>
    </td>
  </tr>
</table>

<br>

---
## Description

- The metronome sequence leverages event-driven programming to dynamically update a buzzer and seven segment display. The metronome's menu can be navigated using the LaunchPad's push buttons, allowing for real time selection of time signatures and buzzer frequencies. 

- Additionally, the potentiometer is configured to use one of the LaunchPad's ADCs to measure voltage changes for dynamic switching of the metronome's BPM. This program utilizes pulse width modulation for tone generation and interrupt routines for the potentiometer.

---
## CCS Configurations
This project was developed for use with CCS (_version 12.8.1_) and TivaWare (_version 2.2.0.295_).
The following configurations are expected by CCS for proper funtionality.

### Project File
| Setting        | Selection                             |
|----------------|---------------------------------------|
| Target         | Tiva TM4C123GH6PM                     |
| Connection     | Stellaris In-Circuit Debug Interface  |
| Output Type    | Executable                            |
| Endianness     | Little                                |

### Project Properties
| Setting                                        | Selection                     |
|------------------------------------------------|-------------------------------|
| ARM Compiler -> Include Options -> Browse      | `TivaWare_C_Series-2.2.0.295` |
| ARM Compiler -> Include Options -> Workspace   | `Util`                        |
| ARM Linker -> File Search Path -> Browse       | `driverlib.lib`               |
| ARM Linker -> File Search Path -> Workspace    | `Util.lib`                    |
| ARM Linker -> Basic Options -> Heap Size       | 2048                          |
| ARM Linker -> Basic Options -> Stack Size      | 2048                          |
