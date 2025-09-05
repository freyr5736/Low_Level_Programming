#include <cstdint>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <vector>

// Alias for byte (8-bit unsigned) to represent raw binary data
using byte = uint8_t;

//--------------------------------------------
// 1. Type and Field Info (Reflection Metadata)
//--------------------------------------------

// Represents metadata about a single field within a struct/class
struct FieldInfo {
    const char *name; // Field name (for debugging/printing)
    size_t offset;    // Byte offset of the field within the struct
    size_t size;      // Size of the field in bytes
};

// Represents metadata for an entire type (class/struct)
struct TypeInfo {
    const char *name;        // Type name (e.g., "Base", "Derived")
    size_t size;             // Total size of the type in bytes
    const FieldInfo *fields; // Array of field metadata
    size_t fieldCount;       // Number of fields in the array
    const TypeInfo
        *baseType; // Pointer to base type's metadata (nullptr if none)
};

//--------------------------------------------
// 2. Serializer / Deserializer
//--------------------------------------------

// Serialize an object to a byte buffer using reflection info
void serialize_object(const void *obj, const TypeInfo *info,
                      std::vector<byte> &buffer) {
    // Recursively serialize base class first, if it exists
    if (info->baseType)
        serialize_object(obj, info->baseType, buffer);

    // Serialize each field: copy raw bytes from object memory into buffer
    for (size_t i = 0; i < info->fieldCount; ++i) {
        const FieldInfo &field = info->fields[i];
        const byte *addr =
            (const byte *)obj + field.offset; // Get pointer to field
        buffer.insert(buffer.end(), addr,
                      addr + field.size); // Copy field bytes
    }
}

// Deserialize from buffer into object using reflection info
void deserialize_object(void *obj, const TypeInfo *info,
                        const byte *&bufferPtr) {
    // Recursively deserialize base class first
    if (info->baseType)
        deserialize_object(obj, info->baseType, bufferPtr);

    // Copy each field from buffer into the object memory
    for (size_t i = 0; i < info->fieldCount; ++i) {
        const FieldInfo &field =
            info->fields[i]; // metadata: offset & size of each field
        byte *addr =
            (byte *)obj + field.offset; // address of field in obj memory
        std::memcpy(addr, bufferPtr,
                    field.size); // write field data from buffer into object
        bufferPtr += field.size; // move to next field in buffer
    }
}

//--------------------------------------------
// 3. Reflection Struct Definitions
//--------------------------------------------

// Base struct (must be standard-layout for manual reflection to work)
struct Base {
    int a;
};

// Reflection metadata for Base
static FieldInfo Base_fields[] = {
    {"a", offsetof(Base, a), sizeof(int)} // Field info for 'a'
};

static TypeInfo Base_typeinfo = {
    "Base",                                  // Type name
    sizeof(Base),                            // Total size
    Base_fields,                             // Field info array
    sizeof(Base_fields) / sizeof(FieldInfo), // Number of fields
    nullptr                                  // No base class
};

// // Helper to get TypeInfo for Base
// const TypeInfo *get_typeinfo(const Base *) { return &Base_typeinfo; }

// Derived struct inherits from Base
// NOTE: Must have no virtual functions and public inheritance for
// standard-layout
struct Derived : public Base {
    float b;
    char c;
};

// Reflection metadata for Derived (only fields declared in Derived)
static FieldInfo Derived_fields[] = {{"b", offsetof(Derived, b), sizeof(float)},
                                     {"c", offsetof(Derived, c), sizeof(char)}};

static TypeInfo Derived_typeinfo = {
    "Derived",       // Type name
    sizeof(Derived), // Total size of Derived object
    Derived_fields,  // Field info (only new fields)
    sizeof(Derived_fields) / sizeof(FieldInfo),
    &Base_typeinfo // Inherits from Base
};

// // Helper to get TypeInfo for Derived
// const TypeInfo *get_typeinfo(const Derived *) { return &Derived_typeinfo; }

//--------------------------------------------
// 4. Debug Utilities
//--------------------------------------------

// Recursively print field layout of a type and its base types
void print_layout(const TypeInfo *info) {
    if (info->baseType)
        print_layout(info->baseType); // Print base first

    std::cout << "\nType: " << info->name << " | Size: " << info->size
              << " bytes\n";
    for (size_t i = 0; i < info->fieldCount; ++i) {
        std::cout << "  Field: " << info->fields[i].name
                  << " | Offset: " << info->fields[i].offset
                  << " | Size: " << info->fields[i].size << "\n";
    }
}

// Print contents of the serialized buffer in hexadecimal
void print_buffer(const std::vector<byte> &buf) {
    std::cout << "Serialized Bytes: ";
    for (byte b : buf)
        std::printf("%02X ", b); // Print as 2-digit hex
    std::cout << "\n";
}

//--------------------------------------------
// 5. Main Test Program
//--------------------------------------------

int main() {
    // Create an object of Derived and initialize its fields
    Derived d;
    d.a = 42;
    d.b = 3.14f;
    d.c = 'Z';

    // Vector to hold serialized data
    std::vector<byte> buffer;

    // Serialize the object using its reflection info
    serialize_object(&d, &Base_typeinfo, buffer);

    // Print memory layout and the serialized byte contents
    print_layout(&Base_typeinfo);
    print_buffer(buffer);

    // Prepare to deserialize into a new object
    Derived d_copy;
    const byte *ptr = buffer.data(); // Pointer to start of buffer

    // Reconstruct d_copy from the serialized data
    deserialize_object(&d_copy, &Derived_typeinfo, ptr);

    // Print the values of the deserialized object
    std::cout << "\nDeserialized Object:\n";
    std::cout << "  a = " << d_copy.a << "\n";
    std::cout << "  b = " << d_copy.b << "\n";
    std::cout << "  c = " << d_copy.c << "\n";

    return 0;
}
