#include "list.h"

List_t gxList;


int main()
{
    vListInitialise(&gxList);

    ListItem_t xItem1;
    ListItem_t xItem2;
    ListItem_t xItem3;

    vListInitialiseItem(&xItem1);
    vListInitialiseItem(&xItem2);
    vListInitialiseItem(&xItem3);

    listSET_LIST_ITEM_VALUE(&xItem1, 1);
    listSET_LIST_ITEM_VALUE(&xItem2, 2);
    listSET_LIST_ITEM_VALUE(&xItem3, 3);

    vListInsertEnd(&gxList, &xItem2);
    vListInsertEnd(&gxList, &xItem3);
    vListInsertEnd(&gxList, &xItem1);

    while (1) {
        
    }
}