#!/usr/bin/python
# ----------------------------------------------------------------------------
# cocos "jscompile" plugin
#
# Copyright 2013 (C) Intel
#
# License: MIT
# ----------------------------------------------------------------------------
'''
"jscompile" plugin for cocos command line tool
'''

__docformat__ = 'restructuredtext'

import sys
import subprocess
import os
import json
import inspect
import platform

import cocos
from MultiLanguage import MultiLanguage

class CCPluginJSCompile(cocos.CCPlugin):
    """
    compiles (encodes) and minifies JS files
    """
    @staticmethod
    def plugin_name():
        return "jscompile"

    @staticmethod
    def brief_description():
        # returns a short description of this module
        return MultiLanguage.get_string('JSCOMPILE_BRIEF')

    # This is not the constructor, just an initializator
    def init(self, options, workingdir):
        """
        Arguments:
        - `options`:
        """
        self._current_src_dir = None
        self._src_dir_arr = self.normalize_path_in_list(options.src_dir_arr)
        self._dst_dir = options.dst_dir
        self._use_closure_compiler = options.use_closure_compiler
        self._verbose = options.verbose
        self._config = None
        self._workingdir = workingdir
        self._closure_params = ''
        if options.compiler_config != None:
            f = open(options.compiler_config)
            self._config = json.load(f)
            f.close()

            self._pre_order = self._config["pre_order"]
            self.normalize_path_in_list(self._pre_order)
            self._post_order = self._config["post_order"]
            self.normalize_path_in_list(self._post_order)
            self._skip = self._config["skip"]
            self.normalize_path_in_list(self._skip)
            self._closure_params = self._config["closure_params"]

        
        if options.closure_params is not None:
            self._closure_params = options.closure_params

        self._js_files = {}
        self._compressed_js_path = os.path.join(self._dst_dir, options.compressed_filename)
        self._compressed_jsc_path = os.path.join(self._dst_dir, options.compressed_filename+"c")

        if(cocos.os_is_linux()):
            if(platform.architecture()[0] == "32bit"):
                self.jsbcc_exe_path = os.path.join(self._workingdir, "bin", "linux", "jsbcc_x86")
            else:
                self.jsbcc_exe_path = os.path.join(self._workingdir, "bin", "linux", "jsbcc_x64")
        else:
            self.jsbcc_exe_path = os.path.join(self._workingdir, "bin", "jsbcc")

    def normalize_path_in_list(self, list):
        for i in list:
            tmp = os.path.normpath(i)
            list[list.index(i)] = tmp
        return list

    def get_relative_path(self, jsfile):
        try:
            # print "current src dir: "+self._current_src_dir)
            pos = jsfile.index(self._current_src_dir)
            if pos != 0:
                raise cocos.CCPluginError(MultiLanguage.get_string('LUACOMPILE_ERROR_SRCDIR_NAME_NOT_FOUND'),
                                          cocos.CCPluginError.ERROR_WRONG_ARGS)
            # print "origin js path: "+ jsfile
            # print "relative path: "+jsfile[len(self._current_src_dir)+1:]
            return jsfile[len(self._current_src_dir)+1:]
        except ValueError:
            raise cocos.CCPluginError(MultiLanguage.get_string('LUACOMPILE_ERROR_SRCDIR_NAME_NOT_FOUND'),
                                      cocos.CCPluginError.ERROR_WRONG_ARGS)

    def get_output_file_path(self, jsfile):
        """
        Gets output file path by source js file
        """
        # create folder for generated file
        jsc_filepath = ""
        relative_path = self.get_relative_path(jsfile)+"c"
        jsc_filepath = os.path.join(self._dst_dir, relative_path)

        dst_rootpath = os.path.split(jsc_filepath)[0]
        try:
            # print "creating dir (%s)" % (dst_rootpath)
            os.makedirs(dst_rootpath)
        except OSError:
            if os.path.exists(dst_rootpath) == False:
                # There was an error on creation, so make sure we know about it
                raise cocos.CCPluginError(MultiLanguage.get_string('LUACOMPILE_ERROR_MKDIR_FAILED_FMT', dst_rootpath),
                                          cocos.CCPluginError.ERROR_PATH_NOT_FOUND)

        # print "return jsc path: "+jsc_filepath
        return jsc_filepath

    def compile_js(self, jsfile, output_file):
        """
        Compiles js file
        """
        cocos.Logging.debug(MultiLanguage.get_string('JSCOMPILE_DEBUG_COMPILE_FILE_FMT', jsfile))

        cmd_str = "\"%s\" \"%s\" \"%s\"" % (self.jsbcc_exe_path, jsfile, output_file)
        self._run_cmd(cmd_str)

    def compress_js(self):
        """
        Compress all js files into one big file.
        """
        jsfiles = ""
        for src_dir in self._src_dir_arr:
            # print "\n----------src:"+src_dir
            jsfiles = jsfiles + " --js ".join(self._js_files[src_dir]) + " "

        compiler_jar_path = os.path.join(self._workingdir, "bin", "compiler.jar")
        command = "java -jar \"%s\" %s --js %s --js_output_file \"%s\"" % (compiler_jar_path, self._closure_params, jsfiles, self._compressed_js_path)
        self._run_cmd(command)

    def deep_iterate_dir(self, rootDir):
        for lists in os.listdir(rootDir):
            path = os.path.join(rootDir, lists)
            if os.path.isdir(path):
                self.deep_iterate_dir(path)
            elif os.path.isfile(path):
                if os.path.splitext(path)[1] == ".js":
                    self._js_files[self._current_src_dir].append(path)


    def index_in_list(self, jsfile, l):
        """
        Arguments:
        - `self`:
        - `jsfile`:
        - `l`:
        """
        index = -1

        for el in l:
            if jsfile.rfind(el) != -1:
                # print "index:"+str(index+1)+", el:"+el
                return index+1
            index = index + 1
        return -1

    def js_filename_pre_order_compare(self, a, b):
        return self._js_filename_compare(a, b, self._pre_order, 1)

    def js_filename_post_order_compare(self, a, b):
        return self._js_filename_compare(a, b, self._post_order, -1)

    def _js_filename_compare(self, a, b, files, delta):
        index_a = self.index_in_list(a, files)
        index_b = self.index_in_list(b, files)
        is_a_in_list = index_a != -1
        is_b_in_list = index_b != -1

        if is_a_in_list and not is_b_in_list:
            return -1 * delta
        elif not is_a_in_list and is_b_in_list:
            return 1 * delta
        elif is_a_in_list and is_b_in_list:
            if index_a > index_b:
                return 1
            elif index_a < index_b:
                return -1
            else:
                return 0
        else:
            return 0

    def reorder_js_files(self):
        if self._config == None:
            return

        # print "before:"+str(self._js_files)

        for src_dir in self._js_files:
            # Remove file in exclude list
            need_remove_arr = []
            for jsfile in self._js_files[src_dir]:
                for exclude_file in self._skip:
                    if jsfile.rfind(exclude_file) != -1:
                        # print "remove:" + jsfile
                        need_remove_arr.append(jsfile)

            for need_remove in need_remove_arr:
                self._js_files[src_dir].remove(need_remove)

            self._js_files[src_dir].sort(cmp=self.js_filename_pre_order_compare)
            self._js_files[src_dir].sort(cmp=self.js_filename_post_order_compare)

        # print '-------------------'
        # print "after:" + str(self._js_files)

    def handle_all_js_files(self):
        """
        Arguments:
        - `self`:
        """
        if self._use_closure_compiler == True:
            cocos.Logging.info(MultiLanguage.get_string('JSCOMPILE_INFO_COMPRESS_TIP'))
            self.compress_js()
            self.compile_js(self._compressed_js_path, self._compressed_jsc_path)
            # remove tmp compressed file
            os.remove(self._compressed_js_path)
        else:
            cocos.Logging.info(MultiLanguage.get_string('JSCOMPILE_INFO_COMPILE_TO_BYTECODE'))
            for src_dir in self._src_dir_arr:
                for jsfile in self._js_files[src_dir]:
                    self._current_src_dir = src_dir
                    self.compile_js(jsfile, self.get_output_file_path(jsfile))

    # will be called from the cocos.py script
    def run(self, argv, dependencies):
        """
        """
        self.parse_args(argv)

        # create output directory
        try:
            os.makedirs(self._dst_dir)
        except OSError:
            if os.path.exists(self._dst_dir) == False:
                raise cocos.CCPluginError(MultiLanguage.get_string('LUACOMPILE_ERROR_MKDIR_FAILED_FMT', self._dst_dir),
                                          cocos.CCPluginError.ERROR_PATH_NOT_FOUND)

        # download the bin folder
        if not os.path.exists(self.jsbcc_exe_path):
            download_cmd_path = os.path.join(self._workingdir, os.pardir, os.pardir)
            subprocess.call("python %s -f -r no" % (os.path.join(download_cmd_path, "download-bin.py")), shell=True, cwd=download_cmd_path)

        # deep iterate the src directory
        for src_dir in self._src_dir_arr:
            self._current_src_dir = src_dir
            self._js_files[self._current_src_dir] = []
            self.deep_iterate_dir(src_dir)

        self.reorder_js_files()
        self.handle_all_js_files()
        cocos.Logging.info(MultiLanguage.get_string('LUACOMPILE_INFO_FINISHED'))

    def parse_args(self, argv):
        """
        """
        from argparse import ArgumentParser

        parser = ArgumentParser(prog="cocos %s" % self.__class__.plugin_name(),
                                description=self.__class__.brief_description())
        parser.add_argument("-v", "--verbose",
                          action="store_true",
                          dest="verbose",
                          help=MultiLanguage.get_string('LUACOMPILE_ARG_VERBOSE'))
        parser.add_argument("-s", "--src",
                          action="append", dest="src_dir_arr",
                          help=MultiLanguage.get_string('JSCOMPILE_ARG_SRC'))

        parser.add_argument("-d", "--dst",
                          action="store", dest="dst_dir",
                          help=MultiLanguage.get_string('JSCOMPILE_ARG_DST'))

        parser.add_argument("-c", "--use_closure_compiler",
                          action="store_true", dest="use_closure_compiler", default=False,
                          help=MultiLanguage.get_string('JSCOMPILE_ARG_CLOSURE'))

        parser.add_argument("-o", "--output_compressed_filename",
                          action="store", dest="compressed_filename", default="game.min.js",
                          help=MultiLanguage.get_string('JSCOMPILE_ARG_OUT_FILE_NAME'))

        parser.add_argument("-j", "--compiler_config",
                          action="store", dest="compiler_config",
                          help=MultiLanguage.get_string('JSCOMPILE_ARG_JSON_FILE'))
        parser.add_argument("-m", "--closure_params",
                          action="store", dest="closure_params",
                          help=MultiLanguage.get_string('JSCOMPILE_ARG_EXTRA_PARAM'))

        options = parser.parse_args(argv)

        if options.src_dir_arr == None:
            raise cocos.CCPluginError(MultiLanguage.get_string('JSCOMPILE_ERROR_SRC_NOT_SPECIFIED'),
                                      cocos.CCPluginError.ERROR_WRONG_ARGS)
        elif options.dst_dir == None:
            raise cocos.CCPluginError(MultiLanguage.get_string('LUACOMPILE_ERROR_DST_NOT_SPECIFIED'),
                                      cocos.CCPluginError.ERROR_WRONG_ARGS)
        else:
            for src_dir in options.src_dir_arr:
                if os.path.exists(src_dir) == False:
                    raise cocos.CCPluginError(MultiLanguage.get_string('LUACOMPILE_ERROR_DIR_NOT_EXISTED_FMT',
                                                                       (src_dir)),
                                              cocos.CCPluginError.ERROR_PATH_NOT_FOUND)


        # script directory
        if getattr(sys, 'frozen', None):
            workingdir = os.path.realpath(os.path.dirname(sys.executable))
        else:
            workingdir = os.path.realpath(os.path.dirname(__file__))

        self.init(options, workingdir)

