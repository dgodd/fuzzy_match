# frozen_string_literal: true

require "test_helper"

class FuzzyMatchTest < Minitest::Spec
  describe ".fuzzy_match" do
    it "returns a score" do
      result = ::FuzzyMatch::Extension.fuzzy_match("got", "game of thrones")

      assert_equal(163, result)
    end

    it "returns false when no match is found" do
      result = ::FuzzyMatch::Extension.fuzzy_match("tog", "game of thrones")

      refute(result)
    end
  end

  it ".sort" do
    result = ::FuzzyMatch.sort("got", [ "getty over throne", "gameofthrones", "game of thrones", "some other words" ])

    assert_equal([ "game of thrones", "getty over throne", "gameofthrones", "some other words" ], result)
  end

  it ".sort_n" do
    result = ::FuzzyMatch.sort_n("got", [ "getty over throne", "gameofthrones", "game of thrones", "some other words" ], 2)

    assert_equal([ "game of thrones", "getty over throne"], result)
  end
end
