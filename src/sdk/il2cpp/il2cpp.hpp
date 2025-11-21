#pragma once

#include <memory/memory.hpp>
#include <sdk/offsets.hpp>

#include <print>

#define FIELD(func, type, offset)                                                   \
    type func {                                                                     \
        return memory->read<type>(reinterpret_cast<std::uintptr_t>(this) + offset); \
    }

#define FIELD_STR(func, offset)                                                                   \
    std::string func {                                                                            \
        auto ptr = memory->read<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(this) + offset); \
        return memory->read_string(ptr);                                                          \
    }

struct Il2CppImage {
    FIELD_STR(name(), 0x0);
};

struct Il2CppAssembly {
    FIELD(image(), Il2CppImage*, 0x0);
    FIELD_STR(name(), 0x18);

    static std::vector<std::pair<std::string, Il2CppAssembly*>> get_assemblies() {
        std::vector<std::pair<std::string, Il2CppAssembly*>> ret = {};

        const auto base = memory->get_module("GameAssembly.dll").base;

        const auto array_begin = memory->read<std::uintptr_t>(base + offsets::ASSEMBLIES_BEGIN);
        const auto array_end = memory->read<std::uintptr_t>(base + offsets::ASSEMBLIES_END);

        std::println("array begin: 0x{:X}", array_begin);
        std::println("array end  : 0x{:X}", array_end);

        for (auto current_ptr = array_begin; current_ptr < array_end; current_ptr += sizeof(std::uintptr_t)) {
            if (!current_ptr) {
                continue;
            }

            auto current = memory->read<Il2CppAssembly*>(current_ptr);
            ret.emplace_back(current->name(), current);
        }

        return ret;
    }

    static Il2CppAssembly* get_assembly(const std::string& req_assembly) {
        const auto assebmlies = get_assemblies();

        const auto itterator = std::ranges::find(assebmlies, req_assembly, &std::pair<std::string, Il2CppAssembly*>::first);
        return itterator == assebmlies.cend() ? nullptr : itterator->second;
    }
};