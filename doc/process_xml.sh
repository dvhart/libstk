#!/bin/bash
xmllint --noout --postvalid --loaddtd libstk.xml
SUCCESS=$?
if [ "$SUCCESS" = "0" ]; then
    xsltproc -o output/ htmlchunkedsheet.xsl libstk.xml;
    dot dot_files/widget_tree_example.dot -o images/widget_tree_example.png -Tpng
else
    echo Libstk.xml didnt validate against docbook dtd;
fi
