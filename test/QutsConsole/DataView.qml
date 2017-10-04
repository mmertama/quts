import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1

Rectangle {
    id: main
    property var dataMap
    property int lineNo
    color: "LightGrey"
    property real textSize: 8
    property real labelTextSize: 8
    RowLayout{
        anchors.fill: parent

        ListView{
            Layout.fillHeight: true
            Layout.minimumWidth: main.width / 6
            id: stackList
            clip: true
            model: []

            delegate: Text{
                font.pixelSize: textSize
                text: modelData ? modelData : "N/A"
            }
            Rectangle{
                z:-1
                anchors.fill: parent
                color: "DarkGrey"
            }
        }
        ListView{
            id: scopes
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: Qt.Horizontal
            spacing: 2
            readonly property var types: ['Invalid', 'For', 'If', 'Call'];
            delegate: Rectangle{
                id: scopeRect
                color: "DarkGrey"
                height: main.height
                width: main.width / 4
                readonly property real mw: (width - 6) / 2
                property real memView: 0.9
                ColumnLayout{
                    id: scope
                    anchors.fill: parent
                    anchors.leftMargin: 1
                    GridLayout{
                        columns: 2
                        Layout.fillWidth: true
                        Text{
                            font.pixelSize: labelTextSize
                            Layout.minimumWidth: scopeRect.mw
                            text: "Scope:" + index
                        }
                        Text{
                            font.pixelSize: labelTextSize
                            Layout.minimumWidth: scopeRect.mw
                            text: modelData ? modelData['begin'] + " - " + modelData['end'] : "N/A";
                        }
                        Text{
                            font.pixelSize: labelTextSize
                            Layout.minimumWidth: scopeRect.mw
                            text: modelData ? scopes.types[modelData['type']] : "N/A"
                        }
                            Button{
                                scale: 0.6
                                text: memView > 0.5 ? ">" : "<"
                                onClicked: memView = 1.0 - memView
                            }
                    }
                    RowLayout{
                        Layout.fillWidth: true
                        ListView{
                            clip: true
                            Layout.fillHeight: true
                            Layout.preferredWidth: 2 * scopeRect.mw * memView
                            model: modelData ? modelData['memory'] : null
                            delegate: Text{
                                text: modelData
                                font.pixelSize: textSize
                            }
                            Rectangle{
                                z: -1
                                anchors.fill: parent
                                color: "#CF8484"
                            }
                        }
                        ListView{
                            clip: true
                            Layout.fillHeight: true
                            Layout.preferredWidth: 2 * scopeRect.mw * (1.0 - memView)
                            model: modelData ? modelData['data'] : null
                            delegate: Text{
                                text: modelData
                                font.pixelSize: textSize
                            }
                            Rectangle{
                                z: -1
                                anchors.fill: parent
                                color: "#84CF84"
                            }
                        }
                    }
                }
            }
        }
    }
    onDataMapChanged: {
        lineNo = 'line' in dataMap ? dataMap['line'] : -1
        stackList.model = 'stack' in dataMap ? dataMap['stack'] : null
        scopes.model = 'scopes' in dataMap ? dataMap['scopes'] : null
    }
}
