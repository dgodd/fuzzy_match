# frozen_string_literal: true

require 'test_helper'
require 'json'
require 'benchmark'

class SortDataTest < Minitest::Spec
  subject { ::FtsFuzzyMatch.new(sequential_bonus: 20, camel_bonus: 0, string_length_penalty: -1) }

  describe 'brands_1' do
    it '.sort' do
      brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_1.json')))
      result = subject.sort('sol', brands).take(10)

      assert_equal(result, ["Sol", "The School Of Life", "Sola", "Sole", "Solo", "SOLT", "Solu", "Sox & Lok", "Solac", "Solal"])
    end

    # it 'benchmark' do
    #   brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_1.json')))

    #   Benchmark.bm do |x|
    #     x.report("ruby") { subject.sort_in_ruby('sol', brands).take(10) }
    #     x.report("extension") { subject.sort('sol', brands).take(10) }
    #     x.report("sort_n") { subject.sort_n('sol', brands, 10) }
    #   end
    # end
  end

  describe 'brands_2' do
    it '.sort' do
      brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_2.json')))
      result = subject.sort('sol', brands).take(10)

      assert_equal(result, ["SOUND OF LIGHT", "THE SCHOOL OF LIFE", "S. OLIVER", "SOLAC", "Soleaf", "SOLERA", "Solgar", "Solidu", "SOLMAR", "SOLTER"])
    end

    # it 'benchmark' do
    #   brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_2.json')))

    #   Benchmark.bm do |x|
    #     x.report("ruby") { subject.sort_in_ruby('sol', brands).take(10) }
    #     x.report("extension") { subject.sort('sol', brands).take(10) }
    #     x.report("sort_n") { subject.sort_n('sol', brands, 10) }
    #   end
    # end
  end
end
