#include <catch2/catch.hpp>

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "book_store.hpp"
#include "utility/dataset_loader.hpp"

using namespace std;
using namespace test::utils;
using namespace Catch::Matchers;

SCENARIO("create bookstore using non-default constructor") {

  GIVEN("valid constructor arguments") {
    const auto name = GENERATE(values({"A", "LA", "Heavens"}));

    INFO("passed constructor arguments");
    CAPTURE(name);

    WHEN("creating a new book store") {
      THEN("no exceptions must be thrown") {
        REQUIRE_NOTHROW(BookStore(name));
      }

      const auto book_store = BookStore(name);

      AND_THEN("all entity fields must be correctly initialized") {
        REQUIRE(book_store.GetSize() == 0);
        REQUIRE(book_store.GetBooks() != nullptr);
        REQUIRE_THAT(name, Equals(book_store.GetName()));
        REQUIRE(book_store.GetCapacity() == BookStore::kInitStorageCapacity);
      }
    }
  }

  AND_GIVEN("invalid constructor arguments") {

    WHEN("creating a new book store with empty title") {
      const string empty_name = GENERATE(value(string{}));

      INFO("passed constructor arguments");
      CAPTURE(empty_name);

      THEN("an exception must be thrown") {
        REQUIRE_THROWS(BookStore(empty_name), Contains("BookStore::name") && EndsWith("empty"));
      }
    }
  }
}

SCENARIO("resize bookstore storage capacity") {

  GIVEN("nullptr storage") {
    Book *storage = nullptr;

    WHEN("resizing to a new valid capacity") {
      const int size = 0;
      const int new_capacity = GENERATE_REF(range(1, 3));

      CAPTURE(size, new_capacity, storage == nullptr);

      const ResizeStorageStatus status = resize_storage(storage, size, new_capacity);

      THEN("storage must not be initialized") {
        REQUIRE(storage == nullptr);
        REQUIRE(status == ResizeStorageStatus::NULL_STORAGE);
      }
    }
  }

  AND_GIVEN("empty storage") {
    const int init_capacity = 5;
    auto storage = new Book[init_capacity];

    WHEN("resizing to the same or smaller capacity") {
      const int size = 0;
      const int new_capacity = GENERATE_COPY(range(init_capacity, 1, -1));

      CAPTURE(init_capacity, size, new_capacity, storage == nullptr);

      const ResizeStorageStatus status = resize_storage(storage, size, new_capacity);

      THEN("storage capacity must be successfully changed") {
        REQUIRE(status == ResizeStorageStatus::SUCCESS);

        const auto book_ref = Book();

        for (int index = 0; index < new_capacity; index++) {
          REQUIRE(storage[index] == book_ref);
        }
      }
    }

    AND_WHEN("resizing to the larger capacity") {
      const int size = 0;
      const int new_capacity = GENERATE_COPY(range(init_capacity, init_capacity + 5));

      CAPTURE(init_capacity, size, new_capacity, storage == nullptr);

      const ResizeStorageStatus status = resize_storage(storage, size, new_capacity);

      THEN("storage capacity must be successfully changed") {
        REQUIRE(status == ResizeStorageStatus::SUCCESS);

        const auto book_ref = Book();

        for (int index = 0; index < init_capacity; index++) {
          REQUIRE(storage[index] == book_ref);
        }
      }
    }

    AND_WHEN("resizing to the non-positive capacity") {
      const int size = 0;
      const int new_capacity = GENERATE(range(-5, 0));

      CAPTURE(init_capacity, size, new_capacity, storage == nullptr);

      const ResizeStorageStatus status = resize_storage(storage, size, new_capacity);

      THEN("storage capacity must be insufficient") {
        REQUIRE(status == ResizeStorageStatus::INSUFFICIENT_CAPACITY);
      }
    }

    AND_WHEN("resizing to a new capacity passing negative size") {
      const int new_capacity = GENERATE(range(1, 3));
      const int incorrect_size = GENERATE(range(-5, 0));

      CAPTURE(init_capacity, incorrect_size, new_capacity, storage == nullptr);

      const ResizeStorageStatus status = resize_storage(storage, incorrect_size, new_capacity);

      THEN("storage size must be incorrect") {
        REQUIRE(status == ResizeStorageStatus::NEGATIVE_SIZE);
      }
    }

    delete[] storage;
    storage = nullptr;
  }

  AND_GIVEN("non-empty storage with books where size <= capacity") {
    const int init_capacity = GENERATE(range(2, 5));
    const int size = GENERATE_COPY(range(init_capacity, 1, -1));

    auto storage = new Book[init_capacity];
    const vector<Book> books = generate_book_samples(size);

    copy(books.data(), books.data() + size, storage);

    WHEN("resizing to a new capacity <= size") {
      const int new_capacity = GENERATE_COPY(range(size, 1, -1));

      CAPTURE(init_capacity, size, new_capacity, storage == nullptr);

      const ResizeStorageStatus status = resize_storage(storage, size, new_capacity);

      THEN("new capacity must be insufficient") {
        REQUIRE(status == ResizeStorageStatus::INSUFFICIENT_CAPACITY);
      }
    }

    AND_WHEN("resizing to a new capacity > size") {
      const int new_capacity = GENERATE_COPY(size + 1, size + 3);

      CAPTURE(init_capacity, size, new_capacity, storage == nullptr);

      const ResizeStorageStatus status = resize_storage(storage, size, new_capacity);

      THEN("resize must be successfully accomplished") {
        REQUIRE(status == ResizeStorageStatus::SUCCESS);

        for (int index = 0; index < size; index++) {
          REQUIRE(storage[index] == books[index]);
        }

        const auto book_ref = Book();

        for (int index = size; index < new_capacity; index++) {
          REQUIRE(storage[index] == book_ref);
        }
      }
    }

    delete[] storage;
    storage = nullptr;
  }
}

SCENARIO("add new books to the bookstore") {

  GIVEN("empty bookstore") {
    auto book_store = BookStore("BookStore Evil Corp.");

    WHEN("adding new books to the store without increasing capacity") {
      const int num_books = GENERATE(range(1, std::min(3, BookStore::kInitStorageCapacity)));
      const vector<Book> books = generate_book_samples(num_books);

      INFO("Before adding books");
      CAPTURE(book_store.GetCapacity(), book_store.GetSize());

      for (const auto &book: books) {
        book_store.AddBook(book);
      }

      THEN("size must be increased and capacity must be the same") {
        INFO("After adding books");
        CAPTURE(book_store.GetCapacity(), book_store.GetSize());

        REQUIRE(book_store.GetCapacity() == BookStore::kInitStorageCapacity);
        REQUIRE(book_store.GetSize() == num_books);

        const Book *store_books = book_store.GetBooks();

        for (int index = 0; index < num_books; index++) {
          REQUIRE(store_books[index] == books[index]);
        }
      }
    }

    AND_WHEN("adding new books to the store with increasing capacity") {
      const int num_books = GENERATE(range(1, 3));

      for (int index = 0; index < BookStore::kInitStorageCapacity; index++) {
        book_store.AddBook(Book{});
      }

      INFO("After filling in books storage to the max capacity");
      CAPTURE(book_store.GetCapacity(), book_store.GetSize());

      REQUIRE(book_store.GetSize() == book_store.GetCapacity());

      const vector<Book> books = generate_book_samples(num_books);

      for (const auto &book: books) {
        book_store.AddBook(book);
      }

      THEN("storage capacity and size must be correspondingly increased") {
        INFO("After adding a few more books into the storage");
        CAPTURE(book_store.GetCapacity(), book_store.GetSize());

        REQUIRE(book_store.GetCapacity() == BookStore::kInitStorageCapacity + BookStore::kCapacityCoefficient);
        REQUIRE(book_store.GetSize() == BookStore::kInitStorageCapacity + num_books);

        const Book *store_books = book_store.GetBooks();
        const Book book_ref = Book{};

        for (int index = 0; index < BookStore::kInitStorageCapacity; index++) {
          REQUIRE(store_books[index] == book_ref);
        }

        for (int index = 0; index < num_books; index++) {
          REQUIRE(store_books[BookStore::kInitStorageCapacity + index] == books[index]);
        }
      }
    }
  }
}

SCENARIO("destruction of the bookstore instance") {

  GIVEN("a bookstore with initial storage") {
    const auto book_store = BookStore("Book Store");

    WHEN("destructing the book_store") {
      book_store.~BookStore();

      THEN("capacity and size must be reset to 0") {
        REQUIRE(book_store.GetCapacity() == 0);
        REQUIRE(book_store.GetSize() == 0);
      }

      AND_THEN("storage must be deleted") {
        REQUIRE(book_store.GetBooks() == nullptr);
      }
    }
  }
}
