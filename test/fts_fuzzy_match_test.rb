# frozen_string_literal: true

require "test_helper"

class FtsFuzzyMatchTest < Minitest::Spec
  describe ".fuzzy_match" do
    it "returns a score" do
      result = ::FtsFuzzyMatch::Extension.new.fuzzy_match("got", "game of thrones")

      assert_equal(148, result)
    end

    it "returns false when no match is found" do
      result = ::FtsFuzzyMatch::Extension.new.fuzzy_match("tog", "game of thrones")

      refute(result)
    end

    # it "different number for each value" do
    #   brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_1.json')))
    #   brands = brands.sort_by { |string| -1 * (::FtsFuzzyMatch::Extension.fuzzy_match('sol', string) || -200) }.take(20)
    #   brands.each do |brand|
    #     result = ::FtsFuzzyMatch::Extension.fuzzy_match("sol", brand)
    #     puts "BRAND: #{brand} : #{result}"
    #   end
    # end
  end

  it ".sort" do
    result = ::FtsFuzzyMatch.new.sort("got", [ "getty over throne", "gameofthrones", "game of thrones", "some other words" ])

    assert_equal([ "game of thrones", "getty over throne", "gameofthrones", "some other words" ], result)
  end

  it ".sort_n" do
    result = ::FtsFuzzyMatch.new.sort_n("got", [ "getty over throne", "gameofthrones", "game of thrones", "some other words" ], 2)

    assert_equal([ "game of thrones", "getty over throne"], result)
  end
end
