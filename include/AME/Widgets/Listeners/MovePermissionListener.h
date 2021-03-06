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


#ifndef __AME_MOVEMENTPERMISSIONLISTENER_H__
#define __AME_MOVEMENTPERMISSIONLISTENER_H__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QtWidgets>
#include <QBoy/Config.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    MovePermissionListener.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    8/25/2016
    /// \brief   Listens to all events sent to "label".
    ///
    ///////////////////////////////////////////////////////////
    class MovePermissionListener : public QObject {
    Q_OBJECT
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes MovePermissionListener with a given parent.
        ///
        ///////////////////////////////////////////////////////////
        MovePermissionListener(QWidget *parent = NULL);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Destroys the MovePermissionListener.
        ///
        ///////////////////////////////////////////////////////////
        ~MovePermissionListener();

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Destroys the MovePermissionListener.
        ///
        ///////////////////////////////////////////////////////////
        UInt16 getSelectedIndex();



    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Handles all events.
        ///
        ///////////////////////////////////////////////////////////
        bool eventFilter(QObject *watched, QEvent *event);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        UInt8 m_SelectedIndex;
        UInt8 m_HighlightedBlock;
        Boolean m_ShowCursor;
        Boolean m_Selecting;

    };
}


#endif //__AME_MOVEMENTPERMISSIONLISTENER_H__
