# dgrep

A fast multi-threaded grep-like tool using the Boyer-Moore-Horspool algorithm for efficient pattern searching in text.

---

## Installation

### Linux

1. Make sure you have a C++ compiler (`g++`) installed.  
2. Use the provided `build.sh` script to automatically detect your Linux distribution, install the compiler if missing, and build the executable:

```shell
chmod +x build.sh
./build.sh
```

3. After successful compilation, you will get the `dgrep` executable in the /usr/local/bin

---

### Windows

1. Install [Visual Studio](https://visualstudio.microsoft.com/) with the "Desktop development with C++" workload, or at least the Build Tools for Visual Studio.  
2. Open the **Developer Command Prompt for Visual Studio** from your start menu.  
3. Run the provided `build.bat` script to compile the project:

```bat
build.bat
```

4. After successful compilation, you will get the `dgrep.exe` executable in the current directory.

---

## Usage

`dgrep` can be used both to search inside files or to search piped input, similar to the Unix `grep`.

### Search inside a file:

```shell
dgrep filename.txt pattern
```

### Search piped input:

```shell
ls | dgrep pattern
```

Example (Linux):

```shell
ls /var/log | dgrep error
```

Example (Windows PowerShell):

```powershell
dir | ./dgrep.exe bmh
```

---

## Notes

- On Windows, run the compilation script from the Developer Command Prompt to ensure `cl.exe` is available.  
- On Linux, the `build.sh` script will try to install `g++` automatically if missing (supports Debian/Ubuntu and RedHat/CentOS).  
- The tool uses multiple threads internally for fast searching.  
- Line numbers are not printed by default, only matching lines.

---

Feel free to open issues or contribute improvements!
