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

import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import akaflieg_freiburg.enroute
import "../dialogs"
import "../items"

Page {
    id: barometricPage
    title: qsTr("Barometric Data")

    header: StandardHeader {}

    DecoratedScrollView {
        id: sView

        anchors.fill: parent
        contentWidth: availableWidth // Disable horizontal scrolling

        clip: true

        bottomPadding: font.pixelSize + SafeInsets.bottom
        leftPadding: font.pixelSize + SafeInsets.left
        rightPadding: font.pixelSize + SafeInsets.right
        topPadding: font.pixelSize

        GridLayout {
            id: gl
            columnSpacing: 30
            columns: 3

            width: sView.availableWidth

            Label {
                Layout.fillWidth: true
                Layout.columnSpan: 2
                text: qsTr("Status")
                font.pixelSize: barometricPage.font.pixelSize*1.2
                font.bold: true
            }
            ToolButton { enabled: false }

            Label { // Status
                Layout.fillWidth: true
                Layout.leftMargin: 4
                Layout.rightMargin: 4
                Layout.columnSpan: 3

                text: TrafficDataProvider.pressureAltitude.isFinite() ? qsTr("Receiving static pressure data from traffic receiver") : qsTr("Not connected to a traffic receiver that provides static pressure data")

                wrapMode: Text.WordWrap
                textFormat: Text.RichText

                bottomPadding: 0.6*font.pixelSize
                topPadding: 0.6*font.pixelSize
                leftPadding: 0.2*font.pixelSize
                rightPadding: 0.2*font.pixelSize

                leftInset: -4
                rightInset: -4

                background: Rectangle {
                    border.color: "black"
                    color: TrafficDataProvider.pressureAltitude.isFinite() ? "green" : "red"
                    opacity: 0.2
                    radius: 4
                }
            }

            Label { // Status
                Layout.fillWidth: true
                Layout.leftMargin: 4
                Layout.rightMargin: 4
                Layout.columnSpan: 3

                text: Sensors.statusString

                wrapMode: Text.WordWrap
                textFormat: Text.RichText

                bottomPadding: 0.6*font.pixelSize
                topPadding: 0.6*font.pixelSize
                leftPadding: 0.2*font.pixelSize
                rightPadding: 0.2*font.pixelSize

                leftInset: -4
                rightInset: -4

                background: Rectangle {
                    border.color: "black"
                    color: Sensors.ambientPressure.isFinite() ? "green" : "red"
                    opacity: 0.2
                    radius: 4
                }
            }

            Label {
                Layout.fillWidth: true
                Layout.columnSpan: 2
                text: qsTr("Barometric Altitudes")
                font.pixelSize: barometricPage.font.pixelSize*1.2
                font.bold: true
            }
            ToolButton {
                icon.source: "/icons/material/ic_info_outline.svg"
                onClicked: {
                    PlatformAdaptor.vibrateBrief()
                    helpDialog.title = qsTr("Altitudes")
                    helpDialog.text = "<p>"+qsTr("Pressure altitude is the value shown by your altimeter when set to the standard value 1013.2 hPa. The pressure altitude is used to define airspace boundaries and vertical aircraft position above the transition altitude.")+"</p>"
                            +"<p>"+qsTr("Altitude is the value shown by your altimeter when set to QNH. The altitude is used to define airspace boundaries and vertical aircraft position below the transition level.")+"</p>"
                            +"<p>"+qsTr("Density altitude is the altitude at which an aircraft flying in the ICAO standard atmosphere experiences an air density equal to the density measured by the ambient pressure/temperature sensors of your device.")+"</p>"
                            +"<p>"+qsTr("Cabin altitude is the altitude at which an aircraft flying in the ICAO standard atmosphere experiences a static pressure equal to the pressure in the cabin of your aircraft.")+"</p>"
                    helpDialog.open()
                }
            }

            Label {
                text: qsTr("Pressure Altitude") }
            Label {
                Layout.fillWidth: true
                text: TrafficDataProvider.pressureAltitude.isFinite() ? "FL" + ("000" + Math.round(TrafficDataProvider.pressureAltitude.toFeet()/100.0)).slice(-3) : "-"
                wrapMode: Text.Wrap
            }
            Item { }

            Label { text: qsTr("Altitude") }
            Label {
                Layout.fillWidth: true
                text: {
                    var pAlt = TrafficDataProvider.pressureAltitude
                    if (!pAlt.isFinite())
                        return "-"
                    var qnhpAlt = WeatherDataProvider.QNHPressureAltitude
                    if (!qnhpAlt.isFinite())
                        return "-"

                    var a = pAlt.subtract(qnhpAlt)
                    return Navigator.aircraft.verticalDistanceToString(a)
                }
                wrapMode: Text.Wrap
            }
            Item { }

            Label { text: qsTr("Cabin Altitude") }
            Label {
                Layout.fillWidth: true
                text: Sensors.pressureAltitude.isFinite() ? "FL" + ("000" + Math.round(Sensors.pressureAltitude.toFeet()/100.0)).slice(-3) : "-"
                wrapMode: Text.Wrap
            }
            Item { }

            Label {
                Layout.columnSpan: 2
                text: qsTr("Device Sensors")
                font.pixelSize: barometricPage.font.pixelSize*1.2
                font.bold: true
            }
            ToolButton { enabled: false }

            Label { text: qsTr("Pressure") }
            Label {
                Layout.fillWidth: true
                text: Sensors.ambientPressure.isFinite() ? Math.round(Sensors.ambientPressure.toHPa()*10.0)/10.0 + " hPa"  : "-"
                wrapMode: Text.Wrap
            }
            Item { }

            Label { text: qsTr("Temperature") }
            Label {
                Layout.fillWidth: true
                text: Sensors.ambientTemperature.isFinite() ? Math.round(Sensors.ambientTemperature.toDegreeCelsius()) + " °C" : "-"
                wrapMode: Text.Wrap
            }
            Item { }

            Label {
                Layout.columnSpan: 2
                text: qsTr("Other")
                font.pixelSize: barometricPage.font.pixelSize*1.2
                font.bold: true
            }
            ToolButton { enabled: false }

            Label {
                Layout.alignment: Qt.AlignTop
                text: qsTr("QNH")
            }
            Label {
                Layout.fillWidth: true
                text: WeatherDataProvider.QNHInfo === "" ? "-" : WeatherDataProvider.QNHInfo
                wrapMode: Text.Wrap
            }
            Item { }

        } // GridLayout

    }

    LongTextDialog {
        id: helpDialog

        modal: true

        standardButtons: Dialog.Ok
    }

}
