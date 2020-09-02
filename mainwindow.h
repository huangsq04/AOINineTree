#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "treenode.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void wheelEvent(QWheelEvent *event);

    void handleTimeout();  //超时处理函数

    virtual void timerEvent( QTimerEvent *event);

    /** 画节点 */
    void drawTreeNode(TreeNode *treeNode, int layer, QPainter &painter, int x, int y, int length);

private slots:

    void on_pushButton_clicked();

private:

    Ui::MainWindow *ui;

    int m_nTimerID;

    /** 当前显示的节点 */
    TreeNode *m_TreeNode;

    /** 当前显示的层数 */
    int m_currentLayer;

    int m_X;
    int m_Y;

    /** 坐标屏幕点 */
    int m_ScCent;
};

#endif // MAINWINDOW_H
