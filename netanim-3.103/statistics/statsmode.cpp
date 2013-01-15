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

#include "statsmode.h"

namespace netanim {

StatsMode * pStatsMode = 0;

StatsMode *
StatsMode::getInstance()
{
    if(!pStatsMode)
    {
        pStatsMode = new StatsMode();
    }
    return pStatsMode;
}

StatsMode::StatsMode():
    m_nodeCount(1000)
{
    init();
}

QWidget *
StatsMode::getCentralWidget()
{
    return m_centralWidget;
}

QString
StatsMode::getTabName()
{
    return "Stats";
}

void
StatsMode::init()
{
    initControls();
    initToolbars();

    m_vLayout = new QVBoxLayout;
    m_vLayout->addWidget(m_nodeToolbarScrollArea);

    m_centralWidget = new QWidget;
    m_centralWidget->setLayout(m_vLayout);
    setWindowTitle("NetAnim");
}

void
StatsMode::initControls()
{

}

void
StatsMode::initToolbars()
{
    initNodeToolbar();
}

void
StatsMode::initNodeToolbar()
{
    m_nodeToolbarScrollArea = new QScrollArea;
    m_nodeToolbar = new QToolBar;
    m_nodeToolbar->setOrientation(Qt::Vertical);
    for (uint32_t i = 0; i < m_nodeCount; ++i)
    {
        QPushButton * button = new QPushButton(QString::number(i));
        m_nodeToolbar->addWidget(button);
    }
    m_nodeToolbarScrollArea->setWidget(m_nodeToolbar);
}

} // namespace netanim
