#include "fuzzy_match.h"
#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fts_fuzzy_match.h"

VALUE rb_mFuzzyMatch;
VALUE rb_cFuzzyMatchExtension;

static VALUE
rb_fuzzy_match_extension_class_fuzzy_match(VALUE self, VALUE pattern, VALUE str)
{
  char* patternPtr;
  patternPtr = StringValueCStr(pattern);
  char* strPtr;
  strPtr = StringValueCStr(str);

  int outScore;
  int matched = fts_fuzzy_match_simple(patternPtr, strPtr, &outScore);
  // return rb_sprintf("Matched: %d\nScore: %d\n", matched, outScore);
  if (matched) {
      return INT2FIX(outScore);
  } else {
      return Qfalse;
  }
}

void
Init_fuzzy_match(void)
{
    rb_mFuzzyMatch = rb_define_module("FuzzyMatch");
    rb_cFuzzyMatchExtension = rb_define_class_under(rb_mFuzzyMatch, "Extension", rb_cObject);
    rb_define_singleton_method(rb_cFuzzyMatchExtension, "fuzzy_match",
        rb_fuzzy_match_extension_class_fuzzy_match, 2);
}
