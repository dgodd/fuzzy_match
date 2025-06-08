# frozen_string_literal: true

require "test_helper"

class FuzzyMatchTest < Minitest::Spec
  describe "FuzzyMatch" do
    it "defines a class" do
      assert(defined?(::RCEE::FuzzyMatch::Extension))
    end

    it "defines a method" do
      result = ::RCEE::FuzzyMatch::Extension.do_something
      puts "\ndo_something returned #{result.inspect}"
      assert(result)
    end
  end
end
