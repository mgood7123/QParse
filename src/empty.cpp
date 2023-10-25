#include <minidoc.h>

int main() {
    using M = MiniDoc::MiniDoc_T;

    M m;

    m.load(nullptr);
    m.append("a\nb\nc\nd");
    m.print();
    m.seek_line(1);
    m.erase(m.cursor(), m.line_length());
    m.print();
    m.undo();
    m.print();
    m.seek_line(2);
    m.insert(m.cursor(), "cc");
    m.print();
    m.printDocument();
    m.seek_line(2);
    m.append("FOO\nBAR\n");
    m.print();
    m.printDocument();
    m.append("pie");
    m.print();
    m.printDocument();
    m.append("food\nloaf");
    m.print();
    m.printDocument();
}