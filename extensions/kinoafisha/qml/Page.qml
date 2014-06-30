// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Column {opacity: 0;
    Behavior on opacity {
        NumberAnimation {
            duration: 500
        }
    }
}
