# frozen_string_literal: true

require_relative "fuzzy_match/version"
require_relative "fuzzy_match/fuzzy_match"

module FuzzyMatch
  class Error < StandardError; end

  def self.fuzzy_match(pattern, string)
    ::FuzzyMatch::Extension.fuzzy_match(pattern, string)
  end
end
