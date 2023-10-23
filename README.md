# Single-linked-list
Односвязный список
***

## Развертывание
```
g++ main.cpp single-linked-list.h -o list -std=c++17 -O3
./list
```
# Формат входных данных
```
SingleLinkedList<int> lst{ 1, 2, 3 };       Создает список нужного типа
PushFront(123);                             Добавляет элемент в начало списка
PopFront();                                 Удаляет элемент из начала списка
InsertAfter(lst.before_begin(), 123);       Добавляет элемент в определенную позицию списка
EraseAfter(lst.cbefore_begin());            Удаляет элемент из определенной позиции списка
Clear();                                    Очищает список за время O(N)
```
# Формат выходных данных
```
lst.GetSize();           Получает размер списка
```  
## Пример
```
int main() {
	SingleLinkedList<int> lst{ 1, 2, 3 };
	auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
	assert(lst.GetSize() == 4);
	auto item_after_erased = lst.EraseAfter(lst.cbefore_begin());
	assert(lst.GetSize() == 3);
}
```
