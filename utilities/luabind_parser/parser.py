import CppHeaderParser
import os
import sys
import pickle

# TODO: regist static method/property
# TODO: more human-readable lua hint

LUA_BIND_TAG = "LUA_BIND"
LUA_BIND_COMPONENT_TAG = "LUA_BIND_COMPONENT"
LUA_BIND_RESOURCE_TAG = "LUA_BIND_RESOURCE"
LUA_NOBIND_TAG = "LUA_NOBIND"

COMMANDS_WRAPPER_INSERT_TAG = '##COMMANDS_WRAPPER_INSERT_TAG##'
QUERIER_WRAPPER_INSERT_TAG = '##QUERIER_WRAPPER_INSERT_TAG##'
RESOURCE_WRAPPER_INSERT_TAG = '##RESOURCE_WRAPPER_INSERT_TAG##'
LUA_BIND_CONTENT_TAG = '##BIND_CONTENT##'

OUTPUT_PATH = ('' if len(sys.argv) < 2  else sys.argv[1]).replace('\\', '/')

class LuaBindType:
    NORMAL = 0
    COMPONENT = 1
    RESOURCES = 2

class ClassInfo:
    def __init__(self, name: str, namespace: str, properties, methods, bind_type: LuaBindType):
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

def parse_all_files(directories) -> dict[str, CppHeaderParser.CppHeader]:
    parsed_files = {}
    for d in directories:
        for root, _, fs in os.walk(d):
            for f in fs:
                fullname = os.path.join(root, f)
                parsed_files[fullname] = CppHeaderParser.CppHeader(fullname)
    return parsed_files

def create_global_methodinfos(parsed_files: dict[str, CppHeaderParser.CppHeader]) -> dict[(str, str), list[CppHeaderParser.CppMethod]]:
    methods: dict[(str, str), list[CppHeaderParser.CppMethod]] = {}
    for file in parsed_files.values():
        for method in file.functions:
            if method['rtnType'].find(LUA_BIND_TAG) == -1:
                continue
            key = (method['name'], method['namespace'])
            if key in methods:
                methods[key].append(method)
            else:
                methods[key] = [method]
    return methods

def gather_global_enum_infos(parsed_files: dict[str, CppHeaderParser.CppHeader]) -> list[CppHeaderParser.CppEnum]:
    enums = []
    for info in parsed_files.values():
        for enum in info.enums:
            with open(enum['filename'], 'r') as f:
                line_number = enum['line_number'] - 2
                if line_number >= 0 and f.readlines()[line_number].find(LUA_BIND_TAG) != -1:
                    enums.append(enum)
    return enums

def gather_namespaces(parsed_files: dict[str, CppHeaderParser.CppHeader]) -> list[str]:
    namespaces = []
    for file in parsed_files.values():
        for namespace in file.namespaces:
            if namespace not in namespaces:
                namespaces.append(namespace)
    return namespaces

def create_classinfos(parsed_files: dict[str, CppHeaderParser.CppHeader]) -> list[dict[str, list[ClassInfo]]]:
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

def get_method_name_in_lua(name: str, has_param: bool):
    if name.find('operator') != -1:
        if name in Operator2Lua:
            if name == 'operator-':
                return '__sub' if has_param else None
            return Operator2Lua[name]
        else:
            return None
    else:
        return name

def get_name_with_namespace(name, classinfo_table: list[dict[str, ClassInfo]]) -> str:
    for info in classinfo_table:
        if name in info.keys():
            return "{}::{}".format(info[name].namespace, name)
    return name

# strip complex name to raw name(eg: `const Vector2&` -> `Vector2`)
def strip_type_name(name: str) -> str:
    return name.replace('const ', '').replace('&', '').replace('mutable ', '').strip()

def generate_parameter_type_with_namespace(name: str, classinfo_table: list[dict[str, ClassInfo]]) -> str:
    raw_name = strip_type_name(name)
    name_with_namespace = get_name_with_namespace(raw_name, classinfo_table)
    return name.replace(raw_name, name_with_namespace)

def pack_parameters(method: dict, classinfo_table: list[dict[str, ClassInfo]]) -> str:
    params: list = method['parameters']
    result = ""
    for i, param in enumerate(params):
        param_type = generate_parameter_type_with_namespace(
            param['type'] if param['name'].find('&') == -1 else param['type'] + '&', classinfo_table)
        if param_type is None:
            return None
        result += param_type + ("," if i != len(params) -1 else "")
    return result

def pack_class_method_type_for_overload(class_name_with_namespace: str, method_name: str, method: dict, classinfo_table: list[dict[str, ClassInfo]]) -> str:
    tmpl = "static_cast<{}({}::*)({}) {}>(&{}::{})"
    packed_params = pack_parameters(method, classinfo_table)
    if packed_params is None:
        return None
    rntType = generate_parameter_type_with_namespace(method['rtnType'], classinfo_table)
    return tmpl.format(rntType, class_name_with_namespace, packed_params, 'const' if method['const'] else "", class_name_with_namespace, method_name)

def pack_global_method_type_for_overload(namespace: str, method_name: str, method: dict, classinfo_table: list[dict[str, ClassInfo]]) -> str:
    tmpl = "static_cast<{}(*)({})>(&{}::{})"
    packed_params = pack_parameters(method, classinfo_table)
    if packed_params is None:
        return None
    raw_rntType = method['rtnType'].replace(LUA_BIND_TAG, '')
    rntType = generate_parameter_type_with_namespace(raw_rntType, classinfo_table)
    return tmpl.format(rntType, packed_params, namespace, method_name)

def pack_method(class_name: str, method_name: str):
    return "&{}::{};".format(class_name, method_name)

def pack_class_method_in_overload(class_name_with_namespace: str, methods: list, method_name: str, classinfo_table: list[dict[str, ClassInfo]]):
    overload_str = ""
    for m in methods:
        cast_code = pack_class_method_type_for_overload(class_name_with_namespace, method_name, m, classinfo_table)
        if cast_code is not None:
            overload_str += cast_code + '\n,'
    if overload_str == "":
        return None
    return overload_str[: len(overload_str) - 1]

def pack_global_method_in_overload(namespace: str, methods: list, method_name: str, classinfo_table: list[dict[str, ClassInfo]]):
    overload_str = ""
    for m in methods:
        cast_code = pack_global_method_type_for_overload(namespace, method_name, m, classinfo_table)
        if cast_code is not None:
            overload_str += cast_code + '\n,'
    if overload_str == "":
        return None
    return overload_str[: len(overload_str) - 1]

def generate_bind_method_code(variable_name: str, class_name: str, class_name_with_namespace, methods: dict[str, list], classinfo_table: list[dict[str, ClassInfo]]):
    bind_code = ""
    lua_comment = ""
    for method_name, method in methods.items():
        m = method[0]
        method_name = m['name']
        method_name_in_lua = get_method_name_in_lua(m['name'], len(m['parameters']) != 0)
        lua_comment += '---@field {} function\n'.format(method_name_in_lua)
        # TODO: regist static function
        if method_name_in_lua is None or method_name == class_name  or method[0]['rtnType'].find(LUA_NOBIND_TAG) != -1 or method[0]['static']:   # pass all constructor and operator and static function
            continue
        if len(method) == 1:    # only one function, not exists overload
            method = method[0]
            bind_code += '{}[\"{}\"] = {}'.format(variable_name, method_name_in_lua, pack_method(class_name_with_namespace, method_name) + '\n')
        else:   # function has overload
            overload_method = "{}[\"{}\"] = sol::overload({})".format(variable_name, method_name_in_lua, pack_class_method_in_overload(class_name_with_namespace, method, method_name, classinfo_table))
            if overload_method is not None:
                bind_code += overload_method + ';\n'
    return bind_code, lua_comment

def generate_constructor_type_code(class_name: str, class_name_with_namespace: str, method: dict, classinfo_table: list[dict[str, ClassInfo]]) -> str:
    packed_param = pack_parameters(method, classinfo_table)
    return "{}({})".format(class_name_with_namespace, packed_param)

def generate_constructor_bind_code(class_name: str, class_name_with_namespace: str, methods: list[dict], classinfo_table: list[dict[str, ClassInfo]]) -> str:
    cpp_code = ""
    for i, method in enumerate(methods):
        if method['deleted']:
            return None
        cpp_code += generate_constructor_type_code(class_name, class_name_with_namespace, method, classinfo_table)
        if i != len(methods) - 1:
            cpp_code += ','
    return 'sol::constructors<{}>()'.format(cpp_code)

def regist_type(name: str, info: ClassInfo, classinfo_table: list[dict[str, ClassInfo]]):
    namespace: str = info.namespace
    variable_name: str = (namespace if namespace is not None else "") + name.lower()
    name_without_namespace: str = name
    name: str = '{}::{}'.format(namespace, name)
    constructor_code = None
    if name_without_namespace in info.methods.keys():
        code = generate_constructor_bind_code(name_without_namespace, name, info.methods[name_without_namespace], classinfo_table)
        if code is not None and code.find("&&") == -1:
            constructor_code = code 
    cpp_code: str = "sol::usertype<{}> {} = script.lua.new_usertype<{}>(\"{}\"{});\n".format(name, variable_name, name, name_without_namespace, (', ' + constructor_code) if constructor_code is not None else '')
    lua_comment: str = "---@class {}\n".format(name_without_namespace)
    # bind properties
    for prop in info.properties:
        prop_name = prop['name']
        if prop['static'] or prop_name.find(LUA_NOBIND_TAG) != -1:  # remove all static properties
            continue
        new_code = "{}[\"{}\"] = &{}::{};".format(variable_name, prop_name, name, prop_name)
        new_comment = "---@field {} {}".format(prop['name'], prop['type'])
        cpp_code += new_code + '\n'
        lua_comment += new_comment + '\n'
    # bind methods
    overload_str, lua_def = generate_bind_method_code(variable_name, name_without_namespace, name, info.methods, classinfo_table)
    if overload_str is not None:
        cpp_code += overload_str + '\n'
    if lua_def is not None:
        lua_comment += '\n' + lua_def + '\n'
    return cpp_code, lua_comment

def generate_bind_onetype_code(classinfo: dict, classinf_table: list[dict[str, list]]):
    cpp_code = ""
    lua_comment = ""
    for name, info in classinfo.items():
        new_code, new_comment = regist_type(name, info, classinfo_table)
        cpp_code += '\n' + new_code
        lua_comment += '\n' + new_comment
    return cpp_code, lua_comment

def generate_bind_code(classinfo_table: list[dict[str, list]]):
    cpp_code = ""
    lua_comment = ""
    for table in classinfo_table:
        new_code, new_comment = generate_bind_onetype_code(table, classinfo_table)
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
    codes = '''sol::usertype<ResourcesWrapper> resources = script.lua.new_usertype<ResourcesWrapper>("Resources");\n
    resources["Raw"] = &ResourcesWrapper::Raw;\n'''
    lua_defs = '\n---@class Resources\n'
    for name, _ in resource_classinfo.items():
        codes += 'resources["Get{}"] = &ResourcesWrapper::Get{};\n'.format(name, name)
        lua_defs += '---@field Get{} fun(): {}\n'.format(name, name)
    return codes, lua_defs

def generate_querier_wrapper_bind_code(component_classinfo: dict[str, ClassInfo]):
    codes = '''sol::usertype<QuerierWrapper> querier =
        script.lua.new_usertype<QuerierWrapper>("Querier");
    querier["Raw"] = &QuerierWrapper::Raw;
    '''
    lua_defs = '---@class Querier\n'

    for name, _ in component_classinfo.items():
        codes += 'BIND_QUERIER_FUNC({});\n'.format(name)
        lua_defs += '---@field Get{} fun(e: Entity): {}\n'.format(name, name)
        lua_defs += '---@field Query{} fun(): table<Entity>\n'.format(name)
        lua_defs += '---@field Has{} fun(e: Entity): boolean\n'.format(name)
    return codes, lua_defs

def generate_events_wrapper_bind_code():
    codes = '''sol::usertype<EventsWrapper> events =
        script.lua.new_usertype<EventsWrapper>("Events");
    events["Raw"] = &EventsWrapper::Raw;
    events["ReadEventContext"] = &EventsWrapper::ReadEventContext;
    events["WriteEventContext"] = &EventsWrapper::WriteEventContext;
    '''
    lua_defs = '---@class Events\n'

    return codes, lua_defs



def generate_commands_wrapper_bind_code(component_classinfo: dict[str, ClassInfo]):
    codes = '''sol::usertype<CommandsWrapper> cmd = script.lua.new_usertype<CommandsWrapper>("Commands");
    sol::overload(
    {});
    cmd["Spawn"] = &CommandsWrapper::Spawn;
    cmd["DestroyEntity"] = &CommandsWrapper::DestroyEntity;
    cmd["Raw"] = &CommandsWrapper::Raw;\n'''
    lua_defs = '''
---@class Commands
---@field Spawn fun(): Entity
---@field DestroyEntity fun(e: Entity)
---@field Add fun(e: Any) add any component
    '''

    overload_code = ""
    for name, info in component_classinfo.items():
        overload_code += '\tstatic_cast<void(CommandsWrapper::*)(ecs::Entity, {}::{})>(&CommandsWrapper::Add),\n'.format(info.namespace, name)
    overload_code = overload_code[:len(overload_code) - 2] + '\n'
    return codes.format(overload_code), lua_defs

def generate_header_file(classinfo_table: list[dict[str, ClassInfo]]):
    resource_wrapper_code = generate_resource_wrapper_declare_code(classinfo_table[LuaBindType.RESOURCES])
    querier_wrapper_code = generate_querier_wrapper_declare_code(classinfo_table[LuaBindType.COMPONENT])
    commands_wrapper_code = generate_commands_wrapper_declare_code(classinfo_table[LuaBindType.COMPONENT])
    with open("./utilities/luabind_parser/luabind.hpp.tmpl") as f:
        content = f.read()
        content = content.replace(RESOURCE_WRAPPER_INSERT_TAG, resource_wrapper_code)
        content = content.replace(COMMANDS_WRAPPER_INSERT_TAG, commands_wrapper_code)
        content = content.replace(QUERIER_WRAPPER_INSERT_TAG, querier_wrapper_code)
    with open(OUTPUT_PATH + "./luabind/luabind.hpp", 'w+') as f:
        f.write(content)

def generate_global_method_bind_code(methods: dict[(str, str), list[CppHeaderParser.CppMethod]], clasinfo_table) -> str:
    cpp_code = ""
    for key, method_list in methods.items():
        name = key[0]
        namespace = key[1].replace(':', '')
        if len(method_list) > 1:
            packed_func = 'sol::overload({})'.format(pack_global_method_in_overload(namespace, method_list, name, classinfo_table))
            if namespace != '':
                cpp_code += 'namespace_{}["{}"] = {};\n'.format(namespace, name, packed_func)
            else:
                cpp_code += 'script.lua["{}"] = {};\n'.format(name, packed_func)
        else:
            if namespace != '':
                cpp_code += 'namespace_{}["{}"] = {}::{};\n'.format(namespace, name, namespace, name)
            else:
                cpp_code += 'script.lua["{}"] = ::{};\n'.format(name, name)
    return cpp_code

def generate_namespace_bind_code(namespaces: list[str]) -> str:
    cpp_code = ""
    for namespace in namespaces:
        cpp_code += 'auto namespace_{} = script.lua["{}"].get_or_create<sol::table>();\n'.format(namespace, namespace)
    return cpp_code

def generate_enum_defs_in_lua(enums: list[CppHeaderParser.CppEnum]) -> str:
    code = ''
    for enum in enums:
        enum_name = enum['name']
        value_code = ''
        for value in enum['values']:
            value_code += '\t{} = {},\n'.format(value['name'], value['value'])
        code += '---@enum {}\n{} = {{\n\t{} }}'.format(enum_name, enum_name, value_code)
    return code


def generate_impl_and_def_file(classinfo_table, methods, enums):
    cpp_code, lua_comment = generate_bind_code(classinfo_table)
    namespace_code = generate_namespace_bind_code(namespaces)
    querier_impl_code, lua_queirer_comment = generate_querier_wrapper_bind_code(classinfo_table[LuaBindType.COMPONENT])
    resource_impl_code, lua_resource_comment = generate_resource_wrapper_bind_code(classinfo_table[LuaBindType.RESOURCES])
    commands_impl_code, lua_command_comment = generate_commands_wrapper_bind_code(classinfo_table[LuaBindType.COMPONENT])
    events_impl_code, lua_events_comment = generate_events_wrapper_bind_code()
    global_method_bind_code = generate_global_method_bind_code(methods, classinfo_table)
    enum_defs_in_lua = generate_enum_defs_in_lua(enums)

    with open(OUTPUT_PATH + "./luabind/defs.lua", 'w+') as f:
        f.write('\n'.join([enum_defs_in_lua, lua_comment, lua_queirer_comment, lua_resource_comment, lua_command_comment, lua_events_comment]))

    with open("./utilities/luabind_parser/luabind.cpp.tmpl") as f:
        content = f.read()
        with open(OUTPUT_PATH + "./luabind/luabind.cpp", 'w+') as f2:
            f2.write(content.replace(LUA_BIND_CONTENT_TAG, '\n'.join(
                [namespace_code, cpp_code, global_method_bind_code, querier_impl_code, resource_impl_code, commands_impl_code, events_impl_code])))

if __name__ == '__main__':
    directories = ["include/app", "include/core", "include/game"]
    parsed_files: dict[str, CppHeaderParser.CppHeader] = parse_all_files(directories)
    classinfo_table = create_classinfos(parsed_files)
    methods = create_global_methodinfos(parsed_files)
    namespaces = gather_namespaces(parsed_files)
    enums = gather_global_enum_infos(parsed_files)

    packed_data = (classinfo_table, methods, namespaces, enums)

    dump_data_path = OUTPUT_PATH + "./dump.data"

    if os.path.exists(dump_data_path):
        with open(dump_data_path, "rb") as f:
            old_data = f.read()
            if old_data == pickle.dumps(packed_data, 1):
                print("nothing to do")
                exit(0)

    if not os.path.exists(OUTPUT_PATH + './' + "luabind"):
        os.mkdir(OUTPUT_PATH + './' + "luabind")

    generate_header_file(classinfo_table)
    generate_impl_and_def_file(classinfo_table, methods, enums)

    with open(dump_data_path, "wb+") as f:
        pickle.dump(packed_data, f, 1)