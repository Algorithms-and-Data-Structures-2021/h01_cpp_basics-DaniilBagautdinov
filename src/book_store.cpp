#include "book_store.hpp"

#include <algorithm>  // copy
#include <stdexcept>  // invalid_argument

// 1. реализуйте функцию ...
ResizeStorageStatus resize_storage(Book *&storage, int size, int new_capacity) {
    // здесь мог бы быть ваш разносторонний и многогранный код ...
    // Tip 1: проведите валидацию аргументов функции
    // Tip 2: не забудьте высвободить ранее выделенную память под хранилище
    if (storage == nullptr) {
        return ResizeStorageStatus::NULL_STORAGE;
    }
    if (size < 0) {
        return ResizeStorageStatus::NEGATIVE_SIZE;
    }
    if (new_capacity <= size) {
        return ResizeStorageStatus::INSUFFICIENT_CAPACITY;
    }
    if (storage != nullptr && size >= 0 && new_capacity > size) {
        auto resized_storage = new Book[new_capacity];
        std::copy(storage, storage + size, resized_storage);
        delete[] storage;
        storage = resized_storage;
    }
    return ResizeStorageStatus::SUCCESS;
}

// 2. реализуйте конструктор ...
BookStore::BookStore(const std::string &name) : name_{name} {
    // валидация аргумента
    if (name.empty()) {
        throw std::invalid_argument("BookStore::name must not be empty");
    } else {
        name_ = name;
    }
    storage_capacity_ = kInitStorageCapacity;
    storage_ = new Book[storage_capacity_];

    // здесь мог бы быть ваш сотрясающий землю и выделяющий память код ...
}

// 3. реализуйте деструктор ...
BookStore::~BookStore() {
    // здесь мог бы быть ваш высвобождающий разум от негатива код ...
    // Tip 1: я свободен ..., словно память в куче: не забудьте обнулить указатель
    if (storage_ != nullptr) {
        delete[] storage_;
        storage_ = nullptr;
    }
    storage_capacity_ = 0;
    storage_size_ = 0;
}

// 4. реализуйте метод ...
void BookStore::AddBook(const Book &book) {
    if (storage_size_ == storage_capacity_) {
        resize_storage_internal(storage_capacity_ + kCapacityCoefficient);
        // здесь мог бы быть ваш умопомрачительный код ...
        // Tip 1: используйте функцию resize_storage_internal, задав новый размер хранилища
        // Tip 2: не забудьте обработать статус вызова функции
    }
    // Tip 3: не забудьте добавить книгу в наше бездонное хранилище ...
    storage_[storage_size_] = book;
    storage_size_ += 1;
}

// РЕАЛИЗОВАНО

const std::string &BookStore::GetName() const {
    return name_;
}

int BookStore::GetSize() const {
    return storage_size_;
}

int BookStore::GetCapacity() const {
    return storage_capacity_;
}

const Book *BookStore::GetBooks() const {
    return storage_;
}

ResizeStorageStatus BookStore::resize_storage_internal(int new_capacity) {
    // изменяем размеры хранилища с копированием старых данных в хранилище нового объема
    const ResizeStorageStatus status = resize_storage(storage_, storage_size_, new_capacity);

    // если все прошло ОК, то сохраняем новый объем хранилища в объекте
    if (status == ResizeStorageStatus::SUCCESS) {
        storage_capacity_ = new_capacity;
    }

    return status;
}