# hw2
## Pin Connection
* buttons connected to pull-down resistors<br>
  pin vs. function
    * D3 -> up
    * D6 -> down
    * D5 -> enter
* uLCD
    * D1 -> TX
    * D0 -> RX
    * D2 -> RES
* waveform generator
    * D7 -> Aout
* measurement
    * A0 -> Ain

## Usage
* When "enter" is pressed, the output frequency will be updated to the selected one, and measurement will be performed and printed once.
* Run FFT.py. When the sampling result is printed completely, FFT will be performed and plotted.
