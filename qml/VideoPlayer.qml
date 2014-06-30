// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import KinoafishaExtension 1.0

Rectangle {
    id: videoPlayer

    property string title: ""
    property url source: ""
    property int startTime: 0
    property int endTime: 0
    property real brightness: 0
    property real contrast: 0
    property real hue: 0
    property real saturation: 0
    property int mediaId: -1
    property color titleBackground: "black"
    property color titleForeground: "#fcffff"
    property real titleOpacity: 0.8

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

    function playMedia(media)
    {
        if (source === media.filePatch)
            return
        mediaId = media.id
        title = media.title
        startTime = media.startPosition
        endTime = media.endPosition
        brightness = media.birghtness
        contrast = media.contrast
        hue = media.hue
        saturation = media.saturation
        source = media.filePatch
        seak(0)
        play()
    }

    color: "black"

    Rectangle {
        color: "black"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        onWidthChanged: {
            if (width >= 0)
                video.width = Math.ceil(width)     //Меняем так, чтоб не вызывалось ворнингов
        }
        onHeightChanged: {
            if (height >= 0)
                video.height = Math.ceil(height)  //Меняем так, чтоб не вызывалось ворнингов
        }
        QMLVideo {
            id: video
            x: 0
            y: 0
            mute: true
            source: videoPlayer.source
            startTime: videoPlayer.startTime
            endTime: videoPlayer.endTime
            brightness: videoPlayer.brightness
            contrast: videoPlayer.contrast
            hue: videoPlayer.hue
            saturation: videoPlayer.saturation
            onFinished: {
                videoPlayer.finished(mediaId)
            }
        }
    }

    Rectangle {
        id: titleBackgroundRectangle
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: Math.floor(parent.height * 0.1)
        color: titleBackground
        opacity: titleOpacity
        visible: title !== ""
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
        font.bold: true
        font.pixelSize: titleText.height * 0.4
        wrapMode: Text.WordWrap
    }
}
