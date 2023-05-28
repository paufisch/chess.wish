
# chess.wish

This project implements a slightly simplified version of chess in C++ featuring a client/server architecture. The main difference from normal chess is that there is no Checkmate detection and the game ends if one king is taken. It is not required to move out of a check position and it is even possible to move the own king into a check position.

![Chess-logo](./assets/chess_logo.png?raw=true)

Further rule modifications compared to normal chess:
- Castling is not possible
- No en passant 
- After 1000 moves one of the kings randomly dies of old age.



The implementation builds on the template [Lama](https://gitlab.ethz.ch/hraymond/cse-lama-example-project) project and uses [wxWidgets](https://www.wxwidgets.org/) for the GUI, [sockpp](https://github.com/fpagliughi/sockpp) for the network interface, [rapidjson](https://rapidjson.org/md_doc_tutorial.html) for object serialization, and [googletest](https://github.com/google/googletest) for the unit tests. 


## 1. Compile instructions
This project only works on UNIX systems (Linux / MacOS). We recommend using [Ubuntu](https://ubuntu.com/#download), as it offers the easiest way to setup wxWidgets. Therefore, we explain installation only for Ubuntu systems. The following was tested on a Ubuntu 22.4 system, but should also work for earlier versions of Ubuntu.

**Note:** If you create a virtual machine, we recommend to give the virtual machine **at least 12GB** of (dynamic) harddrive space (CLion and wxWidgets need quite a lot of space).

### 1.1 Prepare OS Environment

#### Ubuntu
The OS should already have git installed. If not, you can use: 
`sudo apt-get install git`

Then use  `git clone` to fetch this repository.

Execute the following commands in a console:
1. `sudo apt-get update`
2. `sudo apt-get install build-essential` followed by `sudo reboot`
3. if on virtual machine : install guest-additions (https://askubuntu.com/questions/22743/how-do-i-install-guest-additions-in-a-virtualbox-vm) and then `sudo reboot`
4. `sudo snap install clion --classic` this installs the latest stable CLion version
5. `sudo apt-get install libwxgtk3.0-gtk3-dev` this installs wxWidgets (GUI library used in this project)


### 1.2 Compile Code
1. Open Clion
2. Click `File > Open...` and there select the **/sockpp** folder of this project
3. Click `Build > Build all in 'Debug'`
4. Wait until sockpp is compiled (from now on you never have to touch sockpp again ;))
5. Click `File > Open...` select the **/chess.wish** folder
6. Click `Build > Build all in 'Debug'`
   7. If you want to run the project on a Mac  M1 you need to replace `libsockpp.so` with `libsockpp.dylib` in the `CMakeLists.txt` file.
7. Wait until Chess-server, Chess-client and Chess-tests are compiled

## 2. Run the Game
1. Open a console in the project folder, navigate into "cmake-build-debug" `cd cmake-build-debug`
2. Run server `./Chess-server`
3. In new consoles run two clients `./Chess-client`

## 3. Run the Unit Tests
1. CLion should automatically create a Google Test configuration Lama-tests which will run all tests. See [Google Test run/debug configuration﻿](https://www.jetbrains.com/help/clion/creating-google-test-run-debug-configuration-for-test.html#gtest-config) for more information.
2. From the list on the main toolbar, select the configuration Lama-tests.
3. Click ![run](https://resources.jetbrains.com/help/img/idea/2021.1/artwork.studio.icons.shell.toolbar.run.svg) or press `Shift+F10`.
   
You can run individual tests or test suites by opening the corresponding file in the **/unit-tests** folder and clicking ![run](https://resources.jetbrains.com/help/img/idea/2021.1/artwork.studio.icons.shell.toolbar.run.svg) next to the test method or class. For more information on testing in CLion read the [documentation](https://www.jetbrains.com/help/clion/performing-tests.html).
