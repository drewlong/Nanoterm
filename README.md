
# Nanoterm

I wanted to create a tiny terminal emulator that was extensible enough to use in any (Arduino Framework) project of mine. I also wanted it to be a no-nonsense library without much in the way of bloat, since memory is typically a precious resource in microcontrollers. 

I wrote this with PlatformIO in mind, but this could easily be used with the Arduino IDE with minimal tweaking. 



## Usage/Examples

In your src/main.cpp file:
```c++
#include <Arduino.h>
#include <Commands.h> // where you might store your custom commands
#include "Nanoterm.h"

// You could also just define all of your functions up here.

void setup() {
  Serial.begin(115200);

  // A vector of command strings (these are used in the serial terminal)
  std::vector<std::string> keys = {
    "help", "start_ap", "stop_ap", "scan"
  };

  // A vector of correlating function names
  std::vector<CommandPointer> functions = {
    help, startAP, stopAP, scanNetworks
  };
  buildMap(keys, functions);
}

void loop() {
  getInput();
}

```

Until I add this to the PlatformIO registry, you'll need to create a folder under 'include' and add the Nanoterm files there. As illustrated above, you may also want to separate out your commands into their own files. All you'll need to do is include the Nanoterm.h file in your other includes. 

To make use of command line arguments, you'll have to use the included `ARGM` variable. This is a `std::map<std::string, std::string>` type that will hold key-value pairs of command line flags with their param counterparts. For example, to access the command line arguments of `scan -t all`, you might have something like this in your `scan()` function:
```
if(ARGM.find("-s") != ARGM.end()){
    target = ARGM["-t"];
}
```
## Contributing

Contributions are always appreciated, feel free to start building onto this. Forks are welcome, PRs are welcomer. 


## License

[LGPLv3](https://choosealicense.com/licenses/lgpl-3.0/)

