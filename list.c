#include "list.h"

// 链表节点初始化
void vListInitialiseItem(ListItem_t * const pxItem)
{
    pxItem->pvContainer = NULL;
}

// 链表初始化
void vListInitialise(List_t *const pxList)
{
    pxList->pxIndex = (ListItem_t *)&(pxList->xListEnd);
    pxList->xListEnd.xItemValue = portMAX_DELAY;
    pxList->xListEnd.pxNext = (ListItem_t *)&(pxList->xListEnd);
    pxList->xListEnd.pxPrevious = (ListItem_t *)&(pxList->xListEnd);

    pxList->uxNumberOfItems = (UBaseType_t) 0u;
}

// 将节点插入到链表的尾部
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem)
{
    ListItem_t *const pxIndex = pxList->pxIndex;
    
    pxNewListItem->pxNext = pxIndex;
    pxNewListItem->pxPrevious = pxIndex->pxPrevious;
    pxIndex->pxPrevious->pxNext = pxNewListItem;
    pxIndex->pxPrevious = pxNewListItem;
    
    pxNewListItem->pvContainer = (void *)pxList;
    pxList->uxNumberOfItems += 1u;
}

// 将节点插入到链表的头部
void vListInsertBegin(List_t *const pxList, ListItem_t * const pxNewListItem)
{
    ListItem_t *const pxIndex = pxList->pxIndex;

    pxNewListItem->pxNext = pxIndex->pxNext;
    pxNewListItem->pxPrevious = pxIndex;
    pxIndex->pxNext->pxPrevious = pxNewListItem;
    pxIndex->pxNext = pxNewListItem;

    pxNewListItem->pvContainer = (void *)pxList;
    pxList->uxNumberOfItems += 1u;    
}

// 将节点按照升序排列插入到链表
void vListInsert(List_t * const pxList, ListItem_t *const pxNewListItem)
{
    ListItem_t *pxIterator;
    
    const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;

    if (xValueOfInsertion == portMAX_DELAY) {
        pxIterator = pxList->xListEnd.pxPrevious;
    } else {
        for (pxIterator = (ListItem_t *)&(pxList->xListEnd);
                pxIterator->pxNext->xItemValue <= xValueOfInsertion;
                pxIterator = pxIterator->pxNext) {

                }
    }

    pxNewListItem->pxNext = pxIterator->pxNext;
    pxNewListItem->pxPrevious = pxIterator;
    pxIterator->pxNext->pxPrevious = pxNewListItem;
    pxIterator->pxNext = pxNewListItem;

    pxNewListItem->pvContainer = (void *)pxList;

    pxList->uxNumberOfItems++;
}

// 将节点从链表删除
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemove)
{
    List_t * const pxList = (List_t *)pxItemToRemove->pvContainer;
    pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;
    pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;

    if (pxList->pxIndex == pxItemToRemove) {
        pxList->pxIndex = pxItemToRemove->pxPrevious;
    }

    pxItemToRemove->pvContainer = NULL;
    
    pxList->uxNumberOfItems--;
    return pxList->uxNumberOfItems;
}