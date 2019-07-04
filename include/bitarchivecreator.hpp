/*
 * bit7z - A C++ static library to interface with the 7-zip DLLs.
 * Copyright (c) 2014-2018  Riccardo Ostani - All Rights Reserved.
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

#ifndef BITARCHIVECREATOR_HPP
#define BITARCHIVECREATOR_HPP

#include "../include/bitarchivehandler.hpp"
#include "../include/bitinputarchive.hpp"
#include "../include/bitformat.hpp"
#include "../include/bitcompressionlevel.hpp"
#include "../include/bitcompressionmethod.hpp"

#include <memory>

struct IOutStream;
struct ISequentialOutStream;

namespace bit7z {
    using std::wstring;
    using std::unique_ptr;
    using std::ostream;

    class CompressCallback;

    /**
     * @brief Abstract class representing a generic archive creator.
     */
    class BitArchiveCreator : public BitArchiveHandler {
        public:
            /**
             * @brief BitArchiveCreator constructor.
             *
             * @param lib       the 7z library used.
             * @param format    the output archive format.
             */
            BitArchiveCreator( const Bit7zLibrary& lib, const BitInOutFormat& format );

            /**
             * @brief BitArchiveCreator destructor.
             */
            virtual ~BitArchiveCreator() override = 0;

            /**
             * @return the format used by the archive creator.
             */
            const BitInFormat& format() const override;

            /**
             * @return the format used by the archive creator.
             */
            const BitInOutFormat& compressionFormat() const;

            /**
             * @return whether the creator crypts also the headers of archives or not
             */
            bool cryptHeaders() const;

            /**
             * @return the compression level used by the archive creator.
             */
            BitCompressionLevel compressionLevel() const;

            BitCompressionMethod compressionMethod() const;

            uint32_t dictionarySize() const;

            /**
             * @return whether the archive creator uses solid compression or not.
             */
            bool solidMode() const;

            /**
             * @return whether the archive creator is allowed to update existing archives or not.
             */
            bool updateMode() const;

            /**
             * @return the size (in bytes) of the archive volume used by the creator
             *         (a 0 value means that all files are going in a single archive).
             */
            uint64_t volumeSize() const;

            /**
             * @brief Sets up a password for the output archive.
             *
             * When setting a password, the produced archive will be encrypted using the default
             * cryptographic method of the output format. The option "crypt headers" remains unchanged,
             * in contrast with what happens when calling the setPassword(wstring, bool) method.
             *
             * @note Calling setPassword when the output format doesn't support archive encryption
             * (e.g. GZip, BZip2, etc...) does not have any effects (in other words, it doesn't
             * throw exceptions and it has no effects on compression operations).
             *
             * @note After a password has been set, it will be used for every subsequent operation.
             * To disable the use of the password, you need to call the clearPassword method
             * (inherited from BitArchiveHandler), which is equivalent to setPassword(L"").
             *
             * @param password
             */
            void setPassword( const wstring& password ) override;

            /**
             * @brief Sets up a password for the output archive.
             *
             * When setting a password, the produced archive will be encrypted using the default
             * cryptographic method of the output format. If the format is 7z and the option
             * "crypt_headers" is set to true, also the headers of the archive will be encrypted,
             * resulting in a password request everytime the output file will be opened.
             *
             * @note Calling setPassword when the output format doesn't support archive encryption
             * (e.g. GZip, BZip2, etc...) does not have any effects (in other words, it doesn't
             * throw exceptions and it has no effects on compression operations).
             *
             * @note Calling setPassword with "crypt_headers" set to true does not have effects on
             * formats different from 7z.
             *
             * @note After a password has been set, it will be used for every subsequent operation.
             * To disable the use of the password, you need to call the clearPassword method
             * (inherited from BitArchiveHandler), which is equivalent to setPassword(L"").
             *
             * @param password          the password desired.
             * @param crypt_headers     if true, the headers of the output archive will be encrypted
             *                          (valid only with 7z format).
             */
            void setPassword( const wstring& password, bool crypt_headers );

            /**
             * @brief Sets the compression level to use when creating an archive.
             *
             * @param compression_level the compression level desired.
             */
            void setCompressionLevel( BitCompressionLevel compression_level );

            void setCompressionMethod( BitCompressionMethod compression_method );

            void setDictionarySize( uint32_t dictionary_size );

            /**
             * @brief Sets whether to use solid compression or not.
             *
             * @note Setting the solid compression mode to true has effect only when using the 7z format with multiple
             *       input files.
             *
             * @param solid_mode    if true, it will be used the "solid compression" method.
             */
            void setSolidMode( bool solid_mode );

            /**
             * @brief Sets whether the creator can update existing archives or not.
             *
             * @note If false, an exception will be thrown in case a compression operation targets an existing archive.
             *
             * @param update_archives if true, compressing operations will update existing archives.
             */
            void setUpdateMode( bool update_mode );

            /**
             * @brief Sets the size (in bytes) of the archive volumes.
             *
             * @note This setting has effects only when the destination archive is on filesystem.
             *
             * @param size    The dimension of a volume.
             */
            void setVolumeSize( uint64_t size );

        protected:
            CMyComPtr< IOutArchive > initOutArchive() const;

            CMyComPtr< IOutStream > initOutFileStream( const wstring& out_archive,
                                                       CMyComPtr< IOutArchive >& new_arc,
                                                       unique_ptr< BitInputArchive >& old_arc ) const;

            CMyComPtr< ISequentialOutStream > initOutMemStream( vector< byte_t >& out_buffer ) const;

            CMyComPtr< IOutStream > initOutStdStream( ostream &out_stream ) const;

            static HRESULT compressOut( IOutArchive* out_arc,
                                        ISequentialOutStream* out_stream,
                                        CompressCallback* update_callback );

            static void cleanupOldArc( BitInputArchive* old_arc,
                                       IOutStream* out_stream,
                                       const wstring& out_archive );

            void setArchiveProperties( IOutArchive* out_archive ) const;

            const BitInOutFormat& mFormat;
            BitCompressionLevel mCompressionLevel;
            BitCompressionMethod mCompressionMethod;
            uint32_t mDictionarySize;
            bool mCryptHeaders;
            bool mSolidMode;
            bool mUpdateMode;
            uint64_t mVolumeSize;
    };
}

#endif // BITARCHIVECREATOR_HPP
