import cpp_header_parser as chp
import os
import re

CodeInfos: list[chp.CodeInfo] = []

class LuabindAttrs:
    def __init__(self):
        self.is_component = False
        self.is_resources = False
        self.is_nobind = False
        self.change_name = None

def ApplyOnEachFile(dirname: str, ignore_filenames: list[str], func):
    for dirpath, _, filenames in os.walk(dirname):
        for filename in filenames:
            complete_path = (dirpath + '/' + filename).replace('\\', '/')
            if complete_path in ignore_filenames:
                print("ignore file ", complete_path)
                continue
            print('dealing file ', complete_path, '...')
            func(complete_path)

def GetCodeInfo(filename: str):
    global CodeInfos
    CodeInfos.append(chp.ParseHeaderFile(filename))

def GenerateCode(code_info: list[chp.CodeInfo]) -> tuple[str, str]:
    refl_code = GenerateClassBindCode(code_info)
    classbind_code = GenerateLuabindCode(code_info)
    
    return refl_code, classbind_code

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
    ignore_filenames = []
    with open('./utilities/cpp_parser/ignore_files.txt') as f:
        ignore_filenames = list(map(lambda s: s.strip(), f.readlines()))
    ApplyOnEachFile('./include', ignore_filenames, GetCodeInfo)

    class_num = 0
    gfunc_num = 0

    for info in CodeInfos:
        class_num = class_num + len(info.classes)
        gfunc_num = gfunc_num + len(info.global_functions)
    print('{} classes, {} global functions'.format(class_num, gfunc_num))

    tryCreateDir("./auto-parse")

    print('generating refl codes...')
    refl_code, luabind_code = GenerateCode(CodeInfos)
    with open("./auto-parse/refl-codes.hpp", "w+") as f:
        f.write(refl_code)

    # print('generating luabind codes...')
    with open("./auto-parse/luabind.cpp", "w+") as f:
        f.write(luabind_code)

    # print('generating auto-parse codes...')
    # code = GenerateParserCode(CodeInfos)
    # with open("./auto-parse/parser.hpp", "w+") as f:
    #     f.write(code)
