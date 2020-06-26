#include <windows.h>
#include <iostream>
#include <string>

template<typename T, size_t N>
size_t arrSize(T(&array)[N]) { return N; }

LPWSTR CreateProcessArguements(const char* input)
{
    size_t outSize;
    size_t size = strlen(input) + 1;

    wchar_t* wInput = new wchar_t[size];
    mbstowcs_s(&outSize, wInput, size, input, size - 1);

    return wInput;
}

int main(int argc, char* argv[])
{
    std::string* arg = new std::string("-c ~/.nvim\t");
    
    if (argc > 1) { // Translates powershell tab function.
        std::string* args = new std::string(argv[1]);

        if ((*args).substr(0, 2) == ".\\") { (*args).erase(0, 2); }

        (*arg).append(*args);
        delete args;
    }

    const char* cPATH = "C:\\Windows\\System32\\bash.exe";
    LPWSTR ptrPATH = CreateProcessArguements(cPATH);

    const char* cCMDargs = (*arg).c_str();
    LPWSTR ptrCMDargs = CreateProcessArguements(cCMDargs);

    delete arg, cCMDargs, cPATH;

    PROCESS_INFORMATION ProcessInfo; // Output parameter
    STARTUPINFO StartupInfo; // Input LPWSTR

    ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    StartupInfo.cb = sizeof StartupInfo;

    if (CreateProcess(ptrPATH, ptrCMDargs,
        NULL, NULL, FALSE, 0, NULL,
        NULL, &StartupInfo, &ProcessInfo)) {
        WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
        CloseHandle(ProcessInfo.hThread);
        CloseHandle(ProcessInfo.hProcess);
    }
    else {
        std::cout << "Process ID: " << ProcessInfo.dwProcessId << '\n';
        std::cout << "Could not initialize process: " << GetLastError() << '\n';
    }

    return 0;
}
