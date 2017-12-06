import QtQuick 2.8
import com.widgetci.file 1.0

Item {

    property string savePath: "/home/eminfedar/.config/Widgetci/widgets/Note/notes.txt"

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
                wFile.saveFile(savePath, editor.text);
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
            text: wFile.readFile(savePath);

        }
    }

    // Widgetci's file manager. (for read & write files.)
    WFile{
        id: wFile;
    }
}
