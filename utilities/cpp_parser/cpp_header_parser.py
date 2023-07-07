import clang.cindex
import re

class ACCESS_TYPE:
    PUBLIC = 0
    PROTECTED = 1
    PRIVATE = 2

class Function:
    def __init__(self, name, return_type, parameters, is_static, attrs):
        self.name = name
        self.return_type = return_type
        self.parameters = parameters
        self.is_static = is_static
        self.attributes = attrs

class Variable:
    def __init__(self, type, name, default_value, is_static, is_constexpr, attrs):
        self.type = type
        self.name = name
        self.default_value = default_value
        self.is_static = is_static
        self.is_constexpr = is_constexpr
        self.attributes = attrs

class Namespace:
    def __init__(self, name: str, line_number: int):
        self.name = name
        self.line_number = line_number

    def __str__(self) -> str:
        return 'namespace: {}, line: {}'.format(self.name, self.line_number)

class Class:
    def __init__(self,
                name: str,
                variables: tuple[list[Variable], list[Variable], list[Variable]],
                functions: tuple[list[Variable], list[Variable], list[Variable]],
                namespaces: list[Namespace]) -> None:
        self.name = name
        self.variables = variables
        self.functions = functions
        self.namespaces = namespaces

class Enum:
    def __init__(self, namespaces: list[Namespace], name: str, values: list[tuple[str, int]], is_class: bool, owned_class: Class):
        self.namespaces = namespaces
        self.name = name
        self.values = values
        self.is_class = is_class
        self.owned_class = owned_class


# use regex to find attribute(not walk AST tree because libclang will discard all no-support attributes)
def ParseAttrsFromSourceCode(line_number: int, source_code: list[str]) -> list[str]:
    attributes = []
    if line_number in range(0, len(source_code)):
        line = source_code[line_number]
        matchObj = re.findall(r'\[\[(.*?)\]\]', line);
        for m in matchObj:
            attrs = m.split(',')
            for attr in attrs:
                attributes.append(attr.strip())
    return attributes

def ParseMemberFunctions(class_decl: clang.cindex.Cursor, source_code: list[str]) -> list[Function]:
    member_functions = []
    for child_node in class_decl.get_children():
        if child_node.kind == clang.cindex.CursorKind.CXX_METHOD:
            name = child_node.spelling
            return_type = child_node.result_type.spelling
            child_node.result_type
            parameters = []
            is_static = bool(child_node.is_static_method())

            for arg_node in child_node.get_arguments():
                arg_type = arg_node.type.spelling
                arg_name = arg_node.spelling
                parameters.append((arg_type, arg_name))

            attributes = ParseAttrsFromSourceCode(child_node.location.line - 1, source_code)

            member_functions.append(Function(name, return_type, parameters, is_static, attributes))
    return member_functions


def ParseMemberVariables(class_decl: clang.cindex.Cursor, source_code: list[str]) -> list[Variable]:
    variables = []
    for child_node in class_decl.get_children():
        if (child_node.kind == clang.cindex.CursorKind.VAR_DECL or \
            child_node.kind == clang.cindex.CursorKind.FIELD_DECL) and \
            child_node.access_specifier == clang.cindex.AccessSpecifier.PUBLIC:
            var_type = child_node.type.spelling
            var_name = child_node.spelling
            var_default_value = None
            is_static = child_node.storage_class == clang.cindex.StorageClass.STATIC

            is_constexpr = True if source_code[child_node.location.line - 1].find('constexpr') != -1 else False

            attributes = ParseAttrsFromSourceCode(child_node.location.line - 1, source_code)

            for child_node2 in child_node.get_children():
                if child_node2.kind == clang.cindex.CursorKind.CXX_UNARY_EXPR and child_node2.opcode == clang.cindex.UnaryOperatorCode.IMPLICIT_CAST:
                    var_default_value = child_node2.type.spelling

            variables.append(Variable(var_type, var_name, var_default_value, is_static, is_constexpr, attributes))
    return variables

def RecurseExtractNamespace(node, source_code, namespace_chain = []):
    namespace_decls = [n for n in node.get_children() if n.kind == clang.cindex.CursorKind.NAMESPACE]
    for namespace in namespace_decls:
        namespaces = []
        line_num = namespace.location.line - 1
        if line_num not in range(0, len(source_code)):
            continue
        if namespace.spelling not in namespace_chain:
            namespaces.append(Namespace(namespace.spelling, namespace.location.line - 1))
        RecurseExtractNamespace(namespace, source_code, namespaces)
        namespace_chain.append(namespaces)

def ExtractNamespace(tu, source_code):
    namespace_decls = [n for n in tu.cursor.get_children() if n.kind == clang.cindex.CursorKind.NAMESPACE]
    namespaces = []
    for cursor in namespace_decls:
        line_num = cursor.location.line - 1
        if line_num not in range(0, len(source_code)):
            continue

        namespace = [Namespace(cursor.spelling, cursor.location.line - 1)]
        RecurseExtractNamespace(cursor, source_code, namespace)
        namespaces.append(namespace)
    return namespaces

def ExtractClasses(tu, source_code: list[str]):
    # Find all class declarations in the file
    class_decls = [node for node in tu.cursor.walk_preorder() if node.kind == clang.cindex.CursorKind.CLASS_DECL]

    classes = []

    for class_decl in class_decls:
        class_name = class_decl.spelling

        # parse attributes:
        attributes = []
        line_num = class_decl.location.line - 1
        if line_num in range(0, len(source_code)):
            attributes = ParseAttrsFromSourceCode(class_decl.location.line - 1, source_code)

        # special ruler for class:
        if 'refl' not in attributes:
            continue

        # parse member functions:
        member_functions = ParseMemberFunctions(class_decl, source_code)

        # parse member variables:
        member_variables = ParseMemberVariables(class_decl, source_code)

        classes.append((class_name, member_functions, member_variables, attributes))

    return classes

def ExtractGlobalFunctions(tu, source_code):
    functions = []
    func_decls = [node for node in tu.cursor.walk_preorder() if node.kind == clang.cindex.CursorKind.FUNCTION_DECL]

    for child in func_decls:
        line_num = child.location.line - 1
        if line_num not in range(0, len(source_code)):
            continue

        # parse attributes:
        attributes = ParseAttrsFromSourceCode(line_num, source_code)

        # special ruler for global functions:
        if 'refl' not in attributes:
            continue

        name = child.spelling
        return_type = child.result_type.spelling
        parameters = [(p.type.spelling, p.spelling) for p in child.get_arguments()]
        is_static = child.storage_class == clang.cindex.StorageClass.STATIC
        functions.append(Function(name, return_type, parameters, is_static, attributes))
    return functions

class CodeInfo:
    def __init__(self, classes: list[Class], global_functions: list[Function], namespaces: list[Namespace]) -> None:
        self.classes = classes
        self.global_functions = global_functions
        self.namespaces = namespaces

def ParseHeaderFile(filename: str) -> CodeInfo:
    index = clang.cindex.Index.create()
    tu = index.parse(filename, args=['-std=c++17'])
    source_code = ''
    with open(tu.spelling) as f:
        source_code = f.readlines()

    classes = ExtractClasses(tu, source_code)
    namespaces = ExtractNamespace(tu, source_code)
    functions = ExtractGlobalFunctions(tu, source_code)
    return CodeInfo(classes, functions, namespaces)


# example
if __name__ == '__main__':
    codeinfo = ParseHeaderFile('./utilities/cpp_parser/test.hpp')

    namespaces = codeinfo.namespaces
    classes = codeinfo.classes
    functions = codeinfo.global_functions

    for n in namespaces:
        print(n)

    for clazz in classes:
        print("functions:")
        for func in clazz[1]:
            print(func.name)
            print(func.return_type)
            print(func.parameters)
            print(func.is_static)
        print("variables:")
        for var in clazz[2]:
            print(var.type)
            print(var.name)
            print(var.default_value)
            print(var.is_static)
            print(var.is_constexpr)
        for attr in clazz[3]:
            print(attr)

    for func in functions:
        print(func.name)
        print(func.return_type)
        print(func.parameters)
        print(func.is_static)
