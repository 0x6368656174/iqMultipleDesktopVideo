import QtQuick 1.1

QtObject{
    ////////////////////////////////////////
    //        Основные настройки            //
    ////////////////////////////////////////
    //Максимальное колличество строк в расписании
    property int maxRowNumber: 7
    //Интервал, с которым будут обновляться страницы расписания
    property int pageChangeInterval: 5000
    //Признак того, что необходим в расписании отображать трейлеры
    property bool showTrailers: true
    //Часть экрана, которую будут занимать трейлеры
    property real trailerScreenPart: 0.2
    //Колличество одновременно проигрываемых трейлеров
    property int trainlersCount: 5
    //Растояние между трейлерами, как множитель на высоту трейлера
    property real trailersSpacingFactor: 0.2
    //Путь к изображению, изпользуемуму в качестве фона
    property string backgroundImageSource: "images/background.jpg"
    //Путь к видеофайлу, используемому в качестве фона
    property string backgroundVideoSource: ""

    ////////////////////////////////////////
    //        Настройки заголовка афиши      //
    ////////////////////////////////////////
    //Цвет текста заголовка афиши
    property color affishaTitleTextColor: "#fcffff"
    //Признак того, что текст должен быть полужирным
    property bool affishaTitleTextBold: true
    //Цвет фона заголовка афиши
    property color affishaTitleBackgroundColor: "#010203"
    //Прозрачность фона заголовка афиши
    property real affishaTitleBackgroundOpacity: 0.6
    //Цвет рамки заголовка афиши
    property color affishaTitleBorderColor: "#888888"
    //Толщина рамки заголовка афиши
    property int affishaTitleBorderWidth: 4

    ////////////////////////////////////////
    //        Настройки легенды афиши       //
    ////////////////////////////////////////
    //Цвет текста легенды афиши
    property color affishaLegendTextColor: "#f3f3f3"
    //Признак того, что текст должен быть полужирным
    property bool affishaLegendTextBold: true
    //Цвет фона легенды афиши
    property color affishaLegendBackgroundColor: "#00000000"
    //Прозрачность фона легенды афиши
    property real affishaLegendBackgroundOpacity: 1
    //Цвет рамки легедны афиши
    property color affishaLegendBorderColor: affishaTitleBorderColor
    //Толщина рамки легедны афиши
    property int affishaLegendBorderWidth: affishaTitleBorderWidth

    ////////////////////////////////////////
    //       Настройки списка фильмов        //
    ////////////////////////////////////////
    property bool cinemaHallBackgroundUseGradient: false
    //Признак того, что гардиент должен быть горизонтальным
    property bool cinemaHallBackgroundHorizontalGradient: false
    //Цвет фона
    property color cinemaHallBackgroundColor: "#00000000"
    //Прозрачность фона
    property real cinemaHallBackgroundOpacity: 0.6
    //Первый цвет градиента
    property color cinemaHallBackgroundGradientStartColor: "#010203"
    //Второй цвет градиента
    property color cinemaHallBackgroundGradientEndColor: "#00000000"
    //Цвет названия кинозала
    property color cinemaHallNameTextColor: "#a5a5a5"
    //Признак того, что текст должен быть полужирным
    property bool cinemaHallNameTextBold: true
    //Цвет линии, отделяющие разные фильмы
    property color cinemaListCinemaBorderColor: "#888888"
    //Толщина линии, отделяющей разные фильмы
    property int cinemaListCinemaBorderWidth: 2
    //Цвет линии, отделяющей разные залы одного фильма
    property color cinemaListHallBorderColor: cinemaListCinemaBorderColor
    //Толщина линии, отделяющей разные залы одного фильма
    property int cinemaListHallBorderWidth: 1

    ////////////////////////////////////////
    //   Настройки одного сеанса фильма      //
    ////////////////////////////////////////
    //Цвет фона прямоугольника прогресса фильма
    property color seanceProgressBackgroundColor: "black"
    //Прозрачность прямоугольника прогресса фильма
    property real seanceProgressBackgroundOpacity: 0.7
    ////////////////////////////////////////
    //Настройки для состояния "Будет показан сегодня"
    //Цвет фона
    property color seanceTodayBackgroundColor: "#6c8fa2"
    //Прозрачность фона
    property real seanceTodayBackgroundOpacity: 0.3
    //Цвет текста
    property color seanceTodayTextColor: "#fcffff"
    //Признак того, что текст полужирный
    property bool seanceTodayTextBold: true
    //Цвет значка 3D
    property color seanceToday3DColor: "black"
    //Признак того, что текст значка 3D полужирный
    property bool seanceToday3DTextBold: true
    //Прозрачность значка 3D
    property real seanceToday3DOpacity: 0.4
    ////////////////////////////////////////
    //Настройки для состояния "Будет показан завтра"
    //Цвет фона
    property color seanceTomorrowBackgroundColor: seanceTodayBackgroundColor
    //Прозрачность фона
    property real seanceTomorrowBackgroundOpacity: seanceTodayBackgroundOpacity
    //Цвет текста
    property color seanceTomorrowTextColor: "#050809"
    //Признак того, что текст полужирный
    property bool seanceTomorrowTextBold: seanceTodayTextBold
    //Цвет значка 3D
    property color seanceTomorrow3DColor: seanceToday3DColor
    //Признак того, что текст значка 3D полужирный
    property bool seanceTomorrow3DTextBold: seanceToday3DTextBold
    //Прозрачность значка 3D
    property real seanceTomorrow3DOpacity: seanceToday3DOpacity
    ////////////////////////////////////////
    //Настройки для состояния "Нет билетов"
    //Цвет фона
    property color seanceNoTicketsBackgroundColor: "#26404f"
    //Прозрачность фона
    property real seanceNoTicketsBackgroundOpacity: seanceTodayBackgroundOpacity
    //Цвет текста
    property color seanceNoTicketsTextColor: "#a3a3a3"
    //Признак того, что текст полужирный
    property bool seanceNoTicketsTextBold: seanceTodayTextBold
    //Цвет значка 3D
    property color seanceNoTickets3DColor: seanceToday3DColor
    //Признак того, что текст значка 3D полужирный
    property bool seanceNoTickets3DTextBold: seanceToday3DTextBold
    //Прозрачность значка 3D
    property real seanceNoTickets3DOpacity: seanceToday3DOpacity


    ////////////////////////////////////////
    //   Настройки списка названий фильмов    //
    ////////////////////////////////////////
    //Признак того, что надо использовать градиент для фона
    property bool cinemaTitleBackgroundUseGradient: false
    //Признак того, что гардиент должен быть горизонтальным
    property bool cinemaTitleBackgroundHorizontalGradient: false
    //Цвет фона
    property color cinemaTitleBackgroundColor: "#00000000"
    //Прозрачность фона
    property real cinemaTitleBackgroundOpacity: 0.6
    //Первый цвет градиента
    property color cinemaTitleBackgroundGradientStartColor: "#010203"
    //Второй цвет градиента
    property color cinemaTitleBackgroundGradientEndColor: "#00000000"
    //Цвет текста
    property color cinemaTitleTextColor: "#f3f3f3"
    //Признак того, что текст должен быть полужирным
    property bool cinemaTitleTextBold: true
    //Цвет текста значка 3D
    property color cinemaTitleIs3DTextColor: "black"
    //Признак того, что текст должен быть полужирным
    property bool cinemaTitleIs3DTextBold: true
    //Цвет фона значка 3D
    property color cinemaTitleIs3DBackgroundColor: "#4e6b7c"
    //Прозрачность фона значка 3D
    property real cinemaTitleIs3DBackgroundOpacity: 0.5
    //Цвет текста значка ограничения возраста
    property color cinemaTitleAgeLimitTextColor: cinemaTitleTextColor
    //Признак того, что значек ограничения возраста должен быть полужирным
    property bool cinemaTitleAgeLimitTextBold: true
    //Цвет фона значка ограничения возраста
    property color cinemaTitleAgeLimitBackgroundColor: cinemaTitleIs3DBackgroundColor
    //Прозрачность фона значка ограничения возраста
    property real cinemaTitleAgeLimitBackgroundOpacity: 0.5

    ////////////////////////////////////////
    //  Настройки продолжительности фильмов   //
    ////////////////////////////////////////
    //Признак того, что надо использовать градиент для фона
    property bool cinemaDurationBackgroundUseGradient: false
    //Признак того, что гардиент должен быть горизонтальным
    property bool cinemaDurationBackgroundHorizontalGradient: false
    //Цвет фона
    property color cinemaDurationBackgroundColor: "#00000000"
    //Прозрачность фона
    property real cinemaDurationBackgroundOpacity: 0.6
    //Первый цвет градиента
    property color cinemaDurationBackgroundGradientStartColor: "#010203"
    //Второй цвет градиента
    property color cinemaDurationBackgroundGradientEndColor: "#00000000"
    //Цвет текста продолжительности фильма
    property color cinemaDurationTextColor: "#f3f3f3"
    //Признак того, что текст должен быть полужирным
    property bool cinemaDurationTextBold: true
    ////////////////////////////////////////
    //         Настройки трейлеров           //
    ////////////////////////////////////////
    //Цвет фона рамки
    property color trailerFrameColor: "#26404f"
    //Прозрачность фона рамки
    property real trailerFrameOpacity: 0.3
    //Цвет текста названий трейлера
    property color trailerNameTextColor: "#fcffff"
    //Признак того, что текст должен быть полужирным
    property bool trailerNameTextBold: true

}
