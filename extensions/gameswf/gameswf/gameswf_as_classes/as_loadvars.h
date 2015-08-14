// as_loadvars.h	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifndef GAMESWF_AS_LOADVARS_H
#define GAMESWF_AS_LOADVARS_H

#include "../../base/tu_config.h"
#include "../gameswf_action.h"	// for as_object
#include "../gameswf_character.h"

#if TU_ENABLE_NETWORK == 1

#include "net/net_interface_tcp.h"

namespace gameswf
{

	void	as_global_loadvars_ctor(const fn_call& fn);

	enum loadvars_state
	{
		PARSE_REQUEST,
		PARSE_HEADER,
		PARSE_CONTENT,
		PARSE_END
	};

	struct as_loadvars : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_LOADVARS };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		struct request_data
		{
			request_data() :
				m_iface(NULL),
				m_ns(NULL),
				m_target(NULL)
			{
			}

			net_interface_tcp* m_iface;
			net_socket* m_ns;
			as_loadvars* m_target; // todo, should work with Xml object too. Think of a hierarchy here
			loadvars_state m_state;
			int m_http_status;
			tu_string m_rawdata;

		};

		string_hash<tu_string> m_headers;
		string_hash<tu_string> m_values;
		string_hash<tu_string> m_received_values;
		swf_array<request_data> m_requests;

		as_loadvars(player* player);

		bool	send_and_load(const char * url, as_object * target, const tu_string & method);
		bool	load(const char * url);
		void	add_header(const tu_string& name, const tu_string& value);
		tu_string	override_to_string();
		void	advance(float delta_time);
		void	decode(const tu_string& query_string);

		// copy variables into target
		void	copy_to(as_object* target);

		exported_module virtual bool	set_member(const tu_stringi& name, const as_value& val);
		exported_module virtual bool	get_member(const tu_stringi& name, as_value* val);

	private:

		tu_string create_request(const tu_string& method, const tu_string& uri, bool send_data);
		tu_string create_header();
		void parse_request(const tu_string& line, request_data& request);
		void parse_header(const tu_string& line, request_data& request);
		void parse_content(const tu_string& line, request_data& request);
		bool parse_url(const char* url, tu_string& host, tu_string& uri);
	};

}	// end namespace gameswf

#endif	// TU_ENABLE_NETWORK ==1

#endif // GAMESWF_AS_LOADVARS_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
