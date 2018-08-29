/*
    == Widgetci - https://github.com/eminfedar/widgetci ==

    "THIS 'Note' IS ONE OF THE DEFAULT WIDGETS OF 'WIDGETCI'.
    CAN BE EDITED AND USED FREELY."

    - @eminfedar
*/

import QtQuick 2.5
import QtQuick.Controls 2.0
import com.widgetci.file 1.0 //WFile.

Item {
    id: base;

    // EDITABLES =====================================================
    property string widgetName: "Note"; // Must be Name of the folder & widget.
    property string fileContent: wFile.readFile(widgetName, "notes.txt");
    property string contentBackground: "#333" // #acea79
    property string contentBackgroundChecked: "#222" // #e4ea79
    width: 250;
    height: 400;
    // ===============================================================


    function getTabList(){
        var lst = []
        if(fileContent.length <= 0)
            return lst

        var pages = fileContent.split("_&_")
        for(var i=0; i<pages.length; i++){
            var pageArr = pages[i].split("_%_")
            lst.push(String(pageArr[0]))
        }
        return lst
    }
    function getContentList(){
        var lst = []
        if(fileContent.length <= 0)
            return lst

        var pages = fileContent.split("_&_")
        for(var i=0; i<pages.length; i++){
            var pageArr = pages[i].split("_%_")
            lst.push(String(pageArr[1]))
        }
        return lst
    }

    property var tabList: getTabList()
    property var contentList: getContentList()

    TabBar {
        id: tabBar
        width: parent.width
        height: 31
        background: Rectangle { color: contentBackground }

        currentIndex: swipeview.currentIndex

        Repeater{
            model: tabList

            TabButton{
                text: modelData
                width: contentItem.implicitWidth + 21
                height: tabBar.height

                font.bold: true

                contentItem: Text{
                    text: parent.text
                    font: parent.font
                    color: checked ? "#FFffffff" : "#77ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: parent.checked ? contentBackgroundChecked : contentBackground
                }
            }
        }

        TabButton{
            text: "+"
            width: Math.max(29, tabBar.width/5)
            height: parent.height
            font.bold: true
            background: Rectangle {
                color: parent.checked ? "#DDD" : contentBackground
            }
        }
    }

    SwipeView{
        id: swipeview
        y: tabBar.height
        height: parent.height - tabBar.height
        width: parent.width

        currentIndex: tabBar.currentIndex

        Repeater{
            id: contentRepeater
            model: contentList

            Item{
                width: swipeview.width
                height: swipeview.height
                Rectangle{
                    width: swipeview.width
                    height: swipeview.height
                    color : contentBackgroundChecked


                    TextEdit {
                        objectName: "TE_" + index
                        text: modelData
                        wrapMode: TextEdit.Wrap;
                        textFormat: TextEdit.PlainText;

                        color: "#FFF";

                        selectByKeyboard: true
                        selectByMouse: true

                        anchors.fill: parent
                        anchors.leftMargin: 7
                        anchors.rightMargin: 7
                        anchors.topMargin: 7
                        anchors.bottomMargin: 7

                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                parent.forceActiveFocus()
                            }
                        }

                        onTextChanged: {
                            var thisId = Number(objectName.split("_")[1])
                            contentList[thisId] = text

                            saveAll()
                        }
                    }

                    Button{
                        x: parent.width - 40
                        y: parent.height - 40
                        width: 32
                        height: 32
                        font.bold: true
                        text: "X"

                        contentItem: Text {
                            text: parent.text
                            font.bold: true
                            color: "#AAA"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle{
                            radius: 4
                            color: parent.pressed ? "#191919" : (parent.hovered ? "#444" : contentBackground)
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Double click to delete the Tab"

                        onDoubleClicked: {
                            var index = swipeview.currentIndex
                            deleteTab(index)
                        }

                    }

                }
            }

        }

        Item {
            id: tab_add
            Button{
                id: tab_add_button
                anchors.fill: parent
                text: "Add a new page"
                font.bold: true
                font.pointSize: 14

                onClicked: {
                    tab_add_name.visible = true
                    visible = false
                }

            }

            Rectangle{
                id: tab_add_name
                anchors.fill: parent
                color: contentBackground
                visible: false

                TextEdit{
                    id: tab_add_name_txt
                    anchors.fill: parent

                    color: "#FFF"
                    text: "Tab Name Here..."
                    font.pointSize: 14
                    font.bold: true
                    font.underline: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter


                    Keys.onReturnPressed: {
                        tab_add_name.visible = false
                        tab_add_button.visible = true


                        var item = Qt.createQmlObject('import QtQuick 2.5;import QtQuick.Controls 2.0;import QtQuick.Layouts 1.1;
                            Item{

                                width: swipeview.width
                                height: swipeview.height
                                Rectangle{
                                    width: swipeview.width
                                    height: swipeview.height
                                    color : contentBackgroundChecked


                                    TextEdit {
                                        text: "Edit here!"
                                        wrapMode: TextEdit.Wrap;
                                        textFormat: TextEdit.PlainText;

                                        color: "#FFF";

                                        selectByKeyboard: true
                                        selectByMouse: true

                                        anchors.fill: parent
                                        anchors.leftMargin: 7
                                        anchors.rightMargin: 7
                                        anchors.topMargin: 7
                                        anchors.bottomMargin: 7

                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: {
                                                parent.forceActiveFocus()
                                            }
                                        }

                                        onTextChanged: {
                                            var thisId = Number(objectName.split("_")[1])
                                            if(thisId !== NaN){
                                                contentList[thisId] = text
                                                saveAll()
                                            }
                                        }
                                    }

                                    Button{
                                        x: parent.width - 40
                                        y: parent.height - 40
                                        width: 32
                                        height: 32
                                        font.bold: true
                                        text: "X"

                                        contentItem: Text {
                                            text: parent.text
                                            font.bold: true
                                            color: "#AAA"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }

                                        background: Rectangle{
                                            radius: 4
                                            color: parent.pressed ? "#191919" : (parent.hovered ? "#444" : contentBackground)
                                        }

                                        ToolTip.visible: hovered
                                        ToolTip.text: "Double click to delete the Tab"

                                        onDoubleClicked: {
                                            var index = swipeview.currentIndex
                                            deleteTab(index)
                                        }

                                    }
                                }
                            }', swipeview)

                        var bar = Qt.createQmlObject('import QtQuick 2.5;import QtQuick.Controls 2.0;import QtQuick.Layouts 1.1;
                            TabButton{
                                text: "' + text + '"
                                width: contentItem.implicitWidth + 21
                                height: tabBar.height

                                font.bold: true

                                contentItem: Text{
                                    text: parent.text
                                    font: parent.font
                                    color: checked ? "#FFffffff" : "#77ffffff"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }

                                background: Rectangle {
                                    color: parent.checked ? contentBackgroundChecked : contentBackground
                                }
                            }', tabBar)

                        item.children[0].children[0].objectName = "TE_" + tabList.length

                        tabBar.moveItem(tabBar.count-1, tabBar.count-2)
                        swipeview.moveItem(swipeview.count-1, swipeview.count-2)
                        tabList.push(text)
                        contentList.push("Edit here!")
                        text = "Tab Name Here..."

                        // Save
                        saveAll()
                    }

                    onActiveFocusChanged: {
                        if(activeFocus){
                            if(text === "Tab Name Here...")
                                text = ""
                        }
                    }
                }
            }
        }
    }

    function deleteTab(index){
        swipeview.removeItem(index)
        tabBar.removeItem(index)

        tabList.splice(index,1)
        contentList.splice(index,1)

        for(var i=0; i < tabList.length; i++){
            swipeview.itemAt(i).children[0].children[0].objectName = "TE_"+i;
        }

        //if(tabBar.count > 0 && tabBar.currentIndex < tabBar.count-1)
        //    tabBar.incrementCurrentIndex()

        saveAll()
    }

    function saveAll(){
        var totalString = ""
        for(var i=0; i < tabList.length; i++)
            totalString += tabList[i] + "_%_" + contentList[i] + "_&_"

        if(totalString.length >= 3)
            totalString = totalString.slice(0, -3)

        wFile.saveFile(widgetName, "notes.txt", totalString);
    }

    // Widgetci's file manager. (read & write files.) (import com.widgetci.file 1.0)
    WFile{
        id: wFile;
    }
}
