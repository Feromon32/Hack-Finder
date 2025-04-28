#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <TlHelp32.h>

using namespace std;

vector<string> badbytes;

void Scan(const vector<string>& processNames, const vector<string>& bytesValues) {

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshot, &entry)) {
        do {
            for (const string& processName : processNames) {
                if (wstring(entry.szExeFile) == wstring(processName.begin(), processName.end())) { // It's faster, too.
                    cout << "[+] Starting, wait..." << endl;
                    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, entry.th32ProcessID);
                    if (processHandle != NULL) {
                        MEMORY_BASIC_INFORMATION mbi;
                        SIZE_T bytesRead;

                        DWORD_PTR address = 0;
                        while (VirtualQueryEx(processHandle, (LPCVOID)address, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) {
                            if (mbi.State == MEM_COMMIT && mbi.RegionSize > 0) {
                                vector<BYTE> buffer(mbi.RegionSize);
                                if (ReadProcessMemory(processHandle, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &bytesRead)) {
                                    string bufferString(reinterpret_cast<char*>(buffer.data()), bytesRead);

                                    for (const auto& bytesValue : bytesValues) {
                                        if (bufferString.find(bytesValue) != string::npos) {
                                            if (find(badbytes.begin(), badbytes.end(), bytesValue) != badbytes.end()) break;
                                            cout << "find: " << bytesValue << endl;
                                            badbytes.push_back(bytesValue);
                                        }
                                    }
                                }
                            }
                            address += mbi.RegionSize;
                        }

                        CloseHandle(processHandle);
                        CloseHandle(snapshot);
                    }
                }
            }

        } while (Process32NextW(snapshot, &entry));
        return; // it makes no sense to check other processes, the main logic comes from the main thread and implementing the dll in other subprocesses will not work
    }
    CloseHandle(snapshot);
}

int main() {
    vector<string> processNames = { "gmod.exe", "hl2.exe" }; // works faster by about a 1.3 sec
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
    Scan(processNames, bytesValues);
    system("pause");
    return 0;
}
