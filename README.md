# fortuna-rp2040
A home-made computer based on the RP2040 CPU (Raspberry Pi Pico)

## Ideas
- OS / user code runs of 1st CPU
- Memory: internal RAM (fast RAM) + external RAM (slow RAM)
  - Code can run from both RAMs + flash
  - External RAM used to store other apps state
  - https://kevinboone.me/pico_run_ram.html
- Video
  - Custom generation of VGA
  - 2nd CPU + PIO used to generate video (see https://geoffg.net/picomitevga.html)
- Flash in Raspberry Pi Pico used for
  - BIOS (1st CPU) + copy of current OS (?)
    - OS code checked with checksum on initialization
  - Video code (2nd CPU)
- SDCard
- HID for keyboard (+ mouse?)

## Tests
- Blink in assembly
- Blink in RAM
- Copy blink to RAM and run it
- Copy blink in C to RAM and run it
