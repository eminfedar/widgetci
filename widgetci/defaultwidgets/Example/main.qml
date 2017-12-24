// This is simple example of a widget.
// Main widget must named by "main.qml" in the widget folder.
// You must create a 'Item' or 'Rectangle' object as a parent

// If you want a background has color, you can use Rectangle and set it's color property.
/* FOR INFO ABOUT ALL OF THINGS YOU CAN DO AND QML API DOCUMENT : http://doc.qt.io/qt-5/qmltypes.html */
import QtQuick 2.5

Item {
    width: 300
    height: 300

    // Rectangle Example
    Rectangle{
        width: 30
        height: 30
        y: 30
        color: "#FFFF0000";
    }

    // Timer Example
    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: {
            time.text = Date().toString();
        }
    }
    Text { id: time; text: "Hey!"; color:"#FFFFFF" }

    // Image Example
    Image { // AnimatedImage for gifs.
        id: img1
        width: 30
        height: 30
        x: 50
        y: 30
        source: "test.png"
    }

    // Text Input Example
    TextInput {
        y: 80
        color: "#FFFFFF"
        text: "Write here"
        focus: true
    }

    // Get text from an url:
    function getText(url, textelement){
        var request = new XMLHttpRequest();
        request.onreadystatechange = function() {
            if (request.readyState === XMLHttpRequest.DONE)
                textelement.text = request.responseText;
        }
        request.open("get", url);
        request.setRequestHeader("Content-Encoding", "UTF-8");
        request.send('');

        return "Loading...";
    }

    Text {
        id: urltesttext
        y: 100
        color: "#00FF00"
        text: getText("https://api.ipify.org/?format=plain", urltesttext)
    }

    // Clickable text example.
    /*Text {
        y: 130
        width: 50
        height: 20
        color: "#FF0000"
        text: "Click here!"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                parent.text = "Okay okay enough!";
                console.log(parent.text);
            }
        }
    }

    // Clickable image example.
    Image {
        x: 150
        y: 130
        width: 30
        height: 30
        source: "test.png"
        MouseArea{
            anchors.fill: parent
            onPressed: {
                console.log("img clicked");
            }
        }
    }*/
}
