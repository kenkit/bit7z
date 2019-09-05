// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
 * bit7z - A C++ static library to interface with the 7-zip DLLs.
 * Copyright (c) 2014-2019  Riccardo Ostani - All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * Bit7z is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bit7z; if not, see https://www.gnu.org/licenses/.
 */

#include "../include/bitformat.hpp"

#ifdef BIT7Z_AUTO_FORMAT
#include "../include/bitexception.hpp"
#include "../include/fsutil.hpp"

#include <unordered_map>
#include <algorithm>
#include <cwctype>
#include <cstdint>

#include "7zip/IStream.h"
#endif

using namespace std;

namespace bit7z {
    namespace BitFormat {
#ifdef BIT7Z_AUTO_FORMAT
        const BitInFormat        Auto( 0x00 );
#endif
        const BitInOutFormat      Zip( 0x01, L".zip", BitCompressionMethod::Deflate,
                                       MULTIPLE_FILES | COMPRESSION_LEVEL | ENCRYPTION | MULTIPLE_METHODS );
        const BitInOutFormat    BZip2( 0x02, L".bz2", BitCompressionMethod::BZip2,
                                       COMPRESSION_LEVEL | INMEM_COMPRESSION );
        const BitInFormat         Rar( 0x03 );
        const BitInFormat         Arj( 0x04 );
        const BitInFormat           Z( 0x05 );
        const BitInFormat         Lzh( 0x06 );
        const BitInOutFormat SevenZip( 0x07, L".7z", BitCompressionMethod::Lzma2,
                                       MULTIPLE_FILES | SOLID_ARCHIVE | COMPRESSION_LEVEL |
                                       ENCRYPTION | HEADER_ENCRYPTION | MULTIPLE_METHODS );
        const BitInFormat         Cab( 0x08 );
        const BitInFormat        Nsis( 0x09 );
        const BitInFormat        Lzma( 0x0A );
        const BitInFormat      Lzma86( 0x0B );
        const BitInOutFormat       Xz( 0x0C, L".xz", BitCompressionMethod::Lzma2,
                                       COMPRESSION_LEVEL | ENCRYPTION | HEADER_ENCRYPTION | INMEM_COMPRESSION );
        const BitInFormat        Ppmd( 0x0D );
        const BitInFormat        COFF( 0xC6 );
        const BitInFormat         Ext( 0xC7 );
        const BitInFormat        VMDK( 0xC8 );
        const BitInFormat         VDI( 0xC9 );
        const BitInFormat        QCow( 0xCA );
        const BitInFormat         GPT( 0xCB );
        const BitInFormat        Rar5( 0xCC );
        const BitInFormat        IHex( 0xCD );
        const BitInFormat         Hxs( 0xCE );
        const BitInFormat          TE( 0xCF );
        const BitInFormat       UEFIc( 0xD0 );
        const BitInFormat       UEFIs( 0xD1 );
        const BitInFormat    SquashFS( 0xD2 );
        const BitInFormat      CramFS( 0xD3 );
        const BitInFormat         APM( 0xD4 );
        const BitInFormat        Mslz( 0xD5 );
        const BitInFormat         Flv( 0xD6 );
        const BitInFormat         Swf( 0xD7 );
        const BitInFormat        Swfc( 0xD8 );
        const BitInFormat        Ntfs( 0xD9 );
        const BitInFormat         Fat( 0xDA );
        const BitInFormat         Mbr( 0xDB );
        const BitInFormat         Vhd( 0xDC );
        const BitInFormat          Pe( 0xDD );
        const BitInFormat         Elf( 0xDE );
        const BitInFormat       Macho( 0xDF );
        const BitInFormat         Udf( 0xE0 );
        const BitInFormat         Xar( 0xE1 );
        const BitInFormat         Mub( 0xE2 );
        const BitInFormat         Hfs( 0xE3 );
        const BitInFormat         Dmg( 0xE4 );
        const BitInFormat    Compound( 0xE5 );
        const BitInOutFormat      Wim( 0xE6, L".wim", BitCompressionMethod::Copy, MULTIPLE_FILES );
        const BitInFormat         Iso( 0xE7 );
        const BitInFormat         Chm( 0xE9 );
        const BitInFormat       Split( 0xEA );
        const BitInFormat         Rpm( 0xEB );
        const BitInFormat         Deb( 0xEC );
        const BitInFormat        Cpio( 0xED );
        const BitInOutFormat      Tar( 0xEE, L".tar", BitCompressionMethod::Copy, MULTIPLE_FILES | INMEM_COMPRESSION );
        const BitInOutFormat     GZip( 0xEF, L".gz", BitCompressionMethod::Deflate,
                                       COMPRESSION_LEVEL | INMEM_COMPRESSION );

#ifdef BIT7Z_AUTO_FORMAT
		const unordered_map< wstring, const BitInFormat& > common_extensions = {
			{ L"7z", SevenZip },
			{ L"bzip2", BZip2 },
			{ L"bz2", BZip2 },
			{ L"tbz2", BZip2 },
			{ L"tbz", BZip2 },
			{ L"gz", GZip },
			{ L"gzip", GZip },
			{ L"tgz", GZip },
			{ L"tar", Tar },
			{ L"wim", Wim },
			{ L"swm", Wim },
			{ L"xz", Xz },
			{ L"txz", Xz },
			{ L"zip", Zip },
			{ L"zipx", Zip },
			{ L"jar", Zip },
			{ L"xpi", Zip },
			{ L"odt", Zip },
			{ L"ods", Zip },
			{ L"odp", Zip },
			{ L"docx", Zip },
			{ L"xlsx", Zip },
			{ L"pptx", Zip },
			{ L"epub", Zip },
			{ L"001", Split },
			{ L"ar", Deb },
			{ L"apm", APM },
			{ L"arj", Arj },
			{ L"cab", Cab },
			{ L"chm", Chm },
			{ L"chi", Chm },
			{ L"msi", Compound },
			{ L"doc", Compound },
			{ L"xls", Compound },
			{ L"ppt", Compound },
			{ L"msg", Compound },
			{ L"obj", COFF },
			{ L"cpio", Cpio },
			{ L"cramfs", CramFS },
			{ L"deb", Deb },
			{ L"dmg", Dmg },
			{ L"dll", Pe },
			{ L"dylib", Macho },
			{ L"exe", Pe }, //note: we do not distinguish 7z SFX exe at the moment!
			{ L"ext", Ext },
			{ L"ext2", Ext },
			{ L"ext3", Ext },
			{ L"ext4", Ext },
			{ L"fat", Fat },
			{ L"flv", Flv },
			{ L"gpt", GPT },
			{ L"hfs", Hfs },
			{ L"hfsx", Hfs },
			{ L"hxs", Hxs },
			{ L"ihex", IHex },
			{ L"lzh", Lzh },
			{ L"lha", Lzh },
			{ L"lzma", Lzma },
			{ L"lzma86", Lzma86 },
			{ L"mbr", Mbr },
			{ L"mslz", Mslz },
			{ L"mub", Mub },
			{ L"nsis", Nsis },
			{ L"ntfs", Ntfs },
			{ L"pmd", Ppmd },
			{ L"qcow", QCow },
			{ L"qcow2", QCow },
			{ L"qcow2c", QCow },
			{ L"rpm", Rpm },
			{ L"squashfs", SquashFS },
			{ L"te", TE },
			{ L"udf", Udf },
			{ L"scap", UEFIc },
			{ L"uefif", UEFIs },
			{ L"vmdk", VMDK },
			{ L"vdi", VDI },
			{ L"vhd", Vhd },
			{ L"xar", Xar },
			{ L"pkg", Xar },
			{ L"z", Z },
			{ L"taz", Z }
		};

		/* NOTE: For signatures with less than 8 bytes (size of uint64_t), remaining bytes are set to 0 */
		const unordered_map< uint64_t, const BitInFormat& > common_signatures = {
			{ 0x526172211A070000, Rar },      // R  a  r  !  1A 07 00
			{ 0x526172211A070100, Rar5 },     // R  a  r  !  1A 07 01 00
			{ 0x377ABCAF271C0000, SevenZip }, // 7  z  BC AF 27 1C
			{ 0x425A680000000000, BZip2 },    // B  Z  h
			{ 0x1F8B080000000000, GZip },     // 1F 8B 08
			{ 0x4D5357494D000000, Wim },      // M  S  W  I  M  00 00 00
			{ 0xFD377A585A000000, Xz },       // FD 7  z  X  Z  00
			{ 0x504B000000000000, Zip },      // P  K
			{ 0x4552000000000000, APM },      // E  R
			{ 0x60EA000000000000, Arj },      // `  EA
			{ 0x4D53434600000000, Cab },      // M  S  C  F  00 00 00 00
			{ 0x4954534603000000, Chm },      // I  T  S  F  03
			{ 0xD0CF11E0A1B11AE1, Compound }, // D0 CF 11 E0 A1 B1 1A E1
			{ 0xC771000000000000, Cpio },     // C7 q
			{ 0x71C7000000000000, Cpio },     // q  C7
			{ 0x3037303730000000, Cpio },     // 0  7  0  7  0
			{ 0x213C617263683E00, Deb },      // !  <  a  r  c  h  >  0A
			//{ 0x7801730D62626000, Dmg }, // DMG signature detection is not this simple
			{ 0x7F454C4600000000, Elf },      // 7F E  L  F
			{ 0x4D5A000000000000, Pe },       // M  Z
			{ 0x464C560100000000, Flv },      // F  L  V  01
			{ 0x5D00000000000000, Lzma },     //
			{ 0x015D000000000000, Lzma86 },   //
			{ 0xCEFAEDFE00000000, Macho },    // CE FA ED FE
			{ 0xCFFAEDFE00000000, Macho },    // CF FA ED FE
			{ 0xFEEDFACE00000000, Macho },    // FE ED FA CE
			{ 0xFEEDFACF00000000, Macho },    // FE ED FA CF
			{ 0xCAFEBABE00000000, Mub },      // CA FE BA BE 00 00 00
			{ 0xB9FAF10E00000000, Mub },      // B9 FA F1 0E
			{ 0x535A444488F02733, Mslz },     // S  Z  D  D  88 F0 '  3
			{ 0x8FAFAC8400000000, Ppmd },     // 8F AF AC 84
			{ 0x514649FB00000000, QCow },     // Q  F  I  FB 00 00 00
			{ 0xEDABEEDB00000000, Rpm },      // ED AB EE DB
			{ 0x7371736800000000, SquashFS }, // s  q  s  h
			{ 0x6873717300000000, SquashFS }, // h  s  q  s
			{ 0x7368737100000000, SquashFS }, // s  h  s  q
			{ 0x7173687300000000, SquashFS }, // q  s  h  s
			{ 0x4657530000000000, Swf },      // F  W  S
			{ 0x4357530000000000, Swfc },     // C  W  S
			{ 0x5A57530000000000, Swfc },     // Z  W  S
			{ 0x565A000000000000, TE },       // V  Z
			{ 0x4B444D0000000000, VMDK },     // K  D  M  V
			{ 0x3C3C3C2000000000, VDI },      // Alternatively 0x7F10DABE at offset 0x40)
			{ 0x636F6E6563746978, Vhd },      // c  o  n  e  c  t  i  x
			{ 0x78617221001C0000, Xar },      // x  a  r  !  00 1C
			{ 0x1F9D000000000000, Z },        // 1F 9D
			{ 0x1FA0000000000000, Z }        //
		};

        struct OffsetSignature {
            uint64_t signature;
            std::streamoff offset;
            uint32_t size;
            const BitInFormat& format;
        };

        const OffsetSignature common_signatures_with_offset[] = {
            { 0x2D6C680000000000, 0x02,  3, Lzh },    // -  l  h
            { 0x4E54465320202020, 0x03,  8, Ntfs },   // N  T  F  S  20 20 20 20
            { 0x4E756C6C736F6674, 0x08,  8, Nsis },   // N  u  l  l  s  o  f  t
            { 0x436F6D7072657373, 0x10,  8, CramFS }, // C  o  m  p  r  e  s  s
            { 0x7F10DABE00000000, 0x40,  4, VDI },    // 7F 10 DA BE
            { 0x7573746172000000, 0x101, 5, Tar },    // u  s  t  a  r
            // Note: since GPT files contain also the FAT signature, GPT must be checked before!
            { 0x4546492050415254, 0x200, 8, GPT },    // E  F  I  20 P  A  R  T
            { 0x55AA000000000000, 0x1FE, 2, Fat },    // U  AA
            { 0x4244000000000000, 0x400, 2, Hfs },    // B  D
            { 0x482B000400000000, 0x400, 4, Hfs },    // H  +  00 04
            { 0x4858000500000000, 0x400, 4, Hfs },    // H  X  00 05
            { 0x53EF000000000000, 0x438, 2, Ext }     // S  EF
        };

        uint64_t readSignature( IInStream* stream, uint32_t size ) {
            uint64_t signature = 0;
            stream->Read( &signature, size, nullptr );
            return _byteswap_uint64( signature );
        }

#if ( _MSC_VER <= 1800 )
#define CONSTEXPR const
#else
#define CONSTEXPR constexpr
#endif

        const BitInFormat& detectFormatFromSig( IInStream* stream ) {
            CONSTEXPR auto SIGNATURE_SIZE = 8u;

            uint64_t file_signature = readSignature( stream, SIGNATURE_SIZE );
            uint64_t signature_mask = 0xFFFFFFFFFFFFFFFFull;
            for ( auto i = 0u; i < SIGNATURE_SIZE - 1; ++i ) {
                auto it = common_signatures.find( file_signature );
                if ( it != common_signatures.end() ) {
                    stream->Seek( 0, 0, nullptr );
                    return it->second;
                }
                signature_mask <<= 8ull;          // left shifting the mask of 1 byte, so that
                file_signature &= signature_mask; // the least significant i bytes are masked (set to 0)
            }

            for ( auto& sig : common_signatures_with_offset ) {
                stream->Seek( sig.offset, 0, nullptr );
                file_signature = readSignature( stream, sig.size );
                if ( file_signature == sig.signature ) {
                    stream->Seek( 0, 0, nullptr );
                    return sig.format;
                }
            }

            // Detecting ISO/UDF
            CONSTEXPR auto ISO_SIGNATURE              = 0x4344303031000000; //CD001
            CONSTEXPR auto ISO_SIGNATURE_SIZE         = 5ull;
            CONSTEXPR auto ISO_SIGNATURE_OFFSET       = 0x8001;

            // Checking for ISO signature
            stream->Seek( ISO_SIGNATURE_OFFSET, 0, nullptr );
            file_signature = readSignature( stream, ISO_SIGNATURE_SIZE );
            if ( file_signature == ISO_SIGNATURE ) {
                CONSTEXPR auto MAX_VOLUME_DESCRIPTORS     = 16;
                CONSTEXPR auto ISO_VOLUME_DESCRIPTOR_SIZE = 0x800; //2048

                CONSTEXPR auto UDF_SIGNATURE          = 0x4E53523000000000; //NSR0
                CONSTEXPR auto UDF_SIGNATURE_SIZE     = 4u;

                // The file is ISO, checking if it is also UDF!
                for ( auto descriptor_index = 1ull; descriptor_index < MAX_VOLUME_DESCRIPTORS; ++descriptor_index ) {
                    stream->Seek( ISO_SIGNATURE_OFFSET + descriptor_index * ISO_VOLUME_DESCRIPTOR_SIZE, 0, nullptr );
                    file_signature = readSignature( stream, UDF_SIGNATURE_SIZE );
                    if ( file_signature == UDF_SIGNATURE ) {
                        stream->Seek( 0, 0, nullptr );
                        return Udf;
                    }
                }
                stream->Seek( 0, 0, nullptr );
                return Iso; //No UDF volume signature found, i.e. simple ISO!
            }

            stream->Seek( 0, 0, nullptr );
            throw BitException( "Cannot detect the format of the file" );
        }

        const BitInFormat& detectFormatFromExt( const wstring& in_file ) {
            wstring ext = filesystem::fsutil::extension( in_file );
            if ( ext.empty() ) {
                throw BitException( "Cannot detect the archive format from the extension" );
            }
            std::transform( ext.cbegin(), ext.cend(), ext.begin(), std::towlower );

            // Detecting archives with common file extensions
            auto it = common_extensions.find( ext );
            if ( it != common_extensions.end() ) { //extension found in map
                return it->second;
            }

            // Detecting multivolume archives extension
            if ( ( ext[ 0 ] == L'r' || ext[ 0 ] == L'z' ) &&
                    ( ext.size() == 3 && iswdigit( ext[ 1 ] ) != 0 && iswdigit( ext[ 2 ] ) != 0 ) ) {
                // Extension follows the format zXX or rXX, where X is a number in range [0-9]
                return ext[ 0 ] == L'r' ? Rar : Zip;
            }

            // Note: iso, img and ima extensions can be associated with different formats -> detect by signature

            // The extension did not match any known format extension, delegating the decision to the client
            return Auto;
        }
#endif

    }

}

using namespace bit7z;

BitInFormat::BitInFormat( unsigned char value ) : mValue( value ) {}

int BitInFormat::value() const {
    return mValue;
}

bool BitInFormat::operator==( const BitInFormat& other ) const {
    return mValue == other.value();
}

bool BitInFormat::operator!=( const BitInFormat& other ) const {
    return !( *this == other );
}

const GUID BitInFormat::guid() const {
    return { 0x23170F69, 0x40C1, 0x278A, { 0x10, 0x00, 0x00, 0x01, 0x10, mValue, 0x00, 0x00 } };
}

BitInOutFormat::BitInOutFormat( unsigned char value,
                                const wchar_t* ext,
                                BitCompressionMethod defaultMethod,
                                FeaturesSet features )
    : BitInFormat( value ), mExtension( ext ), mDefaultMethod( defaultMethod ), mFeatures( features ) {}

const wstring& BitInOutFormat::extension() const {
    return mExtension;
}

const FeaturesSet BitInOutFormat::features() const {
    return mFeatures;
}

bool BitInOutFormat::hasFeature( FormatFeatures feature ) const {
    return ( mFeatures & FeaturesSet( feature ) ) != 0;
}

BitCompressionMethod BitInOutFormat::defaultMethod() const {
    return mDefaultMethod;
}
