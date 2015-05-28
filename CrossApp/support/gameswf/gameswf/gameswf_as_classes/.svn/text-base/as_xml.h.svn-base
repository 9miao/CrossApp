#ifndef __XML_H__
#define __XML_H__

//#define DEBUG_MEMORY_ALLOCATION 1

#include "base/tu_config.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_log.h"

#ifdef HAVE_LIBXML

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>

namespace gameswf
{

class XMLAttr {
 public:
  XMLAttr();
  ~XMLAttr();
  
  char        *_name;
  char        *_value;

  XMLAttr *operator = (XMLAttr node) {
    log_msg("\t\tCopying XMLAttr object at %p\n", this);
    
    _name = new char[strlen(node._name)+2];
    memset(_name, 0, strlen(node._name)+2);
    strcpy(_name, node._name);

    _value = new char[strlen(node._value)+2];
    memset(_value, 0, strlen(node._value)+2);
    strcpy(_value, node._value);

    return this;
  }
};

struct xmlattr_as_object : public gameswf::as_object
{
  //XMLAttr obj;
  int   padding;
#ifdef DEBUG_MEMORY_ALLOCATION
  xmlattr_as_object() 
  {
    log_msg("\t\tCreating xmlattr_as_object at %p\n", this);
  };
  ~xmlattr_as_object() 
  {
    log_msg("\tDeleting xmlattr_as_object at %p \n", this);
  };
#endif
};
 
class XMLNode
{
public:
  XMLNode();
  ~XMLNode();

  int length() { return _children.size(); }
  tu_string nodeName() 
  {
    return _name;
  }

  char *nodeValue()
  {
    return _value;
  }
  
  //  nodeType 	XML.nodeType

  bool hasChildNodes()
  {
    if (_children.size() > 0) {
      return true;
    }
    return false;
  }
  
  XMLNode *firstChild()
  {
    return _children[0];
  }
  
  array<XMLNode *>childNodes()
  {
    return _children;
  }  

  XMLNode *operator [] (int x)
  {
    gameswf::log_msg("%s: get element %d\n", __FUNCTION__, x);
    
    return _children[x];
  }
  
  XMLNode *operator = (XMLNode &node)
  {
    _name = node._name;
    _value = node._value;
    _children = node._children;
    _attributes = node._attributes;
    return this;
  }

  XMLNode *operator = (XMLNode *node)
  {
    _name = node->_name;
    _value = node->_value;
    _children = node->_children;
    _attributes = node->_attributes;
    return this;
  }

  void appendChild(XMLNode *node)
  {
    node->_children.push_back(node);
  }

  void  change_stack_frame(int frame, gameswf::as_object *xml, gameswf::as_environment *env);

  
#if 0
appendChild()   	 XML.appendChild()
attributes 	XML.attributes
childNodes 	XML.childNodes
cloneNode() 	XML.cloneNode()
insertBefore() 	XML.insertBefore()
lastChild 	XML.lastChild
nextSibling 	XML.nextSibling
nodeType 	XML.nodeType
parentNode 	XML.parentNode
previousSibling 	XML.previousSibling
removeNode() 	XML.removeNode()
toString() 	XML.toString()
#endif
  char               *_name;
  char               *_value;
  array<XMLNode *>   _children;
  array<XMLAttr *>   _attributes;
};

struct xmlnode_as_object : public gameswf::as_object
{
  //XMLNode obj;
  int                _padding;

#ifdef DEBUG_MEMORY_ALLOCATION
  xmlnode_as_object() 
  {
    log_msg("\tCreating xmlnode_as_object at %p \n", this);
  };
  ~xmlnode_as_object() 
  {
    log_msg("\tDeleting xmlnode_as_object at %p \n", this);
  };
#endif
  virtual bool	get_member(const tu_stringi& name, as_value* val)
  {
    //printf("GET XMLNode MEMBER: %s at %p for object %p\n", name.c_str(), val, this);

    if ((name == "firstChild") || (name == "childNodes")) {
      //printf("Returning a self reference for %s for object at %p\n", name.c_str(), this);
			val->set_as_object_interface(this);
      return true;
    }
    
//    if (m_members.get(name, val) == false) {
    if (get_member(name, val) == false) {
      if (m_prototype != NULL) {
        return m_prototype->get_member(name, val);
      }
      return false;
    }
    return true;
  }
};

class XML {
 public:
  XML();
  XML(tu_string xml_in);
  XML(struct node * childNode);
  virtual ~XML();

  // Methods
  bool parseDoc(xmlDocPtr document, bool mem); // This is the base method used by both parseXML() and load().
  bool parseXML(tu_string xml_in); // Parses an XML document into the specified XML object tree.
  bool load(const char *filespec);  // Loads a document (specified by
                                    // the XML object) from a URL.

  bool onLoad();                   // An event handler that returns a
                                   // Boolean value indicating whether
                                   // the XML object was successfully
                                   // loaded with XML.load() or
                                   // XML.sendAndLoad().

  // Appends a node to the end of the specified object's child list.
  void appendChild(XMLNode *node)
  {
    _nodes->_children.push_back(node);
  }
  
  virtual bool on_event(gameswf::event_id id);
  virtual void	on_event_load();
  bool loaded()    { return _loaded; }

  XMLNode *firstChild()
  {
    return _nodes;
    //return _node_data[0];
  }
  
  void clear()
  {
    delete _nodes;
  }
  
  array<XMLNode *> childNodes()
  {
    return _nodes->_children;
  }
  
  //  Returns true if the specified node has child nodes; otherwise, returns false.
  bool hasChildNodes()
  {
		return _nodes->_children.size() > 0 ? true : false;
  }

  XMLNode *extractNode(xmlNodePtr node, bool mem);
  XMLNode *processNode(xmlTextReaderPtr reader, XMLNode *node);

  void  change_stack_frame(int frame, gameswf::as_object *xml, gameswf::as_environment *env);
  void  setupStackFrames(gameswf::as_object *xml, gameswf::as_environment *env);
  void  cleanupStackFrames( XMLNode *data);
  as_object *setupFrame(gameswf::as_object *xml, XMLNode *data, bool src);
  
  const char *nodeNameGet() 
    {
      return _nodename;
    }
  
  int length() { return _nodes->length(); }
  
#if 0
  void addRequestHeader();          // Adds or changes HTTP headers for POST
                                    // operations. 
  void appendChild();               // Appends a node to the end of
                                    // the specified object's child
                                    // list.
  void cloneNode();                 // Clones the specified node and,
                                    // optionally, recursively clones
                                    // all children. 
  void createElement(); // Creates a new XML element.
  void createTextNode(); //Creates a new XML text node.
  void getBytesLoaded(); // Returns the number of bytes loaded for the specified XML document.
  void getBytesTotal(); // Returns the size of the XML document, in bytes.
  void insertBefore();// Inserts a node in front of an existing node in the specified node's child list.
  void removeNode();// Removes the specified node from its parent.
  void send();             // Sends the specified XML object to a URL.
  void sendAndLoad(); // Sends the specified XML object to a URL and loads the server response into another XML object.void 
  gameswf::tu_string toString();                // Converts the specified node and 
                                                // any children to XML text. 
#endif
  virtual void	on_xml_event(gameswf::event_id id) { on_event(id); }
  
  // Special event handler; 
  void	on_event_close() { on_event(gameswf::event_id::SOCK_CLOSE); }
  
  XMLNode *operator [] (int x);
#if 0
  XMLNode *operator = (XMLNode &node)
  {
    gameswf::log_msg("%s: copy element %s\n", __FUNCTION__, node._name.c_str());
    _nodes = node;
  }

#endif
  XML *operator = (XMLNode *node)
  {
    _nodes = node;    
    return this;
  }

  private:
  //bool _on_event_loaded;
    xmlDocPtr _doc;
    xmlNodePtr _firstChild;
    
    // Properties
    bool _loaded;                           // Read-only; checks if
                                            // the specified XML object has loaded.
    const char  *_nodename;                  // The node name of an XML object.
    XMLNode     *_nodes;
    //    array<XMLNode *>  _node_data;
  //    hash<gameswf::event_id, gameswf::as_value>	_event_handlers;
#if 0
    array<struct node *> _childNodes; // Read-only; returns an array containing
                                            // references to the child
                                            // nodes of the specified
                                            // node.
  contentType; //	Indicates the MIME type transmitted to the server.
  docTypeDecl;	// Sets and returns information about an XML document's DOCTYPE declaration.
  firstChild;	// Read-only; references the first child in the list for the specified node.
  bool ignoreWhite;	// When set to true, text nodes that contain only white space are discarded during the parsing process.
  lastChild;	// References the last child in the list for the specified node.
  nextSibling;	// Read-only; references the next sibling in the parent node's child list.
  tu_string nodeType;	// The type of the specified node (XML element or text node).
  nodeValue;	// The text of the specified node if the node is a text node.
  parentNode;	// Read-only; references the parent node of the specified node.
  previousSibling; //	Read-only; references the previous sibling in the parent node's child list.
  status; //	A numeric status code indicating the success or failure of an XML document parsing operation.
  xmlDecl; //	Specifies information about a document's XML declaration.

  // Collections data
  attributes; // Returns an associative array containing all of the attributes of the specified node.
  // Event Handlers
  onData; // An event handler that is invoked when XML text has been
          // completely downloaded from the server, or when an error
          // occurs downloading XML text from a server. 
#endif  
};

struct xml_as_object : public gameswf::as_object
{
  XML obj;
#ifdef DEBUG_MEMORY_ALLOCATION
  xml_as_object() 
  {
    log_msg("\tCreating xml_as_object at %p\n", this);
  };
  ~xml_as_object() 
  {
    log_msg("\tDeleting xml_as_object at %p\n", this);
  };
#endif
  virtual bool	get_member(const tu_stringi& name, as_value* val)
  {
    //printf("GET XML MEMBER: %s at %p for object %p\n", name.c_str(), val, this);

    if ((name == "firstChild") || (name == "childNodes")) {
      //printf("Returning a self reference for %s for object at %p\n", name.c_str(), this);
			val->set_as_object_interface(this);
      return true;
    }
    
//    if (m_members.get(name, val) == false) {
    if (get_member(name, val) == false) {
      if (m_prototype != NULL) {
        return m_prototype->get_member(name, val);
      }
      return false;
    }
    return true;
  }
};

exported_module virtual bool is_instance_of(const as_function& constructor) const;
void xml_load(const fn_call& fn);
void xml_set_current(const fn_call& fn);
void xml_new(const fn_call& fn);

// These are the event handlers called for this object
void xml_onload(const fn_call& fn);
void xml_ondata(const fn_call& fn);
void xml_loaded(const fn_call& fn);

int memadjust(int x);

}	// end namespace gameswf


#endif // HAVE_LIBXML

#endif	// __XML_H__

