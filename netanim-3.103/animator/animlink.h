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

#ifndef ANIMLINK_H
#define ANIMLINK_H

#include "animatorconstants.h"
#include "main/common.h"
#include <QGraphicsLineItem>
#include <QString>

#include <map>

namespace netanim {

class AnimLink: public QGraphicsLineItem
{
public:
    AnimLink(uint32_t fromId, uint32_t toId, QLineF, QString pointADescription = "", QString pointBDescription = "", QString linkDescription = "", bool p2p = true);
    ~AnimLink();
    uint32_t m_fromId;
    uint32_t m_toId;
    void updateCurrentLinkDescription(QString linkDescription);
    void resetCurrentLinkDescription();
    QString toString();
    QPointF getInterfacePosA();
    QPointF getInterfacePosB();
    QString getInterfaceADescription();
    QString getInterfaceBDescription();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:

    QString * m_pointADescription;
    QString * m_pointBDescription;
    QString * m_currentLinkDescription;
    QString * m_originalLinkDescription;

    QPointF getLinkDescriptionCenter(QPainter *, QPointF *);
    QPointF m_interfacePosA;
    QPointF m_interfacePosB;
    bool    m_p2p;

};

class LinkManager {
public:
    typedef QVector <AnimLink *> AnimLinkVector_t;

    static LinkManager * getInstance();
    AnimLink * addLink(uint32_t fromId, uint32_t toId,
                       QLineF line, QString pointADescription,
                       QString pointBDescription, QString linkDescription, bool p2p = true);


    AnimLinkVector_t * getLinks();
    AnimLink * getAnimLink(uint32_t fromId, uint32_t toId);
    void systemReset();

private:
    LinkManager();
    AnimLinkVector_t             m_pointToPointLinks;

};



} // namespace netanim

#endif // ANIMLINK_H
