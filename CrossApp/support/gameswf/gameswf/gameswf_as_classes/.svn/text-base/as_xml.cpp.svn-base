// gameswf_xml.h      -- Rob Savoye <rob@welcomehome.org> 2005

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.


#include <sys/types.h>
#include <sys/stat.h>

#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_as_classes/as_string.h"
#include "base/tu_config.h"

#ifdef HAVE_LIBXML

#include <unistd.h>
#include "gameswf/gameswf_xml.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>

namespace gameswf
{
  
  //#define DEBUG_MEMORY_ALLOCATION 1
  
array<as_object *> _xmlobjs;    // FIXME: hack alert

XMLAttr::XMLAttr()
{
#ifdef DEBUG_MEMORY_ALLOCATION
  log_msg("\t\tCreating XMLAttr data at %p \n", this);
#endif
  _name = 0;
  _value = 0;
}

XMLAttr::~XMLAttr()
{
#ifdef DEBUG_MEMORY_ALLOCATION
  log_msg("\t\tDeleting XMLAttr data %s at %p \n", this->_name, this);
#endif
  //log_msg("%s: %p \n", __FUNCTION__, this);
  if (_name) {
    delete _name;
  }
  if (_value) {
    delete _value;
  }  
}
  
XMLNode::XMLNode()
{
  //log_msg("%s: %p \n", __FUNCTION__, this);
#ifdef DEBUG_MEMORY_ALLOCATION
  log_msg("\tCreating XMLNode data at %p \n", this);
#endif
  _name = 0;
  _value = 0;
}

XMLNode::~XMLNode()
{
  int i;
  //log_msg("%s: %p \n", __FUNCTION__, this);
#ifdef DEBUG_MEMORY_ALLOCATION
  log_msg("\tDeleting XMLNode data %s at %p\n", this->_name, this);
#endif
  
  for (i=0; i<_children.size(); i++) {
//     if (_children[i]->_name) {
//       delete _children[i]->_name;
//     }
//     if (_children[i]->_value) {
//       delete _children[i]->_value;
//     }
    delete _children[i];
  }

  for (i=0; i<_attributes.size(); i++) {
    //     if (_attributes[i]->_name) {
//       delete _attributes[i]->_name;
//     }
//     if (_attributes[i]->_value) {
//       delete _attributes[i]->_value;
//     }
    delete _attributes[i];
  }

  _children.clear();
  _attributes.clear();

  if (_name) {
    delete _name;
  }
  if (_value) {
    delete _value;
  }
  //  _value.set_undefined();
}

XML::XML()
{
#ifdef DEBUG_MEMORY_ALLOCATION
  log_msg("Creating XML data at %p \n", this);
#endif
  //log_msg("%s: %p \n", __FUNCTION__, this);
  _loaded = false;
  _nodename = 0;
}


// Parse the ASCII XML string into memory
XML::XML(tu_string xml_in)
{
#ifdef DEBUG_MEMORY_ALLOCATION
  log_msg("Creating XML data at %p \n", this);
#endif
  //log_msg("%s: %p \n", __FUNCTION__, this);
  //memset(&_nodes, 0, sizeof(XMLNode));
  parseXML(xml_in);
}

XML::XML(struct node *childNode)
{
#ifdef DEBUG_MEMORY_ALLOCATION
  log_msg("\tCreating XML data at %p \n", this);
#endif
  //log_msg("%s: %p \n", __FUNCTION__, this);
}


XML::~XML()
{
#ifdef DEBUG_MEMORY_ALLOCATION
  if (this->_nodes) {
    log_msg("\tDeleting XML top level node %s at %p \n", this->_nodes->_name, this);
  } else {
    log_msg("\tDeleting XML top level node at %p \n", this);
  }
#endif
  
  //log_msg("%s: %p \n", __FUNCTION__, this);
  delete _nodes;
}

// Dispatch event handler(s), if any.
bool
XML::on_event(event_id id)
{
  // Keep m_as_environment alive during any method calls!
  //  gc_ptr<as_object_interface>	this_ptr(this);
  
#if 0
  // First, check for built-in event handler.
  as_value	method;
  if (get_event_handler(event_id(id), &method))
    {/
      // Dispatch.
      call_method0(method, &m_as_environment, this);
          return true;
    }
  
  // Check for member function.
  // In ActionScript 2.0, event method names are CASE SENSITIVE.
  // In ActionScript 1.0, event method names are CASE INSENSITIVE.
  const tu_string&	method_name = id.get_function_name();
  if (method_name.length() > 0)hostByNameGet
    {
      as_value	method;
      if (get_member(method_name, &method))
        {
          call_method0(method, &m_as_environment, this);
          return true;
        }
    }
#endif
  return false;
}

void
XML::on_event_load()
{
  // Do the events that (appear to) happen as the movie
  // loads.  frame1 tags and actions are executed (even
  // before advance() is called).  Then the onLoad event
  // is triggered.
  {
    on_event(event_id::LOAD);
  }
}

XMLNode*
XML::extractNode(xmlNodePtr node, bool mem)
{
  xmlAttrPtr attr;
  xmlNodePtr childnode;
  xmlChar *ptr = NULL;
  XMLNode *element, *child;
  int len;

  element = new XMLNode;
            
  //log_msg("Created new element for %s at %p\n", node->name, element);
  memset(element, 0, sizeof (XMLNode));

  //log_msg("%s: extracting node %s\n", __FUNCTION__, node->name);

  // See if we have any Attributes (properties)
  attr = node->properties;
  while (attr != NULL) {
    //log_msg("extractNode %s has property %s, value is %s\n",
    //          node->name, attr->name, attr->children->content);
    XMLAttr *attrib = new XMLAttr;
    len = memadjust(strlen(reinterpret_cast<const char *>(attr->name))+1);
    attrib->_name = (char *)new char[len];
    memset(attrib->_name, 0, len);
    strcpy(attrib->_name, reinterpret_cast<const char *>(attr->name));
    len = memadjust(strlen(reinterpret_cast<const char *>(attr->children->content))+1);
    attrib->_value = (char *)new char[len];
    memset(attrib->_value, 0, len);
    strcpy(attrib->_value, reinterpret_cast<const char *>(attr->children->content));
    //log_msg("\tPushing attribute %s for element %s has value %s\n",
    //        attr->name, node->name, attr->children->content);
    element->_attributes.push_back(attrib);
    attr = attr->next;
  }

  len = memadjust(strlen(reinterpret_cast<const char *>(node->name))+1);
  element->_name = (char *)new char[len];
  memset(element->_name, 0, len);
  strcpy(element->_name, reinterpret_cast<const char *>(node->name));
  //element->_name = reinterpret_cast<const char *>(node->name);
  if (node->children) {
    //ptr = node->children->content;
    ptr = xmlNodeGetContent(node->children);
    if (ptr != NULL) {
      if ((strchr((const char *)ptr, '\n') == 0) && (ptr[0] != 0))
      {
        if (node->children->content == NULL) {
          //log_msg("Node %s has no contents\n", node->name);
        } else {
          //log_msg("extractChildNode from text for %s has contents %s\n", node->name, ptr);
          len = memadjust(strlen(reinterpret_cast<const char *>(ptr))+1);
          element->_value = (char *)new char[len];
          memset(element->_value, 0, len);
          strcpy(element->_value, reinterpret_cast<const char *>(ptr));
          //element->_value = reinterpret_cast<const char *>(ptr);
        }
      }
      xmlFree(ptr);
    }
  }

  // See if we have any data (content)
  childnode = node->children;

  while (childnode != NULL) {
    if (childnode->type == XML_ELEMENT_NODE) {
      //log_msg("\t\t extracting node %s\n", childnode->name);
      child = extractNode(childnode, mem);
      //if (child->_value.get_type() != as_value::UNDEFINED) {
      if (child->_value != 0) {
        //log_msg("\tPushing childNode %s, value %s on element %p\n", child->_name.c_str(), child->_value.to_string(), element);
      } else {
        //log_msg("\tPushing childNode %s on element %p\n", child->_name.c_str(), element);
      }
      element->_children.push_back(child);
    }
    childnode = childnode->next;
  }

  return element;
}

// Read in an XML document from the specified source
bool
XML::parseDoc(xmlDocPtr document, bool mem)
{
  XMLNode *top;
  xmlNodePtr cur;

  if (document == 0) {
    log_error("Can't load XML file!\n");
    return false;
  }

  cur = xmlDocGetRootElement(document);
  
  if (cur != NULL) {
    top = extractNode(cur, mem);
    //_nodes->_name = reinterpret_cast<const char *>(cur->name);
    _nodes = top;
    //_node_data.push_back(top);
    //cur = cur->next;
  }  

  _loaded = true;
  return true;
}

// This reads in an XML file from disk and parses into into a memory resident
// tree which can be walked through later.
bool
XML::parseXML(tu_string xml_in)
{
  bool ret = true;
  //log_msg("Parse XML from memory: %s\n", xml_in.c_str());

  if (xml_in.size() == 0) {
    log_error("XML data is empty!\n");
    return false;
  }

#ifndef USE_DMALLOC
  //dump_memory_stats(__FUNCTION__, __LINE__, "before xmlParseMemory");
#endif

#ifdef USE_XMLREADER
  XMLNode *node = 0;
  xmlTextReaderPtr reader;

  reader = xmlReaderForMemory(xml_in.c_str(), xml_in.size(), NULL, NULL, 0);
  if (reader != NULL) {
    ret = true;
    while (ret) {
      ret = xmlTextReaderRead(reader);
      node = processNode(reader, node);
    }
    xmlFreeTextReader(reader);
    if (ret != false) {
      log_error("%s : couldn't parse\n", xml_in.c_str());
      return false;
    }
  } else {
    log_error("Unable to open %s\n", xml_in.c_str());
      return false;
  }
  xmlCleanupParser();
  return true;
#else
#ifdef USE_DOM
  xmlInitParser();
  
  _doc = xmlParseMemory(xml_in.c_str(), xml_in.size());
  if (_doc == 0) {
    log_error("Can't parse XML data!\n");
    return false;
  }
  ret = parseDoc(_doc, true);
  xmlCleanupParser();
  xmlFreeDoc(_doc);
  xmlMemoryDump();
#endif
#ifndef USE_DMALLOC
  //dump_memory_stats(__FUNCTION__, __LINE__, "after xmlParseMemory");
#endif

  return ret;
#endif
  
}

//     XML_READER_TYPE_NONE = 0
//     XML_READER_TYPE_ELEMENT = 1,
//     XML_READER_TYPE_ATTRIBUTE = 2,
//     XML_READER_TYPE_TEXT = 3,
//     XML_READER_TYPE_COMMENT = 8,
//     XML_READER_TYPE_SIGNIFICANT_WHITESPACE = 14,
//     XML_READER_TYPE_END_ELEMENT = 15,
//
// processNode:
// 2 1 IP 0
// processNode:
// 3 3 #text 0 192.168.2.50
// processNode:
// 2 15 IP 0
// processNode:
// 2 14 #text 0
const char *tabs[] = {
  "",
  "\t",
  "\t\t",
  "\t\t\t",
  "\t\t\t",
  "\t\t\t\t",
};

#ifdef USE_XMLREADER
// This is an xmlReader (SAX) based parser. For some reason it core dumps
// when compiled with GCC 3.x, but works just fine with GCC 4.x.
XMLNode*
XML::processNode(xmlTextReaderPtr reader, XMLNode *node)
{
  //log_msg("%s: node is %p\n", __FUNCTION__, node);
  static XMLNode *parent[10];
  xmlChar *name, *value;
  int   depth;
  XMLNode *element;
  //static int previous_depth = 0;
  xmlReaderTypes type;

  if (node == 0) {
    memset(parent, 0, sizeof(XMLNode *));
  }
  type = (xmlReaderTypes)xmlTextReaderNodeType(reader);
  depth = xmlTextReaderDepth(reader);
  value = xmlTextReaderValue(reader);
  name = xmlTextReaderName(reader);
  
  if (name == NULL)
    name = xmlStrdup(BAD_CAST "--");

#if 0
  printf("%d %d %s %d\n",
         depth,
         (int)type,
         name,
         xmlTextReaderIsEmptyElement(reader));  
#endif

  
  //child = node->_children[0];
  switch(xmlTextReaderNodeType(reader)) {
  case XML_READER_TYPE_NONE:
    break;
  case XML_READER_TYPE_SIGNIFICANT_WHITESPACE: // This is an empty text node
    //log_msg("\tWhitespace at depth %d\n", depth);
    break;
  case XML_READER_TYPE_END_ELEMENT:
     if (depth == 0) {          // This is the last node in the file
       element = node;
       break;
     }
     parent[depth]->_children.push_back(element);
//       log_msg("Pushing element %s on node %s\n", node->_name, parent[depth]->_name);
//       log_msg("End element at depth %d is %s for parent %s %p\n", depth, name,
//               parent[depth]->_name, parent[depth]);
     element = parent[depth];
    break;
  case XML_READER_TYPE_ELEMENT:
    element = new XMLNode;
//      log_msg("%sElement at depth %d is %s for node at %p\n", tabs[depth], depth, name, element);
    element->_name = (char *)new char[strlen(reinterpret_cast<const char *>(name))+1];
    memset(element->_name, 0, strlen(reinterpret_cast<const char *>(name))+1);
    strcpy(element->_name, reinterpret_cast<const char *>(name));
    if (node == 0) {
      _nodes = element;
      parent[0] = element;
    } else {
      parent[depth] = node;
      parent[depth+1] = node;
    }
    //  xmlTextReaderAttributeCount(reader);
    if (xmlTextReaderHasAttributes(reader)) {
      // log_msg("Has Attributes!\n");
      xmlTextReaderMoveToFirstAttribute(reader);
      processNode(reader, element);
      while(xmlTextReaderMoveToNextAttribute(reader)) {
        processNode(reader, element);
      }
    }
    break;
  case XML_READER_TYPE_TEXT:
    element = node;
//      log_msg("%sValue at depth %d is \"%s\" for node at %p\n", tabs[depth], depth, value, element);
    element->_value = (char *)new char[strlen(reinterpret_cast<const char *>(value))+1];
    memset(element->_value, 0, strlen(reinterpret_cast<const char *>(value))+1);
    strcpy(element->_value, reinterpret_cast<const char *>(value));
    break;
  case XML_READER_TYPE_ATTRIBUTE:
	{
    element = node;
    XMLAttr *attrib = new XMLAttr;
    attrib->_name = (char *)new char[strlen(reinterpret_cast<const char *>(name))+1];
    memset(attrib->_name, 0, strlen(reinterpret_cast<const char *>(name))+1);
    strcpy(attrib->_name, reinterpret_cast<const char *>(name));
    attrib->_value = (char *)new char[strlen(reinterpret_cast<const char *>(value))+1];
    memset(attrib->_value, 0, strlen(reinterpret_cast<const char *>(value))+1);
    strcpy(attrib->_value, reinterpret_cast<const char *>(value));
//     log_msg("%sPushing attribute %s, value \"%s\" for node %s\n", tabs[depth], name, value, element->_name);
    element->_attributes.push_back(attrib);
    break;
	}
  default:   // FIXME: why does this break GCC 3.3.3 but not 3.4.3 ?
    log_error("Unsupported XML type %d\n!", type);
    break;
  };

  xmlFree(name);
  if (value != NULL) {
    xmlFree(value);
  }
  //previous_depth = depth;
  return element;
}
#endif

// This reads in an XML file from disk and parses into into a memory resident
// tree which can be walked through later.
bool
XML::load(const char *filespec)
{
  bool ret = true;
  log_msg("Load disk XML file: %s\n", filespec);
  
  //log_msg("%s: mem is %d\n", __FUNCTION__, mem);

#ifdef USE_XMLREADER
  XMLNode *node = 0;
  xmlTextReaderPtr reader;  
  
  reader = xmlNewTextReaderFilename(filespec);
  if (reader != NULL) {
    ret = true;
    while (ret) {
      ret = xmlTextReaderRead(reader);
      node = processNode(reader, node);
    }
    xmlFreeTextReader(reader);
    if (ret != false) {
      log_error("%s : couldn't parse\n", filespec);
      return false;
    }
  } else {
    log_error("Unable to open %s\n", filespec);
      return false;
  }
  xmlCleanupParser();
  return true;
#else
#ifdef USE_DOM
  xmlInitParser();
  _doc = xmlParseFile(filespec);
  if (_doc == 0) {
    log_error("Can't load XML file: %s!\n", filespec);
    return false;
  }
  ret = parseDoc(_doc, false);
  xmlCleanupParser();
  xmlFreeDoc(_doc);
  xmlMemoryDump();
  return true;
#else
#error "You have to enable either a DOM or an xmlReader XML parser"
#endif
#endif
}

bool
XML::onLoad()
{
  log_msg("%s: FIXME: onLoad Default event handler\n", __FUNCTION__);

  return(_loaded);
}

XMLNode *
XML::operator [] (int x) {
  log_msg("%s:\n", __FUNCTION__);

  return _nodes->_children[x];
}

void
XML::cleanupStackFrames(XMLNode *xml)
{
}

as_object *
XML::setupFrame(as_object *obj, XMLNode *xml, bool mem)
{
  int           child, i;
  const char    *nodename;
  //const char    *nodevalue;
  //AS_value      nodevalue;
  int           length;
  as_value      inum;
  XMLNode       *childnode;
  xmlnode_as_object *xmlchildnode_obj;
  xmlattr_as_object* attr_obj;

  //log_msg("\t%s: processing node %s for object %p, mem is %d\n", __FUNCTION__, xml->_name, obj, mem);
  
  // Get the data for this node
  nodename   = xml->_name;
  //nodename   = xml->_name.c_str();
  //nodevalue  = xml->_value;
  length     = xml->length();

  // Set these members in the top level object passed in. This are used
  // primarily by the disk based XML parser, where at least in all my current
  // test cases this is referenced with firstChild first, then nodeName and
  // childNodes.
  obj->set_member("nodeName",           nodename);
  obj->set_member("length",             length);
  if (xml->_value != 0) {
    obj->set_member("nodeValue",        xml->_value);
    //log_msg("\tnodevalue for %s is: %s\n", nodename, xml->_value);
  } else {
    obj->set_member("nodeValue", as_value::UNDEFINED);
  }

//   if (nodevalue.get_type() != as_value::UNDEFINED) {
//     obj->set_member("nodeValue",        nodevalue.to_string());
//     log_msg("\tnodevalue for %s is: %s\n", nodename, nodevalue.to_string());
//   } else {
//     // If there is no value, we want to define it as an empty
//     // string.
//     obj->set_member("nodeValue", "");
//   }

  
  // Process the attributes, if any
  if (xml->_attributes.size() == 0) {
    //log_msg("\t\tNo attributes for node %s, created empty object at %p\n", nodename, attr_obj);
//     log_msg("\t\tNo attributes for node %s\n", nodename);
  } else {
    attr_obj = new xmlattr_as_object;
    for (i=0; i<xml->_attributes.size(); i++) {
      attr_obj->set_member(xml->_attributes[i]->_name, xml->_attributes[i]->_value);
//        log_msg("\t\tAdding attribute as member %s, value is %s to node %s (%p)\n",
//                xml->_attributes[i]->_name,
//                xml->_attributes[i]->_value, nodename, obj);
    }
    obj->set_member("attributes", attr_obj);
  }

  //xml->_attributes.resize(0);
  //obj->set_member("attributes", attr_obj);

  // Process the children, if there are any
  if (length) {
    //log_msg("\tProcessing %d children nodes for %s\n", length, nodename);
    inum = 0;
    for (child=0; child<length; child++) {
      // Create a new AS object for this node's children
      xmlchildnode_obj = new xmlnode_as_object;
      // When parsing XML from memory, the test movies I have expect the firstChild
      // to be the first element of the array instead.
      if (mem) {
        childnode = xml;
        //obj->set_member(inum.to_string(), obj);
        //inum += 1;
        //childnode = xml->_children[child];
      } else {
        childnode = xml->_children[child];
      }
      setupFrame(xmlchildnode_obj, childnode, false); // setup child node
      obj->set_member(inum.to_string(), xmlchildnode_obj);
      inum += 1;
    }
  } else {
    //log_msg("\tNode %s has no children\n", nodename);
  }  

  return obj;
}
  
//
// Callbacks. These are the wrappers for the C++ functions so they'll work as
// callbacks from within gameswf.
//
void
xml_load(const fn_call& fn)
{
  as_value	method;
  as_value	val;
  bool          ret;
  struct stat   stats;


  //log_msg("%s:\n", __FUNCTION__);
  
  xml_as_object *xml_obj = (xml_as_object*)fn.this_ptr;
  
  assert(xml_obj);

  const tu_string filespec = fn.env->bottom(fn.first_arg_bottom_index).to_string();

  // If the file doesn't exist, don't try to do anything.
  if (stat(filespec.c_str(), &stats) < 0) {
    fprintf(stderr, "ERROR: doesn't exist.%s\n", filespec.c_str());
		fn.result->set_bool(false);
    return;
  }
  
  // Set the argument to the function event handler based on whether the load
  // was successful or failed.
  ret = xml_obj->obj.load(filespec);
	fn.result->set_bool(ret);

  if (ret == false) {
    return;
  }
    
  //env->bottom(first_arg) = ret;
  array<with_stack_entry> with_stack;
  array<with_stack_entry> dummy_stack;
  //  struct node *first_node = ptr->obj.firstChildGet();
  
  //const char *name = ptr->obj.nodeNameGet();

  if (xml_obj->obj.hasChildNodes() == false) {
    log_error("%s: No child nodes!\n", __FUNCTION__);
  }  
  xml_obj->obj.setupFrame(xml_obj, xml_obj->obj.firstChild(), false);
  
#if 1
  if (fn.this_ptr->get_member("onLoad", &method)) {
    //    log_msg("FIXME: Found onLoad!\n");
    fn.env->set_variable("success", true, 0);
    fn.env->bottom(fn.first_arg_bottom_index) = true;
    as_c_function_ptr	func = method.to_c_function();
    if (func)
      {
        // It's a C function.  Call it.
        log_msg("Calling C function for onLoad\n");
        (*func)(fn_call(&val, xml_obj, fn.env, fn.nargs, fn.first_arg_bottom_index)); // was this_ptr instead of node
      }
    else if (as_as_function* as_func = method.to_as_function())
      {
        // It's an ActionScript function.  Call it.
        log_msg("Calling ActionScript function for onLoad\n");
        (*as_func)(fn_call(&val, xml_obj, fn.env, fn.nargs, fn.first_arg_bottom_index)); // was this_ptr instead of node
      } else {
        log_error("error in call_method(): method is not a function\n");
      }
  } else {
    log_msg("Couldn't find onLoad event handler, setting up callback\n");
    // ptr->set_event_handler(event_id::XML_LOAD, (as_c_function_ptr)&xml_onload);
  }
#else
  xml_obj->set_event_handler(event_id::XML_LOAD, &xml_onload);

#endif

  fn.result->set_bool(true);
}

// This executes the event handler for XML::XML_LOAD if it's been defined,
// and the XML file has loaded sucessfully.
void
xml_onload(const fn_call& fn)
{
  //log_msg("%s:\n", __FUNCTION__);
    
  as_value	method;
  as_value      val;
  static bool first = true;     // This event handler should only be executed once.
  array<with_stack_entry>	empty_with_stack;
  xml_as_object*	ptr = (xml_as_object*) (as_object*) fn.this_ptr;
  assert(ptr);
  
  if ((ptr->obj.loaded()) && (first)) {
    // env->set_variable("success", true, 0);
    //as_value bo(true);
    //env->push_val(bo);

    first = false;
    log_msg("The XML file has been loaded successfully!\n");
    // ptr->on_event(event_id::XML_LOAD);
    //env->set_variable("success", true, 0);
    //env->bottom(0) = true;
    
    if (fn.this_ptr->get_member("onLoad", &method)) {
      // log_msg("FIXME: Found onLoad!\n");
      as_c_function_ptr	func = method.to_c_function();
      if (func)
        {
          // It's a C function.  Call it.
          log_msg("Calling C function for onLoad\n");
          (*func)(fn_call(&val, fn.this_ptr, fn.env, 0, 0));
        }
      else if (as_as_function* as_func = method.to_as_function())
        {
          // It's an ActionScript function.  Call it.
          log_msg("Calling ActionScript function for onLoad\n");
        (*as_func)(fn_call(&val, fn.this_ptr, fn.env, 0, 0));
        }
      else
        {
          log_error("error in call_method(): method is not a function\n");
        }    
    } else {
      log_msg("FIXME: Couldn't find onLoad!\n");
    }
  }
      
	*fn.result = val;
}

// This is the default event handler, and is usually redefined in the SWF script
void
xml_ondata(const fn_call& fn)
{
  log_msg("%s:\n", __FUNCTION__);
    
  as_value	method;
  as_value	val;
  static bool first = true;     // FIXME: ugly hack!
  
  xml_as_object*	ptr = (xml_as_object*)fn.this_ptr;
  assert(ptr);
  
  if ((ptr->obj.loaded()) && (first)) {
    if (fn.this_ptr->get_member("onData", &method)) {
      log_msg("FIXME: Found onData!\n");
      as_c_function_ptr	func = method.to_c_function();
      fn.env->set_variable("success", true, 0);
      if (func)
        {
          // It's a C function.  Call it.
          log_msg("Calling C function for onData\n");
          (*func)(fn_call(&val, fn.this_ptr, fn.env, 0, 0));
      }
      else if (as_as_function* as_func = method.to_as_function())
        {
          // It's an ActionScript function.  Call it.
          log_msg("Calling ActionScript function for onData\n");
          (*as_func)(fn_call(&val, fn.this_ptr, fn.env, 0, 0));
        }
      else
        {
          log_error("error in call_method(): method is not a function\n");
        }    
    } else {
      log_msg("FIXME: Couldn't find onData!\n");
    }
  }

  *fn.result = val;
}

void
xml_new(const fn_call& fn)
{
  as_value      inum;
  xml_as_object *xml_obj;
  //const char    *data;
  
  //log_msg("%s: nargs=%d\n", __FUNCTION__, nargs);
  
  if (fn.nargs > 0) {
    if (fn.env->top(0).get_type() == as_value::STRING) {
      xml_obj = new xml_as_object;
      //log_msg("\tCreated New XML object at %p\n", xml_obj);
      tu_string datain = fn.env->top(0).to_tu_string();
      xml_obj->obj.parseXML(datain);
      //log_msg("*** Start setting up the stack frames ***\n");
      xml_obj->obj.setupFrame(xml_obj, xml_obj->obj.firstChild(), true);
      //xml_obj->obj.clear();
      //delete xml_obj->obj.firstChild();
    } else {
      xml_as_object*	xml_obj = (xml_as_object*)fn.env->top(0).to_object();
      //log_msg("\tCloned the XML object at %p\n", xml_obj);
      //result->set(xml_obj);
      fn.result->set_as_object_interface(xml_obj);
      return;
    }
  } else {
    xml_obj = new xml_as_object;
    //log_msg("\tCreated New XML object at %p\n", xml_obj);
    xml_obj->set_member("load", &xml_load);
    xml_obj->set_member("loaded", &xml_loaded);
  }

  fn.result->set_as_object_interface(xml_obj);
}

//
// SWF Property of this class. These are "accessors" into the private data
// of the class.
//

// determines whether the document-loading process initiated by the XML.load()
// call has completed. If the process completes successfully, the method
// returns true; otherwise, it returns false.
void
xml_loaded(const fn_call& fn)
{
  as_value	method;
  as_value	val;

  log_msg("%s:\n", __FUNCTION__);
    
  xml_as_object*	ptr = (xml_as_object*) (as_object*) fn.this_ptr;
  assert(ptr);
  tu_string filespec = fn.env->bottom(fn.first_arg_bottom_index).to_string();
  fn.result->set_bool(ptr->obj.loaded());
}

int
memadjust(int x)
{
  return (x + (4 - x % 4));
}


} // end of gameswf namespace


// HAVE_LIBXML
#endif
