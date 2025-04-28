# MemoryReader

A C++ utility for searching specific byte patterns in the memory of target Windows processes.

![WindowsTerminal_2SQzOQhmUK](https://github.com/Feromon32/Memory-reader/assets/65503900/fd263afa-7e73-4087-a797-ad90a2d4a22b)

## Prerequisites
- C++11-compliant compiler
- Windows OS
- Microsoft Visual Studio (recommended) or equivalent IDE with required libraries

## Features
- Searches for predefined byte patterns in the memory of specified processes
- Uses Windows API for process enumeration and memory reading
- Outputs and stores found byte patterns

## Code Structure
- Includes essential Windows API, vector, and string headers
- Defines target process names and byte patterns
- Implements utility function for process name matching
- Provides `Scan` function to scan process memory for byte patterns
- Manages process handles and memory reading via Windows API
- Stores results in a vector and closes handles properly

## Usage
1. Define target processes in the `processNames` vector
2. Specify byte patterns in the `bytesValues` vector
3. Run the program to search and display results

## Customization
- Edit `processNames` to target different processes
- Modify `bytesValues` to search for custom byte patterns

## Notes
- Windows-specific implementation
- Use responsibly and for legal purposes only

## License
Released under the [MIT License](https://opensource.org/license/mit/). See LICENSE for details.

For issues or questions, please open an issue in this repository.

