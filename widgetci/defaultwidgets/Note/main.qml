import QtQuick 2.5
import com.widgetci.file 1.0

Item {
    id: base;
    // Required for saving file.
    property string widgetName: "Note"; // Name of the folder.
    property string fileContent: wFile.readFile(widgetName, "notes.txt");

    width: 250;
    height: 400;

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
    }

    // Widgetci's file manager. (for read & write files.) (import com.widgetci.file 1.0)
    WFile{
        id: wFile;
    }
}
