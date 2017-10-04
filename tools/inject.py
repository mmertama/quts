# This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt 
import sys
import os

inject = sys.argv[1]
start = sys.argv[2]

templates = dict()

for i in range (3, len(sys.argv)):
    param = sys.argv[i]
    pos = param.index(',')
    template = param[(pos + 1):]
    templates['.' + param[:pos]] = template.replace('$', inject)

for root, dirs, files in os.walk(start):
    for file in files:
        fullname = os.path.join(root, file)
        base, ext = os.path.splitext(file)
        if ext in templates:
            line = templates[ext]
            f = open(fullname, 'r')
            content = f.readlines()
            f.close()
            if len(content) == 0:
                print("warning: empty file " + fullname)
            if len(content) > 0 and line not in content[0]:
                with open(fullname, 'w') as f:
                    f.write(line + '\n' + "".join(content))


