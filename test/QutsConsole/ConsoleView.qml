/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item{
    id: main
    property alias text: view
    property bool enableBreakpoints: false
    property bool enableLineNumbers: false
    readonly property bool hasContent: positionMark.visible

    function append(line, lineNo){
        if(undefined !== lineNo){
            var end =  view.positionToRectangle(view.length)
            var at = lineNo > 0 ? end.y + end.width : 0
            lineNumbers.set(lineNo + 1, at )
        }
        view.append(line)
    }

    function scrollTop(){
        view.cursorPosition = 0;
    }

    function clear(){
        view.clear()
        lineNumbers.clear()
    }

    function setPosition(line){
        var item = lineNumbers.closestItem(line)
        positionMark.parent = item
    }

    function resetPosition(){
        positionMark.parent = main
    }

    signal setBreakpoint(int line, bool selected)

    Flickable{
        id: flick
        anchors.fill: parent;
        contentHeight: view.paintedHeight
        clip: true
        function ensureVisible(r){
            if (contentX >= r.x)
                contentX = r.x;
            else if (contentX+width <= r.x+r.width)
                contentX = r.x+r.width-width;
            if (contentY >= r.y)
                contentY = r.y;
            else if (contentY+height <= r.y+r.height)
                contentY = r.y+r.height-height;
            }
        RowLayout{
            width: flick.width
            height: flick.height
            Column{
                Layout.minimumWidth: 20
                Layout.fillHeight: true
                visible: true
                Repeater{
                    id: lineNumbers
                    model: []
                    function closestItem(index){
                        for(var i = 0; i < model.length; i++){
                            var v = model[i]
                            if(v >= index){
                                if(v > index)
                                    return itemAt(i - 1);
                                else
                                    return itemAt(i);
                            }
                        }
                        return null
                    }
                    function clear() {
                        model = []
                    }
                    function set(num, at){
                        var pos = Math.ceil(at / view.lineHeight)
                        var copy = model.slice()
                        copy[pos] = num
                        model = copy
                    }
                    function setSelected(itemIndex, isSelected){
                            var fromPos = itemIndex
                            while(fromPos >= 0 && model[fromPos] < 0){
                                --fromPos;
                            }
                            var line = model[fromPos]
                            if(line >= 0){
                                setBreakpoint(line, isSelected)
                            }
                            if(fromPos !== itemIndex){
                                itemAt(itemIndex).selected = false;
                                itemAt(fromPos).selected = isSelected;
                            }
                        }
                    Item{
                        width: 20
                        height: lineNo.contentHeight
                        property bool selected: false
                        Rectangle{
                            anchors.fill: parent
                            color: "Red"
                            visible: parent.selected
                        }
                        Text{
                            id: lineNo
                            text: modelData > 0 ? modelData : ''
                            font.family: fixedFont.name
                        }
                    MouseArea{
                        enabled: enableBreakpoints
                        anchors.fill: parent
                        onClicked:{
                            parent.selected = !parent.selected
                            lineNumbers.setSelected(index, parent.selected)
                        }
                        }
                    }
                }
            }
            TextEdit{
                id: view
                property real lineHeight: contentHeight / lineCount
                readOnly: true
                font.family: fixedFont.name
                wrapMode: TextEdit.Wrap
                Layout.fillHeight: true
                Layout.fillWidth: true
                textFormat: TextEdit.PlainText
                onCursorRectangleChanged:
                    flick.ensureVisible(cursorRectangle)
                onTextChanged:
                    main.textChanged

            }
        }
    }
    Rectangle{
        id: positionMark
        height: 12
        width: 20
        color: "Yellow"
        opacity: 0.5
        visible: parent != main
    }
    FontLoader { id: fixedFont; name: "Courier" }
}
