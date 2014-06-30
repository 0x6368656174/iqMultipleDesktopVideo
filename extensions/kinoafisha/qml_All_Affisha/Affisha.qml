// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Image {
    id: affisha
    property int hallCount: 20
    property int maxHallHeigth: 0
    onMaxHallHeigthChanged: updateOnFilmAffishaChildHeight()
    onHeightChanged: {
        title.height = Math.ceil(affisha.height / hallCount * 1.2)
        legends.height = Math.ceil(affisha.height / hallCount)
        onHallCountChanged()
    }

    Component.onCompleted: onHallCountChanged()

    source: "images/background.jpg"

    Timer {
        id: timeUpdateTimer
        interval: 1000
        repeat: true
        running: true
        onTriggered: updateDateAndTime()
    }

    function updateDateAndTime()
    {
        var curDateTime = new Date()
        dateText.text = Qt.formatDate(curDateTime, "dd.MM.yyyy ddd")
        timeText.text = Qt.formatDateTime(curDateTime, "hh:mm:ss")
    }

    Rectangle {
        id: title
        color: "#00000000"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
//        height: Math.ceil(affisha.height / hallCount * 1.2)
        Text {
            id: dateText
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: height * 0.6
            text: "01.01.2001 Thu"
            font.bold: true
            font.pixelSize: height * 0.45
            color: "#f3f3f3"
            verticalAlignment: Qt.AlignVCenter
        }

        Text {
            id: timeText
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: height * 0.6
            text: "00:00:00"
            font.bold: dateText.font.bold
            font.pixelSize: dateText.font.pixelSize
            color: dateText.color
            verticalAlignment: dateText.verticalAlignment
        }

        Text {
            id: titleText
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: dateText.right
            anchors.right: timeText.left
            text: qsTr("Film Billboard")
            font.bold: dateText.font.bold
            font.pixelSize: dateText.font.pixelSize
            color: dateText.color
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: dateText.verticalAlignment
        }
    }
    Rectangle {
        id: titleBorder
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: title.bottom
        height: 4
        color: "#888888"
    }

    Column {
        id: onFilmAffisha
//        anchors.top: liveAffisha.bottom
        anchors.top: titleBorder.bottom
        anchors.bottom: legendsBorder.top
        anchors.right: parent.right
        anchors.left: parent.left
        onHeightChanged: {updateOnFilmAffishaChildHeight(); updateAffishaTitleWidths()}
    }

    Rectangle {
        id: legendsBorder
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: legends.top
        height: titleBorder.height
        color: titleBorder.color
    }
    Row {
        id: legends
        anchors.bottom: parent.bottom
        anchors.right: parent.right
//        height: Math.ceil(affisha.height / hallCount)
            CinemaSeance {
                startDateTime: "01-01-01 21:30"
                duration: 120
                price: [100, 200]
                freePlace: 200
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }
            Text {
                text: qsTr(" - film is over  ")
                color: "#f3f3f3"
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Qt.AlignVCenter
                font.bold: true
                font.pixelSize: height * 0.4
            }
            CinemaSeance {
                id: cinemaLegend
                startDateTime: getCurrentSeanse()
                duration: 120
                price: [100, 200]
                freePlace: 200
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                function getCurrentSeanse()
                {
                    var currendDateTime = new Date()
                    currendDateTime.setTime(currendDateTime.getTime() - 2000000)
                    return Qt.formatDateTime(currendDateTime, "yyyy-MM-dd hh:mm:ss")
                }
                Component.onCompleted: stop()
                function stop()
                {
                    cinemaLegend.stopUpdate()
                }
            }
            Text {
                text: qsTr(" - film has already shown   ")
                color: "#f3f3f3"
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Qt.AlignVCenter
                font.bold: true
                font.pixelSize: height * 0.4
            }
            CinemaSeance {
                startDateTime: getCurrentSeanse()
                duration: 120
                price: [150]
                freePlace: 200
                is3D: true
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                function getCurrentSeanse()
                {
                    var currendDateTime = new Date()
                    currendDateTime.setTime(Date.parse(currendDateTime.toDateString() + " 23:30:00"))
                    return Qt.formatDateTime(currendDateTime, "yyyy-MM-dd hh:mm:ss")
                }
            }
            Text {
                text: qsTr(" - film today   ")
                color: "#f3f3f3"
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Qt.AlignVCenter
                font.bold: true
                font.pixelSize: height * 0.4
            }
            CinemaSeance {
                startDateTime: getCurrentSeanse()
                duration: 120
                price: [100, 200]
                freePlace: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                function getCurrentSeanse()
                {
                    var currendDateTime = new Date()
                    currendDateTime.setTime(Date.parse(currendDateTime.toDateString() + " 23:30:00"))
                    return Qt.formatDateTime(currendDateTime, "yyyy-MM-dd hh:mm:ss")
                }
            }
            Text {
                text: qsTr(" - no tickets   ")
                color: "#f3f3f3"
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Qt.AlignVCenter
                font.bold: true
                font.pixelSize: height * 0.4
            }
            CinemaSeance {
                startDateTime: getCurrentSeanse()
                duration: 120
                price: [300, 350]
                freePlace: 200
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                function getCurrentSeanse()
                {
                    var currendDateTime = new Date()
                    currendDateTime.setTime(currendDateTime.getTime() + 3600000 * 24)
                    return Qt.formatDateTime(currendDateTime, "yyyy-MM-dd hh:mm:ss")
                }
            }
            Text {
                text: qsTr(" - film tomorrow   ")
                color: "#f3f3f3"
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Qt.AlignVCenter
                font.bold: true
                font.pixelSize: height * 0.4
            }
    }

    function addCinemaSeance(cinemaId, hallName, name, startDateTime, duration, is3D, price, freePlace)
    {
        var cinemaFinded = false
        for (var i = 0; i < onFilmAffisha.children.length; i++)
        {
            if (onFilmAffisha.children[i].name === name)
            {
                onFilmAffisha.children[i].addCinemaSeance(cinemaId, hallName, startDateTime, duration, is3D, price, freePlace)
                cinemaFinded = true
            }
        }
        if (!cinemaFinded)
        {
            createCinema(name)
            addCinemaSeance(cinemaId, hallName, name, startDateTime, duration, is3D, price, freePlace)
        }
        onHallCountChanged()
        updateAffishaTitleWidths()
    }

    function setCinemaFreePlace(cinemaId, freePlace)
    {
        for (var i = 0; i < onFilmAffisha.children.length; i++)
        {
            onFilmAffisha.children[i].setCinemaFreePlace(cinemaId, freePlace)
        }
    }

    function createCinema(name)
    {
        var component = Qt.createComponent("Cinema.qml")
        var cinema = component.createObject(onFilmAffisha, {"name": name})
        if (cinema === null)
        {
            console.log("Error creating object")
        } else
        {
            cinema.anchors.left = onFilmAffisha.left
            cinema.anchors.right = onFilmAffisha.right
            cinema.maxHallHeigth = maxHallHeigth
            cinema.hallCountChanged.connect(onHallCountChanged())
        }
        updateOnFilmAffishaChildHeight()
    }

    function updateAffishaTitleWidths()
    {
        var affishaMaxTitleWidth = getAffishaMasxTitleWidth()

        for (var i = 0; i < onFilmAffisha.children.length; i++)
        {
            onFilmAffisha.children[i].titleWidth = affishaMaxTitleWidth
        }

    }

    function getAffishaMasxTitleWidth()
    {
        var affishaMaxTitleWidth = -1
        for (var i = 0; i < onFilmAffisha.children.length; i++)
        {
            if (onFilmAffisha.children[i].getTitleWidth() > affishaMaxTitleWidth)
                affishaMaxTitleWidth = onFilmAffisha.children[i].getTitleWidth()
        }
        return affishaMaxTitleWidth
    }

    function updateOnFilmAffishaChildHeight()
    {
        for (var i = 0; i < onFilmAffisha.children.length; i++)
        {
            onFilmAffisha.children[i].maxHallHeigth = maxHallHeigth
        }
        updateAffishaTitleWidths()
    }

    function onHallCountChanged()
    {
        var newHallCount = 0
        for (var i = 0; i < onFilmAffisha.children.length; i++)
        {
            newHallCount += onFilmAffisha.children[i].hallCount
        }
        var affishaRepHeight = onFilmAffisha.height
        affishaRepHeight = affishaRepHeight - onFilmAffisha.children.length * 1
        if (newHallCount > hallCount)
        {
//            maxHallHeigth = Math.floor(affishaRepHeight / newHallCount)
            maxHallHeigth = Math.floor(affishaRepHeight / hallCount)
        } else
        {
            maxHallHeigth = Math.floor(affishaRepHeight / hallCount)
        }
    }
}
