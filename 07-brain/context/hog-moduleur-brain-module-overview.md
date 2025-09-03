# Hog Moduleur "Brain" module

## About

We're building a module for a synthesizer, called Brain. It is a universal digital module in a Eurorack synthesizer with multipurpose inputs and outputs which allows users to write their own firmware. Additionally we're going to provide some basic modules and probably an SDK for it so that it's easy to get started.

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

## Factory programs

The first (default) factory program will be a MIDI to CV converter, as the rest of the synth works with control voltage. Later we'll work on a sequencer/arpeggiator, delay or reverb, external sound sources, basic drum machine and so on.

The factory programs are all written in C++ using the Pico SDK.