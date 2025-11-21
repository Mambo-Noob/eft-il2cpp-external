#pragma once

#include <cstdint>

namespace offsets {
    constexpr auto ASSEMBLIES_BEGIN = 0x593D8B8; // il2cpp_domain_get_assemblies -> first call -> lea rax, offset
    constexpr auto ASSEMBLIES_END = ASSEMBLIES_BEGIN + sizeof(std::uintptr_t);
} // namespace offsets