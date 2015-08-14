// container.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some non-inline implementation help for generic containers.


#include "container.h"
#include "utf8.h"
#include "tu_random.h"
#include <stdarg.h>


void tu_string::append_wide_char(uint16 c)
{
	char buf[8];
	int index = 0;
	utf8::encode_unicode_character(buf, &index, (uint32) c);
	buf[index] = 0;

	*this += buf;
}


void tu_string::append_wide_char(uint32 c)
{
	char buf[8];
	int index = 0;
	utf8::encode_unicode_character(buf, &index, c);
	buf[index] = 0;

	*this += buf;
}


void	tu_string::resize(int new_size)
{
	assert(new_size >= 0);

	if (new_size == size()) {
		return;
	}

	int old_size = size();

	if (using_heap() == false)
	{
		if (new_size <= sizeof(m_union.m_local.m_buffer))
		{
			// Stay with internal storage.
			m_union.m_local.m_bufsize_minus_length = (char) (sizeof(m_union.m_local.m_buffer) - new_size);
			m_union.m_local.m_buffer[new_size] = 0;	// terminate
		}
		else
		{
			// need to allocate heap buffer.
			int	capacity = new_size + 1;
			// round up.
			// TODO: test to see if this rounding-up is actually a performance win.
			capacity = (capacity + 15) & ~15;
			char*	buf = (char*) tu_malloc(capacity);
			memset(buf, 0, capacity);

			// Copy existing data.
			memcpy(buf, m_union.m_local.m_buffer, old_size);

			// Set the heap state.
			m_union.m_local.m_bufsize_minus_length = char(~0);
			m_union.m_heap.m_buffer = buf;
			m_union.m_heap.m_size = new_size;
			m_union.m_heap.m_capacity = capacity;
		}
	}
	else
	{
		// Currently using heap storage.
		if (new_size <= sizeof(m_union.m_local.m_buffer))
		{
			// Switch to local storage.

			// Be sure to get stack copies of m_heap info, before we overwrite it.
			char*	old_buffer = m_union.m_heap.m_buffer;
			int	old_capacity = m_union.m_heap.m_capacity;
			UNUSED(old_capacity);

			// Copy existing string info.
			m_union.m_local.m_bufsize_minus_length = (char) (sizeof(m_union.m_local.m_buffer) - new_size);
			assert(old_size >= new_size);
			memcpy(m_union.m_local.m_buffer, old_buffer, new_size);
			m_union.m_local.m_buffer[new_size] = 0;	// ensure termination.

			tu_free(old_buffer, old_capacity);
		}
		else
		{
			// Changing size of heap buffer.
			int	capacity = new_size + 1;
			// Round up.
			capacity = (capacity + 15) & ~15;
			if (capacity != m_union.m_heap.m_capacity)	// @@ TODO should use hysteresis when resizing
			{
				m_union.m_heap.m_buffer = (char*) tu_realloc(m_union.m_heap.m_buffer, capacity, m_heap.m_capacity);
				m_union.m_heap.m_capacity = capacity;
			}
			// else we're OK with existing buffer.

			m_union.m_heap.m_size = new_size;

			// Ensure termination.
			m_union.m_heap.m_buffer[new_size] = 0;

			assert(m_union.m_local.m_bufsize_minus_length == (char) ~0);
		}
	}
}


template<class char_type>
/*static*/ void	encode_utf8_from_wchar_generic(tu_string* result, const char_type* wstr)
{
	const char_type*	in = wstr;

	// First pass: compute the necessary string length.
	int	bytes_needed = 0;
	char	dummy[10];
	int	offset;
	for (;;)
	{
		Uint32	uc = *in++;
		offset = 0;
		utf8::encode_unicode_character(dummy, &offset, uc);
		bytes_needed += offset;

		assert(offset <= 6);

		if (uc == 0)
		{
			break;
		}
	}

	// Second pass: transfer the data.
	result->resize(bytes_needed - 1);	// resize() adds 1 for the \0 terminator
	in = wstr;
	char*	out = &((*result)[0]);
	offset = 0;
	for (;;)
	{
		assert(offset < bytes_needed);

		Uint32	uc = *in++;
		utf8::encode_unicode_character(out, &offset, uc);

		assert(offset <= bytes_needed);

		if (uc == 0)
		{
			break;
		}
	}

	assert(offset == bytes_needed);
	assert((*result)[offset - 1] == 0);
	assert(result->length() == (int) strlen(result->c_str()));
}


void tu_string::encode_utf8_from_wchar(tu_string* result, const uint32* wstr)
{
	encode_utf8_from_wchar_generic<uint32>(result, wstr);
}


void tu_string::encode_utf8_from_wchar(tu_string* result, const uint16* wstr)
{
	encode_utf8_from_wchar_generic<uint16>(result, wstr);
}


/*static*/ int	tu_string::stricmp(const char* a, const char* b)
{
#ifdef _WIN32
	return _stricmp(a, b);
#else
	return strcasecmp(a, b);
#endif
}


uint32	tu_string::utf8_char_at(int index) const
{
	const char*	buf = get_buffer();
	uint32	c;

	do
	{
		c = utf8::decode_next_unicode_character(&buf);
		index--;

		if (c == 0)
		{
			// We've hit the end of the string; don't go further.
			assert(index == 0);
			return c;
		}
	}
	while (index >= 0);

	return c;
}


tu_string	tu_string::utf8_to_upper() const
{
	const char*	buf = get_buffer();
	tu_string str;
	for (;;)
	{
		uint32 c = utf8::decode_next_unicode_character(&buf);
          
		if (c == 0)
		{
			// We've hit the end of the string; don't go further.
			return str;
		}
		str += toupper(c);
	}
  
	return str;
}


tu_string	tu_string::utf8_to_lower() const
{
	const char*	buf = get_buffer();
	tu_string str;
	for (;;)
	{
		uint32 c = utf8::decode_next_unicode_character(&buf);
    
		if (c == 0) {
			// We've hit the end of the string; don't go further.
			return str;
		}
		str += tolower(c);
	}
  
	return str;
}


/*static*/ int	tu_string::utf8_char_count(const char* buf, int buflen)
{
	const char*	p = buf;
	int	length = 0;

	while (p - buf < buflen)
	{
		uint32	c = utf8::decode_next_unicode_character(&p);
		if (c == 0)
		{
			break;
		}

		length++;
	}

	return length;
}


tu_string	tu_string::utf8_substring(int start, int end) const
{
	assert(start <= end);

	if (start == end)
	{
		// Special case, always return empty string.
		return tu_string();
	}

	const char*	p = get_buffer();
	int	index = 0;
	const char*	start_pointer = p;
	const char*	end_pointer = p;

	for (;;)
	{
		if (index == start)
		{
			start_pointer = p;
		}

		uint32	c = utf8::decode_next_unicode_character(&p);
		index++;

		if (index == end)
		{
			end_pointer = p;
			break;
		}

		if (c == 0)
		{
			if (index < end)
			{
				assert(0);
				end_pointer = p;
			}
			break;
		}
	}

	if (end_pointer < start_pointer)
	{
		end_pointer = start_pointer;
	}

	return tu_string(start_pointer, int(end_pointer - start_pointer));
}


#ifdef _WIN32
#define vsnprintf	_vsnprintf
#endif // _WIN32

tu_string string_printf(const char* fmt, ...)
// Handy sprintf wrapper.
{
	static const int	BUFFER_SIZE = 500;
	char	s_buffer[BUFFER_SIZE];

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(s_buffer, BUFFER_SIZE, fmt, ap);
	va_end(ap);

	return s_buffer;
}



#ifdef CONTAINER_UNIT_TEST


// Compile this test case with something like:
//
// gcc container.cpp utf8.cpp tu_random.cpp -g -I.. -DCONTAINER_UNIT_TEST -lstdc++ -o container_test
//
//    or
//
// cl container.cpp utf8.cpp tu_random.cpp dlmalloc.c -Zi -Od -DCONTAINER_UNIT_TEST -DUSE_DL_MALLOC -DWIN32 -I..


void	test_hash()
{
	// Collect a bunch of random key/value pairs.
	swf_array<Uint32>	data;
	for (int i = 0; i < 1000; i++)
	{
		data.push_back(tu_random::next_random());
	}

	// Push into hash.
	hash<Uint32, Uint32>	h;
	{for (int i = 0; i < data.size() / 2; i++)
	{
		h.add(data[i*2], data[i*2 + 1]);

		// Verify the contents of the hash so far.
		for (int j = 0; j < i; j++)
		{
			Uint32	key = data[j*2];
			Uint32	val;
			bool	got = h.get(key, &val);
			assert(got);
			assert(val == data[j*2 + 1]);
		}
	}}

	// Manually copy stuff over to h2, using iterator interface.
	hash<Uint32, Uint32>	h2;
	{for (hash<Uint32, Uint32>::iterator it = h.begin(); it != h.end(); ++it)
	{
		//printf("first = 0x%X, second = 0x%X\n", it->first, it->second);//xxxxx
		assert(h.get(it->first, NULL) == true);

		h2.add(it->first, it->second);

		Uint32	val;
		bool	got = h2.get(it->first, &val);
		assert(got);
		assert(val == it->second);
	}}

	// Verify the contents of h2.
	{for (int i = 0; i < data.size() / 2; i++)
	{
		Uint32	key = data[i*2];
		Uint32	val;
		bool	got = h2.get(key, &val);
		assert(got);
		assert(val == data[i*2 + 1]);
	}}

	h.clear();
	assert(h.size() == 0);

	// Verify that h really is missing the stuff it had before, and h2 really has it.
	{for (hash<Uint32, Uint32>::iterator it = h2.begin(); it != h2.end(); ++it)
	{
		assert(h.get(it->first, NULL) == false);
		assert(h2.get(it->first, NULL) == true);
		assert(h.find(it->first) == h.end());
		assert(h2.find(it->first) != h2.end());
	}}

	// Copy h2 back into h.
	h = h2;

	assert(h.size() == h2.size());
	
	// Verify h2 contents are in h.
	{for (hash<Uint32, Uint32>::iterator it = h2.begin(); it != h2.end(); ++it)
	{
		assert(h.get(it->first, NULL) == true);
		assert(h2.get(it->first, NULL) == true);
		assert(h.find(it->first) != h.end());
		assert(h2.find(it->first) != h2.end());
	}}

	// Verify h contents are in h2.
	{for (hash<Uint32, Uint32>::iterator it = h.begin(); it != h.end(); ++it)
	{
		assert(h.get(it->first, NULL) == true);
		assert(h2.get(it->first, NULL) == true);
		assert(h.find(it->first) != h.end());
		assert(h2.find(it->first) != h2.end());
	}}

	// Test element removal.
	int h2_size = h2.size();
	{for (hash<Uint32, Uint32>::iterator it = h2.begin(); it != h2.end(); ) {
		hash<Uint32, Uint32>::iterator next = it;
		++next;
		h2_size--;
		Uint32 key = it->first;
		Uint32 value = it->second;
		h2.erase(it);
		assert(h2.size() == h2_size);
		assert(h2.find(key) == h2.end());
		it = next;
	}}
}


//#include <ext/hash_map>
//#include <hash_map>
//#include <map>

void	test_hash_speed()
// Test function for hash performance of adding keys and doing lookup.
{

// Hash type, for doing comparative tests.
//
// tu_hash tests faster than the map and hash_map included with GCC
// 3.3 as well as map and hash_map from MSVC7.  In some cases, several
// times faster.  GCC's hash_map is the closest to tu_hash, but is
// still 33% slower in my tests.

// // tu's hash
#define HASH gameswf::hash<uint32, uint32 >
#define HASH_ADD(h, k, v) h.add(k, v)

// STL's hash
//#define HASH __gnu_cxx::hash_map<uint32, uint32>
//#define HASH std::hash_map<uint32, uint32>
//#define HASH_ADD(h, k, v) h[k] = v

// STL's map
//#define HASH std::map<uint32, uint32>
//#define HASH_ADD(h, k, v) h[k] = v

//	const int	SIZE = 10000000;
	const int	SIZE = 1000000;

	// Make an array of random numbers.
	swf_array<uint32>	numbers;
	numbers.resize(SIZE);

	for (int i = 0, n = numbers.size(); i < n; i++)
	{
		numbers[i] = tu_random::next_random();
	}

	// Uniquify the array.
	HASH	new_index;
	int	next_new_index = 0;
	{for (int i = 0, n = numbers.size(); i < n; i++)
	{
		HASH::iterator	it = new_index.find(numbers[i]);
		if (it == new_index.end())
		{
			// First time this number has been seen.
			HASH_ADD(new_index, numbers[i], next_new_index);
			next_new_index++;
		}
		else
		{
			// This number already appears in the list.
// 			printf("duplicate entry %x, prev new index %d, current array index %d\n",
// 				   numbers[i],
// 				   it->second,
// 				   i);
		}
	}}

	printf("next_new_index = %d\n", next_new_index);
}


void	test_stringi()
{
	tu_stringi	a, b;

	// Equality.
	a = "this is a test";
	b = "This is a test";
	assert(a == b);

	b = "tHiS Is a tEsT";
	assert(a == b);

	a += "Hello";
	b += "hellO";
	assert(a == b);

	tu_string	c(b);
	assert(a.to_tu_string() != c);

	// Ordering.
	a = "a";
	b = "B";
	assert(a < b);

	a = "b";
	b = "A";
	assert(a > b);
}


void	test_stringi_hash()
{
	stringi_hash<int>	a;

	assert(a.is_empty());

	a.add("bobo", 1);

	assert(a.is_empty() == false);

	a.add("hello", 2);
	a.add("it's", 3);
	a.add("a", 4);
	a.add("beautiful day!", 5);

	int	result = 0;
	a.get("boBO", &result);
	assert(result == 1);

	a.set("BObo", 2);
	a.get("bObO", &result);
	assert(result == 2);

	assert(a.is_empty() == false);
	a.clear();
	assert(a.is_empty() == true);

	// Hammer on one key that differs only by case.
	tu_stringi	original_key("thisisatest");
	tu_stringi	key(original_key);
	a.add(key, 1234567);

	int	variations = 1 << key.length();
	for (int i = 0; i < variations; i++)
	{
		// Twiddle the case of the key.
		for (int c = 0; c < key.length(); c++)
		{
			if (i & (1 << c))
			{
				key[c] = toupper(key[c]);
			}
			else
			{
				key[c] = tolower(key[c]);
			}
		}

		a.set(key, 7654321);

		// Make sure original entry was modified.
		int	value = 0;
		a.get(original_key, &value);
		assert(value == 7654321);

		// Make sure hash keys are preserving case.
		assert(a.find(key)->first.to_tu_string() == original_key.to_tu_string());

		// Make sure they're actually the same entry.
		assert(a.find(original_key) == a.find(key));
		
		a.set(original_key, 1234567);
		assert(a.find(key)->second == 1234567);
	}
}


void test_unicode()
{
	tu_string a;

	tu_string::encode_utf8_from_wchar(&a, L"19 character string");
	assert(a.length() == 19);

	// TODO add some more tests; should test actual UTF-8 conversions.
}


int	main()
{
#if 1
	printf("sizeof(tu_string) == %d\n", sizeof(tu_string));

	swf_array<tu_string>	storage;
	storage.resize(2);

	tu_string&	a = storage[0];
	tu_string&	b = storage[1];
	a = "test1";
	
	printf("&a = 0x%X, &b = 0x%X\n", int(&a), int(&b));

	printf("%s\n", a.c_str());

	assert(a == "test1");
	assert(a.length() == 5);

	a += "2";
	assert(a == "test12");

	a += "this is some more text";
	assert(a.length() == 28);

	assert(a[2] == 's');
	assert(a[3] == 't');
	assert(a[4] == '1');
	assert(a[5] == '2');
	assert(a[7] == 'h');
	assert(a[28] == 0);

	assert(b.length() == 0);
	assert(b[0] == 0);
	assert(b.c_str()[0] == 0);

	tu_string c = a + b;

	assert(c.length() == a.length());

	c.resize(2);
	assert(c == "te");
	assert(c == tu_string("te"));

	assert(tu_string("fourscore and sevent") == "fourscore and sevent");

	b = "#sacrificial lamb";

	// Test growing & shrinking.
	a = "";
	for (int i = 0; i < 1000; i++)
	{
		assert(a.length() == i);

		if (i == 8)
		{
			assert(a == "01234567");
		}
		else if (i == 27)
		{
			assert(a == "012345678901234567890123456");
		}

		a.resize(a.length() + 1);
		a[a.length() - 1] = '0' + (i % 10);
	}

	{for (int i = 999; i >= 0; i--)
	{
		a.resize(a.length() - 1);
		assert(a.length() == i);

		if (i == 8)
		{
			assert(a == "01234567");
		}
		else if (i == 27)
		{
			assert(a == "012345678901234567890123456");
		}
	}}

	// Test larger shrinking across heap/local boundary.
	a = "this is a string longer than 16 characters";
	a = "short";

	// Test larger expand across heap/local boundary.
	a = "another longer string...";

	assert(b == "#sacrificial lamb");

	// Test erase()
	a = "abcdef";
	a.erase(3, 1);
	assert(a == "abcef");
	a = "abcdefghijklmnopqrst";
	a.erase(12, 6);
	assert(a == "abcdefghijklst");

	// Test insert()
	a = "abcdef";
	a.insert(3, 'z');
	assert(a == "abczdef");
	a = "abcdefghijklmn";
	a.insert(12, 'y');
	a.insert(13, 'z');
	assert(a == "abcdefghijklyzmn");

	// Test operator+=(tu_string)
	a = "abcdef";
	b = "ghijklmnopqrst";
	a += b;
	assert(a == "abcdefghijklmnopqrst");

	// Test operator+(const char*)
	a = "abcdef";
	b = "ghi";
	assert(a + b == "abcdefghi");

	// Test operator<(const char*)
	a = "abc";
	assert(a < "def");
	assert(a < "abd");
	assert(!(a < "aab"));
	assert(!(a < "aa"));
	assert(!(a < "aabz"));
	assert(a < "abcd");

	// Test operator<(const tu_string&)
	a = "abc";
	assert(a < tu_string("def"));
	assert(a < tu_string("abd"));
	assert(!(a < tu_string("aab")));
	assert(!(a < tu_string("aa")));
	assert(!(a < tu_string("aabz")));
	assert(a < tu_string("abcd"));

	// Test that we can store strings with embedded \0's.
	a = "abc";
	a += '\0';
	assert(a.length() == 4);
	a += "def";
	assert(a.length() == 7);
	assert(a[4] == 'd');
	assert(a[5] == 'e');
	assert(a[6] == 'f');
	assert(a[7] == 0);

	// Test that we can upsize strings with embedded \0's
	a = "abc";
	a += '\0';
	a += "def";
	b = a;
	assert(a == b);
	assert(b.length() == 7);
	a = a + b;
	assert(a.length() == 14);
	assert(a == (b + b));
	assert(a > b);
	a = a + b;
	assert(a.size() == 21);
	assert(a == (b + b + b));
	assert(a > b);

	// Test that we can downsize strings with embedded \0's
	a = "abc";
	a += '\0';
	a += "def";
	assert(a.length() == 7);
	b = a + a + a;
	assert(b.length() == 21);
	b.resize(14);
	assert(b.length() == 14);
	assert(b == (a + a));
	b.erase(3, 1);
	assert(b.size() == 13);
	assert(b == tu_string("abcdef") + a);

	// Test strings that are nothing but \0's.
	a = "";
	b = "";
	for (int i = 0; i < 100; i++) {
		a += '\0';
		assert(a.length() == i + 1);
		assert(a > b);
		b += '\0';
		assert(b.length() == i + 1);
		assert(a == b);
		assert((a + b).length() == 2 * (i + 1));
	}

	test_hash();
	test_stringi();
	test_stringi_hash();

	test_unicode();

	// TODO: unit tests for swf_array<>, string_hash<>
#endif

	test_hash_speed();

	printf("OK\n");

	return 0;
}


#endif // CONTAINER_UNIT_TEST


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
