# frozen_string_literal: true

require 'test_helper'
require 'json'
require 'benchmark'

class SortDataTest < Minitest::Spec
  describe 'brands_1' do
    it '.sort' do
      brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_1.json')))
      result = ::FtsFuzzyMatch.sort('sol', brands).take(10)

      assert_equal(result, ["Sol", "The School Of Life", "Solu", "Sox & Lok", "Solo", "SOLT", "Sola", "Sole", "Solar", "Solal"])
    end

    it 'benchmark' do
      brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_1.json')))

      Benchmark.bm do |x|
        x.report("ruby") { ::FtsFuzzyMatch.sort_in_ruby('sol', brands).take(10) }
        x.report("extension") { ::FtsFuzzyMatch.sort('sol', brands).take(10) }
        x.report("sort_n") { ::FtsFuzzyMatch.sort_n('sol', brands, 10) }
      end
    end
  end

  describe 'brands_2' do
    it '.sort' do
      brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_2.json')))
      result = ::FtsFuzzyMatch.sort('sol', brands).take(10)

      assert_equal(result, ["SOUND OF LIGHT", "THE SCHOOL OF LIFE", "S. OLIVER", "SOLAC", "Solidu", "SOLTER", "SOLERA", "SOLMAR", "Soleaf", "Solgar"])
    end

    it 'benchmark' do
      brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_2.json')))

      Benchmark.bm do |x|
        x.report("ruby") { ::FtsFuzzyMatch.sort_in_ruby('sol', brands).take(10) }
        x.report("extension") { ::FtsFuzzyMatch.sort('sol', brands).take(10) }
        x.report("sort_n") { ::FtsFuzzyMatch.sort_n('sol', brands, 10) }
      end
    end
  end
end
