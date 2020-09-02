#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QDateTime>
#include "treenode.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_nTimerID = this->startTimer(200);
    srand(QDateTime::currentDateTime().time().msec());

    m_currentLayer = TreeNode::g_Root->GetLayer();


    m_TreeNode = TreeNode::g_Root;

    m_X = 0;
    m_Y = 0;
    m_ScCent = 394;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::drawTreeNode
 * 画节点
 * @param treeNode
 * @param layer
 * @param painter
 * @param x
 * @param y
 * @param length
 */
void MainWindow::drawTreeNode(TreeNode *treeNode, int layer, QPainter &painter, int x, int y, int length)
{
    int celllength = length/3;
    NineNode *nine = NULL;
    nine = treeNode->getChildren();
    if(layer == 3)
    {
        //获取实体数量
        int number = 0;
        treeNode->getMaxDensity(number);
        int density = number/10;
        if(density > 255)
        {
            density = 255;
        }
        painter.fillRect(QRect(x, y, length, length), QBrush(QColor(255, 255 - density, 255 - density)));
        return;
    }

    if(layer == 0)
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QColor(0, 160, 160));
    }
    else
    {
        painter.setRenderHint(QPainter::Antialiasing, false);
        painter.setPen(QColor(100, 100, 100));
    }

    if(treeNode->GetLayer() == 0)
    {
        //显示Entity点
        if(treeNode->getEntitiesMap() != NULL)
        {
            int cx = treeNode->GetX() * 100;
            int cy = treeNode->GetY() * 100;
            std::map<int, Entity *>::iterator itor = treeNode->getEntitiesMap()->begin();
            for(; itor != treeNode->getEntitiesMap()->end(); ++itor)
            {
                float ex;
                float ey;
                int px, py;
                itor->second->GetPosition(ex, ey);
                px = x + length/2 + (ex - cx) * length/100;
                py = (y + length/2 - (ey - cy) * length/100);
                painter.drawEllipse(QPointF(px, py), 2, 2);
            }
        }

        return;
    }

    if(treeNode->getChildren() == NULL)
    {
        painter.fillRect(QRect(x, y, length, length), QBrush(Qt::gray));
        return;
    }

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            int cellx = x + celllength * i;
            int celly = y + celllength * j;
            TreeNode *node = treeNode->getChildren()->GetCellFromDirection(i - 1, 1 - j);
            if(node)
            {
               //如果是叶子节点
                drawTreeNode(node, layer + 1, painter, cellx, celly, length/3);
            }
            else
            {
                painter.fillRect(QRect(cellx, celly, celllength, celllength), QBrush(Qt::gray));
            }
            painter.drawRect(cellx, celly, celllength, celllength);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    // 设置画笔颜色
    painter.setPen(QColor(0, 0, 0));

    NineNode *nine = NULL;
    int layer = m_TreeNode->GetLayer();
    drawTreeNode(m_TreeNode, 0, painter, 20, 20, 729);

    //显示单位长度
    painter.setPen(QColor(255, 0, 0));
    painter.drawLine(QPointF(20, 10 ), QPointF(749, 10));
    painter.drawLine(QPointF(20, 6 ), QPointF(20, 14));
    painter.drawLine(QPointF(749, 6 ), QPointF(749, 14));
    painter.setPen(QColor(160, 160, 160));
    float scale = pow(3, layer) * 100;
    if(scale < 1000)
    {
        painter.drawText(754,14, QString("%1M").arg(scale));
    }
    else
    {
        painter.drawText(754,14, QString("%1KM").arg(scale/1000));
    }

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QColor(0, 160, 160));
    if(layer != 0)
    {
        int celllength = 729/3;
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                int cellx = 20 + celllength * i;
                int celly = 20 + celllength * j;
                painter.drawRect(cellx, celly, celllength, celllength);
            }
        }
    }
    // 绘制线框
    painter.drawRect(20, 20, 729, 729);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 如果是鼠标左键按下
    if(event->button() == Qt::LeftButton)
    {
        int x = event->x() - m_ScCent;
        int y = -(event->y() - m_ScCent);
        qDebug()<<"X:" << x << "," << y;

        int layer = m_TreeNode->GetLayer();
        int cx = m_TreeNode->GetX();
        int cy = m_TreeNode->GetY();

        if(m_currentLayer == 0)
        {
            //显示叶子节点
        }
        else
        {
            if(x > 124)
            {
                cx = cx + pow(3, layer - 1);
            }
            else if(x < -124)
            {
                cx = cx - pow(3, layer - 1 );
            }


            if(y > 124)
            {
                cy = cy + pow(3, layer - 1);
            }
            else if(y < -124)
            {
                cy = cy - pow(3, layer - 1);
            }

            qDebug()<<"C:" << cx << "," << cy;

            TreeNode *node = m_TreeNode->getChildTreeNode(cx, cy);
            if(node)
            {
                qDebug()<<"AAA:" << node;
                m_TreeNode = node;
            }
        }

        this->update();
    }

    // 如果是鼠标右键按下
    else if(event->button() == Qt::RightButton)
    {

    }
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_A)
    {
    }
}
void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerID)
    {
        std::map<int, Entity *>::iterator itor = Entity::g_entities.begin();
        for (itor; itor !=  Entity::g_entities.end(); ++itor)
        {
            itor->second->randMove();
            this->update();
        }
        QString text = QString("Layer:%1, X=%2 Y=%3 Entity Number:%4")
                .arg(m_TreeNode->GetLayer())
                .arg(m_TreeNode->GetX() * 100)
                .arg(m_TreeNode->GetY() * 100)
                .arg(Entity::g_entities.size());
        ui->label->setText(text);
    }
}

/**
 * @brief MainWindow::wheelEvent
 * @param 滚轮事件
 */
void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(event->delta() < 0)
    {
        if(m_TreeNode->getParent() != NULL)
        {
            m_TreeNode = m_TreeNode->getParent();
        }
    }
    else
    {
        //缩小节点
        if(m_TreeNode->GetLayer() > 0)
        {
            TreeNode *node = m_TreeNode->getChildTreeNode(m_TreeNode->GetX(), m_TreeNode->GetY());
            if(node)
            {
                m_TreeNode = node;
            }
        }
    }
    this->update();
}
void MainWindow::handleTimeout()
{
    qDebug()<<"Enter timeout processing function\n";
    killTimer(m_nTimerID);
}

/**
 * @brief MainWindow::on_pushButton_clicked
 * 创建Entity
 */
void MainWindow::on_pushButton_clicked()
{
    QString ty = ui->lineEdit->text();
    QString tx = ui->lineEdit_2->text();
    QString tn = ui->lineEdit_3->text();
    int createNumber = tn.toInt();

    for(int i = 0; i < createNumber; ++i)
    {
        Entity::createEntity(int(pow( -1, rand()) * rand())%tx.toInt(), int(pow( -1, rand()) * rand())%ty.toInt());
    }
    this->update();
}
