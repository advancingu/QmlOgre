/***************************************************************************
**
** This file is part of the qmlogre example on http://qt.gitorious.org.
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).*
**
** All rights reserved.
** Contact:  Nokia Corporation (qt-info@nokia.com)
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**  * Redistributions of source code must retain the above copyright notice,
**    this list of conditions and the following disclaimer.
**  * Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and/or other materials provided with ** the distribution.
**  * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor the
**    names of its contributors may be used to endorse or promote products
**    derived from this software without specific ** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**************************************************************************/

#ifndef OGREITEM_H
#define OGREITEM_H

#include "ogreengineitem.h"
#include "ogrecamerawrapper.h"

#include <QtQuick/QQuickItem>
#include <QtCore/QPropertyAnimation>

class OgreItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QObject *camera READ camera WRITE setCamera)
    Q_PROPERTY(OgreEngineItem *ogreEngine READ ogreEngine WRITE setOgreEngine)

public:
    OgreItem(QQuickItem *parent = 0);

    QObject *camera() const { return qobject_cast<QObject *>(m_camera); }
    void setCamera(QObject *camera);

    OgreEngineItem *ogreEngine() const { return m_ogreEngineItem; }
    void setOgreEngine(OgreEngineItem *ogreEngine);

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    void timerEvent(QTimerEvent *);

private:
    int m_timerID;

    OgreCameraWrapper *m_camera;
    OgreEngineItem *m_ogreEngineItem;
};

#endif // OGREITEM_H
