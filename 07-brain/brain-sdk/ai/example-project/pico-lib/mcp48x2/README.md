# Raspberry Pi Pico driver for MCP4802, MCP4812 and MCP4822 DAC's

Raspberry Pi Pico driver for Microchip MCP4802/MCP4812/MCP4822

The MCP4802/4812/4822 devices are dual 8-bit, 10-bit and 12-bit digital to
analog converters with SPI interface. This is a Raspberry Pi Pico driver library 
for these chips.


## Usage

1. Attach pins

The Pico has two SPI controllers (spi0 and spi1) and you can use either of 
them. Each controller can be connected to multiple GPIO pins, check the 
datasheet for more info. Below is an example pinout that you can use with 
this driver:

```
|-------------------------------------------------------|
| SPI Controller: spi0                                  |
|-------------------------------------------------------|
| Pico        | Pico GP#    | MCP48x2     | MCP48x2 pin |
|-------------|-------------|-------------|-------------|
| SCK         | 2 (pin 4)   | SCK         | 7           |
| TX          | 3 (pin 5)   | SDI         | 4           |
| RX          | 4 (pin 6)   | N/A         | -           |
| Csn         | any GPIO    | ~CS         | 2           |
|-------------------------------------------------------|
```

N/A – Not available (these DACs have only inputs)
CS (chip select) is active low, you can use any GPIO pin for it. GP5
(pin 7) is officially called CSn on the Pico so you might wanna use that.

2. Include driver in your project and create an mcp48x2 object:

```
#include <mcp48x2.h>
MCP48x2 dac;
```

3. Initailize the DAC matching the way you set your pins up:

```
dac.init(spi0, 7, 4, 5);
```

where the parameters are: spi port, CS, SCK, SDI (or TX) respectively.

4. Write value to the DAC. If you use the default config then 12bits pretty much 
match the output voltage in mV.

```
dac.write(2000); // To write 2V
```

5. If you want to change the chip's config (e.g. write to another DAC channel) 
then you can use the config or the set_channel, set_gain or set_active methods. 
E.g.:

```
dac.set_channel(MCP48X2_CHANNEL_B); // Note untested
```

## Resources
[Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/20002249B.pdf)
