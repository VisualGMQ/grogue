import clang.cindex
import re
import copy

class ACCESS_TYPE:
    PUBLIC = 0
    PROTECTED = 1
    PRIVATE = 2

class Constructor:
    def __init__(self, class_name, parameters, attrs, is_deleted_method: bool):
        self.class_name = class_name
        self.parameters = parameters
        self.attrs = attrs
        self.is_deleted_method = is_deleted_method

class Function:
    def __init__(self, name: str, return_type: str, parameters: list[tuple[str, str]], is_static: bool, is_const_method: bool, attrs: list[str], namespaces: list[str], is_deleted_method: bool):
        self.name = name
        self.return_type = return_type
        self.parameters = parameters
        self.is_static = is_static
        self.attributes = attrs
        self.namespaces = namespaces
        self.is_const_method = is_const_method
        self.is_deleted_method = is_deleted_method

class Variable:
    def __init__(self, ty: str, name: str, default_value: str | None, is_static: bool, is_constexpr: bool, attrs: list[str]):
        self.type = ty
        self.name = name
        self.default_value = default_value
        self.is_static = is_static
        self.is_constexpr = is_constexpr
        self.attributes = attrs

class Class:
    def __init__(self,
                name: str,
                filename: str,
                variables: list[Variable],
                constructors: list[Constructor],
                functions: list[list[Function]],
                namespaces: list[str],
                attribtues: list[str]) -> None:
        self.name = name
        self.filename = filename
        self.variables = variables
        self.constructors = constructors
        self.functions = functions
        self.namespaces = namespaces
        self.attributes = attribtues

class Enum:
    def __init__(self, ty: str, name: str, filename: str, values: list[tuple[str, int]], is_class: bool, namespaces: list[str], attributes: list[str]):
        self.name = name
        self.values = values
        self.is_class = is_class
        self.namespaces = namespaces
        self.type = ty
        self.filename = filename
        self.attributes = attributes

def shouldParseMember(node, source_code: list[str]) -> bool:
    line_number = node.location.line - 1
    return line_number in range(0, len(source_code)) and 'norefl' not in source_code[line_number]

def ParseAttrsFromSourceCode(node):
    with open(node.location.file.name, "r") as f:
        line = ""
        for _ in range(0, node.location.line):
            line = f.readline()
        attributes = []
        matchObj = re.findall(r'\[\[(.*?)\]\]', line);
        for m in matchObj:
            attrs = m.split(',')
            for attr in attrs:
                attributes.append(attr.strip())
        return attributes

def findOverloadList(name, funcs: list[list[Function]]) -> list[Function] | None:
    for func in funcs:
        if func[0].name == name:
            return func
    return None

# use regex to find attribute(not walk AST tree because libclang will discard all no-support attributes)
def ParseConstructor(node: clang.cindex.Cursor, source_code: list[str]) -> Constructor | None:
    attributes = ParseAttrsFromSourceCode(node)
    if 'norefl' in attributes:
        return None

    name = node.spelling
    parameters = []

    for arg_node in node.get_arguments():
        arg_type = arg_node.type.spelling
        arg_name = arg_node.spelling
        parameters.append((arg_type, arg_name))

    return Constructor(name, parameters, attributes, node.is_deleted_method())

def ParseMemberFunction(node: clang.cindex.Cursor, source_code: list[str]) -> Function | None:
    if node.is_pure_virtual_method():
        return None

    attributes = ParseAttrsFromSourceCode(node)
    if 'norefl' in attributes:
        return None

    name = node.spelling
    return_type = node.result_type.spelling
    node.is_const_method()
    parameters = []
    is_static = bool(node.is_static_method())
    is_const_method = node.is_const_method()

    for arg_node in node.get_arguments():
        arg_type = arg_node.type.spelling
        arg_name = arg_node.spelling
        parameters.append((arg_type, arg_name))

    return Function(name, return_type, parameters, is_static, is_const_method, attributes, [], node.is_deleted_method())

def ParseMemberVariable(node: clang.cindex.Cursor, source_code: list[str], filename: str) -> Variable | None:
    attributes = ParseAttrsFromSourceCode(node)
    if 'norefl' in attributes:
        return None

    var_type = node.type.spelling
    var_name = node.spelling

    if var_type.strip()[-1] == '&': # any reference type can't has a member pointer
        return None

    var_default_value = None
    is_static = node.storage_class == clang.cindex.StorageClass.STATIC

    is_constexpr = True if source_code[node.location.line - 1].find('constexpr') != -1 else False

    for child_node2 in node.get_children():
        if child_node2.kind == clang.cindex.CursorKind.CXX_UNARY_EXPR and child_node2.opcode == clang.cindex.UnaryOperatorCode.IMPLICIT_CAST:
            var_default_value = child_node2.type.spelling

    return Variable(var_type, var_name, var_default_value, is_static, is_constexpr, attributes)

def ExtractClass(filename: str, class_decl: clang.cindex.Cursor, namespaces: list[str], source_code: list[str]) -> Class | None:
    if class_decl.location.file.name.find(filename) == -1:
        return None

    class_name = class_decl.spelling

    if class_decl.availability != clang.cindex.AvailabilityKind.AVAILABLE:
        return None

    # parse attributes:
    attributes = ParseAttrsFromSourceCode(class_decl)

    # special ruler for class:
    if 'refl' not in attributes or 'norefl' in attributes:
        return None

    constructors = []
    functions = []
    variables = []

    # parse constructors:
    for node in class_decl.get_children():
        if node.access_specifier != clang.cindex.AccessSpecifier.PUBLIC:
            continue

        if node.kind == clang.cindex.CursorKind.CONSTRUCTOR:
            constructor = ParseConstructor(node, source_code)
            if constructor is not None:
                constructors.append(constructor)
        elif node.kind == clang.cindex.CursorKind.CXX_METHOD:
            function = ParseMemberFunction(node, source_code)
            if function is not None:
                overload = findOverloadList(function.name, functions)
                if overload is not None:
                    overload.append(function)
                else:
                    functions.append([function])
        elif node.kind == clang.cindex.CursorKind.VAR_DECL or \
             node.kind == clang.cindex.CursorKind.FIELD_DECL:
            variable = ParseMemberVariable(node, source_code, filename)
            if variable is not None:
                variables.append(variable)

    return Class(class_name, filename, variables, constructors, functions, copy.deepcopy(namespaces), attributes)

def ExtractGlobalFunction(node: clang.cindex.Cursor, namespaces: list[str], source_code: list[str], filename: str) -> Function | None:
    if node.is_pure_virtual_method() or node.location.file.name.find(filename) == -1:
        return None

    # parse attributes:
    attributes = ParseAttrsFromSourceCode(node)

    # special ruler for global functions:
    if 'refl' not in attributes or 'norefl' in attributes:
        return None

    name = node.spelling
    return_type = node.result_type.spelling
    parameters = [(p.type.spelling, p.spelling) for p in node.get_arguments()]
    is_static = node.storage_class == clang.cindex.StorageClass.STATIC
    is_const_method = node.is_const_method()

    return Function(name, return_type, parameters, is_static, is_const_method, attributes, namespaces, False)

def ExtractGlobalEnum(node: clang.cindex.Cursor, namespaces: list[str], source_code: list[str], filename: str) -> Enum | None:
    if node.location.file.name.find(filename) == -1:
        return None

    name = node.spelling
    enum_type = node.enum_type.spelling

    # parse attributes:
    attributes = ParseAttrsFromSourceCode(node)

    # special ruler for global functions:
    if 'refl' not in attributes or 'norefl' in attributes:
        return None

    fields = []
    for child in node.get_children():
        if child.kind == clang.cindex.CursorKind.ENUM_CONSTANT_DECL:
            fields.append((child.spelling, child.enum_value))

    return Enum(enum_type, name, filename, fields, False, namespaces, attributes)

class CodeInfo:
    def __init__(self, file: str, classes: list[Class], global_functions: list[list[Function]], enums: list[Enum]) -> None:
        self.file = file
        self.classes = classes
        self.global_functions = global_functions
        self.enums = enums

def RecurseExtractCodeInfo(parent: clang.cindex.Cursor, source_code, filename: str, namespaces: list[str], classes: list[Class], global_functions: list[list[Function]], enums: list[Enum]):
    for node in parent.get_children():
        if node.kind == clang.cindex.CursorKind.NAMESPACE:
            new_namespaces = copy.deepcopy(namespaces)
            new_namespaces.append(node.spelling)
            RecurseExtractCodeInfo(node, source_code, filename, copy.deepcopy(new_namespaces), classes, global_functions, enums)
        elif node.kind == clang.cindex.CursorKind.CLASS_DECL or node.kind == clang.cindex.CursorKind.STRUCT_DECL:
            classinfo = ExtractClass(filename, node, copy.deepcopy(namespaces), source_code)
            if classinfo is not None:
                classes.append(classinfo)
        elif node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
            function = ExtractGlobalFunction(node, copy.deepcopy(namespaces), source_code, filename)
            if function is not None:
                overload = findOverloadList(function.name, global_functions)
                if overload is not None:
                    overload.append(function)
                else:
                    global_functions.append([function])
        elif node.kind == clang.cindex.CursorKind.ENUM_DECL:
            enum = ExtractGlobalEnum(node, copy.deepcopy(namespaces), source_code, filename)
            if enum is not None:
                enums.append(enum)

def ExtractCodeInfoUnderNamespace(node, source_code: list[str], filename: str) -> CodeInfo:
    classes = []
    global_functions = []
    enums = []
    RecurseExtractCodeInfo(node, source_code, filename, [], classes, global_functions, enums)
    return CodeInfo(filename, classes, global_functions, enums)

def ParseHeaderFile(filename: str) -> CodeInfo:
    index = clang.cindex.Index.create()
    tu = index.parse(filename, args=['-std=c++17',
                                    '-I./include',
                                    ])
    source_code = ''
    with open(tu.spelling) as f:
        source_code = f.readlines()

    return ExtractCodeInfoUnderNamespace(tu.cursor, source_code, filename)


# example
if __name__ == '__main__':
    codeinfo = ParseHeaderFile('./utilities/cpp_parser/test.hpp')

    classes = codeinfo.classes
    functions = codeinfo.global_functions

    for clazz in classes:
        print("class filename: ", clazz.filename)
        print("functions:")
        for func_list in clazz.functions:
            if len(func_list) == 1:
                print(func_list[0].name)
            else:
                print("overload:")
                print(func_list[0].name)
            for func in func_list:
                print(func.return_type)
                print(func.parameters)
                print(func.is_static)

        print("variables:")
        for var in clazz.variables:
            print(var.type)
            print(var.name)
            print(var.default_value)
            print(var.is_static)
            print(var.is_constexpr)
        for attr in clazz.attributes:
            print(attr)

    for func_list in functions:
        for func in func_list:
            print(func.name)
            print(func.return_type)
            print(func.parameters)
            print(func.is_static)
