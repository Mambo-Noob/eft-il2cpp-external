#include <memory/memory.hpp>

#include <tlhelp32.h>

#include <print>

bool memory_c::init(const std::string& process) {
    const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snap == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 pe = {0};
    pe.dwSize = sizeof(PROCESSENTRY32);

    for (bool success = Process32First(snap, &pe); success == true; success = Process32Next(snap, &pe)) {
        if (strcmp(process.c_str(), pe.szExeFile) == 0) {
            if (snap) {
                CloseHandle(snap);
            }

            this->pid = pe.th32ProcessID;
            std::println("pid: {}", this->pid);

            this->handle = OpenProcess(PROCESS_ALL_ACCESS, false, this->pid);

            return this->add_module(process);
        }
    }

    if (snap) {
        CloseHandle(snap);
    }

    return false;
}

bool memory_c::add_module(const std::string& req_module) {
    const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->pid);

    if (snap == INVALID_HANDLE_VALUE) {
        return false;
    }

    MODULEENTRY32 me = {0};
    me.dwSize = sizeof(MODULEENTRY32);

    for (bool success = Module32First(snap, &me); success == true; success = Module32Next(snap, &me)) {
        if (strcmp(req_module.c_str(), me.szModule) == 0) {
            if (snap) {
                CloseHandle(snap);
            }

            this->modules[req_module] = {(std::uint64_t)me.modBaseAddr, me.modBaseSize};

            return true;
        }
    }

    if (snap) {
        CloseHandle(snap);
    }

    return false;
}

module_t memory_c::get_module(const std::string& req_module) {
    return this->modules[req_module];
}

std::string memory_c::read_string(std::uintptr_t address) {
    std::string buffer{};

    do {
        buffer.push_back(this->read<char>(address++));
    } while (this->read<char>(address) != '\0');

    return buffer;
}

std::shared_ptr<memory_c> memory = std::make_shared<memory_c>();