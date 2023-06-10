import CppHeaderParser
import os

LUA_BIND_TAG = "LUA_BIND"

parsed_files = {}
directories = ["include/app", "include/core", "include/game"]

for d in directories:
    for root, ds, fs in os.walk(d):
        for f in fs:
            fullname = os.path.join(root, f)
            parsed_files[fullname] = CppHeaderParser.CppHeader(fullname)

for file in parsed_files.values():
    for (name, value) in file.classes.items():
        idx = name.find(LUA_BIND_TAG)
        if idx != -1:
            print("component class: ", name[idx + len(LUA_BIND_TAG) :])

            public_properties = value['properties']['public']
            for property in public_properties:
                print(property['type'], property['name'])

            public_functions = value['methods']['public']
            for func in public_functions:
                print(func['rtnType'], func['name'])
                for param in func['parameters']:
                    print(param['type'], ":", param['name'])

