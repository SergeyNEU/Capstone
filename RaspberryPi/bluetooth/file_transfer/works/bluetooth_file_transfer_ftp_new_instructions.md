To compile and execute the `bluetooth_file_transfer_ftp_new.cpp` file, follow these instructions:

1. **Install a C++ compiler**, such as GCC (GNU Compiler Collection), if you haven't already. You can download GCC from https://gcc.gnu.org/ or install it using a package manager on Linux or macOS.

2. **Save the code** in a file named `bluetooth_file_transfer_ftp_new.cpp`.

3. **Open a terminal** (Command Prompt on Windows, Terminal on Linux/macOS) and navigate to the directory containing the `bluetooth_file_transfer_ftp_new.cpp` file using the `cd` command. For example:
```bash
cd /path/to/directory
```
4. **Compile the code** using the following command:
```c
g++ -std=c++11 -o bluetooth_file_transfer bluetooth_file_transfer_ftp_new.cpp
```
This command compiles the code using the C++11 standard and creates an executable named `bluetooth_file_transfer` (or `bluetooth_file_transfer.exe` on Windows).

5. **Run the compiled executable** by providing the required command-line arguments, `<bt_addr>`, `<ftp_channel>`, and `<file_path>`. Replace these placeholders with the appropriate values. For example:
```bash
./bluetooth_file_transfer 00:11:22:33:44:55 12 /path/to/your/file.txt
```
On Windows, you can run the executable without the `./` prefix:
```powershell
bluetooth_file_transfer 00:11:22:33:44:55 12 C:\path\to\your\file.txt
```
**Note:**
This process compiles and runs the C++ program, allowing you to transfer a file over Bluetooth FTP. Specifically, the provided script uses the `obexftp` command-line tool to transfer files to a remote device over Bluetooth FTP. The `obexftp` tool will attempt to establish a connection to the specified Bluetooth device address and channel, so you don't need to have a prior connection established between the machine and the Android device.

However, you do need to ensure that your machine has a working Bluetooth adapter and the appropriate drivers installed. Additionally, the Android device should be discoverable and have Bluetooth File Transfer Profile (FTP) enabled. Some Android devices may require enabling the FTP profile or installing a dedicated FTP server app from the Google Play Store to allow file transfers over Bluetooth.

It is also important to note that the `obexftp` tool may not be installed by default on your system. You will need to install it if you haven't already. You can find installation instructions on the official website: http://dev.zuckschwerdt.org/openobex/wiki/ObexFtp











