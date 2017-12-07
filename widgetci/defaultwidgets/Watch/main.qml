/*
    == Widgetci - https://github.com/eminfedar/widgetci ==

    "THIS 'watch' IS ONE OF THE DEFAULT WIDGETS OF 'WIDGETCI'.
    CAN BE EDITED AND USED."

    - @eminfedar
*/

import QtQuick 2.5

Item {
    width: 320;
    height: 200;

    property var locale: Qt.locale();
    property real intervalMs:  (60000 - ((new Date().getSeconds() * 1000) + new Date().getMilliseconds()));

    Timer{
        interval: intervalMs; running: true; repeat: true;
        onTriggered: {
            intervalMs = 60000; // Did this because of precise reloading when the time arrived.
            timeText.text = new Date().toLocaleString(locale, "HH:mm");
        }
    }
    Text{
        id:timeText;
        width: 290;

        horizontalAlignment: Text.AlignRight;
        font.pixelSize: 130;
        font.family: "San Francisco Display";
        font.styleName: "Ultralight";

        text: new Date().toLocaleString(locale, "HH:mm");
        color: "#FFFFFFFF";
        opacity: 0.8;
    }

    Text{
        id:dateText;
        y:130;
        width:283;

        horizontalAlignment: Text.AlignRight;
        font.pixelSize: 40;
        font.family: "San Francisco Display";
        font.styleName: "Ultralight";

        text: new Date().toLocaleString(locale, "dddd, dd/MM");
        color: "#FFFFFFFF";
        opacity: 0.8;
    }
}
