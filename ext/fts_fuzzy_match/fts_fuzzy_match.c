#include "ruby.h"
#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fts_fuzzy_match.h"

VALUE rb_mFtsFuzzyMatch;
VALUE rb_cFtsFuzzyMatchExtension;

static VALUE
rb_fts_fuzzy_match_extension_class_fuzzy_match(VALUE self, VALUE pattern, VALUE str)
{
  char* patternPtr;
  patternPtr = StringValueCStr(pattern);
  char* strPtr;
  strPtr = StringValueCStr(str);

  struct FtsConfig config = fts_default_config();
  int outScore;
  int matched = fts_fuzzy_match_simple(patternPtr, strPtr, &config, &outScore);
  // return rb_sprintf("Matched: %d\nScore: %d\n", matched, outScore);
  if (matched) {
      return INT2FIX(outScore);
  } else {
      return Qfalse;
  }
}

struct StringScore {
  VALUE rbStr;
  char *cStr;
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
  if (aa->score != bb->score) {
    return bb->score - aa->score;
  } else {
      // Given the score is the same, sort alphabetically to keep the order consistent
      return strcasecmp(aa->cStr, bb->cStr);
  }
}

static VALUE
rb_fts_fuzzy_match_extension_class_sort_n(VALUE self, VALUE pattern, VALUE strings, VALUE n)
{
  char* patternPtr;
  patternPtr = StringValueCStr(pattern);
  long stringsLen = RARRAY_LEN(strings);

  struct FtsConfig config = fts_default_config();

  struct StringScore *scores = (struct StringScore *)malloc(stringsLen * sizeof(struct StringScore));
  for (long i=0; i<stringsLen; i++) {
    volatile VALUE str = RARRAY_AREF(strings, i);
    scores[i].rbStr = str;
    scores[i].cStr = StringValueCStr(str);
    scores[i].matched = fts_fuzzy_match_simple(patternPtr, scores[i].cStr, &config, &scores[i].score);
  }

  qsort(scores, stringsLen, sizeof(struct StringScore), comp);

  long n2 = NUM2INT(n);
  if (n2 > stringsLen) n2 = stringsLen;

  VALUE result = rb_ary_new_capa(n2);
  for (long i=0; i<n2; i++) {
    rb_ary_push(result, scores[i].rbStr);
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
