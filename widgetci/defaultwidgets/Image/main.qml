/*
    == Widgetci - https://github.com/eminfedar/widgetci ==

    "THIS 'Image' IS ONE OF THE DEFAULT WIDGETS OF 'WIDGETCI'.
    CAN BE EDITED AND USED."

    - @eminfedar
*/

import QtQuick 2.5

Item {
    id: base;
    width: 200
    height: 200*9/16;

    /*property bool imgExpanded: false;

    // Expanding when double click
    SmoothedAnimation {
        id: widthAnim;
        targets: [img1, base]
        properties: "width";
        duration: 300
        from: 200;
        to: 600;
        easing.type: Easing.InOutQuad;
    }
    SmoothedAnimation {
        id: heightAnim;
        targets: [img1, base]
        properties: "height";
        duration: 300
        from: 200*9/16;
        to: 600*9/16;
        easing.type: Easing.InOutQuad;
    }

    // De-Expanding
    SmoothedAnimation {
        id: widthAnimRev;
        targets: [img1, base]
        properties: "width";
        duration: 300
        from: 600;
        to: 200;
        easing.type: Easing.InOutQuad;
    }
    SmoothedAnimation {
        id: heightAnimRev;
        targets: [img1, base]
        properties: "height";
        duration: 300
        from: 600*9/16;
        to: 200*9/16;
        easing.type: Easing.InOutQuad;
    } */

    // Image Example
    Image { // AnimatedImage for gifs.
        id: img1;
        width: 200;
        height: 200*9/16;
        source: "clock_img.jpg";
        opacity: 0.5;

        MouseArea{
            id: img1_ma;
            anchors.fill: parent;
            hoverEnabled: true;
            /*onDoubleClicked: {
                if(!imgExpanded){
                    widthAnim.start();
                    heightAnim.start();
                }else{;
                    widthAnimRev.start();
                    heightAnimRev.start();
                }

                imgExpanded = !imgExpanded;
            }*/
            onDoubleClicked: {
                Qt.openUrlExternally(img1.source.toString());
            }

            onEntered: {
                img1.opacity = 1;
                img1_ma.cursorShape = Qt.PointingHandCursor;
            }
            onExited: {
                img1.opacity = 0.5;
                //img1_ma.cursorShape = Qt.ArrowCursor;
            }
        }
    }
}
