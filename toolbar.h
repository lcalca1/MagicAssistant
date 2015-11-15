#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

namespace Ui {
class ToolBar;
}

class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = 0);
    ~ToolBar();

    ///< \note 屏幕截图是否压缩.
    inline void setCompressedEnable(bool is_compressed) {_is_compressed = is_compressed;}

signals:
    ///< \note 为了不让本程序出现在截图中，截图时先隐藏所有窗体，之后在显示.
    void hideRequested();
    void showRequested();

private slots:
    void on_toolButton_clicked();
    void on_toolButton2_clicked();
    void on_pOpenBtn_clicked();

    ///< \note 打开命令行.
    void on_pCommandBtn_clicked();

    ///< \note 屏幕截图.
    void on_pScreenshotBtn_clicked();

    ///< \note 定时关机.
    void on_pShutdownBtn_clicked();

private:
    Ui::ToolBar *ui;

    ///< \note 屏幕截图是否压缩.
    bool _is_compressed;
};

#endif // TOOLBAR_H
