#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "gvpm.h"

#include "gv_eval.h"
/* In unit testing evaluate() must be invoked in immediate mode */

TEST_CASE ("SingleOperator", "alloperators") {
    float res;
    gvpm_init();

    SECTION("add") {
        res = evaluate("2.0 * 3.0", true);
        CHECK (res == Approx(6.0).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 1);
    }

    SECTION("subtract") {
        res = evaluate("3.0 - 5.0", true);
        CHECK (res == Approx(-2).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 1);
    }

    SECTION("multiply") {
        res = evaluate("3.0 * 5.2", true);
        CHECK (res == Approx(15.6).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 1);
    }

    SECTION("divide") {
        res = evaluate("3.0 / 5.0", true);
        CHECK (res == Approx(0.6).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 1);
    }
}

TEST_CASE ("Group2", "Two Operators") {
    float res;
    gvpm_init();

    SECTION ("add") {
        res = evaluate("3.0 + 5.0 + 10.0", true);
        CHECK (res == Approx(18.0).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 2);
    }

    SECTION ("subtract") {
        res = evaluate("3.0 - 5.0 - 10.0", true);
        CHECK (res == Approx(-12.0).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 2);
    }

    SECTION ("multiply") {
        res = evaluate("3.0 * 5.0 * 10.0", true);
        CHECK (res == Approx(150.0).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 2);
    }

    SECTION ("divide") {
        res = evaluate("3.0 / 5.0 / 10.0", true);
        CHECK (res == Approx(0.06).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 2);
    }

    SECTION ("test 1") {
        res = evaluate("3.0 + 5.0 * 10.0", true);
        CHECK (res == Approx(53).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 2);
    }

    SECTION ("test 2") {
        res = evaluate("3.0 / 5.0 + 10.0", true);
        CHECK (res == Approx(10.6).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 2);
    }

    SECTION ("test 3") {
        res = evaluate("3.0 + 5.0 / 10.0", true);
        CHECK (res == Approx(3.5).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 2);
    }

    SECTION ("test 4") {
        res = evaluate("100.0 / 5.0 + 34.0", true);
        CHECK (res == Approx(54).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 2);
    }

    SECTION ("test 5") {
        res = evaluate("100.0 / 8.0 * 3.0", true);
        CHECK (res == Approx(4.16666).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 2);
    }
}

TEST_CASE ("More Operators", "") {
    float res;
    gvpm_init();

    SECTION("three") {
        res = evaluate ("39 - 5 * 6 / 2", true);
        CHECK ( res == Approx (24).epsilon(0.0001));
        CHECK (gvpm_fork_count() == 3);
    }
}
