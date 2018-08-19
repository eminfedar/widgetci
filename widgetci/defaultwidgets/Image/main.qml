/*
    == Widgetci - https://github.com/eminfedar/widgetci ==

    "THIS 'Image' IS ONE OF THE DEFAULT WIDGETS OF 'WIDGETCI'.
    CAN BE EDITED AND USED."

    - @eminfedar
*/

import QtQuick 2.5

Item {
    id: base
    width: 200
    height: 200*9/16

    // Image source
    property var imageSource: "clock_img.jpg"

    // Prevents from dragging.
    property bool locked: false

    // Image Example
    Image {
        id: img1
        anchors.fill: parent
        source: imageSource
        opacity: 0.5

        MouseArea{
            id: img1_ma
            anchors.fill: parent
            hoverEnabled: true
            onDoubleClicked: {
                Qt.openUrlExternally(img1.source.toString())
            }

            onEntered: {
                base.opacity = 1;
                img1_ma.cursorShape = Qt.PointingHandCursor
            }
            onExited: {
                base.opacity = 0.5
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
                locked = true
            }
            onPositionChanged: {
                if(locked){
                    var delta = Qt.point(mouse.x - rPoint.x, mouse.y - rPoint.y)
                    base.width += delta.x
                    base.height += delta.y
                }
            }
            onReleased: {
                locked = false
            }
        }
    }
}
