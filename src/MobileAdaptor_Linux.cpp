/***************************************************************************
 *   Copyright (C) 2019-2020 by Stefan Kebekus                             *
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

#include <QCoreApplication>
#include <QDir>
#include <QPointer>
#include <QStandardPaths>
#include <QTimer>

#include "Global.h"
#include "MobileAdaptor.h"
#include "geomaps/GeoMapProvider.h"
#include "traffic/TrafficDataProvider.h"


void MobileAdaptor::hideSplashScreen()
{
}


void MobileAdaptor::lockWifi(bool lock)
{
    Q_UNUSED(lock)
}


Q_INVOKABLE auto MobileAdaptor::missingPermissionsExist() -> bool
{
    Q_UNUSED(this);
    return false;
}


void MobileAdaptor::vibrateBrief()
{
}

auto MobileAdaptor::getSSID() -> QString
{
    return "<unknown ssid>";
}

void MobileAdaptor::hideNotification(NotificationType notificationType)
{
    auto notification = notifications.value(notificationType, nullptr);
    if (!notification.isNull()) {
        notification->close();
        delete notification;
    }

    if (notifications.contains(notificationType)) {
        notifications.remove(notificationType);
    }
}

void MobileAdaptor::showNotification(NotificationType notificationType, const QString& title, const QString& text, const QString& longText)
{
    // Get notificonst cation, &if it exists; otherwise get nullptr
    auto notification = notifications.value(notificationType, nullptr);

    // Otherwise, generate a new notification
    if (notification.isNull()) {
        switch (notificationType) {
        case DownloadInfo:
            notification = new KNotification(QStringLiteral("downloading"), KNotification::Persistent, this);
            break;
        case TrafficReceiverSelfTestError:
        case TrafficReceiverProblem:
            notification = new KNotification(QStringLiteral("trafficReceiverProblem"), KNotification::Persistent, this);
            break;
        }
        notification->setDefaultAction( tr("Open Application") );
        notification->setPixmap( {":/icons/appIcon.png"} );
    }

    notification->setTitle(title);
    notifications[notificationType] = notification;
    connect(notification, &KNotification::defaultActivated, [this, notificationType]() { emit notificationClicked(notificationType); });
    if (longText.isEmpty()) {
        notification->setText(text);
    } else {
        notification->setText(longText);
    }
    notification->sendEvent();

}
