/*
    == Widgetci - https://github.com/eminfedar/widgetci ==

    "THIS 'note' IS ONE OF THE DEFAULT WIDGETS OF 'WIDGETCI'.
    CAN BE EDITED AND USED."

    - @eminfedar
*/

import QtQuick 2.5
import com.widgetci.file 1.0

Item {
    id: base;
    // Required for saving file.
    property string widgetName: "Note"; // Name of the folder.
    property string fileContent: wFile.readFile(widgetName, "notes.txt");

    width: 250;
    height: 300;

    Rectangle{
        id: baseRect;
        width: base.width;
        height: base.height;
        color: "#CC111111";
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
            y: 5;
            width: base.width - 10;

            text: fileContent.split("%^")[0].length > 0 ? fileContent.split("%^")[0] : "#Give a Name#";
            color: "#F0F0F0";


            horizontalAlignment: Text.AlignHCenter;
        }

        TextEdit{
            id:editor
            x: 10;
            y: 27;
            width: base.width - 20;
            height: base.height - 50 + 10;

            font.bold: true;

            wrapMode: TextEdit.Wrap;
            textFormat: TextEdit.PlainText;

            focus:true;
            selectByMouse: true;
            selectByKeyboard: true;

            color: "#FFF";
            text: fileContent.split("%^")[1] !== undefined ? fileContent.split("%^")[1] : "#Notes here!#";

        }
    }

    // Widgetci's file manager. (for read & write files.) (import com.widgetci.file 1.0)
    WFile{
        id: wFile;
    }
}
