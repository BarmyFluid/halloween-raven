# Halloween Raven

Halloween Raven is the C source code for firmware that can run on a [Microchip PIC16F1825](https://www.microchip.com/en-us/product/PIC16F1825) microcontroller. It drives two cheap SG90 servos, two ultra-voilet LEDs and a [DFPlayer Mini](https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299) MP3 audio player. I've uploaded it here as someone might find some of the techniques useful. Of particular interest is the handling of multiple servos in a round-robin style using an interrupt service routine that only needs a single timer. I can't take credit for the idea but I found it super useful as a concept. I think it could be applied to other problems where you have limited resources or don't want the complexity of setting up multiple timers.

The completed project has the raven turn its head, blink a few times, squawk, wink and repeat forever. Nothing too fancy but could easily be extended to randomly look or be triggered by a sensor etc.

## Hardware

The raven (or crow if you notice in the source code!) was a [toy raven I ordered on Amazon](https://www.amazon.co.uk/gp/product/B004J3GLSO) that I lopped the head off and sliced the bottom part of the beak away. I 3D printed some parts so that the beak could be operated by a servo to open and close the mouth and the head can turn by the other servo. I drilled out the eyes and replaced them with independently operated LEDs that utilise the higher sink current of the PIC so the cathodes are wired via a current limiting resistor to the PIC output pins.

I've not included a circuit diagram as it's just as simple as current limiting resistors (as you'd expect) for the LEDs, UART for communications with the DFPlayer Mini over two wires (Rx and Tx) and direct connections to the servo signal wires. The source code reveals what pins I used for each element.

On my board I did wire up the BUSY output pin of the DFPlayer to one of the PORTA inputs of the PIC but ended up not using that and simply just implemented a delay after playing the MP3. It does the job but I might consider using it if I were to play different MP3 files and just use the BUSY signal to know when it has finished playing. In its current form it does mean the firmware would need to be updated if the audio file was changed.

Using a PIC microcontroller after spending years building things in the [Arduino](https://www.arduino.cc/) and [Expressif ESP-IDF](https://www.espressif.com/en/products/sdks/esp-idf) ecosystems brought many different emotions. At first frustration at going bare-metal and needing to look-up registers that needed configuring, then at not just going straight for assembly rather than C and then loving the simplicity and speed of programming a PIC. For simple tasks like this I think they are perfect especially for battery operated systems. PIC microcontrollers were my first experience with embedded devices a couple of decades ago so it was nice to return.

## Building

I use the very excellent [MPLAB X IDE 5.5](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide) to develop, build and flash the PIC. My toolchain includes the free XC8 (V2.2) compiler and I use a PICkit3 to upload the firmware.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)