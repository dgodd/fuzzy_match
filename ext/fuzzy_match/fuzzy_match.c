#include "fuzzy_match.h"

VALUE rb_mRCEE;
VALUE rb_mFuzzyMatch;
VALUE rb_cFuzzyMatchExtension;

static VALUE
rb_fuzzy_match_extension_class_do_something(VALUE self)
{
  /* todo: perform CPU-intensive operation */
  return rb_str_new_cstr("something has been done");
}

void
Init_fuzzy_match(void)
{
  rb_mRCEE = rb_define_module("RCEE");
  rb_mFuzzyMatch = rb_define_module_under(rb_mRCEE, "FuzzyMatch");
  rb_cFuzzyMatchExtension = rb_define_class_under(rb_mFuzzyMatch, "Extension", rb_cObject);
  rb_define_singleton_method(rb_cFuzzyMatchExtension, "do_something",
                             rb_fuzzy_match_extension_class_do_something, 0);
}
