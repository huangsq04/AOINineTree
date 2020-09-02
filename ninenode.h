#ifndef NINENODE_H
#define NINENODE_H
#include "entity.h"
#include "treenode.h"
class TreeNode;
class Entity;
/**
 * @brief The NineNode class
 * 子节点控制类
 * 8(-1,1)   1(0,1)   2(1,1)
 * 7(-1,0)   0(0,0)   3(1,0)
 * 6(-1,-1)  5(0,-1)  4(1,-1)
 */
class NineNode
{
public:
    NineNode(unsigned char layer, TreeNode *treeparent, int x = 0, int y = 0);

    ~NineNode();

    /** 增加实体 */
    bool AddEntity(Entity *entity);

    /** 设置节点 */
    bool SetTreeNode(TreeNode *node);

    /** 删除节点 */
    bool deleteTreeNode(int x, int y);

    /** 获取单元格位置 */
    int GetCellIndex(unsigned short idx);

    /** 获取单元格位置 */
    int GetCellIndex(int x, int y);

    /** 通过与中心点方向获取单元格 */
    TreeNode *GetCellFromDirection(int x, int y);

    /** 发信息到Entity */
    void SentMessageToEntity(int x, int y);

    void Info();

    /** 获取一个节点 */
    TreeNode *getTreeNode(int x, int y);

    /** 获取实体数量 */
    void getEntityNumber(int &outnumb);

    /** 获取叶子节点密度最大的值 */
    void getMaxDensity(int &outdensity);

private:

    TreeNode *m_Node[9];

    TreeNode *m_TreeParent;

    int m_GenLayer;
};

#endif // NINENODE_H
