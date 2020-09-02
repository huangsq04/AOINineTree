#ifndef ENTITY_H
#define ENTITY_H
#include "treenode.h"
#include <stdio.h>
#include <map>
class Entity;
class TreeNode;
/**
 * @brief The Entity class
 * 可移动实例
 */
class Entity
{
public:
    Entity();

    TreeNode *GetTreeNode() { return m_Node; }

    void SetTreeNode(TreeNode *node) { m_Node = node; }

    int GetID();
    int GetNodeX();
    int GetNodeY();
    int GetX();
    int GetY();
    void SetID(int id);
    void SetNodeX(int x);
    void SetNodeY(int y);
    void Info();

    void SetPosition(float x, float y);
    void GetPosition(float &x, float &y);

    /** 随机移动 */
    void randMove();

    int getVisibleRange();

    static std::map<int, Entity *> g_entities;

    /** 创建Entity */
    static int createEntity(float x = 0.0, float y = 0.0);

private:
    int m_ID;

    float m_X;

    float m_Y;

    float m_SpeedX;

    float m_SpeedY;

    int m_NodeX;

    int m_NodeY;

    //所在节点
    TreeNode *m_Node;

    //看见范围
    int m_VisibleRange;
};

#endif // ENTITY_H
