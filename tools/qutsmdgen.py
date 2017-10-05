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
    writeFile.write(line + "\n")


def findField(line, name):
    md = re.search(r'\{\"' + name + '\",\s*QVariant\((.*)\)\}', line)
    return md.group(1) if md else None

def toStringParam(p):
    m = re.match(r'QStringList\(\{(.*)\}\)', p)
    return m.group(1)


def outPut(o):
    writeTo("###\t" + o['name'].lower().capitalize())
    writeTo("<code>" + o['definition'] + "</code>")
    writeTo("Parameters:\t" + toStringParam(o['parameters']))
    writeTo("Return:\t" + o['returnValue'] if len(o['returnValue']) > 0 else 'None')
    writeTo("Comments:\t" + o['comment'] if len(o['comment']) > 0 else 'Not available')
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
                    obj[k] = md
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

for o in objs:
    if o['sys'] == 'CORE':
        outPut(o)

currentAPI = None
for o in objs:
    if currentAPI != o['sys']:
        currentAPI = o['sys']
        writeTo("##" + o['sys'].lower().capitalize())
    if o['sys'] != 'CORE':
        outPut(o)


writeFile.close()
