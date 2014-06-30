// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import KinoafishaExtension 1.0

Rectangle {
    id: trailerVideoPlayer

    property string title: "Title"
    property url source: ""
    property int startTime: 0
    property int endTime: 0
    property real brightness: 0
    property real contrast: 0
    property real hue: 0
    property real saturation: 0
    property int mediaId: -1
    property color titleBackground: style.trailerFrameColor
    property color titleForeground: style.trailerNameTextColor
    property real titleOpacity: style.trailerFrameOpacity

    Style {
        id: style
    }

    signal finished (int id)

    function playing()
    {
        return video.isPlaying()
    }

    function play()
    {
        video.play()
    }

    function stop()
    {
        video.stop()
    }

    function seak(time)
    {
        video.seak(time)
    }

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            if (!playing())
                finished(-1)
        }
    }

    color: "#00000000"

    Rectangle {
        id: backgroundRect
        color: titleBackground
        opacity: titleOpacity
        anchors.fill: parent
    }

    Rectangle {
        color: "black"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: titleBackgroundRectangle.top
        anchors.topMargin: Math.floor(parent.height * 0.025)
        anchors.leftMargin: anchors.topMargin
        anchors.rightMargin: anchors.topMargin
        onWidthChanged: {
            if (width >= 0)
                video.width = Math.floor(width)     //Меняем так, чтоб не вызывалось ворнингов
        }
        onHeightChanged: {
            if (height >= 0)
                video.height = Math.floor(height)  //Меняем так, чтоб не вызывалось ворнингов
        }
        QMLVideo {
            id: video
            x: 0
            y: 0
            autoPlay: true
            mute: true
            source: trailerVideoPlayer.source
            startTime: trailerVideoPlayer.startTime
            endTime: trailerVideoPlayer.endTime
            brightness: trailerVideoPlayer.brightness
            contrast: trailerVideoPlayer.contrast
            hue: trailerVideoPlayer.hue
            saturation: trailerVideoPlayer.saturation
            onFinished: {
                trailerVideoPlayer.finished(mediaId)
            }
        }
    }

    Rectangle {
        id: titleBackgroundRectangle
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: Math.floor(parent.height * 0.2)
        color: "#00000000"
    }

    Text {
        id: titleText
        text: title
        color: titleForeground
        anchors.top: titleBackgroundRectangle.top
        anchors.bottom: titleBackgroundRectangle.bottom
        anchors.left: titleBackgroundRectangle.left
        anchors.right: titleBackgroundRectangle.right
        anchors.rightMargin: titleText.height
        anchors.leftMargin: titleText.height
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        font.bold: style.trailerNameTextBold
        font.pixelSize: titleText.height * 0.4
        wrapMode: Text.WordWrap
    }
}
