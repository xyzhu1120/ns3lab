/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: John Abraham <john.abraham@gatech.edu>
 */

#ifndef STATSMODE_H
#define STATSMODE_H

#include "main/common.h"

#include <QWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>

namespace netanim {

class StatsMode: public QWidget
{
    Q_OBJECT

public:
    // Getters
    static StatsMode * getInstance();
    QWidget * getCentralWidget();
    QString getTabName();

private:
    // Controls
    QWidget * m_centralWidget;
    QVBoxLayout * m_vLayout;
    QToolBar * m_nodeToolbar;
    QScrollArea * m_nodeToolbarScrollArea;

    // State
    uint32_t m_nodeCount;

    StatsMode();
    void init();
    void initControls();
    void initToolbars();
    void initNodeToolbar();

};

} // namespace netanim

#endif // STATSMODE_H
