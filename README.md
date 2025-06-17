# Evalo

## Overview

Evalo is cli tool designed to evaluate mathematical expressions  
and return results. It's perfect for quick calculations directly  
from the terminal without needing a calculator or opening a separate app.  
Evalo supports linux and windows (maybe mac, i'm not test it ^-^).  

## Examples

```
evalo 12+22
```
```
evalo -f file.txt
```
```
evalo --help
```

## How Build

For build you need install cmake.  
Copy repo somewhere, then paste this command:  
```
cmake -B build && cmake --build build
```

## How Install

```
cmake -B build && cmake --build build --config Release && sudo cmake --install build
```
  
## Contribution

Contributions are welcome! If you have any suggestions,  
bug reports, or would like to contribute a new feature,  
please create an issue or submit a pull request.  
