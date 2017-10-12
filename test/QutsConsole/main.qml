/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0

ApplicationWindow {
    id: main
    property string currentFile
    property string currentName
    property var lines
    objectName: "qutsConsole"
    visible: true

    signal echo(string str)

    Connections{
        target: QutsAPI
        onEnd: {
            status.text += " completed"
        }

        onPrintLine:{
            consoleView.append(line)
        }

        onQutsSignal:{
            if(signalName == "ECHO"){
                var timer = Qt.createQmlObject('import QtQuick 2.4; Timer {interval: 1500; running: true; repeat: false}',
                main, "qutsConsole");
                timer.onTriggered.connect(function(){
                    echo(params[0])
                })
            }
        }

        onLineAdded:{
            sourceView.append(scriptName, lineNo, line)
        }

        onBreakAt:{
            var pos = scriptName.indexOf('.')
            var basename = pos > 0 ? scriptName.substring(0, pos) : scriptName;
            sourceView.setPosition(basename, lineNo)
        }

        onError:{
            if(line < 0)
                consoleView.append(message + " in " + script)
        }
    }

    statusBar: StatusBar{
        Layout.fillWidth: true
        RowLayout {
            anchors.fill: parent
            Label {
                id: status
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: main.currentName
            }
        }
    }


    function doLine(){
        var name = "*Command line*"
        status.text = ""
        QutsAPI.execute(name, [edit.text]);
    }


    FileDialog {
        id: openDlg
        selectMultiple: false
        title: "Please choose a file"
        readonly property string scriptFolderKey: "scriptFolder"
        folder: QutsApp.getSetting(scriptFolderKey, shortcuts.home)
        nameFilters: [ "Quts files (*.qts)" ]
        onVisibleChanged:
            main.currentFile = ""
        onAccepted: {
            QutsApp.setSetting(scriptFolderKey, folder)
            main.currentFile = openDlg.fileUrl
            if(main.currentFile.length > 0){
                status.text = ""
                sourceView.clear()
                consoleView.clear()
                QutsAPI.removeAllBreakpoints();
                main.currentName = QutsAPI.read(openDlg.fileUrl);
                if(main.currentName.length == 0){
                    status.text = "Error opening file: " + main.currentFile
                }
                sourceView.scrollTop()

            } else{
                status.text = "Error"
            }

        }
        onRejected: {
        }
    }

    FileList{
        id: filelist
        objectName: "resources"
        visible: false
        height: openDlg.height
        width: openDlg.width

        content: resourceList.filter(function(name){
            var suffix = ".qts"
            return name.indexOf(suffix, name.length - suffix.length) !== -1
        })

        onClosed: visible = false
        onVisibleChanged: {
            mainview.visible = !visible
        }
    }

    HelpView{
        id: helpView
        objectName: "helpView"
        visible: false
        anchors.fill: parent
        onClosed: visible = false
        onVisibleChanged: {
            mainview.visible = !visible
        }
    }

    ColumnLayout{
        id:mainview
        objectName: "mainView"
        RowLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true
            SourceView{
                id: sourceView
                Layout.minimumHeight: 480
                Layout.minimumWidth: 320
                onSetBreakpoint:{
                    if(!QutsAPI.setBreakpoint(sourceView.current, line, selected)){
                        status.text = "Cannot set breakpoint at " + sourceView.current + " " + line
                    }
                }
            }
            ConsoleView{
                id: consoleView
                Layout.minimumHeight: 480
                Layout.minimumWidth: 320
            }
            ColumnLayout{
                Button{
                    text: "Open"
                    onClicked: openDlg.visible = true
                    enabled:  !QutsAPI.active
                }
                Button{
                    id:  fileOpenButton
                    text: "Examples"
                    onClicked: filelist.visible = true
                    enabled:  !QutsAPI.active
                }
                Button{
                    text: "Help"
                    onClicked: helpView.visible = true
                }
                Button{
                    text: "Run"
                    enabled: main.currentName.length > 0 && !QutsAPI.active

                    onClicked: {
                        status.text = ""
                        if(!QutsAPI.start(main.currentName, {debug: debugBtn.checked ? "true" : "false", breakpoints: "true"}))
                            sourceView.append("Cannot start " + main.currentName)
                    }
                }
                Button{
                    text: "Pause"
                    enabled: currentName.length > 0 && QutsAPI.active
                    onClicked: {
                        QutsAPI.pause()
                    }
                }
                Button{
                    text: "Stop"
                    enabled: currentName.length > 0 && QutsAPI.active
                    onClicked: {
                        QutsAPI.stop();
                    }
                }
                Button{
                    text: "Continue"
                    enabled: currentName.length > 0 && sourceView.hasContent
                    onClicked: {
                        sourceView.resetPosition()
                        status.text = ""
                        QutsAPI.resume()
                    }
                }
                Button{
                    visible: false
                    objectName: "button1"
                }
                Button{
                    visible: false
                    objectName: "button2"
                }
                CheckBox{
                    id: debugBtn
                    text: "Debug"
                }
                CheckBox{
                    id: consoleOut
                    text: "Console print"
                    checked: QutsAPI.isPrintStd
                    onClicked: QutsAPI.isPrintStd = checked
                }
                Button{
                   text: "Clear"
                   onClicked: consoleView.clear()
                }
            }
        }
        DataView{
            id: dataView
            dataMap: QutsAPI.snapShot
            Layout.fillWidth: true
            Layout.minimumHeight: 160
        }
        RowLayout{
            Layout.fillWidth: true
            TextField{
                id: edit
                Layout.fillWidth: true
                focus: true
                Keys.onReturnPressed: doLine()
                Keys.onEnterPressed:  doLine()
            }
            Button{
                text: "Do"
                objectName: "doBtn"
                onClicked: doLine()
            }
        }
    }
}
