# Hog Moduleur "Brain" module

## About

We're building a module for a synthesizer, called Brain. It is a universal digital module in a Eurorack synthesizer with multipurpose inputs and outputs which allows users to write their own firmwares. Additionally we're going to provide some basic modules and an SDK for it so that it's easy to get started.

## Features

Brain is based on the Raspberry Pi Pico. It's a universal digital module for Eurorack synths with flexible inputs and outputs.
- 2 analog inputs for CV and audio signals using 2 of the the Pico's ADCs
- 2 analog outputs which uses an MCP4822 DAC. The output of the DAC can be programmatically AC or DC coupled, using a CD4053 IC.
- 1 pulse input, using digital GPIO, preceded by a transistor switch (for safety)
- 1 pulse output, using digital GPIO with a simple transistor switch
- 1 dedicated MIDI input. This input is not universal because MIDI requires special input circuitry

Additionally there are some controls on the UI:
- 3 potmeters, multiplexed, using the third ADC of the Pico
- 2 pushbuttons
- 6 LEDs. The power for the LEDs are taken from the external power supply (using transistors) because of the Pico's limited power capabilities. The LEDs brightness can be set with PWM output from the Pico

As the Pico and Pico2 are pin compatible, the newer version can be used for heavier programs like effects or DSP.

## Code

Code can be flashed to the Pico using the normal procedure of holding the dedicated button while plugging the pico in a computer. Then the Pico appears as a drive which can accept firmware files.

### Factory apps

The factory programs are all written in C++ using the Pico SDK.

### User apps

Anyone can write their own apps for the Brain module. For this we'll provide an SDK with easy access to the built in I/O and interface components. We'll also provide a boilerplate shell script which makes it easy for anyone to get started with a new app/firmware.



