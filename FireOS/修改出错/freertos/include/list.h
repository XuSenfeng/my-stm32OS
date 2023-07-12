#ifndef __LIST_H
#define __LIST_H
#include "FreeRTOS.h"

struct xLIST_ITEM
{
	TickType_t	xItemValue;				//辅助值
	struct xLIST_ITEM * pxNext;			//指向下一个节点
	struct xLIST_ITEM * pxPrevious;		//指向上一个节点
	void * pvOwner;				//指向拥有该节点的内核对象
	void * pvCountainer;		//指向节点所在的链表
};
typedef struct xLIST_ITEM ListItem_t;

//结尾节点
struct xMINI_LIST_ITEM
{
	TickType_t	xItemValue;				//辅助值, 帮助节点升序排列
	struct xLIST_ITEM * pxNext;			//指向上一个节点
	struct xLIST_ITEM * pxPrevious;		//指向下一个节点
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;

//初始化最开始的链表
typedef struct xLIST
{
	UBaseType_t uxNumberOfItem;		//节点计数器
	ListItem_t	* pxIndex;			//链表节点索引,用来遍历链表
	MiniListItem_t xListEnd;		//最后一个节点, 这里创建的是一个实际的类型, 不是指针
}List_t;

/*
************************************************************************
*                                宏定义
************************************************************************
*/
/* 初始化节点的拥有者 */
#define listSET_LIST_ITEM_OWNER( pxListItem, pxOwner )		( ( pxListItem )->pvOwner = ( void * ) ( pxOwner ) )
/* 获取节点拥有者 */
#define listGET_LIST_ITEM_OWNER( pxListItem )	( ( pxListItem )->pvOwner )

/* 初始化节点排序辅助值 */
#define listSET_LIST_ITEM_VALUE( pxListItem, xValue )	( ( pxListItem )->xItemValue = ( xValue ) )

/* 获取节点排序辅助值 */
#define listGET_LIST_ITEM_VALUE( pxListItem )	( ( pxListItem )->xItemValue )

/* 获取链表根节点的节点计数器的值 */
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxList )	( ( ( pxList )->xListEnd ).pxNext->xItemValue )

/* 获取链表的入口节点 */
#define listGET_HEAD_ENTRY( pxList )	( ( ( pxList )->xListEnd ).pxNext )

/* 获取链表的第一个节点 */
#define listGET_NEXT( pxListItem )	( ( pxListItem )->pxNext )

/* 获取链表的最后一个节点 */
#define listGET_END_MARKER( pxList )	( ( ListItem_t const * ) ( &( ( pxList )->xListEnd ) ) )

/* 判断链表是否为空 */
#define listLIST_IS_EMPTY( pxList )	( ( BaseType_t ) ( ( pxList )->uxNumberOfItem == ( UBaseType_t ) 0 ) )

/* 获取链表的节点数 */
#define listCURRENT_LIST_LENGTH( pxList )	( ( pxList )->uxNumberOfItems )

/* 获取链表节点的OWNER，即TCB */
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )										\
{																							\
	List_t * const pxConstList = ( pxList );											    \
	/* 节点索引指向链表第一个节点调整节点索引指针，指向下一个节点，
    如果当前链表有N个节点，当第N次调用该函数时，pxInedex则指向第N个节点 */\
	( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;							\
	/* 当前链表为空 */                                                                       \
	if( ( void * ) ( pxConstList )->pxIndex == ( void * ) &( ( pxConstList )->xListEnd ) )	\
	{																						\
		( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;						\
	}																						\
	/* 获取节点的OWNER，即TCB */                                                             \
	( pxTCB ) = ( pxConstList )->pxIndex->pvOwner;											 \
}

#define listGET_OWNER_OF_HEAD_ENTRY( pxList )  ( (&( ( pxList )->xListEnd ))->pxNext->pvOwner )




void vListInitialiseItem(ListItem_t *const pxItem);
void vListInitialist(List_t * const pxList);
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem);
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem);
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemov);

#endif




