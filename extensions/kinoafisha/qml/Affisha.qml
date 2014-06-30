// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import KinoafishaExtension 1.0

Image {
    id: affisha
    property int hallCount: style.maxRowNumber
    property int maxHallHeigth: 0
    property int pageChangeInterval: style.pageChangeInterval
    property bool showTrailers: style.showTrailers
    property string backgroundVideoSource: style.backgroundVideoSource

    onMaxHallHeigthChanged: {
        updateOnFilmAffishaChildHeight()
    }

    onHeightChanged: {
        console.log("QML height: " + height)
        legends.height = Math.ceil(affisha.height / 18)
        if (showTrailers)
        {
            pages.anchors.bottomMargin = legendsBorder.height
            trailersLoader.height = Math.ceil(height*style.trailerScreenPart)
            trailersLoader.anchors.bottomMargin = legendsBorder.height
        } else
        {
            pages.anchors.bottomMargin = 0
            trailersLoader.height = 0
            trailersLoader.anchors.bottomMargin = 0
        }
        var affishaRepHeight = pages.height
        affishaRepHeight = affisha.height - titleBorder.height - trailersLoader.height - trailersLoader.anchors.bottomMargin
                - pages.anchors.bottomMargin - legendsBorder.height - legends.height
                - affisha.height / 15 - hallCount * 1
        maxHallHeigth = Math.floor(affishaRepHeight / hallCount)
        title.height = Math.ceil(affisha.height - maxHallHeigth * hallCount - hallCount * 1 - titleBorder.height
                                 - pages.anchors.bottomMargin - trailersLoader.height - trailersLoader.anchors.bottomMargin
                                 - legendsBorder.height - legends.height)
        //Вызовем метод, который пересортировывает фильмы
        onHallCountChanged()
    }

    onWidthChanged: {
        console.log("QML width: " + width)
        updateOnFilmAffishaChildWidth()
    }

    Component.onCompleted: {
        onHallCountChanged()
        onShowTrailersChanged()
    }

    signal trailerPlayFinished(int id)

    source: style.backgroundImageSource

    Style {
        id: style
    }

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

    Loader {
        id: backgrounVideoLoader
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        visible: false
    }

    Rectangle {
        id: title
        color: "#00000000"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        Rectangle {
            anchors.fill: parent
            color: style.affishaTitleBackgroundColor
            opacity: style.affishaTitleBackgroundOpacity
        }

        Text {
            id: dateText
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: height * 0.6
            text: "01.01.2001 Thu"
            font.bold: style.affishaTitleTextBold
            font.pixelSize: height * 0.45
            color: style.affishaTitleTextColor
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
        height: style.affishaTitleBorderWidth
        color: style.affishaTitleBorderColor
    }

    Rectangle {
        id: pages
//        clip: true
        color: "#00000000"
        anchors.top: titleBorder.bottom
        anchors.bottom: trailersLoader.top
        anchors.right: parent.right
        anchors.left: parent.left
        onHeightChanged: {updateOnFilmAffishaChildHeight()}
    }

    Loader {
        id: trailersLoader
        anchors.bottom: legendsBorder.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Connections {
        target: trailersLoader.item
        onFinished: {
            affisha.trailerPlayFinished(id)
        }
    }

    Rectangle {
        id: legendsBorder
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: legends.top
        height: style.affishaLegendBorderWidth
        color: style.affishaLegendBorderColor
    }
    Rectangle {
        id: legendBackground
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: legends.bottom
        anchors.top: legends.top
        color: style.affishaLegendBackgroundColor
        opacity: style.affishaLegendBackgroundOpacity
    }

    Row {
        id: legends
        anchors.bottom: parent.bottom
        anchors.right: parent.right
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
                color: style.affishaLegendTextColor
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Qt.AlignVCenter
                font.bold: style.affishaLegendTextBold
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

    SequentialAnimation {
        id: pageChangeAnimation
        running: true
        loops: Animation.Infinite
        ScriptAction {
            script: {
                if (pages.children.length === 0)
                    return;
                //Найдем текущую видимую страничку
                var visiblePage = -1
                for (var i = 0; i < pages.children.length; i++)
                {
                    if (pages.children[i].opacity === 1)
                    {
                        visiblePage = i
                        break
                    }
                }
                //Скроем текущую видимую страничку
                if (visiblePage !== -1)
                    pages.children[visiblePage].opacity = 0
                //Отобразим следующую страничку
                if (visiblePage < pages.children.length - 1)
                {
                    pages.children[visiblePage + 1].opacity = 1
                } else
                {
                    pages.children[0].opacity = 1
                }
            }
        }
        PauseAnimation { duration: pageChangeInterval }
    }

    function addCinemaSeance(cinemaObj)
    {
        var cinemaFinded = false
        for (var i = 0; i < pages.children.length; i++)
        {
            for (var j = 0; j < pages.children[i].children.length; j++)
            {
                if (pages.children[i].children[j].name === cinemaObj.name)
                {
                    pages.children[i].children[j].addCinemaSeance(cinemaObj)
                    cinemaFinded = true
                }
            }
        }
        if (!cinemaFinded)
        {
            createCinema(cinemaObj.name)
            addCinemaSeance(cinemaObj)
        }
        onHallCountChanged()
    }

    function setCinemaFreePlace(cinemaId, freePlace)
    {
        for (var i = 0; i < pages.children.length; i++)
        {
            for (var j = 0; j < pages.children[i].children.length; j++)
            {
                pages.children[i].children[j].setCinemaFreePlace(cinemaId, freePlace)
            }
        }
    }

    function createNewPage()
    {
        var component = Qt.createComponent("Page.qml")
        var newPage = component.createObject(pages)
        if (newPage === null)
        {
           console.log("Error creating object")
        } else
        {
            newPage.anchors.left = pages.left
            newPage.anchors.right = pages.right
        }
        if (pages.children.length === 1)
            pageChangeAnimation.restart()
        return newPage
    }

    function createCinema(name)
    {
        //Найдем страничку, в которой есть свободное место
        var freePage = undefined
        for (var i = 0; i < pages.children.length; i++)
        {
            var pageHallCount = 0
            for (var j = 0; j < pages.children[i].children.length; j++)
            {
                pageHallCount += pages.children[i].children[j].hallCount
            }

            if (pageHallCount < hallCount)
            {
                freePage = pages.children[i]
                break
            }
        }
        //Если все странички заняты, то создадим новую
        if (!freePage)
            freePage = createNewPage()

        var component = Qt.createComponent("Cinema.qml")
        var cinema = component.createObject(freePage, {"name": name})
        if (cinema === null)
        {
            console.log("Error creating object")
        } else
        {
            cinema.maxHallHeigth = maxHallHeigth
            cinema.hallCountChanged.connect(onHallCountChanged())
        }
        updateOnFilmAffishaChildHeight()
    }

    function updateOnFilmAffishaChildHeight()
    {
        for (var i = 0; i < pages.children.length; i++)
        {
            for (var j = 0; j < pages.children[i].children.length; j++)
            {
                pages.children[i].children[j].maxHallHeigth = maxHallHeigth
            }
        }
    }

    function updateOnFilmAffishaChildWidth()
    {
        for (var i = 0; i < pages.children.length; i++)
        {
            for (var j = 0; j < pages.children[i].children.length; j++)
            {
                pages.children[i].children[j].width = width
            }
        }
    }

    function onHallCountChanged()
    {         
        //Функция устанавливающся для фильма страницу, куда он поместится
        function setCinemaToFreePage(cinema)
        {
            var freePageFound = false
            for (var i = 0; i < pages.children.length; i++)
            {
                var pageHallCount = 0
                for (var j = 0; j < pages.children[i].children.length; j++)
                {
                    pageHallCount += pages.children[i].children[j].hallCount
                }

                if (pageHallCount + cinema.hallCount <= hallCount)
                {
                    //Если данная страница подходит для размещения данного сеанса, то добавим его туда
                    cinema.parent = pages.children[i]
                    freePageFound = true
                    break
                }
            }


            //Если не нашлось не одной страницы, куда влез бы фильм, то создадим новую
            if (!freePageFound)
            {
                var newPage = createNewPage()
                cinema.parent = newPage
            }
        }

        //Проверяем, чтоб на каждой странички все помещались
        for (var i = 0; i < pages.children.length; i++)
        {
            var pageHallCount = 0
            for (var j = 0; j < pages.children[i].children.length; j++)
            {
                pageHallCount += pages.children[i].children[j].hallCount
            }

            if (pageHallCount > hallCount)
            {
                //Если на страничке холов больше, чем максимальное кол-во,
                //то перенесем последний фильм в страничку, где хватит для него места
                var lastFilm = pages.children[i].children[pages.children[i].children.length - 1]
                setCinemaToFreePage(lastFilm)
            }
        }

        //На всякий случай обновим у всех ширины
        updateOnFilmAffishaChildWidth()
    }

    function onShowTrailersChanged()
    {
        if (showTrailers)
        {
            trailersLoader.source = "Trailers.qml"
        } else
        {
            trailersLoader.source = ""
        }
    }


    function onBackgroundVideoSourceChanged()
    {
        if (backgroundVideoSource === "")
        {
            backgrounVideoLoader.source = ""
            backgrounVideoLoader.visible = false
        } else
        {
            backgrounVideoLoader.source = "../../../qml/VideoPlayer.qml"
            backgrounVideoLoader.item.autoPlay = true
            backgrounVideoLoader.item.souce = backgroundVideoSource
            backgrounVideoLoader.visible = true
        }
    }

    //Воспроизводит трейлер
    function playTrayled(mediaObject)
    {
        if (trailersLoader.status === Loader.Ready)
            trailersLoader.item.playMedia(mediaObject)
    }

    //Возвращает ид всех трейлеров, которые сейчас воспроизводятся
    function playedTraylerIdList()
    {
        if (trailersLoader.status === Loader.Ready)
        {
            return trailersLoader.item.playedMediaIdList()
        }
        return []
    }
}
