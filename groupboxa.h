#ifndef GROUPBOXA_H
#define GROUPBOXA_H

#include <QGroupBox>

namespace Ui {
class GroupBoxa;
}

class GroupBoxa : public QGroupBox
{
    Q_OBJECT
    
public:
    explicit GroupBoxa(QWidget *parent = 0);
    ~GroupBoxa();
    
private:
    Ui::GroupBoxa *ui;
};

#endif // GROUPBOXA_H
