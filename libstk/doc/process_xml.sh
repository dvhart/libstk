#!/bin/bash
xmllint --noout --postvalid --loaddtd libstk.xml
SUCCESS=$?
if [ "$SUCCESS" = "0" ]; then
    xsltproc -o output/ htmlchunkedsheet.xsl libstk.xml;
else
    echo Libstk.xml didnt validate against docbook dtd;
fi
