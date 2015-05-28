// as_load_vars.cpp	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "gameswf/gameswf_as_classes/as_loadvars.h"
#include "gameswf/gameswf_root.h"
#include "base/container.h"

#if TU_ENABLE_NETWORK == 1

#include "net/http_helper.h"

namespace gameswf
{

	// Point(x:Number, y:Number)
	void	as_global_loadvars_ctor(const fn_call& fn)
	{
		gc_ptr<as_loadvars>	obj;
		obj = new as_loadvars(fn.get_player());
		fn.result->set_as_object(obj.get_ptr());
	}

	//addRequestHeader(header:Object, headerValue:String) : Void
	void	as_loadvars_addrequestheader(const fn_call& fn)
	{
		if( fn.nargs == 1 )
		{
			as_loadvars * loadvars = cast_to<as_loadvars>( fn.this_ptr );

			assert( loadvars );

			assert( 0 && "todo" );
		}
		else if( fn.nargs == 2 )
		{
			as_loadvars * loadvars = cast_to<as_loadvars>( fn.this_ptr );

			assert( loadvars );

			if( fn.arg(0).is_string() && fn.arg(1).is_string() )
				loadvars->add_header( fn.arg(0).to_tu_string(), fn.arg(1).to_tu_string() );
		}
	}

	//decode(queryString:String) : Void
	void	as_loadvars_decode(const fn_call& fn)
	{
		if( fn.nargs < 1 )
		{
			return;
		}

		as_loadvars * loadvars = cast_to<as_loadvars>( fn.this_ptr );

		assert( loadvars );

		loadvars->decode(fn.arg(0).to_tu_string());
	}

	//getBytesLoaded() : Number
	void	as_loadvars_getbytesloaded(const fn_call& fn)
	{
		assert( 0 && "todo" );
	}

	//getBytesTotal() : Number
	void	as_loadvars_getbytestotal(const fn_call& fn)
	{
		assert( 0 && "todo" );
	}

	//load(url:String) : Boolean
	void	as_loadvars_load(const fn_call& fn)
	{
		if (fn.nargs < 1 || fn.arg(0).is_null())
		{
			fn.result->set_bool(false);
			return;
		}

		as_loadvars* loadvars = cast_to<as_loadvars>(fn.this_ptr);

		assert(loadvars);

		fn.result->set_bool(loadvars->load( fn.arg(0).to_string()));
	}

	//send(url:String, target:String, [method:String]) : Boolean
	void	as_loadvars_send(const fn_call& fn)
	{
		assert( 0 && "todo" );
	}

	//sendAndLoad(url:String, target:Object, [method:String]) : Boolean
	void	as_loadvars_sendandload(const fn_call& fn)
	{
		if (fn.nargs < 2)
		{
			fn.result->set_bool(false);
			return;
		}

		const char * method = "POST";
		if( fn.nargs == 3 )
		{
			method = fn.arg(2).to_string();

			assert( strcmp(method, "POST") == 0 || strcmp(method,"GET") == 0 );
		}

		as_loadvars* loadvars = cast_to<as_loadvars>(fn.this_ptr);

		assert(loadvars);

		fn.result->set_bool(loadvars->send_and_load( fn.arg(0).to_string(), fn.arg(1).to_object(), method));
	}
   
	//toString() : String
	void	as_loadvars_tostring(const fn_call& fn)
	{
		as_loadvars* loadvars = cast_to<as_loadvars>(fn.this_ptr);

		assert(loadvars);

		fn.result->set_tu_string(loadvars->override_to_string());
	}

	as_loadvars::as_loadvars(player* player) :
		as_object(player)
	{
		builtin_member( "addRequestHeader" , as_loadvars_addrequestheader );
		builtin_member( "decode" , as_loadvars_decode );
		builtin_member( "getBytesLoaded" , as_loadvars_getbytesloaded );
		builtin_member( "getBytesTotal" , as_loadvars_getbytestotal );
		builtin_member( "load" , as_loadvars_load );
		builtin_member( "send" , as_loadvars_send );
		builtin_member( "sendAndLoad" , as_loadvars_sendandload );
		builtin_member( "toString" , as_loadvars_tostring );

		// default values
		m_headers.set("Content-Type", "application/x-www-form-urlencoded");
		m_headers.set("Cache-Control", "no-cache");
		m_headers.set("User-Agent", "gameswf");
	}

	bool	as_loadvars::send_and_load(const char * c_url, as_object * target, const tu_string & method)
	{
		tu_string host, uri;
		request_data request;

		if( parse_url(c_url, host, uri) )
		{
			request.m_iface = new net_interface_tcp();
			request.m_ns = request.m_iface->connect(host, 80);
		}

		bool is_connected = request.m_ns ? true : false;

		if( !is_connected )
		{
			as_value function;
			if (target && target->get_member("onLoad", &function))
			{
				as_environment env(get_player());
				env.push(false);
				call_method(function, &env, target, 1, env.get_top_index());
			}
			
			delete request.m_iface;
			
			return false;
		}

		request.m_target = cast_to<as_loadvars>(target);
		get_root()->add_listener(this);

		m_headers.set("Host", host);

		tu_string request_string = create_request( method, uri, true );

		printf( request_string.c_str() );
		request.m_ns->write_string(request_string, 1);
		request.m_state = PARSE_REQUEST;

		m_requests.push_back( request );

		return true;
	}

	bool	as_loadvars::load(const char * c_url)
	{
		tu_string host, uri;
		request_data request;

		if( parse_url(c_url, host, uri) )
		{
			request.m_iface = new net_interface_tcp();
			request.m_ns = request.m_iface->connect(host, 80);
		}

		bool is_connected = request.m_ns ? true : false;

		if( !is_connected )
		{
			as_value function;
			if (get_member("onLoad", &function))
			{
				as_environment env(get_player());
				env.push(false);
				call_method(function, &env, this, 1, env.get_top_index());
			}

			delete request.m_iface;

			return false;
		}

		request.m_target = this;
		get_root()->add_listener(this);

		m_headers.set("Host", host);

		tu_string request_string = create_request( "GET", uri, true );

		printf( request_string.c_str() );
		request.m_ns->write_string(request_string, 1);
		request.m_state = PARSE_REQUEST;

		m_requests.push_back( request );

		return true;
	}

	void	as_loadvars::add_header(const tu_string& name, const tu_string& value)
	{
		m_headers.set(name, value);
	}

	tu_string	as_loadvars::override_to_string()
	{
		tu_string information;
		//create information in the form of name=value (urlencoded)
		string_hash<tu_string>::iterator it = m_received_values.begin();
		for(bool first = true; it != m_received_values.end(); ++it)
		{
			tu_string name, value;

			name = it->first;
			value = it->second;

			url_encode( &name );
			url_encode( &value );
			information += string_printf("%s%s=%s", first? "":"&",name.c_str(), value.c_str() );
			first = false;
		}

		return information;
	}

	void	as_loadvars::advance(float delta_time)
	{
		tu_string str;

		for(int i=0;i<m_requests.size();++i)
		{
			request_data & request = m_requests[i];

			while(request.m_ns->is_readable())
			{
				int byte_read = request.m_ns->read_line(&str, 100000, 0);

				if( byte_read == -1 || request.m_state == PARSE_END )
				{
					//Handle end of connection
					if( request.m_target )
					{
						as_value function;
						if (request.m_target->get_member("onHttpStatus", &function))
						{
							as_environment env(get_player());
							env.push(request.m_http_status);
							call_method(function, &env, request.m_target, 0, env.get_top_index());
						}

						if (request.m_target->get_member("onLoad", &function))
						{
							as_environment env(get_player());
							env.push(request.m_state != PARSE_END);
							call_method(function, &env, request.m_target, 1, env.get_top_index());
						}

						if (request.m_target->get_member("onData", &function))
						{
							as_environment env(get_player());
							env.push(request.m_rawdata);
							call_method(function, &env, request.m_target, 1, env.get_top_index());
						}
					}

					get_root()->remove_listener(this);

					delete request.m_ns;
					delete request.m_iface;
					m_requests.remove(i);
					i--;
					return;
				}

				switch(request.m_state)
				{
					case PARSE_REQUEST:
						parse_request(str, request);
						break;

					case PARSE_HEADER:
						parse_header(str, request);
						break;

					case PARSE_CONTENT:
						parse_content(str, request);
						break;
				}

				str.clear();
			}
		}
	}

	void	as_loadvars::decode(const tu_string& query_string)
	{
		char *start, *end;
		start = (char*)query_string.c_str();
		end = start;

		while( start < query_string.c_str() + query_string.size() )
		{
			while( *end!='&' && *end!=0 ) ++end;

			if( end == start )
			{
				//empty pair
				++start;
				continue;
			}

			*end=0;

			const char *after_name = strstr(start, "=");

			if( after_name == NULL )
				return;

			tu_string name = tu_string(start, int(after_name - start));
			tu_string value = tu_string( after_name + 1 ); //Skip the "="

			url_decode( &name );
			url_decode( &value );

			m_received_values.set(name, value);

			start = end + 1;
			++end;
		}
	}

	bool	as_loadvars::set_member(const tu_stringi& name, const as_value& val)
	{
		// todo: check for callbacks

		if( name == "onData"
			|| name == "onHTTPStatus"
			||name == "onLoad"
			)
		{
			return as_object::set_member( name, val );
		}

		m_values.set( name.to_tu_string(), val.to_tu_string() );

		return true;
	}

	bool	as_loadvars::get_member(const tu_stringi& name, as_value* val)
	{
		string_hash<tu_string>::iterator it = m_values.find( name.to_tu_string() );
		if( it != m_values.end() )
		{
			val->set_string( it->second );
			return true;
		}

		string_hash<tu_string>::iterator it2 = m_received_values.find( name.to_tu_string() );
		if( it2 != m_received_values.end() )
		{
			val->set_string( it2->second );
			return true;
		}

		return as_object::get_member( name, val );
	}

	tu_string as_loadvars::create_request(const tu_string& method, const tu_string& uri, bool send_data)
	{
		tu_string information;
		//create information in the form of name=value (urlencoded)
		string_hash<tu_string>::iterator it = m_values.begin();
		for(bool first = true; it != m_values.end(); ++it)
		{
			tu_string name, value;

			name = it->first;
			value = it->second;

			url_encode( &name );
			url_encode( &value );
			information += string_printf("%s%s=%s", first? "":"&",name.c_str(), value.c_str() );
			first = false;
		}

		if( method == "POST")
		{
			tu_string request = string_printf( "POST %s HTTP/1.1\r\n", uri.c_str() );

			m_headers.set("Content-Length", string_printf( "%i", information.size()));

			request += create_header();
			request += "\r\n";
			request += information;

			return request;
		}
		else if(method == "GET")
		{
			tu_string request = string_printf( "GET %s?%s HTTP/1.1\r\n", uri.c_str(), information.c_str() );
			request += create_header();
			request += "\r\n";

			return request;
		}
		else
		{
			assert(0 && "unsupported");
		}

		return "";
	}

	tu_string 	as_loadvars::create_header()
	{
		tu_string header;

		// Process header
		for( string_hash<tu_string>::iterator it2 = m_headers.begin(); it2 != m_headers.end(); ++it2 )
		{
			header += string_printf( "%s: %s\r\n", it2->first.c_str(), it2->second.c_str() );
		}

		return header;
	}

	void	as_loadvars::parse_request(const tu_string& line, request_data& request)
	{
		assert(request.m_state == PARSE_REQUEST);
		
		// Method.
		const char* first_space = strchr(line.c_str(), ' ');
		if (first_space == NULL || first_space == line.c_str())
		{
			request.m_state = PARSE_END;
			return;
		}

		// URI.
		const char* second_space = strchr(first_space + 1, ' ');
		assert( second_space );
		request.m_http_status = atoi(first_space + 1);
		// todo use http status to trigger a onLoad(false)

		request.m_state = PARSE_HEADER;
	}

	void	as_loadvars::parse_header(const tu_string& line, request_data& request)
	{
		if( line == "\r\n" )
		{
			request.m_state = PARSE_CONTENT;
		}
	}

	void	as_loadvars::parse_content(const tu_string& line, request_data& request)
	{
		request.m_rawdata += line;

		request.m_target->decode( line );
	}

	bool as_loadvars::parse_url(const char* c_url, tu_string& host, tu_string& uri)
	{
		char* url = strdup(c_url);

		// remove the http:// if it exists

		int start = 0;
		if (strncasecmp(url, "http://", 7 ) == 0)
		{
			start = 7;
		}

		// get host name from url
		// find the first '/'
		int i, n;
		for (i = start, n = (int) strlen(url); url[i] != '/' && i < n; i++);
		if (i == n)
		{
			// '/' is not found
			fprintf(stderr, "invalid url '%s'\n", url);
			free(url);
			return false;
		}

		uri = url + i;
		url[i] = 0;
		host = url + start;
		free(url);
		url = NULL;
		return true;
	}

	void	as_loadvars::copy_to(as_object* target)
	{
		for (string_hash<tu_string>::iterator it = m_received_values.begin(); it != m_received_values.end(); ++it)
		{
			target->set_member(it->first, it->second.c_str()); 		
		}
	}
};

#endif
