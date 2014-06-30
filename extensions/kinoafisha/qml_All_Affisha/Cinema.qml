// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: cinema
    property string name: ""
    property string duration: "00:00:00"
    property bool is3D: false
    property int maxHallHeigth: -1
    property int titleWidth: -1
    property int hallCount: halls.children.length
    onHallCountChanged: updateHeigth()
    onMaxHallHeigthChanged: updateHeigth()

    function updateHeigth()
    {
        var newHeight = maxHallHeigth
        if (halls.children.length > 0)
            newHeight = Math.ceil(maxHallHeigth * halls.children.length + borderRect.height - 1)
        cinema.height = newHeight
    }

    color: "#00000000"

    Row{
        id: titleRow
        anchors.left: parent.left
        anchors.leftMargin: Math.ceil(cinema.maxHallHeigth * 0.2)
        anchors.top: parent.top
        anchors.bottom: borderRect.top
        width: getWidth()
        spacing: Math.ceil(cinema.maxHallHeigth * 0.2)
        onHeightChanged: nameText.font.pixelSize = nameText.getFontPixelSize()

        function getWidth()
        {
            if (titleWidth < 0)
                return undefined
            var result = Math.ceil(titleWidth)
            return result
        }
        Text {
            id: nameText
            text: name
            color: "#f3f3f3"
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            verticalAlignment: Qt.AlignVCenter
            font.bold: true
            font.pixelSize: getFontPixelSize()
            function getFontPixelSize()
            {
                var result = Math.ceil(maxHallHeigth * 0.4)
                return result
            }
        }

        Text {
            id: is3Dtext
            visible: cinema.is3D
            text: "3D"
            color: "#000000"
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            font.bold: true
            font.pixelSize: nameText.font.pixelSize
            Rectangle
            {
                id: is3Drect
                anchors.centerIn: is3Dtext
                width: invisibleIs3DText.width
                height: invisibleIs3DText.height
                color: "#4e6b7c"
                opacity: 0.5
                z: -1
            }
        }


    }

    Row {
        id: invisibleTitleRow
        visible: false
        Text {
            id: invisibleNameText
            text: nameText.text
            font.bold: nameText.font.bold
            font.pixelSize: nameText.font.pixelSize
        }

        Text {
            id: invisibleIs3DText
            text: is3Dtext.text
            font.bold: is3Dtext.font.bold
            font.pixelSize: is3Dtext.font.pixelSize
        }
    }

    Column {
        id: halls
        anchors.left: titleRow.right
        anchors.leftMargin: Math.ceil(cinema.maxHallHeigth * 0.2)
        anchors.top: parent.top
        anchors.bottom: borderRect.top
        anchors.right: durationText.left
        anchors.rightMargin: Math.ceil(cinema.maxHallHeigth * 0.2)
        onHeightChanged: updateHallsChildHeight()
    }

    Text {
        id: durationText
        text: getText()
        color: "#f3f3f3"
        anchors.top: parent.top
        anchors.bottom: borderRect.top
        anchors.right: parent.right
        anchors.rightMargin: Math.ceil(cinema.maxHallHeigth * 0.2)
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        font.pixelSize: nameText.font.pixelSize
        font.bold: true
        function getText()
        {
            var currentDateTime = new Date()
            currentDateTime.setTime(Date.parse(currentDateTime.toDateString() + " 00:00:00"))
            currentDateTime.setTime(currentDateTime.getTime() + duration*60000)
            return Qt.formatTime(currentDateTime, "hh:mm")
        }
    }

    Rectangle {
        id: borderRect
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 2
        color: "#00000000"
        Rectangle {
            id: border1
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: titleRow.width + halls.anchors.leftMargin + titleRow.anchors.leftMargin
            color: "#888888"
        }
        Rectangle {
            id: border2
            anchors.left: border1.right
            anchors.leftMargin: borderRect.height
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: border3.left
            anchors.rightMargin: borderRect.height
            color: border1.color
        }
        Rectangle {
            id: border3
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: durationText.width + halls.anchors.rightMargin + durationText.anchors.rightMargin
            color: border1.color
        }
    }

    function getTitleWidth()
    {
        return invisibleTitleRow.width
    }

    function getMinTitleWidth()
    {

    }

    function updateHallsChildHeight()
    {
        for (var i = 0; i < halls.children.length; i++)
        {
                halls.children[i].height = maxHallHeigth
        }
    }

    function createHall(name)
    {
        var component = Qt.createComponent("CinemaHall.qml")
        var hall = component.createObject(halls, {"name": name})
        if (hall === null)
        {
            console.log("Error creating object")
        } else
        {
            hall.anchors.left = halls.left
            hall.anchors.leftMargin = Math.ceil(borderRect.height * 0.5)
            hall.anchors.right = halls.right
            hall.anchors.rightMargin = Math.ceil(borderRect.height * 0.5)
        }
        updateHallsChildHeight()
    }

    function addCinemaSeance(cinemaId, hallName, startDateTime, duration, is3D, price, freePlace)
    {
        var result = null
        var hallFinded = false
        for (var i = 0; i < halls.children.length; i++)
        {
            if (halls.children[i].name === hallName)
            {
                result = halls.children[i].addCinema(cinemaId, startDateTime, duration, price, freePlace, is3D)
                if (is3D && !cinema.is3D)
                    cinema.is3D = true
                if (cinema.duration === "00:00:00")
                    cinema.duration = duration
                hallFinded = true
            }
        }
        if (!hallFinded)
        {
            createHall(hallName)
            result = addCinemaSeance(cinemaId, hallName, startDateTime, duration, is3D, price, freePlace)
        }
        return result
    }

    function setCinemaFreePlace(cinemaId, freePlace)
    {
        for (var i = 0; i < halls.children.length; i++)
        {
            halls.children[i].setCinemaFreePlace(cinemaId, freePlace)
        }
    }
}

