import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import com.widgetci.file 1.0 //WFile.

Item {
    id: base;

    // EDITABLES
    property string widgetName: "Note"; // Must be Name of the folder & widget.
    property string fileContent: wFile.readFile(widgetName, "notes.txt");
    property string contentBackground: "#222"
    property string contentBackgroundSecondary: "#CCC"

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

    Component.onCompleted: {
        console.log(contentList)
        console.log(tabList)

        contentRepeater.update()
    }

    TabBar {
        id: tabBar
        width: parent.width
        height: 32
        currentIndex: swipeview.currentIndex
        background: Rectangle { color: "#00000000" }

        Repeater{
            model: tabList

            TabButton{
                text: modelData
                width: Math.max(80, tabBar.width/4)
                height: parent.height
                font.bold: true
                background: Rectangle {
                          implicitWidth: 100
                          implicitHeight: 40
                          color: parent.checked ? contentBackgroundSecondary : contentBackground
                      }
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
                Rectangle{
                    anchors.fill: parent
                    color:"#202020"
                    TextEdit {
                        text: modelData
                        wrapMode: TextEdit.Wrap;
                        textFormat: TextEdit.PlainText;

                        focus:true;
                        selectByMouse: true;
                        selectByKeyboard: true;

                        color: "#FFF";

                        anchors.fill: parent
                        anchors.leftMargin: 5
                        anchors.rightMargin: 5
                        anchors.topMargin: 5
                        anchors.bottomMargin: 5

                        MouseArea{
                            anchors.fill: parent
                            onDoubleClicked: {
                                parent.selectByMouse = true
                                parent.focus = true
                            }
                        }
                    }
                }
            }

        }

        /* Item {
            id: tab_add
            Button{
                anchors.fill: parent
                text: "Add a new page"
                font.bold: true
                font.pointSize: 14

                onClicked: {
                    var item = Qt.createQmlObject("import QtQuick 2.5;
                        Item{
                            Rectangle{
                                anchors.fill: parent
                                color:'#202020'
                                TextEdit {
                                    text: 'EDITABLE'
                                    wrapMode: TextEdit.Wrap;
                                    textFormat: TextEdit.PlainText;

                                    focus:true;
                                    selectByMouse: true;
                                    selectByKeyboard: true;

                                    color: '#FFF';

                                    anchors.fill: parent
                                    anchors.leftMargin: 5
                                    anchors.rightMargin: 5
                                    anchors.topMargin: 5
                                    anchors.bottomMargin: 5
                                }
                            }
                        }", swipeview, "swipeitem")

                    var bar = Qt.createQmlObject('import QtQuick 2.5;
                            TabButton{
                                text: "New Tab"
                                width: Math.max(80, tabBar.width/4)
                                height: parent.height
                                font.bold: true
                                background: Rectangle {
                                          implicitWidth: 100
                                          implicitHeight: 40
                                          color: parent.checked ? contentBackgroundSecondary : contentBackground
                                      }
                            }', tabBar, "baritem")
                    //swipeview.addItem(item)
                    //tabBar.addItem(bar)
                }

            }
        }*/
    }

    /*

    Rectangle{
        id: baseRect;
        width: base.width;
        height: base.height-10;
        color: "#F6111111";
        radius: 12;
        border.color: "#11FFFFFF";
        border.width: 1;
    }

    FocusScope{
        id:focusScope;
        focus:true;

        onActiveFocusChanged: {
            if(!focusScope.activeFocus){
                wFile.saveFile(widgetName, "notes.txt", (editorName.text + "%^" + editor.text));
                baseRect.border.width = 0;
            }else {
                baseRect.border.width = 1;
            }
        }

        TextEdit{
            id: editorName
            y: 8;
            width: base.width - 10;

            text: fileContent.split("%^")[0].length > 0 ? fileContent.split("%^")[0] : "#Give a Name#";
            color: "#F0F0F0";

            font.bold: true;


            horizontalAlignment: Text.AlignHCenter;
        }

        TextEdit{
            id:editor
            x: 10;
            y: 32;
            width: base.width - 20;
            height: base.height - 50 + 10;

            wrapMode: TextEdit.Wrap;
            textFormat: TextEdit.PlainText;

            focus:true;
            selectByMouse: true;
            selectByKeyboard: true;

            color: "#FFF";
            text: fileContent.split("%^")[1] !== undefined ? fileContent.split("%^")[1] : "#Notes here!#";

        }
    } */

    // Widgetci's file manager. (for read & write files.) (import com.widgetci.file 1.0)
    WFile{
        id: wFile;
    }
}
