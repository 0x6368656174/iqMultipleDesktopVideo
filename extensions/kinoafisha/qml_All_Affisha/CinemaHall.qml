// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: hall
    property string name: ""
    property Item animationTaget
    clip: true
    color: "#00000000"

    function addCinema(cinemaId, startDateTime, duration, price, freePlace, is3D)
    {
        var component = Qt.createComponent("CinemaSeance.qml")
        var lastCinema = null
        if (futured.children.length > 0)
            lastCinema = futured.children[futured.children.length - 1]
        var cinema = component.createObject(futured, {"cinemaId": cinemaId, "startDateTime": startDateTime,
                                            "price" : price, "freePlace": freePlace, "duration": duration,
                                            "is3D": is3D})
        if (cinema === null)
        {
            console.log("Error creating object")
        } else
        {
            cinema.anchors.top = futured.top
            cinema.anchors.bottom = futured.bottom
            cinema.anchors.right = futured.right
            if (lastCinema)
            {
                lastCinema.anchors.right = cinema.left
            }
        }
        return cinema
    }

    function setCinemaFreePlace(cinemaId, freePlace)
    {
        for (var i = 0; i < hall.children.length; i++)
        {
            if (hall.children[i].cinemaId === cinemaId)
            {
                hall.children[i].freePlace = freePlace
            }
        }
        for (i = 0; i < futured.children.length; i++)
        {
            if (futured.children[i].cinemaId === cinemaId)
            {
                futured.children[i].freePlace = freePlace
            }
        }
        for (i = 0; i < passed.children.length; i++)
        {
            if (passed.children[i].cinemaId === cinemaId)
            {
                passed.children[i].freePlace = freePlace
            }
        }
    }


    function stopUpdate()
    {
        updateTimer.stop()
    }

    function startUpdate()
    {
        updateTimer.interval = 6000
        updateTimer.start()
        fastUpdateTimer.start()
    }

    function stopFastUpdate()
    {
        updateTimer.interval = 30000
    }

    function updateHall()
    {
        var currentDataTime = new Date()
        for (var i = 0; i < futured.children.length; i++)
        {
            if (futured.children[i].getEndDateTime().getTime() < currentDataTime.getTime())
            {
                moveCinemaToPassed(futured.children[i])
                return
            }
        }
        futured.anchors.rightMargin = -getFuturedAnchors()
        futured.anchors.leftMargin = -getPassedAnchors()
    }

    function moveCinemaToPassed(target)
    {
        target.parent = hall
        target.anchors.right = undefined
        target.anchors.left = passed.right
        var passedWidth = getPassedWidth()
        var futuredWidth = getFuturedWidth()
        target.anchors.leftMargin = hall.width - ((passedWidth - getPassedAnchors()) + (futuredWidth - getFuturedAnchors())) - target.width
        animationTaget = target
        hallName.anchors.rightMargin = target.width
        cinemaMoveAnimation.start()
    }

    function getPassedAnchors()
    {
        var result = 0
        if (passed.children.length > getMaxPassedCinemaNumber())
        {
            result = Math.ceil((passed.children.length - getMaxPassedCinemaNumber()) * passed.children[0].width)
        }
        return result
    }

    function getFuturedAnchors()
    {
        if (futured.children.length === 0)
            return 0
        var numberOfShowCinema = 0
        var currentDateTime = new Date()
        for (var i = 0; i < futured.children.length; i++)
        {
            if (!futured.children[i].isAfterTomorrow())
                numberOfShowCinema++;
        }
        return Math.ceil((futured.children.length - numberOfShowCinema) * futured.children[0].width)
    }

    function getFuturedWidth()
    {
        var futuredWidth = 0
        if (futured.children.length > 0)
            futuredWidth = Math.ceil(futured.children.length * futured.children[0].width)
        return futuredWidth
    }

    function getPassedWidth()
    {
        var passedWidth = 0
        if (passed.children.length > 0)
            passedWidth = Math.ceil(passed.children.length * passed.children[0].width)
        return passedWidth
    }

    function getMaxPassedCinemaNumber()
    {
        var result = 0
        var cinema = null
        if (passed.children.length > 0)
        {
            cinema = passed.children[0]
        } else if (futured.children.length > 0)
        {
            cinema = futured.children[0]
        }
        if (!cinema)
            return 0

        var busyWidth = ivisibleHallName.width + (getFuturedWidth() - getFuturedAnchors())
        result = Math.floor(((hall.width - busyWidth) / 2) / cinema.width)
        if (result < 0)
            return 0
        return result
    }

    Text
    {
        id: hallName
        color: "#a5a5a5"
        text: hall.name + "   "
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: passed.right
        anchors.right: futured.left
        font.bold: true
        font.pixelSize: Math.ceil(hall.height * 0.6)
        horizontalAlignment: Qt.AlignRight
        verticalAlignment: Qt.AlignVCenter
    }

    Text
    {
        id: ivisibleHallName
        text: hallName.text
        visible: false
        font.bold: hallName.font.bold
        font.pixelSize: hallName.font.pixelSize
    }

    Timer
    {
        id: updateTimer
        interval: 6000
        running: true
        repeat: true
        onTriggered: hall.updateHall()
    }

    Timer
    {
        id: fastUpdateTimer
        interval: 120000
        running: true
        onTriggered: stopFastUpdate()
    }

    SequentialAnimation {
        id: cinemaMoveAnimation
        ParentAnimation {
            PropertyAnimation {
                target: hall.animationTaget
                property: "anchors.leftMargin"
                to: 0
                duration: 5000
                easing.type: Easing.OutQuad
            }
            PropertyAnimation {
                target: hallName
                property: "anchors.rightMargin"
                to: 0
                duration: 1000
                easing.type: Easing.OutQuad
            }
        }
        ScriptAction {
            script:{
                var lastCinema = null
                if (passed.children.length > 0)
                    lastCinema = passed.children[passed.children.length - 1]
                hall.animationTaget.parent = passed
                if (lastCinema)
                {
                    hall.animationTaget.anchors.left = lastCinema.right
                    hall.animationTaget.anchors.leftMargin = 0
                } else
                {
                    hall.animationTaget.anchors.left = passed.left
                }
                futured.anchors.rightMargin = -getFuturedAnchors()
                passed.anchors.leftMargin = -getPassedAnchors()
            }
        }
    }

    Rectangle {
        id: passed
        anchors.top: parent.top
        anchors.bottom: bottomBorderImage.top
        anchors.left: parent.left
        anchors.leftMargin: -hall.getPassedAnchors()
        width: hall.getPassedWidth()
        color: "#00000000"

        Behavior on anchors.leftMargin { NumberAnimation {duration: 500; easing.type: Easing.OutQuad;} }
    }

    Rectangle {
        id: futured
        anchors.top: parent.top
        anchors.bottom: bottomBorderImage.top
        anchors.right: parent.right
        anchors.rightMargin: -hall.getFuturedAnchors()
        width: hall.getFuturedWidth()
        color: "#00000000"

        Behavior on anchors.rightMargin  { NumberAnimation {duration: 500; easing.type: Easing.OutQuad;} }
    }

    Image {
        id: bottomBorderImage
        fillMode: Image.TileHorizontally
        source: "images/dot.png"
        height: 1
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
