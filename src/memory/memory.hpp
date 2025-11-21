#pragma once

#include <windows.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

struct module_t {
    std::uint64_t base{};
    std::uint32_t size{};
};

class memory_c {
private:
    std::uint32_t pid = 0;
    HANDLE handle = nullptr;
    std::unordered_map<std::string, module_t> modules = {};

public:
    bool init(const std::string& process);

    bool add_module(const std::string& req_module);
    module_t get_module(const std::string& req_module);

    template <typename t = std::uintptr_t>
    t read(std::uintptr_t address);

    std::string read_string(std::uintptr_t address);
};

template <typename t>
t memory_c::read(std::uintptr_t address) {
    t buffer{};

    if (!ReadProcessMemory(this->handle, reinterpret_cast<const void*>(address), &buffer, sizeof(buffer), nullptr)) {
        throw std::runtime_error("memory read fail");
    }

    return buffer;
}

extern std::shared_ptr<memory_c> memory;