#include "entity.h"

std::map<int, Entity *> Entity::g_entities;

Entity::Entity()
{
    m_SpeedX = (int(pow( -1, rand() ) * rand())) % 2;
    m_SpeedY = (int(pow( -1, rand() ) * rand())) % 2;
    m_VisibleRange = 1;
}

/**
 * @brief Entity::createEntity
 * 静态创建Entity
 * @return
 */
int Entity::createEntity(float x, float y)
{
    Entity *entity = new Entity;
    entity->SetID(Entity::g_entities.size() + 1);
    float px = x;
    float py = y;
    entity->SetPosition(px, py);
    TreeNode::g_Root->AddEntity(entity);
    Entity::g_entities.insert(std::map<int, Entity *>::value_type(entity->GetID(), entity));
    return Entity::g_entities.size();
}
int Entity::GetID()
{
    return m_ID;
}
int Entity::GetNodeX()
{
    return m_NodeX;
}
int Entity::GetNodeY()
{
    return m_NodeY;
}
int Entity::GetX()
{
    return m_X;
}
int Entity::GetY()
{
    return m_Y;
}
void Entity::SetID(int id)
{
    m_ID = id;
}
void Entity::SetNodeX(int x)
{
    m_NodeX = x;
}
void Entity::SetNodeY(int y)
{
    m_NodeY = y;
}

void Entity::SetPosition(float x, float y)
{
    m_X = x;
    m_Y = y;
    SetNodeX(round(x/100));
    SetNodeY(round(y/100));
}

void Entity::GetPosition(float &x, float &y)
{
    x = m_X;
    y = m_Y;
}

void Entity::Info()
{
    printf("%d:(%d,%d)(%f:%f)\n", m_ID, m_NodeX, m_NodeY, m_X, m_Y);
}

/**
 * @brief Entity::randMove
 * 随机移动
 */
void Entity::randMove()
{
    if(rand()%10 == 0)
    {
        m_SpeedX = (int(pow( -1, rand() ) * rand())) % 2;
        m_SpeedY = (int(pow( -1, rand() ) * rand())) % 2;
    }

    m_X = m_X + m_SpeedX;
    m_Y = m_Y + m_SpeedY;

    m_NodeX = round(m_X/100);
    m_NodeY = round(m_Y/100);
    m_Node->MoveEntity(this);
}

/**
 * @brief Entity::getVisibleRange
 * 获取看见范围
 */
int Entity::getVisibleRange()
{
    return m_VisibleRange;
}
