#include <iostream>
#include <algorithm>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "linux-pe/includes/img_common.hpp"
#include "linux-pe/includes/nt/image.hpp"


std::pair<win::section_header_t*, win::section_header_t*> GetTextRdataSection(uint64_t imageVA) {
    std::pair<win::section_header_t*, win::section_header_t*> res;
    auto img = (win::image_x64_t*)imageVA;
    auto ntHeader = img->dos_header.get_nt_headers();

    for (auto i = 0; i < ntHeader->sections().count; i++)
    {
        auto section = ntHeader->get_section(i);
        if (section->name.to_string() == ".text")
        {
            res.first = section;
        }
        else if (section->name.to_string() == ".rdata")
        {
            res.second = section;
        }
    }
    return res;
}

uint64_t findXRef(uint64_t begin, uint64_t length, uint64_t valueVA) {
    for (auto pointer = begin; pointer < begin + length - 7; pointer++)
    {
        const char* leaRCX = "\x48\x8d\x0d";
        pointer = (uint64_t)std::search((const char*)pointer, (const char*)begin + length - 7, leaRCX, leaRCX + 3);
        if (*(char*)pointer != 0x48 && *(char*)(pointer + 1) != 0x8d && *(char*)(pointer + 2) != 0x0d)
            continue;

        if (*(uint32_t*)(pointer + 3) != valueVA - pointer - 7)
            continue;

        return pointer;
    }
    return 0;
};

uint64_t findLastJNE(uint64_t asmVA) {
    for (auto pointer = asmVA - 2; pointer > asmVA - 32; pointer++)
    {
        if (*(char*)pointer == 0x75)
            return pointer;
    }
    return 0;
}

uint64_t findString(uint64_t begin, uint64_t length, std::string str){
    auto it = std::search((const char*)begin, (const char*)begin + length, str.data(), str.data() + str.length());
    if(it == (const char*)begin + length)
        return 0;
    return (uint64_t)it;
}

bool Patch() {
    uint64_t imageVA = (uint64_t)GetModuleHandle(L"GameAssembly.dll") | (uint64_t)GetModuleHandle(L"UserAssembly.dll");
    if (!imageVA)
    {
        std::cerr << "Cannot find GameAssembly.dll or UserAssembly.dll." << std::endl;
        return false;

    }

    auto sections = GetTextRdataSection(imageVA);
    if (!sections.first || !sections.second)
    {
        std::cerr << "Cannot find Text or Rdata section." << std::endl;
        return false;
    }

    auto str = findString(sections.second->virtual_address + imageVA,
        sections.second->size_raw_data,
        "Collecting from unknown thread"
        );
    if (!str)
    {
        std::cerr << "Cannot find Error String('Collecting from unknown thread')." << std::endl;
        return false;
    }

    auto asmVA = findXRef(sections.first->virtual_address + imageVA,
        sections.first->size_raw_data,
        str);
    if (!asmVA)
    {
        std::cerr << "Cannot find String('Collecting from unknown thread') XRef." << std::endl;
        return false;
    }

    auto lastJNE = findLastJNE(asmVA);
    if (!lastJNE)
    {
        std::cerr << "Cannot find JNE." << std::endl;
        return false;
    }

    char patch[] = { 0xeb };
    WriteProcessMemory((HANDLE)~0, (char*)lastJNE, patch, 1, 0);
    return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        return Patch();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

