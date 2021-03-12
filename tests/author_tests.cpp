#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include "author.hpp"
#include "utility/dataset_loader.hpp"

using namespace std;
using namespace test::utils;
using namespace Catch::Matchers;

CATCH_REGISTER_ENUM(Sex, Sex::MALE, Sex::FEMALE, Sex::UNDEFINED)

TEST_CASE("create author using non-default constructor") {

  GIVEN("valid constructor arguments") {
    const string full_name = GENERATE_REF(from_range(load_token_samples("author_names.txt", 3)));
    const auto sex = static_cast<Sex>(GENERATE(range(0, static_cast<int>(Sex::UNDEFINED))));
    const int age = GENERATE(range(Author::kMinAuthorAge, Author::kMinAuthorAge + 500, 50));

    INFO("passed constructor arguments");
    CAPTURE(full_name, age, sex);

    WHEN("creating a new author") {
      THEN("no exceptions must be thrown") {
        REQUIRE_NOTHROW(Author(full_name, age, sex));
      }

      const auto author = Author(full_name, age, sex);

      AND_THEN("all entity fields must be correctly initialized") {
        REQUIRE_THAT(author.GetFullName(), Equals(full_name));
        REQUIRE(author.GetAge() == age);
        REQUIRE(author.GetSex() == sex);
      }
    }
  }

  AND_GIVEN("invalid constructor arguments") {

    WHEN("creating a new author with empty full name") {
      const string empty_full_name = GENERATE(value(std::string{}));
      const auto sex = static_cast<Sex>(GENERATE(range(0, static_cast<int>(Sex::UNDEFINED))));
      const int age = GENERATE(values({Author::kMinAuthorAge, Author::kMinAuthorAge + 1, 50, 100}));

      INFO("passed constructor arguments");
      CAPTURE(empty_full_name, age, sex);

      THEN("an exception must be thrown") {
        REQUIRE_THROWS(Author(empty_full_name, age, sex), StartsWith("Author::full_name") && EndsWith("empty"));
      }
    }

    AND_WHEN("creating a new author with age lesser than " << Author::kMinAuthorAge) {
      const string full_name = GENERATE(as < string > {}, "J.K. Rowling", "S. King");
      const auto sex = static_cast<Sex>(GENERATE(range(0, static_cast<int>(Sex::UNDEFINED))));
      const int invalid_age = GENERATE(range(Author::kMinAuthorAge - 1, Author::kMinAuthorAge - 5, -1));

      INFO("passed constructor arguments");
      CAPTURE(full_name, invalid_age, sex);

      THEN("an exception must be thrown") {
        const auto matcher = StartsWith("Author::age") && EndsWith(to_string(Author::kMinAuthorAge));
        REQUIRE_THROWS(Author(full_name, invalid_age, sex), matcher);
      }
    }
  }
}

// === внутренние тесты ===

SCENARIO("change author fields using setters") {
  auto author = Author();

  GIVEN("valid setter arguments") {
    const string full_name = GENERATE_REF(from_range(load_token_samples("author_names.txt", 3)));
    const int age = GENERATE(range(Author::kMinAuthorAge, Author::kMinAuthorAge + 500, 50));

    INFO("passed setter arguments");
    CAPTURE(full_name, age);

    WHEN("setting author private members") {
      THEN("no exceptions must be thrown") {
        REQUIRE_NOTHROW(author.SetAge(age));
        REQUIRE_NOTHROW(author.SetFullName(full_name));
        REQUIRE(author.GetAge() == age);
        REQUIRE_THAT(author.GetFullName(), Equals(full_name));
      }
    }
  }

  AND_GIVEN("invalid setter arguments") {
    const string empty_full_name = GENERATE(value(std::string{}));
    const int invalid_age = GENERATE(range(Author::kMinAuthorAge - 10, Author::kMinAuthorAge - 1));

    INFO("passed setter arguments");
    CAPTURE(empty_full_name, invalid_age);

    WHEN("setting author private members") {
      THEN("an exception must be thrown") {
        const auto age_matcher = StartsWith("Author::age") && EndsWith(to_string(Author::kMinAuthorAge));
        REQUIRE_THROWS(author.SetAge(invalid_age), age_matcher);
        REQUIRE_THROWS(author.SetFullName(empty_full_name), StartsWith("Author::age") && EndsWith("empty"));
      }
    }
  }
}

SCENARIO("compare authors using operator ==") {

  GIVEN("identical author instances") {
    const string full_name = GENERATE_REF(from_range(load_token_samples("author_names.txt", 3)));
    const auto sex = static_cast<Sex>(GENERATE(range(0, static_cast<int>(Sex::UNDEFINED))));
    const int age = GENERATE(range(Author::kMinAuthorAge, Author::kMinAuthorAge + 500, 50));

    const auto author_1 = Author(full_name, age, sex);
    const auto author_2 = Author(full_name, age, sex);

    CAPTURE(author_1.GetFullName(), author_1.GetAge(), author_1.GetSex());
    CAPTURE(author_2.GetFullName(), author_2.GetAge(), author_2.GetSex());

    WHEN("comparing author objects") {
      THEN("they must be equal to each other") {
        REQUIRE(author_2 == author_2);
        REQUIRE(author_1 == author_1);
        REQUIRE(author_1 == author_2);
        REQUIRE(author_2 == author_1);
      }
    }
  }

  AND_GIVEN("non-identical author instances") {
    auto author_1 = Author();
    auto author_2 = Author();

    WHEN("setting different author full names") {
      author_1.SetFullName("Author 1");
      author_2.SetFullName("Author 2");

      CAPTURE(author_1.GetFullName(), author_1.GetAge(), author_1.GetSex());
      CAPTURE(author_2.GetFullName(), author_2.GetAge(), author_2.GetSex());

      THEN("author objects must be unequal to each other") {
        REQUIRE(author_1 != author_2);
        REQUIRE(author_2 != author_1);
      }
    }

    AND_WHEN("setting different author ages") {
      author_1.SetAge(Author::kMinAuthorAge);
      author_2.SetAge(author_1.GetAge() + 1);

      CAPTURE(author_1.GetFullName(), author_1.GetAge(), author_1.GetSex());
      CAPTURE(author_2.GetFullName(), author_2.GetAge(), author_2.GetSex());

      THEN("author objects must be unequal to each other") {
        REQUIRE(author_1 != author_2);
        REQUIRE(author_2 != author_1);
      }
    }

    AND_WHEN("setting different author sex") {
      author_1.SetSex(Sex::MALE);
      author_2.SetSex(Sex::FEMALE);

      CAPTURE(author_1.GetFullName(), author_1.GetAge(), author_1.GetSex());
      CAPTURE(author_2.GetFullName(), author_2.GetAge(), author_2.GetSex());

      THEN("author objects must be unequal to each other") {
        REQUIRE(author_1 != author_2);
        REQUIRE(author_2 != author_1);
      }
    }
  }
}