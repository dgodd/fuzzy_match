# frozen_string_literal: true

require_relative "fts_fuzzy_match/version"
require_relative "fts_fuzzy_match/fts_fuzzy_match"

# FTS Fuzzy Match module. Can score or sort
class FtsFuzzyMatch
  class Error < StandardError; end

  def initialize(...)
    @extension = ::FtsFuzzyMatch::Extension.new(...)
  end

  def fuzzy_match(pattern, string)
    @extension.fuzzy_match(pattern, string)
  end

  def sort_in_ruby(pattern, strings)
    # fuzzy_match is -50..50 so -200 is the lowest possible score
    strings.sort_by { |string| -1 * (fuzzy_match(pattern, string) || -200) }
  end

  def sort_n(pattern, strings, n)
    @extension.sort_n(pattern, strings, n)
  end

  def sort(pattern, strings)
    @extension.sort_n(pattern, strings, strings.length)
  end

  class Extension
    def initialize(**args)
      @sequential_bonus = args[:sequential_bonus]
      @separator_bonus = args[:separator_bonus]
      @camel_bonus = args[:camel_bonus]
      @first_letter_bonus = args[:first_letter_bonus]
      @leading_letter_penalty = args[:leading_letter_penalty]
      @max_leading_letter_penalty = args[:max_leading_letter_penalty]
      @unmatched_letter_penalty = args[:unmatched_letter_penalty]
      @string_length_penalty = args[:string_length_penalty]
    end
  end
end
