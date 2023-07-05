#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <TlHelp32.h>

using namespace std;

// 👉 Vector to store discovered bad bytes sequences
vector<string> badbytes;

// 👉 List of target process names to scan
vector<string> processNames = { "gmod.exe", "hl2.exe" };

// 👉 List of byte sequences to search for
vector<string> bytesValues = {
    "exechack",
    "urbanichka",
    "onetap",
    "aimbot",
    "memoriam",
    "neverpivo",
    "ub3rhag",
    "Lemi",
    "LeeCheat",
    "smeghack",
    "oink.industrias",
    "scripthook",
    "antiaim",
};

// 👉 Function to check if the process name matches the target name
bool IsProcessNameMatch(const wstring& processName, const string& targetName) {
    wstring targetNameW(targetName.begin(), targetName.end());
    return (processName == targetNameW);
}

// 👉 Function to find byte sequences in target processes
void FindBytes(const vector<string>& processNames, const vector<string>& bytesValues) {
    cout << "[+] Creating snapshot..." << endl;

    // 👉 Create a snapshot of running processes
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        cout << "Failed to create snapshot" << endl;
        return;
    }
    cout << "[+] Snapshot created" << endl;

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    // 👉 Iterate through the processes in the snapshot
    if (Process32FirstW(snapshot, &entry)) {
        do {
            wstring processNameW(entry.szExeFile);

            // 👉 Check if the process name matches any of the target names
            for (const string& processName : processNames) {
                if (IsProcessNameMatch(processNameW, processName)) {
                    cout << "[+] Found process: " << processName << endl;
                    cout << "[+] Starting, wait..." << endl;

                    // 👉 Open the process for reading memory
                    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, entry.th32ProcessID);
                    if (processHandle != NULL) {
                        cout << "[+] Process opened" << endl;

                        MEMORY_BASIC_INFORMATION mbi;
                        SIZE_T bytesRead;

                        DWORD_PTR address = 0;
                        while (VirtualQueryEx(processHandle, (LPCVOID)address, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) {
                            if (mbi.State == MEM_COMMIT && mbi.RegionSize > 0) {
                                cout << "[+] Reading memory at address: 0x" << hex << mbi.BaseAddress << endl;

                                // 👉 Allocate buffer to read memory
                                vector<BYTE> buffer(mbi.RegionSize);

                                // 👉 Read memory from the target process into the buffer
                                if (ReadProcessMemory(processHandle, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &bytesRead)) {
                                    cout << "[+] Successfully read memory" << endl;

                                    // 👉 Convert the buffer to a string for pattern matching
                                    string bufferString(reinterpret_cast<char*>(buffer.data()), bytesRead);

                                    // 👉 Search for bad byte sequences in the memory buffer
                                    for (const auto& bytesValue : bytesValues) {
                                        if (bufferString.find(bytesValue) != string::npos) {
                                           
                                            // 👉 Check if the byte sequence already exists in badbytes
                                            if (find(badbytes.begin(), badbytes.end(), bytesValue) != badbytes.end()) {
                                                cout << "[-] Byte sequence already exists in badbytes" << endl;
                                                break;
                                            }

                                            // 👉 Byte sequence found! Add it to the badbytes vector
                                            cout << "[+] Found byte sequence: " << bytesValue << endl;
                                            badbytes.push_back(bytesValue);
                                        }
                                    }
                                } else {
                                    cout << "[-] Failed to read memory" << endl;
                                }
                            }
                            address += mbi.RegionSize;
                        }

                        CloseHandle(processHandle);
                    } else {
                        cout << "[-] Failed to open process" << endl;
                    }
                }
            }

        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
}

int main() {
    cout << "[+] Starting FindBytes..." << endl;
    FindBytes(processNames, bytesValues);
    cout << "[+] FindBytes completed" << endl;

    // 👉 Pause the program to keep the console window open
    system("pause");
    return 0;
}
