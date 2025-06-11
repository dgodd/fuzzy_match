# frozen_string_literal: true

require "test_helper"

class FtsFuzzyMatchTest < Minitest::Spec
  subject { ::FtsFuzzyMatch.new(sequential_bonus: 20, camel_bonus: 0, string_length_penalty: -1) }

  describe ".fuzzy_match" do
    it "returns a score" do
      result = subject.fuzzy_match("got", "game of thrones")
      assert_equal(148, result)
    end

    it 'uses defult config' do
      subject = ::FtsFuzzyMatch.new
      result = subject.fuzzy_match("got", "game of thrones")
      assert_equal(163, result)
    end

    it "returns false when no match is found" do
      result = subject.fuzzy_match("tog", "game of thrones")

      refute(result)
    end

    # it "different number for each value" do
    #   brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_1.json')))
    #   brands = brands.sort_by { |string| -1 * (subject.fuzzy_match('sol', string) || -200) }.take(20)
    #   brands.each do |brand|
    #     result = subject.fuzzy_match("sol", brand)
    #     puts "BRAND: #{brand} : #{result}"
    #   end
    # end
  end

  it ".sort" do
    result = subject.sort("got", [ "getty over throne", "gameofthrones", "game of thrones", "some other words" ])
    assert_equal([ "game of thrones", "getty over throne", "gameofthrones", "some other words" ], result)
  end

  it ".sort_n" do
    result = subject.sort_n("got", [ "getty over throne", "gameofthrones", "game of thrones", "some other words" ], 2)
    assert_equal([ "game of thrones", "getty over throne"], result)
  end
end
