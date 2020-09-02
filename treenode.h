#ifndef TREENODE_H
#define TREENODE_H
#include "entity.h"
#include "ninenode.h"
#include <map>
class Entity;
class NineNode;
/**
 * @brief The TreeNode class
 * 九宫格的每个节点
 */
class TreeNode
{
public:
    TreeNode(unsigned char layer = 1, TreeNode *parent = NULL, int x = 0, int y = 0);

    ~TreeNode();
    /** 增加实体 */
    bool AddEntity(Entity *entity);

    /** 删除实体 */
    bool DeleteEntity(Entity *entity);

    /** 移动实体 */
    bool MoveEntity(Entity *entity);

    /** 发信息到周边entity */
    void SendMessageAroundEntities(int dist);

    /** 发信息到Entity */
    void SentMessageToEntity(int x, int y);

    void Info();

    unsigned char GetLayer() { return m_Layer; }

    int GetX() { return m_X; }
    int GetY() { return m_Y; }

    /** 设置父节点 */
    void SetParent(TreeNode *parent) { m_Parent = parent; }
    /** 获取父节点 */
    TreeNode *getParent() { return m_Parent; }

    void SetChildren(NineNode *child) { m_Children = child; }

    /** 是否在当前节点的控制范围 */
    bool InRange(int x, int y);

    /** 设置子节点 */
    bool SetChildren(TreeNode *node);

    NineNode *getChildren() { return m_Children; }

    /** 获取一个子节点 */
    TreeNode *getChildTreeNode(int x, int y);

    /** 获取实体数量 */
    void getEntityNumber(int &outnumb);

    /** 获取叶子节点密度最大的值 */
    void getMaxDensity(int &outdensity);

    /** 获取一个子节点的实体列表 */
    std::map<int, Entity *> *getEntitiesMap() { return m_EntitiesMap; }

    static TreeNode *g_Root;

private:

    /** 节点属于那层 0是叶子 */
    unsigned char m_Layer;

    /** 父节点 */
    TreeNode *m_Parent = NULL;

    /** 子节点 */
    NineNode *m_Children = NULL;

    /** 节点的实体列表 */
    std::map<int, Entity *> *m_EntitiesMap = NULL;

    /** 中心点坐标 */
    int m_X, m_Y;

    int m_GenLayer;
};

#endif // TREENODE_H
