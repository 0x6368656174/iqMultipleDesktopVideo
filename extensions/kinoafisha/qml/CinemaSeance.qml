// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: cinemaSeance

    property variant price: []
    property int freePlace: -1
    property string startDateTime: "2001-01-01 00:00:00"
    property int duration: 0
    property bool is3D: false
    property string cinemaId: ""
    property int priceUpdateInterval: 300
    property real progress: 0
    onFreePlaceChanged: startStopPriceAnimation()
    onPriceChanged: startStopPriceAnimation()


    width: Math.ceil(height * 1.8)
    Component.onCompleted: updateStates()
    color: "#00000000"

    function stopUpdate()
    {
        updateStates()
        updateTimer.stop()
    }

    function isFutured()
    {
        var start = getStartDateTime()
        var currentDateTime = new Date()
        if (start.getTime() - 24*3600000 > currentDateTime.getTime())
            return true
        return false
    }

    function isTomorrow()
    {
        var start = getStartDateTime()
        var current = new Date()
        var tomorow = new Date()
        tomorow.setTime(current.getTime() + 3600000 * 24)
        tomorow.setTime(Date.parse(Qt.formatDateTime(tomorow, "MM/dd/yyyy") + " 00:00:00"))
        return start.getTime() >= tomorow.getTime()
    }

    function isAfterTomorrow()
    {
        var start = getStartDateTime()
        var current = new Date()
        var afterTomorow = new Date()
        afterTomorow.setTime(current.getTime() + 3600000 * 48)
        afterTomorow.setTime(Date.parse(Qt.formatDateTime(afterTomorow, "MM/dd/yyyy") + " 00:00:00"))
        return start.getTime() >= afterTomorow.getTime()
    }

    function getStartDateTime()
    {
        var start = new Date()
        var startDateTimeStr = startDateTime
        startDateTimeStr = startDateTimeStr.replace(/(\d+)-(\d+)-(\d+)/, '$2/$3/$1')
        start.setTime(Date.parse(startDateTimeStr))
        return start
    }

    function getEndDateTime()
    {
        var start = getStartDateTime()
        var end = new Date()
        end.setTime(start.getTime() + duration * 60000)
        return end
    }

    function startStopPriceAnimation ()
    {
        //Установим новое значение цены
        var priceStr = ""
        if (freePlace === 0)
        {
            priceStr = qsTr("NO Tickets")
        }
        else if (price)
        {
            priceStr = price.join(", ")

            if (price.length > 1)
                priceStr += ", "
        }
        priceText.text = priceStr

        //Запустим анимацию цены
        if (cinemaSeance.price && cinemaSeance.price.length > 1 && freePlace != 0)
        {
            if (!nextPriceAnimation.running)
                nextPriceAnimation.start()
        } else
        {
            nextPriceAnimation.stop()
            priceText.anchors.horizontalCenterOffset = 0
        }
    }

    function updateStates()
    {
        //Обновим статусы
        var start = getStartDateTime()
        var end = getEndDateTime()
        var current = new Date()
        if (isTomorrow())
        {
            cinemaSeance.state = "Future"
            return
        } else
        {
            cinemaSeance.state = ""
        }

        if (freePlace === 0)
        {
            cinemaSeance.state = "NoTickets"
        }

        if (current.getTime() > end.getTime())
        {
            cinemaSeance.state = "Past"
            updateTimer.stop()
            return
        }

        if (current.getTime() > start.getTime())
        {
            var elapsedTime = current.getTime() - start.getTime()
            var allTime = end.getTime() - start.getTime()
            cinemaSeance.progress = elapsedTime / allTime
            if (cinemaSeance.progress > 1)
                cinemaSeance.progress = 1
        }
    }

    SequentialAnimation {
        id: nextPriceAnimation
        loops: Animation.Infinite
        ScriptAction {
            script: {
                invisiblePriceText.text = priceText.text
                invisiblePriceText.text = invisiblePriceText.text.substring(1)
                priceText.anchors.horizontalCenterOffset = Math.ceil((priceText.width - invisiblePriceText.width))
                var newPriceText = priceText.text
                newPriceText += newPriceText[0]
                newPriceText = newPriceText.substring(1)
                priceText.text = newPriceText
            }
        }

        PropertyAnimation {
            target: priceText
            property: "anchors.horizontalCenterOffset"
            to: 0
            duration: priceUpdateInterval
            easing.type: Easing.Linear
        }
    }

    Timer {
        id: updateTimer
        interval: 30000
        running: true
        repeat: true
        onTriggered: updateStates()
    }

    Style {
        id: style
    }

    Rectangle {
        id: background
        anchors.fill: content
        color: style.seanceTodayBackgroundColor
        opacity: style.seanceTodayBackgroundOpacity
    }

    Rectangle {
        id: content
        color: "#00000000"
        anchors.fill: parent
        anchors.leftMargin: Math.ceil(cinemaSeance.height * 0.05)
        anchors.topMargin: content.anchors.leftMargin
        anchors.bottomMargin: content.anchors.leftMargin
        Text {
            id: cinema3DText
            text: "3D"
            color: style.seanceToday3DColor
            opacity: style.seanceToday3DOpacity
            anchors.fill: parent
            visible: is3D
            font.bold: style.seanceToday3DTextBold
            font.pixelSize: cinemaSeance.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: timeText
            color: style.seanceTodayTextColor
            text: {
                var start = getStartDateTime()
                return Qt.formatTime(start, "hh:mm")
            }

            wrapMode: Text.WordWrap
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height * 0.5
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: cinemaSeance.height * 0.4
            font.bold: style.seanceTodayTextBold
        }

        Rectangle {
            id: priceCliper
            clip: true
            color: "#00000000"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: timeText.bottom
            anchors.bottom: parent.bottom
            anchors.leftMargin: cinemaSeance.height * 0.1
            anchors.rightMargin: anchors.leftMargin

            Text {
                id: priceText
                color: timeText.color
                anchors.horizontalCenter: parent.horizontalCenter
                verticalAlignment: Qt.AlignVCenter
                anchors.top: parent.top
                anchors.topMargin: parent.height * 0.1
                anchors.bottom: parent.bottom
                font.bold: timeText.font.bold
                font.pixelSize: {
                    if (freePlace === 0)
                    {
                        return Math.ceil(cinemaSeance.height * 0.20)
                    } else
                    {
                        return Math.ceil(cinemaSeance.height * 0.32)
                    }
                }
            }

            Text {
                id: invisiblePriceText
                visible: false
                text: ""
                font.bold: priceText.font.bold
                font.family: priceText.font.family
                font.pixelSize: priceText.font.pixelSize
            }
        }



        Rectangle {
            id: progressRectangle
            color: style.seanceProgressBackgroundColor
            opacity: style.seanceProgressBackgroundOpacity
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            width: content.width * cinemaSeance.progress
        }
    }

    states: [
        State {
            name: "NoTickets"

            PropertyChanges {
                target: background
                color: style.seanceNoTicketsBackgroundColor
                opacity: style.seanceNoTicketsBackgroundOpacity
            }

            PropertyChanges {
                target: timeText
                color: style.seanceNoTicketsTextColor
                font.bold: style.seanceNoTicketsTextBold
            }

            PropertyChanges {
                target: cinema3DText
                color: style.seanceNoTickets3DColor
                font.bold: style.seanceNoTickets3DTextBold
                opacity: style.seanceNoTickets3DOpacity
            }

        },
        State {
            name: "Past"

            PropertyChanges {
                target: progressRectangle
                width: content.width
            }
        },
        State {
            name: "Future"

            PropertyChanges {
                target: background
                color: style.seanceTomorrowBackgroundColor
                opacity: style.seanceTomorrowBackgroundOpacity
            }

            PropertyChanges {
                target: timeText
                color: style.seanceTomorrowTextColor
                font.bold: style.seanceTomorrowTextBold
            }

            PropertyChanges {
                target: cinema3DText
                color: style.seanceTomorrow3DColor
                font.bold: style.seanceTomorrow3DTextBold
                opacity: style.seanceTomorrow3DOpacity
            }


        }
    ]

}
