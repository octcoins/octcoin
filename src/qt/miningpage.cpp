// Copyright present Ashata Nakaawa
// Copyright present The Octcoin developers

#include <qt/miningpage.h>
#include <qt/walletmodel.h>
#include <qt/clientmodel.h>
#include <node/interface_ui.h>
#include <rpc/request.h>
#include <rpc/server.h>
#include <univalue.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QTimer>
#include <QThread>
#include <QMessageBox>

MiningPage::MiningPage(QWidget* parent) : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(16);
    layout->setContentsMargins(24, 24, 24, 24);

    // Title
    auto* title = new QLabel(tr("<b>CPU Mining</b>"));
    title->setStyleSheet("font-size: 16px;");
    layout->addWidget(title);

    // Address
    auto* rowAddress = new QHBoxLayout();
    rowAddress->addWidget(new QLabel(tr("Mining address:")));
    editAddress = new QLineEdit();
    editAddress->setPlaceholderText(tr("Enter your Octcoin receiving address"));
    rowAddress->addWidget(editAddress);
    layout->addLayout(rowAddress);

    // Threads
    auto* rowThreads = new QHBoxLayout();
    rowThreads->addWidget(new QLabel(tr("Threads:")));
    spinThreads = new QSpinBox();
    spinThreads->setMinimum(1);
    spinThreads->setMaximum(QThread::idealThreadCount());
    spinThreads->setValue(QThread::idealThreadCount() / 2);
    rowThreads->addWidget(spinThreads);
    rowThreads->addStretch();
    layout->addLayout(rowThreads);

    // Status
    lblStatus   = new QLabel(tr("Status: Idle"));
    lblHashrate = new QLabel(tr("Hashrate: —"));
    layout->addWidget(lblStatus);
    layout->addWidget(lblHashrate);

    // Button
    btnToggle = new QPushButton(tr("Start Mining"));
    btnToggle->setFixedHeight(40);
    btnToggle->setStyleSheet(
        "QPushButton { background-color: #2ecc71; color: white; border-radius: 6px; font-size: 14px; }"
        "QPushButton:hover { background-color: #27ae60; }");
    layout->addWidget(btnToggle);
    layout->addStretch();

    // Timer for status updates
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MiningPage::updateStatus);

    connect(btnToggle, &QPushButton::clicked, this, &MiningPage::toggleMining);
}

void MiningPage::setWalletModel(WalletModel* walletModel) { m_walletModel = walletModel; }
void MiningPage::setClientModel(ClientModel* clientModel) { m_clientModel = clientModel; }

void MiningPage::toggleMining()
{
    m_mining ? stopMining() : startMining();
}

void MiningPage::startMining()
{
    if (editAddress->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Mining"), tr("Please enter a valid mining address."));
        return;
    }

    try {
        JSONRPCRequest req;
        req.strMethod = "generatecontinuoustoaddress";
        req.params    = UniValue(UniValue::VARR);
        req.params.push_back(editAddress->text().trimmed().toStdString());
        req.params.push_back(spinThreads->value());
        tableRPC.execute(req);

        m_mining = true;
        btnToggle->setText(tr("Stop Mining"));
        btnToggle->setStyleSheet(
            "QPushButton { background-color: #e74c3c; color: white; border-radius: 6px; font-size: 14px; }"
            "QPushButton:hover { background-color: #c0392b; }");
        lblStatus->setText(tr("Status: Mining…"));
        timer->start(3000);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Mining Error"), QString::fromStdString(e.what()));
    }
}

void MiningPage::stopMining()
{
    try {
        JSONRPCRequest req;
        req.strMethod = "setgenerate";
        req.params    = UniValue(UniValue::VARR);
        req.params.push_back(false);
        tableRPC.execute(req);
    } catch (...) {}

    m_mining = false;
    timer->stop();
    btnToggle->setText(tr("Start Mining"));
    btnToggle->setStyleSheet(
        "QPushButton { background-color: #2ecc71; color: white; border-radius: 6px; font-size: 14px; }"
        "QPushButton:hover { background-color: #27ae60; }");
    lblStatus->setText(tr("Status: Idle"));
    lblHashrate->setText(tr("Hashrate: —"));
}

void MiningPage::updateStatus()
{
    if (!m_clientModel) return;
    // Hashrate via RPC getmininginfo
    try {
        JSONRPCRequest req;
        req.strMethod = "getmininginfo";
        req.params    = UniValue(UniValue::VARR);
        UniValue result = tableRPC.execute(req);
        double hashrate = result["networkhashps"].get_real();
        lblHashrate->setText(QString("Hashrate: %1 H/s").arg(hashrate, 0, 'f', 0));
    } catch (...) {}
}