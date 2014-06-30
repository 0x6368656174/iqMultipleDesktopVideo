// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: cinema
    property string name: ""
    property string duration: "00:00:00"
    property bool is3D: false
    property string ageLimit: ""
    property int maxHallHeigth: -1
    property int hallCount: halls.children.length
    onHallCountChanged: updateHeigth()
    onMaxHallHeigthChanged: updateHeigth()

    function updateHeigth()
    {
        var newHeight = maxHallHeigth
        if (halls.children.length > 0)
            newHeight = Math.ceil(maxHallHeigth * halls.children.length + borderRect.height - 1)
        cinema.height = newHeight
        animatedNameXAnimation.restart()
        if (nameText.paintedWidth > titleRow.width || nameText.paintedHeight > titleRow.height)
        {
            nameText.visible = false
            animatedNameText.visible = true
        } else
        {
            nameText.visible = true
            animatedNameText.visible = false
        }
    }

    color: "#00000000"

    Style {
        id: style
    }

    Rectangle {
        id: titleBackground
        color: {
            if (style.cinemaTitleBackgroundUseGradient)
            {
                return "#00000000"
            } else
            {
                return style.cinemaTitleBackgroundColor
            }
        }

        Rectangle {
            visible: style.cinemaTitleBackgroundUseGradient && style.cinemaTitleBackgroundHorizontalGradient
            rotation: 270
            anchors.centerIn: parent
            width: parent.height
            height: parent.width
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: style.cinemaTitleBackgroundGradientStartColor
                }

                GradientStop {
                    position: 1
                    color: style.cinemaTitleBackgroundGradientEndColor
                }
            }
        }
        Rectangle {
            visible: style.cinemaTitleBackgroundUseGradient && !style.cinemaTitleBackgroundHorizontalGradient
            anchors.fill: parent
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: style.cinemaTitleBackgroundGradientStartColor
                }

                GradientStop {
                    position: 1
                    color: style.cinemaTitleBackgroundGradientEndColor
                }
            }
        }
        opacity: style.cinemaTitleBackgroundOpacity
        anchors.top: titleRow.top
        anchors.bottom: titleRow.bottom
        anchors.left: parent.left
        anchors.right: halls.left
    }

    Row{
        id: titleRow
        anchors.left: parent.left
        anchors.leftMargin: Math.ceil(cinema.maxHallHeigth * 0.2)
        anchors.top: parent.top
        anchors.bottom: borderRect.top
        width: Math.ceil(cinema.width / 4)
        spacing: Math.ceil(cinema.maxHallHeigth * 0.2)
        onHeightChanged: nameText.font.pixelSize = nameText.getFontPixelSize()

        Text {
            id: nameText
            text: name
            color: style.cinemaTitleTextColor
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            verticalAlignment: Qt.AlignVCenter
            font.bold: style.cinemaTitleTextBold
            font.pixelSize: getFontPixelSize()
            width: parent.width - is3Dtext.width*is3Dtext.visible - ageLimitText.width
            wrapMode: Text.WordWrap

            function getFontPixelSize()
            {
                var result = Math.ceil(maxHallHeigth * 0.38)
                return result
            }
        }

        Rectangle{
            id: animatedNameText
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: nameText.width
            clip: true
            visible: false
            color: "#00000000"
            Row {
                NumberAnimation on x { id: animatedNameXAnimation; from: 0; to: -text.width; duration: 10000; loops: Animation.Infinite }
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    id: text;
                    font.pixelSize: nameText.font.pixelSize
                    font.bold: nameText.font.bold
                    color: nameText.color
                    text: name + "         "
                }
                Text {
                    color: text.color
                    font.pixelSize: text.font.pixelSize
                    font.bold: text.font.bold
                    text: text.text
                }
                Text {
                    color: text.color
                    font.pixelSize: text.font.pixelSize
                    font.bold: text.font.bold
                    text: text.text
                }
            }
        }

        Text {
            id: is3Dtext
            visible: cinema.is3D
            text: "3D"
            color: style.cinemaTitleIs3DTextColor
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            font.bold: style.cinemaTitleTextBold
            font.pixelSize: nameText.font.pixelSize
            Rectangle
            {
                id: is3Drect
                anchors.centerIn: is3Dtext
                width: is3Dtext.width
                height: is3Dtext.font.pixelSize
                color: style.cinemaTitleIs3DBackgroundColor
                opacity: style.cinemaTitleIs3DBackgroundOpacity
                z: -1
            }
        }

        Text {
            id: ageLimitText
            visible: text !== ""
            text: ageLimit
            color: style.cinemaTitleAgeLimitTextColor
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            font.bold: style.cinemaTitleAgeLimitTextBold
            font.pixelSize: nameText.font.pixelSize
            width: ageLimitRect.width
            Rectangle
            {
                id: ageLimitRect
                anchors.centerIn: ageLimitText
                width: nameText.font.pixelSize * 5
                height: is3Dtext.font.pixelSize
                color: style.cinemaTitleAgeLimitBackgroundColor
                opacity: style.cinemaTitleAgeLimitBackgroundOpacity
                z: -1
            }
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

    Rectangle {
        id: durationBackground
        color: {
            if (style.cinemaDurationBackgroundUseGradient)
            {
                return "#00000000"
            } else
            {
                return style.cinemaDurationBackgroundColor
            }
        }

        Rectangle {
            visible: style.cinemaDurationBackgroundUseGradient && style.cinemaDurationBackgroundHorizontalGradient
            rotation: 270
            anchors.centerIn: parent
            width: parent.height
            height: parent.width
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: style.cinemaDurationBackgroundGradientStartColor
                }

                GradientStop {
                    position: 1
                    color: style.cinemaDurationBackgroundGradientEndColor
                }
            }
        }
        Rectangle {
            visible: style.cinemaDurationBackgroundUseGradient && !style.cinemaDurationBackgroundHorizontalGradient
            anchors.fill: parent
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: style.cinemaDurationBackgroundGradientStartColor
                }

                GradientStop {
                    position: 1
                    color: style.cinemaDurationBackgroundGradientEndColor
                }
            }
        }
        opacity: style.cinemaDurationBackgroundOpacity
        anchors.top: titleRow.top
        anchors.bottom: titleRow.bottom
        anchors.left: halls.right
        anchors.leftMargin: Math.ceil(maxHallHeigth * 0.05)
        anchors.right: parent.right
    }

    Text {
        id: durationText
        text: getText()
        color: style.cinemaDurationTextColor
        anchors.top: parent.top
        anchors.bottom: borderRect.top
        anchors.right: parent.right
        anchors.rightMargin: Math.ceil(cinema.maxHallHeigth * 0.2)
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        font.pixelSize: nameText.font.pixelSize
        font.bold: style.cinemaDurationTextBold
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
        height: style.cinemaListCinemaBorderWidth
        color: "#00000000"
        Rectangle {
            id: border1
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: titleRow.width + halls.anchors.leftMargin + titleRow.anchors.leftMargin
            color: style.cinemaListCinemaBorderColor
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

    function addCinemaSeance(cinemaObj)
    {
        var result = null
        var hallFinded = false
        var hallName = cinemaObj.hallName

        //Хак для кинотеатра Хабаровск
        if (hallName === "зал 1"){
            hallName = "Rio"
        }
        else if (hallName === "зал 2"){
            hallName = "Ice"
        }
        else if (hallName === "зал 3"){
            hallName = "Tron"
        }

        for (var i = 0; i < halls.children.length; i++)
        {
            if (halls.children[i].name === hallName)
            {
                result = halls.children[i].addCinema(cinemaObj)
                if (cinemaObj.is3D && !cinema.is3D)
                    cinema.is3D = true
                if (cinema.duration === "00:00:00")
                    cinema.duration = cinemaObj.duration
                if (cinema.ageLimit === "")
                    cinema.ageLimit = cinemaObj.ageLimit
                hallFinded = true
            }
        }
        if (!hallFinded)
        {
            createHall(hallName)
            result = addCinemaSeance(cinemaObj)
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

