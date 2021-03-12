#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include "book.hpp"
#include "utility/dataset_loader.hpp"

using namespace std;
using namespace test::utils;
using namespace Catch::Matchers;

CATCH_REGISTER_ENUM(Genre, Genre::ACTION_AND_ADVENTURE, Genre::CLASSIC, Genre::DRAMA,
                    Genre::FANTASY, Genre::SCI_FI, Genre::HORROR, Genre::ROMANCE,
                    Genre::ADULT, Genre::THRILLER, Genre::BIOGRAPHY, Genre::HISTORY,
                    Genre::POETRY, Genre::UNDEFINED)

CATCH_REGISTER_ENUM(Publisher, Publisher::USA, Publisher::RUS, Publisher::ENG, Publisher::AUS, Publisher::UNDEFINED);

SCENARIO("create book using non-default constructor") {

  GIVEN("valid constructor arguments") {
    const string title = GENERATE(from_range(load_token_samples("book_titles.txt", 3)));
    const string content = GENERATE(from_range(load_book_contents({"1.txt", "2.txt", "3.txt"}, 3)));
    const vector<Author> authors = GENERATE(load_author_samples("authors.txt", 3));

    const auto genre = static_cast<Genre>(GENERATE(range(0, static_cast<int>(Genre::UNDEFINED), 3)));
    const auto publisher = static_cast<Publisher>(GENERATE(0, static_cast<int>(Publisher::UNDEFINED), 2));

    INFO("passed constructor arguments");
    CAPTURE(title, content.empty(), genre, publisher, authors.size());

    WHEN("creating a new book") {
      THEN("no exceptions must be thrown") {
        REQUIRE_NOTHROW(Book(title, content, genre, publisher, authors));
      }

      const auto book = Book(title, content, genre, publisher, authors);

      AND_THEN("all entity fields must be correctly initialized") {
        REQUIRE(book.GetGenre() == genre);
        REQUIRE(book.GetPublisher() == publisher);
        REQUIRE_THAT(book.GetTitle(), Equals(title));
        REQUIRE_THAT(book.GetContent(), Equals(content));
        REQUIRE_THAT(book.GetAuthors(), Equals(authors));
      }
    }
  }

  AND_GIVEN("invalid constructor arguments") {

    WHEN("creating a new book with empty title") {
      const string empty_title = GENERATE(value(string{}));
      const string content = GENERATE(from_range(load_book_contents({"1.txt", "2.txt", "3.txt"}, 3)));
      const auto genre = static_cast<Genre>(GENERATE(range(0, static_cast<int>(Genre::UNDEFINED), 3)));
      const auto publisher = static_cast<Publisher>(GENERATE(0, static_cast<int>(Publisher::UNDEFINED), 2));

      const vector<Author> authors = GENERATE(load_author_samples("authors.txt", 3));

      INFO("passed constructor arguments");
      CAPTURE(empty_title, content.empty(), genre, publisher, authors.size());

      THEN("an exception must be thrown") {
        const auto matcher = StartsWith("Book::empty_title") && EndsWith("empty");
        REQUIRE_THROWS(Book(empty_title, content, genre, publisher, authors), matcher);
      }
    }

    AND_WHEN("creating a new book with empty content") {
      const string empty_content = GENERATE(value(string{}));
      const string title = GENERATE(from_range(load_token_samples("book_titles.txt", 3)));
      const vector<Author> authors = GENERATE(load_author_samples("authors.txt", 3));

      const auto genre = static_cast<Genre>(GENERATE(range(0, static_cast<int>(Genre::UNDEFINED), 3)));
      const auto publisher = static_cast<Publisher>(GENERATE(0, static_cast<int>(Publisher::UNDEFINED), 2));

      INFO("passed constructor arguments");
      CAPTURE(title, empty_content.empty(), genre, publisher, authors.size());

      THEN("an exception must be thrown") {
        const auto matcher = StartsWith("Book::empty_content") && EndsWith("empty");
        REQUIRE_THROWS(Book(title, empty_content, genre, publisher, authors), matcher);
      }
    }

    AND_WHEN("creating a new book with no authors") {
      const string content = GENERATE(from_range(load_book_contents({"1.txt", "2.txt", "3.txt"}, 3)));
      const string title = GENERATE(from_range(load_token_samples("book_titles.txt", 3)));

      const auto genre = static_cast<Genre>(GENERATE(range(0, static_cast<int>(Genre::UNDEFINED), 3)));
      const auto publisher = static_cast<Publisher>(GENERATE(0, static_cast<int>(Publisher::UNDEFINED), 2));

      const vector<Author> empty_authors;

      INFO("passed constructor arguments");
      CAPTURE(title, content.empty(), genre, publisher, empty_authors.empty());

      THEN("an exception must be thrown") {
        const auto matcher = StartsWith("Book::authors") && EndsWith("empty");
        REQUIRE_THROWS(Book(title, content, genre, publisher, empty_authors), matcher);
      }
    }
  }
}

SCENARIO("add author to the existing book") {

  GIVEN("an existing book with a single author") {
    const auto author = Author("J.K. Rowling", Author::kMinAuthorAge, Sex::FEMALE);
    auto book = Book("Harry Potter", "Contents", Genre::FANTASY, Publisher::USA, {author});

    WHEN("adding a new author with a different name") {
      Author author_other = author;
      author_other.SetFullName("Noname");

      const bool status = book.AddAuthor(author_other);

      THEN("author must be included in the authors list") {
        REQUIRE(status == true);

        const std::vector<Author> expected_authors = {author, author_other};
        REQUIRE_THAT(expected_authors, Equals(book.GetAuthors()));
      }
    }

    AND_WHEN("adding a new author with a duplicate name") {
      const bool status = book.AddAuthor(author);

      THEN("number of authors must stay the same") {
        REQUIRE_FALSE(status);

        const std::vector<Author> expected_authors = {author};
        REQUIRE_THAT(expected_authors, Equals(book.GetAuthors()));
      }
    }
  }
}

// === внутренние тесты ===

SCENARIO("change book fields using setters") {
  auto book = Book();

  GIVEN("valid setter arguments") {
    const string title = GENERATE_REF(from_range(load_token_samples("book_titles.txt", 3)));
    const string content = GENERATE_REF(from_range(load_book_contents({"1.txt", "2.txt", "3.txt"}, 3)));

    INFO("passed setter arguments");
    CAPTURE(title, content);

    WHEN("setting bok private members") {
      THEN("no exceptions must be thrown") {
        REQUIRE_NOTHROW(book.SetTitle(title));
        REQUIRE_NOTHROW(book.SetContent(content));
        REQUIRE(book.GetTitle() == title);
        REQUIRE(book.GetContent() == content);
      }
    }
  }

  AND_GIVEN("invalid setter arguments") {
    const string empty_title = GENERATE(value(std::string{}));
    const string empty_content = GENERATE(value(std::string{}));

    INFO("passed setter arguments");
    CAPTURE(empty_title, empty_content);

    WHEN("setting book private members") {
      THEN("an exception must be thrown") {
        REQUIRE_THROWS(book.SetTitle(empty_title), StartsWith("Book::title") && EndsWith("empty"));
        REQUIRE_THROWS(book.SetContent(empty_content), StartsWith("Book::content") && EndsWith("empty"));
      }
    }
  }
}

SCENARIO("compare books using operator ==") {

  GIVEN("identical book instances") {
    const string title = GENERATE_REF(from_range(load_token_samples("book_titles.txt", 3)));
    const string content = GENERATE_REF(from_range(load_book_contents({"1.txt", "2.txt", "3.txt"}, 3)));

    auto book_1 = Book();
    auto book_2 = Book();

    book_1.SetTitle(title);
    book_1.SetContent(content);

    book_2.SetTitle(title);
    book_2.SetContent(content);

    CAPTURE(book_1.GetTitle(), book_1.GetContent());
    CAPTURE(book_2.GetTitle(), book_2.GetContent());

    WHEN("comparing book objects") {
      THEN("they must be equal to each other") {
        REQUIRE(book_2 == book_2);
        REQUIRE(book_1 == book_1);
        REQUIRE(book_1 == book_2);
        REQUIRE(book_2 == book_1);
      }
    }
  }

  AND_GIVEN("non-identical book instances") {
    auto book_1 = Book();
    auto book_2 = Book();

    WHEN("setting different book titles") {
      book_1.SetTitle("Title 1");
      book_2.SetTitle("Title 2");

      CAPTURE(book_1.GetTitle(), book_1.GetContent());
      CAPTURE(book_2.GetTitle(), book_2.GetContent());

      THEN("book objects must be unequal to each other") {
        REQUIRE(book_1 != book_2);
        REQUIRE(book_2 != book_1);
      }
    }

    AND_WHEN("setting different book contents") {
      book_1.SetContent("Content 1");
      book_2.SetContent("Content 2");

      CAPTURE(book_1.GetTitle(), book_1.GetContent());
      CAPTURE(book_2.GetTitle(), book_2.GetContent());

      THEN("book objects must be unequal to each other") {
        REQUIRE(book_1 != book_2);
        REQUIRE(book_2 != book_1);
      }
    }
  }
}
