#!/usr/bin/env python
# generator.py
# simple C++ generator, originally targetted for Spidermonkey bindings
#
# Copyright (c) 2011 - Zynga Inc.

from clang import cindex
import sys
import pdb
import ConfigParser
import yaml
import re
import os
import inspect
import traceback
from Cheetah.Template import Template

type_map = {
    cindex.TypeKind.VOID        : "void",
    cindex.TypeKind.BOOL        : "bool",
    cindex.TypeKind.CHAR_U      : "unsigned char",
    cindex.TypeKind.UCHAR       : "unsigned char",
    cindex.TypeKind.CHAR16      : "char",
    cindex.TypeKind.CHAR32      : "char",
    cindex.TypeKind.USHORT      : "unsigned short",
    cindex.TypeKind.UINT        : "unsigned int",
    cindex.TypeKind.ULONG       : "unsigned long",
    cindex.TypeKind.ULONGLONG   : "unsigned long long",
    cindex.TypeKind.CHAR_S      : "char",
    cindex.TypeKind.SCHAR       : "char",
    cindex.TypeKind.WCHAR       : "wchar_t",
    cindex.TypeKind.SHORT       : "short",
    cindex.TypeKind.INT         : "int",
    cindex.TypeKind.LONG        : "long",
    cindex.TypeKind.LONGLONG    : "long long",
    cindex.TypeKind.FLOAT       : "float",
    cindex.TypeKind.DOUBLE      : "double",
    cindex.TypeKind.LONGDOUBLE  : "long double",
    cindex.TypeKind.NULLPTR     : "NULL",
    cindex.TypeKind.OBJCID      : "id",
    cindex.TypeKind.OBJCCLASS   : "class",
    cindex.TypeKind.OBJCSEL     : "SEL",
    # cindex.TypeKind.ENUM        : "int"
}

INVALID_NATIVE_TYPE = "??"

default_arg_type_arr = [

# An integer literal.
cindex.CursorKind.INTEGER_LITERAL,

# A floating point number literal.
cindex.CursorKind.FLOATING_LITERAL,

# An imaginary number literal.
cindex.CursorKind.IMAGINARY_LITERAL,

# A string literal.
cindex.CursorKind.STRING_LITERAL,

# A character literal.
cindex.CursorKind.CHARACTER_LITERAL,

# [C++ 2.13.5] C++ Boolean Literal.
cindex.CursorKind.CXX_BOOL_LITERAL_EXPR,

# [C++0x 2.14.7] C++ Pointer Literal.
cindex.CursorKind.CXX_NULL_PTR_LITERAL_EXPR,

# An expression that refers to some value declaration, such as a function,
# varible, or enumerator.
cindex.CursorKind.DECL_REF_EXPR
]

def native_name_from_type(ntype, underlying=False):
    kind = ntype.kind #get_canonical().kind
    const = "" #"const " if ntype.is_const_qualified() else ""
    if not underlying and kind == cindex.TypeKind.ENUM:
        decl = ntype.get_declaration()
        return get_namespaced_name(decl)
    elif kind in type_map:
        return const + type_map[kind]
    elif kind == cindex.TypeKind.RECORD:
        # might be an std::string
        decl = ntype.get_declaration()
        parent = decl.semantic_parent
        cdecl = ntype.get_canonical().get_declaration()
        cparent = cdecl.semantic_parent
        if decl.spelling == "string" and parent and parent.spelling == "std":
            return "std::string"
        elif cdecl.spelling == "function" and cparent and cparent.spelling == "std":
            return "std::function"
        else:
            # print >> sys.stderr, "probably a function pointer: " + str(decl.spelling)
            return const + decl.spelling
    else:
        # name = ntype.get_declaration().spelling
        # print >> sys.stderr, "Unknown type: " + str(kind) + " " + str(name)
        return INVALID_NATIVE_TYPE
        # pdb.set_trace()


def build_namespace(cursor, namespaces=[]):
    '''
    build the full namespace for a specific cursor
    '''
    if cursor:
        parent = cursor.semantic_parent
        if parent:
            if parent.kind == cindex.CursorKind.NAMESPACE or parent.kind == cindex.CursorKind.CLASS_DECL:
                namespaces.append(parent.displayname)
                build_namespace(parent, namespaces)

    return namespaces


def get_namespaced_name(declaration_cursor):
    ns_list = build_namespace(declaration_cursor, [])
    ns_list.reverse()
    ns = "::".join(ns_list)
    if len(ns) > 0:
        return ns + "::" + declaration_cursor.displayname
    return declaration_cursor.displayname

def generate_namespace_list(cursor, namespaces=[]):
    '''
    build the full namespace for a specific cursor
    '''
    if cursor:
        parent = cursor.semantic_parent
        if parent:
            if parent.kind == cindex.CursorKind.NAMESPACE or parent.kind == cindex.CursorKind.CLASS_DECL:
                if parent.kind == cindex.CursorKind.NAMESPACE:
                    namespaces.append(parent.displayname)
                generate_namespace_list(parent, namespaces)
    return namespaces

def get_namespace_name(declaration_cursor):
    ns_list = generate_namespace_list(declaration_cursor, [])
    ns_list.reverse()
    ns = "::".join(ns_list)

    if len(ns) > 0:
        return ns + "::"

    return declaration_cursor.displayname


class NativeType(object):
    def __init__(self):
        self.is_object = False
        self.is_function = False
        self.is_enum = False
        self.is_numeric = False
        self.not_supported = False
        self.param_types = []
        self.ret_type = None
        self.namespaced_name = ""
        self.namespace_name  = ""
        self.name = ""
        self.whole_name = None
        self.is_const = False
        self.is_pointer = False
        self.canonical_type = None

    @staticmethod
    def from_type(ntype):
        if ntype.kind == cindex.TypeKind.POINTER:
            nt = NativeType.from_type(ntype.get_pointee())

            if None != nt.canonical_type:
                nt.canonical_type.name += "*"
                nt.canonical_type.namespaced_name += "*"
                nt.canonical_type.whole_name += "*"

            nt.name += "*"
            nt.namespaced_name += "*"
            nt.whole_name = nt.namespaced_name
            nt.is_enum = False
            nt.is_const = ntype.get_pointee().is_const_qualified()
            nt.is_pointer = True
            if nt.is_const:
                nt.whole_name = "const " + nt.whole_name
        elif ntype.kind == cindex.TypeKind.LVALUEREFERENCE:
            nt = NativeType.from_type(ntype.get_pointee())
            nt.is_const = ntype.get_pointee().is_const_qualified()
            nt.whole_name = nt.namespaced_name + "&"

            if nt.is_const:
                nt.whole_name = "const " + nt.whole_name

            if None != nt.canonical_type:
                nt.canonical_type.whole_name += "&"
        else:
            nt = NativeType()
            decl = ntype.get_declaration()

            if ntype.kind == cindex.TypeKind.RECORD:
                if decl.kind == cindex.CursorKind.CLASS_DECL:
                    nt.is_object = True
                nt.name = decl.displayname
                nt.namespaced_name = get_namespaced_name(decl)
                nt.namespace_name  = get_namespace_name(decl)
                nt.whole_name = nt.namespaced_name
            else:
                if decl.kind == cindex.CursorKind.NO_DECL_FOUND:
                    nt.name = native_name_from_type(ntype)
                else:
                    nt.name = decl.spelling
                nt.namespaced_name = get_namespaced_name(decl)
                nt.namespace_name  = get_namespace_name(decl)

                if nt.namespaced_name == "std::string":
                    nt.name = nt.namespaced_name

                if nt.namespaced_name.startswith("std::function"):
                    nt.name = "std::function"

                if len(nt.namespaced_name) == 0 or nt.namespaced_name.find("::") == -1:
                    nt.namespaced_name = nt.name

                nt.whole_name = nt.namespaced_name
                nt.is_const = ntype.is_const_qualified()
                if nt.is_const:
                    nt.whole_name = "const " + nt.whole_name

                # Check whether it's a std::function typedef
                cdecl = ntype.get_canonical().get_declaration()
                if None != cdecl.spelling and 0 == cmp(cdecl.spelling, "function"):
                    nt.name = "std::function"

                if nt.name != INVALID_NATIVE_TYPE and nt.name != "std::string" and nt.name != "std::function":
                    if ntype.kind == cindex.TypeKind.UNEXPOSED or ntype.kind == cindex.TypeKind.TYPEDEF:
                        ret = NativeType.from_type(ntype.get_canonical())
                        if ret.name != "":
                            if decl.kind == cindex.CursorKind.TYPEDEF_DECL:
                                ret.canonical_type = nt
                            return ret

                nt.is_enum = ntype.get_canonical().kind == cindex.TypeKind.ENUM

                if nt.name == "std::function":
                    nt.namespaced_name = get_namespaced_name(cdecl)
                    tmp = cdecl.displayname
                    r = re.compile('function<(.*)\((.*)\)>').search(tmp)
                    (ret_type, params) = r.groups()
                    if ret_type[-1] == ' ':
                        ret_type = ret_type[:-1]
                    
                    params = filter(None, params.split(", "))
                    nt.is_function = True
                    nt.ret_type = NativeType.from_string(ret_type)
                    nt.param_types = [NativeType.from_string(string) for string in params]

        # mark argument as not supported
        if nt.name == INVALID_NATIVE_TYPE:
            nt.not_supported = True

        if re.search("(short|int|double|float|long|ssize_t)$", nt.name) != None:
            nt.is_numeric = True;

        if re.search("(DPoint)$", nt.name) != None:
            nt.is_numeric = False;

        return nt

    @staticmethod
    def from_string(displayname):
        displayname = displayname.replace(" *", "*")

        nt = NativeType()
        nt.name = displayname.split("::")[-1]
        nt.namespaced_name = displayname
        nt.whole_name = nt.namespaced_name
        nt.is_object = True
        return nt

    @property
    def lambda_parameters(self):
        params = ["%s larg%d" % (str(nt), i) for i, nt in enumerate(self.param_types)]
        return ", ".join(params)

    @staticmethod
    def dict_has_key_re(dict, real_key_list):
        for real_key in real_key_list:
            for (k, v) in dict.items():
                if k.startswith('@'):
                    k = k[1:]
                    match = re.match("^" + k + "$", real_key)
                    if match:
                        return True
                else:
                    if k == real_key:
                        return True
        return False

    @staticmethod
    def dict_get_value_re(dict, real_key_list):
        for real_key in real_key_list:
            for (k, v) in dict.items():
                if k.startswith('@'):
                    k = k[1:]
                    match = re.match("^" + k + "$", real_key)
                    if match:
                        return v
                else:
                    if k == real_key:
                        return v
        return None

    @staticmethod
    def dict_replace_value_re(dict, real_key_list):
        for real_key in real_key_list:
            for (k, v) in dict.items():
                if k.startswith('@'):
                    k = k[1:]
                    match = re.match('.*' + k, real_key)
                    if match:
                        return re.sub(k, v, real_key)
                else:
                    if k == real_key:
                        return v
        return None

    def from_native(self, convert_opts):
        assert(convert_opts.has_key('generator'))
        generator = convert_opts['generator']
        keys = []

        if self.canonical_type != None:
            keys.append(self.canonical_type.name)
        keys.append(self.name)

        from_native_dict = generator.config['conversions']['from_native']

        if self.is_object:
            if not NativeType.dict_has_key_re(from_native_dict, keys):
                keys.append("object")
        elif self.is_enum:
            keys.append("int")

        if NativeType.dict_has_key_re(from_native_dict, keys):
            tpl = NativeType.dict_get_value_re(from_native_dict, keys)
            tpl = Template(tpl, searchList=[convert_opts])
            return str(tpl).rstrip()

        return "#pragma warning NO CONVERSION FROM NATIVE FOR " + self.name

    def to_native(self, convert_opts):
        assert('generator' in convert_opts)
        generator = convert_opts['generator']
        keys = []

        if self.canonical_type != None:
            keys.append(self.canonical_type.name)
        keys.append(self.name)

        to_native_dict = generator.config['conversions']['to_native']
        if self.is_object:
            if not NativeType.dict_has_key_re(to_native_dict, keys):
                keys.append("object")
        elif self.is_enum:
            keys.append("int")

        if self.is_function:
            tpl = Template(file=os.path.join(generator.target, "templates", "lambda.c"),
                searchList=[convert_opts, self])
            indent = convert_opts['level'] * "\t"
            return str(tpl).replace("\n", "\n" + indent)


        if NativeType.dict_has_key_re(to_native_dict, keys):
            tpl = NativeType.dict_get_value_re(to_native_dict, keys)
            tpl = Template(tpl, searchList=[convert_opts])
            return str(tpl).rstrip()
        return "#pragma warning NO CONVERSION TO NATIVE FOR " + self.name + "\n" + convert_opts['level'] * "\t" +  "ok = false"

    def to_string(self, generator):
        conversions = generator.config['conversions']
        if conversions.has_key('native_types'):
            native_types_dict = conversions['native_types']
            if NativeType.dict_has_key_re(native_types_dict, [self.namespaced_name]):
                return NativeType.dict_get_value_re(native_types_dict, [self.namespaced_name])

        name = self.namespaced_name

        to_native_dict = generator.config['conversions']['to_native']
        from_native_dict = generator.config['conversions']['from_native']
        use_typedef = False

        typedef_name = self.canonical_type.name if None != self.canonical_type else None

        if None != typedef_name:
            if NativeType.dict_has_key_re(to_native_dict, [typedef_name]) or NativeType.dict_has_key_re(from_native_dict, [typedef_name]):
                use_typedef = True

        if use_typedef and self.canonical_type:
            name = self.canonical_type.namespaced_name
        return "const " + name if (self.is_pointer and self.is_const) else name

    def get_whole_name(self, generator):
        conversions = generator.config['conversions']
        to_native_dict = conversions['to_native']
        from_native_dict = conversions['from_native']
        use_typedef = False
        name = self.whole_name
        typedef_name = self.canonical_type.name if None != self.canonical_type else None

        if None != typedef_name:
            if NativeType.dict_has_key_re(to_native_dict, [typedef_name]) or NativeType.dict_has_key_re(from_native_dict, [typedef_name]):
                use_typedef = True

        if use_typedef and self.canonical_type:
            name = self.canonical_type.whole_name

        to_replace = None
        if conversions.has_key('native_types'):
            native_types_dict = conversions['native_types']
            to_replace = NativeType.dict_replace_value_re(native_types_dict, [name])

        if to_replace:
            name = to_replace

        return name

    def object_can_convert(self, generator, is_to_native = True):
        if self.is_object:
            keys = []
            if  self.canonical_type != None:
                keys.append(self.canonical_type.name)
            keys.append(self.name)
            if is_to_native:
                to_native_dict = generator.config['conversions']['to_native']
                if NativeType.dict_has_key_re(to_native_dict, keys):
                    return True
            else:
                from_native_dict = generator.config['conversions']['from_native']
                if NativeType.dict_has_key_re(from_native_dict, keys):
                    return True

        return False

    def __str__(self):
        return  self.canonical_type.whole_name if None != self.canonical_type else self.whole_name

class NativeField(object):
    def __init__(self, cursor):
        cursor = cursor.canonical
        self.cursor = cursor
        self.name = cursor.displayname
        self.kind = cursor.type.kind
        self.location = cursor.location
        member_field_re = re.compile('m_(\w+)')
        match = member_field_re.match(self.name)
        self.signature_name = self.name
        self.ntype  = NativeType.from_type(cursor.type)
        if match:
            self.pretty_name = match.group(1)
        else:
            self.pretty_name = self.name

    @staticmethod
    def can_parse(ntype):
        if ntype.kind == cindex.TypeKind.POINTER:
            return False
        native_type = NativeType.from_type(ntype)
        if ntype.kind == cindex.TypeKind.UNEXPOSED and native_type.name != "std::string":
            return False
        return True

    def generate_code(self, current_class = None, generator = None):
        gen = current_class.generator if current_class else generator
        config = gen.config

        if config['definitions'].has_key('public_field'):
            tpl = Template(config['definitions']['public_field'],
                                    searchList=[current_class, self])
            self.signature_name = str(tpl)
        tpl = Template(file=os.path.join(gen.target, "templates", "public_field.c"),
                       searchList=[current_class, self])
        gen.impl_file.write(str(tpl))

# return True if found default argument.
def iterate_param_node(param_node, depth=1):
    for node in param_node.get_children():
        # print(">"*depth+" "+str(node.kind))
        if node.kind in default_arg_type_arr:
            return True

        if iterate_param_node(node, depth + 1):
            return True

    return False

class NativeFunction(object):
    def __init__(self, cursor):
        self.cursor = cursor
        self.func_name = cursor.spelling
        self.signature_name = self.func_name
        self.arguments = []
        self.argumtntTips = []
        self.static = cursor.kind == cindex.CursorKind.CXX_METHOD and cursor.is_static_method()
        self.implementations = []
        self.is_overloaded = False
        self.is_constructor = False
        self.not_supported = False
        self.is_override = False
        self.ret_type = NativeType.from_type(cursor.result_type)
        self.comment = self.get_comment(cursor.getRawComment())

        # parse the arguments
        # if self.func_name == "spriteWithFile":
        #   pdb.set_trace()
        for arg in cursor.get_arguments():
            self.argumtntTips.append(arg.spelling)

        for arg in cursor.type.argument_types():
            nt = NativeType.from_type(arg)
            self.arguments.append(nt)
            # mark the function as not supported if at least one argument is not supported
            if nt.not_supported:
                self.not_supported = True

        found_default_arg = False
        index = -1

        for arg_node in self.cursor.get_children():
            if arg_node.kind == cindex.CursorKind.CXX_OVERRIDE_ATTR:
                self.is_override = True
            if arg_node.kind == cindex.CursorKind.PARM_DECL:
                index += 1
                if iterate_param_node(arg_node):
                    found_default_arg = True
                    break

        self.min_args = index if found_default_arg else len(self.arguments)

    def get_comment(self, comment):
        replaceStr = comment

        if comment is None:
            return ""

        regular_replace_list = [
            ("(\s)*//!",""),
            ("(\s)*//",""),
            ("(\s)*/\*\*",""),
            ("(\s)*/\*",""),
            ("\*/",""),
            ("\r\n", "\n"),
            ("\n(\s)*\*", "\n"),
            ("\n(\s)*@","\n"),
            ("\n(\s)*","\n"),
            ("\n(\s)*\n", "\n"),
            ("^(\s)*\n",""),
            ("\n(\s)*$", ""),
            ("\n","<br>\n"),
            ("\n", "\n-- ")
        ]

        for item in regular_replace_list:
            replaceStr = re.sub(item[0], item[1], replaceStr)


        return replaceStr

    def generate_code(self, current_class=None, generator=None, is_override=False, is_ctor=False):
        self.is_ctor = is_ctor
        gen = current_class.generator if current_class else generator
        config = gen.config
        if not is_ctor:
            tpl = Template(file=os.path.join(gen.target, "templates", "function.h"),
                        searchList=[current_class, self])
            if not is_override:
                gen.head_file.write(str(tpl))
        if self.static:
            if config['definitions'].has_key('sfunction'):
                tpl = Template(config['definitions']['sfunction'],
                                    searchList=[current_class, self])
                self.signature_name = str(tpl)
            tpl = Template(file=os.path.join(gen.target, "templates", "sfunction.c"),
                            searchList=[current_class, self])
        else:
            if not self.is_constructor:
                if config['definitions'].has_key('ifunction'):
                    tpl = Template(config['definitions']['ifunction'],
                                    searchList=[current_class, self])
                    self.signature_name = str(tpl)
            else:
                if config['definitions'].has_key('constructor'):
                    if not is_ctor:
                        tpl = Template(config['definitions']['constructor'],
                                    searchList=[current_class, self])
                    else:
                        tpl = Template(config['definitions']['ctor'],
                                    searchList=[current_class, self])
                    self.signature_name = str(tpl)
            if self.is_constructor and gen.script_type == "spidermonkey" :
                if not is_ctor:
                    tpl = Template(file=os.path.join(gen.target, "templates", "constructor.c"),
                                                searchList=[current_class, self])
                else:
                    tpl = Template(file=os.path.join(gen.target, "templates", "ctor.c"),
                                                searchList=[current_class, self])
            else :
                tpl = Template(file=os.path.join(gen.target, "templates", "ifunction.c"),
                                searchList=[current_class, self])
        if not is_override:
            gen.impl_file.write(str(tpl))
        if not is_ctor:
            apidoc_function_script = Template(file=os.path.join(gen.target,
                                                            "templates",
                                                            "apidoc_function.script"),
                                          searchList=[current_class, self])
            if gen.script_type == "spidermonkey":
                gen.doc_file.write(str(apidoc_function_script))
            else:
                if gen.script_type == "lua" and current_class != None :
                    current_class.doc_func_file.write(str(apidoc_function_script))


class NativeOverloadedFunction(object):
    def __init__(self, func_array):
        self.implementations = func_array
        self.func_name = func_array[0].func_name
        self.signature_name = self.func_name
        self.min_args = 100
        self.is_constructor = False
        self.is_overloaded = True
        self.is_ctor = False
        for m in func_array:
            self.min_args = min(self.min_args, m.min_args)

        self.comment = self.get_comment(func_array[0].cursor.getRawComment())

    def get_comment(self, comment):
        replaceStr = comment

        if comment is None:
            return ""

        regular_replace_list = [
            ("(\s)*//!",""),
            ("(\s)*//",""),
            ("(\s)*/\*\*",""),
            ("(\s)*/\*",""),
            ("\*/",""),
            ("\r\n", "\n"),
            ("\n(\s)*\*", "\n"),
            ("\n(\s)*@","\n"),
            ("\n(\s)*","\n"),
            ("\n(\s)*\n", "\n"),
            ("^(\s)*\n",""),
            ("\n(\s)*$", ""),
            ("\n","<br>\n"),
            ("\n", "\n-- ")
        ]

        for item in regular_replace_list:
            replaceStr = re.sub(item[0], item[1], replaceStr)

        return replaceStr

    def append(self, func):
        self.min_args = min(self.min_args, func.min_args)
        self.implementations.append(func)

    def generate_code(self, current_class=None, is_override=False, is_ctor=False):
        self.is_ctor = is_ctor
        gen = current_class.generator
        config = gen.config
        static = self.implementations[0].static
        if not is_ctor:
            tpl = Template(file=os.path.join(gen.target, "templates", "function.h"),
                        searchList=[current_class, self])
            if not is_override:
                gen.head_file.write(str(tpl))
        if static:
            if config['definitions'].has_key('sfunction'):
                tpl = Template(config['definitions']['sfunction'],
                                searchList=[current_class, self])
                self.signature_name = str(tpl)
            tpl = Template(file=os.path.join(gen.target, "templates", "sfunction_overloaded.c"),
                            searchList=[current_class, self])
        else:
            if not self.is_constructor:
                if config['definitions'].has_key('ifunction'):
                    tpl = Template(config['definitions']['ifunction'],
                                    searchList=[current_class, self])
                    self.signature_name = str(tpl)
            else:
                if config['definitions'].has_key('constructor'):
                    if not is_ctor:
                        tpl = Template(config['definitions']['constructor'],
                                        searchList=[current_class, self])
                    else:
                        tpl = Template(config['definitions']['ctor'],
                                        searchList=[current_class, self])
                    self.signature_name = str(tpl)
            tpl = Template(file=os.path.join(gen.target, "templates", "ifunction_overloaded.c"),
                            searchList=[current_class, self])
        if not is_override:
            gen.impl_file.write(str(tpl))

        if current_class != None and not is_ctor:
            if gen.script_type == "lua":
                apidoc_function_overload_script = Template(file=os.path.join(gen.target,
                                                        "templates",
                                                        "apidoc_function_overload.script"),
                                      searchList=[current_class, self])
                current_class.doc_func_file.write(str(apidoc_function_overload_script))
            else:
                if gen.script_type == "spidermonkey":
                    apidoc_function_overload_script = Template(file=os.path.join(gen.target,
                                                        "templates",
                                                        "apidoc_function_overload.script"),
                                      searchList=[current_class, self])
                    gen.doc_file.write(str(apidoc_function_overload_script))


class NativeClass(object):
    def __init__(self, cursor, generator):
        # the cursor to the implementation
        self.cursor = cursor
        self.class_name = cursor.displayname
        self.is_ref_class = self.class_name == "Ref"
        self.namespaced_class_name = self.class_name
        self.parents = []
        self.fields = []
        self.public_fields = []
        self.methods = {}
        self.static_methods = {}
        self.generator = generator
        self.is_abstract = self.class_name in generator.abstract_classes
        self._current_visibility = cindex.AccessSpecifierKind.PRIVATE
        #for generate lua api doc
        self.override_methods = {}
        self.has_constructor  = False
        self.namespace_name   = ""

        registration_name = generator.get_class_or_rename_class(self.class_name)
        if generator.remove_prefix:
            self.target_class_name = re.sub('^' + generator.remove_prefix, '', registration_name)
        else:
            self.target_class_name = registration_name
        self.namespaced_class_name = get_namespaced_name(cursor)
        self.namespace_name        = get_namespace_name(cursor)
        self.parse()

    @property
    def underlined_class_name(self):
        return self.namespaced_class_name.replace("::", "_")

    def parse(self):
        '''
        parse the current cursor, getting all the necesary information
        '''
        self._deep_iterate(self.cursor)

    def methods_clean(self):
        '''
        clean list of methods (without the ones that should be skipped)
        '''
        ret = []
        for name, impl in self.methods.iteritems():
            should_skip = False
            if name == 'constructor':
                should_skip = True
            else:
                if self.generator.should_skip(self.class_name, name):
                    should_skip = True
            if not should_skip:
                ret.append({"name": name, "impl": impl})
        return ret

    def static_methods_clean(self):
        '''
        clean list of static methods (without the ones that should be skipped)
        '''
        ret = []
        for name, impl in self.static_methods.iteritems():
            should_skip = self.generator.should_skip(self.class_name, name)
            if not should_skip:
                ret.append({"name": name, "impl": impl})
        return ret

    def override_methods_clean(self):
        '''
        clean list of override methods (without the ones that should be skipped)
        '''
        ret = []
        for name, impl in self.override_methods.iteritems():
            should_skip = self.generator.should_skip(self.class_name, name)
            if not should_skip:
                ret.append({"name": name, "impl": impl})
        return ret

    def generate_code(self):
        '''
        actually generate the code. it uses the current target templates/rules in order to
        generate the right code
        '''

        if not self.is_ref_class:
            self.is_ref_class = self._is_ref_class()

        config = self.generator.config
        prelude_h = Template(file=os.path.join(self.generator.target, "templates", "prelude.h"),
                            searchList=[{"current_class": self}])
        prelude_c = Template(file=os.path.join(self.generator.target, "templates", "prelude.c"),
                            searchList=[{"current_class": self}])
        apidoc_classhead_script = Template(file=os.path.join(self.generator.target,
                                                         "templates",
                                                         "apidoc_classhead.script"),
                                       searchList=[{"current_class": self}])
        if self.generator.script_type == "lua":
            docfuncfilepath = os.path.join(self.generator.outdir + "/api", self.class_name + ".lua")
            self.doc_func_file = open(docfuncfilepath, "w+")
            apidoc_fun_head_script  = Template(file=os.path.join(self.generator.target,
                                                         "templates",
                                                         "apidoc_function_head.script"),
                                       searchList=[{"current_class": self}])
            self.doc_func_file.write(str(apidoc_fun_head_script))

        self.generator.head_file.write(str(prelude_h))
        self.generator.impl_file.write(str(prelude_c))
        self.generator.doc_file.write(str(apidoc_classhead_script))
        for m in self.methods_clean():
            m['impl'].generate_code(self)
        for m in self.static_methods_clean():
            m['impl'].generate_code(self)
        if self.generator.script_type == "lua":
            for m in self.override_methods_clean():
                m['impl'].generate_code(self, is_override = True)
        for m in self.public_fields:
            if self.generator.should_bind_field(self.class_name, m.name):
                m.generate_code(self)
        # generate register section
        register = Template(file=os.path.join(self.generator.target, "templates", "register.c"),
                            searchList=[{"current_class": self}])
        apidoc_classfoot_script = Template(file=os.path.join(self.generator.target,
                                                         "templates",
                                                         "apidoc_classfoot.script"),
                                       searchList=[{"current_class": self}])
        self.generator.impl_file.write(str(register))
        self.generator.doc_file.write(str(apidoc_classfoot_script))
        if self.generator.script_type == "lua":
            apidoc_fun_foot_script  = Template(file=os.path.join(self.generator.target,
                                                         "templates",
                                                         "apidoc_function_foot.script"),
                                       searchList=[{"current_class": self}])
            self.doc_func_file.write(str(apidoc_fun_foot_script))
            self.doc_func_file.close()
    def _deep_iterate(self, cursor=None, depth=0):
        for node in cursor.get_children():
            # print("%s%s - %s" % ("> " * depth, node.displayname, node.kind))
            if self._process_node(node):
                self._deep_iterate(node, depth + 1)

    @staticmethod
    def _is_method_in_parents(current_class, method_name):
        if len(current_class.parents) > 0:
            if method_name in current_class.parents[0].methods:
                return True
            return NativeClass._is_method_in_parents(current_class.parents[0], method_name)
        return False

    def _is_ref_class(self, depth = 0):
        """
        Mark the class as 'CrossApp::CAObject' or its subclass.
        """
        # print ">" * (depth + 1) + " " + self.class_name

        for parent in self.parents:
            if parent._is_ref_class(depth + 1):
                return True

        if self.is_ref_class:
            return True

        return False

    def _process_node(self, cursor):
        '''
        process the node, depending on the type. If returns true, then it will perform a deep
        iteration on its children. Otherwise it will continue with its siblings (if any)

        @param: cursor the cursor to analyze
        '''
        if cursor.kind == cindex.CursorKind.CXX_BASE_SPECIFIER:
            parent = cursor.get_definition()
            parent_name = parent.displayname

            if not self.class_name in self.generator.classes_have_no_parents:
                if parent_name and parent_name not in self.generator.base_classes_to_skip:
                    #if parent and self.generator.in_listed_classes(parent.displayname):
                    if not self.generator.generated_classes.has_key(parent.displayname):
                        parent = NativeClass(parent, self.generator)
                        self.generator.generated_classes[parent.class_name] = parent
                    else:
                        parent = self.generator.generated_classes[parent.displayname]

                    self.parents.append(parent)

            if parent_name == "Ref":
                self.is_ref_class = True

        elif cursor.kind == cindex.CursorKind.FIELD_DECL:
            self.fields.append(NativeField(cursor))
            if self._current_visibility == cindex.AccessSpecifierKind.PUBLIC and NativeField.can_parse(cursor.type):
                self.public_fields.append(NativeField(cursor))
        elif cursor.kind == cindex.CursorKind.CXX_ACCESS_SPEC_DECL:
            self._current_visibility = cursor.get_access_specifier()
        elif cursor.kind == cindex.CursorKind.CXX_METHOD and cursor.get_availability() != cindex.AvailabilityKind.DEPRECATED:
            # skip if variadic
            if self._current_visibility == cindex.AccessSpecifierKind.PUBLIC and not cursor.type.is_function_variadic():
                m = NativeFunction(cursor)
                registration_name = self.generator.should_rename_function(self.class_name, m.func_name) or m.func_name
                # bail if the function is not supported (at least one arg not supported)
                if m.not_supported:
                    return False
                if m.is_override:
                    if NativeClass._is_method_in_parents(self, registration_name):
                        if self.generator.script_type == "lua":
                            if not self.override_methods.has_key(registration_name):
                                self.override_methods[registration_name] = m
                            else:
                                previous_m = self.override_methods[registration_name]
                                if isinstance(previous_m, NativeOverloadedFunction):
                                    previous_m.append(m)
                                else:
                                    self.override_methods[registration_name] = NativeOverloadedFunction([m, previous_m])
                        return False

                if m.static:
                    if not self.static_methods.has_key(registration_name):
                        self.static_methods[registration_name] = m
                    else:
                        previous_m = self.static_methods[registration_name]
                        if isinstance(previous_m, NativeOverloadedFunction):
                            previous_m.append(m)
                        else:
                            self.static_methods[registration_name] = NativeOverloadedFunction([m, previous_m])
                else:
                    if not self.methods.has_key(registration_name):
                        self.methods[registration_name] = m
                    else:
                        previous_m = self.methods[registration_name]
                        if isinstance(previous_m, NativeOverloadedFunction):
                            previous_m.append(m)
                        else:
                            self.methods[registration_name] = NativeOverloadedFunction([m, previous_m])
            return True

        elif self._current_visibility == cindex.AccessSpecifierKind.PUBLIC and cursor.kind == cindex.CursorKind.CONSTRUCTOR and not self.is_abstract:
            # Skip copy constructor
            if cursor.displayname == self.class_name + "(const " + self.namespaced_class_name + " &)":
                # print "Skip copy constructor: " + cursor.displayname
                return True

            m = NativeFunction(cursor)
            m.is_constructor = True
            self.has_constructor = True
            if not self.methods.has_key('constructor'):
                self.methods['constructor'] = m
            else:
                previous_m = self.methods['constructor']
                if isinstance(previous_m, NativeOverloadedFunction):
                    previous_m.append(m)
                else:
                    m = NativeOverloadedFunction([m, previous_m])
                    m.is_constructor = True
                    self.methods['constructor'] = m
            return True
        # else:
            # print >> sys.stderr, "unknown cursor: %s - %s" % (cursor.kind, cursor.displayname)
        return False

class Generator(object):
    def __init__(self, opts):
        libchangPath = os.path.abspath(os.path.join(os.path.dirname(__file__), 'libclang'))
        cindex.Config.set_library_path(libchangPath)
        self.index = cindex.Index.create()
        self.outdir = opts['outdir']
        self.prefix = opts['prefix']
        self.headers = opts['headers'].split(' ')
        self.classes = opts['classes']
        self.classes_need_extend = opts['classes_need_extend']
        self.classes_have_no_parents = opts['classes_have_no_parents'].split(' ')
        self.base_classes_to_skip = opts['base_classes_to_skip'].split(' ')
        self.abstract_classes = opts['abstract_classes'].split(' ')
        self.clang_args = opts['clang_args']
        self.target = opts['target']
        self.remove_prefix = opts['remove_prefix']
        self.target_ns = opts['target_ns']
        self.cpp_ns = opts['cpp_ns']
        self.impl_file = None
        self.head_file = None
        self.skip_classes = {}
        self.bind_fields = {}
        self.generated_classes = {}
        self.rename_functions = {}
        self.rename_classes = {}
        self.replace_headers = {}
        self.out_file = opts['out_file']
        self.script_control_cpp = opts['script_control_cpp'] == "yes"
        self.script_type = opts['script_type']
        self.macro_judgement = opts['macro_judgement']
        self.hpp_headers = opts['hpp_headers']
        self.cpp_headers = opts['cpp_headers']
        self.win32_clang_flags = opts['win32_clang_flags']

        extend_clang_args = []

        for clang_arg in self.clang_args:
            if not os.path.exists(clang_arg.replace("-I","")):
                pos = clang_arg.find("lib/clang/3.3/include")
                if -1 != pos:
                    extend_clang_arg = clang_arg.replace("3.3", "3.4")
                    if os.path.exists(extend_clang_arg.replace("-I","")):
                        extend_clang_args.append(extend_clang_arg)

        if len(extend_clang_args) > 0:
            self.clang_args.extend(extend_clang_args)

        if sys.platform == 'win32' and self.win32_clang_flags != None:
            self.clang_args.extend(self.win32_clang_flags)


        if opts['skip']:
            list_of_skips = re.split(",\n?", opts['skip'])
            for skip in list_of_skips:
                class_name, methods = skip.split("::")
                self.skip_classes[class_name] = []
                match = re.match("\[([^]]+)\]", methods)
                if match:
                    self.skip_classes[class_name] = match.group(1).split(" ")
                else:
                    raise Exception("invalid list of skip methods")
        if opts['field']:
            list_of_fields = re.split(",\n?", opts['field'])
            for field in list_of_fields:
                class_name, fields = field.split("::")
                self.bind_fields[class_name] = []
                match = re.match("\[([^]]+)\]", fields)
                if match:
                    self.bind_fields[class_name] = match.group(1).split(" ")
                else:
                    raise Exception("invalid list of bind fields")
        if opts['rename_functions']:
            list_of_function_renames = re.split(",\n?", opts['rename_functions'])
            for rename in list_of_function_renames:
                class_name, methods = rename.split("::")
                self.rename_functions[class_name] = {}
                match = re.match("\[([^]]+)\]", methods)
                if match:
                    list_of_methods = match.group(1).split(" ")
                    for pair in list_of_methods:
                        k, v = pair.split("=")
                        self.rename_functions[class_name][k] = v
                else:
                    raise Exception("invalid list of rename methods")

        if opts['rename_classes']:
            list_of_class_renames = re.split(",\n?", opts['rename_classes'])
            for rename in list_of_class_renames:
                class_name, renamed_class_name = rename.split("::")
                self.rename_classes[class_name] = renamed_class_name

        if opts['replace_headers']:
            list_of_replace_headers = re.split(",\n?", opts['replace_headers'])
            for replace in list_of_replace_headers:
                header, replaced_header = replace.split("::")
                self.replace_headers[header] = replaced_header


    def should_rename_function(self, class_name, method_name):
        if self.rename_functions.has_key(class_name) and self.rename_functions[class_name].has_key(method_name):
            # print >> sys.stderr, "will rename %s to %s" % (method_name, self.rename_functions[class_name][method_name])
            return self.rename_functions[class_name][method_name]
        return None


    def get_class_or_rename_class(self, class_name):

        if self.rename_classes.has_key(class_name):
            # print >> sys.stderr, "will rename %s to %s" % (method_name, self.rename_functions[class_name][method_name])
            return self.rename_classes[class_name]
        return class_name

    def should_skip(self, class_name, method_name, verbose=False):
        if class_name == "*" and self.skip_classes.has_key("*"):
            for func in self.skip_classes["*"]:
                if re.match(func, method_name):
                    return True
        else:
            for key in self.skip_classes.iterkeys():
                if key == "*" or re.match("^" + key + "$", class_name):
                    if verbose:
                        print "%s in skip_classes" % (class_name)
                    if len(self.skip_classes[key]) == 1 and self.skip_classes[key][0] == "*":
                        if verbose:
                            print "%s will be skipped completely" % (class_name)
                        return True
                    if method_name != None:
                        for func in self.skip_classes[key]:
                            if re.match(func, method_name):
                                if verbose:
                                    print "%s will skip method %s" % (class_name, method_name)
                                return True
        if verbose:
            print "%s will be accepted (%s, %s)" % (class_name, key, self.skip_classes[key])
        return False

    def should_bind_field(self, class_name, field_name, verbose=False):
        if class_name == "*" and self.bind_fields.has_key("*"):
            for func in self.bind_fields["*"]:
                if re.match(func, method_name):
                    return True
        else:
            for key in self.bind_fields.iterkeys():
                if key == "*" or re.match("^" + key + "$", class_name):
                    if verbose:
                        print "%s in bind_fields" % (class_name)
                    if len(self.bind_fields[key]) == 1 and self.bind_fields[key][0] == "*":
                        if verbose:
                            print "All public fields of %s will be bound" % (class_name)
                        return True
                    if field_name != None:
                        for field in self.bind_fields[key]:
                            if re.match(field, field_name):
                                if verbose:
                                    print "Field %s of %s will be bound" % (field_name, class_name)
                                return True
        return False

    def in_listed_classes(self, class_name):
        """
        returns True if the class is in the list of required classes and it's not in the skip list
        """
        for key in self.classes:
            md = re.match("^" + key + "$", class_name)
            if md and not self.should_skip(class_name, None):
                return True
        return False

    def in_listed_extend_classed(self, class_name):
        """
        returns True if the class is in the list of required classes that need to extend
        """
        for key in self.classes_need_extend:
            md = re.match("^" + key + "$", class_name)
            if md:
                return True
        return False

    def sorted_classes(self):
        '''
        sorted classes in order of inheritance
        '''
        sorted_list = []
        for class_name in self.generated_classes.iterkeys():
            nclass = self.generated_classes[class_name]
            sorted_list += self._sorted_parents(nclass)
        # remove dupes from the list
        no_dupes = []
        [no_dupes.append(i) for i in sorted_list if not no_dupes.count(i)]
        return no_dupes

    def _sorted_parents(self, nclass):
        '''
        returns the sorted list of parents for a native class
        '''
        sorted_parents = []
        for p in nclass.parents:
            if p.class_name in self.generated_classes.keys():
                sorted_parents += self._sorted_parents(p)
        if nclass.class_name in self.generated_classes.keys():
            sorted_parents.append(nclass.class_name)
        return sorted_parents

    def generate_code(self):
        # must read the yaml file first
        stream = file(os.path.join(self.target, "conversions.yaml"), "r")
        data = yaml.load(stream)
        self.config = data
        implfilepath = os.path.join(self.outdir, self.out_file + ".cpp")
        headfilepath = os.path.join(self.outdir, self.out_file + ".hpp")

        docfiledir   = self.outdir + "/api"
        if not os.path.exists(docfiledir):
            os.makedirs(docfiledir)

        if self.script_type == "lua":
            docfilepath = os.path.join(docfiledir, self.out_file + "_api.lua")
        else:
            docfilepath = os.path.join(docfiledir, self.out_file + "_api.js")
            print docfilepath

        self.impl_file = open(implfilepath, "w+")
        self.head_file = open(headfilepath, "w+")
        self.doc_file = open(docfilepath, "w+")

        layout_h = Template(file=os.path.join(self.target, "templates", "layout_head.h"),
                            searchList=[self])
        layout_c = Template(file=os.path.join(self.target, "templates", "layout_head.c"),
                            searchList=[self])
        apidoc_ns_script = Template(file=os.path.join(self.target, "templates", "apidoc_ns.script"),
                                searchList=[self])
        self.head_file.write(str(layout_h))
        self.impl_file.write(str(layout_c))
        self.doc_file.write(str(apidoc_ns_script))

        self._parse_headers()

        layout_h = Template(file=os.path.join(self.target, "templates", "layout_foot.h"),
                            searchList=[self])
        layout_c = Template(file=os.path.join(self.target, "templates", "layout_foot.c"),
                            searchList=[self])
        self.head_file.write(str(layout_h))
        self.impl_file.write(str(layout_c))
        if self.script_type == "lua":
            apidoc_ns_foot_script = Template(file=os.path.join(self.target, "templates", "apidoc_ns_foot.script"),
                                searchList=[self])
            self.doc_file.write(str(apidoc_ns_foot_script))

        self.impl_file.close()
        self.head_file.close()
        self.doc_file.close()


    def _pretty_print(self, diagnostics):
        print("====\nErrors in parsing headers:")
        severities=['Ignored', 'Note', 'Warning', 'Error', 'Fatal']
        for idx, d in enumerate(diagnostics):
            print "%s. <severity = %s,\n    location = %r,\n    details = %r>" % (
                idx+1, severities[d.severity], d.location, d.spelling)
        print("====\n")

    def _parse_headers(self):
        for header in self.headers:
            tu = self.index.parse(header, self.clang_args)
            if len(tu.diagnostics) > 0:
                self._pretty_print(tu.diagnostics)
                is_fatal = False
                for d in tu.diagnostics:
                    if d.severity >= cindex.Diagnostic.Error:
                        is_fatal = True
                if is_fatal:
                    print("*** Found errors - can not continue")
                    raise Exception("Fatal error in parsing headers")
            self._deep_iterate(tu.cursor)

    def _deep_iterate(self, cursor, depth=0):
        # get the canonical type
        if cursor.kind == cindex.CursorKind.CLASS_DECL:
            if cursor == cursor.type.get_declaration() and len(cursor.get_children_array()) > 0:
                is_targeted_class = True
                if self.cpp_ns:
                    is_targeted_class = False
                    namespaced_name = get_namespaced_name(cursor)
                    for ns in self.cpp_ns:
                        if namespaced_name.startswith(ns):
                            is_targeted_class = True
                            break

                if is_targeted_class and self.in_listed_classes(cursor.displayname):
                    if not self.generated_classes.has_key(cursor.displayname):
                        nclass = NativeClass(cursor, self)
                        nclass.generate_code()
                        self.generated_classes[cursor.displayname] = nclass
                    return

        for node in cursor.get_children():
            # print("%s %s - %s" % (">" * depth, node.displayname, node.kind))
            self._deep_iterate(node, depth + 1)
    def scriptname_from_native(self, namespace_class_name, namespace_name):
        script_ns_dict = self.config['conversions']['ns_map']
        for (k, v) in script_ns_dict.items():
            if k == namespace_name:
                return namespace_class_name.replace("*","").replace("const ", "").replace(k, v)
        if namespace_class_name.find("::") >= 0:
            if namespace_class_name.find("std::") == 0:
                return namespace_class_name
            else:
                raise Exception("The namespace (%s) conversion wasn't set in 'ns_map' section of the conversions.yaml" % namespace_class_name)
        else:
            return namespace_class_name.replace("*","").replace("const ", "")

    def is_cocos_class(self, namespace_class_name):
        script_ns_dict = self.config['conversions']['ns_map']
        for (k, v) in script_ns_dict.items():
            if namespace_class_name.find("std::") == 0:
                return False
            if namespace_class_name.find(k) >= 0:
                return True

        return False

    def scriptname_cocos_class(self, namespace_class_name):
        script_ns_dict = self.config['conversions']['ns_map']
        for (k, v) in script_ns_dict.items():
            if namespace_class_name.find(k) >= 0:
                return namespace_class_name.replace("*","").replace("const ", "").replace(k,v)
        raise Exception("The namespace (%s) conversion wasn't set in 'ns_map' section of the conversions.yaml" % namespace_class_name)

    def js_typename_from_natve(self, namespace_class_name):
        script_ns_dict = self.config['conversions']['ns_map']
        if namespace_class_name.find("std::") == 0:
            if namespace_class_name.find("std::string") == 0:
                return "String"
            if namespace_class_name.find("std::vector") == 0:
                return "Array"
            if namespace_class_name.find("std::map") == 0 or namespace_class_name.find("std::unordered_map") == 0:
                return "map_object"
            if namespace_class_name.find("std::function") == 0:
                return "function"

        for (k, v) in script_ns_dict.items():
            if namespace_class_name.find(k) >= 0:
                if namespace_class_name.find("CrossApp::CAVector") == 0:
                    return "Array"
                if namespace_class_name.find("CrossApp::CAMap") == 0:
                    return "map_object"
                if namespace_class_name.find("CrossApp::DPoint")  == 0:
                    return "point_object"
                if namespace_class_name.find("CrossApp::DSize")  == 0:
                    return "size_object"
                if namespace_class_name.find("CrossApp::DRect")  == 0:
                    return "rect_object"
                if namespace_class_name.find("CrossApp::CAColor4B") == 0:
                    return "color4b_object"
                if namespace_class_name.find("CrossApp::CAColor4F") == 0:
                    return "color4f_object"
                if namespace_class_name.find("CrossApp::CAFont") == 0:
                    return "font_object"
                if namespace_class_name.find("CrossApp::DVerticalLayout") == 0:
                    return "verticalLayout_object"
                if namespace_class_name.find("CrossApp::DHorizontalLayout") == 0:
                    return "horizontalLayout_object"
                if namespace_class_name.find("CrossApp::DLayout") == 0:
                    return "layout_object"
                else:
                    return namespace_class_name.replace("*","").replace("const ", "").replace(k,v)
        return namespace_class_name.replace("*","").replace("const ", "")

    def lua_typename_from_natve(self, namespace_class_name, is_ret = False):
        script_ns_dict = self.config['conversions']['ns_map']
        if namespace_class_name.find("std::") == 0:
            if namespace_class_name.find("std::string") == 0:
                return "string"
            if namespace_class_name.find("std::vector") == 0:
                return "array_table"
            if namespace_class_name.find("std::map") == 0 or namespace_class_name.find("std::unordered_map") == 0:
                return "map_table"
            if namespace_class_name.find("std::function") == 0:
                return "function"

        for (k, v) in script_ns_dict.items():
            if namespace_class_name.find(k) >= 0:
                if namespace_class_name.find("cocos2d::Vec2") == 0:
                    return "vec2_table"
                if namespace_class_name.find("cocos2d::Vec3") == 0:
                    return "vec3_table"
                if namespace_class_name.find("cocos2d::Vec4") == 0:
                    return "vec4_table"
                if namespace_class_name.find("cocos2d::Vector") == 0:
                    return "array_table"
                if namespace_class_name.find("cocos2d::Mat4") == 0:
                    return "mat4_table"
                if namespace_class_name.find("cocos2d::Map") == 0:
                    return "map_table"
                if namespace_class_name.find("cocos2d::Point")  == 0:
                    return "point_table"
                if namespace_class_name.find("cocos2d::Size")  == 0:
                    return "size_table"
                if namespace_class_name.find("cocos2d::Rect")  == 0:
                    return "rect_table"
                if namespace_class_name.find("cocos2d::Color3B") == 0:
                    return "color3b_table"
                if namespace_class_name.find("cocos2d::Color4B") == 0:
                    return "color4b_table"
                if namespace_class_name.find("cocos2d::Color4F") == 0:
                    return "color4f_table"
                if is_ret == 1:
                    return namespace_class_name.replace("*","").replace("const ", "").replace(k,"")
                else:
                    return namespace_class_name.replace("*","").replace("const ", "").replace(k,v)
        return namespace_class_name.replace("*","").replace("const ","")

    def api_param_name_from_native(self,native_name):
        lower_name = native_name.lower()
        if lower_name == "std::string":
            return "str"

        if lower_name.find("unsigned ") >= 0 :
            return native_name.replace("unsigned ","")

        if lower_name.find("unordered_map") >= 0 or lower_name.find("map") >= 0:
            return "map"

        if lower_name.find("vector") >= 0 :
            return "array"

        if lower_name == "std::function":
            return "func"
        else:
            return lower_name

    def js_ret_name_from_native(self, namespace_class_name, is_enum) :
        if self.is_cocos_class(namespace_class_name):
            if namespace_class_name.find("CrossApp::CAVector") >=0:
                return "new Array()"
            if namespace_class_name.find("CrossApp::CAMap") >=0:
                return "map_object"
            if is_enum:
                return 0
            else:
                return self.scriptname_cocos_class(namespace_class_name)

        lower_name = namespace_class_name.lower()

        if lower_name.find("unsigned ") >= 0:
            lower_name = lower_name.replace("unsigned ","")

        if lower_name == "std::string":
            return ""

        if lower_name == "char" or lower_name == "short" or lower_name == "int" or lower_name == "float" or lower_name == "double" or lower_name == "long":
            return 0

        if lower_name == "bool":
            return "false"

        if lower_name.find("std::vector") >= 0 or lower_name.find("vector") >= 0:
            return "new Array()"

        if lower_name.find("std::map") >= 0 or lower_name.find("std::unordered_map") >= 0 or lower_name.find("unordered_map") >= 0 or lower_name.find("map") >= 0:
            return "map_object"

        if lower_name == "std::function":
            return "func"
        else:
            return namespace_class_name
def main():
    print'*****************main*************'

    from optparse import OptionParser

    parser = OptionParser("usage: %prog [options] {configfile}")
    parser.add_option("-s", action="store", type="string", dest="section",
                        help="sets a specific section to be converted")
    parser.add_option("-t", action="store", type="string", dest="target",
                        help="specifies the target vm. Will search for TARGET.yaml")
    parser.add_option("-o", action="store", type="string", dest="outdir",
                        help="specifies the output directory for generated C++ code")
    parser.add_option("-n", action="store", type="string", dest="out_file",
                        help="specifcies the name of the output file, defaults to the prefix in the .ini file")

    (opts, args) = parser.parse_args()

    #print '++++++++++++++++++++++++++++++++++++888888888'
    #print opts.outdir
    #print opts.target
    #print opts.out_file
    #print opts.section

    # script directory
    workingdir = os.path.dirname(inspect.getfile(inspect.currentframe()))

    if len(args) == 0:
        parser.error('invalid number of arguments')

    userconfig = ConfigParser.SafeConfigParser()
    userconfig.read('userconf.ini')
    print 'Using userconfig \n ', userconfig.items('DEFAULT')

    config = ConfigParser.SafeConfigParser()
    config.read(args[0])
    print '000000000000000000'
    print args[0]
    print '000000000000000000'

    if (0 == len(config.sections())):
        raise Exception("No sections defined in config file")

    sections = []
    if opts.section:
        if (opts.section in config.sections()):
            sections = []
            sections.append(opts.section)
        else:
            raise Exception("Section not found in config file")
    else:
        print("processing all sections")
        sections = config.sections()

    # find available targets
    targetdir = os.path.join(workingdir, "targets")
    targets = []
    if (os.path.isdir(targetdir)):
        targets = [entry for entry in os.listdir(targetdir)
                    if (os.path.isdir(os.path.join(targetdir, entry)))]
    if 0 == len(targets):
        raise Exception("No targets defined")

    if opts.target:
        if (opts.target in targets):
            targets = []
            targets.append(opts.target)

    if opts.outdir:
        outdir = opts.outdir
    else:
        outdir = os.path.join(workingdir, "gen")
    if not os.path.exists(outdir):
        os.makedirs(outdir)

    for t in targets:
        # Fix for hidden '.svn', '.cvs' and '.git' etc. folders - these must be ignored or otherwise they will be interpreted as a target.
        if t == ".svn" or t == ".cvs" or t == ".git" or t == ".gitignore":
            continue

        print "\n.... Generating bindings for target", t
        for s in sections:
            print "\n.... .... Processing section", s, "\n"
            gen_opts = {
                'prefix': config.get(s, 'prefix'),
                'headers':    (config.get(s, 'headers'        , 0, dict(userconfig.items('DEFAULT')))),
                'replace_headers': config.get(s, 'replace_headers') if config.has_option(s, 'replace_headers') else None,
                'classes': config.get(s, 'classes').split(' '),
                'classes_need_extend': config.get(s, 'classes_need_extend').split(' ') if config.has_option(s, 'classes_need_extend') else [],
                'clang_args': (config.get(s, 'extra_arguments', 0, dict(userconfig.items('DEFAULT'))) or "").split(" "),
                'target': os.path.join(workingdir, "targets", t),
                'outdir': outdir,
                'remove_prefix': config.get(s, 'remove_prefix'),
                'target_ns': config.get(s, 'target_namespace'),
                'cpp_ns': config.get(s, 'cpp_namespace').split(' ') if config.has_option(s, 'cpp_namespace') else None,
                'classes_have_no_parents': config.get(s, 'classes_have_no_parents'),
                'base_classes_to_skip': config.get(s, 'base_classes_to_skip'),
                'abstract_classes': config.get(s, 'abstract_classes'),
                'skip': config.get(s, 'skip'),
                'field': config.get(s, 'field') if config.has_option(s, 'field') else None,
                'rename_functions': config.get(s, 'rename_functions'),
                'rename_classes': config.get(s, 'rename_classes'),
                'out_file': opts.out_file or config.get(s, 'prefix'),
                'script_control_cpp': config.get(s, 'script_control_cpp') if config.has_option(s, 'script_control_cpp') else 'no',
                'script_type': t,
                'macro_judgement': config.get(s, 'macro_judgement') if config.has_option(s, 'macro_judgement') else None,
                'hpp_headers': config.get(s, 'hpp_headers', 0, dict(userconfig.items('DEFAULT'))).split(' ') if config.has_option(s, 'hpp_headers') else None,
                'cpp_headers': config.get(s, 'cpp_headers', 0, dict(userconfig.items('DEFAULT'))).split(' ') if config.has_option(s, 'cpp_headers') else None,
                'win32_clang_flags': (config.get(s, 'win32_clang_flags', 0, dict(userconfig.items('DEFAULT'))) or "").split(" ") if config.has_option(s, 'win32_clang_flags') else None
                }
            generator = Generator(gen_opts)
            generator.generate_code()

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        traceback.print_exc()
        sys.exit(1)
