#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QTabWidget>

class MyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MyTabWidget(QWidget *parent = 0);
    
    QTabBar *tabBar() const;
    
};

#endif // MYTABWIDGET_H
