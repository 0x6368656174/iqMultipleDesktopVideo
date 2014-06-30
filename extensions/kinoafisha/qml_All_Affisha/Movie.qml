// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: movie
    property string name: ""
    property variant price: []
    property int freePlace: -1
    property string startDateTime: "01-01-2000 00:00:00"
    property string duration: "00:00:00"
    property string curentDateTime: "01-01-2000 00:00:00"
    property bool is3D: false
    property int minAge: 0

    function getWidth()
    {
        var start = getStartDateTime()
        var end = getEndDateTime()
        return (end.getTime() - start.getTime()) / 3600000 * movie.height * 1.2
    }

    function getProgress()
    {
        var start = getStartDateTime()
        var end = getEndDateTime()
        var progress = getProgressDateTime()
        if (progress.getTime() > end.getTime())
        {
            return width
        }
        return (progress.getTime() - start.getTime()) / 3600000 * movie.height * 1.2
    }

    function getStartDateTime()
    {
        var start = new Date()
        var startDateTimeStr = startDateTime
        startDateTimeStr = startDateTimeStr.replace(/(\d+)-(\d+)-(\d+)/, '$2/$1/$3')
        start.setTime(Date.parse(startDateTimeStr))
        return start
    }

    function getEndDateTime()
    {
        var currentDateTime = new Date()
        var currentDate = new Date()
        currentDate.setTime(Date.parse(currentDateTime.toDateString() + " 00:00:00"))
        var start = getStartDateTime()
        var end = new Date()
        end.setTime(Date.parse(currentDate.toDateString() + " " + duration))
        end.setTime(start.getTime() + end.getTime() - currentDate.getTime())
        return end
    }

    function getProgressDateTime()
    {
        var progress = new Date()
        var progressDateTimeStr = curentDateTime
        progressDateTimeStr = progressDateTimeStr.replace(/(\d+)-(\d+)-(\d+)/, '$2/$1/$3')
        progress.setTime(Date.parse(progressDateTimeStr))
        return progress
    }

    function getBorgedWidth()
    {
        return movie.height / 35
    }

    function updateProgress()
    {
        passedBar.width = getProgress()
    }

    onCurentDateTimeChanged: updateProgress()

    width: getWidth()

    height: 122
    color: "#cfcfcf"

    Rectangle {
        id: filmContent
        color: "#00000000"
        border.width: getBorgedWidth()
        border.color: "#1e1e1e"
        anchors.right: parent.right
        anchors.rightMargin: getBorgedWidth() / 2
        anchors.left: parent.left
        anchors.leftMargin: getBorgedWidth() / 2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: getBorgedWidth() / 2
        anchors.top: parent.top
        anchors.topMargin: getBorgedWidth() / 2

        Image {
            id: fon
            anchors.fill: parent
            anchors.margins: getBorgedWidth() / 2
            opacity: 0.400
            fillMode: Image.TileHorizontally
            source: "images/frame.png"
            sourceSize.width: (movie.height - getBorgedWidth()) * 1.2
        }

        Text {
            function getPrice()
            {
                var result = "";
                result = price.join("\n")
                return result
            }

            function getVisible()
            {
                var result = true
                var fullWidth = invisibleFreePlaceFon.width + freePlaceFon.anchors.rightMargin
                fullWidth += priceFon.width + priceFon.anchors.leftMargin
                if (fullWidth > filmContent.width)
                {
                    result = false
                }
                if (price.length === 0)
                {
                    result = false
                }

                return result
            }
            id: priceFon
            visible: getVisible()
            color: "#848484"
            font.pixelSize: filmContent.height / 5
            text: getPrice()
            anchors.top: parent.top
            anchors.topMargin: filmContent.height / 7
            anchors.left: parent.left
            anchors.leftMargin: filmContent.height / 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: filmContent.height / 7
            font.bold: true
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            function getVisible()
            {
                var result = true
                var start = getStartDateTime()
                var end = getEndDateTime()
                end.setTime(Date.parse(currentDate.toDateString() + " " + duration))
                end.setTime(start.getTime() + end.getTime() - currentDate.getTime())
                if (end.getTime() - start.getTime() < 3600000)
                {
                    result = false
                }
                if (freePlace === -1)
                {
                    result = false
                }
                return result
            }

            function getFree()
            {
                var currentDateTime = new Date()
                var currentDate = new Date()
                currentDate.setTime(Date.parse(currentDateTime.toDateString() + " 00:00:00"))
                var start = new Date()
                var startDateTimeStr = startDateTime
                startDateTimeStr = startDateTimeStr.replace(/(\d+)-(\d+)-(\d+)/, '$2/$1/$3')
                start.setTime(Date.parse(startDateTimeStr))
                var end = new Date()
                end.setTime(Date.parse(currentDate.toDateString() + " " + duration))
                end.setTime(start.getTime() + end.getTime() - currentDate.getTime())
                var progress = new Date()
                var progressDateTimeStr = curentDateTime
                progressDateTimeStr = progressDateTimeStr.replace(/(\d+)-(\d+)-(\d+)/, '$2/$1/$3')
                progress.setTime(Date.parse(progressDateTimeStr))
                if (progress.getTime() > end.getTime())
                    return 0
                return freePlace
            }

            function getHorizontalAlignment()
            {
                if (priceFon.visible)
                {
                    return Text.AlignRight
                } else
                {
                    return Text.AlignHCenter
                }
            }

            id: freePlaceFon
            color: priceFon.color
            visible: getVisible()
            font.pixelSize: filmContent.height / 2.5
            text: getFree()
            anchors.top: parent.top
            anchors.topMargin: filmContent.height / 7
            anchors.right: parent.right
            anchors.rightMargin: filmContent.height / 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: filmContent.height / 7
            anchors.left: parent.left
            anchors.leftMargin: filmContent.height / 5
            font.bold: true
            horizontalAlignment: getHorizontalAlignment()
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: invisibleFreePlaceFon
            visible: false
            text: freePlaceFon.text
            font.pixelSize: freePlaceFon.font.pixelSize
            font.bold: freePlaceFon.font.bold
            anchors.top: freePlaceFon.top
            anchors.right: freePlaceFon.right
            anchors.bottom: freePlaceFon.bottom
            verticalAlignment: freePlaceFon.verticalAlignment
        }

        Text {
            id: thriDText
            visible: is3D
            text: "3D"
            anchors.top: parent.top
            anchors.topMargin: filmContent.height / 25
            anchors.left: parent.left
            anchors.leftMargin: filmContent.height / 12
            font.pixelSize: filmContent.height / 7
            font.bold: true
            color: nameText.color
            styleColor: nameText.styleColor
            style: nameText.state

        }

        Text {
            id: ageCategory
            text: minAge + "+"
            anchors.top: parent.top
            anchors.topMargin: filmContent.height / 25
            anchors.right: parent.right
            anchors.rightMargin: filmContent.height / 20
            font.pixelSize: filmContent.height / 7
            font.bold: true
            color: nameText.color
            styleColor: nameText.styleColor
            style: nameText.state
        }

        Text {
            id: nameText
            color: "#0b0b0b"
            text: name
            style: Text.Outline
            styleColor: "#c4c4c4"
            anchors.top: parent.top
            anchors.topMargin: filmContent.height / 6
            horizontalAlignment: Text.AlignHCenter
            anchors.right: parent.right
            anchors.rightMargin: filmContent.height / 5.5
            anchors.left: parent.left
            anchors.leftMargin: filmContent.height / 5.5
            wrapMode: Text.WordWrap
            font.bold: true
            font.pixelSize: filmContent.height / 7
        }

        Text {
            function getPrice()
            {
                var result = "";
                result = price.join(", ")
                return result + qsTr(" rub.")
            }

            function getVisible()
            {
                var result = true
                if (nameText.lineCount > 2)
                    result = false
                if (width >= filmContent.width)
                    result = false
                if (price.length === 0 && freePlace === -1)
                    result = false
                return result
            }

            function getText()
            {
                var result = ""
                if (price.length > 0)
                    result += qsTr("Price: ") + getPrice()
                if (price.length > 0 && freePlace > -1)
                    result += "; "
                if (freePlace > -1)
                    result += qsTr("Free: ") + freePlaceFon.text
                return result
            }

            id: priceFreePlaceText
            color: nameText.color
            text: getText()
            visible: getVisible()
            style: Text.Outline
            styleColor: nameText.styleColor
            anchors.bottom: startTimeText.top
            anchors.bottomMargin: font.pixelSize / 40
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
            font.pixelSize: filmContent.height / 15
        }

        Text {
            id: startTimeText

            function getFilmTime()
            {
                var currentDateTime = new Date()
                var currentDate = new Date()
                currentDate.setTime(Date.parse(currentDateTime.toDateString() + " 00:00:00"))
                var start = new Date()
                var startDateTimeStr = startDateTime
                startDateTimeStr = startDateTimeStr.replace(/(\d+)-(\d+)-(\d+)/, '$2/$1/$3')
                start.setTime(Date.parse(startDateTimeStr))
                var end = new Date()
                end.setTime(Date.parse(currentDate.toDateString() + " " + duration))
                end.setTime(start.getTime() + end.getTime() - currentDate.getTime())
                return Qt.formatTime(start, "hh:mm") + "-" + Qt.formatTime(end, "hh:mm")
            }

            text: getFilmTime()
            color: nameText.color
            anchors.bottom: parent.bottom
            anchors.bottomMargin: filmContent.height / 6
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            styleColor: nameText.styleColor
            style: nameText.style
            font.bold: true
            font.pixelSize: filmContent.height / 8
        }

        Rectangle {
            id: passedBar
            width: getProgress()
            color: "#000000"
            clip: true
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            opacity: 0.850

            Text {
                id: thriDTextPassed
                visible: thriDText.visible
                x: thriDText.x
                y: thriDText.y
                width: thriDText.width
                height: thriDText.height
                color: namePassed.color
                styleColor: namePassed.styleColor
                style: thriDText.style
                font.pixelSize: thriDText.font.pixelSize
                font.bold: thriDText.font.bold
            }

            Text {
                id: ageCategoryPassed
                x: ageCategory.x
                y: ageCategory.y
                width: ageCategory.width
                height: ageCategory.height
                color: namePassed.color
                text: ageCategory.text
                styleColor: namePassed.styleColor
                font.pixelSize: ageCategory.font.pixelSize
                font.bold: ageCategory.font.bold
                style: ageCategory.style
            }

            Text {
                id: namePassed
                y: nameText.y
                x: nameText.x
                width: nameText.width
                height: nameText.height
                color: "#f3f3f3"
                text: nameText.text
                styleColor: "#3c3c3c"
                wrapMode: nameText.wrapMode
                font.pixelSize: nameText.font.pixelSize
                font.bold: nameText.font.bold
                style: nameText.style
                horizontalAlignment: nameText.horizontalAlignment
            }

            Text {
                id: priceFreePlacePassed
                y: priceFreePlaceText.y
                x: priceFreePlaceText.x
                visible: priceFreePlaceText.visible
                width: priceFreePlaceText.width
                height: priceFreePlaceText.height
                color: namePassed.color
                text: priceFreePlaceText.text
                styleColor: namePassed.styleColor
                font.pixelSize: priceFreePlaceText.font.pixelSize
                font.bold: priceFreePlaceText.font.bold
                style: priceFreePlaceText.style
                horizontalAlignment: priceFreePlaceText.horizontalAlignment
            }

            Text {
                id: timePassed
                x: startTimeText.x
                y: startTimeText.y
                width: startTimeText.width
                height: startTimeText.height
                color: namePassed.color
                text: startTimeText.text
                styleColor: namePassed.styleColor
                font.pixelSize: startTimeText.font.pixelSize
                font.bold: startTimeText.font.bold
                style: startTimeText.style
                horizontalAlignment: startTimeText.horizontalAlignment
                wrapMode: startTimeText.wrapMode
            }
    }
        Rectangle {
            function getOpacity()
            {
                if (freePlace === 0)
                {
                    return 0.8
                } else
                {
                    return 0
                }
            }

            id: noFreePlace
            color: "#FFFFFF"
            clip: true
            anchors.left: passedBar.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.right: parent.right
            opacity: getOpacity()
        }
    }
}
