import cpp_header_parser as chp
import os
import re
import sys

class LuabindAttrs:
    def __init__(self):
        self.is_component = False
        self.is_resources = False
        self.is_nobind = False
        self.change_name = None

GenerateFilenames = ["ecs_wrapper.hpp", "refl-codes.hpp", "luabind.cpp", "ecs_wrapper_refl.hpp"]

def ParseAllFileUnderDir(dirname: str, ignore_filenames: list[str], func) -> list[chp.CodeInfo]:
    codeinfos = []
    for dirpath, _, filenames in os.walk(dirname):
        for filename in filenames:
            complete_path = (dirpath + '/' + filename).replace('\\', '/')
            if complete_path in ignore_filenames:
                print("ignore file ", complete_path)
                continue
            print('dealing file ', complete_path, '...')
            codeinfo = func(complete_path)
            if codeinfo is not None:
                codeinfos.append(codeinfo)
    return codeinfos

def GetCodeInfo(filename: str):
    return chp.ParseHeaderFile(filename)

class ECSWrapperCodes:
    def __init__(self) -> None:
        self.commands_add = ""
        self.commands_destroy = ""
        self.querier_query = ""
        self.querier_get = ""
        self.querier_has = ""
        self.querier_convenient_get = ""
        self.resource_convenient_get = ""

def GenerateCode(code_info: list[chp.CodeInfo]) -> tuple[dict[str, str], str]:
    refl_code_dict = GenerateReflCode(code_info)
    classbind_code = GenerateLuabindCode(code_info)
    
    return refl_code_dict, classbind_code

def GenerateECSWrapperCode(code_info: list[chp.CodeInfo]) -> ECSWrapperCodes:
    wrapper_codes = ECSWrapperCodes()

    for codeinfo in code_info:
        for classinfo in codeinfo.classes:
            refl_attrs = ConvertLuabindAttr2ReflAttr(classinfo.attributes, False)
            wrapper_code = GenerateOneClassWrapperCode(classinfo, refl_attrs)
            if wrapper_code is not None:
                if refl_attrs.is_component:
                    wrapper_codes.commands_add += "\n\t" + wrapper_code.commands_add
                    wrapper_codes.commands_destroy += "\n\t" + wrapper_code.commands_destroy
                    wrapper_codes.querier_query += "\n\t" + wrapper_code.querier_query
                    wrapper_codes.querier_get += "\n\t" + wrapper_code.querier_get
                    wrapper_codes.querier_has += "\n\t" + wrapper_code.querier_has
                    wrapper_codes.querier_convenient_get += "\n\t" + wrapper_code.querier_convenient_get
                if refl_attrs.is_resources:
                    wrapper_codes.resource_convenient_get += "\n\t" + wrapper_code.resource_convenient_get
                    # wrapper_codes.resource_get += "\n\t" + wrapper_code.resource_get

    return wrapper_codes

def GenerateOneClassWrapperCode(classinfo: chp.Class, refl_attrs: LuabindAttrs) -> ECSWrapperCodes | None:
    wrapper_code = ECSWrapperCodes()

    classname_with_namespaces = '::'.join(classinfo.namespaces) + '::' + classinfo.name
    bind_name = classinfo.name if refl_attrs.change_name is None else refl_attrs.change_name
    if refl_attrs.is_nobind:
        return None
    if refl_attrs.is_component:
        wrapper_code.querier_query = "DECL_QUERIER_QUERY({}, {})".format(bind_name, classname_with_namespaces)
        wrapper_code.querier_get = "DECL_QUERIER_GET({}, {})".format(bind_name, classname_with_namespaces)
        wrapper_code.querier_has = "DECL_QUERIER_HAS({}, {})".format(bind_name, classname_with_namespaces)
        wrapper_code.commands_add = "DECL_CMDS_ADD_COMP({})".format(classname_with_namespaces)
        wrapper_code.commands_destroy = "DECL_CMDS_DESTROY_COMP({}, {})".format(bind_name, classname_with_namespaces)
        wrapper_code.querier_convenient_get = '''
        if (id == ::ecs::IndexGetter::template Get<{}>()) {{
            return sol::make_object(lua, std::ref(querier_.Get<{}>(entity)));
        }}
        '''.format(classname_with_namespaces, classname_with_namespaces)
    if refl_attrs.is_resources:
        wrapper_code.resource_convenient_get = '''
        if (id == ::ecs::IndexGetter::template Get<{}>()) {{
            return sol::make_object(lua, std::ref(res_.Get<{}>()));
        }}
        '''.format(classname_with_namespaces, classname_with_namespaces)

    return wrapper_code


def GenerateReflCode(code_info: list[chp.CodeInfo]) -> dict[str, str]:
    code_dict = {}
    for info in code_info:
        for class_info in info.classes:
            code = GenerateReflClassCode(class_info)
            if class_info.filename in code_dict.keys():
                code_dict[class_info.filename] += "\n" + code
            else:
                code_dict[class_info.filename] = '#pragma once\n#include "core/refl.hpp"\n#include "{}"' \
                                                    .format(class_info.filename
                                                                .replace("./include/", '')
                                                                .replace(output_dir, '')) + '\n' + code
        for enum_info in info.enums:
            code = GenerateReflEnumCode(enum_info)

            if enum_info.filename in code_dict.keys():
                code_dict[enum_info.filename] += "\n" + code
            else:
                code_dict[enum_info.filename] = '#pragma once\n#include "core/refl.hpp"\n#include "{}"' \
                                                    .format(enum_info.filename
                                                                .replace("./include/", '')
                                                                .replace(output_dir, '')) + '\n' + code

        
    return code_dict

def GenerateLuabindCode(code_info: list[chp.CodeInfo]) -> str:
    bind_code = ""
    for info in code_info:
        for clazz in info.classes:
            attr = ConvertLuabindAttr2ReflAttr(clazz.attributes, False)
            if attr.is_nobind:
                continue
            class_name_with_namespace = GetNamespacePrefixStr(clazz.namespaces) + clazz.name
            bind_name = clazz.name
            if attr.change_name is not None:
                bind_name = attr.change_name[0]
            bind_code += "\tluabind::BindClass<{}>(lua, \"{}\");\n".format(class_name_with_namespace, bind_name)
        for enum in info.enums:
            attr = ConvertLuabindAttr2ReflAttr(enum.attributes, False)
            if attr.is_nobind:
                continue
            name_with_namespace = GetNamespacePrefixStr(enum.namespaces) + enum.name
            bind_name = enum.name
            if attr.change_name is not None:
                bind_name = attr.change_name[0]
            if not attr.is_nobind:
                bind_code += "\tluabind::BindEnum<{}>(lua, \"{}\");\n".format(name_with_namespace, bind_name)


    global_func_code = GenerateGlobalFunctionBindCode(code_info)

    return bind_code + "\n" + global_func_code

def GetNamespacePrefixStr(namespaces: list[str]) -> str:
    return "::" if len(namespaces) == 0 else '::{}::'.format('::'.join(namespaces))

def ConvertLuabindAttr2ReflAttr(attrs: list[str], default_bind: bool) -> LuabindAttrs:
    bind_attrs = LuabindAttrs();
    bind_attrs.is_nobind = not default_bind
    for attr in attrs:
        if attr == "luanobind":
            bind_attrs.is_nobind = True
            continue
        if attr == "luabind":
            bind_attrs.is_nobind = False
            continue
        match_group = re.match("luabind\((.*?)\)", attr)
        if match_group is None:
            continue
        for content in match_group.groups():
            bind_attrs.is_nobind = False
            if content[0] == '"':
                bind_attrs.change_name = [content[1 : len(content) - 1], None]
                bind_attrs.change_name[1] = "luabind::LuaBindName<char, {}>".format(', '.join(list(map(lambda c: "'{}'".format(c), bind_attrs.change_name[0]))))
            else:
                if content == "comp":
                    bind_attrs.is_component = True
                elif content == "res":
                    bind_attrs.is_resources = True

    return bind_attrs

def ConvertBindAttr2Str(attr: LuabindAttrs) -> str:
    result = ""
    if attr.change_name is not None:
        result += attr.change_name[1] + ", "
    if attr.is_nobind:
        result += "luabind::LuaNoBind, "
    return result[:-2] if len(result) > 0 else result

def GenerateReflClassCode(clazz: chp.Class) -> str:
    fmt_code = """
{}

ReflClass({}) {{
    Constructors({})
    Fields({})
}};

"""

    clazz_name = GetNamespacePrefixStr(clazz.namespaces) + clazz.name

    constructor_code = ""
    for ctor in clazz.constructors:
        if ctor.is_deleted_method:
            continue
        params = ', '.join(list(map(lambda param: param[0], ctor.parameters)))
        code = "{}({})".format(clazz_name, params)
        constructor_code += code + ', '
    constructor_code = constructor_code[0: -2]

    fields_code = ""
    for var in clazz.variables:
        if var.is_static:   # FIXME: currently can't reflect static variable
            continue
        if len(var.attributes) > 0:
            attr_str = ConvertBindAttr2Str(ConvertLuabindAttr2ReflAttr(var.attributes, True))
            fields_code += "\n\t\tAttrField(Attrs({}), \"{}\", &{}::{}),".format(attr_str, var.name, clazz_name, var.name)
        else:
            fields_code += "\n\t\tField(\"{}\", &{}::{}),".format(var.name, clazz_name, var.name)

    for func_list in clazz.functions:
        if len(func_list) == 1:
            func = func_list[0]
            if func.is_deleted_method:
                continue
            if len(func.attributes) > 0:
                attr_str = ConvertBindAttr2Str(ConvertLuabindAttr2ReflAttr(func.attributes, True))
                fields_code += "\n\t\tAttrField(Attrs({}), \"{}\", &{}::{}),".format(attr_str, func.name, clazz_name, func.name)
            else:
                fields_code += "\n\t\tField(\"{}\", {}{}::{}),".format(func.name, '&' if not func.is_static else '', clazz_name, func.name)
        else:
            if func_list[0].is_static: # FIXME: can't relfection static overload function
                continue
            pointer = "&{}::{}".format(clazz_name, func_list[0].name)
            fields_code += "\n\t\tOverload(\"{}\", {}),".format(func_list[0].name,
                            ', '.join(['static_cast<{}({}::*)({}){}>({})'.format(
                                func.return_type, clazz_name,
                                ','.join([param[0] for param in func.parameters]),
                                'const' if func.is_const_method else '',
                                pointer
                            ) for func in func_list if not func.is_deleted_method]))
    
    return fmt_code.format('' if len(clazz.namespaces) == 0 else 'using namespace ::{};'.format('::'.join(clazz.namespaces)),
                           clazz_name,
                           constructor_code,
                           fields_code[:-1])


def GenerateReflEnumCode(info: chp.Enum):
    fmt = '''
ReflEnum({}, int) {{
    EnumValues(
        {}
    )
}};
'''
    class_name = '::'.join(info.namespaces) + '::' + info.name
    values = ''
    for value in info.values:
        values += 'EnumValue("{}", {}::{}),\n'.format(value[0], class_name, value[0])

    values = values if len(values) == 0 else values[0: -2]

    return fmt.format(class_name, values)

def GenerateGlobalFunctionBindCode(code_info: list[chp.CodeInfo]) -> str:
    result = ""
    for info in code_info:
        for func_list in info.global_functions:
            if len(func_list) == 1:
                func = func_list[0]
                attrs = ConvertLuabindAttr2ReflAttr(func.attributes, False)
                if attrs.is_nobind:
                    continue
                result += "\tlua[\"{}\"] = &{};\n".format(func.name, func.name)
            else:
                result += "\tlua[\"{}\"] = sol::overload({});\n".format(func_list[0].name,
                                                                     ','.join(map(lambda func: "static_cast<{}(*)({})>(&{})".format(
                                                                        func.return_type,
                                                                        ','.join(list(map(lambda param: param[0], func.parameters))),
                                                                        GetNamespacePrefixStr(func.namespaces) + func.name
                                                                     ), func_list)))
    return result

def GenerateComponentEnumCode(code_info: list[chp.CodeInfo]) -> str:
    result = ""
    for info in code_info:
        for clazz in info.classes:
            attrs = ConvertLuabindAttr2ReflAttr(clazz.attributes, False)
            if attrs.is_component:
                result += '\t\t{{ "{}", ::ecs::IndexGetter::template Get<{}>() }},\n'.format(clazz.name, '::'.join(clazz.namespaces) + '::' + (clazz.name if attrs.change_name is None else attrs.change_name));
    return result

def GenerateResourceEnumCode(code_info: list[chp.CodeInfo]) -> str:
    result = ""
    for info in code_info:
        for clazz in info.classes:
            attrs = ConvertLuabindAttr2ReflAttr(clazz.attributes, False)
            if attrs.is_resources:
                result += '\t\t{{ "{}", ::ecs::IndexGetter::template Get<{}>() }},\n'.format(clazz.name, '::'.join(clazz.namespaces) + '::' + (clazz.name if attrs.change_name is None else attrs.change_name));
    return result

def GenerateLuaECSWrapper(code_info: list[chp.CodeInfo]) -> str:
    result = ""
    for info in code_info:
        for clazz in info.classes:
            attrs = ConvertLuabindAttr2ReflAttr(clazz.attributes, False)
            clazz_name = clazz.name if attrs.change_name is None else attrs.change_name
            if attrs.is_component:
                result += '''
    if id == Component.{} then
        return self:Get{}()
    end
                '''.format(clazz_name, clazz_name)
    return result

def tryCreateDir(dir: str):
    if not os.path.exists(dir):
        os.mkdir(dir)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("please give me a output directory")
        sys.exit(1)

    output_dir = sys.argv[1]
    if output_dir[-1] != '/' and output_dir[-1] != '\\':
        output_dir += '/'
    print("output directory: {}".format(output_dir))

    ignore_filenames = []
    with open('./utilities/cpp_parser/ignore_files.txt') as f:
        ignore_filenames = list(map(lambda s: s.strip(), f.readlines()))
    codeinfos = ParseAllFileUnderDir('./include', ignore_filenames, GetCodeInfo)

    class_num = 0
    gfunc_num = 0

    for info in codeinfos:
        class_num = class_num + len(info.classes)
        gfunc_num = gfunc_num + len(info.global_functions)
    print('{} classes, {} global functions'.format(class_num, gfunc_num))

    tryCreateDir(output_dir)

    # wrapper_code_info = GetCodeInfo(output_dir + "ecs_wrapper.hpp")
    # codeinfos.append(wrapper_code_info)

    # generate reflection codes
    print('generating refl codes...')
    refl_code_dict, luabind_code = GenerateCode(codeinfos)

    convertPath2Relate = lambda filename: filename.replace('./include/', '').replace(output_dir, '').replace("/", "_").replace("\\", "_").replace('.hpp', '') + '_refl.hpp'

    all_include_filenames = []

    for filename, code in refl_code_dict.items():
        filename = convertPath2Relate(filename)
        all_include_filenames.append(filename)
        with open(output_dir + filename, "w+") as f:
            f.write(code)

    all_include_file_str = '\n'.join(['#include "{}"'.format(f) for f in all_include_filenames if f != "ecs_wrapper_refl.hpp"])

    # generate 
    ecs_wrapper_code = GenerateECSWrapperCode(codeinfos)

    # generate ecs wrapper code
    print("generating ecs wrapper code...")
    template_code = ""
    with open("./utilities/cpp_parser/bind_template/ecs_wrapper.hpp", "r+") as f:
        template_code = f.read()
    with open(output_dir + "ecs_wrapper.hpp", "w+") as f:
        template_code = template_code.replace('{{ DECL_ADD_COMP }}', ecs_wrapper_code.commands_add) \
                                     .replace('{{ DECL_DESTROY_COMP }}', ecs_wrapper_code.commands_destroy) \
                                     .replace('{{ DECL_RES }}', ecs_wrapper_code.resource_convenient_get) \
                                     .replace('{{ INCLUDE_FILES }}', all_include_file_str) \
                                     .replace('{{ DECL_QUERIER_CONVENIENCE_GET }}', ecs_wrapper_code.querier_convenient_get)
        f.write(template_code)   

    # generate ecs wrapper code
    wrapper_code_info = GetCodeInfo(output_dir + "ecs_wrapper.hpp")
    wrapper_code_refl_code, ecs_luabind_code = GenerateCode([wrapper_code_info])
    for filename, code in wrapper_code_refl_code.items():
        filename = convertPath2Relate(filename)
        all_include_filenames.append(filename)
        with open(output_dir + filename, "w+") as f:
            f.write(code)

    luabind_code += '\n' + ecs_luabind_code

    # generate luabind codes
    print('generating luabind codes...')

    bind_code = """
#pragma once
#include \"script/luabind.hpp\"
#include \"ecs_wrapper_refl.hpp\"
{}

void BindLua(sol::state& lua) {{
{}

    lua.new_enum<::ecs::ComponentID>("ComponentEnum", {{
        {}
    }});

    lua.new_enum<::ecs::ComponentID>("ResourceEnum", {{
        {}
    }});

    luabind::BindClass<::KeyButton>(lua, "KeyButton");
    luabind::BindClass<::MouseButton>(lua, "MouseButton");
}}
"""
    enum_code = GenerateComponentEnumCode(codeinfos);
    res_code = GenerateResourceEnumCode(codeinfos);
    luabind_code = bind_code.format(all_include_file_str, luabind_code, enum_code, res_code)
    with open(output_dir + "luabind.cpp", "w+") as f:
        f.write(luabind_code)
