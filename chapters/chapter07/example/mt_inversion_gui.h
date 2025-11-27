#ifndef MT_INVERSION_GUI_H
#define MT_INVERSION_GUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QProgressBar>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QThread>
#include <QTimer>
#include <QSplitter>
#include <QTabWidget>
#include <QPointer>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QScatterSeries>
#include "mt_inversion_core.h"

/**
 * 反演工作线程
 * 在后台执行反演计算，避免阻塞UI
 */
class InversionWorkerThread : public QThread {
    Q_OBJECT

public:
    InversionWorkerThread(MTInversionCore* core,
                         const MTInversionCore::InversionParams& params,
                         QObject* parent = nullptr)
        : QThread(parent), m_core(core), m_params(params), m_shouldStop(false) {}

    MTInversionCore::InversionResult getResult() const { return m_result; }
    
    // 请求停止线程（优雅停止）
    void requestStop() {
        m_shouldStop = true;
    }

signals:
    void progressUpdated(int iteration, double residual, double dmNorm);
    void inversionFinished(const MTInversionCore::InversionResult& result);

protected:
    void run() override {
        // 设置进度回调
        m_core->setProgressCallback([](int iter, double res, double dm, void* data) {
            InversionWorkerThread* thread = static_cast<InversionWorkerThread*>(data);
            if (thread && !thread->m_shouldStop) {
                emit thread->progressUpdated(iter, res, dm);
            }
        }, this);

        // 执行反演
        if (!m_shouldStop) {
            m_result = m_core->invert(m_params);
        } else {
            m_result.success = false;
            m_result.errorMessage = "反演被用户取消";
        }

        // 发送完成信号
        if (!m_shouldStop) {
            emit inversionFinished(m_result);
        }
    }

private:
    MTInversionCore* m_core;
    MTInversionCore::InversionParams m_params;
    MTInversionCore::InversionResult m_result;
    volatile bool m_shouldStop;  // 停止标志（使用volatile确保多线程可见性）
};

/**
 * MT一维反演GUI主窗口
 */
class MTInversionGUI : public QWidget {
    Q_OBJECT

public:
    explicit MTInversionGUI(QWidget *parent = nullptr);
    ~MTInversionGUI();

private slots:
    void onStartInversion();
    void onStopInversion();
    void onGenerateRandomModel();
    void onProgressUpdated(int iteration, double residual, double dmNorm);
    void onInversionFinished(const MTInversionCore::InversionResult& result);
    void updatePlot();

private:
    void setupUI();
    void setupParameterPanel();
    void setupResultPanel();
    void setupPlotPanel();
    void updateResultTable(const MTInversionCore::InversionResult& result);
    void updatePlotData(const MTInversionCore::InversionResult& result);

    // 图表更新函数
    void updateModelChart();
    void updateResistivityChart();
    void updatePhaseChart();
    void updateResidualChart();            // 更新残差下降曲线

    // 清除图表
    void clearCharts();

    // UI组件
    QGroupBox* m_paramGroup;
    QSpinBox* m_spinMLayers;
    QSpinBox* m_spinNFreq;
    QDoubleSpinBox* m_spinLambda;
    QDoubleSpinBox* m_spinEpsilon;
    QSpinBox* m_spinMaxIter;
    QDoubleSpinBox* m_spinTolDm;

    QPushButton* m_btnStart;
    QPushButton* m_btnStop;
    QPushButton* m_btnGenerateRandom;
    QProgressBar* m_progressBar;
    QLabel* m_statusLabel;

    QDoubleSpinBox* m_spinMinRho;
    QDoubleSpinBox* m_spinMaxRho;
    QDoubleSpinBox* m_spinFirstThickness;
    QDoubleSpinBox* m_spinThicknessGrowth;

    QTextEdit* m_logText;
    QTableWidget* m_resultTable;

    // Qt Charts相关
    QWidget* m_chartContainer;         // 图表容器
    QChartView* m_modelChartView;      // 模型对比图
    QChartView* m_resistivityChartView; // 视电阻率曲线
    QChartView* m_phaseChartView;      // 相位曲线

    QChart* m_modelChart;
    QChart* m_resistivityChart;
    QChart* m_phaseChart;
    QChart* m_residualChart;              // 残差下降曲线图
    QChartView* m_residualChartView;       // 残差图表视图

    // 核心计算对象
    MTInversionCore* m_core;
    QPointer<InversionWorkerThread> m_workerThread;  // 使用QPointer自动管理线程指针

    // 数据存储
    MTInversionCore::InversionResult m_currentResult;
    QTimer* m_plotTimer;
};

#endif // MT_INVERSION_GUI_H
