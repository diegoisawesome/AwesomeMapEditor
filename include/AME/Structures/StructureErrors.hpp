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
// Copyright (C) 2016 diegoisawesome, Pokedude
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


#ifndef __AME_STRUCTUREERRORS_HPP__
#define __AME_STRUCTUREERRORS_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <AME/System/IUndoable.hpp>
#include <AME/System/WriteEntry.hpp>
#include <AME/Structures/WildPokemonSubTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    StructureErrors.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/3/2016
    /// \brief   Defines all errors for 'Structure' classes.
    ///
    ///////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    // Class: WildPokemonTable
    //
    ///////////////////////////////////////////////////////////
    #define WPT_ERROR_OFFSET    "The location of the wildpokemon head table (%offset%) is invalid."
    #define WPT_ERROR_RANGE     "The wildpokemon head table exceeded the ROM's range."
}


#endif // __AME_STRUCTUREERRORS_HPP__
