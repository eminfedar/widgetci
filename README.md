# Widgetci
a Widget Management App can run on Win/Linux/Mac.

Uses **QT5.7.1**'s "**QML** + **Javascript** Engine" for **widgets.**

![Image](https://github.com/eminfedar/widgetci/raw/master/firststable.gif)

### Simple widget file (.qml):
```
import QtQuick 2.5

// Widget's root object
Item {
    width: 70
    height: 30
    
    
    // A Text element
    Text {
        x: 20
        y: 5
        text: "Hey!"
    }
}
```

