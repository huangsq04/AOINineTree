#include "treenode.h"
#include "ninenode.h"

TreeNode *TreeNode::g_Root = new TreeNode;
TreeNode::TreeNode(unsigned char layer, TreeNode *parent, int x, int y) :
    m_Layer(layer),
    m_Parent(parent),
    m_X(x),
    m_Y(y),
    m_GenLayer(pow(3, layer))
{
    if (parent == NULL)
    {
        //根节点
    }
    else
    {
        m_X = m_Parent->GetX() + m_X * m_GenLayer;
        m_Y = m_Parent->GetY() + m_Y * m_GenLayer;
    }

    if (m_Layer == 0)
    {
        //叶子节点
        m_EntitiesMap = new std::map<int, Entity *>();
    }
}
TreeNode::~TreeNode()
{
    if (m_Children != NULL)
    {
        delete m_Children;
    }
    if (m_EntitiesMap)
    {
        delete m_EntitiesMap;
    }
}

/**
 * @brief SentMessageToEntity
 * 发信息到Entity
 */
void TreeNode::SentMessageToEntity(int x, int y)
{
    //当前节点
    if (m_Layer == 0)
    {
        if (m_EntitiesMap)
        {
            std::map<int, Entity *>::iterator itor = m_EntitiesMap->begin();
            for (itor; itor !=  m_EntitiesMap->end(); ++itor)
            {
                itor->second->Info();
            }
        }
    }
    else
    {
        //是否控制范围
        if (InRange(x, y))
        {
            m_Children->SentMessageToEntity(x ,y);
        }
        else
        {
            if (m_Parent)
            {
                m_Parent->SentMessageToEntity(x, y);
            }
            else
            {
                printf("Info SentMessageToEntity not in Range %d:(%d:%d):(%d:%d)\n", m_Layer, m_X, m_Y, x, y);
            }
        }

    }
}

/**
 * @brief SendMessageAroundEntities
 * 发信息到周边entity
 * @param dist 周边多少网格。1表示周边9个，2表示周边16个
 */
void TreeNode::SendMessageAroundEntities(int dist)
{
    SentMessageToEntity(m_X, m_Y);
    for (int i = 1; i <= dist; i++)
    {
        int x = m_X - i;
        int y = m_Y - i;
        int dp = 1 + pow(2, i);
        for (int k = 0; k < dp; k++)
        {
            y = m_Y - i;
            for (int t = 0; t < dp; t++)
            {
                if (!(x == m_X && y == m_Y))
                {
                    m_Parent->SentMessageToEntity(x, y);
                }
                y++;
            }
            x++;
        }
    }
}

/**
 * @brief MoveEntity
 * 移动实体，只在叶子节点调用
 * @param entity
 * @return
 */
bool TreeNode::MoveEntity(Entity *entity)
{
    int x  = entity->GetNodeX();
    int y  = entity->GetNodeY();

    if (m_Layer != 0)
    {
        printf("Error MoveEntity m_Layer(%d).\n", m_Layer);
        return false;
    }

    if (x == m_X && y == m_Y)
    {
        return true;
    }

    std::map<int, Entity *>::iterator itor = m_EntitiesMap->find(entity->GetID());
    if (itor != m_EntitiesMap->end())
    {
        if (m_Parent->AddEntity(entity))
        {
            DeleteEntity(entity);
            return true;
        }
        else
        {
            printf("Error MoveEntity m_Parent->AddEntity Entity x(%d):y(%d).\n", x, y);
            return false;
        }
    }
    else
    {
        //如果entity不在这个节点
        printf("Error MoveEntity m_Layer(%d) Entity id(%d) not in m_EntitiesMap.\n", m_Layer, entity->GetID());
        return false;
    }
}

/**
 * @brief DeleteEntity
 * 删除实体
 * @param entity
 * @return
 */
bool TreeNode::DeleteEntity(Entity *entity)
{
    std::map<int, Entity *>::iterator itor = m_EntitiesMap->find(entity->GetID());
    if (itor != m_EntitiesMap->end())
    {
        m_EntitiesMap->erase(itor);
    }
    else
    {
        printf("Error DeleteEntity Not found Entity id(%d).\n", entity->GetID());
        return false;
    }

    if(m_EntitiesMap->size() == 0)
    {
        //通知管理器收回这个节点
    }
}

/**
 * @brief SetChildren
 * 设置子节点
 * @param node
 * @return
 */
bool TreeNode::SetChildren(TreeNode *node)
{
    if (m_Children == NULL)
    {
        m_Children = new NineNode(m_Layer, this, m_X, m_Y);
    }

    return m_Children->SetTreeNode(node);
}
/**
 * @brief InRange
 * 是否在当前节点的控制范围
 * @param x
 * @param y
 * @return
 */
bool TreeNode::InRange(int x, int y)
{
    int max = (m_GenLayer + 1) / 2;
    if ((abs(x - m_X) < max) && (abs(y - m_Y) < max))
    {
        return true;
    }

    return false;
}
bool TreeNode::AddEntity(Entity *entity)
{
    int id = entity->GetID();
    int x  = entity->GetNodeX();
    int y  = entity->GetNodeY();

    if (m_Layer == 0)
    {
        //叶子节点
        if (x == m_X && y == m_Y)
        {
            m_EntitiesMap->insert(std::map<int, Entity *>::value_type(id, entity));

            entity->SetTreeNode(this);
            return true;
        }
        else
        {
            printf("Error AddEntity x=%d, y=%d m_X=%d, m_Y=%d\n Layer=%d .\n", x, y, m_X, m_Y, m_Layer);
            return false;
        }
    }
    else
    {
        //非叶子节点
        //判断是否在控制范围
        if (InRange(x, y))
        {
            if (m_Children)
            {
                return m_Children->AddEntity(entity);
            }
            else
            {
                //创建子节点
                m_Children = new NineNode(m_Layer, this, m_X, m_Y);
                return m_Children->AddEntity(entity);
            }
        }
        else
        {
            //不在控制范围
            if (m_Parent == NULL)
            {
                //增加层数
                if(m_Children != NULL)
                {
                    //有子节点创建新的根节点
                    TreeNode *newRoot = new TreeNode(m_Layer + 1, NULL, 0, 0);
                    this->SetParent(newRoot);
                    newRoot->SetChildren(this);
                    TreeNode::g_Root = newRoot;
                }
                else
                {
                    m_Layer++;

                    m_GenLayer = pow(3, m_Layer);
                }
                return TreeNode::g_Root->AddEntity(entity);
            }
            else
            {
                return m_Parent->AddEntity(entity);
            }
        }
    }

    return false;
}
/**
 * @brief TreeNode::Info
 */
void TreeNode::Info()
{
    if (m_Layer == 0)
    {
        if (m_EntitiesMap->size() > 0)
        {
            printf("%d->%d:%d .\n", m_Layer, m_X, m_Y);
            std::map<int, Entity *>::const_iterator itor = m_EntitiesMap->begin();
            for (; itor != m_EntitiesMap->end(); ++itor)
            {
                printf("id = %d .\n", itor->first);
            }
        }
    }

    if (m_Children != NULL)
    {
        m_Children->Info();
    }
}

/**
 * @brief TreeNode::getChildTreeNode
 * 获取一个子节点
 * @param x
 * @param y
 * @return
 */
TreeNode *TreeNode::getChildTreeNode(int x, int y)
{
    if(m_Children)
    {
        return m_Children->getTreeNode(x, y);
    }
    return NULL;
}

/**
 * @brief TreeNode::getEntityNumber
 * 获取实体数量
 * @param outnumb 统计的数量
 */
void TreeNode::getEntityNumber(int &outnumb)
{
    if(m_EntitiesMap != NULL)
    {
        outnumb = outnumb + m_EntitiesMap->size();
    }
    if(m_Children != NULL)
    {
        m_Children->getEntityNumber(outnumb);
    }
}

/**
 * @brief TreeNode::getMaxDensity
 * 获取叶子节点密度最大的值
 * @param outdensity
 */
void TreeNode::getMaxDensity(int &outdensity)
{
    if(m_EntitiesMap != NULL)
    {
        if(outdensity < m_EntitiesMap->size())
        {
            outdensity = m_EntitiesMap->size();
        }
    }
    if(m_Children != NULL)
    {
        m_Children->getMaxDensity(outdensity);
    }
}
