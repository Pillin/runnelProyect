#ifndef PEUCKERNETWORKCONF_H
#define PEUCKERNETWORKCONF_H

#include <QWidget>

namespace Ui {
    class PeuckerNetworkConf;
}

class PeuckerNetworkConf : public QWidget
{
        Q_OBJECT

    public:
        explicit PeuckerNetworkConf(QWidget *parent = 0);
        ~PeuckerNetworkConf();

    private:
        Ui::PeuckerNetworkConf *ui;
};

#endif // PEUCKERNETWORKCONF_H
