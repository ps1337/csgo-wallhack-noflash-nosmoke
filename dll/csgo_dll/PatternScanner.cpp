#include "PatternScanner.h"
#include <sstream>

PatternScanner::PatternScanner(std::string moduleName) {
    std::cout << std::hex << mm.GetModuleBase(moduleName) << std::endl;
    size = mm.GetModuleSize(moduleName);

    data = std::make_unique< unsigned char[] >(size);

    auto bytesRead{ SIZE_T() };

    if (!ReadProcessMemory(mm.hProc, (LPCVOID)mm.GetModuleBase(moduleName), data.get(), size, &bytesRead) || bytesRead != size)
    {
        throw std::runtime_error("[!] Can't Read Module Bytes");
    }
}

DWORD PatternScanner::FindPattern(std::string pattern) {
    std::vector<unsigned char> pattern_bytes;
    
    std::string tmp;
    std::stringstream ss(pattern);
    std::vector<std::string> hex_chars;

    while (getline(ss, tmp, ' ')) {
        hex_chars.push_back(tmp);
    }

    for (unsigned int i = 0; i < hex_chars.size(); i ++) {
        if (hex_chars[i] == "?") {
            // dummy
            pattern_bytes.push_back(0xFF);
        }
        else {
            char b = (char)strtol(hex_chars[i].c_str(), NULL, 16);
            pattern_bytes.push_back(b);
        }
    }

    for (uint64_t i = 0; i < size; i++) {
        bool found = true;
        for (uint64_t j = 0; j < pattern_bytes.size(); j++) {
            if (hex_chars[j] != "?" && pattern_bytes[j] != data[i + j]) {
                found = false;
                break;
            }
        }
        if (found) {
            return i;
        }
    }
    return 0;
}