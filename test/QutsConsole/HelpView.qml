/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item {
    signal closed
    RowLayout{
        anchors.fill: parent
        ListView{
            id: view
            spacing: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: ListModel{
                id: helpModel
            }

            delegate: Rectangle{
                color: "LightGrey"
                width: view.width
                height: content.height
                ColumnLayout{
                    id: content
                    property var stuff: parameters
                    Text{
                        objectName: "subItem"
                        text: sub
                    }
                    Text{
                        objectName: "nameItem"
                        text: name
                    }
                    Text{
                        text: definition
                    }
                    Repeater{
                        model: paramCount
                        delegate: Text{
                            text: parameters[index]
                        }
                    }
                    Text{
                        text: returnValue
                    }
                    Text{
                        objectName: "comments"
                        text: comments
                    }
                 }
            }
        }
        Button{
            text: "Back"
            onClicked: closed()
        }
    }

    Component.onCompleted: {
        var subs = QutsAPI.help()
        for(var i = 0; i < subs.length; i++){
            var kws = QutsAPI.help(subs[i])
            for(var k = 0; k < kws.length; k++){
                var data = QutsAPI.functionHelp(subs[i], kws[k])
                var paramList = data['parameters']
                helpModel.append({
                             sub: subs[i],
                             name:kws[k],
                             definition: data['definition'],
                             paramCount: paramList ? paramList.length : -1,
                             parameters: paramList,
                             returnValue: data['returnValue'],
                             comments: data['comment']})
            }
        }
    }
}
