#include "fts_fuzzy_match.h"
#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fts_fuzzy_match_impl.h"

VALUE rb_mFtsFuzzyMatch;
VALUE rb_cFtsFuzzyMatchExtension;

static VALUE
rb_fts_fuzzy_match_extension_class_fuzzy_match(VALUE self, VALUE pattern, VALUE str)
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

struct StringScore {
  VALUE str;
  bool matched;
  int score;
};

int comp(const void *a, const void *b) {
  const struct StringScore *aa = (const struct StringScore *)a;
  const struct StringScore *bb = (const struct StringScore *)b;
  if (!aa->matched && !bb->matched) {
    return 0;
  } else if (aa->matched && !bb->matched) {
    return -1;
  } else if (!aa->matched && bb->matched) {
    return 1;
  }
  return bb->score - aa->score;
}

static VALUE
rb_fts_fuzzy_match_extension_class_sort_n(VALUE self, VALUE pattern, VALUE strings, VALUE n)
{
  char* patternPtr;
  patternPtr = StringValueCStr(pattern);
  long stringsLen = RARRAY_LEN(strings);

  struct StringScore *scores = (struct StringScore *)malloc(stringsLen * sizeof(struct StringScore));
  for (long i=0; i<stringsLen; i++) {
    const VALUE str = RARRAY_AREF(strings, i);
    const char* strPtr = StringValueCStr(str);
    scores[i].str = str;
    scores[i].matched = fts_fuzzy_match_simple(patternPtr, strPtr, &scores[i].score);
  }

  qsort(scores, stringsLen, sizeof(struct StringScore), comp);

  long n2 = NUM2INT(n);
  if (n2 > stringsLen) n2 = stringsLen;

  VALUE result = rb_ary_new_capa(n2);
  for (long i=0; i<n2; i++) {
    rb_ary_push(result, scores[i].str);
  }

  return result;
}

void
Init_fts_fuzzy_match(void)
{
    rb_mFtsFuzzyMatch = rb_define_module("FtsFuzzyMatch");
    rb_cFtsFuzzyMatchExtension = rb_define_class_under(rb_mFtsFuzzyMatch, "Extension", rb_cObject);
    rb_define_singleton_method(rb_cFtsFuzzyMatchExtension, "fuzzy_match",
        rb_fts_fuzzy_match_extension_class_fuzzy_match, 2);
    rb_define_singleton_method(rb_cFtsFuzzyMatchExtension, "sort_n",
        rb_fts_fuzzy_match_extension_class_sort_n, 3);
}
