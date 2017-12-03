Requirements:
- Boost libraries installed at system
- A not too old CMake (3.5.1 at development machine)
- A not too old Clang with C++14 support (4.0.0 at development machine)
- The project should work in any fairly modern Linux installation, it
  has been developed and tested on Linux Mint 18.1 (codebase Ubuntu
  16.04) and Lubuntu 16.04 (in a virtual machine).

Build:
1. Unpack zip file
2. $ cd kvstore
3. kvstore$ mkdir build
4. kvstore$ cd build
5. kvstore/build$ cmake ..
6. kvstore/build$ make all
7. Just delete the "build" directory to start from scratch

Run unit tests:
kvstore/build$ ./unittest

TODO
Run server:
- To stop/kill server, press Ctrl+C in terminal.
kvstore/build$ ./server



====================================================================

Development targets follows, have fun with them if so inclined :)

Run cppcheck:
- Version 1.81 installed at development machine.
kvstore/build$ make cppcheck

Run clang-tidy:
- Version 4.0.0 installed at development machine.
kvstore/build$ make clang-tidy

Run clang-format:
- Version 4.0.0 installed at development machine.
kvstore/build$ make clang-format

====================================================================

Third party libraries used:
* boost
  - Used by crow
  - NOT bundled, must be on target system
* crow - https://github.com/ipkn/crow
  - Used for REST HTTP server
  - Bundled in kvstore/3rdparty/
* picotest - https://github.com/nyanp/picotest
  - Used for unit testing
  - Bundled in kvstore/3rdparty/
* embeddedRest - https://github.com/fnc12/embeddedRest
  - Used for HTTP client
  - Bundled in kvstore/3rdparty/
* RapidJSON - https://github.com/Tencent/rapidjson
  - Used for generating and parsing JSON, both server and client side
  - Bundled with embeddedRest in kvstore/3rdparty/
