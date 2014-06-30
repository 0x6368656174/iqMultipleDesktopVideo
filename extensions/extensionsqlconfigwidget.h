#ifndef EXTENSIONSQLCONFIGWIDGET_H
#define EXTENSIONSQLCONFIGWIDGET_H

#include <QWidget>

namespace Ui {
class ExtensionSQLConfigWidget;
}

class ExtensionSQLConfigWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ExtensionSQLConfigWidget(QWidget *parent = 0);
    ~ExtensionSQLConfigWidget();

    void setDatabaseDriver(const QString & driverName);

    void setDatabaseHostName(const QString & hostName);

    void setDatabasePort(const int & port);

    void setDatabasePatch(const QString & patch);

    void setDatabaseName(const QString & name);

    void setDatabaseUser(const QString & user);

    void setDatabasePassword(const QString & password);

    QString getDatabaseDriver() const;

    QString getDatabaseHostName() const;

    int getDatabasePort() const;

    QString getDatabasePath() const;

    QString getDatabaseName() const;

    QString getDatabaseUser() const;

    QString getDatabasePassword() const;

    QHash<QString, QVariant> getParams() const;

    void setParams(const QHash<QString, QVariant> params);

private slots:
    void databaseDriverIndexChanged(const int index);
    void browseDatabasePatch();
    
private:
    Ui::ExtensionSQLConfigWidget *ui;
};

#endif // EXTENSIONSQLCONFIGWIDGET_H
