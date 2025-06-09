# frozen_string_literal: true

require_relative "fts_fuzzy_match/version"
require_relative "fts_fuzzy_match/fts_fuzzy_match"

# FTS Fuzzy Match module. Can score or sort
module FtsFuzzyMatch
  class Error < StandardError; end

  def self.sort_in_ruby(pattern, strings)
    # fuzzy_match is -50..50 so -200 is the lowest possible score
    strings.sort_by { |string| -1 * (::FtsFuzzyMatch::Extension.fuzzy_match(pattern, string) || -200) }
  end

  def self.sort_n(pattern, strings, n)
    ::FtsFuzzyMatch::Extension.sort_n(pattern, strings, n)
  end

  def self.sort(pattern, strings)
    ::FtsFuzzyMatch::Extension.sort_n(pattern, strings, strings.length)
  end
end
