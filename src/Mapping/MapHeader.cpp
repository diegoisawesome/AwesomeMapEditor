//////////////////////////////////////////////////////////////////////////////////
//
//
//                     d88b         888b           d888  888888888888
//                    d8888b        8888b         d8888  888
//                   d88''88b       888'8b       d8'888  888
//                  d88'  '88b      888 '8b     d8' 888  8888888
//                 d88Y8888Y88b     888  '8b   d8'  888  888
//                d88""""""""88b    888   '8b d8'   888  888
//               d88'        '88b   888    '888'    888  888
//              d88'          '88b  888     '8'     888  888888888888
//
//
// AwesomeMapEditor: A map editor for GBA Pokémon games.
// Copyright (C) 2016 Diegoisawesome, Pokedude
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/System/Configuration.hpp>
#include <AME/Mapping/MappingErrors.hpp>
#include <AME/Mapping/MapHeader.hpp>
#include <AME/Graphics/TilesetManager.hpp>
#include <QSize>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapHeader::MapHeader()
        : /*IUndoable(),*/
          m_Offset(0),
          m_Width(0),
          m_Height(0),
          m_PtrBlocks(0),
          m_PtrBorder(0),
          m_PtrPrimary(0),
          m_PtrSecondary(0)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapHeader::MapHeader(const MapHeader &rvalue)
        : /*IUndoable(),*/
          m_Offset(rvalue.m_Offset),
          m_Width(rvalue.m_Width),
          m_Height(rvalue.m_Height),
          m_PtrBlocks(rvalue.m_PtrBlocks),
          m_PtrBorder(rvalue.m_PtrBlocks),
          m_PtrPrimary(rvalue.m_PtrPrimary),
          m_PtrSecondary(rvalue.m_PtrSecondary),
          m_Blocks(rvalue.m_Blocks),
          m_Primary(rvalue.m_Primary),
          m_Secondary(rvalue.m_Secondary)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapHeader &MapHeader::operator=(const MapHeader &rvalue)
    {
        m_Blocks = rvalue.m_Blocks;
        m_Width = rvalue.m_Width;
        m_Height = rvalue.m_Height;
        m_PtrBlocks = rvalue.m_PtrBlocks;
        m_PtrBorder = rvalue.m_PtrBlocks;
        m_PtrPrimary = rvalue.m_PtrPrimary;
        m_PtrSecondary = rvalue.m_PtrSecondary;
        m_Primary = rvalue.m_Primary;
        m_Secondary = rvalue.m_Secondary;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapHeader::~MapHeader()
    {
        foreach (MapBlock *block, m_Blocks)
            delete block;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    bool MapHeader::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(HDR_ERROR_OFFSET, rom.redirected());


        // Reads the map's dimensions
        m_Width = rom.readWord();
        m_Height = rom.readWord();

        // Reads all the pointers within the structure
        m_PtrBorder = rom.readPointerRef();
        m_PtrBlocks = rom.readPointer();
        m_PtrPrimary = rom.readPointerRef();
        m_PtrSecondary = rom.readPointerRef();

        // Determines whether these pointers are valid
        if (!rom.checkOffset(m_PtrBorder))
            AME_THROW(HDR_ERROR_BORDER, offset + 8);
        if (!rom.checkOffset(m_PtrBlocks))
            AME_THROW(HDR_ERROR_BLOCKS, offset + 12);
        if (!rom.checkOffset(m_PtrPrimary))
            AME_THROW(HDR_ERROR_PRIMARY, offset + 16);
        if (!rom.checkOffset(m_PtrSecondary))
            AME_THROW(HDR_ERROR_SECONDARY, offset + 20);


        // Retrieves the border (different between games!)
        if (CONFIG(RomType) == RT_FRLG)
        {
            QSize borderSize(rom.readByte(), rom.readByte());
            m_Border.read(rom, m_PtrBorder, borderSize);
        }
        else
        {
            m_Border.read(rom, m_PtrBorder, QSize(2, 2));
        }

        // Retrieves the map block data
        rom.seek(m_PtrBlocks);
        for (unsigned i = 0; i < m_Width * m_Height; i++)
        {
            MapBlock *block = new MapBlock;
            UInt16 data = rom.readHWord();

            block->block = (data & 0x3FF);
            block->permission = (data >> 0xA);
            m_Blocks.push_back(block);
        }

        // Loads the tilesets, if necessary
        if ((m_Primary = TilesetManager::get(m_PtrPrimary)) == NULL)
        {
            m_Primary = new Tileset;
            m_Primary->read(rom, m_PtrPrimary);
            TilesetManager::add(m_Primary);
        }
        if ((m_Secondary = TilesetManager::get(m_PtrSecondary)) == NULL)
        {
            m_Secondary = new Tileset;
            m_Secondary->read(rom, m_PtrSecondary);
            TilesetManager::add(m_Secondary);
        }


        // Loading successful
        m_Offset = offset;
        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapBorder &MapHeader::border()
    {
        return m_Border;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<MapBlock *> &MapHeader::blocks() const
    {
        return m_Blocks;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   11/9/2016
    //
    ///////////////////////////////////////////////////////////
    MapBlock *MapHeader::getBlock(int x, int y)
    {
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
            return NULL;
        return m_Blocks[x + (m_Width * y)];
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   11/9/2016
    //
    ///////////////////////////////////////////////////////////
    MapBlock *MapHeader::getBlock(QPoint pos)
    {
        return this->getBlock(pos.x(), pos.y());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/4/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 MapHeader::offset() const
    {
        return m_Offset;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 MapHeader::ptrPrimary() const
    {
        return m_PtrPrimary;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 MapHeader::ptrSecondary() const
    {
        return m_PtrSecondary;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    Tileset *MapHeader::primary() const
    {
        return m_Primary;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    Tileset *MapHeader::secondary() const
    {
        return m_Secondary;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    const QSize MapHeader::size() const
    {
        return QSize(m_Width, m_Height);
    }   

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/2/2016
    //
    //////////////////////////////////////////////////////////
    QByteArray MapHeader::rawData()
    {
        QByteArray ba;
        ba.append((Int8 *) &m_Width, 4);
        ba.append((Int8 *) &m_Height, 4);
        m_PtrBorder += 0x08000000;
        m_PtrBlocks += 0x08000000;
        m_PtrPrimary += 0x08000000;
        m_PtrSecondary += 0x08000000;
        ba.append((Int8 *) &m_PtrBorder, 4);
        ba.append((Int8 *) &m_PtrBlocks, 4);
        ba.append((Int8 *) &m_PtrPrimary, 4);
        ba.append((Int8 *) &m_PtrSecondary, 4);
        m_PtrBorder -= 0x08000000;
        m_PtrBlocks -= 0x08000000;
        m_PtrPrimary -= 0x08000000;
        m_PtrSecondary -= 0x08000000;

        if (CONFIG(RomType) == RT_FRLG)
        {
            ba.push_back((Int8) m_Border.height());
            ba.push_back((Int8) m_Border.width());
        }

        return ba;
    }
}
