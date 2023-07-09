#include "list.h"


//�ڵ��ʼ������
void vListInitialiseItem(ListItem_t *const pxItem)
{
	pxItem->pvCountainer = NULL;	//��ָ����������
}

//���ڵ��ʼ������
void vListInitialist(List_t * const pxList)
{
	pxList->pxIndex = (ListItem_t *) & (pxList->xListEnd);		//����ָ�����һ���ڵ�
	pxList->xListEnd.xItemValue = portMAX_DELAY;//���һ���ڵ������ָ�����
	pxList->xListEnd.pxNext = (ListItem_t *) &(pxList->xListEnd);//���ڵ�pxNext��pxPreviousָ������
	pxList->xListEnd.pxPrevious = (ListItem_t *) &(pxList->xListEnd);
	pxList->uxNumberOfItem = (UBaseType_t)0U;		//���ýڵ������Ϊ0
}

//��һ���յ�������뵽ָ��ָ���λ��
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem)
{
	ListItem_t * const pxIndex = pxList->pxIndex;	//������һ���ڵ��ָ��, ת��ΪListItem����
	
	pxNewListItem->pxNext = pxIndex;		//�½ڵ����ָ��ĩβ�Ľڵ�
	pxNewListItem->pxPrevious = pxIndex->pxPrevious;	//ǰһ���ڵ�ָ��ĩβ����ǰһ���ڵ�
	pxIndex->pxPrevious->pxNext = pxNewListItem;	//֮ǰ�ĵ����ڶ����ڵ�ָ���Լ�
	pxIndex->pxPrevious = pxNewListItem;	//ĩβ���ǰһ���ڵ�ָ���Լ�
	
	(pxList->uxNumberOfItem)++;			//��һ���ڵ�ļ���ֵ��һ
}

//�������ֵĴ�С���в���
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem)
{
	ListItem_t * pxIterator;
	
	const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;	//��ȡ�ڵ������
	
	if(xValueOfInsertion == portMAX_DELAY)
	{
		pxIterator = pxList->xListEnd.pxPrevious;//������ȼ����ֱ�Ӳ嵽���
		
		
	}else
	{
		for(pxIterator = (ListItem_t *) &pxList->xListEnd;
			pxIterator->pxNext->xItemValue <= xValueOfInsertion;
		pxIterator = pxIterator->pxNext)
		{
			//��ʼΪ��һ������, ��������ֵ�Ƚ�С, �ͱ�Ϊ��һ������
			//����ֻ��Ϊ���ҵ�λ��
		
		}
	}
	//��������ֵ�ı�
	pxNewListItem->pxNext = pxIterator->pxNext;
	pxNewListItem->pxNext->pxPrevious = pxNewListItem;
	//�ı�ǰ��
	pxNewListItem->pxPrevious = pxIterator;
	pxIterator->pxNext = pxNewListItem;
	//����ýڵ����ڵ�����
	pxNewListItem->pvCountainer = (void *)pxList;

	(pxList->uxNumberOfItem)++;
}

//�Ƴ�һ������
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemov)
{
	//��ȡ����Ľڵ�
	List_t * const pxList = (List_t *)pxItemToRemov->pvCountainer;
	
	
	pxItemToRemov->pxNext->pxPrevious = pxItemToRemov->pxPrevious;
	pxItemToRemov->pxPrevious->pxNext = pxItemToRemov->pxNext;
	
	if(pxList->pxIndex == pxItemToRemov)
	{
		//���ɾ�����Ǳ���ָ��ָ��Ľڵ��������Ϊǰһ��
		pxList->pxIndex = pxItemToRemov->pxPrevious;
	}
	pxItemToRemov->pvCountainer = NULL;
	(pxList->uxNumberOfItem)--;
	
	return pxList->uxNumberOfItem;
}











