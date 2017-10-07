import sys
import os
import re

top = sys.argv[1]
name = sys.argv[2]
target = sys.argv[3]
helps = []

print ("Generating" + target)

writeFile = open(target, 'w')

def writeTo(line):
    writeFile.write(line + "  \n")

def findField(line, name):
    md = re.search(r'\{\"' + name + '\",\s*QVariant\((.*)\)\}', line)
    return md.group(1) if md else None

def toStringParam(p):
    md = re.match(r'QStringList\(\{(.*)\}\)', p)
    return md.group(1) if md else p

def unquote(n):
    md = re.match(r'\"(.*)\"$', n)
    return md.group(1) if md  else n

def outPut(o):
    params =  toStringParam(o['parameters'])
    writeTo("###\t" + o['name'].lower().capitalize())
    if len(o['definition']) > 0:
        writeTo("<code>" + o['definition'] + "</code>")
    writeTo("<b>Parameters:</b>\t" + (params if len(params) > 0 else 'None'))
    writeTo("<b>Return:</b>\t" + (o['returnValue'] if len(o['returnValue']) > 0 else 'None'))
    writeTo(o['comment'] if len(o['comment']) > 0 else 'Documentation missing')
    writeTo ("\n")



for root, dirs, files in os.walk(top):
    for file in files:
        if file == name:
            helps.append(os.path.join(root, name))

objs = []

for help in helps:
    with open(help, 'r') as f:
        obj = {'definition': '', 'parameters': '', 'returnValue': '', 'comment': ''}
        for line in f:
            for k in obj:
                md = findField(line, k)
                if md:
                    obj[k] = unquote(md)
            name = re.search(r'\{\"(.+)/(.+)\",\s*\{', line)
            if name:
                if 'name' in obj:
                    objs.append(obj)
                    obj = {'definition': '', 'parameters': '', 'returnValue': '', 'comment': ''}
                subsys = name.group(1)
                fname = name.group(2)
                obj['sys'] = subsys
                obj['name'] = fname
        if 'name' in obj:
            objs.append(obj)

objs.sort(key=lambda k: k['name'])
objs.sort(key=lambda k: k['sys'])

writeTo ("# Quts API")

compareCore = lambda oo: re.match('CORE', oo['sys'], re.IGNORECASE)

writeTo("<!--Generated file, please do not edit--!>")
writeTo("")
writeTo("Quts core functions")

toLinkName = lambda oo : oo['sys'].lower() + oo['name'].lower()

for o in objs:
    l =  o['name'].lower()
    if compareCore(o):
        writeTo('[' + l.capitalize() + '][' + toLinkName(o) + ']')
    else:
        writeTo('[' + o['sys'] + '.' + l.capitalize() + '][' + toLinkName(o) + ']')

for o in objs:
    if compareCore(o):
        writeTo('[' + toLinkName(o) + ']:#' + toLinkName(o))
        outPut(o)

currentAPI = None
for o in objs:
    if currentAPI != o['sys']:
        currentAPI = o['sys']
        writeTo("##" + o['sys'].lower().capitalize())
    if not compareCore(o):
        writeTo('[' + toLinkName(o) + ']:#' + toLinkName(o))
        outPut(o)


writeFile.close()
