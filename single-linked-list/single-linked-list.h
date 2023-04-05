#pragma once
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, был доступ к приватной области итератора
        friend class SingleLinkedList;
    public:
        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = ValueType;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        // Конвертирующий конструктор итератора из указателя на узел списка
        BasicIterator() = default;
        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) { }
        explicit BasicIterator(Node* node) : node_(node) {}
        BasicIterator& operator=(const BasicIterator& rhs) = default;


        [[nodiscard]] bool operator== (const BasicIterator<const Type>& rval) const noexcept {
            return node_ == rval.node_;
        }
        [[nodiscard]] bool operator!= (const BasicIterator<const Type>& rval) const noexcept {
            return !(*this == rval);
        }
        [[nodiscard]] bool operator== (const BasicIterator<Type>& rval) const noexcept {
            return node_ == rval.node_;
        }
        [[nodiscard]] bool operator!= (const BasicIterator<Type>& rval) const noexcept {
            return !(*this == rval);
        }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }
        BasicIterator operator++(int) noexcept {
            BasicIterator copy = *this;
            ++(*this);
            return copy;
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator* () const noexcept {
            if (node_) {
                return node_->value;
            }
            static Type default_value = Type(); //либо добавить приватное поле и сделать функцию неконстантной
            return default_value;
        }


        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator-> () const noexcept {
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
        //Type default_value = Type();
        /*inline static Type default_value = Type();*/
    };



public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() : head_(Type(), nullptr), size_(0) {}

    SingleLinkedList(const SingleLinkedList& item) {
        PushValues(item.begin(), item.end());
    }

    SingleLinkedList(std::initializer_list<Type> list) {
        PushValues(list.begin(), list.end());
    }

    ~SingleLinkedList() { Clear(); }


    SingleLinkedList& operator=(const SingleLinkedList<Type>& rhs) {
        if (this != &rhs) { //(*this != rhs)  (this != &rhs)
            SingleLinkedList<Type> copy(rhs);
            this->swap(copy);
        }
        return *this;
    }

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* elem = pos.node_;
        //if (*elem != nullptr) {
        Node* elem_after = elem->next_node;
        Node* new_node = new Node(value, elem_after);
        elem->next_node = new_node;
        ++size_;
        return Iterator(new_node);
        //};
        //return Iterator(elem);
    }

    void PopFront() noexcept {
        if (head_.next_node != nullptr) {
            Node* elem = head_.next_node;
            Node* elem_after = elem->next_node;
            delete elem;
            head_.next_node = elem_after;
            --size_;
        }
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* elem = pos.node_;
        //if (*elem == nullptr) return Iterator(elem);

        Node* elem_after = elem->next_node;
        //if (*elem_after == nullptr) return Iterator(elem_after);

        Node* elem_after_after = elem_after->next_node;
        elem->next_node = elem_after_after;
        delete elem_after;
        --size_;
        return Iterator(elem_after_after);
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    // Очищает список за время O(N)
    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* node = head_.next_node;
            head_.next_node = node->next_node;
            delete node;
            --size_;
        };
    }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0 ? true : false;
    }


    bool operator== (const SingleLinkedList& rhs) {
        if (rhs.GetSize() != this->GetSize()) return false;
        return std::equal(this->begin(), this->end(), rhs.begin(), rhs.end());
    }
    bool operator!= (const SingleLinkedList& rhs) {
        return !(*this == rhs);
    }
    bool operator< (const SingleLinkedList& rhs) {
        return std::lexicographical_compare(this->cbegin(), this->cend(), rhs.cbegin(), rhs.cend());
    }
    bool operator<= (const SingleLinkedList& rhs) {
        return *this == rhs || *this < rhs;
    }

    bool operator> (const SingleLinkedList& rhs) {
        return !(*this <= rhs);
    }
    bool operator>= (const SingleLinkedList& rhs) {
        return !(*this < rhs);
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator  begin() const noexcept {
        return ConstIterator(head_.next_node);
    }
    [[nodiscard]] ConstIterator  end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(this->head_.next_node, other.head_.next_node);
        std::swap(this->size_, other.size_);
    }

private:
    template<typename iter>
    void PushValues(iter begin, iter end) {
        if (begin != end) {
            Node* last = GetLast();
            for (auto& it = begin; it != end; ++it) {
                last->next_node = new Node{ *it, nullptr };
                last = last->next_node;
                ++size_;
            }
        }
    }

    Node* GetLast() {
        Node* last = &head_;
        Node* prev = &head_;
        while (last->next_node != nullptr) { prev = last; last = last->next_node; }
        return prev;
    }

    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}