import QtQuick 2.5
import com.widgetci.system 1.0 //WSystem

Item {
    width: cputext.width
    height: cputext.height

    // For getting usage percentages.
    WSystem{
        id:wSystem
    }



    Timer{
        interval: 750; running: true; repeat: true;
        onTriggered: {
            if(Qt.platform.os !== "linux" ){
                cputext.text = "This widget works only on Linux Systems."
                running = false
            }else{
                cputext.text = "CPU: " + wSystem.getCPUName() +
                        "\n - Cores: " + wSystem.getCPUUsage() + "% (" + wSystem.getCPUCoresUsages().join("%, ") + "%)" +
                        "\n - MHz(s): " + wSystem.getCPUCurrentClockSpeeds().join(", ") + "\n" +
                        "GPU: " + "..." + "\n" +
                        "Storage: " + "..." + "\n" +
                        "Network: " + "..." + "\n" +
                        "Process List: " + "...";
            }
        }
    }

    Text{
        id: cputext;
        text: "Getting cpu info..."
        color: "#FFFFFF"
        font.pixelSize: 16
    }
}


























