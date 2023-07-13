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
        self.resource_get = ""

def GenerateCode(code_info: list[chp.CodeInfo]) -> tuple[str, str]:
    refl_code = GenerateClassBindCode(code_info)
    classbind_code = GenerateLuabindCode(code_info)
    
    return refl_code, classbind_code

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
                if refl_attrs.is_resources:
                    wrapper_codes.resource_get += "\n\t" + wrapper_code.resource_get

    return wrapper_codes

def GenerateOneClassWrapperCode(classinfo: chp.Class, refl_attrs: LuabindAttrs) -> ECSWrapperCodes | None:
    wrapper_code = ECSWrapperCodes()

    classname_with_namespaces = ""
    if len(classinfo.namespaces) > 0:
        classname_with_namespaces = "::{}::{}".format('::'.join(classinfo.namespaces), classinfo.name)
    else:
        classname_with_namespaces = "::{}".format(classinfo.name)
    if refl_attrs.is_nobind:
        return None
    if refl_attrs.is_component:
        wrapper_code.querier_query = "DECL_QUERIER_QUERY({}, {})".format(classinfo.name, classname_with_namespaces)
        wrapper_code.querier_get = "DECL_QUERIER_GET({}, {})".format(classinfo.name, classname_with_namespaces)
        wrapper_code.querier_has = "DECL_QUERIER_HAS({}, {})".format(classinfo.name, classname_with_namespaces)
        wrapper_code.commands_add = "DECL_CMDS_ADD_COMP({}, {})".format(classinfo.name, classname_with_namespaces)
        wrapper_code.commands_destroy = "DECL_CMDS_DESTROY_COMP({}, {})".format(classinfo.name, classname_with_namespaces)
    if refl_attrs.is_resources:
        wrapper_code.resource_get = "DECL_RES_GET({}, {})".format(classinfo.name, classname_with_namespaces)

    return wrapper_code


def GenerateClassBindCode(code_info: list[chp.CodeInfo]) -> str:
    result = ""
    header_files = "#include \"core/refl.hpp\""
    for info in code_info:
        header_files += "\n#include \"{}\"".format(info.file.replace("./include/", ""))
        for class_info in info.classes:
            code = GenerateReflClassCode(class_info)
            result += code + "\n"
    return header_files + "\n" + result

def GenerateLuabindCode(code_info: list[chp.CodeInfo]) -> str:
    luabind_code = """
#include \"script/luabind.hpp\"
#include \"refl-codes.hpp\"

void BindLua(sol::state& lua) {{
{}
}}
"""

    classbind_code = ""
    for info in code_info:
        for clazz in info.classes:
            attr = ConvertLuabindAttr2ReflAttr(clazz.attributes, False)
            bind_name = clazz.name
            if attr.change_name is not None:
                bind_name = attr.change_name[0]
            if not attr.is_nobind:
                classbind_code += "\tluabind::BindClass<{}>(lua, \"{}\")\n".format(clazz.name, bind_name)

    global_func_code = GenerateGlobalFunctionBindCode(code_info)

    return luabind_code.format(classbind_code + "\n" + global_func_code)

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
                bind_attrs.change_name[1] = "LuaBindName<char, {}>".format(', '.join(list(map(lambda c: "'{}'".format(c), bind_attrs.change_name[0]))))
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
        result += "LuaNoBind, "
    return result

def GenerateReflClassCode(clazz: chp.Class) -> str:
    fmt_code = """
ReflClass({}) {{
    Constructors({})
    Fields({})
}};
"""

    constructor_code = ""
    for ctor in clazz.constructors:
        params = ', '.join(list(map(lambda param: param[0], ctor.parameters)))
        code = "{}({})".format(ctor.class_name, params)
        constructor_code += code + ', '

    fields_code = ""
    for var in clazz.variables:
        if len(var.attributes) > 0:
            attr_str = ConvertBindAttr2Str(ConvertLuabindAttr2ReflAttr(var.attributes, True))
            fields_code += "\n\t\tAttrField(Attrs({}), \"{}\", &{}::{}),".format(attr_str, var.name, clazz.name, var.name)
        else:
            fields_code += "\n\t\tField(\"{}\", &{}::{}),".format(var.name, clazz.name, var.name)
    for func_list in clazz.functions:
        if len(func_list) == 1:
            func = func_list[0]
            if len(func.attributes) > 0:
                attr_str = ConvertBindAttr2Str(ConvertLuabindAttr2ReflAttr(func.attributes, True))
                fields_code += "\n\t\tAttrField(Attrs({}), \"{}\", &{}::{}),".format(attr_str, func.name, clazz.name, func.name)
            else:
                fields_code += "\n\t\tField(\"{}\", {}{}::{}),".format(func.name, '&' if not func.is_static else '', clazz.name, func.name)
        else:
            pointer = "&{}::{}".format(clazz.name, func_list[0].name)
            fields_code += "\n\t\tOverload(\"{}\", {})".format(func_list[0].name,
                            ', '.join(map(lambda func: "static_cast<{}({}::*)({})>({})".format(func.return_type, clazz.name, 
                                ','.join(list(map(lambda param: param[0], func.parameters)))
                                , pointer), func_list)
                            ))
    
    return fmt_code.format(clazz.name, constructor_code, fields_code)

def GenerateGlobalFunctionBindCode(code_info: list[chp.CodeInfo]) -> str:
    result = ""
    for info in code_info:
        for func_list in info.global_functions:
            if len(func_list) == 1:
                func = func_list[0]
                result += "\tlua[\"{}\"] = &{}\n".format(func.name, func.name)
            else:
                result += "\tlua[\"{}\"] = sol::overload({})\n".format(func_list[0].name,
                                                                     ','.join(map(lambda func: "static_cast<{}(*)({})>(&{})".format(
                                                                        func.return_type,
                                                                        ','.join(list(map(lambda param: param[0], func.parameters))),
                                                                        '::{}::{}'.format('::'.join(func.namespaces), func.name)
                                                                     ), func_list)))
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

    print("generating ecs wrapper code...")
    ecs_wrapper_code = GenerateECSWrapperCode(codeinfos)
    template_code = ""
    with open("./utilities/cpp_parser/bind_template/ecs_wrapper.hpp", "r+") as f:
        template_code = f.read()
    with open(output_dir + "ecs_wrapper.hpp", "w+") as f:
        template_code = template_code.replace('{{ DECL_ADD_COMP }}', ecs_wrapper_code.commands_add)
        template_code = template_code.replace('{{ DECL_DESTROY_COMP }}', ecs_wrapper_code.commands_destroy)
        template_code = template_code.replace('{{ DECL_RES }}', ecs_wrapper_code.resource_get)
        template_code = template_code.replace('{{ DECL_QUERIER_QUERY }}', ecs_wrapper_code.querier_query)
        template_code = template_code.replace('{{ DECL_QUERIER_GET }}', ecs_wrapper_code.querier_get)
        template_code = template_code.replace('{{ DECL_QUERIER_HAS }}', ecs_wrapper_code.querier_has)
        f.write(template_code)   

    # parse generated esc wrapper code
    wrapper_code_info = GetCodeInfo(output_dir + "ecs_wrapper.hpp")
    codeinfos.append(wrapper_code_info)

    print('generating refl codes...')
    refl_code, luabind_code = GenerateCode(codeinfos)
    with open(output_dir + "refl-codes.hpp", "w+") as f:
        f.write(refl_code)

    print('generating luabind codes...')
    with open(output_dir + "luabind.cpp", "w+") as f:
        f.write(luabind_code)

    # print('generating auto-parse codes...')
    # code = GenerateParserCode(CodeInfos)
    # with open("./auto-parse/parser.hpp", "w+") as f:
    #     f.write(code)
