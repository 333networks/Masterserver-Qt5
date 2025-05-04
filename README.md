# 333Networks Master Server Qt5/C++
---

## DESCRIPTION
Qt5/C++ implementation of the 333networks masterserver for the Gamespy v0 protocol. The
software receives UDP beacons from game servers and provides TCP server lists for game clients. The masterserver queries other masterservers to synchronise lists and checks individual game servers for additional display information.

NOTE: This is the GitHub mirror/copy for the 333networks masterserver backend. Check 333networks.com for the latest version. This is a work in progress. Please join us on [discord](https://333networks.com/discord) and participate in one or more of the workgroups or consider making a donation.

Website: [https://333networks.com](https://333networks.com)

## INSTALL
The masterserver requires the Qt5 library v5.9.5 or above to run. On linux, these binaries are available through the repository as `qt5-default` or `qtbase5-dev`.

After installing the Qt5 libraries, check which version is installed and active:
```
qmake --version
```
This should return information similar to 
```
QMake version 3.1
Using Qt version 5.9.5 in /usr/lib/x86_64-linux-gnu
```

For Windows users an installer will be made available at a later stage. With an older linux distribution or older Qt5 version in the repository, you _will_ have to build the Qt5 library (v5.9.5 or above) from source or install it through the Qt5 development environment. See https://www.qt.io/download-open-source for more information.  

## CONFIGURATION
The masterserver is configured from the settings file at `data/MasterServer-Settings.ini`. In this settings file you control the database, network behaviour and synchronisation options. These options are briefly described and illustrated with examples.

#### Default paths
The masterserver uses several external files that are either provided with the repository or generated upon first use. These files are all relative to the application and are found in the following locations, where the parent directory is the repository directory containing the folders `bin`, `data` and `log`:

```
MasterServer-Qt5
|
+-- bin
|   +-- MasterServer-linux-amd64
|
+-- data
|   +-- masterserver.db
|   +-- MasterServer-Settings.ini
|   +-- SupportedGames.json
|
+-- log
    +-- MasterServer-Qt-<version>-<date>.log
```

#### Public Information
Before an online masterserver can be operated, your public details **must be set**. This is to prevent confusion in the community about who is responsible for the masterserver and the games it supports. You provide your host domain, display name and contact address. These may NOT contain a backslash ("\\").
```
[PublicDetails]
Hostname=master.example.com
AdminName=John Doe
Contact=John.Doe@example.com
```

#### Logging
All operations can be logged for daily use and development. The logfiles are stored in the `log` folder and can be cycled `never`, `yearly`, `monthly`, `weekly` and `daily`. Output to the console window can be enabled or disabled. It is possible to suppress certain message types, for example:

* `[2019-10-31 16:19:22.283] [example1] message to be suppressed.`

Adding "`example1`" to the `SuppressLog=debug example1` value suppresses messages of the types "debug" and "example1" in the log file. The keyword `all` suppresses all messages to display or log file.

```
[Logging]
CycleLogs=weekly
SuppressLog=debug udp tcp
SuppressDisplay=debug udp tcp
```

When you choose not to suppress several message types, your log file will grow rapidly. It is recommended that you balance your log cycling based on how many message types you decide to suppress.

#### Heartbeat Server
The masterserver listens for UDP heartbeats. The beacon server port can be edited in the `[BeaconServer]` section. Uplinks to other masterservers can be enabled to advertise your own masterserver and synchronise in two directions. 
```
[BeaconServer]
BeaconPort=27900
DoUplink=false
```

#### Client List Server
The masterserver provides server lists for clients through a TCP server. The port for this server can be changed in the `[ListenServer]` section. In this section you can also set the server lifetime (cache time) in seconds before a server is considered offline/outdated.
```
[ListenServer]
ListenPort=28900
ServerLifeTime_s=1800
```

#### Synchronisation
It is possible to synchronise with other masterservers. The advantage is that every masterserver has the same server list, so clients only need one masterserver (and additional others as backup). 

This can be enabled with the setting `DoSync` in the `[Syncer]` section. With `SyncGames` you can specify which game lists are synchronised: default setting `all` requests all available gameservers from the remote masterserver. In the example below, the values `ut unreal deusex` synchronise *only* specified games. More gamenames can be found in the *Supported Games* file below.

Other masterservers can be added to the syncer list in the format `"address, beaconPort, listenPort"`. The `size` value *must* correspond to the amount of masterserver entries. With `SyncInterval_s` in seconds can be specified how often synchronisation happens with other servers.
```
[Syncer]
DoSync=true
1\SyncServer="master.errorist.eu, 27900, 28900"
2\SyncServer="master.333networks.com, 27900, 28900"
size=2
SyncGames=all
SyncInterval_s=1800
```

#### Checker
To make sure that all server info is up to date, it is possible to query all game servers periodically. With `DoCheck` in the `[Checker]` you enable retrieving information every `CycleInterval_s` seconds. With `GetExtendedInformation` you can opt to retrieve detailed information such as active mutators, current players and teams.

```
[Checker]
CycleInterval_s=900
DoCheck=true
GetExtendedInformation=true
ServerCheckInterval_ms=250
```

With `ServerCheckInterval_ms` in milliseconds, it is possible to regulate the strain on bandwidth. Higher intervals lead to lower bandwidth use. The checker runs on a separate UDP socket and uses its own port, the BeaconPort +1. This port does not require firewall forwarding, but should be kept reserved for the masterserver and not utilised by other services or programs.

#### Maintenance
As the database fills, information may require pruning over time. With the maintenance routine, server statistics are updated and server entries pruned where necessary. Pruning consists of three steps: servers that stopped sending heartbeats are no longer counted as direct uplinks; server info entries for servers that are no longer in the server list (outdated or otherwise) are removed from the database and player info entries that are no longer associated with a server, or older than several hours, are pruned from the database.

```
[Maintenance]
DoMaintenance=true
MaintainRate_s=60
```

With `DoMaintenance`, the maintenance and statistics routine can be disabled. The routine is executed every `MaintainRate_s` seconds.

#### Supported Games
Game support depends on the protocol and the authentication data that is provided in `data/SupportedGames.json`. This spreadsheet contains the means to validate the games, use a default port for game servers and description of the used protocol. Changing these values may prevent servers and clients from connecting. The values in the `gamename` field should be used to select which games are synchronised if the option `all` is not used.

### Port Forwarding
The `BeaconPort` (UDP, default 27900) and `ListenPort` (TCP, default 28900) require port forwarding in order to be reached. The `BeaconPort +1` (UDP) is kept reserved for the masterserver, but does not require forwarding.

## RUNNING
Start the program `./MasterServer-Qt5` from the `bin` folder in console or terminal. Linux users may have to set execution permissions with `chmod +x MasterServer-Qt5` first. Executable names may vary depending on the build.

## KNOWN ISSUES
The following issues are known to us and do not to be reported:
* Message types in logging may or may not be consistent.
* This README is elaborate and long and should be spliced to a decent user manual.

## COPYING
Copyright (c) 2025 Darkelarious & 333networks  
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:  

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
