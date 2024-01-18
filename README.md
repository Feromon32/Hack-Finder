# MemoryReader Prerequisites
To compile and run this code, you need the following:

- ✔️ C++ compiler that supports C++11 or higher
- 🖥️ Windows operating system
- 🧰 Microsoft Visual Studio (recommended) or another development environment with the necessary libraries

![WindowsTerminal_2SQzOQhmUK](https://github.com/Feromon32/Memory-reader/assets/65503900/fd263afa-7e73-4087-a797-ad90a2d4a22b)

# Code Explanation
- 📚 Includes necessary header files for Windows API, vectors, and strings.
- 📋 Defines a set of target process names and byte values to search for.
- 🛠️ Implements a utility function to check if a process name matches a target name.
- 📑 Implements the FindBytes function that searches for the specified byte values in the memory of the target processes.
- ⚙️ Uses Windows API functions to enumerate running processes and open handles to them.
- 📥 Reads the memory of each process and searches for the specified byte values.
- 💡 Outputs the found byte values and stores them in a vector.
- 🔒 Closes the handles and exits the function.
- ▶️ In the main function, calls FindBytes with the predefined process names and byte values.
- ⏸️ Pauses the program to view the results.

# Customization
- ✏️ Modify the `processNames` vector to include the names of the target processes you want to search.
- ✏️ Update the `bytesValues` vector with the byte patterns you want to find in the memory of the target processes.

# Notes
- ℹ️ This code is specific to Windows operating system
- 🚀 Use it responsibly and for legal purposes only

# License
This code is released under the [MIT License](https://opensource.org/license/mit/). Feel free to modify and distribute it according to the terms of the license.
For any questions or issues, please open an issue in this repository
