/*
 * Copyright (c) 2015 Chukong Technologies Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//
// CrossApp boot
//

//+++++++++++++++++++++++++something about async begin+++++++++++++++++++++++++++++++
/**
 * Async Pool class, a helper of ca.async
 * @param {Object|Array} srcObj
 * @param {Number} limit the limit of parallel number
 * @param {function} iterator
 * @param {function} onEnd
 * @param {object} target
 * @constructor
 */
ca.AsyncPool = function(srcObj, limit, iterator, onEnd, target){
    var self = this;
    self._srcObj = srcObj;
    self._limit = limit;
    self._pool = [];
    self._iterator = iterator;
    self._iteratorTarget = target;
    self._onEnd = onEnd;
    self._onEndTarget = target;
    self._results = srcObj instanceof Array ? [] : {};
    self._isErr = false;
    
    ca.each(srcObj, function(value, index){
            self._pool.push({index : index, value : value});
            });
    
    self.size = self._pool.length;
    self.finishedSize = 0;
    self._workingSize = 0;
    
    self._limit = self._limit || self.size;
    
    self.onIterator = function(iterator, target){
        self._iterator = iterator;
        self._iteratorTarget = target;
    };
    
    self.onEnd = function(endCb, endCbTarget){
        self._onEnd = endCb;
        self._onEndTarget = endCbTarget;
    };
    
    self._handleItem = function(){
        var self = this;
        if(self._pool.length == 0)
            return;                                                         //return directly if the array's length = 0
        if(self._workingSize >= self._limit)
            return;                                                         //return directly if the working size great equal limit number
        var item = self._pool.shift();
        var value = item.value, index = item.index;
        self._workingSize++;
        self._iterator.call(self._iteratorTarget, value, index, function(err){
                            if(self._isErr)
                            return;
                            
                            self.finishedSize++;
                            self._workingSize--;
                            if(err) {
                            self._isErr = true;
                            if(self._onEnd)
                            self._onEnd.call(self._onEndTarget, err);
                            return
                            }
                            
                            var arr = Array.prototype.slice.call(arguments, 1);
                            self._results[this.index] = arr[0];
                            if (self.finishedSize === self.size) {
                            if (self._onEnd)
                            self._onEnd.call(self._onEndTarget, null, self._results);
                            return;
                            }
                            self._handleItem();
                            }.bind(item), self);
    };
    
    self.flow = function(){
        var self = this;
        if(self._pool.length === 0) {
            if(self._onEnd)
                self._onEnd.call(self._onEndTarget, null, []);
            return;
        }
        for(var i = 0; i < self._limit; i++)
            self._handleItem();
    }
};

/**
 * @class
 */
ca.async = /** @lends ca.async# */{
    /**
     * Do tasks series.
     * @param {Array|Object} tasks
     * @param {function} [cb] callback
     * @param {Object} [target]
     * @return {ca.AsyncPool}
     */
    series : function(tasks, cb, target){
        var asyncPool = new ca.AsyncPool(tasks, 1, function(func, index, cb1){
                                         func.call(target, cb1);
                                         }, cb, target);
        asyncPool.flow();
        return asyncPool;
    },
    
    /**
     * Do tasks parallel.
     * @param {Array|Object} tasks
     * @param {function} cb callback
     * @param {Object} [target]
     * @return {ca.AsyncPool}
     */
    parallel : function(tasks, cb, target){
        var asyncPool = new ca.AsyncPool(tasks, 0, function(func, index, cb1){
                                         func.call(target, cb1);
                                         }, cb, target);
        asyncPool.flow();
        return asyncPool;
    },
    
    /**
     * Do tasks waterfall.
     * @param {Array|Object} tasks
     * @param {function} cb callback
     * @param {Object} [target]
     * @return {ca.AsyncPool}
     */
    waterfall : function(tasks, cb, target){
        var args = [];
        var lastResults = [null];//the array to store the last results
        var asyncPool = new ca.AsyncPool(tasks, 1,
                                         function (func, index, cb1) {
                                         args.push(function (err) {
                                                   args = Array.prototype.slice.call(arguments, 1);
                                                   if(tasks.length - 1 === index) lastResults = lastResults.concat(args);//while the last task
                                                   cb1.apply(null, arguments);
                                                   });
                                         func.apply(target, args);
                                         }, function (err) {
                                         if (!cb)
                                         return;
                                         if (err)
                                         return cb.call(target, err);
                                         cb.apply(target, lastResults);
                                         });
        asyncPool.flow();
        return asyncPool;
    },
    
    /**
     * Do tasks by iterator.
     * @param {Array|Object} tasks
     * @param {function|Object} iterator
     * @param {function} [callback]
     * @param {Object} [target]
     * @return {ca.AsyncPool}
     */
    map : function(tasks, iterator, callback, target){
        var locIterator = iterator;
        if(typeof(iterator) === "object"){
            callback = iterator.cb;
            target = iterator.iteratorTarget;
            locIterator = iterator.iterator;
        }
        var asyncPool = new ca.AsyncPool(tasks, 0, locIterator, callback, target);
        asyncPool.flow();
        return asyncPool;
    },
    
    /**
     * Do tasks by iterator limit.
     * @param {Array|Object} tasks
     * @param {Number} limit
     * @param {function} iterator
     * @param {function} cb callback
     * @param {Object} [target]
     */
    mapLimit : function(tasks, limit, iterator, cb, target){
        var asyncPool = new ca.AsyncPool(tasks, limit, iterator, cb, target);
        asyncPool.flow();
        return asyncPool;
    }
};
//+++++++++++++++++++++++++something about async end+++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++something about path begin++++++++++++++++++++++++++++++++
/**
 * @class
 */
ca.path = /** @lends ca.path# */{
    normalizeRE: /[^\.\/]+\/\.\.\//,
    
    /**
     * Join strings to be a path.
     * @example
     ca.path.join("a", "b.png");//-->"a/b.png"
     ca.path.join("a", "b", "c.png");//-->"a/b/c.png"
     ca.path.join("a", "b");//-->"a/b"
     ca.path.join("a", "b", "/");//-->"a/b/"
     ca.path.join("a", "b/", "/");//-->"a/b/"
     * @returns {string}
     */
    join: function () {
        var l = arguments.length;
        var result = "";
        for (var i = 0; i < l; i++) {
        result = (result + (result === "" ? "" : "/") + arguments[i]).replace(/(\/|\\\\)$/, "");
        }
    return result;
    },
    
    /**
     * Get the ext name of a path.
     * @example
     ca.path.extname("a/b.png");//-->".png"
     ca.path.extname("a/b.png?a=1&b=2");//-->".png"
     ca.path.extname("a/b");//-->null
     ca.path.extname("a/b?a=1&b=2");//-->null
     * @param {string} pathStr
     * @returns {*}
     */
    extname: function (pathStr) {
        var temp = /(\.[^\.\/\?\\]*)(\?.*)?$/.exec(pathStr);
        return temp ? temp[1] : null;
    },
    
    /**
     * Get the main name of a file name
     * @param {string} fileName
     * @returns {string}
     */
    mainFileName: function(fileName){
        if(fileName){
            var idx = fileName.lastIndexOf(".");
            if(idx !== -1)
                return fileName.substring(0,idx);
        }
        return fileName;
    },
    
    /**
     * Get the file name of a file path.
     * @example
     ca.path.basename("a/b.png");//-->"b.png"
     ca.path.basename("a/b.png?a=1&b=2");//-->"b.png"
     ca.path.basename("a/b.png", ".png");//-->"b"
     ca.path.basename("a/b.png?a=1&b=2", ".png");//-->"b"
     ca.path.basename("a/b.png", ".txt");//-->"b.png"
     * @param {string} pathStr
     * @param {string} [extname]
     * @returns {*}
     */
    basename: function (pathStr, extname) {
        var index = pathStr.indexOf("?");
        if (index > 0) pathStr = pathStr.substring(0, index);
        var reg = /(\/|\\\\)([^(\/|\\\\)]+)$/g;
        var result = reg.exec(pathStr.replace(/(\/|\\\\)$/, ""));
        if (!result) return null;
        var baseName = result[2];
        if (extname && pathStr.substring(pathStr.length - extname.length).toLowerCase() === extname.toLowerCase())
            return baseName.substring(0, baseName.length - extname.length);
        return baseName;
    },
    
    /**
     * Get dirname of a file path.
     * @example
     * unix
     ca.path.driname("a/b/c.png");//-->"a/b"
     ca.path.driname("a/b/c.png?a=1&b=2");//-->"a/b"
     ca.path.dirname("a/b/");//-->"a/b"
     ca.path.dirname("c.png");//-->""
     * windows
     ca.path.driname("a\\b\\c.png");//-->"a\b"
     ca.path.driname("a\\b\\c.png?a=1&b=2");//-->"a\b"
     * @param {string} pathStr
     * @returns {*}
     */
    dirname: function (pathStr) {
        return pathStr.replace(/((.*)(\/|\\|\\\\))?(.*?\..*$)?/, '$2');
    },
    
    /**
     * Change extname of a file path.
     * @example
     ca.path.changeExtname("a/b.png", ".plist");//-->"a/b.plist"
     ca.path.changeExtname("a/b.png?a=1&b=2", ".plist");//-->"a/b.plist?a=1&b=2"
     * @param {string} pathStr
     * @param {string} [extname]
     * @returns {string}
     */
    changeExtname: function (pathStr, extname) {
        extname = extname || "";
        var index = pathStr.indexOf("?");
        var tempStr = "";
        if (index > 0) {
            tempStr = pathStr.substring(index);
            pathStr = pathStr.substring(0, index);
        }
        index = pathStr.lastIndexOf(".");
        if (index < 0) return pathStr + extname + tempStr;
        return pathStr.substring(0, index) + extname + tempStr;
    },
    /**
     * Change file name of a file path.
     * @example
     ca.path.changeBasename("a/b/c.plist", "b.plist");//-->"a/b/b.plist"
     ca.path.changeBasename("a/b/c.plist?a=1&b=2", "b.plist");//-->"a/b/b.plist?a=1&b=2"
     ca.path.changeBasename("a/b/c.plist", ".png");//-->"a/b/c.png"
     ca.path.changeBasename("a/b/c.plist", "b");//-->"a/b/b"
     ca.path.changeBasename("a/b/c.plist", "b", true);//-->"a/b/b.plist"
     * @param {String} pathStr
     * @param {String} basename
     * @param {Boolean} [isSameExt]
     * @returns {string}
     */
    changeBasename: function (pathStr, basename, isSameExt) {
        if (basename.indexOf(".") === 0) return this.changeExtname(pathStr, basename);
        var index = pathStr.indexOf("?");
        var tempStr = "";
        var ext = isSameExt ? this.extname(pathStr) : "";
        if (index > 0) {
            tempStr = pathStr.substring(index);
            pathStr = pathStr.substring(0, index);
        }
        index = pathStr.lastIndexOf("/");
        index = index <= 0 ? 0 : index + 1;
        return pathStr.substring(0, index) + basename + ext + tempStr;
    },
    //todo make public after verification
    _normalize: function(url){
        var oldUrl = url = String(url);
    
        //removing all ../
        do {
            oldUrl = url;
            url = url.replace(this.normalizeRE, "");
        } while(oldUrl.length !== url.length);
        return url;
    }
};
//+++++++++++++++++++++++++something about path end++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++something about loader start+++++++++++++++++++++++++++
ca.loader = {
    _resPath : "",
    _audioPath : "",
    _register : {},//register of loaders
    cache : {},//cache for data loaded
    _langPathCache : {},//cache for lang path
    
    /**
     * Get XMLHttpRequest.
     * @returns {XMLHttpRequest}
     */
    getXMLHttpRequest : function (thread_id) {
        return new XMLHttpRequest(thread_id);
    },
    
    
    //@MODE_BEGIN DEV
    
    _jsCache : {},//cache for js
    
_getArgs4Js: function (args) {
    var a0 = args[0], a1 = args[1], a2 = args[2], results = ["", null, null];
    
    if (args.length === 1) {
        results[1] = a0 instanceof Array ? a0 : [a0];
    } else if (args.length === 2) {
        if (typeof a1 === "function") {
            results[1] = a0 instanceof Array ? a0 : [a0];
            results[2] = a1;
        } else {
            results[0] = a0 || "";
            results[1] = a1 instanceof Array ? a1 : [a1];
        }
    } else if (args.length === 3) {
        results[0] = a0 || "";
        results[1] = a1 instanceof Array ? a1 : [a1];
        results[2] = a2;
    } else throw new Error("arguments error to load js!");
    return results;
},
    
    /**
     * Load js files.
     * @param {?string=} baseDir   The pre path for jsList.
     * @param {array.<string>} jsList    List of js path.
     * @param {function} cb        Callback function
     *
     *      If the arguments.length == 2, then the baseDir turns to be "".
     * @returns {*}
     */
loadJs: function (baseDir, jsList, cb) {
    var self = this, localJsCache = self._jsCache,
    args = self._getArgs4Js(arguments);
    baseDir = args[0];
    jsList = args[1];
    cb = args[2];
    var ccPath = ca.path;
    for(var i = 0, li = jsList.length; i < li; ++i){
        require(ccPath.join(baseDir, jsList[i]));
    }
    if(cb) cb();
},
    /**
     * Load js width loading image.
     *
     * @param {string} [baseDir]
     * @param {array} jsList
     * @param {function} [cb]
     */
    loadJsWithImg : function(baseDir, jsList, cb){
        this.loadJs.apply(this, arguments);
    },
    
    //@MODE_END DEV
    
    /**
     * Load a single resource as txt.
     * @param {!string} url
     * @param {function} cb arguments are : err, txt
     */
    loadTxt : function(url, cb){
        cb(null, jsb.fileUtils.getFileString(url));
    },
    
    loadJson : function(url, cb){
        this.loadTxt(url, function(err, txt){
                     try{
                     err ? cb(err) : cb(null, JSON.parse(txt));
                     }catch(e){
                     throw e;
                     cb("load json [" + url + "] failed : " + e);
                     }
                     });
    },
    
    /**
     * Load a single image.
     * @param {!string} url
     * @param {object} [option]
     * @param {function} cb
     * @returns {Image}
     */
loadImg: function (url, option, cb){
    var l = arguments.length;
    if(l == 2) cb = option;
    
    if (url.match(jsb.urlRegExp)) {
        jsb.loadRemoteImg(url, function(succeed, image) {
                          if (succeed) {
                          cb && cb(null, image);
                          }
                          else {
                          cb && cb("Load image failed");
                          }
                          });
    }
    else {
        ca.imageCache.addImageAsync(url, function (image){
                                       if (image instanceof ca.CAImage)
                                       cb && cb(null, image);
                                       else cb && cb("Load image failed");
                                       });
    }
},
    /**
     * Load binary data by url.
     * @param {String} url
     * @param {Function} cb
     */
    loadBinary : function(url, cb){
        cb(null, jsb.fileUtils.getDataFromFile(url));
    },
    loadBinarySync : function(url){
        return jsb.fileUtils.getDataFromFile(url);
    },
    
    /**
     * Iterator function to load res
     * @param {object} item
     * @param {number} index
     * @param {function} [cb]
     * @returns {*}
     * @private
     */
_loadResIterator: function (item, index, cb) {
    var self = this, url = null;
    var type = item.type;
    if (type) {
        type = "." + type.toLowerCase();
        url = item.src ? item.src : item.name + type;
    } else {
        url = item;
        type = ca.path.extname(url);
    }
    
    var obj = self.cache[url];
    if (obj)
        return cb(null, obj);
    var loader = null;
    if (type) {
        loader = self._register[type.toLowerCase()];
    }
    if (!loader) {
        ca.error("loader for [" + type + "] not exists!");
        return cb();
    }
    var basePath = loader.getBasePath ? loader.getBasePath() : self.resPath;
    var realUrl = self.getUrl(basePath, url);
    
    loader.load(realUrl, url, item, function (err, data) {
                if (err) {
                ca.log(err);
                self.cache[url] = null;
                delete self.cache[url];
                cb();
                } else {
                self.cache[url] = data;
                cb(null, data);
                }
                });
},
    /**
     * Get url with basePath.
     * @param {string} basePath
     * @param {string} [url]
     * @returns {*}
     */
getUrl: function (basePath, url) {
    var self = this, langPathCache = self._langPathCache, path = ca.path;
    if (basePath !== undefined && url === undefined) {
        url = basePath;
        var type = path.extname(url);
        type = type ? type.toLowerCase() : "";
        var loader = self._register[type];
        if (!loader)
            basePath = self.resPath;
        else
            basePath = loader.getBasePath ? loader.getBasePath() : self.resPath;
    }
    url = ca.path.join(basePath || "", url);
    if (url.match(/[\/(\\\\)]lang[\/(\\\\)]/i)) {
        if (langPathCache[url])
            return langPathCache[url];
        var extname = path.extname(url) || "";
        url = langPathCache[url] = url.substring(0, url.length - extname.length) + "_" + ca.sys.language + extname;
    }
    return url;
},
 /**
     * Load resources then call the callback.
     * @param {string} resources
     * @param {function} [option] callback or trigger
     * @param {function|Object} [loadCallback]
     * @return {ca.AsyncPool}
     */
    load : function(resources, option, loadCallback){
        var self = this;
        var len = arguments.length;
        if(len === 0)
            throw new Error("arguments error!");
        
        if(len === 3){
            if(typeof option === "function"){
                if(typeof loadCallback === "function")
                    option = {trigger : option, cb : loadCallback };
                else
                    option = { cb : option, cbTarget : loadCallback};
            }
        }else if(len === 2){
            if(typeof option === "function")
                option = {cb : option};
        }else if(len === 1){
            option = {};
        }
        
        if(!(resources instanceof Array))
            resources = [resources];
        var asyncPool = new ca.AsyncPool(
                                         resources, 0,
                                         function (value, index, AsyncPoolCallback, aPool) {
                                         self._loadResIterator(value, index, function (err) {
                                                               var arr = Array.prototype.slice.call(arguments, 1);
                                                               if (option.trigger)
                                                               option.trigger.call(option.triggerTarget, arr[0], aPool.size, aPool.finishedSize);   //call trigger
                                                               AsyncPoolCallback(err, arr[0]);
                                                               });
                                         },
                                         option.cb, option.cbTarget);
        asyncPool.flow();
        return asyncPool;
    },   
    

    /**
     * <p>
     *     Loads alias map from the contents of a filename.                                        <br/>
     *                                                                                                                 <br/>
     *     @note The plist file name should follow the format below:                                                   <br/>
     *     <?xml version="1.0" encoding="UTF-8"?>                                                                      <br/>
     *         <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">  <br/>
     *             <plist version="1.0">                                                                               <br/>
     *                 <dict>                                                                                          <br/>
     *                     <key>filenames</key>                                                                        <br/>
     *                     <dict>                                                                                      <br/>
     *                         <key>sounds/click.wav</key>                                                             <br/>
     *                         <string>sounds/click.caf</string>                                                       <br/>
     *                         <key>sounds/endgame.wav</key>                                                           <br/>
     *                         <string>sounds/endgame.caf</string>                                                     <br/>
     *                         <key>sounds/gem-0.wav</key>                                                             <br/>
     *                         <string>sounds/gem-0.caf</string>                                                       <br/>
     *                     </dict>                                                                                     <br/>
     *                     <key>metadata</key>                                                                         <br/>
     *                     <dict>                                                                                      <br/>
     *                         <key>version</key>                                                                      <br/>
     *                         <integer>1</integer>                                                                    <br/>
     *                     </dict>                                                                                     <br/>
     *                 </dict>                                                                                         <br/>
     *              </plist>                                                                                           <br/>
     * </p>
     * @param {String} filename  The plist file name.
     * @param {Function} cb     callback
     */
    loadAliases : function(url, cb){
        jsb.fileUtils.loadFilenameLookup(url);
        if(cb) cb();
    },
    
    /**
     * Register a resource loader into loader.
     * @param {string} extNames
     * @param {function} loader
     */
register: function (extNames, loader) {
    if (!extNames || !loader) return;
    var self = this;
    if (typeof extNames === "string")
        return this._register[extNames.trim().toLowerCase()] = loader;
    for (var i = 0, li = extNames.length; i < li; i++) {
        self._register["." + extNames[i].trim().toLowerCase()] = loader;
    }
},
    
    /**
     * Get resource data by url.
     * @param url
     * @returns {*}
     */
    getRes : function(url){
        var cached = this.cache[url];
        if (cached)
            return cached;
        var type = ca.path.extname(url);
        if (!type) return ca.log("ca.loader.getRes: Invalid url");
        var loader = this._register[type.toLowerCase()];
        if (!loader) return ca.log("ca.loader.getRes: loader for [" + type + "] not exists!");
        var basePath = loader.getBasePath ? loader.getBasePath() : this.resPath;
        var realUrl = this.getUrl(basePath, url);
        return loader.load(realUrl, url);
    },
};
ca.defineGetterSetter(ca.loader, "resPath", function(){
                      return this._resPath;
                      }, function(resPath){
                      this._resPath = resPath || "";
                      jsb.fileUtils.addSearchPath(this._resPath);
                      });
ca.defineGetterSetter(ca.loader, "audioPath", function(){
                      return this._audioPath;
                      }, function(audioPath){
                      this._audioPath = audioPath || "";
                      jsb.fileUtils.addSearchPath(this._audioPath);
                      });

//+++++++++++++++++++++++++something about loader end+++++++++++++++++++++++++++++

/**
 * A string tool to construct a string with format string.
 * for example:
 *      ca.formatStr("a: %d, b: %b", a, b);
 *      ca.formatStr(a, b, c);
 * @returns {String}
 */
ca.formatStr = function(){
    var args = arguments;
    var l = args.length;
    if(l < 1)
        return "";
    
    var str = args[0];
    var needToFormat = true;
    if(typeof str === "object"){
        needToFormat = false;
    }
    for(var i = 1; i < l; ++i){
        var arg = args[i];
        if(needToFormat){
            while(true){
                var result = null;
                if(typeof arg === "number"){
                    result = str.match(/(%d)|(%s)/);
                    if(result){
                        str = str.replace(/(%d)|(%s)/, arg);
                        break;
                    }
                }
                result = str.match(/%s/);
                if(result)
                    str = str.replace(/%s/, arg);
                else
                    str += "    " + arg;
                break;
            }
        }else
            str += "    " + arg;
    }
    return str;
};


//+++++++++++++++++++++++++Engine initialization function begin+++++++++++++++++++++++++++

ca.application = ca.CAApplication.getApplication();
ca.winSize = ca.application.getWinSize();

// File utils (Temporary, won't be accessible)
ca.fileUtils = ca.FileUtils.getInstance();
ca.fileUtils.setPopupNotify(false);

ca.imageCache = ca.application.getImageCache();
ca.scheduler = ca.application.getScheduler();
ca.actionManager = ca.application.getActionManager();
ca.notificationCenter = ca.application.getNotificationCenter();
/**
 * @type {Object}
 * @name jsb.fileUtils
 * jsb.fileUtils is the native file utils singleton object,
 * please refer to CrossApp API to know how to use it.
 * Only available in JSB
 */
jsb.fileUtils = ca.fileUtils;
delete ca.FileUtils;

/**
 * @type {Object}
 * @name jsb.reflection
 * jsb.reflection is a bridge to let you invoke Java static functions.
 * please refer to this document to know how to use it: http://www.CrossApp.org/docs/manual/framework/html5/v3/reflection/en
 * Only available on Android platform
 */
jsb.reflection = {
    callStaticMethod : function(){
        ca.log("not supported on current platform");
    }
};

var _initSys = function () {
    /**
     * System variables
     * @namespace
     * @name ca.sys
     */
    ca.sys = window.sys || {};
    var sys = ca.sys;
    
    /**
     * English language code
     * @memberof ca.sys
     * @name LANGUAGE_ENGLISH
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_ENGLISH = "en";
    
    /**
     * Chinese language code
     * @memberof ca.sys
     * @name LANGUAGE_CHINESE
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_CHINESE = "zh";
    
    /**
     * French language code
     * @memberof ca.sys
     * @name LANGUAGE_FRENCH
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_FRENCH = "fr";
    
    /**
     * Italian language code
     * @memberof ca.sys
     * @name LANGUAGE_ITALIAN
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_ITALIAN = "it";
    
    /**
     * German language code
     * @memberof ca.sys
     * @name LANGUAGE_GERMAN
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_GERMAN = "de";
    
    /**
     * Spanish language code
     * @memberof ca.sys
     * @name LANGUAGE_SPANISH
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_SPANISH = "es";
    
    /**
     * Spanish language code
     * @memberof ca.sys
     * @name LANGUAGE_DUTCH
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_DUTCH = "du";
    
    /**
     * Russian language code
     * @memberof ca.sys
     * @name LANGUAGE_RUSSIAN
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_RUSSIAN = "ru";
    
    /**
     * Korean language code
     * @memberof ca.sys
     * @name LANGUAGE_KOREAN
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_KOREAN = "ko";
    
    /**
     * Japanese language code
     * @memberof ca.sys
     * @name LANGUAGE_JAPANESE
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_JAPANESE = "ja";
    
    /**
     * Hungarian language code
     * @memberof ca.sys
     * @name LANGUAGE_HUNGARIAN
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_HUNGARIAN = "hu";
    
    /**
     * Portuguese language code
     * @memberof ca.sys
     * @name LANGUAGE_PORTUGUESE
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_PORTUGUESE = "pt";
    
    /**
     * Arabic language code
     * @memberof ca.sys
     * @name LANGUAGE_ARABIC
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_ARABIC = "ar";
    
    /**
     * Norwegian language code
     * @memberof ca.sys
     * @name LANGUAGE_NORWEGIAN
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_NORWEGIAN = "no";
    
    /**
     * Polish language code
     * @memberof ca.sys
     * @name LANGUAGE_POLISH
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_POLISH = "pl";
    
    /**
     * Unknown language code
     * @memberof ca.sys
     * @name LANGUAGE_UNKNOWN
     * @constant
     * @type {Number}
     */
    sys.LANGUAGE_UNKNOWN = "unkonwn";
    
    /**
     * @memberof ca.sys
     * @name OS_IOS
     * @constant
     * @type {string}
     */
    sys.OS_IOS = "iOS";
    /**
     * @memberof ca.sys
     * @name OS_ANDROID
     * @constant
     * @type {string}
     */
    sys.OS_ANDROID = "Android";
    /**
     * @memberof ca.sys
     * @name OS_WINDOWS
     * @constant
     * @type {string}
     */
    sys.OS_WINDOWS = "Windows";
    /**
     * @memberof ca.sys
     * @name OS_MARMALADE
     * @constant
     * @type {string}
     */
    sys.OS_MARMALADE = "Marmalade";
    /**
     * @memberof ca.sys
     * @name OS_LINUX
     * @constant
     * @type {string}
     */
    sys.OS_LINUX = "Linux";
    /**
     * @memberof ca.sys
     * @name OS_BADA
     * @constant
     * @type {string}
     */
    sys.OS_BADA = "Bada";
    /**
     * @memberof ca.sys
     * @name OS_BLACKBERRY
     * @constant
     * @type {string}
     */
    sys.OS_BLACKBERRY = "Blackberry";
    /**
     * @memberof ca.sys
     * @name OS_OSX
     * @constant
     * @type {string}
     */
    sys.OS_OSX = "OS X";
    /**
     * @memberof ca.sys
     * @name OS_WP8
     * @constant
     * @type {string}
     */
    sys.OS_WP8 = "WP8";
    /**
     * @memberof ca.sys
     * @name OS_WINRT
     * @constant
     * @type {string}
     */
    sys.OS_WINRT = "WINRT";
    /**
     * @memberof ca.sys
     * @name OS_UNKNOWN
     * @constant
     * @type {string}
     */
    sys.OS_UNKNOWN = "Unknown";

    
    sys.BROWSER_TYPE_WECHAT = "wechat";
    sys.BROWSER_TYPE_ANDROID = "androidbrowser";
    sys.BROWSER_TYPE_IE = "ie";
    sys.BROWSER_TYPE_QQ = "qqbrowser";
    sys.BROWSER_TYPE_MOBILE_QQ = "mqqbrowser";
    sys.BROWSER_TYPE_UC = "ucbrowser";
    sys.BROWSER_TYPE_360 = "360browser";
    sys.BROWSER_TYPE_BAIDU_APP = "baiduboxapp";
    sys.BROWSER_TYPE_BAIDU = "baidubrowser";
    sys.BROWSER_TYPE_MAXTHON = "maxthon";
    sys.BROWSER_TYPE_OPERA = "opera";
    sys.BROWSER_TYPE_OUPENG = "oupeng";
    sys.BROWSER_TYPE_MIUI = "miuibrowser";
    sys.BROWSER_TYPE_FIREFOX = "firefox";
    sys.BROWSER_TYPE_SAFARI = "safari";
    sys.BROWSER_TYPE_CHROME = "chrome";
    sys.BROWSER_TYPE_LIEBAO = "liebao";
    sys.BROWSER_TYPE_QZONE = "qzone";
    sys.BROWSER_TYPE_SOUGOU = "sogou";
    sys.BROWSER_TYPE_UNKNOWN = "unknown";
    
    /**
     * Is native ? This is set to be true in jsb auto.
     * @memberof ca.sys
     * @name isNative
     * @type {Boolean}
     */
    sys.isNative = true;

    /**
     * Indicate whether system is mobile system
     * @memberof ca.sys
     * @name isMobile
     * @type {Boolean}
     */
    sys.isMobile = (sys.os === sys.OS_ANDROID ||
                    sys.os === sys.OS_IOS) ? true : false;
    
    /**
     * Indicate the current language of the running system
     * @memberof ca.sys
     * @name language
     * @type {String}
     */
   sys.language = (function(){
                   var language = ca.application.getCurrentLanguage();
                   switch(language){
                   case 0: return sys.LANGUAGE_ENGLISH;
                   case 1: return sys.LANGUAGE_CHINESE;
                   case 2: return sys.LANGUAGE_FRENCH;
                   case 3: return sys.LANGUAGE_ITALIAN;
                   case 4: return sys.LANGUAGE_GERMAN;
                   case 5: return sys.LANGUAGE_SPANISH;
                   case 6: return sys.LANGUAGE_DUTCH;
                   case 7: return sys.LANGUAGE_RUSSIAN;
                   case 8: return sys.LANGUAGE_KOREAN;
                   case 9: return sys.LANGUAGE_JAPANESE;
                   case 10: return sys.LANGUAGE_HUNGARIAN;
                   case 11: return sys.LANGUAGE_PORTUGUESE;
                   case 12: return sys.LANGUAGE_ARABIC;
                   case 13: return sys.LANGUAGE_NORWEGIAN;
                   case 14: return sys.LANGUAGE_POLISH;
                   default : return sys.LANGUAGE_ENGLISH;
                   }
                   })();
    
    /**
     * Indicate the running os name
     * @memberof ca.sys
     * @name os
     * @type {String}
     */
    sys.os = __getOS();
    
    /**
     * Indicate the running browser type
     * @memberof ca.sys
     * @name browserType
     * @type {String}
     */
    sys.browserType = null; //null in jsb
    
    /**
     * Indicate the running browser version
     * @memberof ca.sys
     * @name browserVersion
     * @type {Number}
     */
    sys.browserVersion = null; //null in jsb
    
    /**
     * Indicate the real pixel resolution of the whole game window
     * @memberof ca.sys
     * @name windowPixelResolution
     * @type {Number}
     */
//    sys.windowPixelResolution = ca.view.getFrameSize();
    sys.windowPixelResolution = ca.winSize;
    
    /**
     * Forces the garbage collection, only available in JSB
     * @memberof ca.sys
     * @name garbageCollect
     * @function
     */
    sys.garbageCollect = function() {
        __jsc__.garbageCollect();
    };
    
    /**
     * Dumps rooted objects, only available in JSB
     * @memberof ca.sys
     * @name dumpRoot
     * @function
     */
    sys.dumpRoot = function() {
        __jsc__.dumpRoot();
    };
    
    /**
     * Restart the JS VM, only available in JSB
     * @memberof ca.sys
     * @name restartVM
     * @function
     */
    sys.restartVM = function() {
        __restartVM();
    };
    
    /**
     * Clean a script in the JS VM, only available in JSB
     * @memberof ca.sys
     * @name cleanScript
     * @param {String} jsfile
     * @function
     */
    sys.cleanScript = function(jsFile) {
        __cleanScript(jsFile);
    };
    
    /**
     * Check whether an object is valid,
     * In web engine, it will return true if the object exist
     * In native engine, it will return true if the JS object and the correspond native object are both valid
     * @memberof ca.sys
     * @name isObjectValid
     * @param {Object} obj
     * @return {boolean} Validity of the object
     * @function
     */
    sys.isObjectValid = function(obj) {
        return __isObjectValid(obj);
    };
    
    /**
     * Dump system informations
     * @memberof ca.sys
     * @name dump
     * @function
     */
    sys.dump = function () {
        var self = this;
        var str = "";
        str += "isMobile : " + self.isMobile + "\r\n";
        str += "language : " + self.language + "\r\n";
        str += "browserType : " + self.browserType + "\r\n";
        str += "capabilities : " + JSON.stringify(self.capabilities) + "\r\n";
        str += "os : " + self.os + "\r\n";
        str += "platform : " + self.platform + "\r\n";
        ca.log(str);
    }
    
    /**
     * Open a url in browser
     * @memberof ca.sys
     * @name openURL
     * @param {String} url
     */
    sys.openURL = function(url){
        ca.CADevice.openURL(url);
    }
    
    // JS to Native bridges
    if(window.JavascriptJavaBridge && ca.sys.os == ca.sys.OS_ANDROID){
        jsb.reflection = new JavascriptJavaBridge();
        ca.sys.capabilities["keyboard"] = true;
    }
    else if(window.JavaScriptObjCBridge && (ca.sys.os == ca.sys.OS_IOS || ca.sys.os == ca.sys.OS_OSX)){
        jsb.reflection = new JavaScriptObjCBridge();
    }
};

_initSys();

log = function (){};
ca.log = function (str){log(str);};

ca.logArray = function (array)
{
    /********************************/
    _logArray_ = function (array, _key_)
    {
        for (var key in array)
        {
            var k = _key_ + "." + key;
            if (array[key] instanceof Object)
            {
                _logArray_(array[key], k);
            }
            else {
                ca.log(k + " :        " + array[key]);
            }
        }
    }
    /********************************/

    ca.log("<--------logArray-------->");
    ca.log("[key] :     [value]");
    for (var key in array)
    {
        if (array[key] instanceof Object)
        {
            _logArray_(array[key], key);
        }
        else {
            ca.log(key + " :     " + array[key]);
        }
    }
    ca.log("<________logArray-End________>\n");
}


ca._engineLoaded = false;

ca.initEngine = function (config, cb) {
    require("script/jsb.js");
    ca._renderType = ca.app.RENDER_TYPE_OPENGL;
    ca._initDebugSetting(config[ca.app.CONFIG_KEY.debugMode]);
    ca._engineLoaded = true;
    ca.log(ca.ENGINE_VERSION);
    if (cb) cb();
};

jsb.urlRegExp = new RegExp(
                           "^" +
                           // protocol identifier
                           "(?:(?:https?|ftp)://)" +
                           // user:pass authentication
                           "(?:\\S+(?::\\S*)?@)?" +
                           "(?:" +
                           // IP address dotted notation octets
                           // excludes loopback network 0.0.0.0
                           // excludes reserved space >= 224.0.0.0
                           // excludes network & broacast addresses
                           // (first & last IP address of each class)
                           "(?:[1-9]\\d?|1\\d\\d|2[01]\\d|22[0-3])" +
                           "(?:\\.(?:1?\\d{1,2}|2[0-4]\\d|25[0-5])){2}" +
                           "(?:\\.(?:[1-9]\\d?|1\\d\\d|2[0-4]\\d|25[0-4]))" +
                           "|" +
                           // host name
                           "(?:(?:[a-z\\u00a1-\\uffff0-9]-*)*[a-z\\u00a1-\\uffff0-9]+)" +
                           // domain name
                           "(?:\\.(?:[a-z\\u00a1-\\uffff0-9]-*)*[a-z\\u00a1-\\uffff0-9]+)*" +
                           // TLD identifier
                           "(?:\\.(?:[a-z\\u00a1-\\uffff]{2,}))" +
                           "|" +
                           "(?:localhost)" +
                           ")" +
                           // port number
                           "(?::\\d{2,5})?" +
                           // resource path
                           "(?:/\\S*)?" +
                           "$", "i"
                           );
