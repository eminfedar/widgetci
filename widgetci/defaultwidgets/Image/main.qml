/*
    == Widgetci - https://github.com/eminfedar/widgetci ==

    "THIS 'Image' IS ONE OF THE DEFAULT WIDGETS OF 'WIDGETCI'.
    CAN BE EDITED AND USED."

    - @eminfedar
*/

import QtQuick 2.5
import com.widgetci.file 1.0

Item {
    id: base

    // Image source
    property string imageSource: "clock_img.jpg"
    property string widgetName: "Image" // Required for saving conf file

    property var size: wFile.readFile(widgetName, "image.conf").split("|");

    width: parseInt(size[0])
    height: parseInt(size[1])

    property bool isResizing: false

    // Image Example
    Image {
        id: img1
        anchors.fill: parent
        source: imageSource

        MouseArea{
            id: img1_ma
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                Qt.openUrlExternally(img1.source.toString())
            }

            onEntered: {
                img1_ma.cursorShape = Qt.PointingHandCursor
            }

            onReleased: {
                isResizing = false
            }
        }
    }

    property var rPoint: "0,0"
    Image {
        id: resizer
        x: base.width - 16
        y: base.height - 16
        width: 16
        height: 16
        source: "resizer.png"

        MouseArea{
            id: resizer_ma
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                rPoint = Qt.point(mouse.x, mouse.y)
                isResizing = true
            }
            onPositionChanged: {
                if(isResizing){
                    var delta = Qt.point(mouse.x - rPoint.x, mouse.y - rPoint.y)
                    base.width += delta.x
                    base.height += delta.y
                }
            }
            onReleased:{
                isResizing = false
                wFile.saveFile(widgetName, "image.conf", (base.width + "|" + base.height))
            }
        }
    }

    WFile{
        id: wFile
    }
}
