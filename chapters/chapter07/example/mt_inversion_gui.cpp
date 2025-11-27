#include "mt_inversion_gui.h"
#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QFont>
#include <QDateTime>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QCategoryAxis>
#include <cmath>
#include <limits>
#include <QDebug>

MTInversionGUI::MTInversionGUI(QWidget *parent)
    : QWidget(parent)
    , m_core(new MTInversionCore())
    , m_workerThread(nullptr)
    , m_plotTimer(new QTimer(this))
    , m_residualChart(nullptr)
    , m_residualChartView(nullptr)
{
    setupUI();

    // 连接绘图定时器
    connect(m_plotTimer, &QTimer::timeout, this, &MTInversionGUI::updatePlot);

    // 设置初始状态
    m_btnStop->setEnabled(false);
    m_statusLabel->setText("就绪");
}

MTInversionGUI::~MTInversionGUI() {
    // 停止定时器
    if (m_plotTimer) {
        m_plotTimer->stop();
    }
    
    // 优雅停止工作线程
    if (m_workerThread && m_workerThread->isRunning()) {
        m_workerThread->requestStop();
        // 等待线程结束，最多等待5秒
        if (!m_workerThread->wait(5000)) {
            // 如果等待超时，强制终止（不推荐，但作为最后手段）
            m_workerThread->terminate();
            m_workerThread->wait();
        }
    }
    
    delete m_core;
}

void MTInversionGUI::setupUI() {
    setWindowTitle("大地电磁（MT）一维反演程序");
    resize(1200, 800);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // 左侧：参数面板
    QWidget* leftPanel = new QWidget();
    leftPanel->setMaximumWidth(300);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    setupParameterPanel();
    leftLayout->addWidget(m_paramGroup);
    leftLayout->addStretch();

    // 右侧：结果面板
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    setupResultPanel();
    setupPlotPanel();

    // 使用QSplitter来分割结果表格和图表
    QSplitter* splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(m_resultTable);
    splitter->addWidget(m_chartContainer);
    
    // 创建日志和残差图表的水平分割器
    QSplitter* logSplitter = new QSplitter(Qt::Horizontal);
    logSplitter->addWidget(m_logText);
    
    // 创建残差图表
    m_residualChart = new QChart();
    m_residualChart->setTitle("残差下降曲线");
    m_residualChartView = new QChartView(m_residualChart);
    m_residualChartView->setRenderHint(QPainter::Antialiasing);
    m_residualChartView->setMinimumWidth(300);   // 设置最小宽度，允许拉伸
    m_residualChartView->setMinimumHeight(100); // 设置最小高度，允许拉伸
    logSplitter->addWidget(m_residualChartView);
    logSplitter->setStretchFactor(0, 1);  // 日志文本占1份
    logSplitter->setStretchFactor(1, 1);  // 残差图表占1份
    logSplitter->setChildrenCollapsible(false);  // 防止子部件被完全折叠
    
    splitter->addWidget(logSplitter);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    splitter->setStretchFactor(2, 1);
    splitter->setChildrenCollapsible(false);  // 防止子部件被完全折叠

    rightLayout->addWidget(splitter);

    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel, 1);
}

void MTInversionGUI::setupParameterPanel() {
    m_paramGroup = new QGroupBox("反演参数");
    QGridLayout* layout = new QGridLayout(m_paramGroup);

    // 模型层数
    layout->addWidget(new QLabel("模型层数:"), 0, 0);
    m_spinMLayers = new QSpinBox();
    m_spinMLayers->setRange(10, 100);
    m_spinMLayers->setValue(40);
    layout->addWidget(m_spinMLayers, 0, 1);

    // 频率点数
    layout->addWidget(new QLabel("频率点数:"), 1, 0);
    m_spinNFreq = new QSpinBox();
    m_spinNFreq->setRange(10, 200);
    m_spinNFreq->setValue(61);
    layout->addWidget(m_spinNFreq, 1, 1);

    // 正则化参数
    layout->addWidget(new QLabel("正则化参数λ:"), 2, 0);
    m_spinLambda = new QDoubleSpinBox();
    m_spinLambda->setRange(0.01, 100.0);
    m_spinLambda->setValue(1.0);
    m_spinLambda->setDecimals(2);
    m_spinLambda->setSingleStep(0.1);
    layout->addWidget(m_spinLambda, 2, 1);

    // Jacobian扰动步长
    layout->addWidget(new QLabel("扰动步长ε:"), 3, 0);
    m_spinEpsilon = new QDoubleSpinBox();
    m_spinEpsilon->setRange(1e-8, 1e-3);
    m_spinEpsilon->setValue(1e-5);
    m_spinEpsilon->setDecimals(8);
    m_spinEpsilon->setSingleStep(1e-6);
    layout->addWidget(m_spinEpsilon, 3, 1);

    // 最大迭代次数
    layout->addWidget(new QLabel("最大迭代次数:"), 4, 0);
    m_spinMaxIter = new QSpinBox();
    m_spinMaxIter->setRange(1, 100);
    m_spinMaxIter->setValue(20);
    layout->addWidget(m_spinMaxIter, 4, 1);

    // 模型更新容差
    layout->addWidget(new QLabel("更新容差:"), 5, 0);
    m_spinTolDm = new QDoubleSpinBox();
    m_spinTolDm->setRange(1e-6, 1e-2);
    m_spinTolDm->setValue(1e-4);
    m_spinTolDm->setDecimals(6);
    m_spinTolDm->setSingleStep(1e-5);
    layout->addWidget(m_spinTolDm, 5, 1);

    // 随机模型参数
    layout->addWidget(new QLabel("最小电阻率(Ω·m):"), 6, 0);
    m_spinMinRho = new QDoubleSpinBox();
    m_spinMinRho->setRange(0.1, 100000.0);
    m_spinMinRho->setValue(0.1);
    m_spinMinRho->setDecimals(1);
    m_spinMinRho->setSingleStep(1.0);
    layout->addWidget(m_spinMinRho, 6, 1);

    layout->addWidget(new QLabel("最大电阻率(Ω·m):"), 7, 0);
    m_spinMaxRho = new QDoubleSpinBox();
    m_spinMaxRho->setRange(0.1, 100000.0);
    m_spinMaxRho->setValue(100000.0);
    m_spinMaxRho->setDecimals(1);
    m_spinMaxRho->setSingleStep(10.0);
    layout->addWidget(m_spinMaxRho, 7, 1);

    // 层厚度参数
    layout->addWidget(new QLabel("第一层厚度(m):"), 8, 0);
    m_spinFirstThickness = new QDoubleSpinBox();
    m_spinFirstThickness->setRange(1.0, 1000.0);
    m_spinFirstThickness->setValue(10.0);
    m_spinFirstThickness->setDecimals(1);
    m_spinFirstThickness->setSingleStep(1.0);
    layout->addWidget(m_spinFirstThickness, 8, 1);

    layout->addWidget(new QLabel("厚度增长系数:"), 9, 0);
    m_spinThicknessGrowth = new QDoubleSpinBox();
    m_spinThicknessGrowth->setRange(1.0, 3.0);
    m_spinThicknessGrowth->setValue(1.2);
    m_spinThicknessGrowth->setDecimals(2);
    m_spinThicknessGrowth->setSingleStep(0.1);
    layout->addWidget(m_spinThicknessGrowth, 9, 1);

    // 按钮
    QHBoxLayout* btnLayout1 = new QHBoxLayout();
    m_btnGenerateRandom = new QPushButton("生成随机模型");
    m_btnStart = new QPushButton("开始反演");
    btnLayout1->addWidget(m_btnGenerateRandom);
    btnLayout1->addWidget(m_btnStart);
    layout->addLayout(btnLayout1, 10, 0, 1, 2);

    QHBoxLayout* btnLayout2 = new QHBoxLayout();
    m_btnStop = new QPushButton("停止");
    btnLayout2->addWidget(m_btnStop);
    layout->addLayout(btnLayout2, 11, 0, 1, 2);

    // 进度条
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    layout->addWidget(m_progressBar, 12, 0, 1, 2);

    // 状态标签
    m_statusLabel = new QLabel("就绪");
    layout->addWidget(m_statusLabel, 13, 0, 1, 2);

    // 连接信号
    connect(m_btnGenerateRandom, &QPushButton::clicked, this, &MTInversionGUI::onGenerateRandomModel);
    connect(m_btnStart, &QPushButton::clicked, this, &MTInversionGUI::onStartInversion);
    connect(m_btnStop, &QPushButton::clicked, this, &MTInversionGUI::onStopInversion);
}

void MTInversionGUI::setupResultPanel() {
    // 结果表格
    m_resultTable = new QTableWidget();
    m_resultTable->setColumnCount(5);
    m_resultTable->setHorizontalHeaderLabels(QStringList() << "层号" << "层厚度(m)" << "真实模型" << "初始模型" << "反演结果");
    m_resultTable->horizontalHeader()->setStretchLastSection(true);
    m_resultTable->setAlternatingRowColors(true);
    m_resultTable->setMaximumHeight(200);

    // 日志文本
    m_logText = new QTextEdit();
    m_logText->setReadOnly(true);
    m_logText->setMinimumHeight(100);  // 设置最小高度，允许拉伸
    m_logText->append("欢迎使用MT一维反演程序！");
}

void MTInversionGUI::setupPlotPanel() {
    // 创建图表容器，使用水平布局让三个图表并列显示
    m_chartContainer = new QWidget();
    QHBoxLayout* chartLayout = new QHBoxLayout(m_chartContainer);
    chartLayout->setContentsMargins(0, 0, 0, 0);

    // 1. 模型对比图
    m_modelChart = new QChart();
    m_modelChart->setTitle("模型对比（电阻率随深度变化）");
    m_modelChartView = new QChartView(m_modelChart);
    m_modelChartView->setRenderHint(QPainter::Antialiasing);
    chartLayout->addWidget(m_modelChartView, 1);  // 占1份空间

    // 2. 视电阻率曲线
    m_resistivityChart = new QChart();
    m_resistivityChart->setTitle("视电阻率曲线");
    m_resistivityChartView = new QChartView(m_resistivityChart);
    m_resistivityChartView->setRenderHint(QPainter::Antialiasing);
    chartLayout->addWidget(m_resistivityChartView, 1);  // 占1份空间

    // 3. 相位曲线
    m_phaseChart = new QChart();
    m_phaseChart->setTitle("相位曲线");
    m_phaseChartView = new QChartView(m_phaseChart);
    m_phaseChartView->setRenderHint(QPainter::Antialiasing);
    chartLayout->addWidget(m_phaseChartView, 1);  // 占1份空间

    // 初始化空图表
    clearCharts();
}

void MTInversionGUI::onStartInversion() {
    // 清理之前的线程（如果存在且已结束）
    // QPointer会自动检查对象是否已被删除
    if (m_workerThread) {
        if (m_workerThread->isRunning()) {
            QMessageBox::warning(this, "警告", "反演正在进行中，请先停止当前任务。");
            return;
        } else {
            // 线程已结束，断开连接并清理
            disconnect(m_workerThread, nullptr, this, nullptr);
            m_workerThread->deleteLater();
            m_workerThread.clear();  // 清空QPointer
        }
    }

    // 设置参数
    MTInversionCore::InversionParams params;
    params.M = m_spinMLayers->value();
    params.nFreq = m_spinNFreq->value();
    params.lambda = m_spinLambda->value();
    params.epsilon = m_spinEpsilon->value();
    params.maxIter = m_spinMaxIter->value();
    params.tolDm = m_spinTolDm->value();
    params.firstLayerThickness = m_spinFirstThickness->value();
    params.thicknessGrowth = m_spinThicknessGrowth->value();

    // 如果已经生成了随机模型，使用其观测数据、真实模型等信息
    if (m_currentResult.success && !m_currentResult.dObs.empty() && 
        !m_currentResult.mTrue.empty()) {
        // 检查参数是否匹配
        if (m_currentResult.dObs.size() != params.nFreq * 2 ||
            m_currentResult.mTrue.size() != params.M) {
            QMessageBox::warning(this, "参数不匹配", 
                QString("已生成的模型参数（M=%1, nFreq=%2）与当前设置（M=%3, nFreq=%4）不匹配。\n"
                        "请重新生成随机模型或调整参数。")
                .arg(m_currentResult.mTrue.size())
                .arg(m_currentResult.dObs.size() / 2)
                .arg(params.M)
                .arg(params.nFreq));
            m_logText->append("参数不匹配，使用默认模型进行反演");
        } else {
            params.dObs = m_currentResult.dObs;
            params.mTrue = m_currentResult.mTrue;
            params.periods = m_currentResult.periods;
            params.omega = m_currentResult.omega;
            params.layerThicknesses = m_currentResult.layerThicknesses;
            params.layerDepths = m_currentResult.layerDepths;
            m_logText->append("使用已生成的随机模型数据进行反演");
        }
    } else {
        m_logText->append("使用默认模型进行反演（建议先点击'生成随机模型'）");
    }

    // 更新UI状态
    m_btnStart->setEnabled(false);
    m_btnStop->setEnabled(true);
    m_progressBar->setValue(0);
    m_progressBar->setMaximum(params.maxIter);
    m_statusLabel->setText("正在反演...");

    // 清空日志
    m_logText->clear();
    m_logText->append(QString("开始反演 - %1").arg(QDateTime::currentDateTime().toString()));
    m_logText->append(QString("参数: M=%1, nFreq=%2, λ=%3, ε=%4, maxIter=%5, tol=%6")
                      .arg(params.M).arg(params.nFreq).arg(params.lambda)
                      .arg(params.epsilon).arg(params.maxIter).arg(params.tolDm));

    // 创建并启动工作线程
    m_workerThread = new InversionWorkerThread(m_core, params, this);
    connect(m_workerThread, &InversionWorkerThread::progressUpdated,
            this, &MTInversionGUI::onProgressUpdated);
    connect(m_workerThread, &InversionWorkerThread::inversionFinished,
            this, &MTInversionGUI::onInversionFinished);
            connect(m_workerThread, &InversionWorkerThread::finished,
            this, [this]() {
                // 线程结束时，清理指针
                if (m_workerThread) {
                    m_workerThread->deleteLater();
                    m_workerThread.clear();  // 清空QPointer
                }
            });

    m_workerThread->start();
    m_plotTimer->start(100);  // 每100ms更新一次图表
}

void MTInversionGUI::onStopInversion() {
    if (m_workerThread && m_workerThread->isRunning()) {
        // 请求优雅停止
        m_workerThread->requestStop();
        // 等待线程结束，最多等待3秒
        if (!m_workerThread->wait(3000)) {
            // 如果等待超时，强制终止
            m_workerThread->terminate();
            m_workerThread->wait();
        }
        // 断开连接并清理线程
        disconnect(m_workerThread, nullptr, this, nullptr);
        m_workerThread->deleteLater();
        m_workerThread.clear();  // 清空QPointer
        m_statusLabel->setText("已停止");
        m_logText->append("反演已停止");
        m_plotTimer->stop();
    }

    m_btnStart->setEnabled(true);
    m_btnStop->setEnabled(false);
}

void MTInversionGUI::onGenerateRandomModel() {
    // 如果反演正在进行，先停止
    if (m_workerThread && m_workerThread->isRunning()) {
        QMessageBox::warning(this, "警告", "反演正在进行中，请先停止当前任务。");
        return;
    }
    
    // 停止定时器（如果正在运行）
    if (m_plotTimer->isActive()) {
        m_plotTimer->stop();
    }
    
    int M = m_spinMLayers->value();
    int nFreq = m_spinNFreq->value();
    double minRho = m_spinMinRho->value();
    double maxRho = m_spinMaxRho->value();
    double firstThickness = m_spinFirstThickness->value();
    double growthFactor = m_spinThicknessGrowth->value();

    m_logText->append(QString("生成随机模型 - %1").arg(QDateTime::currentDateTime().toString()));
    m_logText->append(QString("参数: M=%1, nFreq=%2, 电阻率范围=[%3, %4] Ω·m")
                     .arg(M).arg(nFreq).arg(minRho).arg(maxRho));
    m_logText->append(QString("第一层厚度=%1 m, 厚度增长系数=%2")
                     .arg(firstThickness).arg(growthFactor));

    // 生成随机模型并正演（使用0.3作为滤波截止频率，过滤高频变化）
    std::vector<double> mRandom;
    std::vector<double> layerThicknesses;
    std::vector<double> layerDepths;
    std::vector<double> periods;
    std::vector<double> omega;
    std::vector<double> dataRandom;

    // 使用更大的滤波参数（0.3）使模型更光滑
    m_core->generateRandomModelAndForward(M, nFreq, minRho, maxRho,
                                          firstThickness, growthFactor, 0.2,
                                          mRandom, layerThicknesses, layerDepths,
                                          periods, omega, dataRandom);

    // 更新当前结果（用于显示）
    m_currentResult.mTrue = mRandom;
    // 初始模型始终是100 Ω·m（log10(100) = 2.0）
    m_currentResult.mInit.resize(M);
    for (int i = 0; i < M; i++) {
        m_currentResult.mInit[i] = log10(100.0);  // 100 Ω·m
    }
    m_currentResult.mFinal = mRandom; // 暂时相同（随机模型作为最终结果用于显示）
    m_currentResult.layerThicknesses = layerThicknesses;
    m_currentResult.layerDepths = layerDepths;
    m_currentResult.periods = periods;
    m_currentResult.omega = omega;
    m_currentResult.dObs = dataRandom;
    m_currentResult.dSyn = dataRandom;
    m_currentResult.success = true;

    // 更新图表
    updateModelChart();
    updateResistivityChart();
    updatePhaseChart();
    updateResidualChart();  // 更新残差图表（初始时可能为空）

    // 更新结果表格
    updateResultTable(m_currentResult);

    m_logText->append("随机模型生成完成！");
    m_statusLabel->setText("随机模型已生成");
}

void MTInversionGUI::onProgressUpdated(int iteration, double residual, double dmNorm) {
    m_progressBar->setValue(iteration);
    m_statusLabel->setText(QString("迭代 %1/%2 - 残差: %3, 更新: %4")
                          .arg(iteration).arg(m_progressBar->maximum())
                          .arg(residual, 0, 'e', 3)
                          .arg(dmNorm, 0, 'e', 3));

    // 更新日志（每5次迭代记录一次）
    if (iteration % 5 == 0 || iteration == 1) {
        m_logText->append(QString("迭代 %1: 残差=%2, 模型更新=%3")
                         .arg(iteration)
                         .arg(residual, 0, 'e', 6)
                         .arg(dmNorm, 0, 'e', 6));
    }
}

void MTInversionGUI::onInversionFinished(const MTInversionCore::InversionResult& result) {
    m_plotTimer->stop();
    m_currentResult = result;

    if (result.success) {
        m_statusLabel->setText(QString("反演完成 - 迭代次数: %1").arg(result.nIterations));
        m_logText->append(QString("反演成功完成！迭代次数: %1").arg(result.nIterations));

        // 更新结果表格
        updateResultTable(result);

        // 更新图表
        updatePlotData(result);
    } else {
        m_statusLabel->setText("反演失败");
        m_logText->append(QString("反演失败: %1").arg(QString::fromStdString(result.errorMessage)));
        QMessageBox::critical(this, "错误", QString::fromStdString(result.errorMessage));
    }

    // 注意：线程指针的清理在finished信号的处理中完成
    m_btnStart->setEnabled(true);
    m_btnStop->setEnabled(false);
    m_progressBar->setValue(m_progressBar->maximum());
}

void MTInversionGUI::updateResultTable(const MTInversionCore::InversionResult& result) {
    int M = static_cast<int>(result.mFinal.size());
    if (M <= 0) return;
    
    m_resultTable->setRowCount(M);

    for (int i = 0; i < M; i++) {
        // 检查数组边界
        if (i >= static_cast<int>(result.mTrue.size()) || 
            i >= static_cast<int>(result.mInit.size()) || 
            i >= static_cast<int>(result.mFinal.size())) {
            continue;  // 跳过无效行
        }
        
        double rhoTrue = pow(10.0, result.mTrue[i]);
        double rhoInit = pow(10.0, result.mInit[i]);
        double rhoFinal = pow(10.0, result.mFinal[i]);
        
        // 检查NaN和Inf
        if (!std::isfinite(rhoTrue)) rhoTrue = 0.1;
        if (!std::isfinite(rhoInit)) rhoInit = 0.1;
        if (!std::isfinite(rhoFinal)) rhoFinal = 0.1;
        
        // 确保值为正数
        if (rhoTrue <= 0.0) rhoTrue = 0.1;
        if (rhoInit <= 0.0) rhoInit = 0.1;
        if (rhoFinal <= 0.0) rhoFinal = 0.1;
        
        // 获取层厚度，如果没有则使用默认值
        double thickness = 100.0;  // 默认值
        if (!result.layerThicknesses.empty() && i < static_cast<int>(result.layerThicknesses.size())) {
            thickness = result.layerThicknesses[i];
            if (!std::isfinite(thickness) || thickness <= 0.0) {
                thickness = 100.0;
            }
        }

        // 删除旧item（如果存在）以避免内存泄漏
        QTableWidgetItem* item = m_resultTable->item(i, 0);
        if (item) delete item;
        m_resultTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        
        item = m_resultTable->item(i, 1);
        if (item) delete item;
        m_resultTable->setItem(i, 1, new QTableWidgetItem(QString::number(thickness, 'f', 2)));
        
        item = m_resultTable->item(i, 2);
        if (item) delete item;
        m_resultTable->setItem(i, 2, new QTableWidgetItem(QString::number(rhoTrue, 'f', 2)));
        
        item = m_resultTable->item(i, 3);
        if (item) delete item;
        m_resultTable->setItem(i, 3, new QTableWidgetItem(QString::number(rhoInit, 'f', 2)));
        
        item = m_resultTable->item(i, 4);
        if (item) delete item;
        m_resultTable->setItem(i, 4, new QTableWidgetItem(QString::number(rhoFinal, 'f', 2)));
    }
}

void MTInversionGUI::updatePlot() {
    // 检查线程是否还在运行
    if (m_workerThread && !m_workerThread->isRunning()) {
        // 线程已结束，停止定时器
        m_plotTimer->stop();
    }
    
    // 即使反演未完成，也更新残差图表（如果有数据）
    if (!m_currentResult.residualHistory.empty() || !m_currentResult.dmNormHistory.empty()) {
        updateResidualChart();
    }
    
    if (!m_currentResult.success || m_currentResult.mFinal.empty()) {
        return;
    }

    updatePlotData(m_currentResult);
}

void MTInversionGUI::updatePlotData(const MTInversionCore::InversionResult& result) {
    m_currentResult = result;
    updateModelChart();
    updateResistivityChart();
    updatePhaseChart();
    updateResidualChart();
}

void MTInversionGUI::clearCharts() {
    // 清除模型图表
    QList<QAbstractSeries*> series = m_modelChart->series();
    for (QAbstractSeries* s : series) {
        m_modelChart->removeSeries(s);
        delete s;  // 显式删除series
    }
    QList<QAbstractAxis*> axes = m_modelChart->axes();
    for (QAbstractAxis* axis : axes) {
        m_modelChart->removeAxis(axis);
        delete axis;
    }

    // 清除视电阻率图表
    series = m_resistivityChart->series();
    for (QAbstractSeries* s : series) {
        m_resistivityChart->removeSeries(s);
        delete s;  // 显式删除series
    }
    axes = m_resistivityChart->axes();
    for (QAbstractAxis* axis : axes) {
        m_resistivityChart->removeAxis(axis);
        delete axis;
    }

    // 清除相位图表
    series = m_phaseChart->series();
    for (QAbstractSeries* s : series) {
        m_phaseChart->removeSeries(s);
        delete s;  // 显式删除series
    }
    axes = m_phaseChart->axes();
    for (QAbstractAxis* axis : axes) {
        m_phaseChart->removeAxis(axis);
        delete axis;
    }

    // 清除残差图表（如果已创建）
    if (m_residualChart) {
        series = m_residualChart->series();
        for (QAbstractSeries* s : series) {
            m_residualChart->removeSeries(s);
            delete s;  // 显式删除series
        }
        axes = m_residualChart->axes();
        for (QAbstractAxis* axis : axes) {
            m_residualChart->removeAxis(axis);
            delete axis;
        }
    }
}

void MTInversionGUI::updateModelChart() {
    if (m_currentResult.mTrue.empty() || !m_currentResult.success) {
        return;
    }

    // 清除现有series和轴
    QList<QAbstractSeries*> series = m_modelChart->series();
    for (QAbstractSeries* s : series) {
        m_modelChart->removeSeries(s);
        delete s;  // 显式删除series
    }
    QList<QAbstractAxis*> axes = m_modelChart->axes();
    for (QAbstractAxis* axis : axes) {
        m_modelChart->removeAxis(axis);
        delete axis;
    }

    int M = static_cast<int>(m_currentResult.mTrue.size());

    // 使用层厚度和深度信息
    std::vector<double> layerThicknesses = m_currentResult.layerThicknesses;
    std::vector<double> layerDepths = m_currentResult.layerDepths;
    
    // 如果没有层厚度信息，使用默认值
    if (layerThicknesses.empty() || layerDepths.empty()) {
        layerThicknesses.resize(M);
        layerDepths.resize(M);
        double depth = 0.0;
        double dz = 100.0;
        for (int i = 0; i < M; i++) {
            layerDepths[i] = depth;
            layerThicknesses[i] = dz;
            depth += dz;
        }
    }

    // 创建阶梯状序列（每层内部电阻率不变）
    QLineSeries* seriesTrue = new QLineSeries();
    QLineSeries* seriesInit = new QLineSeries();
    QLineSeries* seriesFinal = new QLineSeries();

    seriesTrue->setName("真实模型");
    seriesInit->setName("初始模型");
    seriesFinal->setName("反演结果");

    QPen penTrue(Qt::red, 2);
    QPen penInit(Qt::blue, 2, Qt::DashLine);
    QPen penFinal(Qt::green, 2);

    seriesTrue->setPen(penTrue);
    seriesInit->setPen(penInit);
    seriesFinal->setPen(penFinal);

    // 计算总深度
    double totalDepth = 0.0;
    for (int i = 0; i < M; i++) {
        totalDepth += layerThicknesses[i];
    }

    // 创建阶梯状曲线：每层内部电阻率不变，在层边界处跳跃
    double minRho = 1e10;  // 用于确定电阻率范围
    double maxRho = 1e-10;
    
    for (int i = 0; i < M; i++) {
        // 检查数组边界
        if (i >= static_cast<int>(m_currentResult.mTrue.size())) {
            continue;  // 跳过无效索引
        }
        
        double rhoTrue = pow(10.0, m_currentResult.mTrue[i]);
        // 检查NaN和Inf
        if (!std::isfinite(rhoTrue)) rhoTrue = 0.1;
        // 确保电阻率值为正数
        if (rhoTrue <= 0.0) rhoTrue = 0.1;
        if (rhoTrue < minRho) minRho = rhoTrue;
        if (rhoTrue > maxRho) maxRho = rhoTrue;
        
        // 确保深度值为正数（最小深度为0.01米，用于对数坐标）
        double depthTop = std::max(0.01, layerDepths[i]);
        double thickness = layerThicknesses[i];
        if (!std::isfinite(thickness) || thickness <= 0.0) {
            thickness = 100.0;  // 默认厚度
        }
        double depthBottom = std::max(0.01, depthTop + thickness);
        
        // 阶梯状：在层顶部和底部都添加点，保持水平
        seriesTrue->append(rhoTrue, depthTop);
        seriesTrue->append(rhoTrue, depthBottom);
        
        if (!m_currentResult.mInit.empty() && i < static_cast<int>(m_currentResult.mInit.size())) {
            double rhoInit = pow(10.0, m_currentResult.mInit[i]);
            if (!std::isfinite(rhoInit)) rhoInit = 0.1;
            if (rhoInit <= 0.0) rhoInit = 0.1;
            if (rhoInit < minRho) minRho = rhoInit;
            if (rhoInit > maxRho) maxRho = rhoInit;
            seriesInit->append(rhoInit, depthTop);
            seriesInit->append(rhoInit, depthBottom);
        }

        if (!m_currentResult.mFinal.empty() && i < static_cast<int>(m_currentResult.mFinal.size())) {
            double rhoFinal = pow(10.0, m_currentResult.mFinal[i]);
            if (!std::isfinite(rhoFinal)) rhoFinal = 0.1;
            if (rhoFinal <= 0.0) rhoFinal = 0.1;
            if (rhoFinal < minRho) minRho = rhoFinal;
            if (rhoFinal > maxRho) maxRho = rhoFinal;
            seriesFinal->append(rhoFinal, depthTop);
            seriesFinal->append(rhoFinal, depthBottom);
        }
    }
    
    // 确保范围有效
    if (minRho <= 0.0) minRho = 0.1;
    if (maxRho <= minRho) maxRho = minRho * 10.0;

    m_modelChart->addSeries(seriesTrue);
    if (!m_currentResult.mInit.empty()) {
        m_modelChart->addSeries(seriesInit);
    }
    if (!m_currentResult.mFinal.empty()) {
        m_modelChart->addSeries(seriesFinal);
    }

    // 设置坐标轴：深度在Y轴（垂直），电阻率在X轴（水平），深度向下（0在上）
    QLogValueAxis* axisX = new QLogValueAxis();
    axisX->setTitleText("电阻率 (Ω·m)");
    axisX->setLabelFormat("%.1e");
    axisX->setBase(10.0);
    axisX->setRange(0.1, 10000.0);  // 固定范围：0.1 Ω·m 到 10000 Ω·m
    m_modelChart->addAxis(axisX, Qt::AlignBottom);
    seriesTrue->attachAxis(axisX);
    if (!m_currentResult.mInit.empty()) seriesInit->attachAxis(axisX);
    if (!m_currentResult.mFinal.empty()) seriesFinal->attachAxis(axisX);

    QLogValueAxis* axisY = new QLogValueAxis();
    axisY->setTitleText("深度 (m)");
    axisY->setLabelFormat("%.0f");
    axisY->setBase(10.0);
    // 反转Y轴，使深度0在上方，深度值向下增加
    // 确保最小深度为正数（对数坐标不能为0），使用很小的正数
    double minDepth = 0.01;
    double maxDepth = std::max(1.0, totalDepth);
    if (maxDepth < minDepth) maxDepth = minDepth * 10.0;
    axisY->setRange(maxDepth, minDepth);  // 从最大深度到最小深度（反转）
    axisY->setReverse(true);  // 明确设置反向显示
    m_modelChart->addAxis(axisY, Qt::AlignLeft);
    seriesTrue->attachAxis(axisY);
    if (!m_currentResult.mInit.empty()) seriesInit->attachAxis(axisY);
    if (!m_currentResult.mFinal.empty()) seriesFinal->attachAxis(axisY);

    m_modelChart->legend()->setVisible(true);
    m_modelChart->legend()->setAlignment(Qt::AlignBottom);
}

void MTInversionGUI::updateResistivityChart() {
    if (m_currentResult.periods.empty() || !m_currentResult.success) {
        return;
    }

    // 清除现有series和轴
    QList<QAbstractSeries*> series = m_resistivityChart->series();
    for (QAbstractSeries* s : series) {
        m_resistivityChart->removeSeries(s);
        delete s;  // 显式删除series
    }
    QList<QAbstractAxis*> axes = m_resistivityChart->axes();
    for (QAbstractAxis* axis : axes) {
        m_resistivityChart->removeAxis(axis);
        delete axis;
    }

    int nFreq = static_cast<int>(m_currentResult.periods.size());

    QLineSeries* seriesObs = new QLineSeries();
    QLineSeries* seriesSyn = new QLineSeries();
    QScatterSeries* scatterObs = new QScatterSeries();
    QScatterSeries* scatterSyn = new QScatterSeries();

    seriesObs->setName("观测数据");
    seriesSyn->setName("合成数据");
    scatterObs->setName("观测数据（散点）");
    scatterSyn->setName("合成数据（散点）");

    // 使用更鲜明的颜色和不同的线条样式来区分曲线
    // 观测数据：深红色实线，较粗
    QPen penObs(QColor(200, 0, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    // 合成数据：深蓝色虚线，较粗
    QPen penSyn(QColor(0, 0, 200), 3, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    seriesObs->setPen(penObs);
    seriesSyn->setPen(penSyn);

    // 观测数据：空心方框，稍大
    scatterObs->setMarkerSize(6);
    scatterObs->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    scatterObs->setColor(Qt::white);  // 填充白色（看起来像空心）
    scatterObs->setPen(QPen(QColor(200, 0, 0), 2));  // 边框为深红色，2像素宽
    // 拟合数据：实心圆，稍小
    scatterSyn->setMarkerSize(4);
    scatterSyn->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    scatterSyn->setColor(QColor(0, 0, 200));  // 实心，填充深蓝色
    scatterSyn->setPen(QPen(QColor(0, 0, 200), 1));  // 边框也为深蓝色

    for (int i = 0; i < nFreq; i++) {
        if (i >= static_cast<int>(m_currentResult.periods.size())) break;
        double period = m_currentResult.periods[i];
        
        // 检查数据索引是否有效
        int idxRho = i * 2;
        if (idxRho >= static_cast<int>(m_currentResult.dObs.size()) || 
            idxRho >= static_cast<int>(m_currentResult.dSyn.size())) break;
        
        // 正演输出是log10(视电阻率)，需要转换为线性值
        double rhoObs = pow(10.0, m_currentResult.dObs[idxRho]);
        double rhoSyn = pow(10.0, m_currentResult.dSyn[idxRho]);
        
        // 检查NaN和Inf
        if (!std::isfinite(rhoObs)) rhoObs = 0.1;
        if (!std::isfinite(rhoSyn)) rhoSyn = 0.1;
        // 确保值为正数
        if (rhoObs <= 0.0) rhoObs = 0.1;
        if (rhoSyn <= 0.0) rhoSyn = 0.1;

        seriesObs->append(period, rhoObs);
        seriesSyn->append(period, rhoSyn);
        scatterObs->append(period, rhoObs);
        scatterSyn->append(period, rhoSyn);
    }

    m_resistivityChart->addSeries(seriesObs);
    m_resistivityChart->addSeries(seriesSyn);
    m_resistivityChart->addSeries(scatterObs);
    m_resistivityChart->addSeries(scatterSyn);

    // 设置坐标轴（对数坐标，固定范围）
    QLogValueAxis* axisX = new QLogValueAxis();
    axisX->setTitleText("周期 (s)");
    axisX->setLabelFormat("%.3f");
    axisX->setBase(10.0);
    axisX->setRange(0.001, 1000.0);  // 固定范围：0.001s 到 1000s
    m_resistivityChart->addAxis(axisX, Qt::AlignBottom);
    seriesObs->attachAxis(axisX);
    seriesSyn->attachAxis(axisX);
    scatterObs->attachAxis(axisX);
    scatterSyn->attachAxis(axisX);

    QLogValueAxis* axisY = new QLogValueAxis();
    axisY->setTitleText("视电阻率 (Ω·m)");
    axisY->setLabelFormat("%.1e");
    axisY->setBase(10.0);
    axisY->setRange(0.1, 10000.0);  // 固定范围：0.1 Ω·m 到 10000 Ω·m
    m_resistivityChart->addAxis(axisY, Qt::AlignLeft);
    seriesObs->attachAxis(axisY);
    seriesSyn->attachAxis(axisY);
    scatterObs->attachAxis(axisY);
    scatterSyn->attachAxis(axisY);

    m_resistivityChart->legend()->setVisible(true);
    m_resistivityChart->legend()->setAlignment(Qt::AlignBottom);
    
    // 隐藏散点图的图例，只显示线条图例
    QLegend* legend = m_resistivityChart->legend();
    if (legend) {
        QList<QLegendMarker*> markers = legend->markers();
        for (QLegendMarker* marker : markers) {
            if (marker->series() == scatterObs || marker->series() == scatterSyn) {
                marker->setVisible(false);
            }
        }
    }
}

void MTInversionGUI::updatePhaseChart() {
    if (m_currentResult.periods.empty() || !m_currentResult.success) {
        return;
    }

    // 清除现有series和轴
    QList<QAbstractSeries*> series = m_phaseChart->series();
    for (QAbstractSeries* s : series) {
        m_phaseChart->removeSeries(s);
        delete s;  // 显式删除series
    }
    QList<QAbstractAxis*> axes = m_phaseChart->axes();
    for (QAbstractAxis* axis : axes) {
        m_phaseChart->removeAxis(axis);
        delete axis;
    }

    int nFreq = static_cast<int>(m_currentResult.periods.size());

    QLineSeries* seriesObs = new QLineSeries();
    QLineSeries* seriesSyn = new QLineSeries();
    QScatterSeries* scatterObs = new QScatterSeries();
    QScatterSeries* scatterSyn = new QScatterSeries();

    seriesObs->setName("观测数据");
    seriesSyn->setName("合成数据");
    scatterObs->setName("观测数据（散点）");
    scatterSyn->setName("合成数据（散点）");

    // 使用更鲜明的颜色和不同的线条样式来区分曲线
    // 观测数据：深红色实线，较粗
    QPen penObs(QColor(200, 0, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    // 合成数据：深蓝色虚线，较粗
    QPen penSyn(QColor(0, 0, 200), 3, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    seriesObs->setPen(penObs);
    seriesSyn->setPen(penSyn);

    // 观测数据：空心方框，稍大
    scatterObs->setMarkerSize(6);
    scatterObs->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    scatterObs->setColor(Qt::white);  // 填充白色（看起来像空心）
    scatterObs->setPen(QPen(QColor(200, 0, 0), 2));  // 边框为深红色，2像素宽
    // 拟合数据：实心圆，稍小
    scatterSyn->setMarkerSize(4);
    scatterSyn->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    scatterSyn->setColor(QColor(0, 0, 200));  // 实心，填充深蓝色
    scatterSyn->setPen(QPen(QColor(0, 0, 200), 1));  // 边框也为深蓝色

    for (int i = 0; i < nFreq; i++) {
        if (i >= static_cast<int>(m_currentResult.periods.size())) break;
        double period = m_currentResult.periods[i];
        
        // 检查数据索引是否有效
        int idxPhase = i * 2 + 1;
        if (idxPhase >= static_cast<int>(m_currentResult.dObs.size()) || 
            idxPhase >= static_cast<int>(m_currentResult.dSyn.size())) break;
        
        // 正演输出相位是度数
        double phaseObs = m_currentResult.dObs[idxPhase];
        double phaseSyn = m_currentResult.dSyn[idxPhase];
        
        // 检查NaN和Inf
        if (!std::isfinite(phaseObs)) phaseObs = 0.0;
        if (!std::isfinite(phaseSyn)) phaseSyn = 0.0;

        seriesObs->append(period, phaseObs);
        seriesSyn->append(period, phaseSyn);
        scatterObs->append(period, phaseObs);
        scatterSyn->append(period, phaseSyn);
    }

    m_phaseChart->addSeries(seriesObs);
    m_phaseChart->addSeries(seriesSyn);
    m_phaseChart->addSeries(scatterObs);
    m_phaseChart->addSeries(scatterSyn);

    // 设置坐标轴（固定范围）
    QLogValueAxis* axisX = new QLogValueAxis();
    axisX->setTitleText("周期 (s)");
    axisX->setLabelFormat("%.3f");
    axisX->setBase(10.0);
    axisX->setRange(0.001, 1000.0);  // 固定范围：0.001s 到 1000s
    m_phaseChart->addAxis(axisX, Qt::AlignBottom);
    seriesObs->attachAxis(axisX);
    seriesSyn->attachAxis(axisX);
    scatterObs->attachAxis(axisX);
    scatterSyn->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("相位 (度)");
    axisY->setLabelFormat("%.1f");
    axisY->setRange(0.0, 90.0);  // 固定范围：0° 到 90°
    m_phaseChart->addAxis(axisY, Qt::AlignLeft);
    seriesObs->attachAxis(axisY);
    seriesSyn->attachAxis(axisY);
    scatterObs->attachAxis(axisY);
    scatterSyn->attachAxis(axisY);

    m_phaseChart->legend()->setVisible(true);
    m_phaseChart->legend()->setAlignment(Qt::AlignBottom);
    
    // 隐藏散点图的图例，只显示线条图例
    QLegend* legend = m_phaseChart->legend();
    if (legend) {
        QList<QLegendMarker*> markers = legend->markers();
        for (QLegendMarker* marker : markers) {
            if (marker->series() == scatterObs || marker->series() == scatterSyn) {
                marker->setVisible(false);
            }
        }
    }
}

void MTInversionGUI::updateResidualChart() {
    // 检查图表是否已创建
    if (!m_residualChart) {
        return;
    }
    
    // 清除现有series和轴
    QList<QAbstractSeries*> series = m_residualChart->series();
    for (QAbstractSeries* s : series) {
        m_residualChart->removeSeries(s);
        delete s;
    }
    QList<QAbstractAxis*> axes = m_residualChart->axes();
    for (QAbstractAxis* axis : axes) {
        m_residualChart->removeAxis(axis);
        delete axis;
    }

    // 检查是否有数据
    if (m_currentResult.residualHistory.empty() && m_currentResult.dmNormHistory.empty()) {
        return;
    }

    // 创建series
    QLineSeries* seriesResidual = new QLineSeries();
    seriesResidual->setName("残差");
    seriesResidual->setPen(QPen(Qt::red, 2));
    
    QLineSeries* seriesDmNorm = new QLineSeries();
    seriesDmNorm->setName("模型更新范数");
    seriesDmNorm->setPen(QPen(Qt::blue, 2, Qt::DashLine));

    // 收集所有数据点并计算范围
    int maxIter = 0;
    double minYValue = std::numeric_limits<double>::max();
    double maxYValue = std::numeric_limits<double>::lowest();
    bool hasAnyData = false;

    // 处理残差数据
    if (!m_currentResult.residualHistory.empty()) {
        for (size_t i = 0; i < m_currentResult.residualHistory.size(); i++) {
            double val = m_currentResult.residualHistory[i];
            if (std::isfinite(val) && val > 0.0) {
                int iter = static_cast<int>(i + 1);
                seriesResidual->append(iter, val);
                maxIter = std::max(maxIter, iter);
                minYValue = std::min(minYValue, val);
                maxYValue = std::max(maxYValue, val);
                hasAnyData = true;
            }
        }
    }

    // 处理模型更新范数数据
    if (!m_currentResult.dmNormHistory.empty()) {
        for (size_t i = 0; i < m_currentResult.dmNormHistory.size(); i++) {
            double val = m_currentResult.dmNormHistory[i];
            if (std::isfinite(val) && val >= 0.0) {
                int iter = static_cast<int>(i + 1);
                seriesDmNorm->append(iter, val);
                maxIter = std::max(maxIter, iter);
                if (val > 0.0) {
                    minYValue = std::min(minYValue, val);
                }
                maxYValue = std::max(maxYValue, val);
                hasAnyData = true;
            }
        }
    }

    // 如果没有数据，清理并返回
    if (!hasAnyData || maxIter == 0) {
        delete seriesResidual;
        delete seriesDmNorm;
        return;
    }

    // 添加series到图表
    if (seriesResidual->count() > 0) {
        m_residualChart->addSeries(seriesResidual);
    } else {
        delete seriesResidual;
        seriesResidual = nullptr;
    }
    
    if (seriesDmNorm->count() > 0) {
        m_residualChart->addSeries(seriesDmNorm);
    } else {
        delete seriesDmNorm;
        seriesDmNorm = nullptr;
    }

    // 设置X轴 - 自适应范围
    QValueAxis* axisX = new QValueAxis();
    axisX->setTitleText("迭代次数");
    axisX->setLabelFormat("%d");
    int xMin = 1;
    int xMax = maxIter;
    if (xMax < xMin) xMax = xMin;
    // 添加右边距
    xMax = xMax + 1;
    axisX->setRange(xMin, xMax);
    axisX->setTickCount(std::min(11, std::max(3, xMax - xMin + 1)));
    m_residualChart->addAxis(axisX, Qt::AlignBottom);
    if (seriesResidual) seriesResidual->attachAxis(axisX);
    if (seriesDmNorm) seriesDmNorm->attachAxis(axisX);

    // 设置Y轴 - 自适应范围（对数坐标）
    double yMin = minYValue;
    double yMax = maxYValue;
    
    // // 确保范围有效
    // if (yMin <= 0.0 || yMin < yMax * 0.001) {
    //     yMin = yMax * 0.001;
    // }
    // if (yMin < 1e-10) {
    //     yMin = 1e-10;
    // }
    
    // 添加边距
    yMax = yMax * 1.2;
    yMin = yMin / 1.2;
    
    // // 确保yMax > yMin
    // if (yMax <= yMin) {
    //     yMax = yMin * 10.0;
    // }
    // qDebug() << "yMin: " << yMin << "yMax: " << yMax;

    QLogValueAxis* axisY = new QLogValueAxis();
    axisY->setTitleText("数值");
    axisY->setLabelFormat("%.2e");
    axisY->setBase(10.0);
    axisY->setRange(yMin, yMax);
    m_residualChart->addAxis(axisY, Qt::AlignLeft);
    if (seriesResidual) seriesResidual->attachAxis(axisY);
    if (seriesDmNorm) seriesDmNorm->attachAxis(axisY);

    // 设置图例
    m_residualChart->legend()->setVisible(true);
    m_residualChart->legend()->setAlignment(Qt::AlignTop);
}

// 主函数
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MTInversionGUI window;
    window.show();

    return app.exec();
}
