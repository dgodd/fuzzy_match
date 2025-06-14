# frozen_string_literal: true

require_relative "lib/fts_fuzzy_match/version"

Gem::Specification.new do |spec|
  spec.name          = "fts_fuzzy_match"
  spec.version       = FtsFuzzyMatch::VERSION
  spec.authors       = ["Dave Goddard"]
  spec.email         = ["dave@goddard.au"]

  spec.summary       = "Gem to use FTS for fuzzy matching."
  spec.description   = "Gem to use FTS for fuzzy matching."
  spec.homepage      = "https://github.com/dgodd/fuzzy_match"
  spec.required_ruby_version = ">= 3.3.0"
  spec.license = "MIT"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(File.expand_path(__dir__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{\A(?:test|spec|features)/}) }
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
  spec.extensions    = ["ext/fts_fuzzy_match/extconf.rb"]

  # Uncomment to register a new dependency of your gem
  # spec.add_dependency "example-gem", "~> 1.0"

  # For more information and examples about making a new gem, checkout our
  # guide at: https://bundler.io/guides/creating_gem.html
end
