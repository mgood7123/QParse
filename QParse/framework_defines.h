#ifndef FRAMEWORK_DEFINES_H
#define FRAMEWORK_DEFINES_H

#ifdef QParse_RULES_USE_QT_FRAMEWORK

#define QParse_RULES____STRING_INCLUDE <QString>
#define QParse_RULES____STRING QString
#define QParse_RULES____CHAR QChar

#define QParse_RULES____VECTOR_INCLUDE <QList>
#define QParse_RULES____VECTOR QList

#define QParse_RULES____COUT_INCLUDE <QDebug>
#define QParse_RULES____COUT qDebug()
#define QParse_RULES____COUT_NO_SPACE QParse_RULES____COUT.nospace()
#define QParse_RULES____COUT_NO_SPACE_NO_QUOTE QParse_RULES____COUT_NO_SPACE.noquote()
#define QParse_RULES____COUT_ENDL

#define QParse_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end) (begin - input->cbegin()), (end - begin)
#define QParse_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(input,begin,end,string) input->replace(QParse_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end), string)
#define QParse_RULES____STRING_INSERT____STRING_PTR__INTEGER__STRING(input,pos,string) input->insert(pos, string)
#define QParse_RULES____STRING_SUBSTR____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END(input, begin, end) input->sliced(QParse_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end))
#define QParse_RULES____STRING_SUBSTR____STRING_PTR__POS(input, pos) input->sliced(pos)

#define QParse_RULES____COPY_STRING(str_ptr) new QParse_RULES____STRING(*str_ptr)

#else

#define QParse_RULES____STRING_INCLUDE <string>
#define QParse_RULES____STRING std::string
#define QParse_RULES____CHAR char

#define QParse_RULES____VECTOR_INCLUDE <vector>
#define QParse_RULES____VECTOR std::vector

#define QParse_RULES____COUT_INCLUDE <iostream>
#define QParse_RULES____COUT std::cout
#define QParse_RULES____COUT_NO_SPACE QParse_RULES____COUT
#define QParse_RULES____COUT_NO_SPACE_NO_QUOTE QParse_RULES____COUT_NO_SPACE
#define QParse_RULES____COUT_ENDL << std::endl

#define QParse_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end) (begin - input->cbegin()), (end - begin)
#define QParse_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(input,begin,end,string) input->replace(QParse_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end), string)
#define QParse_RULES____STRING_INSERT____STRING_PTR__INTEGER__STRING(input,pos,string) input->insert(pos, string.c_str())
#define QParse_RULES____STRING_SUBSTR____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END(input, begin, end) input->substr(QParse_RULES____TRANSFORM_ITERATOR_PAIR(input, begin, end))
#define QParse_RULES____STRING_SUBSTR____STRING_PTR__POS(input, pos) input->substr(pos)

#define QParse_RULES____COPY_STRING(str_ptr) new QParse_RULES____STRING(str_ptr->c_str())

#endif

#define QParse_RULES____STRING_APPEND(var_name, str, integer) QParse_RULES____STRING var_name = ""; { auto tmp_i = integer; auto tmp_m = integer; tmp_i = 0; for(;tmp_i<tmp_m;tmp_i++) var_name += str; }

#endif // FRAMEWORK_DEFINES_H
