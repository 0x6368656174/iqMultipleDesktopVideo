// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: hall
    property string name: ""
    property Item animationTaget
    clip: true
    color: "#00000000"

    function addCinema(cinemaObj)
    {
        var component = Qt.createComponent("CinemaSeance.qml")
        var lastCinema = null
        if (futured.children.length > 0)
            lastCinema = futured.children[futured.children.length - 1]
        var cinema = component.createObject(futured, {"cinemaId": cinemaObj.cinemaId,
                                                "startDateTime": Qt.formatDateTime(cinemaObj.startDateTime, "yyyy-MM-dd hh:mm:ss"),
                                                "price" : cinemaObj.price,
                                                "freePlace": cinemaObj.freePlace,
                                                "duration": cinemaObj.duration,
                                                "is3D": cinemaObj.is3D})
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
        futured.anchors.rightMargin = -getFuturedAnchors()
        futured.anchors.leftMargin = -getPassedAnchors()
        var currentDataTime = new Date()
        for (var i = 0; i < futured.children.length; i++)
        {
            if (futured.children[i].getEndDateTime().getTime() < currentDataTime.getTime())
            {
                moveCinemaToPassed(futured.children[i])
                return
            }
        }
    }

    function moveCinemaToPassed(target)
    {
        var passedAnchors = getPassedAnchors()
        var futuredAnchors = getFuturedAnchors()
        var passedWidth = getPassedWidth()
        var futuredWidth = getFuturedWidth()
        target.parent = hall
        target.anchors.right = undefined
        target.anchors.left = passed.right
        target.anchors.leftMargin = hall.width - ((passedWidth - passedAnchors) + (futuredWidth - futuredAnchors))
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
        var result = 0
        if (futured.children.length > getMaxFuturedCinemaNumber())
        {
            result = Math.ceil((futured.children.length - getMaxFuturedCinemaNumber()) * futured.children[0].width)
        }
        return result

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

    function getMaxFuturedCinemaNumber()
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

        result = Math.floor((hall.width - ivisibleHallName.width * 1) / cinema.width)
        if (result < 0)
            return 0
        return result
    }

    Style {
        id: style
    }

    Rectangle {
        id: backgroundRectangle
        color: {
            if (style.cinemaHallBackgroundUseGradient)
            {
                return "#00000000"
            } else
            {
                return style.cinemaHallBackgroundColor
            }
        }

        Rectangle {
            visible: style.cinemaHallBackgroundUseGradient && style.cinemaHallBackgroundHorizontalGradient
            rotation: 270
            anchors.centerIn: parent
            width: parent.height
            height: parent.width
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: style.cinemaHallBackgroundGradientStartColor
                }

                GradientStop {
                    position: 1
                    color: style.cinemaHallBackgroundGradientEndColor
                }
            }
        }
        Rectangle {
            visible: style.cinemaHallBackgroundUseGradient && !style.cinemaHallBackgroundHorizontalGradient
            anchors.fill: parent
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: style.cinemaHallBackgroundGradientStartColor
                }

                GradientStop {
                    position: 1
                    color: style.cinemaHallBackgroundGradientEndColor
                }
            }
        }
        opacity: style.cinemaHallBackgroundOpacity
        anchors.fill: parent
        anchors.topMargin: Math.ceil(hall.height * 0.05)
        anchors.bottomMargin: anchors.topMargin
    }


    Text
    {
        id: hallName
        color: style.cinemaHallNameTextColor
        text: hall.name + "   "
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: passed.right
        anchors.right: futured.left
        font.bold: style.cinemaHallNameTextBold
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
        ParallelAnimation {
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
                duration: 5000
                easing.type: Easing.OutQuad
            }
        }
        ScriptAction {
            script:{
                var lastCinema = null
                if (passed.children.length > 0)
                    lastCinema = passed.children[passed.children.length - 1]
                hall.animationTaget.parent = passed
                hall.animationTaget.anchors.top = passed.top
                hall.animationTaget.anchors.bottom = passed.bottom
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
        anchors.bottom: bottomBorderRectangle.top
        anchors.left: parent.left
        anchors.leftMargin: -hall.getPassedAnchors()
        width: hall.getPassedWidth()
        color: "#00000000"

        Behavior on anchors.leftMargin { NumberAnimation {duration: 500; easing.type: Easing.OutQuad;} }
    }

    Rectangle {
        id: futured
        anchors.top: parent.top
        anchors.bottom: bottomBorderRectangle.top
        anchors.right: parent.right
        anchors.rightMargin: -hall.getFuturedAnchors()
        width: hall.getFuturedWidth()
        color: "#00000000"

        Behavior on anchors.rightMargin  { NumberAnimation {duration: 500; easing.type: Easing.OutQuad;} }
    }

    Rectangle {
        id: bottomBorderRectangle
        height: style.cinemaListHallBorderWidth
        color: style.cinemaListHallBorderColor
        anchors.left: parent.left
        anchors.leftMargin: height
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
