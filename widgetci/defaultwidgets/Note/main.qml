import QtQuick 2.5
import com.widgetci.file 1.0

Item {

    // Required for saving file.
    property string widgetName: "Note"; // Name of the folder.

    width: 200;
    height: 300;

    Rectangle{
        width:200;
        height:300;
        color: "#99111111";
        radius: 12;
    }

    FocusScope{
        id:focusScope;
        focus:true;

        onActiveFocusChanged: {
            if(!focusScope.activeFocus){
                wFile.saveFile(widgetName, "notes.txt", editor.text);
            }
        }

        Text{
            y: 5;
            width: 190;

            text: "Keep";
            color: "#F0F0F0";


            horizontalAlignment: Text.AlignHCenter;
        }

        TextEdit{
            id:editor
            x: 10;
            y: 25;
            width:180;
            height:260;

            font.bold: true;

            wrapMode: TextEdit.Wrap;
            textFormat: TextEdit.PlainText;

            focus:true;
            selectByMouse: true;
            selectByKeyboard: true;

            color: "#FFF";
            text: wFile.readFile(widgetName, "notes.txt");

        }
    }

    // Widgetci's file manager. (for read & write files.) (import com.widgetci.file 1.0)
    WFile{
        id: wFile;
    }
}
