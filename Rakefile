# frozen_string_literal: true

require "bundler/gem_tasks"
require "rubygems/package_task"
require "rake/testtask"
require "rake/extensiontask"

fuzzy_match_spec = Bundler.load_gemspec("fuzzy_match.gemspec")
Gem::PackageTask.new(fuzzy_match_spec).define

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/*_test.rb"]
end

Rake::ExtensionTask.new("fuzzy_match") do |ext|
  ext.lib_dir = "lib/rcee/fuzzy_match"
end

task default: [:clobber, :compile, :test]

CLEAN.add("{ext,lib}/**/*.{o,so}", "pkg")
