/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Itemviews NG project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtCore/QRectF>
#include <QtCore/QDataStream>

struct Message
{
    enum MessageType
    {
        InvalidMessage,
        RequestMessage,
        ResponseMessage,
        EventMessage
    };

    inline Message(MessageType message = InvalidMessage, quint16 type = -1, quint32 id = -1,
                   quint32 value = 0, const QRectF &rect = QRectF())
        : message(message), type(type), id(id), value(value), rect(rect) {}

    quint16 message;
    quint16 type;
    quint32 id;
    quint32 value;
    QRectF rect;
};

struct Request : public Message
{
    enum RequestType
    {
        InvalidRequest,
        CreateWindowRequest,
        DestroyWindowRequest,
        ShowWindowRequest,
        HideWindowRequest,
        RaiseWindowRequest,
        LowerWindowRequest,
        UpdateWindowRequest,
        SetWindowGeometryRequest,
    };

    inline Request(RequestType type = InvalidRequest, quint32 id = -1,
                   quint32 value = 0, const QRectF &rect = QRectF())
        : Message(RequestMessage, type, id, value, rect) {}
};

struct Response : public Message
{
    enum ResponseType
    {
        InvalidResponse,
        CreatedWindowResponse
    };

    inline Response(ResponseType type = InvalidResponse, quint32 id = -1,
                    quint32 value = 0, const QRectF &rect = QRectF())
        : Message(ResponseMessage, type, id, value, rect) {}
};

struct Event : public Message
{
    enum EventType
    {
        InvalidEvent,
        ErrorEvent,
        GeometryChangeEvent,
        MousePressEvent,
        MouseReleaseEvent,
        MouseMoveEvent,
        KeyPressEvent,
        KeyReleaseEvent
    };

    inline Event(EventType type = InvalidEvent, quint32 id = -1,
                 quint32 value = 0, const QRectF &rect = QRectF())
        : Message(EventMessage, type, id, value, rect) {}
};

inline QDataStream &operator>>(QDataStream &in, Message &message)
{
    in >> message.message;
    in >> message.type;
    in >> message.id;
    in >> message.value;
    in >> message.rect;
    return in;
}

inline QDataStream &operator<<(QDataStream &out, const Message &message)
{
    out << message.message;
    out << message.type;
    out << message.id;
    out << message.value;
    out << message.rect;
    return out;
}

#endif//PROTOCOL_H
