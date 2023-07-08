#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <TlHelp32.h>

using namespace std;

vector<string> badbytes;
vector<string> processNames = { "gmod.exe", "hl2.exe" };
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

bool IsProcessNameMatch(const wstring& processName, const string& targetName) {
    wstring targetNameW(targetName.begin(), targetName.end());
    return (processName == targetNameW);
}

void FindBytes(const vector<string>& processNames, const vector<string>& bytesValues) {
    cout << "[+] Starting, wait..." << endl;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return;
    }

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshot, &entry)) {
        do {
            wstring processNameW(entry.szExeFile);

            for (const string& processName : processNames) {
                if (IsProcessNameMatch(processNameW, processName)) {
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
                                            if (find(badbytes.begin(), badbytes.end(), bytesValue) != badbytes.end()) {
                                                break;
                                            }
                                            cout << "find: " << bytesValue << endl;
                                            badbytes.push_back(bytesValue);
                                        }
                                    }
                                }
                            }
                            address += mbi.RegionSize;
                        }

                        CloseHandle(processHandle);
                    }
                }
            }

        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
}

int main() {
    FindBytes(processNames, bytesValues);
    system("pause");
    return 0;
}
