Doubly Linked List - Code is self explainatory.
Revision V0.1

API:
bool_t ConstuctLinkedList(); 
bool_t AddSorted(cell_t * p_cell);
bool_t RemoveCell(uint32_t value);
bool_t ReverseList(cell_t * p_cell_start);
bool_t FindCell(uint32_t value, cell_t ** p_found_cell);
void PrintLinkedList(cell_t * p_cell);
