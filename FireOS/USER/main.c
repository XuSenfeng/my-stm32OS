#include "list.h"

List_t List_Test;
ListItem_t ListItem1;
ListItem_t ListItem2;
ListItem_t ListItem3;


int main(void)
{
	vListInitialist(&List_Test);
	vListInitialiseItem(&ListItem1);
	liseSET_LIST_ITEM_VALUE(&ListItem1, 1);
	
	vListInitialiseItem(&ListItem2);
	liseSET_LIST_ITEM_VALUE(&ListItem2, 2);	

	vListInitialiseItem(&ListItem3);
	liseSET_LIST_ITEM_VALUE(&ListItem3, 3);	
	
	
	vListInsert(&List_Test, &ListItem1);
	vListInsert(&List_Test, &ListItem2);
	vListInsert(&List_Test, &ListItem3);
	
	for(;;)
	{
		/*¿Õº¯Êý*/
	}
}


