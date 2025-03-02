/***************************************************************************
 *   Copyright (C) 2022-2024 by Stefan Kebekus                             *
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

#include "Downloadable_Abstract.h"
#include <QLocale>
#include <chrono>

using namespace Qt::Literals::StringLiterals;
using namespace std::chrono_literals;


DataManagement::Downloadable_Abstract::Downloadable_Abstract(QObject *parent)
    : QObject(parent)
{
    emitFileContentChanged_delayedTimer.setInterval(2s);
    connect(this, &Downloadable_Abstract::fileContentChanged, &emitFileContentChanged_delayedTimer, qOverload<>(&QTimer::start));
    connect(&emitFileContentChanged_delayedTimer, &QTimer::timeout, this, &Downloadable_Abstract::emitFileContentChanged_delayed);

    m_section.setBinding([this]() {return m_hasFile.value() ? u"<a name>"_s + tr("Installed") : m_sectionBuffer.value();});
}



//
// Getter methods
//

auto DataManagement::Downloadable_Abstract::updateSizeString() -> QString
{
    auto size = qMax(Units::ByteSize(0),updateSize());

    return QLocale::system().formattedDataSize( qint64(size), 1, QLocale::DataSizeSIFormat);
}



//
// Setter methods
//

void DataManagement::Downloadable_Abstract::setBoundingBox(const QGeoRectangle& boundingBox)
{
    if (boundingBox == m_boundingBox)
    {
        return;
    }
    m_boundingBox = boundingBox;
    emit boundingBoxChanged();
}


void DataManagement::Downloadable_Abstract::setContentType(DataManagement::Downloadable_Abstract::ContentType contentType)
{
    if (contentType == m_contentType)
    {
        return;
    }
    m_contentType = contentType;
    emit contentTypeChanged();
}



//
// Private methods
//


void DataManagement::Downloadable_Abstract::emitFileContentChanged_delayed()
{
    if (downloading()) {
        return;
    }
    emitFileContentChanged_delayedTimer.stop();
    emit fileContentChanged_delayed();
}
