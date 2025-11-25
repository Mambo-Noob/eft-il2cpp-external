// memory/memory.cpp
#include "memory.hpp"

// Init DMA + attach to process
bool memory_c::init(const std::string& process) {
    process_name = process;

    // You can add retries / sleeps if you want, but your main.cpp
    // already loops until init() returns true.
    if (!mem.Init(process_name, true /*memMap*/, true /*debug*/)) {
        return false;
    }

    // Cache the main module (exe) if you want
    add_module(process_name);
    return true;
}

bool memory_c::add_module(const std::string& req_module) {
    // Use DMA library to find base + size
    size_t base = mem.GetBaseDaddy(req_module);
    if (!base) {
        return false;
    }

    size_t size = mem.GetBaseSize(req_module);
    if (!size) {
        return false;
    }

    module_t mod{};
    mod.base = static_cast<std::uint64_t>(base);
    mod.size = static_cast<std::uint32_t>(size);
    modules[req_module] = mod;
    return true;
}

module_t memory_c::get_module(const std::string& req_module) {
    auto it = modules.find(req_module);
    if (it != modules.end()) {
        return it->second;
    }

    // Lazy-load if not cached yet
    if (!add_module(req_module)) {
        return {}; // zeroed module_t
    }

    return modules[req_module];
}

std::uint32_t memory_c::protect(std::uintptr_t /*address*/, size_t /*size*/, std::uint32_t protect) {
    // Can't change page protections over DMA; keep API for compatibility.
    // If something expects an "old protect", we just echo the requested one.
    return protect;
}

std::string memory_c::read_string(std::uintptr_t address) {
    std::string buffer;

    for (;;) {
        char c{};
        if (!mem.Read(address, &c, sizeof(c))) {
            throw std::runtime_error("dma memory read_string fail");
        }
        if (c == '\0') {
            break;
        }
        buffer.push_back(c);
        ++address;
    }

    return buffer;
}

std::wstring memory_c::read_wstring(std::uintptr_t address) {
    std::wstring buffer;

    for (;;) {
        wchar_t c{};
        if (!mem.Read(address, &c, sizeof(c))) {
            throw std::runtime_error("dma memory read_wstring fail");
        }
        if (c == L'\0') {
            break;
        }
        buffer.push_back(c);
        address += sizeof(wchar_t);
    }

    return buffer;
}

// Global instance for the rest of your code
std::shared_ptr<memory_c> memory = std::make_shared<memory_c>();
