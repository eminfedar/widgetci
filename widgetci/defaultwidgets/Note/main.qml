import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import com.widgetci.file 1.0 //WFile.

Item {
    id: base;

    // EDITABLES
    property string widgetName: "Note"; // Must be Name of the folder & widget.
    property string fileContent: wFile.readFile(widgetName, "notes.txt");
    property string contentBackground: "#333"
    property string contentBackgroundChecked: "#222"

    property var tabList: getTabList()
    property var contentList: getContentList()

    function getTabList(){
        var pages = fileContent.split("_&_")
        var lst = []
        for(var i in pages){
            var pageArr = pages[i].split("_%_")
            lst.push(String(pageArr[0]))
        }
        return lst
    }
    function getContentList(){
        var pages = fileContent.split("_&_")
        var lst = []
        for(var i in pages){
            var pageArr = pages[i].split("_%_")
            lst.push(String(pageArr[1]))
        }
        return lst
    }

    width: 250;
    height: 400;

    TabBar {
        id: tabBar
        width: parent.width
        height: 31
        currentIndex: swipeview.currentIndex
        background: Rectangle { color: contentBackground }


        Repeater{
            model: tabList

            TabButton{
                text: modelData
                width: Math.max(80, tabBar.width/4)
                height: parent.height

                font.bold: true

                contentItem: Text{
                    text: parent.text
                    font: parent.font
                    color: checked ? "#FFF" : "#999"
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
            width: Math.max(80, tabBar.width/4)
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
                    color : "#202020"


                    TextEdit {
                        text: modelData
                        wrapMode: TextEdit.Wrap;
                        textFormat: TextEdit.PlainText;

                        color: "#FFF";

                        anchors.fill: parent
                        anchors.leftMargin: 7
                        anchors.rightMargin: 7
                        anchors.topMargin: 7
                        anchors.bottomMargin: 7

                        MouseArea{
                            anchors.fill: parent
                            onDoubleClicked: {
                                parent.forceActiveFocus()
                            }
                        }

                        onActiveFocusChanged: {
                            if(!activeFocus){
                                var totalString = ""
                                for(var i in tabList){
                                    if(Number(i) === swipeview.currentIndex){
                                        totalString += tabList[i] + "_%_" + text + "_&_"
                                        contentList[i] = text
                                    }
                                    else
                                        totalString += tabList[i] + "_%_" + contentList[i] + "_&_"
                                }
                                totalString = totalString.slice(0, -3)
                                wFile.saveFile(widgetName, "notes.txt", totalString);
                            }
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
                color: "#005500"
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
                                            color : "#202020"


                                            TextEdit {
                                                text: "Edit here!"
                                                wrapMode: TextEdit.Wrap;
                                                textFormat: TextEdit.PlainText;

                                                color: "#FFF";

                                                anchors.fill: parent
                                                anchors.leftMargin: 5
                                                anchors.rightMargin: 5
                                                anchors.topMargin: 5
                                                anchors.bottomMargin: 5

                                                MouseArea{
                                                    anchors.fill: parent
                                                    onDoubleClicked: {
                                                        parent.forceActiveFocus()
                                                    }
                                                }

                                                onActiveFocusChanged: {
                                                    if(!activeFocus){
                                                        var totalString = ""
                                                        for(var i in tabList){
                                                            if(Number(i) === swipeview.currentIndex){
                                                                totalString += tabList[i] + "_%_" + text + "_&_"
                                                                contentList[i] = text
                                                            }
                                                            else
                                                                totalString += tabList[i] + "_%_" + contentList[i] + "_&_"
                                                        }
                                                        console.log(text)
                                                        totalString = totalString.slice(0, -3)
                                                        console.log(totalString)
                                                        wFile.saveFile(widgetName, "notes.txt", totalString);
                                                    }
                                                }
                                            }

                                        }
                                 }', swipeview, "swipeitem")

                        var bar = Qt.createQmlObject('import QtQuick 2.5;import QtQuick.Controls 2.0;import QtQuick.Layouts 1.1;
                                TabButton{
                                    text: "' + text + '"
                                    width: Math.max(80, tabBar.width/4)
                                    height: parent.height
                                    font.bold: true

                                    contentItem: Text{
                                        text: parent.text
                                        font: parent.font
                                        color: checked ? "#FFF" : "#999"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    background: Rectangle {
                                              color: parent.checked ? contentBackgroundChecked : contentBackground
                                          }
                                }', tabBar, "baritem")

                        tabBar.moveItem(tabBar.count-1, tabBar.count-2)
                        swipeview.moveItem(swipeview.count-1, swipeview.count-2)
                        tabList.push(text)
                        contentList.push("Edit here!")

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

    function saveAll(){
        var totalString = ""
        for(var i in tabList)
            totalString += tabList[i] + "_%_" + contentList[i] + "_&_"

        totalString = totalString.slice(0, -3)
        wFile.saveFile(widgetName, "notes.txt", totalString);
    }

    // Widgetci's file manager. (for read & write files.) (import com.widgetci.file 1.0)
    WFile{
        id: wFile;
    }
}
