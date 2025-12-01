#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("AndroidSDK Manager");
    resize(800, 600);

    // 创建布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    getlistButton = new QPushButton("Get List", this);
    layout->addWidget(getlistButton);
    platformsButton = new QPushButton("platformsTools", this);
    layout->addWidget(platformsButton);
    buildtoolsButton = new QPushButton("buildTools", this);
    layout->addWidget(buildtoolsButton);

    packageTable = new QTableWidget(this);
    packageTable->setColumnCount(3);
    packageTable->setHorizontalHeaderLabels({"Package", "Version", "Description"});
    layout->addWidget(packageTable);


    platformsComboBox = new QComboBox(this);
    layout->addWidget(platformsComboBox);

    downloadButton = new QPushButton("下载选中的 platform", this);
    layout->addWidget(downloadButton);

    // 设置中央部件
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(getlistButton, &QPushButton::clicked, this, &MainWindow::getList);
    connect(buildtoolsButton, &QPushButton::clicked, this, &MainWindow::showBuildtools);
    connect(platformsButton, &QPushButton::clicked, this, &MainWindow::showPlatforms);
    connect(downloadButton, &QPushButton::clicked, this, &MainWindow::downloadPlatform);
}

MainWindow::~MainWindow() {}
void MainWindow::getList() {
    QProcess process;
    // 获取环境变量的值
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    // 提取 HOME 变量
    QString sdkManager = env.value("HOME") + "/Android/Sdk/cmdline-tools/latest/bin/sdkmanager";
    // 启动进程
    process.start(sdkManager, QStringList() << "--list");
    // 检查进程启动状态
    if (!process.waitForFinished()) {
        QMessageBox::critical(this, "错误", "无法执行 sdkmanager 命令");
        return;
    }
    QByteArray output = process.readAllStandardOutput();
    // 将输出转换为 QString
    list = QString::fromLocal8Bit(output);
}

void MainWindow::showPlatforms()
{
    // 清空表格和下拉框
    packageTable->setRowCount(0);
    platformsComboBox->clear();

    // 正则表达式匹配三列数据
    QRegularExpression re(R"(^\s*([^|]+)\s*\|\s*(\d+)\s*\|\s*(.+)\s*$)");

    QStringList lines = list.split("\n");
    QStringList filteredLines;
    for (const QString &line : lines) {
        if (line.contains("platforms")) {
            filteredLines.append(line);
        }
    }

    // 遍历过滤后的行
    for (const QString &line : filteredLines) {
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
            // 提取字段
            QString package = match.captured(1).trimmed();
            QString version = match.captured(2).trimmed();
            QString description = match.captured(3).trimmed();

            // 插入表格行
            int row = packageTable->rowCount();
            packageTable->insertRow(row);

            // 填充数据到表格
            packageTable->setItem(row, 0, new QTableWidgetItem(package));
            packageTable->setItem(row, 1, new QTableWidgetItem(version));
            packageTable->setItem(row, 2, new QTableWidgetItem(description));

            // 添加包名到 ComboBox（去重）
            if (platformsComboBox->findText(package) == -1) {
                platformsComboBox->addItem(package);
            }
        }
    }

    // 自动调整列宽
    packageTable->resizeColumnsToContents();
}

void MainWindow::showBuildtools()
{
    packageTable->setRowCount(0);
    platformsComboBox->clear();

    QStringList lines = list.split("\n");
    QStringList filteredLines;
    for (const QString &line : lines) {
        if (line.contains("build-tools")) {
            filteredLines.append(line);
        }
    }

    // 适配 build-tools 的版本格式（例如 30.0.3）
    QRegularExpression re(R"(^\s*([^|]+)\s*\|\s*([\d.]+)\s*\|\s*(.+)\s*$)");

    for (const QString &line : filteredLines) {
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
            QString package = match.captured(1).trimmed();
            QString version = match.captured(2).trimmed();
            QString description = match.captured(3).trimmed();

            int row = packageTable->rowCount();
            packageTable->insertRow(row);
            packageTable->setItem(row, 0, new QTableWidgetItem(package));
            packageTable->setItem(row, 1, new QTableWidgetItem(version));
            packageTable->setItem(row, 2, new QTableWidgetItem(description));

            if (platformsComboBox->findText(package) == -1) {
                platformsComboBox->addItem(package);
            }
        }
    }

    packageTable->resizeColumnsToContents();
}
void MainWindow::downloadPlatform()
{
    QString selectedPlatform = platformsComboBox->currentText();
    if (selectedPlatform.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个 platform");
        return;
    }
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString sdkManager = env.value("HOME") + "/Android/Sdk/cmdline-tools/latest/bin/sdkmanager";
    process.start(sdkManager, QStringList() << selectedPlatform);
    if (!process.waitForFinished()) {
        QMessageBox::critical(this, "错误", "无法执行 sdkmanager 命令");
        return;
    }
    // 读取命令输出
    QByteArray output = process.readAllStandardOutput();
    QString result = QString::fromLocal8Bit(output);
    // 显示下载结果
    QMessageBox::information(this, "下载完成", result);
}
