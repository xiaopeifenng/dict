#ifndef _DICT_H_
#define _DICT_H_

#define MAXWORD 25
#define WORDDATA 6

struct word{
  unsigned int data[WORDDATA];
  unsigned int hashval;
  unsigned int index;
  float nice;
};

unsigned long BKDRhash(char *str);
int wordcmp(struct word *w1, struct word *w2);
inline struct word * string2word(struct word *w, char *s);
struct word * fgetword(struct word *w, FILE *ifp);
void sprintword(char *s, struct word *w);


#endif   /* _DICT_H_ */
