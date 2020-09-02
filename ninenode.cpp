#include "ninenode.h"

NineNode::NineNode(unsigned char layer, TreeNode *treeparent, int x, int y):
    m_TreeParent(treeparent),
    m_GenLayer(pow(3, layer - 1))
{
    for (int i = 0; i < 9; i++)
    {
        m_Node[i] = NULL;
    }
}
NineNode::~NineNode()
{
    for (int i = 0; i < 9; i++)
    {
        if (m_Node[i])
        {
            delete m_Node[i];
        }
    }
}
void NineNode::Info()
{
    for (int i = 0; i < 9; i++)
    {
        if (m_Node[i])
        {
            m_Node[i]->Info();
        }
    }
}

/**
 * @brief AddEntity
 * 增加实体
 * @param entity
 * @return
 */
bool NineNode::AddEntity(Entity *entity)
{
    int idx = GetCellIndex(entity->GetNodeX(), entity->GetNodeY());
    if (idx != -1)
    {
        if (m_Node[idx] == NULL)
        {
            unsigned char childlayer = m_TreeParent->GetLayer() - 1;

            switch (idx)
            {
            case 0:
                m_Node[0] = new TreeNode(childlayer, m_TreeParent, 0, 0);
                break;
            case 1:
                m_Node[1] = new TreeNode(childlayer, m_TreeParent, 0, 1);
                break;
            case 2:
                m_Node[2] = new TreeNode(childlayer, m_TreeParent, 1, 1);
                break;
            case 3:
                m_Node[3] = new TreeNode(childlayer, m_TreeParent, 1, 0);
                break;
            case 4:
                m_Node[4] = new TreeNode(childlayer, m_TreeParent, 1, -1);
                break;
            case 5:
                m_Node[5] = new TreeNode(childlayer, m_TreeParent, 0, -1);
                break;
            case 6:
                m_Node[6] = new TreeNode(childlayer, m_TreeParent, -1, -1);
                break;
            case 7:
                m_Node[7] = new TreeNode(childlayer, m_TreeParent, -1, 0);
                break;
            case 8:
                m_Node[8] = new TreeNode(childlayer, m_TreeParent, -1, 1);
                break;
            default:
                break;
            }
        }
        return m_Node[idx]->AddEntity(entity);
    }
    return false;
}

/**
 * @brief SetTreeNode
 * 设置节点
 * @param node
 * @return
 */
bool NineNode::SetTreeNode(TreeNode *node)
{
    int cx = node->GetX();
    int cy = node->GetY();
    int idx = GetCellIndex(cx, cy);
    if (idx != -1)
    {
        if (m_Node[idx])
        {
            delete m_Node[idx];
        }
        m_Node[idx] = node;
        return true;
    }
    else
    {
        printf("Error SetTreeNode %d:%d .\n", cx, cy);
        return false;
    }
}

/**
 * @brief SentMessageToEntity
 * 发信息到Entity
 * @param x
 * @param y
 */
void NineNode::SentMessageToEntity(int x, int y)
{
    int idx = GetCellIndex(x, y);
    if (m_Node[idx])
    {
        m_Node[idx]->SentMessageToEntity(x, y);
    }
}

/**
 * @brief GetCellIndex
 * 获取单元格位置
 * @param x
 * @param y
 * @return
 */
int NineNode::GetCellIndex(int x, int y)
{
    //中心点
    int cx = m_TreeParent->GetX();
    int cy = m_TreeParent->GetY();

    char dx = 0;
    char dy = 0;
    int ix = x - cx;
    int iy = y - cy;

    if (ix > 0)
    {
        dx = (ix + m_GenLayer / 2) / m_GenLayer;
    }
    else
    {
        dx = (ix - m_GenLayer / 2) / m_GenLayer;
    }

    if (iy > 0)
    {
        dy = (iy + m_GenLayer / 2) / m_GenLayer;
    }
    else
    {
        dy = (iy - m_GenLayer / 2) / m_GenLayer;
    }

    unsigned char sx = dx;
    unsigned char sy = dy;

    dy = 0x00FF & dy;
    unsigned short idx = (sx << 8) | sy;

    return GetCellIndex(idx);
}

/**
 * @brief GetCellIndexFromDirection
 * 通过与中心点方向获取单元格位置
 * @param x -1 0 1
 * @param y -1 0 1
 * @return
 */
TreeNode *NineNode::GetCellFromDirection(int x, int y)
{
    y = 0x00FF & y;
    unsigned short idx = (x << 8) | y;
    int index = GetCellIndex(idx);
    if(index != -1)
    {
        return m_Node[index];
    }
    return NULL;
}
/**
 * @brief GetCellIndex
 * 获取单元格位置
 * @param index
 * @return
 */
int NineNode::GetCellIndex(unsigned short idx)
{
    int index = -1;
    switch (idx)
    {
    case 0x0000:
        index = 0;
        break;
    case 0x0001:
        index = 1;
        break;
    case 0x0101:
        index = 2;
        break;
    case 0x0100:
        index = 3;
        break;
    case 0x01FF:
        index = 4;
        break;
    case 0x00FF:
        index = 5;
        break;
    case 0xFFFF:
        index = 6;
        break;
    case 0xFF00:
        index = 7;
        break;
    case 0xFF01:
        index = 8;
        break;
    default:
        index = -1;
        break;
    }
    if (index == -1)
    {
        printf("Error GetCellIndex (%x) .\n", idx);
    }
    return index;

}

/**
 * @brief NineNode::getTreeNode
 * 获取一个节点
 * @param x
 * @param y
 * @return
 */
TreeNode *NineNode::getTreeNode(int x, int y)
{
    int idx = GetCellIndex(x, y);
    if(idx == -1)
    {
       return NULL;
    }
    if (m_Node[idx])
    {
        return m_Node[idx];
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief NineNode::deleteTreeNode
 * 删除节点
 * @param x
 * @param y
 * @return
 */
bool NineNode::deleteTreeNode(int x, int y)
{
    int idx = GetCellIndex(x, y);
    if(idx != -1)
    {
        delete m_Node[idx];
        m_Node[idx] = NULL;
    }
    return false;
}

/**
 * @brief NineNode::getEntityNumber
 * 获取实体数量
 * @param outnumb
 */
void NineNode::getEntityNumber(int &outnumb)
{
    for (int i = 0; i < 9; i++)
    {
        if (m_Node[i])
        {
            m_Node[i]->getEntityNumber(outnumb);
        }
    }
}

/**
 * @brief getMaxDensity
 * 获取叶子节点密度最大的值
 * @param outdensity
 */
void NineNode::getMaxDensity(int &outdensity)
{
    for (int i = 0; i < 9; i++)
    {
        if (m_Node[i])
        {
            m_Node[i]->getMaxDensity(outdensity);
        }
    }
}
