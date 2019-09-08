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

#include "../include/bitarchiveitemoffset.hpp"

#include "../include/bitinputarchive.hpp"

using namespace bit7z;

BitArchiveItemOffset::BitArchiveItemOffset( uint32_t item_index, const BitInputArchive& item_arc )
    : BitArchiveItem( item_index ), mArc( &item_arc ) {}

BitArchiveItemOffset& BitArchiveItemOffset::operator++() {
    ++mItemIndex;
    return *this;
}

BitArchiveItemOffset BitArchiveItemOffset::operator++( int ) {
    BitArchiveItemOffset retval = *this;
    ++( *this );
    return retval;
}

bool BitArchiveItemOffset::operator==( const BitArchiveItemOffset& other ) const {
    return mItemIndex == other.mItemIndex;
}

bool BitArchiveItemOffset::operator!=( const BitArchiveItemOffset& other ) const {
    return !( *this == other );
}

BitPropVariant BitArchiveItemOffset::getProperty( BitProperty property ) const {
    return mArc != nullptr ? mArc->getItemProperty( mItemIndex, property ) : BitPropVariant();
}
