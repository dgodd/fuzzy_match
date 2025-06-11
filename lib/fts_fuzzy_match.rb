# frozen_string_literal: true

require_relative "fts_fuzzy_match/version"
require_relative "fts_fuzzy_match/fts_fuzzy_match"

# FTS Fuzzy Match module. Can score or sort
class FtsFuzzyMatch
  class Error < StandardError; end

  def initialize
    @extension = ::FtsFuzzyMatch::Extension.new
  end

  def sort_in_ruby(pattern, strings)
    # fuzzy_match is -50..50 so -200 is the lowest possible score
    strings.sort_by { |string| -1 * (@extension.fuzzy_match(pattern, string) || -200) }
  end

  def sort_n(pattern, strings, n)
    @extension.sort_n(pattern, strings, n)
  end

  def sort(pattern, strings)
    @extension.sort_n(pattern, strings, strings.length)
  end

  class Extension
    def initialize
      @sequential_bonus = 20
      @camel_bonus = 0
      @string_length_penalty = -1
    end
  end
end
