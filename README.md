# FtsFuzzyMatch

This will return a match score from a pattern and string. These scores are only
useful for sorting against each other.

## Installation

Add this line to your application's Gemfile:

## Usage

To find the score for a pattern and string, use the `fuzzy_match` method:

```ruby
subject = ::FtsFuzzyMatch.new
result = subject.fuzzy_match("got", "game of thrones")
```

To sort strings based on their match scores, use the `sort` method:

```ruby
subject = ::FtsFuzzyMatch.new(sequential_bonus: 20, camel_bonus: 0, string_length_penalty: -1)
sorted_strings = subject.sort("got", ["game of thrones", "got", "winter is coming"])
```

To sort strings based on their match scores, returning only the top 2 results, use the `sort` method:

```ruby
subject = ::FtsFuzzyMatch.new
sorted_strings = subject.sort_n("got", ["game of thrones", "got", "winter is coming"], 2)
```

## Credits

- This gem was started by using the Ruby C Extensions Explained project at
  https://github.com/flavorjones/ruby-c-extensions-explained
- This gem used the fts fuzzy_match library from
  https://github.com/forrestthewoods/lib_fts/tree/master
