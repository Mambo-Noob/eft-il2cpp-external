// memory/memory.hpp
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>

// Adjust include path if needed
#include "../DMALibrary/Memory/Memory.h"

struct module_t {
    std::uint64_t base{};
    std::uint32_t size{};
};

class memory_c {
private:
    std::string process_name{};
    std::unordered_map<std::string, module_t> modules{};

public:
    // Attach to process via DMA
    bool init(const std::string& process) {
        process_name = process;

        // Your main.cpp loops until this returns true, so just fail fast here.
        if (!mem.Init(process_name, true /*memMap*/, true /*debug*/)) {
            return false;
        }

        // Optionally cache exe module
        add_module(process_name);
        return true;
    }

    // Module handling
    bool add_module(const std::string& req_module) {
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

    module_t get_module(const std::string& req_module) {
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

    // No-op for compatibility (no VirtualProtect over DMA)
    std::uint32_t protect(std::uintptr_t /*address*/, size_t /*size*/, std::uint32_t protect) {
        // Can't change page protections over DMA; keep API for compatibility.
        // If something expects an "old protect", we just echo the requested one.
        return protect;
    }

    // Typed read/write
    template <typename T = std::uintptr_t>
        T read(std::uintptr_t address) {
            T buffer{};
            if (!address || address < 0x10000)
                return buffer;
        
            if (!mem.Read(address, &buffer, sizeof(T))) {
                // optional: log instead of throw
                //printf("[READ FAIL] 0x%llX\n", address);
                return buffer;
            }
            return buffer;
    }

    template <typename T = std::uintptr_t>
    void write(std::uintptr_t address, const T& value) {
        if (!mem.Write(address, (void*)&value, sizeof(T))) {
            throw std::runtime_error("dma memory write fail");
        }
    }

    // String helpers
    std::string read_string(std::uintptr_t address) {
        std::string buffer;

        // quick sanity: reject clearly bogus addresses
        if (!address || address < 0x10000 || address > 0x00007FFFFFFFFFFF) {
            //std::printf("[read_string] bad addr 0x%llX\n", (unsigned long long)address);
            return buffer;
        }

        // cap length so we don't run off into la-la-land
        for (int i = 0; i < 1024; ++i) {
            char c{};
            if (!mem.Read(address, &c, sizeof(c))) {
                //std::printf("[read_string] READ FAIL @ 0x%llX\n", (unsigned long long)address);
                break;
            }
            if (c == '\0')
                break;

            buffer.push_back(c);
            ++address;
        }

        return buffer;
    }

    std::wstring read_wstring(std::uintptr_t address) {
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
};

// Global pointer, header-only via inline so there¡¯s no linker symbol to resolve
inline std::shared_ptr<memory_c> memory = std::make_shared<memory_c>();
