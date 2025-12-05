# 4164 / 41256 DRAM Tester

This is a simple DRAM tester for 4164 and 41256 DRAM chips. It writes and reads
various patterns to verify the integrity of the memory chips. During the the
test red an green LEDs blink alternatively to indicate that the test is running.
If an error was detected, the the red LED will stay on permanently. If the chip
passed the test, the green LED will stay on. With JP1 closed, the tester will
expect a 4164 chip, with JP1 open, it will expect a 41256 chip.

## Why do we need this?

When working with vintage computers, it's common to encounter faulty DRAM chips.
This tester helps to quikly identify bad chips. There is no particular reason
why I decided to make this tester. There are already many similar designs, but
the ones I found were either too slow, or didn't work for me. So I decided to
learn a bit more about DRAMs and make my own tester which is fast and simple.

## Bill of Materials
- Arduino Nano or compatible board
- DIP-16 ZIF socket for 4164/41256 DRAM chips
- 2x 330 Ohm resistors (for LEDs)
- 2x LEDs (red and green)
- 22-100µF capacitor (for power supply stabilization)
- 2x1 2.54mm jumper (for JP1)

