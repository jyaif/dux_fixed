#!/bin/sh

find ./ -iname *.h -o -iname *.cpp | xargs clang-format -i -style=Chromium -sort-includes