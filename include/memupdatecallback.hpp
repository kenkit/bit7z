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

#ifndef MEMUPDATECALLBACK_HPP
#define MEMUPDATECALLBACK_HPP

#include "7zip/Archive/IArchive.h"
#include "7zip/IPassword.h"
#include "Common/MyCom.h"

#include "../include/bitarchivecreator.hpp"
#include "../include/bitinputarchive.hpp"
#include "../include/bittypes.hpp"
#include "../include/compresscallback.hpp"

#include <vector>

namespace bit7z {
    using std::vector;
    using std::wstring;

    class MemUpdateCallback : public CompressCallback {
        public:
            MemUpdateCallback( const BitArchiveCreator& creator,
                               const vector< byte_t >& in_buffer,
                               const wstring& in_buffer_name,
                               const BitInputArchive *old_arc = nullptr );

            virtual ~MemUpdateCallback();

            // IArchiveUpdateCallback2
            STDMETHOD( GetProperty )( UInt32 /*index*/, PROPID propID, PROPVARIANT* value );
            STDMETHOD( GetStream )( UInt32 /*index*/, ISequentialInStream** inStream );
            STDMETHOD( GetVolumeSize )( UInt32 /*index*/, UInt64* size );
            STDMETHOD( GetVolumeStream )( UInt32 /*index*/, ISequentialOutStream** volumeStream );

            uint32_t itemsCount() const override;

        private:
            const vector< byte_t >& mBuffer;
            const wstring& mBufferName;
    };
}
#endif // MEMUPDATECALLBACK_HPP
