# This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt 
import sys
import os
import re

def esc(s):
    s = s.replace('\\', '\\\\')
    s = s.replace('"', '\\"')
    s = s.replace('\'', '\\\'')
    return s;


target = os.path.basename(sys.argv[1])
sources = sys.argv[2:]
path = os.path.dirname(sys.argv[1])

content = []

assert len(sources) > 0, "No input files in " + str(sys.argv)

for source in sources:
    with open(source, "r") as f:
        inComment = False
        funcName = None
        subName = None
        bnf = ""
        param = []
        retVal = ""
        comment = []
        lineNo = 0
        for line in f:
            lineNo = lineNo + 1
            if re.match(r'^/\*\*DOC', line):
                inComment = True
            elif inComment:
                m = re.match(r'^\s*\*\s*N\s+(.*)', line)
                if m:
                    funcName = m.group(1)
                    continue
                b = re.match(r'^\s*\*\s*B\s+(.*)', line)
                if b:
                    bnf = b.group(1)
                    continue
                b = re.match(r'^\s*\*\s*S\s+(.*)', line)
                if b:
                    subName = b.group(1)
                    continue
                p = re.match(r'^\s*\*\s*P\s+(.*)', line)
                if p:
                    param.append(p.group(1))
                    continue
                r = re.match(r'^\s*\*\s*R\s+(.*)', line)
                if r:
                    retVal = r.group(1)
                    continue
                c = re.match(r'\s*\*\s*\*/', line)
                if c:
                    inComment = False
                    assert funcName is not None, "Invalid comment in " + source + " above line " + str(lineNo)
                    assert subName is not None, "Invalid comment in " + source + " above line " + str(lineNo)
                    content.append({
                        'name':funcName,
                        'bnf': bnf,
                        'param': param,
                        'retVal': retVal,
                        'subName': subName,
                        'comment': comment})
                    funcName = None
                    bnf = ""
                    # subName = "", Not reset as last can be ok, room for lazy!
                    param = []
                    retVal = ""
                    comment = []
                else:
                    cc = re.match(r'\s*\*\s*(.*)', line)
                    if cc:
                        comment.append(cc.group(1))
                    else:
                        comment.append(line)


print("Generating " + os.path.abspath(os.path.join(path, target)))
#with sys.stdout as f:
with open(os.path.join(path, target), "w") as f:
    f.write("#ifndef " + target.replace(".", "_").upper() + "\n")
    f.write("#define " + target.replace(".", "_").upper() + "\n")
    f.write("#include<QHash>\n");
    f.write("#include<QMap>\n");
    f.write("#include<QVariant>\n");
    f.write(3 * "\n")
    f.write("/**Generated file, please do not edit**/\n")
    f.write(3 * "\n")
    f.write("namespace Quts{\n" )
    f.write("const QHash<QString, QMap<QString, QVariant>> qutsDoc = {\n")
    for idx, e in enumerate(content):
        f.write("\t{\"" + esc(e['subName']).upper() + '/' + e['name'].upper() + "\", {\n")
        f.write("\t\t{\"definition\", QVariant(\"" +  esc(e['bnf'])  + "\")},\n")
        f.write("\t\t{\"parameters\", QVariant(QStringList({" + ",".join("\"" + esc(item) + "\"" for item in e['param']) + "}))}, \n")
        f.write("\t\t{\"returnValue\", QVariant(\"" +  esc(e['retVal'])  + "\")},\n")
      #  f.write("\t\t{\"subName\", QVariant(\"" +  esc(e['subName'])  + "\")},\n")
        f.write("\t\t{\"comment\", QVariant(\"" + " ".join([esc(c) for c in e['comment']]) + "\")}\n")
        f.write("\t}}")
        if idx != len(content) - 1:
            f.write(",\n")
        else:
            f.write("\n");
    f.write("};\n")
    f.write("}//namespace Quts\n");
    f.write(3 * "\n")
    f.write('#ifndef QUTSLIB\n')
    f.write('extern "C" void* QUTS_EXPORT_FUNCTION HELP_FUNCTION() {return new QHash<QString, QMap<QString, QVariant>>(Quts::qutsDoc);}\n');
    f.write('#endif\n\n\n')
    f.write("#endif\n")


