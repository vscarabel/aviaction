/***************************************************************************
 *   Copyright (C) 2019-2023 by Stefan Kebekus                             *
 *   stefan.kebekus@gmail.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#pragma once

#include <QQmlEngine>

#include "GlobalObject.h"
#include "Notification.h"


namespace Notifications {


class NotificationManager : public GlobalObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    //
    // Constructors and destructors
    //

    /*! \brief Standard constructor
     *
     * @param parent The standard QObject parent pointer
     */
    explicit NotificationManager(QObject* parent = nullptr);

    // deferred initialization
    void deferredInitialization() override;

    // No default constructor, important for QML singleton
    explicit NotificationManager() = delete;

    /*! \brief Standard destructor */
    ~NotificationManager() override = default;

    // factory function for QML singleton
    static Notifications::NotificationManager* create(QQmlEngine* /*unused*/, QJSEngine* /*unused*/)
    {
        return GlobalObject::notificationManager();
    }


    //
    // PROPERTIES
    //

    Q_PROPERTY(Notifications::Notification* currentNotification READ currentNotification NOTIFY currentNotificationChanged)
    Notifications::Notification* currentNotification() const {
        if (m_notifications.isEmpty())
        {
            return nullptr;
        }
        return m_notifications[0].data();
    }

    void add(Notifications::Notification* notification);
signals:
    void currentNotificationChanged();

private slots:
    void update();

private:
    QVector<QSharedPointer<Notifications::Notification>> m_notifications;

    Notifications::Notification* testNotification;
};

} // namespace Notifications
