#include <windows.h>
#include <locale>
#define BUFFER_SIZE 2048
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

wchar_t path[BUFFER_SIZE];
void writePathToClipboard();

int wmain(int argc, wchar_t* argv[]) {
    setlocale(LC_ALL, ".UTF8");

    if (argc >= 2) {
        size_t size_m = wcslen(argv[1]) + 1;
        wcscpy_s(path, size_m, argv[1]);
    } else {
        GetCurrentDirectory(BUFFER_SIZE, path);
    }

    writePathToClipboard();
}

void writePathToClipboard() {
    if (wcslen(path) == 0) return;
    if (!OpenClipboard(NULL)) return;

    EmptyClipboard();
    size_t size_m = sizeof(wchar_t) * (wcslen(path) + 1);
    HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, size_m);
    if (!hClipboardData) return;

    wchar_t* pchData;
    pchData = (wchar_t*)GlobalLock(hClipboardData);
    if (!pchData) return;

    wcscpy_s(pchData, size_m / sizeof(wchar_t), path);
    GlobalUnlock(hClipboardData);

    SetClipboardData(CF_UNICODETEXT, hClipboardData);
    CloseClipboard();
}
