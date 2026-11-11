# Safety and validation checklist

1. Remove all propellers before bench testing.
2. Verify battery divider scaling with a multimeter before connecting PA0.
3. Verify INA219 address, shunt resistance and current LSB against the actual breakout.
4. Check the INA219 board and shunt continuous/peak current ratings. Do not use an undersized module in a propulsion current path.
5. Confirm ESC signal voltage compatibility and common ground.
6. Verify TIM1 output with an oscilloscope or logic analyzer before connecting ESC.
7. Confirm ESC minimum pulse and arming procedure from the ESC manufacturer.
8. Start with a current-limited bench supply and conservative test pulse.
9. Calibrate thresholds using safe, repeatable tests for the specific motor/ESC/battery.
10. Test sensor disconnects and fault shutdown behaviour independently.

This code is not flight-certified. A single short current sample is not sufficient to guarantee drone propulsion safety. The OLED text renderer is a placeholder and must be replaced with a tested font-enabled SSD1306 driver. Add a physical start button and a watchdog/interlock before any practical use.
