/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

TabView{
    id: source
    readonly property bool hasContent: count > 0 && currentItem !== null && currentItem.hasContent
    signal setBreakpoint(string file, int line, bool selected)
    readonly property string current : count > 0 ? getTab(currentIndex).title : ""
    readonly property var currentItem: count > 0 ? getTab(currentIndex).item : null
    Component{
        id: sourceView
        ConsoleView{
            enableBreakpoints: true
            enableLineNumbers: true
        }
    }

    function _findTab(name){
        for(var i = 0; i < count; i++){
            var t = getTab(i)
            if(t.title.toUpperCase() === name.toUpperCase())
                return t
        }
        return null
    }

    function _setCurrent(name){
        for(var i = 0; i < count; i++){
            var t = getTab(i)
            if(t.title.toUpperCase() === name.toUpperCase()){
                currentIndex = i
                return t
            }
        }
        return null
    }

    function setPosition(scriptName, lineNo){
        var tab = _setCurrent(scriptName)
        if(tab)
            tab.item.setPosition(lineNo)
    }

    function append(scriptName, lineNo, line){
        var tab = _findTab(scriptName)
        if(tab === null){
            tab = addTab(scriptName, sourceView)
            tab.active = true
            tab.item.setBreakpoint.connect(function(line, selected){
            setBreakpoint(scriptName, line, selected)
            })
        }
        tab.item.append(line, lineNo)
    }

    function clear(){
       for(var i = 0; i < count; i++)
           removeTab(i)
    }

    function resetPosition(){
        for(var i = 0; i < count; i++){
            var t = getTab(i).item
            if(t.hasContent){
                t.resetPosition()
            }
        }
    }

    function scrollTop(){
        if(count  > 0){
            currentItem.scrollTop()
        }
    }
}
