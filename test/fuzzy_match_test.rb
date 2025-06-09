# frozen_string_literal: true

require 'test_helper'

class FuzzyMatchTest < Minitest::Spec
  describe 'FuzzyMatch::Extension' do
    it 'defines a class' do
      assert(defined?(::FuzzyMatch::Extension))
    end

    describe '.fuzzy_match' do
      it 'returns a score' do
        result = ::FuzzyMatch::Extension.fuzzy_match('got', 'game of thrones')
        assert_equal(result, 163)
      end

      it 'returns false when no match is found' do
        result = ::FuzzyMatch::Extension.fuzzy_match('tog', 'game of thrones')
        assert_equal(result, false)
      end
    end
  end

  describe 'FuzzyMatch' do
    it '.sort' do
      result = ::FuzzyMatch.sort('got', ['getty over throne', 'gameofthrones', 'game of thrones', 'some other words'])
      assert_equal(result, ['game of thrones', 'getty over throne', 'gameofthrones', 'some other words'])
    end
  end
end
