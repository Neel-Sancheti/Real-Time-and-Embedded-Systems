# STM32L476VGTx Timer Input Capture Test

# Overview:
Using the STM32 Discovery board design and implement an embedded, bare-metal (no operating system) program that will display a list of counts for one thousand rising edge pulse inter-arrival times. The inter-arrival time between the pulses should represent the range of 101 “buckets”. The upper and lower limits must be user configurable via the virtual terminal.

# User interface:
After successfully completing the POST routine (see below) design your user interface to operate as follows (using the virtual terminal):
• On startup the program displays the upper and lower limits and allows the user to either accept those values or to change the lower limit to a new value.
• The lower limit can be from 50 microseconds to 9950 microseconds.
• The upper limit will always be 100 microseconds longer than the current lower limit.
• After reviewing and possibly changing the limits the program issues a start prompt and waits for the user to press the Enter key.
• After completing the 1000 measurements (1001 rising edges) the program displays every non-zero via the puTTY logging feature. Do not display entries with 0 counts.
• The list of counts display should have the time in microseconds as the first column and the count in the second column.
• The list of counts should be in ascending time order. A typical display would have 3 lines like this:
998 5
999 950
1000 45
• After completing the display of the non-zero entries provide the option to run again with either the same or different limits.

# POST:
Power On Self Test (POST) is performed when the program starts. This test must confirm that the GPIO port is seeing pulses at least once in 100 milliseconds. If it fails this test give the user the option to try the POST again. If the POST is successful then the program proceeds with the normal user interface operation described above.
