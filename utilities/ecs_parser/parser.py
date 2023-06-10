import CppHeaderParser
import os

# TODO: change method/property name from aaBbCc to aa_bb_cc
# TODO: regist static method/property
# TODO: when regist function, return type/parameter type should add namespace prefix
# TODO: more human-readable lua hint
# TODO: generate ResourcesWrapper, CommandWrapper, EventWrapper, QuerierWrapper codes

LUA_BIND_TAG = "LUA_BIND"
LUA_BIND_COMPONENT_TAG = "LUA_BIND_COMPONENT"
LUA_BIND_RESOURCE_TAG = "LUA_BIND_RESOURCE"
LUA_NOBIND_TAG = "LUA_NOBIND"

class LuaBindType:
    NORMAL = 0
    COMPONENT = 1
    RESOURCES = 2

class ClassInfo:
    def __init__(self, name, namespace, properties, methods, bind_type):
        self.name = name
        self.properties = properties
        self.methods: dict[str, list] = {}
        self.namespace = namespace
        for method in methods:
            method_name = method['name'];
            if method_name in self.methods.keys():
                self.methods[method_name].append(method)
            else:
                self.methods[method_name] = [method]
        self.bind_type = bind_type

def parse_all_files(directories):
    parsed_files = {}
    for d in directories:
        for root, ds, fs in os.walk(d):
            for f in fs:
                fullname = os.path.join(root, f)
                parsed_files[fullname] = CppHeaderParser.CppHeader(fullname)
    return parsed_files

def create_classinfos(parsed_files):
    classinfo_table = [{}, {}, {}]
    for file in parsed_files.values():
        for (name, value) in file.classes.items():
            bind_type = None
            idx = name.find(LUA_BIND_TAG)
            true_name = None
            if idx != -1:
                true_name = name[idx + len(LUA_BIND_TAG) :]
                bind_type = LuaBindType.NORMAL

            idx = name.find(LUA_BIND_COMPONENT_TAG)
            if idx != -1:
                true_name = name[idx + len(LUA_BIND_COMPONENT_TAG) :]
                bind_type = LuaBindType.COMPONENT

            idx = name.find(LUA_BIND_RESOURCE_TAG)
            if idx != -1:
                true_name = name[idx + len(LUA_BIND_RESOURCE_TAG) :]
                bind_type = LuaBindType.RESOURCES

            if true_name:
                public_properties = value['properties']['public']
                public_functions = value['methods']['public']

                info = ClassInfo(true_name, value['namespace'], public_properties, public_functions, bind_type)
                classinfo_table[bind_type][info.name] = info
    return classinfo_table

def regist_type(name: str, info: ClassInfo):
    namespace = info.namespace
    lower_name = (namespace if namespace is not None else "") + name.lower()
    name_without_namespace = name
    name = '{}::{}'.format(namespace, name)
    cpp_code = "sol::usertype<{}> {} = script.lua.new_usertype<{}>(\"{}\");\n".format(name, lower_name, name, name_without_namespace)
    lua_comment = "---@class {}\n".format(name)
    # bind properties
    for prop in info.properties:
        prop_name = prop['name']
        if prop['static'] or prop_name.find(LUA_NOBIND_TAG) != -1:  # remove all static properties
            continue
        new_code = "{}[\"{}\"] = &{}::{};".format(lower_name, prop_name, name, prop_name)
        new_comment = "---@field {} {}".format(prop['name'], prop['type'])
        cpp_code += new_code + '\n'
        lua_comment += new_comment + '\n'
    # bind methods
    for method_name, method in info.methods.items():
        method_name = method[0]['name']
        # TODO: regist static function
        if method_name == name_without_namespace or method[0]['rtnType'].find(LUA_NOBIND_TAG) != -1 or method_name.find('operator') != -1 or method[0]['static']:   # pass all constructor and operator and static function
            continue
        if len(method) == 1:    # only one function, not exists overload
            method = method[0]
            new_code = "{}[\"{}\"] = &{}::{};".format(lower_name, method_name.lower(), name, method_name)
        else:   # function has overload
            cast_tmpl = "static_cast<{}({}::*)({}) {}>(&{}::{})"
            overload_methods = ""
            for m in method:
                params = m['parameters']
                parameter_str = ""
                for i, param in enumerate(params, 0):
                    param_type = param['type'] if param['name'].find('&') == -1 else param['type'] + '&'
                    parameter_str += param_type + ("," if i != len(params) -1 else "")

                if parameter_str.find('&&') != -1: # don't bind function which has rvalue reference parameter
                    continue
                rtnType: str = m['rtnType']
                overload = cast_tmpl.format(rtnType, name, parameter_str, 'const' if m['const'] else "", name, method_name)
                overload_methods += overload + '\n'
                new_code = "{}[\"{}\"] = sol::overload({});".format(lower_name, method_name.lower(), overload)
        # TODO generate more human-readable comment
        new_comment = "---@field {} {}".format(method_name, "func")
        cpp_code += new_code + '\n'
        lua_comment += new_comment + '\n'
    return cpp_code, lua_comment

def generate_bind_onetype_code(classinfo_table: dict):
    cpp_code = ""
    lua_comment = ""
    for name, info in classinfo_table.items():
        new_code, new_comment = regist_type(name, info)
        cpp_code += '\n' + new_code
        lua_comment += '\n' + new_comment
    return cpp_code, lua_comment

def generate_bind_code(classinfo: list):
    cpp_code = ""
    lua_comment = ""
    for table in classinfo:
        new_code, new_comment = generate_bind_onetype_code(table)
        cpp_code += '\n' + new_code
        lua_comment += '\n' + new_comment
    return cpp_code, lua_comment

BIND_LUA_CPP_TMPL = '''#include "luabind.hpp"
namespace lua_bind {

using ui::UIEventListener;
using ui::EventType;

void BindLua(LuaScript& script) {
##CONTENT##
}

}'''

CONTENT_TAG = '##CONTENT##'

if __name__ == '__main__':
    directories = ["include/app", "include/core", "include/game"]
    parsed_files = parse_all_files(directories)
    classinfo_table = create_classinfos(parsed_files)

    cpp_code, lua_comment = generate_bind_code(classinfo_table)

    if not os.path.exists("luabind"):
        os.mkdir("luabind")

    with open("luabind/defs.lua", 'w+') as f:
        f.write(lua_comment)

    with open("luabind/luabind.hpp", 'w+') as f:
        f.write('''#include "core/pch.hpp"
#include "app/lua.hpp"
#include "app/script.hpp"
#include "game/types.hpp"
#include "app/ui.hpp"

namespace lua_bind {
    void BindLua(LuaScript& script);
}
''')

    with open("luabind/luabind.cpp", 'w+') as f:
        f.write(BIND_LUA_CPP_TMPL.replace(CONTENT_TAG, cpp_code))

    # print(cpp_code)
    # print(lua_comment)
