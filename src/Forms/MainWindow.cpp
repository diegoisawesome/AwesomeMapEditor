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


#include <AME/System/LoadedData.hpp>
#include <AME/System/Configuration.hpp>
#include <AME/Widgets/Misc/Messages.hpp>
#include <AME/Widgets/OpenGL/AMEMapView.h>
#include <AME/Forms/MainWindow.h>
#include <AME/Forms/ErrorWindow.h>
#include "ui_MainWindow.h"
#include <QFileDialog>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/2/2016
    // Comment:
    //
    // Added a function call to set the GUI up.
    //
    ///////////////////////////////////////////////////////////
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/2/2016
    // Comment:
    //
    // Added a call to delete the GUI.
    //
    ///////////////////////////////////////////////////////////
    MainWindow::~MainWindow()
    {
        delete ui;
    }



    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/16/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::openRomDialog()
    {
        /* Replace QDir::homePath with SETTINGS(LastPath) */

        QString file = QFileDialog::getOpenFileName(
                    this,
                    QString("Open ROM file"),
                    QDir::homePath(),
                    QString("ROMs (*.gba *.bin)")
        );


        // Determines whether the dialog was successful
        if (!file.isNull() && !file.isEmpty())
        {
            // Close a previous ROM and destroy objects
            if (m_Rom.info().isLoaded())
            {
                // Clears the ROM data
                m_Rom.clearCache();
                m_Rom.close();

                // Clears old data and UI
                clearAllMapData();
                //clearBeforeLoading();
            }

            // Attempts to open the new ROM file
            if (!m_Rom.loadFromFile(file))
            {
                m_Rom.clearCache();
                m_Rom.close();

                Messages::showError(this, WND_ERROR_ROM);
                return false;
            }

            // Loading successful
            return true;
        }

        // Dialog cancelled
        return false;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/16/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::loadMapData()
    {
        /* TODO: Design form to show error messages */

        bool result = loadAllMapData(m_Rom);
        if (!result)
        {
            ErrorWindow errorWindow(this);
            errorWindow.exec();
            return;
        }

        setupAfterLoading();
        m_Rom.clearCache();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::setupAfterLoading()
    {
        QStandardItemModel *model = new QStandardItemModel;
        for (unsigned i = 0; i < CONFIG(PokemonCount); i++)
        {
            QStandardItem *item = new QStandardItem;
            item->setText(dat_PokemonTable->names().at(i));
            item->setIcon(QIcon(QPixmap::fromImage(dat_PokemonTable->images().at(i))));
            model->appendRow(item);
        }

        // Fills all wild-pokemon comboboxes with the names
        foreach (QComboBox *box, ui->tabWidget_3->findChildren<QComboBox *>())
            box->setModel(model);

        // Sets the max pokemon IDs within the spinboxes
        foreach (QSpinBox *box, ui->tabWidget_3->findChildren<QSpinBox *>(QRegularExpression("sbWild")))
            box->setRange(0, CONFIG(PokemonCount));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::setupWildPokemon(Map *map)
    {
        WildPokemonSubTable *table = dat_WildPokemonTable->tables()[map->wildpokeTable()];
        WildPokemonArea &grass = table->encounter(EA_AREA_GRASS);
        WildPokemonArea &water = table->encounter(EA_AREA_WATER);
        WildPokemonArea &fish = table->encounter(EA_AREA_FISH);
        WildPokemonArea &rock = table->encounter(EA_AREA_ROCK);


        // Fills the encounter probabilities
        ui->sbWildGrassChance->setValue(grass.probability());
        ui->sbWildWaterChance->setValue(water.probability());
        ui->sbWildFishingChance->setValue(fish.probability());
        ui->sbWildOtherChance->setValue(rock.probability());

        // Fills the pokemon ID and min/max level
        for (int i = 0; i < grass.entries().size(); i++)
        {
            QString num = QString::number(i+1);
            WildPokemonEncounter *pkm = grass.entries().at(i);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWildGrass") + num)
                    ->setValue(pkm->id);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbGrassMin") + num)
                    ->setValue(pkm->min);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbGrassMax") + num)
                    ->setValue(pkm->max);
        }
        for (int i = 0; i < water.entries().size(); i++)
        {
            QString num = QString::number(i+1);
            WildPokemonEncounter *pkm = water.entries().at(i);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWildWater") + num)
                    ->setValue(pkm->id);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWaterMin") + num)
                    ->setValue(pkm->min);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWaterMax") + num)
                    ->setValue(pkm->max);
        }
        for (int i = 0; i < fish.entries().size(); i++)
        {
            QString num = QString::number(i+1);
            WildPokemonEncounter *pkm = fish.entries().at(i);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWildFishing") + num)
                    ->setValue(pkm->id);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbFishingMin") + num)
                    ->setValue(pkm->min);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbFishingMax") + num)
                    ->setValue(pkm->max);
        }
        for (int i = 0; i < rock.entries().size(); i++)
        {
            QString num = QString::number(i+1);
            WildPokemonEncounter *pkm = rock.entries().at(i);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWildOther") + num)
                    ->setValue(pkm->id);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbOtherMin") + num)
                    ->setValue(pkm->min);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbOtherMax") + num)
                    ->setValue(pkm->max);
        }
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/16/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_action_Open_ROM_triggered()
    {
        if (openRomDialog())
            loadMapData();
    }

    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionRecent_Files_triggered()
    {
        ui->openGLWidget_2->setMap(m_Rom, dat_MapBankTable->banks()[1]->maps()[0]);
        ui->openGLWidget_2->makeGL();
        ui->openGLWidget_2->update();
        ui->openGLWidget_3->setMapView(ui->openGLWidget_2);
        ui->openGLWidget_3->update();
        ui->openGLWidget->setMapView(ui->openGLWidget_2);
        ui->openGLWidget->update();
        ui->openGLWidget_5->setEntities(dat_MapBankTable->banks()[1]->maps()[0]);
        ui->openGLWidget_5->setMapView(ui->openGLWidget_2);
        ui->openGLWidget_5->update();
        setupWildPokemon(dat_MapBankTable->banks()[1]->maps()[0]);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_sldWildGrassChance_valueChanged(int value)
    {
        ui->lblWildGrassChance->setText(QString::number((int)((value / 255.0)*100)) + QString("%"));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_sldWildWaterChance_valueChanged(int value)
    {
        ui->lblWildWaterChance->setText(QString::number((int)((value / 255.0)*100)) + QString("%"));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_sldWildFishingChance_valueChanged(int value)
    {
        ui->lblWildFishingChance->setText(QString::number((int)((value / 255.0)*100)) + QString("%"));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_sldWildOtherChance_valueChanged(int value)
    {
        ui->lblWildOtherChance->setText(QString::number((int)((value / 255.0)*100)) + QString("%"));
    }
}
