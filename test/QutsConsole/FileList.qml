/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2


Dialog {
    id: name
    signal closed
    title: "Examples"
    property alias content : files.model
    property string currentName: files.currentIndex >= 0 ? files.model[files.currentIndex] : null
    width: 640
    height: 480
    RowLayout{
        anchors.fill: parent
        ListView{
            id: files
            objectName: "fileList"
            Layout.fillHeight: true
            Layout.fillWidth: true
            focus: true
            highlightFollowsCurrentItem: true
            delegate: Item{
                id: delegate
                objectName: "listitem"
                property string name: modelData
                height: text.height + 2
                width: files.width
                Text{
                    id: text
                    text: name
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        files.currentIndex = index
                    }
                }
            }
            highlight: Rectangle {color:"Red"; opacity: 0.5}
        }
    }

    footer: DialogButtonBox {
        alignment: Qt.AlignRight
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel |  StandardButton.Save

        onAccepted: {
            if(clickedButton === StandardButton.Save ) {
                exportDlg.visible = true
            }
            name.closed()
        }
        onRejected: {
                currentName = null
            }
    }



    FileDialog {
        id: exportDlg
        selectMultiple: false
        selectFolder: true
        selectExisting: true
        title: "Save to..."
        readonly property string scriptFolderKey: "scriptFolder"
        folder: QutsApp.getSetting(scriptFolderKey, shortcuts.home)
        nameFilters: [  name.currentName ]

        onAccepted: {
            QutsApp.setSetting(scriptFolderKey, folder)
            var base = name.currentName.substring(name.currentName.lastIndexOf('/') + 1)
            var fileName = folder + "/" + base
            if(QutsApp.copyTo(name.currentName, fileName)){
                console.log("save to", fileName)
            } else {
                console.log("save to", fileName, "failed")
            }

        }
        onRejected: {
        }
    }
}
