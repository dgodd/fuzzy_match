# frozen_string_literal: true

require 'test_helper'
require 'json'
require 'benchmark'

class SortDataTest < Minitest::Spec
  describe 'brands_1' do
    it '.sort' do
      brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_1.json')))
      result = ::FuzzyMatch.sort('sol', brands).take(10)

      assert_equal(result, ["FaSoLa", "SoL Cups", "The School Of Life", "Silence of the Lambs", "Small Fibre Optic Lamp", "Sox & Lok", "EcoSouLife", "Santa Fe Ole", "Soft Landing", "Somatik Labs"])
    end

    it 'benchmark' do
      brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_1.json')))

      Benchmark.bm do |x|
        x.report("ruby") { ::FuzzyMatch.sort_in_ruby('sol', brands).take(10) }
        x.report("extension") { ::FuzzyMatch.sort('sol', brands).take(10) }
        x.report("sort_n") { ::FuzzyMatch.sort_n('sol', brands, 10) }
      end
    end
  end

  describe 'brands_2' do
    it '.sort' do
      brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_2.json')))
      result = ::FuzzyMatch.sort('sol', brands).take(10)

      assert_equal(result, ["SOUND OF LIGHT", "THE SCHOOL OF LIFE", "THE SCHOOL OF LIFE PRESS", "S. OLIVER", "THE STATIONERY OFFICE LTD", "SOPA DE LETRAS", "SOCIETY OF BIBLICAL LITERATURE", "SOCCER BOOKS LTD", "Sophie La Girafe", "SOUND VOLUMES LLC"])
    end

    it 'benchmark' do
      brands = JSON.parse(File.read(File.join(__dir__, 'data', 'brands_2.json')))

      Benchmark.bm do |x|
        x.report("ruby") { ::FuzzyMatch.sort_in_ruby('sol', brands).take(10) }
        x.report("extension") { ::FuzzyMatch.sort('sol', brands).take(10) }
        x.report("sort_n") { ::FuzzyMatch.sort_n('sol', brands, 10) }
      end
    end
  end
end
