/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4


Item {
    id: name
    signal closed

    RowLayout{
        anchors.fill: parent
        ListView{
            id: files
            objectName: "fileList"
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: resourceList
            delegate: Row{
                objectName: "listitem"
                property string name: modelData
                Rectangle{
                    width: 10
                    height: 10
                    color: "Blue"
                    radius: 45
                }
                Text{
                    id: text
                    text: name
                }
            }
            highlight: Rectangle {color:"Red"; opacity: 0.5}
        }
        ColumnLayout{
            Button{
                text: "Open"
                onClicked: closed()
            }
            Button{
                id: fileCloseButton
                text: "Back"
                onClicked: closed()
            }
        }
    }
}
