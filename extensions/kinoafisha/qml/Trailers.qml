// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: trailers

    property int playerCount: style.trainlersCount

    Style {
        id: style
    }

    //Законченно воспроизведенеи медиа с данным ид
    signal finished(int id)

    color: "#00000000"

    Row {
        id: row
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: height * style.trailersSpacingFactor
        Repeater {
            id: playerRepeater
            model: playerCount
            TrailerVideoPlayer{
                height: row.height
                width: Math.floor(height * 0.8 * 16 / 9)
                source: ""
                onFinished: {
                    trailers.finished(id)
                }
            }
        }
    }

    //Находит первый незанятый проигрыватель и воспроизводит в нем видео
    function playMedia(mediaSource)
    {
        for (var i = 0; i < playerRepeater.count; i++)
        {
            if (!playerRepeater.itemAt(i).playing())
            {
                playerRepeater.itemAt(i).mediaId = mediaSource.id
                playerRepeater.itemAt(i).title = mediaSource.title
                playerRepeater.itemAt(i).startTime = mediaSource.startPosition
                playerRepeater.itemAt(i).endTime = mediaSource.endPosition
                playerRepeater.itemAt(i).brightness = mediaSource.birghtness
                playerRepeater.itemAt(i).contrast = mediaSource.contrast
                playerRepeater.itemAt(i).hue = mediaSource.hue
                playerRepeater.itemAt(i).saturation = mediaSource.saturation
                playerRepeater.itemAt(i).source = mediaSource.filePatch
                playerRepeater.itemAt(i).seak(0)
                playerRepeater.itemAt(i).play()
                break
            }
        }
    }

    //Возвращает скисок ид медиа, которое сейчас проигрывается
    function playedMediaIdList()
    {
        var result = []
        for (var i = 0; i < playerRepeater.count; i++)
        {
            if (playerRepeater.itemAt(i).playing())
            {
                result.push(playerRepeater.itemAt(i).mediaId)
            }
        }
        return result
    }
}
