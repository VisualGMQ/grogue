import CppHeaderParser
import os

# TODO: regist static method/property
# TODO: more human-readable lua hint
# TODO: regist global function

LUA_BIND_TAG = "LUA_BIND"
LUA_BIND_COMPONENT_TAG = "LUA_BIND_COMPONENT"
LUA_BIND_RESOURCE_TAG = "LUA_BIND_RESOURCE"
LUA_NOBIND_TAG = "LUA_NOBIND"

COMMANDS_WRAPPER_INSERT_TAG = '##COMMANDS_WRAPPER_INSERT_TAG##'
QUERIER_WRAPPER_INSERT_TAG = '##QUERIER_WRAPPER_INSERT_TAG##'
RESOURCE_WRAPPER_INSERT_TAG = '##RESOURCE_WRAPPER_INSERT_TAG##'
LUA_BIND_CONTENT_TAG = '##BIND_CONTENT##'

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
        for root, _, fs in os.walk(d):
            for f in fs:
                fullname = os.path.join(root, f)
                parsed_files[fullname] = CppHeaderParser.CppHeader(fullname)
    return parsed_files

def create_classinfos(parsed_files) -> list[dict[str, list[ClassInfo]]]:
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

Operator2Lua = {
    'operator==': '__eq',
    'operator<': '__lt',
    'operator%': '__mod',
    'operator+': '__add',
    'operator-': '__sub',
    'operator*': '__mul',
    'operator/': '__div',
}

def get_method_name_in_lua(name: str):
    if name.find('operator') != -1:
        if name in Operator2Lua:
            return Operator2Lua[name]
        else:
            return None
    else:
        return name

def get_name_with_namespace(name, classinfo_table: list[dict[str, ClassInfo]]) -> str:
    for info in classinfo_table:
        if name in info.keys():
            return "{}::{}".format(info[name].namespace, name)
    return None

# strip complex name to raw name(eg: `const Vector2&` -> `Vector2`)
def strip_name(name: str) -> str:
    return name.replace('const ', '').replace('&', '').replace('mutable ', '').strip()

def regist_type(name: str, info: ClassInfo):
    namespace: str = info.namespace
    lower_name: str = (namespace if namespace is not None else "") + name.lower()
    name_without_namespace: str = name
    name: str = '{}::{}'.format(namespace, name)
    cpp_code: str = "sol::usertype<{}> {} = script.lua.new_usertype<{}>(\"{}\");\n".format(name, lower_name, name, name_without_namespace)
    lua_comment: str = "---@class {}\n".format(name_without_namespace)
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
        method_name_in_lua = get_method_name_in_lua(method[0]['name'])
        # TODO: regist static function
        if method_name_in_lua is None or method_name == name_without_namespace or method[0]['rtnType'].find(LUA_NOBIND_TAG) != -1 or method[0]['static']:   # pass all constructor and operator and static function
            continue
        if len(method) == 1:    # only one function, not exists overload
            method = method[0]
            new_code = "{}[\"{}\"] = &{}::{};".format(lower_name, method_name_in_lua, name, method_name)
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
                new_code = "{}[\"{}\"] = sol::overload({});".format(lower_name, method_name_in_lua, overload)
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

def generate_resource_wrapper_declare_code(resource_classinfo: dict[str, ClassInfo]):
    codes = ""
    for name, info in resource_classinfo.items():
        codes += "{}::{}& Get{}() {{ return res_.Get<{}::{}>(); }}\n\t".format(info.namespace, name, name, info.namespace, name)
    return codes

def generate_querier_wrapper_declare_code(component_classinfo: dict[str, ClassInfo]):
    codes = ""
    for name, info in component_classinfo.items():
        codes += "IMPL_FUNC({}, {}::{});\n\t".format(name, info.namespace, name)
    return codes

def generate_commands_wrapper_declare_code(component_classinfo: dict[str, ClassInfo]):
    codes = ""
    for name, info in component_classinfo.items():
        codes += "IMPL_ADD_FUNC({}::{});\n\t".format(info.namespace, name)
    return codes

def generate_resource_wrapper_bind_code(resource_classinfo: dict[str, ClassInfo]):
    codes = 'sol::usertype<ResourcesWrapper> resources = script.lua.new_usertype<ResourcesWrapper>("Resources");\n'
    for name, _ in resource_classinfo.items():
        codes += 'resources["Get{}"] = &ResourcesWrapper::Get{};\n'.format(name, name)
    return codes

def generate_querier_wrapper_bind_code(component_classinfo: dict[str, ClassInfo]):
    codes = '''sol::usertype<QuerierWrapper> querier =
        script.lua.new_usertype<QuerierWrapper>("Querier");
    '''
    for name, info in component_classinfo.items():
        codes += 'BIND_QUERIER_FUNC({});\n'.format(name)
    return codes

def generate_commands_wrapper_bind_code(component_classinfo: dict[str, ClassInfo]):
    codes = '''sol::usertype<CommandsWrapper> cmd = script.lua.new_usertype<CommandsWrapper>("Commands");
    sol::overload(
    {});
    cmd["spawn"] = &CommandsWrapper::Spawn;
    cmd["destroy_entity"] = &CommandsWrapper::DestroyEntity;\n'''

    overload_code = ""
    for name, info in component_classinfo.items():
        overload_code += '\tstatic_cast<void(CommandsWrapper::*)(ecs::Entity, {}::{})>(&CommandsWrapper::Add),\n'.format(info.namespace, name)
    overload_code = overload_code[:len(overload_code) - 2] + '\n'
    return codes.format(overload_code)

def generate_header_file(classinfo_table: list[dict[str, ClassInfo]]):
    resource_wrapper_code = generate_resource_wrapper_declare_code(classinfo_table[LuaBindType.RESOURCES])
    querier_wrapper_code = generate_querier_wrapper_declare_code(classinfo_table[LuaBindType.COMPONENT])
    commands_wrapper_code = generate_commands_wrapper_declare_code(classinfo_table[LuaBindType.COMPONENT])
    with open("./utilities/ecs_parser/luabind.hpp.tmpl") as f:
        content = f.read()
        content = content.replace(RESOURCE_WRAPPER_INSERT_TAG, resource_wrapper_code)
        content = content.replace(COMMANDS_WRAPPER_INSERT_TAG, commands_wrapper_code)
        content = content.replace(QUERIER_WRAPPER_INSERT_TAG, querier_wrapper_code)
    with open("./luabind/luabind.hpp", 'w+') as f:
        f.write(content)

def generate_cpp_file(classinfo_table):
    cpp_code, lua_comment = generate_bind_code(classinfo_table)
    querier_impl_code = generate_querier_wrapper_bind_code(classinfo_table[LuaBindType.COMPONENT])
    resource_impl_code = generate_resource_wrapper_bind_code(classinfo_table[LuaBindType.RESOURCES])
    commands_impl_code = generate_commands_wrapper_bind_code(classinfo_table[LuaBindType.COMPONENT])
    cpp_code += "{}\n\n{}\n\n{}".format(querier_impl_code, resource_impl_code, commands_impl_code)

    with open("luabind/defs.lua", 'w+') as f:
        f.write(lua_comment)

    with open("./utilities/ecs_parser/luabind.cpp.tmpl") as f:
        content = f.read()
        with open("luabind/luabind.cpp", 'w+') as f2:
            f2.write(content.replace(LUA_BIND_CONTENT_TAG, cpp_code))

if __name__ == '__main__':
    directories = ["include/app", "include/core", "include/game"]
    parsed_files = parse_all_files(directories)
    classinfo_table = create_classinfos(parsed_files)

    if not os.path.exists("luabind"):
        os.mkdir("luabind")

    generate_header_file(classinfo_table)
    generate_cpp_file(classinfo_table)
