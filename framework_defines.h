#ifndef FRAMEWORK_DEFINES_H
#define FRAMEWORK_DEFINES_H

#ifdef CPP_RULES_USE_QT_FRAMEWORK

#define CPP_RULES____STRING_INCLUDE <QString>
#define CPP_RULES____STRING QString
#define CPP_RULES____CHAR QChar

#define CPP_RULES____VECTOR_INCLUDE <QList>
#define CPP_RULES____VECTOR QList

#define CPP_RULES____COUT_INCLUDE <QDebug>
#define CPP_RULES____COUT qDebug()
#define CPP_RULES____COUT_NO_SPACE CPP_RULES____COUT.nospace()
#define CPP_RULES____COUT_NO_SPACE_NO_QUOTE CPP_RULES____COUT_NO_SPACE.noquote()
#define CPP_RULES____COUT_ENDL

#define CPP_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end) (begin - input->cbegin()), (end - begin)
#define CPP_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(input,begin,end,string) input->replace(CPP_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end), string)
#define CPP_RULES____STRING_INSERT____STRING_PTR__INTEGER__STRING(input,pos,string) input->insert(pos, string)
#define CPP_RULES____STRING_SUBSTR____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END(input, begin, end) input->sliced(CPP_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end))
#define CPP_RULES____STRING_SUBSTR____STRING_PTR__POS(input, pos) input->sliced(pos)

#define CPP_RULES____COPY_STRING(str_ptr) new CPP_RULES____STRING(*str_ptr)

#else

#define CPP_RULES____STRING_INCLUDE <string>
#define CPP_RULES____STRING std::string
#define CPP_RULES____CHAR char

#define CPP_RULES____VECTOR_INCLUDE <string>
#define CPP_RULES____VECTOR std::vector

#define CPP_RULES____COUT_INCLUDE <iostream>
#define CPP_RULES____COUT std::cout
#define CPP_RULES____COUT_NO_SPACE CPP_RULES____COUT
#define CPP_RULES____COUT_NO_SPACE_NO_QUOTE CPP_RULES____COUT_NO_SPACE
#define CPP_RULES____COUT_ENDL << std::endl

#define CPP_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end) (begin - input->cbegin()), (end - begin)
#define CPP_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(input,begin,end,string) input->replace(CPP_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end), string)
#define CPP_RULES____STRING_INSERT____STRING_PTR__INTEGER__STRING(input,pos,string) input->insert(pos, string.c_str())
#define CPP_RULES____STRING_SUBSTR____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END(input, begin, end) input->substr(CPP_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end))
#define CPP_RULES____STRING_SUBSTR____STRING_PTR__POS(input, pos) input->substr(pos)

#define CPP_RULES____COPY_STRING(str_ptr) new CPP_RULES____STRING(str_ptr->c_str())

#endif

#define CPP_RULES____STRING_APPEND(var_name, str, integer) CPP_RULES____STRING var_name = ""; { auto tmp_i = integer; auto tmp_m = integer; tmp_i = 0; for(;tmp_i<tmp_m;tmp_i++) var_name += str; }

#endif // FRAMEWORK_DEFINES_H
