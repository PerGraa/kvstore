Requirements
------------
- Boost libraries installed at system
- A not too old CMake (3.5.1 at development machine)
- A not too old Clang with C++14 support (4.0.0 at development machine)
- The project should work in any fairly modern Linux installation, it
  has been developed and tested on Linux Mint 18.1 (codebase Ubuntu
  16.04) and Lubuntu 16.04 (in a virtual machine).


Build
-----
1. Unpack zip file
2. $ cd kvstore
3. kvstore$ mkdir build
4. kvstore$ cd build
5. kvstore/build$ cmake ..
6. kvstore/build$ make all
7. Just delete the "build" directory to start from scratch


Run unit tests
--------------
kvstore/build$ ./unittest


Run servers
-----------
To stop/kill server, press Ctrl+C in terminal.

Server with complete cache, which writes simple output to clients.
kvstore/build$ ./simple_server

FIFO cache based server, which writes JSON output to clients.
kvstore/build$ ./fifo_json_server

LRU cache based server, which writes JSON output to clients.
kvstore/build$ ./lru_json_server


Run client
----------
The client can and must be configured at command line as in the
following example. The client will send randomized requests to the
running server within these parameters.

kvstore/build$ ./client 2 10 20 500

Number of threads: 2
Number of keys in store: 10
Messages to send per thread: 20
Time to let thread sleep between each message: 500 (milliseconds)


Manual test of server using curl
--------------------------------
$ curl -i --request PUT    0.0.0.0:8888/put/key/foo/value/bar
$ curl -i --request GET    0.0.0.0:8888/get/key/foo
$ curl -i --request GET    0.0.0.0:8888/size
$ curl -i --request DELETE 0.0.0.0:8888/key/foo

====================================================================

Development targets follows, have fun with them if so inclined :)

Run cppcheck
------------
- Version 1.81 installed at development machine.
kvstore/build$ make cppcheck


Run clang-tidy
--------------
- Version 4.0.0 installed at development machine.
kvstore/build$ make clang-tidy


Run clang-format
----------------
- Version 4.0.0 installed at development machine.
kvstore/build$ make clang-format

====================================================================

Third party libraries used
--------------------------
* boost
  - Used by crow
  - NOT bundled, must be on target system
* crow - https://github.com/ipkn/crow
  - Used for REST HTTP server
  - Bundled in kvstore/3rdparty/
* embeddedRest - https://github.com/fnc12/embeddedRest
  - Used for HTTP client
  - Bundled in kvstore/3rdparty/
* picotest - https://github.com/nyanp/picotest
  - Used for unit testing
  - Bundled in kvstore/3rdparty/
