# frozen_string_literal: true

require "test_helper"

class FuzzyMatchTest < Minitest::Spec
  describe "FuzzyMatch" do
    it "defines a class" do
      assert(defined?(::FuzzyMatch::Extension))
    end

    it "returns a score" do
      result = ::FuzzyMatch.fuzzy_match("got", "game of thrones")
      assert_equal(result, 163)
    end

    it "returns false when no match is found" do
      result = ::FuzzyMatch.fuzzy_match("tog", "game of thrones")
      assert_equal(result, false)
    end
  end
end
