#ifndef __LIST_H
#define __LIST_H
#include "FreeRTOS.h"

struct xLIST_ITEM
{
	TickType_t	xItemValue;				//����ֵ
	struct xLIST_ITEM * pxNext;			//ָ����һ���ڵ�
	struct xLIST_ITEM * pxPrevious;		//ָ����һ���ڵ�
	void * pvOwner;				//ָ��ӵ�иýڵ���ں˶���
	void * pvCountainer;		//ָ��ڵ����ڵ�����
};
typedef struct xLIST_ITEM ListItem_t;

//��β�ڵ�
struct xMINI_LIST_ITEM
{
	TickType_t	xItemValue;				//����ֵ, �����ڵ���������
	struct xLIST_ITEM * pxNext;			//ָ����һ���ڵ�
	struct xLIST_ITEM * pxPrevious;		//ָ����һ���ڵ�
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;

//��ʼ���ʼ������
typedef struct xLIST
{
	UBaseType_t uxNumberOfItem;		//�ڵ������
	ListItem_t	* pxIndex;			//����ڵ�����,������������
	MiniListItem_t xListEnd;		//���һ���ڵ�, ���ﴴ������һ��ʵ�ʵ�����, ����ָ��
}List_t;

//��ʼ���ڵ�ӵ����
#define liseSET_LIST_ITEM_OWNER(pxListItem, pxOwner) 	((pxListItem->pvOwner)=(void *)(pxOwner))
//��ýڵ��ӵ����
#define listGET_ITEM_ITEM_OWNER(pxListItem)  			(pxListItem->pvOwner)
//��ʼ���ڵ��������ֵ
#define liseSET_LIST_ITEM_VALUE(pxListItem, xValue) 	((pxListItem)->xItemValue)=xValue
//��ȡ������ڵ�ļ�����ֵ
#define liseGET_LIST_ITEM_VALUE(pxListItem)				(pxListItem->xItemValue)
//��ȡ�������ڽڵ��������ֵ
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY(pxList) 		(((pxList)->xListEnd).pxNext->xItemValue)
//��ȡ��ڽڵ�
#define listGET_HEAD_ENTRY(pxList)						( ( ( pxList )->xListEnd ).pxNext )
//��ȡ��һ���ڵ�
#define listGET_NEXT(pxListItem) 						( ( pxListItem )->pxNext )
//��ȡ���һ���ڵ�
#define listGET_END_MARKER(pxList) 						((ListItem_t const *)(&((pxList)->xListEnd)))
//�����Ƿ�Ϊ��
#define listLIST_IS_EMPTY( pxList ) 					((BaseType_t)((pxList)->uxNumberOfItems==(UBaseType_t)0))
//��ȡ������
#define listCURRENT_LIST_LENGTH( pxList )  				( ( pxList )->uxNumberOfItems )
/* ��ȡ����ڵ��OWNER����TCB */
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )										\
{																							\
	List_t * const pxConstList = ( pxList );											    \
	/* �ڵ�����ָ�������һ���ڵ�����ڵ�����ָ�룬ָ����һ���ڵ㣬
    �����ǰ������N���ڵ㣬����N�ε��øú���ʱ��pxInedex��ָ���N���ڵ� */\
	( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;							\
	/* ��ǰ����Ϊ�� */                                                                       \
	if( ( void * ) ( pxConstList )->pxIndex == ( void * ) &( ( pxConstList )->xListEnd ) )	\
	{																						\
		( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;						\
	}																						\
	/* ��ȡ�ڵ��OWNER����TCB */                                                             \
	( pxTCB ) = ( pxConstList )->pxIndex->pvOwner;											 \
}
#define listGET_OWNER_OF_HEAD_ENTRY( pxList )  ( (&( ( pxList )->xListEnd ))->pxNext->pvOwner )



void vListInitialiseItem(ListItem_t *const pxItem);
void vListInitialist(List_t * const pxList);
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem);
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem);
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemov);
#endif




