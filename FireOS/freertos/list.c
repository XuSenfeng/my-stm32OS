#include "list.h"


//节点初始化函数
void vListInitialiseItem(ListItem_t *const pxItem)
{
	pxItem->pvCountainer = NULL;	//不指向任意链表
}

//根节点初始化函数
void vListInitialist(List_t * const pxList)
{
	pxList->pxIndex = (ListItem_t *) & (pxList->xListEnd);		//索引指向最后一个节点
	pxList->xListEnd.xItemValue = portMAX_DELAY;//最后一个节点的数字指向最大
	pxList->xListEnd.pxNext = (ListItem_t *) &(pxList->xListEnd);//最后节点pxNext和pxPrevious指向自身
	pxList->xListEnd.pxPrevious = (ListItem_t *) &(pxList->xListEnd);
	pxList->uxNumberOfItem = (UBaseType_t)0U;		//设置节点的数量为0
}

//把一个空的链表插入到指针指向的位置
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem)
{
	ListItem_t * const pxIndex = pxList->pxIndex;	//获得最后一个节点的指针, 转换为ListItem类型
	
	pxNewListItem->pxNext = pxIndex;		//新节点最后指向末尾的节点
	pxNewListItem->pxPrevious = pxIndex->pxPrevious;	//前一个节点指向末尾结点的前一个节点
	pxIndex->pxPrevious->pxNext = pxNewListItem;	//之前的倒数第二个节点指向自己
	pxIndex->pxPrevious = pxNewListItem;	//末尾结点前一个节点指向自己
	
	(pxList->uxNumberOfItem)++;			//第一个节点的计数值加一
}

//按照数字的大小进行插入
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem)
{
	ListItem_t * pxIterator;
	
	const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;	//获取节点的数字
	
	if(xValueOfInsertion == portMAX_DELAY)
	{
		pxIterator = pxList->xListEnd.pxPrevious;//如果优先级最低直接插到最后
		
		
	}else
	{
		for(pxIterator = (ListItem_t *) &pxList->xListEnd;
			pxIterator->pxNext->xItemValue <= xValueOfInsertion;
		pxIterator = pxIterator->pxNext)
		{
			//初始为第一个链表, 如果链表的值比较小, 就变为下一个链表
			//这里只是为了找到位置
		
		}
	}
	//插入后面的值改变
	pxNewListItem->pxNext = pxIterator->pxNext;
	pxNewListItem->pxNext->pxPrevious = pxNewListItem;
	//改变前面
	pxNewListItem->pxPrevious = pxIterator;
	pxIterator->pxNext = pxNewListItem;
	//保存该节点所在的链表
	pxNewListItem->pvCountainer = (void *)pxList;

	(pxList->uxNumberOfItem)++;
}

//移除一个链表
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemov)
{
	//获取链表的节点
	List_t * const pxList = (List_t *)pxItemToRemov->pvCountainer;
	
	
	pxItemToRemov->pxNext->pxPrevious = pxItemToRemov->pxPrevious;
	pxItemToRemov->pxPrevious->pxNext = pxItemToRemov->pxNext;
	
	if(pxList->pxIndex == pxItemToRemov)
	{
		//如果删除的是遍历指针指向的节点把他设置为前一个
		pxList->pxIndex = pxItemToRemov->pxPrevious;
	}
	pxItemToRemov->pvCountainer = NULL;
	(pxList->uxNumberOfItem)--;
	
	return pxList->uxNumberOfItem;
}











