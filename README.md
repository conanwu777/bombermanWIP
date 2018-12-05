Project requirements:

MAC:
* run Mac_OS_install.sh in root directory.
    *  ./Mac_OS_install.sh

UBUNTU:
* run Ubuntu_install.sh in root directory.
    * ./Ubuntu_install.sh

THEN:
* If you are using CLion, reload CMakeProject.
* If you are using command line use:
    * cmake . && make && cd ./bin && ./minimal_opengl && cd ..
* If your cmake does not work:
  - DL it here https://cmake.org/download/
  -	Browse Source (the root folder)
  -	Browse Build (the /bin)
  - Generate
  - in the bin folder build with "make" and then launch it with "./program_name"