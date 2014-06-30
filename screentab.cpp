#include "screentab.h"
#include "ui_screentab.h"
#include "mdvplayerapplication.h"
#include "saveloadinterface.h"
#include "mytabwidget.h"
#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QInputDialog>
#include <QDebug>

ScreenTab::ScreenTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenTab),
    _currentMedia(NULL),
    _screen(NULL),
    _desktopMediaPlayer(new DesktopMediaPlayer(this))
{
    ui->setupUi(this);
    setScreen(new Screen(this));

//    ui->currentDisplaySpinBox->setMaximum(QApplication::desktop()->screenCount() - 1);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setSizes(QList<int>() << 10000 << 1);
    setContextMenuPolicy(Qt::PreventContextMenu);

    addAction(ui->actionStartStopPlaying);

    ui->mediaListTableView->horizontalHeader()->setResizeMode(MediaTableModel::FilePatch, QHeaderView::Stretch);
    ui->mediaListTableView->horizontalHeader()->setResizeMode(MediaTableModel::NumberToShow, QHeaderView::ResizeToContents);
    ui->mediaListTableView->horizontalHeader()->setResizeMode(MediaTableModel::RandomWeigth, QHeaderView::ResizeToContents);
    ui->mediaListTableView->addAction(ui->actionAddMedia);
    ui->mediaListTableView->addAction(ui->actionAddExtension);
    ui->mediaListTableView->addAction(ui->actionRemoveRow);
    ui->mediaListTableView->addAction(ui->actionMoveMediaUp);
    ui->mediaListTableView->addAction(ui->actionMoveMediaDown);
    ui->mediaListTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(ui->actionAddMedia, SIGNAL(triggered()), this, SLOT(addMedia()));
    connect(ui->actionAddExtension, SIGNAL(triggered()), this, SLOT(addExtension()));
    connect(ui->actionRemoveRow, SIGNAL(triggered()), this, SLOT(removeMedia()));
    connect(ui->actionMoveMediaUp, SIGNAL(triggered()), this, SLOT(moveMediaUp()));
    connect(ui->actionMoveMediaDown, SIGNAL(triggered()), this, SLOT(moveMediaDown()));
    connect(ui->actionStartStopPlaying, SIGNAL(triggered()), this, SLOT(startStopPlaying()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(load()));

//    connect(ui->currentDisplaySpinBox, SIGNAL(valueChanged(int)), this, SLOT(onCurrentDisplaySpinBoxChanged(int)));

    connect(_desktopMediaPlayer, SIGNAL(graphicSceneChanged(QGraphicsScene*)), this, SLOT(setSceneToPreview(QGraphicsScene*)));
    connect(_desktopMediaPlayer, SIGNAL(escKeyPressed()), this, SLOT(stopPlaying()));

    mediaListSelectedChanded(QItemSelection(), QItemSelection());

    //Законектим изменение данных в энжайне расширений со слотом обновления расширений
    connect(MDVPlayerApplication::instance(), SIGNAL(extensionEngineDataChanged()), this, SLOT(updateExtensions()));

    onCurrentDisplaySpinBoxChanged(1);
//    onCurrentDisplaySpinBoxChanged(ui->currentDisplaySpinBox->value());
}

void ScreenTab::setSceneToPreview(QGraphicsScene *scene)
{
    if (ui->mediaPreview->scene() != scene)
        ui->mediaPreview->setScene(scene);
}

void ScreenTab::setScreen(Screen *screen)
{
    if (_screen == screen)
        return;
    if (_screen && _screen->parent() == this)
        _screen->deleteLater();
    _screen = screen;
    _currentMedia = NULL;
    _desktopMediaPlayer->setScreen(screen);
    emit currentMediaChanged(NULL);
    if (_screen)
    {
        if (!_screen->parent())
            _screen->setParent(this);
        setModel(_screen->getMediaModel());
        ui->randomMediaCheckBox->setChecked(_screen->getRandomEnabled());
        connect(_screen, SIGNAL(randomEnabledChanged(bool)), ui->randomMediaCheckBox, SLOT(setChecked(bool)));
        connect(ui->randomMediaCheckBox, SIGNAL(clicked(bool)), _screen, SLOT(setRandomEnabled(bool)));
//        ui->currentDisplaySpinBox->setValue(_screen->getNumberOfScreen());
//        connect(_screen, SIGNAL(numberOfScreenChanged(int)), ui->currentDisplaySpinBox, SLOT(setValue(int)));
//        connect(ui->currentDisplaySpinBox, SIGNAL(valueChanged(int)), _screen, SLOT(setNumberOfScreen(int)));
        //Установим имя таба
        if (!_screen->getName().isEmpty())
            setTabName(_screen->getName());
        connect(_screen, SIGNAL(nameChanged(QString)), this, SLOT(setTabName(QString)));
        connect(_screen, SIGNAL(dataToSaveChanged()), this, SIGNAL(dataChanged()));
        //Если это не реальный экран, то выключим ненужные кнопки
        if (_screen->getIsRealScreen())
        {
            ui->startPlayingPushButton->setEnabled(true);
            ui->actionStartStopPlaying->setEnabled(true);
//            ui->currentDisplayLabel->setEnabled(true);
//            ui->currentDisplaySpinBox->setEnabled(true);
            ui->addExtensionPushButton->setEnabled(true);
            ui->actionAddExtension->setEnabled(true);
        } else
        {
            ui->startPlayingPushButton->setEnabled(false);
            ui->actionStartStopPlaying->setEnabled(false);
//            ui->currentDisplayLabel->setEnabled(false);
//            ui->currentDisplaySpinBox->setEnabled(false);
            ui->addExtensionPushButton->setEnabled(false);
            ui->actionAddExtension->setEnabled(false);
        }
    }
    emit screenPointerChanged(_screen);
}

void ScreenTab::onCurrentDisplaySpinBoxChanged(const int screenNumber)
{
    QDesktopWidget *pDesktop = QApplication::desktop();
    QRect screenGeometry = pDesktop->availableGeometry(screenNumber);
    qreal screenAspectRatio = screenGeometry.width()/screenGeometry.height();
    ui->mediaPreview->setAspectRatio(screenAspectRatio);
    ui->mediaPreview->setRealSceneSize(screenGeometry);
}

void ScreenTab::setModel(MediaTableModel *mediaModel)
{
    _currentMedia = NULL;
    emit currentMediaChanged(NULL);
    ui->mediaListTableView->setModel(mediaModel);
    if (mediaModel)
    {
        connect(ui->mediaListTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(mediaListSelectedChanded(QItemSelection,QItemSelection)));
    }
}

ScreenTab::~ScreenTab()
{
    qDebug() << "Call ~ScreenTab()";
    delete ui;
}

void ScreenTab::updateExtensions()
{
    ui->actionAddExtension->setEnabled(false);
    ui->addExtensionPushButton->setEnabled(false);
    if (_screen && !_screen->getIsRealScreen())
        return;
    ExtensionsEngine * extEngine = MDVPlayerApplication::getExtensionEngine();
    if (!extEngine)
        return;
    QList<AbstarctExtension *> extensions = extEngine->getExtensions();
    foreach (AbstarctExtension * extension, extensions)
    {
        if (extension->getIsEnabled())
        {
            ui->actionAddExtension->setEnabled(true);
            ui->addExtensionPushButton->setEnabled(true);
            break;
        }
    }
}

void ScreenTab::setTabName(const QString &name)
{
    if (_screen->getName() != name)
    {
        _screen->setName(name);
    }
    emit tabNameChanged(name);
}

void ScreenTab::mediaListSelectedChanded(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    ui->actionAddMedia->setEnabled(false);
    ui->actionAddExtension->setEnabled(false);
    ui->actionMoveMediaDown->setEnabled(false);
    ui->actionMoveMediaUp->setEnabled(false);
    ui->actionRemoveRow->setEnabled(false);
    ui->addMediaPushButton->setEnabled(false);
    ui->addExtensionPushButton->setEnabled(false);
    ui->removeRowPushButton->setEnabled(false);
    ui->moveRowDownPushButton->setEnabled(false);
    ui->moveRowUpPushButton->setEnabled(false);
    _currentMedia = NULL;

    QAbstractItemModel *mediaModel = ui->mediaListTableView->model();
    if (!mediaModel)
        return;
    ui->actionAddMedia->setEnabled(true);
    ui->addMediaPushButton->setEnabled(true);
    updateExtensions();

    QList<QModelIndex> selectedIndex = selected.indexes();
    if (selectedIndex.count() > 0)
    {
        ui->actionRemoveRow->setEnabled(true);
        ui->removeRowPushButton->setEnabled(true);
        QModelIndex index = selectedIndex[0];
        if (index.row() > 0)
        {
            ui->moveRowUpPushButton->setEnabled(true);
            ui->actionMoveMediaUp->setEnabled(true);
        } else
        {
            ui->moveRowUpPushButton->setEnabled(false);
            ui->actionMoveMediaUp->setEnabled(false);
        }
        if (index.row() < mediaModel->rowCount() - 1)
        {
            ui->moveRowDownPushButton->setEnabled(true);
            ui->actionMoveMediaDown->setEnabled(true);
        } else
        {
            ui->moveRowDownPushButton->setEnabled(false);
            ui->actionMoveMediaDown->setEnabled(false);
        }
        Media * currentObject = mediaModel->data(index, MediaTableModel::MediaPnt).value<Media *>();
        _currentMedia = currentObject;
    }
    emit currentMediaChanged(_currentMedia);

}

void ScreenTab::addMedia()
{
    QAbstractItemModel *mediaModel = ui->mediaListTableView->model();
    if (!mediaModel)
        return;
    QStringList newMediaFilePatchList = QFileDialog::getOpenFileNames(this, tr("Select media"), QString());
    if (newMediaFilePatchList.isEmpty())
        return;
    int currentRow = getCurrentMediaTableRow();
    foreach (QString newMediaFilePatch, newMediaFilePatchList)
    {
        if (currentRow == -1)
            currentRow = mediaModel->rowCount() - 1;
        mediaModel->insertRow(currentRow + 1);
        mediaModel->setData(mediaModel->index(currentRow + 1, MediaTableModel::FilePatch), newMediaFilePatch);
    }
    ui->mediaListTableView->selectRow(currentRow + 1);
}

void ScreenTab::addExtension()
{
    QAbstractItemModel *mediaModel = ui->mediaListTableView->model();
    ExtensionsEngine *extEngine = MDVPlayerApplication::getExtensionEngine();
    if (!mediaModel || !extEngine)
        return;
    QList<AbstarctExtension *> extensions = extEngine->getExtensions();
    QStringList extensionNames;
    foreach (AbstarctExtension *extension, extensions)
    {
        if (!extension || !extension->getIsEnabled())
            continue;
        extensionNames << extension->getName();
    }
    bool ok = false;
    QString extentionName = QInputDialog::getItem(this, tr("Select extension"), tr("Select extension"), extensionNames, 0, false, &ok);
    if (extentionName.isEmpty() || !ok)
        return;
    AbstarctExtension *extentionToAdd = NULL;
    foreach (AbstarctExtension *extension, extensions)
    {
        if (!extension)
            continue;
        if (extension->getName() == extentionName)
        {
            extentionToAdd = extension;
            break;
        }
    }

    int currentRow = getCurrentMediaTableRow();
    if (currentRow == -1)
        currentRow = mediaModel->rowCount() - 1;
    mediaModel->insertRow(currentRow + 1);
    mediaModel->setData(mediaModel->index(currentRow + 1, 0), extentionToAdd->getIdentificationName(), MediaTableModel::ExtensionIdentificationName);
    ui->mediaListTableView->selectRow(currentRow + 1);

}

void ScreenTab::removeMedia()
{
    QAbstractItemModel *mediaModel = ui->mediaListTableView->model();
    if (!mediaModel)
        return;
    int currentRow = getCurrentMediaTableRow();
    if (currentRow == -1)
        return;
    mediaModel->removeRow(currentRow);
    ui->mediaListTableView->clearSelection();
}

int ScreenTab::getCurrentMediaTableRow() const
{
    QItemSelection currentSelection = ui->mediaListTableView->selectionModel()->selection();
    QList<QModelIndex> indexes = currentSelection.indexes();
    if (indexes.count() < 1)
        return -1;
    return indexes[0].row();
}

void ScreenTab::moveMediaUp()
{
    MediaTableModel *mediaModel = qobject_cast<MediaTableModel*>(ui->mediaListTableView->model());
    if (!mediaModel)
        return;
    int currentRow = getCurrentMediaTableRow();
    if (currentRow == -1)
        return;
    mediaModel->moveRow(currentRow, currentRow - 1);
    ui->mediaListTableView->selectRow(currentRow - 1);
}

void ScreenTab::moveMediaDown()
{
    MediaTableModel *mediaModel = qobject_cast<MediaTableModel*>(ui->mediaListTableView->model());
    if (!mediaModel)
        return;
    int currentRow = getCurrentMediaTableRow();
    if (currentRow == -1)
        return;
    mediaModel->moveRow(currentRow, currentRow + 1);
    ui->mediaListTableView->selectRow(currentRow + 1);
}

void ScreenTab::startStopPlaying()
{
    qDebug() << "Start or stop playing...";
    if (!_desktopMediaPlayer)
        return;
    if (!_desktopMediaPlayer->isShowed())
    {
        startPlaying();
    } else
    {
        stopPlaying();
        ui->mediaPreview->setScene(NULL);
    }
}

void ScreenTab::startPlaying()
{
    qDebug() << "Start playing...";
    if (!_desktopMediaPlayer)
        return;
    if (!_desktopMediaPlayer->isShowed())
    {
        if (_desktopMediaPlayer->play())
        {
            ui->startPlayingPushButton->setIcon(QIcon(":/icons/control-stop-square.png"));
            ui->startPlayingPushButton->setText(tr("Stop"));
            ui->actionStartStopPlaying->setIcon(QIcon(":/icons/control-stop-square.png"));
            ui->actionStartStopPlaying->setText(tr("Stop"));
        }
    }
}

void ScreenTab::stopPlaying()
{
    qDebug () << "Stop plaing...";
    if (_desktopMediaPlayer)
        _desktopMediaPlayer->stop();
    ui->startPlayingPushButton->setIcon(QIcon(":/icons/control.png"));
    ui->startPlayingPushButton->setText(tr("Start"));
    ui->actionStartStopPlaying->setIcon(QIcon(":/icons/control.png"));
    ui->actionStartStopPlaying->setText(tr("Start"));
}

void ScreenTab::save()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save Media List"), QString(), tr("XML (*.xml)"));
    if (saveFileName.isEmpty())
        return;
    QHash<QString, QVariant> params;
    MediaTableModel *modelToSave = NULL;
    getDataToSave(modelToSave, &params);
    SaveLoadInterface saveLoadInt;
    saveLoadInt.saveScreen(saveFileName, _screen);
}

void ScreenTab::getDataToSave(MediaTableModel *& screenModel, QHash<QString, QVariant> *screenParams)
{
    MediaTableModel *mediaModel = qobject_cast<MediaTableModel*>(ui->mediaListTableView->model());
    if (!mediaModel)
        return;
    screenModel = mediaModel;
    screenParams->insert("name", _screen->getName());
    screenParams->insert("random-enabled", ui->randomMediaCheckBox->isChecked());
//    screenParams->insert("screen-number", ui->currentDisplaySpinBox->value());
}

void ScreenTab::load()
{
    QString loadFilePatch = QFileDialog::getOpenFileName(this, tr("Load Media List"), QString(), tr("XML (*.xml)"));
    if (loadFilePatch.isNull())
        return;
    SaveLoadInterface saveLoadInt;
    setScreen(saveLoadInt.loadScreen(loadFilePatch));
}

void ScreenTab::setData(MediaTableModel *screenModel, const QHash<QString, QVariant> screenParams)
{
    if (!screenModel)
        return;
    setModel(screenModel);
    setTabName(screenParams["name"].toString());
    ui->randomMediaCheckBox->setChecked(screenParams["random-enabled"].toBool());
//    ui->currentDisplaySpinBox->setValue(screenParams["screen-number"].toInt());
}
