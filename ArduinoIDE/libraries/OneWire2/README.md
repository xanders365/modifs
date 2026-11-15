# OneWire2 Arduino IDE library bridge

This is a minimal Arduino IDE library wrapper around the canonical implementation in `ArduinoOBI/lib/OneWire`. It avoids maintaining a second copy of the OneWire source.

This wrapper uses repository-relative includes, so keep it inside this checkout. To make Arduino IDE discover it, set the Arduino IDE sketchbook location to the repository's `ArduinoIDE` directory, then open `ArduinoIDE/ArduinoOBI/ArduinoOBI.ino`. Alternatively, install the canonical `ArduinoOBI/lib/OneWire` folder as `OneWire2` in your normal Arduino sketchbook `libraries` folder.
