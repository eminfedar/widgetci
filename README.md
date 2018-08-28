# Widgetci
a Widget Management App can run on Win/Linux/Mac.

Uses **QT5.7.1**'s "**QML** + **Javascript** Engine" for **widgets.**

![Image](https://raw.githubusercontent.com/eminfedar/widgetci/dev-unstable/widgetci_1.0.3.png)

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

### How to Create a Widget?
Visit the Wiki =>> [How to Create a Widget](https://github.com/eminfedar/widgetci/wiki/How-to-create-a-widget)


### How to build from source code (Linux) ###
Download the project:
```
git clone https://github.com/eminfedar/widgetci.git
```

#### Building: ####
Requirements:
- **Qt 5.7.1** (or >=)
- **QtCreator** *(optional, you can build it from qtcreator just by opening the project on it and building it)*

#### Building WITHOUT QtCreator: ####
```
cd widgetci/widgetci/ #path of the .pro file
mkdir buildFolder
cd buildFolder
qmake ../
make
```

#### Run:
`./widgetci`
