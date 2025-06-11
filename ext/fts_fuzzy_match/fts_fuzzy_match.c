#include "ruby.h"
#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fts_fuzzy_match.h"

VALUE rb_cFtsFuzzyMatch;
VALUE rb_cFtsFuzzyMatchExtension;

int get_num_from_self(VALUE self, char *name, int default_value) {
  VALUE val = rb_iv_get(self, name);
  switch TYPE(val) {
      case 7: return NUM2INT(RARRAY_AREF(val, 0));
      case 21: return NUM2INT(val);
      default: return default_value;
  }
}

struct FtsConfig fts_config(VALUE self) {
    struct FtsConfig config = {
        .sequential_bonus = get_num_from_self(self, (char*)"@sequential_bonus", 15),
        .separator_bonus = get_num_from_self(self, (char*)"@separator_bonus", 30),
        .camel_bonus = get_num_from_self(self, (char*)"@camel_bonus", 30),
        .first_letter_bonus = get_num_from_self(self, (char*)"@first_letter_bonus", 15),
        .leading_letter_penalty = get_num_from_self(self, (char*)"@leading_letter_penalty", -5),
        .max_leading_letter_penalty = get_num_from_self(self, (char*)"@max_leading_letter_penalty", -15),
        .unmatched_letter_penalty = get_num_from_self(self, (char*)"@unmatched_letter_penalty", -1),
        .string_length_penalty = get_num_from_self(self, (char*)"@string_length_penalty", 0)
    };
    return config;
}

static VALUE
rb_fts_fuzzy_match_extension_class_fuzzy_match(VALUE self, VALUE pattern, VALUE str)
{
  char* patternPtr;
  patternPtr = StringValueCStr(pattern);
  char* strPtr;
  strPtr = StringValueCStr(str);

  struct FtsConfig config = fts_config(self);
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

  struct FtsConfig config = fts_config(self);

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
    rb_cFtsFuzzyMatch = rb_define_class("FtsFuzzyMatch", rb_cObject);
    rb_cFtsFuzzyMatchExtension = rb_define_class_under(rb_cFtsFuzzyMatch, "Extension", rb_cObject);
    rb_define_method(rb_cFtsFuzzyMatchExtension, "fuzzy_match",
        rb_fts_fuzzy_match_extension_class_fuzzy_match, 2);
    rb_define_method(rb_cFtsFuzzyMatchExtension, "sort_n",
        rb_fts_fuzzy_match_extension_class_sort_n, 3);
}
