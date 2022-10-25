# Dwin Simulator | ESP32 | ESP-IDF

Simulate the dwin display outputs (RX/TX), using an esp32.


## How to use 

Write the VP and value in IDF-Monitor.

Ex: 
VP - > 1020 (HEX) || Value to write - > 12 (INT)

In IDF-Monitor:
VP[0]: 10
VP[1]: 20
Value: 12

Output-TX_Pin(HEX): 5a a5 06 83 vp[0] vp[1] 0x00 0x00 value(hex)
