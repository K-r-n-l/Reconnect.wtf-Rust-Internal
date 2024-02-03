#pragma once

#include "../Globals/Globals.hpp"


/*
The MIT License (MIT)

PASTA PASTA PASTA PASTA PASTA PASTA PASTA PASTA PASTA PASTA PASTA PASTA

Copyright (c) 2018 Ivor Wanders

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef COMPILE_TIME_CRC_H
#define COMPILE_TIME_CRC_H

#include <cstdint>
#include <type_traits>

// Based on code from Python's crcmod module. Polynomial "crc-32".
// polynomial: 0x104C11DB7, bit reverse algorithm: 
//  crcmod.Crc(0x104C11DB7, initCrc=0, xorOut=0xFFFFFFFF)

namespace crcdetail
{
    static constexpr const uint32_t table[256] =
    {
        0x00000000U, 0x77073096U, 0xEE0E612CU, 0x990951BAU, 0x076DC419U,
        0x706AF48FU, 0xE963A535U, 0x9E6495A3U, 0x0EDB8832U, 0x79DCB8A4U,
        0xE0D5E91EU, 0x97D2D988U, 0x09B64C2BU, 0x7EB17CBDU, 0xE7B82D07U,
        0x90BF1D91U, 0x1DB71064U, 0x6AB020F2U, 0xF3B97148U, 0x84BE41DEU,
        0x1ADAD47DU, 0x6DDDE4EBU, 0xF4D4B551U, 0x83D385C7U, 0x136C9856U,
        0x646BA8C0U, 0xFD62F97AU, 0x8A65C9ECU, 0x14015C4FU, 0x63066CD9U,
        0xFA0F3D63U, 0x8D080DF5U, 0x3B6E20C8U, 0x4C69105EU, 0xD56041E4U,
        0xA2677172U, 0x3C03E4D1U, 0x4B04D447U, 0xD20D85FDU, 0xA50AB56BU,
        0x35B5A8FAU, 0x42B2986CU, 0xDBBBC9D6U, 0xACBCF940U, 0x32D86CE3U,
        0x45DF5C75U, 0xDCD60DCFU, 0xABD13D59U, 0x26D930ACU, 0x51DE003AU,
        0xC8D75180U, 0xBFD06116U, 0x21B4F4B5U, 0x56B3C423U, 0xCFBA9599U,
        0xB8BDA50FU, 0x2802B89EU, 0x5F058808U, 0xC60CD9B2U, 0xB10BE924U,
        0x2F6F7C87U, 0x58684C11U, 0xC1611DABU, 0xB6662D3DU, 0x76DC4190U,
        0x01DB7106U, 0x98D220BCU, 0xEFD5102AU, 0x71B18589U, 0x06B6B51FU,
        0x9FBFE4A5U, 0xE8B8D433U, 0x7807C9A2U, 0x0F00F934U, 0x9609A88EU,
        0xE10E9818U, 0x7F6A0DBBU, 0x086D3D2DU, 0x91646C97U, 0xE6635C01U,
        0x6B6B51F4U, 0x1C6C6162U, 0x856530D8U, 0xF262004EU, 0x6C0695EDU,
        0x1B01A57BU, 0x8208F4C1U, 0xF50FC457U, 0x65B0D9C6U, 0x12B7E950U,
        0x8BBEB8EAU, 0xFCB9887CU, 0x62DD1DDFU, 0x15DA2D49U, 0x8CD37CF3U,
        0xFBD44C65U, 0x4DB26158U, 0x3AB551CEU, 0xA3BC0074U, 0xD4BB30E2U,
        0x4ADFA541U, 0x3DD895D7U, 0xA4D1C46DU, 0xD3D6F4FBU, 0x4369E96AU,
        0x346ED9FCU, 0xAD678846U, 0xDA60B8D0U, 0x44042D73U, 0x33031DE5U,
        0xAA0A4C5FU, 0xDD0D7CC9U, 0x5005713CU, 0x270241AAU, 0xBE0B1010U,
        0xC90C2086U, 0x5768B525U, 0x206F85B3U, 0xB966D409U, 0xCE61E49FU,
        0x5EDEF90EU, 0x29D9C998U, 0xB0D09822U, 0xC7D7A8B4U, 0x59B33D17U,
        0x2EB40D81U, 0xB7BD5C3BU, 0xC0BA6CADU, 0xEDB88320U, 0x9ABFB3B6U,
        0x03B6E20CU, 0x74B1D29AU, 0xEAD54739U, 0x9DD277AFU, 0x04DB2615U,
        0x73DC1683U, 0xE3630B12U, 0x94643B84U, 0x0D6D6A3EU, 0x7A6A5AA8U,
        0xE40ECF0BU, 0x9309FF9DU, 0x0A00AE27U, 0x7D079EB1U, 0xF00F9344U,
        0x8708A3D2U, 0x1E01F268U, 0x6906C2FEU, 0xF762575DU, 0x806567CBU,
        0x196C3671U, 0x6E6B06E7U, 0xFED41B76U, 0x89D32BE0U, 0x10DA7A5AU,
        0x67DD4ACCU, 0xF9B9DF6FU, 0x8EBEEFF9U, 0x17B7BE43U, 0x60B08ED5U,
        0xD6D6A3E8U, 0xA1D1937EU, 0x38D8C2C4U, 0x4FDFF252U, 0xD1BB67F1U,
        0xA6BC5767U, 0x3FB506DDU, 0x48B2364BU, 0xD80D2BDAU, 0xAF0A1B4CU,
        0x36034AF6U, 0x41047A60U, 0xDF60EFC3U, 0xA867DF55U, 0x316E8EEFU,
        0x4669BE79U, 0xCB61B38CU, 0xBC66831AU, 0x256FD2A0U, 0x5268E236U,
        0xCC0C7795U, 0xBB0B4703U, 0x220216B9U, 0x5505262FU, 0xC5BA3BBEU,
        0xB2BD0B28U, 0x2BB45A92U, 0x5CB36A04U, 0xC2D7FFA7U, 0xB5D0CF31U,
        0x2CD99E8BU, 0x5BDEAE1DU, 0x9B64C2B0U, 0xEC63F226U, 0x756AA39CU,
        0x026D930AU, 0x9C0906A9U, 0xEB0E363FU, 0x72076785U, 0x05005713U,
        0x95BF4A82U, 0xE2B87A14U, 0x7BB12BAEU, 0x0CB61B38U, 0x92D28E9BU,
        0xE5D5BE0DU, 0x7CDCEFB7U, 0x0BDBDF21U, 0x86D3D2D4U, 0xF1D4E242U,
        0x68DDB3F8U, 0x1FDA836EU, 0x81BE16CDU, 0xF6B9265BU, 0x6FB077E1U,
        0x18B74777U, 0x88085AE6U, 0xFF0F6A70U, 0x66063BCAU, 0x11010B5CU,
        0x8F659EFFU, 0xF862AE69U, 0x616BFFD3U, 0x166CCF45U, 0xA00AE278U,
        0xD70DD2EEU, 0x4E048354U, 0x3903B3C2U, 0xA7672661U, 0xD06016F7U,
        0x4969474DU, 0x3E6E77DBU, 0xAED16A4AU, 0xD9D65ADCU, 0x40DF0B66U,
        0x37D83BF0U, 0xA9BCAE53U, 0xDEBB9EC5U, 0x47B2CF7FU, 0x30B5FFE9U,
        0xBDBDF21CU, 0xCABAC28AU, 0x53B39330U, 0x24B4A3A6U, 0xBAD03605U,
        0xCDD70693U, 0x54DE5729U, 0x23D967BFU, 0xB3667A2EU, 0xC4614AB8U,
        0x5D681B02U, 0x2A6F2B94U, 0xB40BBE37U, 0xC30C8EA1U, 0x5A05DF1BU,
        0x2D02EF8DU
    };

    constexpr uint32_t compute(const char* data, uint32_t len, uint32_t crc = 0) {
        crc = crc ^ 0xFFFFFFFFU;
        for (uint32_t i = 0; i < len; i++) {
            crc = table[(*data ^ crc) & 0xFF] ^ (crc >> 8);
            data++;
        }
        crc = crc ^ 0xFFFFFFFFU;
        return crc;
    }

    constexpr uint32_t compute(const wchar_t* data, uint32_t len, uint32_t crc = 0) {
        crc = crc ^ 0xFFFFFFFFU;
        for (uint32_t i = 0; i < len; i++) {
            crc = table[(*data ^ crc) & 0xFF] ^ (crc >> 8);
            data++;
        }
        crc = crc ^ 0xFFFFFFFFU;
        return crc;
    }
}  // namespace crcdetail

// Macro to be used, guarantees hash is computed at compile time.
#define STATIC_CRC32(A)                                                           \
    std::integral_constant<uint32_t, crcdetail::compute(A, sizeof(A)-1)>::value

#define RUNTIME_CRC32(B) crcdetail::compute(B, strlen(B))
#define RUNTIME_CRC32_W(B) crcdetail::compute(B, wcslen(B))

#endif  // COMPILE_TIME_CRC_H

/*
 * Copyright 2018-2020 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 // documentation is available at https://github.com/JustasMasiulis/lazy_importer

#ifndef LAZY_IMPORTER_HPP
#define LAZY_IMPORTER_HPP

#define LI_FN(name) \
    ::li::detail::lazy_function<::li::detail::khash(#name), decltype(&name)>()

#define LI_FIND(name)                  \
    reinterpret_cast<decltype(&name)>( \
        ::li::detail::find_nocache<::li::detail::khash(#name)>())
#define LI_MODULE(name) ::li::detail::module_handle<::li::detail::khash(name)>()



// NOTE only std::forward is used from this header.
// If there is a need to eliminate this dependency the function itself is very small.
#include <utility>
#include <cstddef>
#include <intrin.h>

#ifndef LAZY_IMPORTER_NO_FORCEINLINE
#if defined(_MSC_VER)
#define LAZY_IMPORTER_FORCEINLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ > 3
#define LAZY_IMPORTER_FORCEINLINE inline __attribute_xorstr((__always_inline__))
#else
#define LAZY_IMPORTER_FORCEINLINE inline
#endif
#else
#define LAZY_IMPORTER_FORCEINLINE inline
#endif

#ifdef LAZY_IMPORTER_CASE_INSENSITIVE
#define LAZY_IMPORTER_TOLOWER(c) (c >= 'A' && c <= 'Z' ? (c | (1 << 5)) : c)
#else
#define LAZY_IMPORTER_TOLOWER(c) (c)
#endif
#define LI_FIND(name)                  \
    reinterpret_cast<decltype(&name)>( \
        ::li::detail::find_nocache<::li::detail::khash(#name)>())

#define LI_FIND_DEF(name) \
    reinterpret_cast<name>(::li::detail::find_nocache<::li::detail::khash(#name)>())

#define LI_MODULE(name) ::li::detail::module_handle<::li::detail::khash(name)>()

// returns dll base address or nullptr if it does not exist
#define LI_MODULE_SAFE_(name) reinterpret_cast<uintptr_t>(::li::detail::module_handle_safe(::li::detail::hash(name)))

// returns dll size or 0 if it does not exist
#define LI_MODULESIZE_SAFE_(name) ::li::detail::module_size_safe(::li::detail::hash(name))

namespace li {
    namespace detail {

        template<class First, class Second>
        struct pair {
            First  first;
            Second second;
        };

        namespace win {

            struct LIST_ENTRY_T {
                const char* Flink;
                const char* Blink;
            };

            struct UNICODE_STRING_T {
                unsigned short Length;
                unsigned short MaximumLength;
                wchar_t* Buffer;
            };

            struct PEB_LDR_DATA_T {
                unsigned long Length;
                unsigned long Initialized;
                const char* SsHandle;
                LIST_ENTRY_T  InLoadOrderModuleList;
            };

            struct PEB_T {
                unsigned char   Reserved1[2];
                unsigned char   BeingDebugged;
                unsigned char   Reserved2[1];
                const char* Reserved3[2];
                PEB_LDR_DATA_T* Ldr;
            };

            struct LDR_DATA_TABLE_ENTRY_T {
                LIST_ENTRY_T InLoadOrderLinks;
                LIST_ENTRY_T InMemoryOrderLinks;
                LIST_ENTRY_T InInitializationOrderLinks;
                const char* DllBase;
                const char* EntryPoint;
                union {
                    unsigned long SizeOfImage;
                    const char* _dummy;
                };
                UNICODE_STRING_T FullDllName;
                UNICODE_STRING_T BaseDllName;

                LAZY_IMPORTER_FORCEINLINE const LDR_DATA_TABLE_ENTRY_T*
                    load_order_next() const noexcept
                {
                    return reinterpret_cast<const LDR_DATA_TABLE_ENTRY_T*>(
                        InLoadOrderLinks.Flink);
                }
            };

            struct IMAGE_DOS_HEADER { // DOS .EXE header
                unsigned short e_magic; // Magic number
                unsigned short e_cblp; // Bytes on last page of file
                unsigned short e_cp; // Pages in file
                unsigned short e_crlc; // Relocations
                unsigned short e_cparhdr; // Size of header in paragraphs
                unsigned short e_minalloc; // Minimum extra paragraphs needed
                unsigned short e_maxalloc; // Maximum extra paragraphs needed
                unsigned short e_ss; // Initial (relative) SS value
                unsigned short e_sp; // Initial SP value
                unsigned short e_csum; // Checksum
                unsigned short e_ip; // Initial IP value
                unsigned short e_cs; // Initial (relative) CS value
                unsigned short e_lfarlc; // File address of relocation table
                unsigned short e_ovno; // Overlay number
                unsigned short e_res[4]; // Reserved words
                unsigned short e_oemid; // OEM identifier (for e_oeminfo)
                unsigned short e_oeminfo; // OEM information; e_oemid specific
                unsigned short e_res2[10]; // Reserved words
                long           e_lfanew; // File address of new exe header
            };

            struct IMAGE_FILE_HEADER {
                unsigned short Machine;
                unsigned short NumberOfSections;
                unsigned long  TimeDateStamp;
                unsigned long  PointerToSymbolTable;
                unsigned long  NumberOfSymbols;
                unsigned short SizeOfOptionalHeader;
                unsigned short Characteristics;
            };

            struct IMAGE_EXPORT_DIRECTORY {
                unsigned long  Characteristics;
                unsigned long  TimeDateStamp;
                unsigned short MajorVersion;
                unsigned short MinorVersion;
                unsigned long  Name;
                unsigned long  Base;
                unsigned long  NumberOfFunctions;
                unsigned long  NumberOfNames;
                unsigned long  AddressOfFunctions; // RVA from base of image
                unsigned long  AddressOfNames; // RVA from base of image
                unsigned long  AddressOfNameOrdinals; // RVA from base of image
            };

            struct IMAGE_DATA_DIRECTORY {
                unsigned long VirtualAddress;
                unsigned long Size;
            };

            struct IMAGE_OPTIONAL_HEADER64 {
                unsigned short       Magic;
                unsigned char        MajorLinkerVersion;
                unsigned char        MinorLinkerVersion;
                unsigned long        SizeOfCode;
                unsigned long        SizeOfInitializedData;
                unsigned long        SizeOfUninitializedData;
                unsigned long        AddressOfEntryPoint;
                unsigned long        BaseOfCode;
                unsigned long long   ImageBase;
                unsigned long        SectionAlignment;
                unsigned long        FileAlignment;
                unsigned short       MajorOperatingSystemVersion;
                unsigned short       MinorOperatingSystemVersion;
                unsigned short       MajorImageVersion;
                unsigned short       MinorImageVersion;
                unsigned short       MajorSubsystemVersion;
                unsigned short       MinorSubsystemVersion;
                unsigned long        Win32VersionValue;
                unsigned long        SizeOfImage;
                unsigned long        SizeOfHeaders;
                unsigned long        CheckSum;
                unsigned short       Subsystem;
                unsigned short       DllCharacteristics;
                unsigned long long   SizeOfStackReserve;
                unsigned long long   SizeOfStackCommit;
                unsigned long long   SizeOfHeapReserve;
                unsigned long long   SizeOfHeapCommit;
                unsigned long        LoaderFlags;
                unsigned long        NumberOfRvaAndSizes;
                IMAGE_DATA_DIRECTORY DataDirectory[16];
            };

            struct IMAGE_OPTIONAL_HEADER32 {
                unsigned short       Magic;
                unsigned char        MajorLinkerVersion;
                unsigned char        MinorLinkerVersion;
                unsigned long        SizeOfCode;
                unsigned long        SizeOfInitializedData;
                unsigned long        SizeOfUninitializedData;
                unsigned long        AddressOfEntryPoint;
                unsigned long        BaseOfCode;
                unsigned long        BaseOfData;
                unsigned long        ImageBase;
                unsigned long        SectionAlignment;
                unsigned long        FileAlignment;
                unsigned short       MajorOperatingSystemVersion;
                unsigned short       MinorOperatingSystemVersion;
                unsigned short       MajorImageVersion;
                unsigned short       MinorImageVersion;
                unsigned short       MajorSubsystemVersion;
                unsigned short       MinorSubsystemVersion;
                unsigned long        Win32VersionValue;
                unsigned long        SizeOfImage;
                unsigned long        SizeOfHeaders;
                unsigned long        CheckSum;
                unsigned short       Subsystem;
                unsigned short       DllCharacteristics;
                unsigned long        SizeOfStackReserve;
                unsigned long        SizeOfStackCommit;
                unsigned long        SizeOfHeapReserve;
                unsigned long        SizeOfHeapCommit;
                unsigned long        LoaderFlags;
                unsigned long        NumberOfRvaAndSizes;
                IMAGE_DATA_DIRECTORY DataDirectory[16];
            };

            struct IMAGE_NT_HEADERS {
                unsigned long     Signature;
                IMAGE_FILE_HEADER FileHeader;
#ifdef _WIN64
                IMAGE_OPTIONAL_HEADER64 OptionalHeader;
#else
                IMAGE_OPTIONAL_HEADER32 OptionalHeader;
#endif
            };

        } // namespace win

        // hashing stuff
        struct hash_t {
            using value_type = unsigned long;
            constexpr static value_type         offset = 2166136261;
            constexpr static value_type         prime = 16777619;
            constexpr static unsigned long long prime64 = prime;

            LAZY_IMPORTER_FORCEINLINE constexpr static value_type single(value_type value,
                char c) noexcept
            {
                return static_cast<hash_t::value_type>(
                    (value ^ LAZY_IMPORTER_TOLOWER(c)) *
                    static_cast<unsigned long long>(prime));
            }
        };

        template<class CharT = char>
        LAZY_IMPORTER_FORCEINLINE constexpr hash_t::value_type
            khash(const CharT* str, hash_t::value_type value = hash_t::offset) noexcept
        {
            return (*str ? khash(str + 1, hash_t::single(value, *str)) : value);
        }

        template<class CharT = char>
        LAZY_IMPORTER_FORCEINLINE hash_t::value_type hash(const CharT* str) noexcept
        {
            hash_t::value_type value = hash_t::offset;

            for (;;) {
                char c = *str++;
                if (!c)
                    return value;
                value = hash_t::single(value, c);
            }
        }

        LAZY_IMPORTER_FORCEINLINE hash_t::value_type hash(
            const win::UNICODE_STRING_T& str) noexcept
        {
            auto       first = str.Buffer;
            const auto last = first + (str.Length / sizeof(wchar_t));
            auto       value = hash_t::offset;
            for (; first != last; ++first)
                value = hash_t::single(value, static_cast<char>(*first));

            return value;
        }

        LAZY_IMPORTER_FORCEINLINE pair<hash_t::value_type, hash_t::value_type> hash_forwarded(
            const char* str) noexcept
        {
            pair<hash_t::value_type, hash_t::value_type> module_and_function{
                hash_t::offset, hash_t::offset
            };

            for (; *str != '.'; ++str)
                module_and_function.first = hash_t::single(module_and_function.first, *str);

            ++str;

            for (; *str; ++str)
                module_and_function.second = hash_t::single(module_and_function.second, *str);

            return module_and_function;
        }


        // some helper functions
        LAZY_IMPORTER_FORCEINLINE const win::PEB_T* peb() noexcept
        {
#if defined(_WIN64)
            return reinterpret_cast<const win::PEB_T*>(__readgsqword(0x60));
#elif defined(_WIN32)
            return reinterpret_cast<const win::PEB_T*>(__readfsdword(0x30));
#else
#error Unsupported platform. Open an issue and I'll probably add support.
#endif
        }

        LAZY_IMPORTER_FORCEINLINE const win::PEB_LDR_DATA_T* ldr()
        {
            return reinterpret_cast<const win::PEB_LDR_DATA_T*>(peb()->Ldr);
        }

        LAZY_IMPORTER_FORCEINLINE const win::IMAGE_NT_HEADERS* nt_headers(
            const char* base) noexcept
        {
            return reinterpret_cast<const win::IMAGE_NT_HEADERS*>(
                base + reinterpret_cast<const win::IMAGE_DOS_HEADER*>(base)->e_lfanew);
        }

        LAZY_IMPORTER_FORCEINLINE const win::IMAGE_EXPORT_DIRECTORY* image_export_dir(
            const char* base) noexcept
        {
            return reinterpret_cast<const win::IMAGE_EXPORT_DIRECTORY*>(
                base + nt_headers(base)->OptionalHeader.DataDirectory->VirtualAddress);
        }

        LAZY_IMPORTER_FORCEINLINE const win::LDR_DATA_TABLE_ENTRY_T* ldr_data_entry() noexcept
        {
            return reinterpret_cast<const win::LDR_DATA_TABLE_ENTRY_T*>(
                ldr()->InLoadOrderModuleList.Flink);
        }

        struct exports_directory {
            const char* _base;
            const win::IMAGE_EXPORT_DIRECTORY* _ied;
            unsigned long                      _ied_size;

        public:
            using size_type = unsigned long;

            LAZY_IMPORTER_FORCEINLINE
                exports_directory(const char* base) noexcept : _base(base)
            {
                const auto ied_data_dir = nt_headers(base)->OptionalHeader.DataDirectory[0];
                _ied = reinterpret_cast<const win::IMAGE_EXPORT_DIRECTORY*>(
                    base + ied_data_dir.VirtualAddress);
                _ied_size = ied_data_dir.Size;
            }

            LAZY_IMPORTER_FORCEINLINE explicit operator bool() const noexcept
            {
                return reinterpret_cast<const char*>(_ied) != _base;
            }

            LAZY_IMPORTER_FORCEINLINE size_type size() const noexcept
            {
                return _ied->NumberOfNames;
            }

            LAZY_IMPORTER_FORCEINLINE const char* base() const noexcept { return _base; }
            LAZY_IMPORTER_FORCEINLINE const win::IMAGE_EXPORT_DIRECTORY* ied() const noexcept
            {
                return _ied;
            }

            LAZY_IMPORTER_FORCEINLINE const char* name(size_type index) const noexcept
            {
                return reinterpret_cast<const char*>(
                    _base + reinterpret_cast<const unsigned long*>(
                        _base + _ied->AddressOfNames)[index]);
            }

            LAZY_IMPORTER_FORCEINLINE const char* address(size_type index) const noexcept
            {
                const auto* const rva_table =
                    reinterpret_cast<const unsigned long*>(_base + _ied->AddressOfFunctions);

                const auto* const ord_table = reinterpret_cast<const unsigned short*>(
                    _base + _ied->AddressOfNameOrdinals);

                return _base + rva_table[ord_table[index]];
            }

            LAZY_IMPORTER_FORCEINLINE bool is_forwarded(const char* export_address) const
                noexcept
            {
                const auto ui_ied = reinterpret_cast<const char*>(_ied);
                return (export_address > ui_ied && export_address < ui_ied + _ied_size);
            }
        };

        struct safe_module_enumerator {
            using value_type = const detail::win::LDR_DATA_TABLE_ENTRY_T;
            value_type* value;
            value_type* const head;

            LAZY_IMPORTER_FORCEINLINE safe_module_enumerator() noexcept
                : value(ldr_data_entry()), head(value)
            {}

            LAZY_IMPORTER_FORCEINLINE void reset() noexcept { value = head; }

            LAZY_IMPORTER_FORCEINLINE bool next() noexcept
            {
                value = value->load_order_next();
                return value != head && value->DllBase;
            }
        };

        struct unsafe_module_enumerator {
            using value_type = const detail::win::LDR_DATA_TABLE_ENTRY_T*;
            value_type value;

            LAZY_IMPORTER_FORCEINLINE unsafe_module_enumerator() noexcept
                : value(ldr_data_entry())
            {}

            LAZY_IMPORTER_FORCEINLINE void reset() noexcept { value = ldr_data_entry(); }

            LAZY_IMPORTER_FORCEINLINE bool next() noexcept
            {
                value = value->load_order_next();
                return true;
            }
        };

        // provides the cached functions which use Derive classes methods
        template<class Derived, class DefaultType = void*>
        class lazy_base {
        protected:
            // This function is needed because every templated function
            // with different args has its own static buffer
            LAZY_IMPORTER_FORCEINLINE static void*& _cache() noexcept
            {
                static void* value = nullptr;
                return value;
            }

        public:
            template<class T = DefaultType>
            LAZY_IMPORTER_FORCEINLINE static T safe() noexcept
            {
                return Derived::template get<T, safe_module_enumerator>();
            }

            template<class T = DefaultType, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static T cached() noexcept
            {
                auto& cached = _cache();
                if (!cached)
                    cached = Derived::template get<void*, Enum>();

                return (T)(cached);
            }

            template<class T = DefaultType>
            LAZY_IMPORTER_FORCEINLINE static T safe_cached() noexcept
            {
                return cached<T, safe_module_enumerator>();
            }
        };

        template<hash_t::value_type Hash>
        struct lazy_module : lazy_base<lazy_module<Hash>> {
            template<class T = void*, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static T get() noexcept
            {
                Enum e;
                do {
                    if (hash(e.value->BaseDllName) == Hash)
                        return (T)(e.value->DllBase);
                } while (e.next());
                return {};
            }
        };

        template<hash_t::value_type Hash, class T>
        struct lazy_function : lazy_base<lazy_function<Hash, T>, T> {
            using base_type = lazy_base<lazy_function<Hash, T>, T>;

            template<class... Args>
            LAZY_IMPORTER_FORCEINLINE decltype(auto) operator()(Args&&... args) const
            {
#ifndef LAZY_IMPORTER_CACHE_OPERATOR_PARENS
                return get()(std::forward<Args>(args)...);
#else
                return this->cached()(std::forward<Args>(args)...);
#endif
            }

            template<class F = T, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static F get() noexcept
            {
                // for backwards compatability.
                // Before 2.0 it was only possible to resolve forwarded exports when
                // this macro was enabled
#ifdef LAZY_IMPORTER_RESOLVE_FORWARDED_EXPORTS
                return forwarded<F, Enum>();
#else
                Enum e;
                do {
                    const exports_directory exports(e.value->DllBase);

                    if (exports) {
                        auto export_index = exports.size();
                        while (export_index--)
                            if (hash(exports.name(export_index)) == Hash)
                                return (F)(exports.address(export_index));
                    }
                } while (e.next());
                return {};
#endif
            }

            template<class F = T, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static F forwarded() noexcept
            {
                detail::win::UNICODE_STRING_T name;
                hash_t::value_type            module_hash = 0;
                auto                          function_hash = Hash;

                Enum e;
                do {
                    name = e.value->BaseDllName;
                    name.Length -= 8; // get rid of .dll extension

                    if (!module_hash || hash(name) == module_hash) {
                        const exports_directory exports(e.value->DllBase);

                        if (exports) {
                            auto export_index = exports.size();
                            while (export_index--)
                                if (hash(exports.name(export_index)) == function_hash) {
                                    const auto addr = exports.address(export_index);

                                    if (exports.is_forwarded(addr)) {
                                        auto hashes = hash_forwarded(
                                            reinterpret_cast<const char*>(addr));

                                        function_hash = hashes.second;
                                        module_hash = hashes.first;

                                        e.reset();
                                        break;
                                    }
                                    return (F)(addr);
                                }
                        }
                    }
                } while (e.next());
                return {};
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F forwarded_safe() noexcept
            {
                return forwarded<F, safe_module_enumerator>();
            }

            template<class F = T, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static F forwarded_cached() noexcept
            {
                auto& value = base_type::_cache();
                if (!value)
                    value = forwarded<void*, Enum>();
                return (F)(value);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F forwarded_safe_cached() noexcept
            {
                return forwarded_cached<F, safe_module_enumerator>();
            }

            template<class F = T, bool IsSafe = false, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in(Module m) noexcept
            {
                if (IsSafe && !m)
                    return {};

                const exports_directory exports((const char*)(m));
                if (IsSafe && !exports)
                    return {};

                for (unsigned long i{};; ++i) {
                    if (IsSafe && i == exports.size())
                        break;

                    if (hash(exports.name(i)) == Hash)
                        return (F)(exports.address(i));
                }
                return {};
            }

            template<class F = T, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in_safe(Module m) noexcept
            {
                return in<F, true>(m);
            }

            template<class F = T, bool IsSafe = false, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in_cached(Module m) noexcept
            {
                auto& value = base_type::_cache();
                if (!value)
                    value = in<void*, IsSafe>(m);
                return (F)(value);
            }

            template<class F = T, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in_safe_cached(Module m) noexcept
            {
                return in_cached<F, true>(m);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt() noexcept
            {
                return in<F>(ldr_data_entry()->load_order_next()->DllBase);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt_safe() noexcept
            {
                return in_safe<F>(ldr_data_entry()->load_order_next()->DllBase);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt_cached() noexcept
            {
                return in_cached<F>(ldr_data_entry()->load_order_next()->DllBase);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt_safe_cached() noexcept
            {
                return in_safe_cached<F>(ldr_data_entry()->load_order_next()->DllBase);
            }
        };

        struct allow_all_modules {
            LAZY_IMPORTER_FORCEINLINE constexpr bool
                operator()(const win::LDR_DATA_TABLE_ENTRY_T*) const noexcept
            {
                return true;
            }
        };

        template<hash_t::value_type Hash>
        LAZY_IMPORTER_FORCEINLINE const char* find_nocache() noexcept
        {
            return find_nocache(Hash, allow_all_modules{});
        }

        template<class ModuleFilter = allow_all_modules>
        LAZY_IMPORTER_FORCEINLINE const char* find_nocache(hash_t::value_type function_hash, ModuleFilter module_filter) noexcept
        {
            const auto* head = ldr_data_entry();

            while (true) {
                if (module_filter(head)) {
                    const exports_directory exports(head->DllBase);

                    if (exports)
                        for (auto i = 0u; i < exports.size(); ++i)
                            if (hash(exports.name(i)) == function_hash) {
                                const auto addr = exports.address(i);

#ifdef LAZY_IMPORTER_RESOLVE_FORWARDED_EXPORTS
                                if (exports.is_forwarded(addr)) {
                                    auto hashes =
                                        hash_forwarded(reinterpret_cast<const char*>(addr));
                                    return find_nocache(hashes.second,
                                        modules_by_hash{ hashes.first });
                                }
#endif
                                return addr;
                            }
                }

                head = head->load_order_next();
            }
        }

        LAZY_IMPORTER_FORCEINLINE size_t module_size_safe(hash_t::value_type h) {
            const auto head = ldr_data_entry();
            auto       it = head;
            while (true) {
                if (hash(it->BaseDllName) == h)
                    return it->SizeOfImage;

                if (it->InLoadOrderLinks.Flink == reinterpret_cast<const char*>(head))
                    return 0;

                it = it->load_order_next();
            }
        }

        LAZY_IMPORTER_FORCEINLINE const char* module_handle_safe(hash_t::value_type h) {
            const auto head = ldr_data_entry();
            auto       it = head;
            while (true) {
                if (hash(it->BaseDllName) == h)
                    return it->DllBase;

                if (it->InLoadOrderLinks.Flink == reinterpret_cast<const char*>(head))
                    return 0;

                it = it->load_order_next();
            }
        }
    }
} // namespace li::detail

#endif // include guard