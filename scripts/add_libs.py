import json

lib = "libs/magnetics"
file = "pxt-microbit/pxtarget.json"
content = {}

with open(file, 'r') as f:
    content = json.loads(f.read())

if( lib not in content["bundleddirs"] ):
    content["bundleddirs"].append(lib)

    with open(file, 'w') as f:
        f.write( json.dumps(content, indent=4) )
