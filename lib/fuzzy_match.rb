# frozen_string_literal: true

require_relative "fuzzy_match/version"
require_relative "fuzzy_match/fuzzy_match"

# FTS Fuzzy Match module. Can score or sort
module FuzzyMatch
  class Error < StandardError; end

  def self.sort_in_ruby(pattern, strings)
    # fuzzy_match is -50..50 so -200 is the lowest possible score
    strings.sort_by { |string| -1 * (::FuzzyMatch::Extension.fuzzy_match(pattern, string) || -200) }
  end

  def self.sort_n(pattern, strings, n)
    ::FuzzyMatch::Extension.sort_n(pattern, strings, n)
  end

  def self.sort(pattern, strings)
    ::FuzzyMatch::Extension.sort_n(pattern, strings, strings.length)
  end
end
