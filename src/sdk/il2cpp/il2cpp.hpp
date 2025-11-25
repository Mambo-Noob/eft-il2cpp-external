#pragma once

#include "../../memory/memory.hpp"
#include "../offsets.hpp"

#include <print>

#define OFFSET(func, type, offset)                                                  \
    type func {                                                                     \
        return memory->read<type>(reinterpret_cast<std::uintptr_t>(this) + offset); \
    }

#define OFFSET_STR(func, offset)                                                                  \
    std::string func {                                                                            \
        auto ptr = memory->read<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(this) + offset); \
        return memory->read_string(ptr);                                                          \
    }

enum Il2CppTypeEnum : byte {
    IL2CPP_TYPE_END = 0x00, /* End of List */
    IL2CPP_TYPE_VOID = 0x01,
    IL2CPP_TYPE_BOOLEAN = 0x02,
    IL2CPP_TYPE_CHAR = 0x03,
    IL2CPP_TYPE_I1 = 0x04,
    IL2CPP_TYPE_U1 = 0x05,
    IL2CPP_TYPE_I2 = 0x06,
    IL2CPP_TYPE_U2 = 0x07,
    IL2CPP_TYPE_I4 = 0x08,
    IL2CPP_TYPE_U4 = 0x09,
    IL2CPP_TYPE_I8 = 0x0a,
    IL2CPP_TYPE_U8 = 0x0b,
    IL2CPP_TYPE_R4 = 0x0c,
    IL2CPP_TYPE_R8 = 0x0d,
    IL2CPP_TYPE_STRING = 0x0e,
    IL2CPP_TYPE_PTR = 0x0f, /* arg: <type> token */
    IL2CPP_TYPE_BYREF = 0x10, /* arg: <type> token */
    IL2CPP_TYPE_VALUETYPE = 0x11, /* arg: <type> token */
    IL2CPP_TYPE_CLASS = 0x12, /* arg: <type> token */
    IL2CPP_TYPE_VAR = 0x13, /* Generic parameter in a generic type definition, represented as number (compressed unsigned integer) number */
    IL2CPP_TYPE_ARRAY = 0x14, /* type, rank, boundsCount, bound1, loCount, lo1 */
    IL2CPP_TYPE_GENERICINST = 0x15, /* <type> <type-arg-count> <type-1> \x{2026} <type-n> */
    IL2CPP_TYPE_TYPEDBYREF = 0x16,
    IL2CPP_TYPE_I = 0x18,
    IL2CPP_TYPE_U = 0x19,
    IL2CPP_TYPE_FNPTR = 0x1b, /* arg: full method signature */
    IL2CPP_TYPE_OBJECT = 0x1c,
    IL2CPP_TYPE_SZARRAY = 0x1d, /* 0-based one-dim-array */
    IL2CPP_TYPE_MVAR = 0x1e, /* Generic parameter in a generic method definition, represented as number (compressed unsigned integer)  */
    IL2CPP_TYPE_CMOD_REQD = 0x1f, /* arg: typedef or typeref token */
    IL2CPP_TYPE_CMOD_OPT = 0x20, /* optional arg: typedef or typref token */
    IL2CPP_TYPE_INTERNAL = 0x21, /* CLR internal type */

    IL2CPP_TYPE_MODIFIER = 0x40, /* Or with the following types */
    IL2CPP_TYPE_SENTINEL = 0x41, /* Sentinel for varargs method signature */
    IL2CPP_TYPE_PINNED = 0x45, /* Local var that points to pinned object */

    IL2CPP_TYPE_ENUM = 0x55, /* an enumeration */
    IL2CPP_TYPE_IL2CPP_TYPE_INDEX = 0xff /* an index into IL2CPP type metadata table */
};

std::string type_to_string(Il2CppTypeEnum type) {
    switch (type) {
    case IL2CPP_TYPE_END:
        return "IL2CPP_TYPE_END";
    case IL2CPP_TYPE_VOID:
        return "IL2CPP_TYPE_VOID";
    case IL2CPP_TYPE_BOOLEAN:
        return "IL2CPP_TYPE_BOOLEAN";
    case IL2CPP_TYPE_CHAR:
        return "IL2CPP_TYPE_CHAR";
    case IL2CPP_TYPE_I1:
        return "IL2CPP_TYPE_I1";
    case IL2CPP_TYPE_U1:
        return "IL2CPP_TYPE_U1";
    case IL2CPP_TYPE_I2:
        return "IL2CPP_TYPE_I2";
    case IL2CPP_TYPE_U2:
        return "IL2CPP_TYPE_U2";
    case IL2CPP_TYPE_I4:
        return "IL2CPP_TYPE_I4";
    case IL2CPP_TYPE_U4:
        return "IL2CPP_TYPE_U4";
    case IL2CPP_TYPE_I8:
        return "IL2CPP_TYPE_I8";
    case IL2CPP_TYPE_U8:
        return "IL2CPP_TYPE_U8";
    case IL2CPP_TYPE_R4:
        return "IL2CPP_TYPE_R4";
    case IL2CPP_TYPE_R8:
        return "IL2CPP_TYPE_R8";
    case IL2CPP_TYPE_STRING:
        return "IL2CPP_TYPE_STRING";
    case IL2CPP_TYPE_PTR:
        return "IL2CPP_TYPE_PTR";
    case IL2CPP_TYPE_BYREF:
        return "IL2CPP_TYPE_BYREF";
    case IL2CPP_TYPE_VALUETYPE:
        return "IL2CPP_TYPE_VALUETYPE";
    case IL2CPP_TYPE_CLASS:
        return "IL2CPP_TYPE_CLASS";
    case IL2CPP_TYPE_VAR:
        return "IL2CPP_TYPE_VAR";
    case IL2CPP_TYPE_ARRAY:
        return "IL2CPP_TYPE_ARRAY";
    case IL2CPP_TYPE_GENERICINST:
        return "IL2CPP_TYPE_GENERICINST";
    case IL2CPP_TYPE_TYPEDBYREF:
        return "IL2CPP_TYPE_TYPEDBYREF";
    case IL2CPP_TYPE_I:
        return "IL2CPP_TYPE_I";
    case IL2CPP_TYPE_U:
        return "IL2CPP_TYPE_U";
    case IL2CPP_TYPE_FNPTR:
        return "IL2CPP_TYPE_FNPTR";
    case IL2CPP_TYPE_OBJECT:
        return "IL2CPP_TYPE_OBJECT";
    case IL2CPP_TYPE_SZARRAY:
        return "IL2CPP_TYPE_SZARRAY";
    case IL2CPP_TYPE_MVAR:
        return "IL2CPP_TYPE_MVAR";
    case IL2CPP_TYPE_CMOD_REQD:
        return "IL2CPP_TYPE_CMOD_REQD";
    case IL2CPP_TYPE_CMOD_OPT:
        return "IL2CPP_TYPE_CMOD_OPT";
    case IL2CPP_TYPE_INTERNAL:
        return "IL2CPP_TYPE_INTERNAL";

    case IL2CPP_TYPE_MODIFIER:
        return "IL2CPP_TYPE_MODIFIER";
    case IL2CPP_TYPE_SENTINEL:
        return "IL2CPP_TYPE_SENTINEL";
    case IL2CPP_TYPE_PINNED:
        return "IL2CPP_TYPE_PINNED";

    case IL2CPP_TYPE_ENUM:
        return "IL2CPP_TYPE_ENUM";
    case IL2CPP_TYPE_IL2CPP_TYPE_INDEX:
        return "IL2CPP_TYPE_IL2CPP_TYPE_INDEX";

    default:
        return "UNKNOWN_TYPE";
    }
}

struct Il2CppType {
    OFFSET(data(), std::uintptr_t, 0x0); // union; value depends on type()
    OFFSET(type(), Il2CppTypeEnum, 0xA);
};

struct FieldInfo {
    OFFSET_STR(name(), 0x0);
    OFFSET(offset(), std::int32_t, 0x18);
};

typedef void (*Il2CppMethodPointer)();

struct MethodInfo {
    OFFSET(methodPointer(), Il2CppMethodPointer, 0x0);
    OFFSET_STR(name(), 0x18);
};

struct Il2CppClass {
    OFFSET_STR(name(), 0x10);
    OFFSET_STR(namespaze(), 0x18);
    OFFSET(byval_arg(), Il2CppType*, 0x20);
    OFFSET(parent(), Il2CppClass*, 0x58);
    OFFSET(fields(), FieldInfo*, 0x80);
    OFFSET(methods(), std::uintptr_t, 0x98);
    OFFSET(static_fields(), std::uintptr_t, 0xB8);
    OFFSET(static_fields_size(), std::uint32_t, 0x10C);
    OFFSET(method_count(), std::uint16_t, 0x120);
    OFFSET(field_count(), std::uint16_t, 0x124);

    using fields_t = std::vector<std::pair<std::string, FieldInfo*>>;

    fields_t get_fields() {
        fields_t ret = {};

        for (auto current = this->fields(); current < this->fields() + (this->field_count() * 0x20); current += 0x20) {
            ret.emplace_back(current->name(), current);
        }

        return ret;
    }

    FieldInfo* get_field(const std::string& req_field) {
        const auto fields = get_fields();

        const auto itterator = std::ranges::find(fields, req_field, &std::pair<std::string, FieldInfo*>::first);
        return itterator == fields.cend() ? nullptr : itterator->second;
    }

    using methods_t = std::vector<std::pair<std::string, MethodInfo*>>;

    methods_t get_methods() {
        methods_t ret = {};

        const auto array = memory->read<MethodInfo*>(this->methods());

        for (auto current = array; current < array + (this->method_count() * 0x58); current += 0x58) {
            ret.emplace_back(current->name(), current);
        }

        return ret;
    }

    MethodInfo* get_method(const std::string& req_method) {
        const auto methods = get_methods();

        const auto itterator = std::ranges::find(methods, req_method, &std::pair<std::string, MethodInfo*>::first);
        return itterator == methods.cend() ? nullptr : itterator->second;
    }
};

struct Il2CppString {
    OFFSET(length(), std::int32_t, 0x10);

    std::wstring chars() {
        return memory->read_wstring(reinterpret_cast<std::uintptr_t>(this) + 0x14);
    }
};

struct Il2CppImageGlobalMetadata {
    OFFSET(typeStart(), std::int32_t, 0x0);
};

struct Il2CppImage {
    OFFSET_STR(name(), 0x0);
    OFFSET(typeCount(), std::uint32_t, 0x18);
    OFFSET(metadataHandle(), Il2CppImageGlobalMetadata*, 0x28);

    using classes_t = std::vector<std::pair<std::string, Il2CppClass*>>;

    classes_t get_classes() {
        classes_t ret = {};

        const auto base = memory->get_module("GameAssembly.dll").base;

        const auto type_info_table = memory->read<std::uintptr_t>(base + offsets::TYPE_INFO_TABLE);

        for (auto i = 0u; i < this->typeCount(); i++) {
            const auto index = this->metadataHandle()->typeStart() + i;
            const auto current = memory->read<Il2CppClass*>(type_info_table + (index * sizeof(std::uintptr_t)));

            const auto name = std::format("{}.{}", current->namespaze(), current->name());
            ret.emplace_back(name, current);
        }

        return ret;
    }

    Il2CppClass* get_class(const std::string& req_class) {
        const auto classes = get_classes();

        const auto itterator = std::ranges::find(classes, req_class, &std::pair<std::string, Il2CppClass*>::first);
        return itterator == classes.cend() ? nullptr : itterator->second;
    }
};

struct Il2CppAssembly {
    OFFSET(image(), Il2CppImage*, 0x0);
    OFFSET_STR(name(), 0x18);

    using assemblies_t = std::vector<std::pair<std::string, Il2CppAssembly*>>;

    static assemblies_t get_assemblies() {
        assemblies_t ret = {};

        const auto base = memory->get_module("GameAssembly.dll").base;

        const auto array_begin = memory->read<std::uintptr_t>(base + offsets::ASSEMBLIES_BEGIN);
        const auto array_end = memory->read<std::uintptr_t>(base + offsets::ASSEMBLIES_END);

        for (auto current_ptr = array_begin; current_ptr < array_end; current_ptr += sizeof(std::uintptr_t)) {
            if (!current_ptr) {
                continue;
            }

            const auto current = memory->read<Il2CppAssembly*>(current_ptr);
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