#ifndef __onehot
#define __onehot

#include <iostream>
#include <string_view>
#include "wrapper.h"

WRAP(std::string_view, IntegerSuffix)

void __genOneHotEncodeMacros(size_t bitWidth,
    IntegerSuffix suffix)
{
    if (!bitWidth)
    {
        return;
    }
    
    // only __ONEOT_ENCODE1 does real encoding,
    // other __ONEHOT_ENCODENs are just
    // recursive permutation of __oneHotEncode1.
    
    std::cout << "#define __ONEHOT_ENCODE1(x, bitPlace) "
        << "x = 1" << static_cast<std::string_view>(
            suffix) << " << bitPlace\n";
    
    // __ONEHOT_ENCODEN concatenates
    // __ONEHOT_ENCODEN-1 and __ONEHOT_ENCODE1.
    
    for (size_t idxMacro = 2; idxMacro <= bitWidth;
         ++idxMacro)
    {
        std::cout << "#define __ONEHOT_ENCODE"
            << idxMacro << '(';
        
        for (size_t idxVaradic = 0; idxVaradic < idxMacro;
             ++idxVaradic)
        {
            std::cout << 'x' << idxVaradic << ", ";
        }
        
        std::cout << "bitPlace)\t\\\n";
        std::cout << "\t__ONEHOT_ENCODE"
            << idxMacro - 1 << '(';
        
        for (size_t idxVaradic = 0;
             idxVaradic < idxMacro - 1; ++idxVaradic)
        {
            std::cout << 'x' << idxVaradic << ", ";
        }
        
        std::cout << "bitPlace),\t\\\n";
        std::cout << "\t__ONEHOT_ENCODE1(x"
            << idxMacro - 1 << ", (bitPlace + "
            << idxMacro - 1 << static_cast<std::string_view>(
            suffix) << "))\n";
    }
    
    // __ONEHOT_EXTRACT_NUMARGS extracts
    // (bitWidth + 1)th argument from varadic arguments.
    
    std::cout << "#define __ONEHOT_EXTRACT_NUMARGS(";
    
    for (size_t idxVaradic = 0; idxVaradic < bitWidth;
         ++idxVaradic)
    {
        std::cout << 'x' << idxVaradic << ", ";
    }
    
    std::cout << "N, ...) N\n";
    
    // __ONEHOT_NUMARGS puts the number of
    // varadic arguments received
    // on (bitWidth + 1)th argument
    // of varadic arguments forwarding
    // to __ONEHOT_EXTRACT_NUMARGS.
    
    std::cout << "#define __ONEHOT_NUMARGS(...)\t\\\n";
    std::cout << "\t__ONEHOT_EXTRACT_NUMARGS(__VA_ARGS__, ";
    
    for (size_t numArgsCandidate = bitWidth;
         numArgsCandidate; --numArgsCandidate)
    {
        std::cout << numArgsCandidate << ", ";
    }
    
    std::cout << "0)\n";
    
    // make ONEHOT_ENCODE(...) replace
    // __ONEHOT_ENCODEN
    
    std::cout << "#define __ONEHOT_ENCODE_SPECIFICATION(N, ...) "
        << "__ONEHOT_ENCODE ## N(__VA_ARGS__, 0" << static_cast<
        std::string_view>(suffix) << ")\n";
        
    std::cout << "#define __ONEHOT_ENCODE_FORWARD_NUMARGS"
        << "(N, ...) __ONEHOT_ENCODE_SPECIFICATION"
        << "(N, __VA_ARGS__)\n";
        
    std::cout << "#define ONEHOT_ENCODE(...) "
        << "__ONEHOT_ENCODE_FORWARD_NUMARGS("
        << "__ONEHOT_NUMARGS(__VA_ARGS__), "
        << "__VA_ARGS__)\n";
}

#define __ONEHOT_ENCODE1(x, bitPlace) x = 1ull << bitPlace
#define __ONEHOT_ENCODE2(x0, x1, bitPlace)      \
        __ONEHOT_ENCODE1(x0, bitPlace), \
        __ONEHOT_ENCODE1(x1, (bitPlace + 1ull))
#define __ONEHOT_ENCODE3(x0, x1, x2, bitPlace)  \
        __ONEHOT_ENCODE2(x0, x1, bitPlace),     \
        __ONEHOT_ENCODE1(x2, (bitPlace + 2ull))
#define __ONEHOT_ENCODE4(x0, x1, x2, x3, bitPlace)      \
        __ONEHOT_ENCODE3(x0, x1, x2, bitPlace), \
        __ONEHOT_ENCODE1(x3, (bitPlace + 3ull))
#define __ONEHOT_ENCODE5(x0, x1, x2, x3, x4, bitPlace)  \
        __ONEHOT_ENCODE4(x0, x1, x2, x3, bitPlace),     \
        __ONEHOT_ENCODE1(x4, (bitPlace + 4ull))
#define __ONEHOT_ENCODE6(x0, x1, x2, x3, x4, x5, bitPlace)      \
        __ONEHOT_ENCODE5(x0, x1, x2, x3, x4, bitPlace), \
        __ONEHOT_ENCODE1(x5, (bitPlace + 5ull))
#define __ONEHOT_ENCODE7(x0, x1, x2, x3, x4, x5, x6, bitPlace)  \
        __ONEHOT_ENCODE6(x0, x1, x2, x3, x4, x5, bitPlace),     \
        __ONEHOT_ENCODE1(x6, (bitPlace + 6ull))
#define __ONEHOT_ENCODE8(x0, x1, x2, x3, x4, x5, x6, x7, bitPlace)      \
        __ONEHOT_ENCODE7(x0, x1, x2, x3, x4, x5, x6, bitPlace), \
        __ONEHOT_ENCODE1(x7, (bitPlace + 7ull))
#define __ONEHOT_ENCODE9(x0, x1, x2, x3, x4, x5, x6, x7, x8, bitPlace)  \
        __ONEHOT_ENCODE8(x0, x1, x2, x3, x4, x5, x6, x7, bitPlace),     \
        __ONEHOT_ENCODE1(x8, (bitPlace + 8ull))
#define __ONEHOT_ENCODE10(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, bitPlace)     \
        __ONEHOT_ENCODE9(x0, x1, x2, x3, x4, x5, x6, x7, x8, bitPlace), \
        __ONEHOT_ENCODE1(x9, (bitPlace + 9ull))
#define __ONEHOT_ENCODE11(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, bitPlace)        \
        __ONEHOT_ENCODE10(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, bitPlace),    \
        __ONEHOT_ENCODE1(x10, (bitPlace + 10ull))
#define __ONEHOT_ENCODE12(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, bitPlace)   \
        __ONEHOT_ENCODE11(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, bitPlace),       \
        __ONEHOT_ENCODE1(x11, (bitPlace + 11ull))
#define __ONEHOT_ENCODE13(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, bitPlace)      \
        __ONEHOT_ENCODE12(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, bitPlace),  \
        __ONEHOT_ENCODE1(x12, (bitPlace + 12ull))
#define __ONEHOT_ENCODE14(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, bitPlace) \
        __ONEHOT_ENCODE13(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, bitPlace),     \
        __ONEHOT_ENCODE1(x13, (bitPlace + 13ull))
#define __ONEHOT_ENCODE15(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, bitPlace)    \
        __ONEHOT_ENCODE14(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, bitPlace),        \
        __ONEHOT_ENCODE1(x14, (bitPlace + 14ull))
#define __ONEHOT_ENCODE16(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, bitPlace)       \
        __ONEHOT_ENCODE15(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, bitPlace),   \
        __ONEHOT_ENCODE1(x15, (bitPlace + 15ull))
#define __ONEHOT_ENCODE17(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, bitPlace)  \
        __ONEHOT_ENCODE16(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, bitPlace),      \
        __ONEHOT_ENCODE1(x16, (bitPlace + 16ull))
#define __ONEHOT_ENCODE18(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, bitPlace)     \
        __ONEHOT_ENCODE17(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, bitPlace), \
        __ONEHOT_ENCODE1(x17, (bitPlace + 17ull))
#define __ONEHOT_ENCODE19(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, bitPlace)        \
        __ONEHOT_ENCODE18(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, bitPlace),    \
        __ONEHOT_ENCODE1(x18, (bitPlace + 18ull))
#define __ONEHOT_ENCODE20(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, bitPlace)   \
        __ONEHOT_ENCODE19(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, bitPlace),       \
        __ONEHOT_ENCODE1(x19, (bitPlace + 19ull))
#define __ONEHOT_ENCODE21(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, bitPlace)      \
        __ONEHOT_ENCODE20(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, bitPlace),  \
        __ONEHOT_ENCODE1(x20, (bitPlace + 20ull))
#define __ONEHOT_ENCODE22(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, bitPlace) \
        __ONEHOT_ENCODE21(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, bitPlace),     \
        __ONEHOT_ENCODE1(x21, (bitPlace + 21ull))
#define __ONEHOT_ENCODE23(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, bitPlace)    \
        __ONEHOT_ENCODE22(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, bitPlace),        \
        __ONEHOT_ENCODE1(x22, (bitPlace + 22ull))
#define __ONEHOT_ENCODE24(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, bitPlace)       \
        __ONEHOT_ENCODE23(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, bitPlace),   \
        __ONEHOT_ENCODE1(x23, (bitPlace + 23ull))
#define __ONEHOT_ENCODE25(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, bitPlace)  \
        __ONEHOT_ENCODE24(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, bitPlace),      \
        __ONEHOT_ENCODE1(x24, (bitPlace + 24ull))
#define __ONEHOT_ENCODE26(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, bitPlace)     \
        __ONEHOT_ENCODE25(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, bitPlace), \
        __ONEHOT_ENCODE1(x25, (bitPlace + 25ull))
#define __ONEHOT_ENCODE27(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, bitPlace)     \
        __ONEHOT_ENCODE26(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, bitPlace),    \
        __ONEHOT_ENCODE1(x26, (bitPlace + 26ull))
#define __ONEHOT_ENCODE28(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, bitPlace)\
        __ONEHOT_ENCODE27(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, bitPlace),    \
        __ONEHOT_ENCODE1(x27, (bitPlace + 27ull))
#define __ONEHOT_ENCODE29(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, bitPlace)    \
        __ONEHOT_ENCODE28(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, bitPlace),\
        __ONEHOT_ENCODE1(x28, (bitPlace + 28ull))
#define __ONEHOT_ENCODE30(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, bitPlace)       \
        __ONEHOT_ENCODE29(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, bitPlace),   \
        __ONEHOT_ENCODE1(x29, (bitPlace + 29ull))
#define __ONEHOT_ENCODE31(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, bitPlace)  \
        __ONEHOT_ENCODE30(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, bitPlace),      \
        __ONEHOT_ENCODE1(x30, (bitPlace + 30ull))
#define __ONEHOT_ENCODE32(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, bitPlace)     \
        __ONEHOT_ENCODE31(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, bitPlace), \
        __ONEHOT_ENCODE1(x31, (bitPlace + 31ull))
#define __ONEHOT_ENCODE33(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, bitPlace)        \
        __ONEHOT_ENCODE32(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, bitPlace),    \
        __ONEHOT_ENCODE1(x32, (bitPlace + 32ull))
#define __ONEHOT_ENCODE34(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, bitPlace)   \
        __ONEHOT_ENCODE33(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, bitPlace),       \
        __ONEHOT_ENCODE1(x33, (bitPlace + 33ull))
#define __ONEHOT_ENCODE35(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, bitPlace)      \
        __ONEHOT_ENCODE34(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, bitPlace),  \
        __ONEHOT_ENCODE1(x34, (bitPlace + 34ull))
#define __ONEHOT_ENCODE36(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, bitPlace) \
        __ONEHOT_ENCODE35(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, bitPlace),     \
        __ONEHOT_ENCODE1(x35, (bitPlace + 35ull))
#define __ONEHOT_ENCODE37(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, bitPlace)    \
        __ONEHOT_ENCODE36(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, bitPlace),        \
        __ONEHOT_ENCODE1(x36, (bitPlace + 36ull))
#define __ONEHOT_ENCODE38(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, bitPlace)       \
        __ONEHOT_ENCODE37(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, bitPlace),   \
        __ONEHOT_ENCODE1(x37, (bitPlace + 37ull))
#define __ONEHOT_ENCODE39(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, bitPlace)  \
        __ONEHOT_ENCODE38(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, bitPlace),      \
        __ONEHOT_ENCODE1(x38, (bitPlace + 38ull))
#define __ONEHOT_ENCODE40(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, bitPlace)     \
        __ONEHOT_ENCODE39(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, bitPlace), \
        __ONEHOT_ENCODE1(x39, (bitPlace + 39ull))
#define __ONEHOT_ENCODE41(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, bitPlace)        \
        __ONEHOT_ENCODE40(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, bitPlace),    \
        __ONEHOT_ENCODE1(x40, (bitPlace + 40ull))
#define __ONEHOT_ENCODE42(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, bitPlace)   \
        __ONEHOT_ENCODE41(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, bitPlace),       \
        __ONEHOT_ENCODE1(x41, (bitPlace + 41ull))
#define __ONEHOT_ENCODE43(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, bitPlace)      \
        __ONEHOT_ENCODE42(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, bitPlace),  \
        __ONEHOT_ENCODE1(x42, (bitPlace + 42ull))
#define __ONEHOT_ENCODE44(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, bitPlace) \
        __ONEHOT_ENCODE43(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, bitPlace),     \
        __ONEHOT_ENCODE1(x43, (bitPlace + 43ull))
#define __ONEHOT_ENCODE45(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, bitPlace)    \
        __ONEHOT_ENCODE44(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, bitPlace),        \
        __ONEHOT_ENCODE1(x44, (bitPlace + 44ull))
#define __ONEHOT_ENCODE46(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, bitPlace)       \
        __ONEHOT_ENCODE45(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, bitPlace),   \
        __ONEHOT_ENCODE1(x45, (bitPlace + 45ull))
#define __ONEHOT_ENCODE47(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, bitPlace)  \
        __ONEHOT_ENCODE46(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, bitPlace),      \
        __ONEHOT_ENCODE1(x46, (bitPlace + 46ull))
#define __ONEHOT_ENCODE48(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, bitPlace)     \
        __ONEHOT_ENCODE47(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, bitPlace), \
        __ONEHOT_ENCODE1(x47, (bitPlace + 47ull))
#define __ONEHOT_ENCODE49(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, bitPlace)        \
        __ONEHOT_ENCODE48(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, bitPlace),    \
        __ONEHOT_ENCODE1(x48, (bitPlace + 48ull))
#define __ONEHOT_ENCODE50(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, bitPlace)   \
        __ONEHOT_ENCODE49(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, bitPlace),       \
        __ONEHOT_ENCODE1(x49, (bitPlace + 49ull))
#define __ONEHOT_ENCODE51(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, bitPlace)      \
        __ONEHOT_ENCODE50(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, bitPlace),  \
        __ONEHOT_ENCODE1(x50, (bitPlace + 50ull))
#define __ONEHOT_ENCODE52(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, bitPlace) \
        __ONEHOT_ENCODE51(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, bitPlace),     \
        __ONEHOT_ENCODE1(x51, (bitPlace + 51ull))
#define __ONEHOT_ENCODE53(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, bitPlace)    \
        __ONEHOT_ENCODE52(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, bitPlace),        \
        __ONEHOT_ENCODE1(x52, (bitPlace + 52ull))
#define __ONEHOT_ENCODE54(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, bitPlace)       \
        __ONEHOT_ENCODE53(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, bitPlace),   \
        __ONEHOT_ENCODE1(x53, (bitPlace + 53ull))
#define __ONEHOT_ENCODE55(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, bitPlace)  \
        __ONEHOT_ENCODE54(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, bitPlace),      \
        __ONEHOT_ENCODE1(x54, (bitPlace + 54ull))
#define __ONEHOT_ENCODE56(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, bitPlace)     \
        __ONEHOT_ENCODE55(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, bitPlace), \
        __ONEHOT_ENCODE1(x55, (bitPlace + 55ull))
#define __ONEHOT_ENCODE57(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, bitPlace)        \
        __ONEHOT_ENCODE56(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, bitPlace),    \
        __ONEHOT_ENCODE1(x56, (bitPlace + 56ull))
#define __ONEHOT_ENCODE58(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, bitPlace)   \
        __ONEHOT_ENCODE57(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, bitPlace),       \
        __ONEHOT_ENCODE1(x57, (bitPlace + 57ull))
#define __ONEHOT_ENCODE59(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, bitPlace)      \
        __ONEHOT_ENCODE58(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, bitPlace),  \
        __ONEHOT_ENCODE1(x58, (bitPlace + 58ull))
#define __ONEHOT_ENCODE60(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, bitPlace) \
        __ONEHOT_ENCODE59(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, bitPlace),     \
        __ONEHOT_ENCODE1(x59, (bitPlace + 59ull))
#define __ONEHOT_ENCODE61(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, bitPlace) \
        __ONEHOT_ENCODE60(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, bitPlace),     \
        __ONEHOT_ENCODE1(x60, (bitPlace + 60ull))
#define __ONEHOT_ENCODE62(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, bitPlace)     \
        __ONEHOT_ENCODE61(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, bitPlace),\
        __ONEHOT_ENCODE1(x61, (bitPlace + 61ull))
#define __ONEHOT_ENCODE63(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, x62, bitPlace)        \
        __ONEHOT_ENCODE62(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, bitPlace),    \
        __ONEHOT_ENCODE1(x62, (bitPlace + 62ull))
#define __ONEHOT_ENCODE64(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, x62, x63, bitPlace)   \
        __ONEHOT_ENCODE63(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, x62, bitPlace),       \
        __ONEHOT_ENCODE1(x63, (bitPlace + 63ull))
#define __ONEHOT_EXTRACT_NUMARGS(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, x62, x63, N, ...) N
#define __ONEHOT_NUMARGS(...)   \
        __ONEHOT_EXTRACT_NUMARGS(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __ONEHOT_ENCODE_SPECIFICATION(N, ...) __ONEHOT_ENCODE ## N(__VA_ARGS__, 0ull)
#define __ONEHOT_ENCODE_FORWARD_NUMARGS(N, ...) __ONEHOT_ENCODE_SPECIFICATION(N, __VA_ARGS__)
#define ONEHOT_ENCODE(...) __ONEHOT_ENCODE_FORWARD_NUMARGS(__ONEHOT_NUMARGS(__VA_ARGS__), __VA_ARGS__)

#endif  // __onehot