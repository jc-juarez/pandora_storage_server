# Pandora Storage Server 🪐🗃️

Pandora Storage Server is an HTTP Server for Key-Value Pair Element Containers implented in C++ for GNU/Linux.

How to Install Pandora Storage Server
==========

For installing Pandora Storage Server in a GNU/Linux distribution run the following commands in the terminal.

First, on Debian-based Operating Systems perform an apt-get update:

```shell
sudo apt-get update
```

Then make sure you have the [GNU libmicrohttpd library](https://www.gnu.org/software/libmicrohttpd/) installed in your system as well as the cURL library and Python3; in case you need any of these for Debian-based Operating Systems run:

```shell
sudo apt-get install -y libmicrohttpd-dev
```
```shell
sudo apt install curl
```
```shell
sudo apt install python3
```

For Debian-based Operating Systems run the following command to download the Pandora Storage Server .deb package:

```shell
curl -LJO `curl -s https://api.github.com/repos/jc-juarez/pandora_storage_server/releases/latest | python3  -c 'import sys, json; print(json.load(sys.stdin)["assets"][1]["browser_download_url"])'`
```

And finally install the .deb package in your system:

```shell
sudo dpkg -i pandora-storage-server-pkg.deb
```

(For other GNU/Linux distributions you can also download the **pandora** executable file from the [latest release](https://github.com/jc-juarez/pandora_storage_server/releases/latest) section and execute it by itself). 

For running the Pandora Storage Server run **pandora** with root privileges in your terminal:

```shell
sudo pandora
```

A prompt like the following indicating that the server is running should now appear:

```
<<< Pandora Storage Server >>>

<> Debug mode: Enabled
<> Logs: Enabled
<> Server Session ID: 20230106-903513481
<> Logs will be recorded at file: /var/lib/pandora/logs/pandoralog-20230106-903513481.txt

* Running on port: 5000
```





