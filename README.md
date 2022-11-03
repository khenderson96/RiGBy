# RiGBy
A customizable lighting controller for addressable LED strips that supports multiple independent sections with respective modifiers. RiGBy is controlled by a custom dashboard via MQTT, where section brightness, animation speec, etc. may be modified. 

Setup:
1. Install and run MQTT broker
2. Download provided Nodered configuration and dashboard
3. Configure hardware.h file for pins, additional sensors, number of sections, leds per section, etc.
4. Add SSID, wifi password, mqtt broker IP, and mqtt broker port to sercrets.h
5. upload to controller, control via dashboard
