# RiGBy
A customizable lighting controller for addressable LED strips that supports multiple independent sections with respective animations and modifiers. RiGBy is controlled by a custom Vue dashboard supported by lightweight MQTT messaging. This software was developed for the ESP32, but can be easily ported to other wifi enabled platforms.

Setup:
1. Install and run MQTT broker on Raspberry Pi (Mosquito recommended)
2. Download provided Nodered configuration with custom Vue shboard
3. Configure setup.h file for pins, additional sensors, number of sections, leds per section, etc.
4. Add SSID, wifi password, mqtt broker IP, and mqtt broker port to sercrets.h
5. upload to controller, control via dashboard

Dashboard Notes:
- Provided dashboard will need configured for each deployed controlled
- Master Mode on the dashboard treats the entire strip as a single section
- Rainbow Multiplier refers to the spread of a rainbow over a section. Higher number relates to less spread, and therefore denser color cycles.
- Decay modulates how long a pixel stays lit
- Speed modulates the rate between events

