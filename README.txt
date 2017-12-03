Requirements:
- New-ish CMake (3.5.1 at development machine)
- New-ish GCC (5.4.0 at development machine) with C++14 support (C++11
  might be enough though)
- The project should work in any fairly modern Linux installation, it
  has been developed and tested on Linux Mint 18.1 (codebase Ubuntu
  16.04) and Lubuntu 16.04 (in a virtual machine).
- I have cheated a bit by using the googletest bundled with Pistache.

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
Run server
- To stop/kill server, press Ctrl+C in terminal.
kvstore/build$ ./unittest



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

Third party libraries used and bundled with repository in
kvstore/3rdparty/:
* crow - https://github.com/ipkn/crow
  - Used for REST HTTP server
* picotest - https://github.com/nyanp/picotest
  - Used for unit testing
* embeddedRest - https://github.com/fnc12/embeddedRest
  - Used for HTTP client
* RapidJSON - https://github.com/Tencent/rapidjson
  - Used for generating and parsing JSON, both server and client side
  - Bundled with embeddedRest
TODO BOOST
