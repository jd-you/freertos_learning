#ifndef LIST_H
#define LIST_H
#include "portmacro.h"

struct xLIST_ITEM
{
    TickType_t xItemValue; // 任务优先级？
    struct xLIST_ITEM *pxNext;
    struct xLIST_ITEM *pxPrevious;
    void *pvOwner;
    void *pvContainer;
};
typedef struct xLIST_ITEM ListItem_t;

struct xMINI_LIST_ITEM
{
    TickType_t xItemValue;
    ListItem_t *pxNext;
    ListItem_t *pxPrevious;
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;

struct xLIST
{
    UBaseType_t uxNumberOfItems;
    ListItem_t *pxIndex;
    MiniListItem_t xListEnd;
};
typedef struct xLIST List_t;

/* 设置节点的拥有者 */
#define listSET_LIST_ITEM_OWNER(pxListItem, pxOwner) \
    ((pxListItem)->pvOwner = (void *)(pxOwner))

/* 获取节点的拥有者 */
#define listGET_LIST_ITEM_OWNER(pxListItem) \
    ((pxListItem)->pvOwner)

/* 设置节点排序辅助值 */
#define listSET_LIST_ITEM_VALUE(pxListItem, xValue) \
    ((pxListItem)->xItemValue = (xValue))

/* 获取节点排序辅助值 */
#define listGET_LIST_ITEM_VALUE(pxListItem) \
    ((pxListItem)->xItemValue)

/* 获取链表根节点的节点计数器值 */
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY(pxList) \
    (((pxList)->xListEnd).pxNext->xItemValue)

/* 获取链表的入口节点 */
#define listGET_HEAD_ENTRY(pxList) \
    (((pxList)->xListEnd).pxNext)

/* 获取节点的下一个节点 */
#define listGET_NEXT(pxListItem) \
    ((pxListItem)->pxNext)

/* 获取链表的最后一个节点 */
#define listGET_END_MARKER(pxList) \
    ((ListItem_t const *)(&((pxList)->xListEnd)))

/* 判断链表是否为空 */
#define listLIST_IS_EMPTY(pxList) \
    ((BaseType_t)((pxList)->uxNumberOfItems == (BaseType_t)0))

/* 获取链表的节点数 */
#define listCURRENT_LIST_LENGTH(pxList) \
    ((pxList)->uxNumberOfItems)

/* 获取第一个节点的owner */
#define listGET_OWNER_OF_NEXT_ENTRY(pxTCB, pxList)                             \
    {                                                                          \
        List_t *const pxConstList = (pxList);                                  \
        (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;               \
        if ((void *)(pxConstList)->pxIndex == (void *)(&((pxList)->xListEnd))) \
        {                                                                      \
            (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;           \
        }                                                                      \
        (pxTCB) = (pxConstLIst)->pxIndex->pvOwner;                             \
    }

#define listGET_OWNER_OF_HEAD_ENTRY( pxList )            ( ( &( ( pxList )->xListEnd ) )->pxNext->pvOwner )
// 链表节点初始化
extern void vListInitialiseItem(ListItem_t *const pxItem);
// 链表初始化
extern void vListInitialise(List_t *const pxList);
// 将节点插入到链表的尾部
extern void vListInsertEnd(List_t *const pxList, ListItem_t *const pxNewListItem);
// 将节点按照升序排列插入到链表
extern void vListInsert(List_t *const pxList, ListItem_t *const pxNewListItem);
// 将节点从链表删除
extern UBaseType_t uxListRemove(ListItem_t *const pxItemToRemove);
#endif
