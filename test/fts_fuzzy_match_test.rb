# frozen_string_literal: true

require "test_helper"

class FtsFuzzyMatchTest < Minitest::Spec
  describe ".fuzzy_match" do
    it "returns a score" do
      result = ::FtsFuzzyMatch::Extension.fuzzy_match("got", "game of thrones")

      assert_equal(148, result)
    end

    it "returns false when no match is found" do
      result = ::FtsFuzzyMatch::Extension.fuzzy_match("tog", "game of thrones")

      refute(result)
    end
  end

  it ".sort" do
    result = ::FtsFuzzyMatch.sort("got", [ "getty over throne", "gameofthrones", "game of thrones", "some other words" ])

    assert_equal([ "game of thrones", "getty over throne", "gameofthrones", "some other words" ], result)
  end

  it ".sort_n" do
    result = ::FtsFuzzyMatch.sort_n("got", [ "getty over throne", "gameofthrones", "game of thrones", "some other words" ], 2)

    assert_equal([ "game of thrones", "getty over throne"], result)
  end
end
