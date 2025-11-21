#include <memory/memory.hpp>
#include <sdk/il2cpp/il2cpp.hpp>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int main() {
    while (!memory->init("EscapeFromTarkov.exe")) {
        std::println("waiting for game...");
        std::this_thread::sleep_for(2.5s);
    }

    while (!memory->add_module("GameAssembly.dll")) {
        std::println("waiting for module...");
        std::this_thread::sleep_for(2.5s);
    }

    const auto mod = memory->get_module("GameAssembly.dll");

    std::println("GameAssembly.dll: 0x{:X}", mod.base);

    const auto assembly = Il2CppAssembly::get_assembly("Assembly-CSharp");
    std::println("{}: 0x{:X}", assembly->name(), (uintptr_t)assembly);
}