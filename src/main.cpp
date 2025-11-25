#include "memory/memory.hpp"
#include "sdk/il2cpp/il2cpp.hpp"

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

    // sanity test DMA at module base
    uint8_t test = 0;
    if (mem.Read(mod.base, &test, sizeof(test))) {
        std::printf("[TEST] DMA read OK at GameAssembly base: 0x%llX -> 0x%02X\n",
                    static_cast<unsigned long long>(mod.base), test);
    } else {
        std::printf("[TEST] DMA read FAIL at GameAssembly base: 0x%llX\n",
                    static_cast<unsigned long long>(mod.base));
    }

    const auto assembly = Il2CppAssembly::get_assembly("Assembly-CSharp");
    std::println("{}: 0x{:X}", assembly->name(), (std::uintptr_t)assembly);

    const auto klass = assembly->image()->get_class("EFT.TarkovApplication");
    std::println("{}.{}: 0x{:X}", klass->namespaze(), klass->name(), (std::uintptr_t)klass);

    const auto type = klass->byval_arg();
    std::println("{}: 0x{:X}", type_to_string(type->type()), (std::uintptr_t)type);

    const auto field = klass->get_field("UnlockAndShowAllLocations");
    std::println("{}: 0x{:X}", field->name(), field->offset());

    const auto method = klass->get_method("ValidateAnticheat");
    std::println("{}: 0x{:X}", method->name(), (std::uintptr_t)method->methodPointer());
    std::ofstream out("dump.txt");
    if (!out) {
        std::println("Failed to open dump.txt");
        return 1;
    }

    auto assemblies = Il2CppAssembly::get_assemblies();
    for (auto &[asmName, asmPtr] : assemblies) {
        if (!asmPtr) continue;
        out << "[Assembly] " << asmName << "\n";

        auto image = asmPtr->image();
        if (!image) continue;

        auto classes = image->get_classes();
        for (auto &[className, classPtr] : classes) {
            if (!classPtr) continue;
            out << "  [Class] " << className << "\n";

            for (auto &[fieldName, fieldInfo] : classPtr->get_fields()) {
                if (!fieldInfo) continue;
                out << "    [Field] " << fieldName
                    << " Offset: 0x" << std::hex << fieldInfo->offset() << std::dec << "\n";
            }

            for (auto &[methodName, methodInfo] : classPtr->get_methods()) {
                if (!methodInfo) continue;
                out << "    [Method] " << methodName << "\n";
            }

            out << "\n";
        }

        out << "\n";
    }

    out.close();
    std::println("Dumped IL2CPP metadata to dump.txt");
    
}
