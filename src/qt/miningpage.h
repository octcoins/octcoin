// Copyright present Ashata Nakaawa
// Copyright present The Octcoin developers

#ifndef OCTCOIN_QT_MININGPAGE_H
#define OCTCOIN_QT_MININGPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLineEdit;
class QSpinBox;
class QTimer;
QT_END_NAMESPACE

class WalletModel;
class ClientModel;

class MiningPage : public QWidget
{
    Q_OBJECT

public:
    explicit MiningPage(QWidget* parent = nullptr);
    void setWalletModel(WalletModel* walletModel);
    void setClientModel(ClientModel* clientModel);

private Q_SLOTS:
    void toggleMining();
    void updateStatus();

private:
    QPushButton* btnToggle;
    QLineEdit*   editAddress;
    QSpinBox*    spinThreads;
    QLabel*      lblStatus;
    QLabel*      lblHashrate;
    QTimer*      timer;
    bool         m_mining{false};
    WalletModel* m_walletModel{nullptr};
    ClientModel* m_clientModel{nullptr};

    void startMining();
    void stopMining();
};

#endif // OCTCOIN_QT_MININGPAGE_H